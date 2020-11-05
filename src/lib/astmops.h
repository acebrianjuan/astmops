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

#include <QDebug>
#include <QSettings>
#include <QtGlobal>

using TrackNum = quint32;
using IcaoAddr = quint32;

enum SystemType
{
    Smr,
    Mlat,
    Adsb,
    Unknown
};


const double ed116TgtRepUpdateRateHz = 1.0;  // Hz
const double ed116SrvMsgUpdateRateHz = 1.0;  // Hz

const double ed117TgtRepUpdateRateHz = 1.0;  // Hz
const double ed117SrvMsgUpdateRateHz = 1.0;  // Hz

const double silencePeriodSeconds = 60.0;  // s


static quint8 readSic(QString key)
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
    QString key = QLatin1String("MOPS.ed116TgtRepUpdateRate");

    QSettings settings;

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
    QString key = QLatin1String("MOPS.ed116SrvMsgUpdateRate");

    QSettings settings;

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
    QString key = QLatin1String("MOPS.ed117TgtRepUpdateRate");

    QSettings settings;

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
    QString key = QLatin1String("MOPS.ed117SrvMsgUpdateRate");

    QSettings settings;

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
    QString key = QLatin1String("MOPS.SilencePeriod");

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

};  // namespace Configuration

#endif  // ASTMOPS_ASTMOPS_H
