/*!
 * \file config.cpp
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

#include "config.h"

Sic readSic(const QString &key)
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
    //settings.beginGroup(QLatin1String("General"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QString timeStr = settings.value(key).toString();
    QDate date = QDate::fromString(timeStr, Qt::ISODate);

    return date;
}

bool Configuration::useXmlTimestamp()
{
    QString key = QLatin1String("useXmlTimestamp");

    QSettings settings;
    //settings.beginGroup(QLatin1String("Global"));

    if (!settings.contains(key))
    {
        return false;
    }

    bool b = settings.value(key).toBool();
    return b;
}

Sic Configuration::smrSic()
{
    QString key = QLatin1String("SMR.SIC");
    return readSic(key);
}

Sic Configuration::mlatSic()
{
    QString key = QLatin1String("MLAT.SIC");
    return readSic(key);
}

Sic Configuration::adsbSic()
{
    QString key = QLatin1String("ADSB.SIC");
    return readSic(key);
}

QString Configuration::kmlFile()
{
    QString key = QLatin1String("filepath");

    QSettings settings;
    settings.beginGroup(QLatin1String("KML"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QString pathStr = settings.value(key).toString();

    return pathStr;
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
        qWarning() << "Invalid Update Rate, using default value: "
                   << MOPS::ED116::defaultTgtRepUpdateRateHz;

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
        qWarning() << "Invalid Update Rate, using default value: "
                   << MOPS::ED116::defaultSrvMsgUpdateRateHz;

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
        qWarning() << "Invalid Update Rate, using default value: "
                   << MOPS::ED117::defaultTgtRepUpdateRateHz;

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
        qWarning() << "Invalid Update Rate, using default value: "
                   << MOPS::ED117::defaultSrvMsgUpdateRateHz;

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
        qWarning() << "Invalid Silence Period, using default value: "
                   << MOPS::defaultSilencePeriodSeconds;

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
            qWarning() << "Invalid value for" << key
                       << "using default value: " << period;
            var = period;
        }

        periods.insert(area, var);
        return var;
    };

    switch (area)
    {
    case Aerodrome::Area::Runway:
        return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodRunway"),
            MOPS::ED117::defaultProbDetectionPeriodRunway);
    case Aerodrome::Area::Apron:
        return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodApron"),
            MOPS::ED117::defaultProbDetectionPeriodApron);
    default:
        break;
    }

    return readConfig(QStringLiteral("MOPS.ProbDetectionPeriodOther"),
        MOPS::ED117::defaultProbDetectionPeriodOther);
}

qint32 Configuration::dgpsTodOffset()
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
        qWarning() << "Invalid Time of Day Offset, using default value: "
                   << MOPS::defaultDgpsTimeOfDayOffset;

        return MOPS::defaultDgpsTimeOfDayOffset;
    }

    return var;
}

Mode3A Configuration::dgpsMode3A()
{
    QString key = QLatin1String("Mode3A");

    QSettings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    quint16 val = settings.value(key).toUInt();

    if (val > 07777)
    {
        qFatal("Invalid %s value.", qPrintable(key));
    }

    return val;
}

ModeS Configuration::dgpsModeS()
{
    QString key = QLatin1String("ModeS");

    QSettings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    bool ok;
    quint32 val = settings.value(key).toUInt(&ok);

    if (!ok || val > 0xFFFFFF)
    {
        qFatal("Invalid %s value.", qPrintable(key));
    }

    return val;
}

Ident Configuration::dgpsIdent()
{
    QString key = QLatin1String("Ident");

    QSettings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QString val = settings.value(key).toString();

    if (val.size() > 8)  // Maximum 8 characters.
    {
        qFatal("Invalid %s value.", qPrintable(key));
    }

    return val;
}

QString Configuration::logRules()
{
    QString key = QLatin1String("rules");

    QSettings settings;
    settings.beginGroup(QLatin1String("LOG"));

    if (!settings.contains(key))
    {
        return QString();
    }

    QString rules = settings.value(key).toString();

    return rules;
}

QString Configuration::logPattern()
{
    QString key = QLatin1String("pattern");

    QSettings settings;
    settings.beginGroup(QLatin1String("LOG"));

    if (!settings.contains(key))
    {
        return QString();
    }

    QString pattern = settings.value(key).toString();

    return pattern;
}
