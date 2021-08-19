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
#include <QCoreApplication>
#include <QStandardPaths>

Settings::Settings() : QSettings(configFilePath(), QSettings::IniFormat)
{
}

QString Settings::configFilePath()
{
    static QString path;
    if (!path.isEmpty())
    {
        return path;
    }

    const QStringList args = QCoreApplication::arguments();
    const int idx = args.indexOf(QLatin1String("--config"));

    if (idx != -1 && args.size() > idx + 1)
    {
        // Use user-defined config location.
        path = args.at(idx + 1);
    }
    else
    {
        // Use default config location.
        path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
        path.append(QLatin1String("/"));
        path.append(QCoreApplication::organizationName());
        path.append(QLatin1String("/"));
        path.append(QCoreApplication::applicationName());
        path.append(QLatin1String(".conf"));
    }

    return path;
}

Sic Configuration::readSic(const QString &key)
{
    Settings settings;

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    //QStringList str = settings.value(key).toStringList();
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

    Settings settings;
    //settings.beginGroup(QLatin1String("General"));

    if (!settings.contains(key))
    {
        return QDate();
    }

    QString timeStr = settings.value(key).toString();
    QDate date = QDate::fromString(timeStr, Qt::ISODate);

    return date;
}

bool Configuration::useXmlTimestamp()
{
    QString key = QLatin1String("useXmlTimestamp");

    Settings settings;
    //settings.beginGroup(QLatin1String("Global"));

    if (!settings.contains(key))
    {
        return false;
    }

    bool b = settings.value(key).toBool();
    return b;
}

// TODO: This function should allow reading more than one SIC value.
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

    Settings settings;
    settings.beginGroup(QLatin1String("KML"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QString pathStr = settings.value(key).toString();

    return pathStr;
}

QString Configuration::dgpsFile()
{
    QString key = QLatin1String("filepath");

    Settings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QString pathStr = settings.value(key).toString();

    return pathStr;
}

ModeS Configuration::dgpsModeS()
{
    QString key = QLatin1String("ModeS");

    Settings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    bool ok;
    QString val_str = settings.value(key).toString();
    quint32 val = val_str.toUInt(&ok, 16);

    if (!ok || val > 0xFFFFFF)
    {
        qFatal("Invalid %s value.", qPrintable(key));
    }

    return val;
}

Mode3A Configuration::dgpsMode3A()
{
    QString key = QLatin1String("Mode3A");

    Settings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    bool ok;
    QString val_str = settings.value(key).toString();
    quint16 val = val_str.toUInt(&ok, 8);

    if (val > 07777)
    {
        qFatal("Invalid %s value.", qPrintable(key));
    }

    return val;
}

Ident Configuration::dgpsIdent()
{
    QString key = QLatin1String("Ident");

    Settings settings;
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

qint32 Configuration::dgpsTodOffset()
{
    QString key = QLatin1String("TodOffset");

    Settings settings;
    settings.beginGroup(QLatin1String("DGPS"));

    if (!settings.contains(key))
    {
        return MOPS::defaultDgpsTimeOfDayOffset;
    }

    bool ok;
    QString val_str = settings.value(key).toString();
    quint16 val = val_str.toUInt(&ok);

    if (!ok || qFabs(val) > 86400)  // Maximum TOD.
    {
        qWarning() << "Invalid Time of Day Offset, using default value: "
                   << MOPS::defaultDgpsTimeOfDayOffset;

        return MOPS::defaultDgpsTimeOfDayOffset;
    }

    return val;
}

std::optional<QString> Configuration::logRules()
{
    QString key = QLatin1String("rules");

    Settings settings;
    settings.beginGroup(QLatin1String("LOG"));

    if (!settings.contains(key))
    {
        return std::nullopt;
    }

    QString rules = settings.value(key).toString();

    return rules;
}

std::optional<QString> Configuration::logPattern()
{
    QString key = QLatin1String("pattern");

    Settings settings;
    settings.beginGroup(QLatin1String("LOG"));

    if (!settings.contains(key))
    {
        return std::nullopt;
    }

    QString pattern = settings.value(key).toString();

    return pattern;
}
