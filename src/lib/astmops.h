/*!
 * \file astmops.h
 * \brief Header file with essential definitions to be used in the scope
 * of the ASTMOPS program.
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

#ifndef ASTMOPS_ASTMOPS_H
#define ASTMOPS_ASTMOPS_H

#include "aerodrome.h"
#include <QDate>
#include <QDebug>
#include <QGeoCircle>
#include <QGeoPolygon>
#include <QGeoShape>
#include <QSettings>
#include <QtGlobal>
#include <QtMath>

using TrackNum = quint32;
using IcaoAddr = quint32;
using Utn = quint32;

enum SystemType
{
    Smr,
    Mlat,
    Adsb,
    Dgps,
    Unknown
};

enum Layer
{
    GroundLayer,
    AirborneLayer,
    UnknownLayer
};

const double ed116TgtRepUpdateRateHz = 1.0;  // Hz
const double ed116SrvMsgUpdateRateHz = 1.0;  // Hz

const double ed117TgtRepUpdateRateHz = 1.0;  // Hz
const double ed117SrvMsgUpdateRateHz = 1.0;  // Hz

const double silencePeriodSeconds = 60.0;  // s

const double ed117ProbDetectionPeriodRunway = 1.0;
const double ed117ProbDetectionPeriodApron = 5.0;
const double ed117ProbDetectionPeriodOther = 2.0;

const int defaultDgpsTimeOfDayOffset = 0;  // s


static quint8 readSic(const QString &key)
{
    QSettings settings;

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    quint32 val = settings.value(key).toUInt();

    if (val > 255)
    {
        qFatal("Invalid %s value.", qPrintable(key));
    }

    return val;
}

namespace Configuration
{
inline QDate asterixDate()
{
    QString key = QLatin1String("Date");

    QSettings settings;
    //settings.beginGroup(QLatin1String("Global"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QString timeStr = settings.value(key).toString();
    QDate date = QDate::fromString(timeStr, Qt::ISODate);

    return date;
}

inline quint8 smrSic()
{
    QString key = QLatin1String("SMR.SIC");
    return readSic(key);
}

inline quint8 mlatSic()
{
    QString key = QLatin1String("MLAT.SIC");
    return readSic(key);
}

inline quint8 adsbSic()
{
    QString key = QLatin1String("ADSB.SIC");
    return readSic(key);
}

inline double ed116TgtRepUpdateRate()
{
    QString key = QLatin1String("TgtRepUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED116"));

    if (!settings.contains(key))
    {
        return ed116TgtRepUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed116TgtRepUpdateRateHz;
        return ed116TgtRepUpdateRateHz;
    }

    return var;
}

inline double ed116SrvMsgUpdateRate()
{
    QString key = QLatin1String("SrvMsgUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED116"));

    if (!settings.contains(key))
    {
        return ed116SrvMsgUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed116SrvMsgUpdateRateHz;
        return ed116SrvMsgUpdateRateHz;
    }

    return var;
}

inline double ed117TgtRepUpdateRate()
{
    QString key = QLatin1String("TgtRepUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED117"));

    if (!settings.contains(key))
    {
        return ed117TgtRepUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed117TgtRepUpdateRateHz;
        return ed117TgtRepUpdateRateHz;
    }

    return var;
}

inline double ed117SrvMsgUpdateRate()
{
    QString key = QLatin1String("SrvMsgUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED117"));

    if (!settings.contains(key))
    {
        return ed117SrvMsgUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed117SrvMsgUpdateRateHz;
        return ed117SrvMsgUpdateRateHz;
    }

    return var;
}

inline double silencePeriod()
{
    QString key = QLatin1String("TimeOutPeriod");

    QSettings settings;

    if (!settings.contains(key))
    {
        return silencePeriodSeconds;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Silence Period, using default value: " << silencePeriodSeconds;
        return silencePeriodSeconds;
    }

    return var;
}

inline double probDetectionPeriod(Aerodrome::Area area)
{
    static QHash<Aerodrome::Area, double> periods;

    if (periods.contains(area))
    {
        return periods.value(area);
    }

    auto readConfig = [area](const QString &key, double period) {
        QSettings settings;
        double var = settings.value(key, period).toDouble();

        if (var <= 0)
        {
            qWarning() << "Invalid value for" << key << "using default value: " << period;
            var = period;
        }

        periods.insert(area, var);
        return var;
    };

    switch (area)
    {
    case Aerodrome::Area::Runway:
        return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodRunway"), ed117ProbDetectionPeriodRunway);
    case Aerodrome::Area::Apron:
        return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodApron"), ed117ProbDetectionPeriodApron);
    default:
        break;
    }

    return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodOther"), ed117ProbDetectionPeriodOther);
}

inline qint32 dgpsTimeOfDayOffset()
{
    QString key = QLatin1String("TimeOfDayOffset");

    QSettings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        return defaultDgpsTimeOfDayOffset;
    }

    qint32 var = settings.value(key).toInt();

    if (qFabs(var) > 86400)  // Maximum TOD.
    {
        qWarning() << "Invalid Time of Day Offset, using default value: " << defaultDgpsTimeOfDayOffset;
        return defaultDgpsTimeOfDayOffset;
    }

    return var;
}

//inline quint16 dgpsMode3ACode()
//{
//    QString key = QLatin1String("Mode3ACode");

//    QSettings settings;
//    settings.beginGroup(QLatin1String("DGPS"));

//    if (!settings.contains(key))
//    {
//        qFatal("%s is mandatory.", qPrintable(key));
//    }

//    quint16 val = settings.value(key).toUInt();

//    if (val > 07777)
//    {
//        qFatal("Invalid %s value.", qPrintable(key));
//    }

//    return val;
//}

inline IcaoAddr dgpsTargetAddress()
{
    QString key = QLatin1String("TargetAddress");

    QSettings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    quint32 val = settings.value(key).toUInt();

    if (val > 0xFFFFFF)
    {
        qFatal("Invalid %s value.", qPrintable(key));
    }

    return val;
}

//inline QString dgpsIdentification()
//{
//    QString key = QLatin1String("Identification");

//    QSettings settings;
//    settings.beginGroup(QLatin1String("DGPS"));

//    if (!settings.contains(key))
//    {
//        qFatal("%s is mandatory.", qPrintable(key));
//    }

//    QString val = settings.value(key).toString();

//    if (val.size() > 8)  // Maximum 8 characters.
//    {
//        qFatal("Invalid %s value.", qPrintable(key));
//    }

//    return val;
//}

inline QGeoShape evalSector()
{
    // TODO: Allow defining more than one sector.

    QString keyPolygon = QLatin1String("Polygon");
    QString keyCenter = QLatin1String("Center");
    QString keyRadius = QLatin1String("Radius");

    QSettings settings;
    settings.beginGroup(QLatin1String("EvalSector"));

    int polygonSize = settings.beginReadArray(keyPolygon);

    if (!polygonSize)
    {
        if (!settings.contains(keyCenter) && !settings.contains(keyRadius))
        {
            // No keys found. Return empty QGeoShape.
            return QGeoShape();
        }

        // Circular sector case.
        QGeoCoordinate center = settings.value(keyCenter).value<QGeoCoordinate>();
        double radius = settings.value(keyRadius).toDouble();

        return QGeoCircle(center, radius);
    }

    // Polygon sector case.
    QList<QGeoCoordinate> list;
    list.reserve(polygonSize);

    for (int i = 0; i < polygonSize; ++i)
    {
        settings.setArrayIndex(i);
        QStringList strList = settings.value(QLatin1String("Coordinates")).toStringList();
        QGeoCoordinate coord = QGeoCoordinate(strList.at(0).toDouble(), strList.at(1).toDouble(), 0);
        list.append(coord);
    }

    return QGeoPolygon(list);
}
};  // namespace Configuration

#endif  // ASTMOPS_ASTMOPS_H
