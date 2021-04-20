/*!
 * \file astmops.cpp
 * \brief Implementation of essential definitions to be used in the scope
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

#include "astmops.h"

RecordType::RecordType() {}

RecordType::RecordType(SystemType st, MessageType mt) : sys_typ_(st), msg_typ_(mt){};

bool RecordType::isUnknown() const
{
    return sys_typ_ == SystemType::Unknown || msg_typ_ == MessageType::Unknown;
}

bool operator==(RecordType a, RecordType b)
{
    return a.sys_typ_ == b.sys_typ_ &&
           a.msg_typ_ == b.msg_typ_;
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(SystemType st, uint seed)
#else
size_t qHash(SystemType st, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, static_cast<int>(st));
    return seed;
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(MessageType mt, uint seed)
#else
size_t qHash(MessageType mt, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, static_cast<int>(mt));
    return seed;
}


#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(RecordType rt, uint seed)
#else
size_t qHash(RecordType rt, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, static_cast<int>(rt.sys_typ_));
    seed = hash(seed, static_cast<int>(rt.msg_typ_));
    return seed;
}

bool isCategorySupported(const quint8 cat)
{
    /* SUPPORTED ASTERIX CATEGORIES:
     * CAT010: Monosensor Surface Movement Data
     * CAT021: ADS-B Messages
     */
    if (cat == 10 || cat == 21)
    {
        return true;
    }

    return false;
}

quint8 readSic(const QString &key)
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

QDate Configuration::asterixDate()
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

quint8 Configuration::smrSic()
{
    QString key = QLatin1String("SMR.SIC");
    return readSic(key);
}

quint8 Configuration::mlatSic()
{
    QString key = QLatin1String("MLAT.SIC");
    return readSic(key);
}

quint8 Configuration::adsbSic()
{
    QString key = QLatin1String("ADSB.SIC");
    return readSic(key);
}

double Configuration::ed116TgtRepUpdateRate()
{
    QString key = QLatin1String("TgtRepUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED116"));

    if (!settings.contains(key))
    {
        return MOPS::ED116::defaultTgtRepUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << MOPS::ED116::defaultTgtRepUpdateRateHz;
        return MOPS::ED116::defaultTgtRepUpdateRateHz;
    }

    return var;
}

double Configuration::ed116SrvMsgUpdateRate()
{
    QString key = QLatin1String("SrvMsgUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED116"));

    if (!settings.contains(key))
    {
        return MOPS::ED116::defaultSrvMsgUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << MOPS::ED116::defaultSrvMsgUpdateRateHz;
        return MOPS::ED116::defaultSrvMsgUpdateRateHz;
    }

    return var;
}

double Configuration::ed117TgtRepUpdateRate()
{
    QString key = QLatin1String("TgtRepUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED117"));

    if (!settings.contains(key))
    {
        return MOPS::ED117::defaultTgtRepUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << MOPS::ED117::defaultTgtRepUpdateRateHz;
        return MOPS::ED117::defaultTgtRepUpdateRateHz;
    }

    return var;
}

double Configuration::ed117SrvMsgUpdateRate()
{
    QString key = QLatin1String("SrvMsgUpdateRate");

    QSettings settings;
    settings.beginGroup(QLatin1String("MOPS/ED117"));

    if (!settings.contains(key))
    {
        return MOPS::ED117::defaultSrvMsgUpdateRateHz;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << MOPS::ED117::defaultSrvMsgUpdateRateHz;
        return MOPS::ED117::defaultSrvMsgUpdateRateHz;
    }

    return var;
}

double Configuration::silencePeriod()
{
    QString key = QLatin1String("TimeOutPeriod");

    QSettings settings;

    if (!settings.contains(key))
    {
        return MOPS::defaultSilencePeriodSeconds;
    }

    double var = settings.value(key).toDouble();

    if (var <= 0)
    {
        qWarning() << "Invalid Silence Period, using default value: " << MOPS::defaultSilencePeriodSeconds;
        return MOPS::defaultSilencePeriodSeconds;
    }

    return var;
}

double Configuration::probDetectionPeriod(Aerodrome::Area area)
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
        return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodRunway"), MOPS::ED117::defaultProbDetectionPeriodRunway);
    case Aerodrome::Area::Apron:
        return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodApron"), MOPS::ED117::defaultProbDetectionPeriodApron);
    default:
        break;
    }

    return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodOther"), MOPS::ED117::defaultProbDetectionPeriodOther);
}

qint32 Configuration::dgpsTimeOfDayOffset()
{
    QString key = QLatin1String("TimeOfDayOffset");

    QSettings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        return MOPS::defaultDgpsTimeOfDayOffset;
    }

    qint32 var = settings.value(key).toInt();

    if (qFabs(var) > 86400)  // Maximum TOD.
    {
        qWarning() << "Invalid Time of Day Offset, using default value: " << MOPS::defaultDgpsTimeOfDayOffset;
        return MOPS::defaultDgpsTimeOfDayOffset;
    }

    return var;
}

//Mode3A Configuration::dgpsMode3ACode()
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

ModeS Configuration::dgpsTargetAddress()
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

//Ident Configuration::dgpsIdentification()
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

QGeoShape Configuration::evalSector()
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
