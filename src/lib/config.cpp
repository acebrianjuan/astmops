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

QSet<Sic> Configuration::readSic(const QString& key)
{
    Settings settings;
    settings.beginGroup(QLatin1String("Asterix"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QSet<Sic> sicSet;

    QString str = settings.value(key).toString();

#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
    QStringList strList = str.split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    QStringList strList = str.split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif

    for (const QString& str : qAsConst(strList))
    {
        bool ok = false;
        quint32 val = str.toUInt(&ok);

        if (!ok || val > 255)
        {
            qFatal("Invalid %s value.", qPrintable(key));
        }

        sicSet << val;
    }

    return sicSet;
}

QDate Configuration::asterixDate()
{
    QString key = QLatin1String("Date");

    Settings settings;
    settings.beginGroup(QLatin1String("Asterix"));

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
    QString key = QLatin1String("UseXmlTimestamp");

    Settings settings;
    settings.beginGroup(QLatin1String("Asterix"));

    if (!settings.contains(key))
    {
        return false;
    }

    bool b = settings.value(key).toBool();
    return b;
}

QSet<Sic> Configuration::smrSic()
{
    QString key = QLatin1String("SmrSic");
    return readSic(key);
}

QSet<Sic> Configuration::mlatSic()
{
    QString key = QLatin1String("MlatSic");
    return readSic(key);
}

QSet<Sic> Configuration::adsbSic()
{
    QString key = QLatin1String("AdsbSic");
    return readSic(key);
}

QString Configuration::kmlFile()
{
    QString key = QLatin1String("Filepath");

    Settings settings;
    settings.beginGroup(QLatin1String("Kml"));

    if (!settings.contains(key))
    {
        qFatal("%s is mandatory.", qPrintable(key));
    }

    QString pathStr = settings.value(key).toString();

    return pathStr;
}

QString Configuration::dgpsFile()
{
    QString key = QLatin1String("Filepath");

    Settings settings;
    settings.beginGroup(QLatin1String("Dgps"));

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
    settings.beginGroup(QLatin1String("Dgps"));

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
    settings.beginGroup(QLatin1String("Dgps"));

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
    settings.beginGroup(QLatin1String("Dgps"));

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
    settings.beginGroup(QLatin1String("Dgps"));

    if (!settings.contains(key))
    {
        return MOPS::defaultDgpsTodOffset;
    }

    bool ok;
    QString val_str = settings.value(key).toString();
    quint16 val = val_str.toUInt(&ok);

    if (!ok || qFabs(val) > 86400)  // Maximum TOD.
    {
        qWarning() << "Invalid Time of Day Offset, using default value: "
                   << MOPS::defaultDgpsTodOffset;

        return MOPS::defaultDgpsTodOffset;
    }

    return val;
}

std::optional<QString> Configuration::logRules()
{
    QString key = QLatin1String("Rules");

    Settings settings;
    settings.beginGroup(QLatin1String("Log"));

    if (!settings.contains(key))
    {
        return std::nullopt;
    }

    QString rules = settings.value(key).toString();

    return rules;
}

std::optional<QString> Configuration::logPattern()
{
    QString key = QLatin1String("Pattern");

    Settings settings;
    settings.beginGroup(QLatin1String("Log"));

    if (!settings.contains(key))
    {
        return std::nullopt;
    }

    QString pattern = settings.value(key).toString();

    return pattern;
}
