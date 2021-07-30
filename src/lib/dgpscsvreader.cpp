/*!
 * \file dgpscsvreader.cpp
 * \brief
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * -----------------------------------------------------------------------
 *
 * Copyright (C) 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
 *
 * ASTMOPS is a surveillance performance analyzer
 *      for Air Traffic Control
 *
 * This file is part of ASTMOPS.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------
 */

#include "dgpscsvreader.h"
#include "config.h"
#include "geofunctions.h"
#include <QDebug>

/*!
 * \brief readDgpsCsv
 * \param file
 * \param error
 * \return
 */
QVector<QGeoPositionInfo> readDgpsCsv(QIODevice *file, ErrorType *error)
{
    if (error)
    {
        *error = NoError;
    }

    QVector<QGeoPositionInfo> result;

    enum Timestamp
    {
        Unix,
        Iso
    };

    enum Position
    {
        Degrees,
        Dms
    };

    enum Altitude
    {
        Feet,
        Meters
    };

    // Default units.
    Timestamp timeUnit = Unix;
    Position positionUnit = Degrees;
    Altitude altitudeUnit = Feet;

    // Default column values.
    int timeColumn = -1;
    int latitudeColumn = -1;
    int longitudeColumn = -1;
    int altitudeColumn = -1;

    const QByteArray first = file->readLine();

    // Split line at ";" delimiter.
    const auto headerParts = first.split(';');

    // Header must contain 4 elements.
    if (headerParts.size() != 4)
    {
        if (error)
        {
            *error = NotWellFormedHeaderError;
        }
        return result;
    }

    for (int index = 0, last = headerParts.size(); index < last; ++index)
    {
        const QByteArray fieldSpec = headerParts.at(index);
        const QByteArrayList fieldSpecParts = fieldSpec.split('_');

        // Split "FieldName_format" into "FieldName" + "format" parts.
        if (fieldSpecParts.size() != 2)
        {
            if (error)
            {
                *error = NotWellFormedHeaderError;
            }
            return result;
        }

        const QByteArray fieldName = fieldSpecParts.at(0).trimmed();

        if (qstricmp(fieldName.constData(), "datetime") == 0)
        {
            timeColumn = index;
        }
        else if (qstricmp(fieldName.constData(), "latitude") == 0)
        {
            latitudeColumn = index;
        }
        else if (qstricmp(fieldName.constData(), "longitude") == 0)
        {
            longitudeColumn = index;
        }
        else if (qstricmp(fieldName.constData(), "gpsaltitude") == 0)
        {
            altitudeColumn = index;
        }
    }

    // Stop if any column is missing.
    if (timeColumn == -1 || latitudeColumn == -1 || longitudeColumn == -1 || altitudeColumn == -1)
    {
        if (error)
        {
            *error = NotWellFormedHeaderError;
        }
        return result;
    }

    const QByteArray timeFormat =
        headerParts.at(timeColumn).trimmed().split('_').at(1);
    if (qstricmp(timeFormat.constData(), "unix") == 0)
    {
        timeUnit = Unix;
    }
    else if (qstricmp(timeFormat.constData(), "iso8601") == 0)
    {
        timeUnit = Iso;
    }

    const QByteArray positionFormat =
        headerParts.at(latitudeColumn).trimmed().split('_').at(1);
    if (qstricmp(positionFormat.constData(), "deg") == 0)
    {
        positionUnit = Degrees;
    }
    else if (qstricmp(positionFormat.constData(), "dms") == 0)
    {
        positionUnit = Dms;
    }

    const QByteArray altitudeFormat =
        headerParts.at(altitudeColumn).trimmed().split('_').at(1);
    if (qstricmp(altitudeFormat.constData(), "ft") == 0)
    {
        altitudeUnit = Feet;
    }
    else if (qstricmp(altitudeFormat.constData(), "m") == 0)
    {
        altitudeUnit = Meters;
    }

    while (!file->atEnd())
    {
        const QByteArray line = file->readLine().trimmed();

        // Skip empty lines or comments.
        if (line.isEmpty() || line.startsWith('#'))
        {
            continue;
        }

        // Split line at ";" delimiter.
        const QByteArrayList lineParts = line.split(';');

        // Data must contain 4 elements.
        if (lineParts.size() != 4)
        {
            qWarning() << "Ignoring line:" << line;
            continue;
        }

        const QByteArray timeText = lineParts.at(timeColumn).trimmed();
        const QByteArray latitudeText = lineParts.at(latitudeColumn).trimmed();
        const QByteArray longitudeText = lineParts.at(longitudeColumn).trimmed();
        const QByteArray altitudeText = lineParts.at(altitudeColumn).trimmed();

        QString timeStr = QString::fromLatin1(timeText);

        // Remove decimal point from Unix timestamp.
        if (timeUnit == Unix)
        {
            timeStr.remove(QLatin1Char('.'));
        }

        const QDateTime timeValue =
            timeUnit == Unix ? QDateTime::fromMSecsSinceEpoch(timeStr.toULong(), Qt::UTC)
                             : QDateTime::fromString(timeStr, Qt::ISODateWithMs);

        const double latitude =
            positionUnit == Degrees ? latitudeText.toDouble() : qQNaN();
        const double longitude =
            positionUnit == Degrees ? longitudeText.toDouble() : qQNaN();

        const double altitudeValue =
            altitudeText.toDouble() * (altitudeUnit == Meters ? 1 : 0.3048);

        const QGeoCoordinate position =
            QGeoCoordinate(latitude, longitude, altitudeValue);

        if (!timeValue.isValid() || !position.isValid())
        {
            qWarning() << "Ignoring invalid reference data:" << line;
        }

        result.append(QGeoPositionInfo(position, timeValue));
    }

    return result;
}

QMultiMap<QDateTime, TargetReport> makeDgpsTgtRep(const QVector<QGeoPositionInfo> &posInfo)
{
    QMultiMap<QDateTime, TargetReport> mmap;

    // Read from configuration.
    ModeS mode_s = Configuration::dgpsModeS();

    // TODO: HARDOCODED VALUE! READ ARP COORDINATES FROM CONFIGURATION INSTEAD!
    QGeoCoordinate leblArpGeo(41.297076579982225, 2.0784629201158662, 4.3200000000000003);

    // Read data.
    for (const QGeoPositionInfo &pi : posInfo)
    {
        QVector3D cart = geoToLocalEnu(pi.coordinate(), leblArpGeo);

        TargetReport tr;
        tr.sys_typ_ = SystemType::Dgps;
        tr.tod_ = pi.timestamp();
        tr.mode_s_ = mode_s;
        tr.x_ = cart.x();
        tr.y_ = cart.y();
        tr.z_ = cart.z();

        mmap.insert(pi.timestamp(), tr);
    }

    return mmap;
}

QMultiMap<QDateTime, TargetReport> dgpsTgtRep(QIODevice *file, ErrorType *error)
{
    QMultiMap<QDateTime, TargetReport> mmap;

    QVector<QGeoPositionInfo> geo = readDgpsCsv(file, error);
    if (*error != ErrorType::NoError)
    {
        // Return empty multimap.
        return mmap;
    }

    mmap = makeDgpsTgtRep(geo);
    return mmap;
}
