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


const float ed116TgtRepUpdateRateHz = 1.0;  // Hz
const float ed116SrvMsgUpdateRateHz = 1.0;  // Hz

const float ed117TgtRepUpdateRateHz = 1.0;  // Hz
const float ed117SrvMsgUpdateRateHz = 1.0;  // Hz

const float silencePeriodSeconds = 60;  // s


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

inline float ed116TgtRepUpdateRate()
{
    QString key = QLatin1String("MOPS.ed116TgtRepUpdateRate");

    QSettings settings;

    if (!settings.contains(key))
    {
        return ed116TgtRepUpdateRateHz;
    }

    float var = settings.value(key).toFloat();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed116TgtRepUpdateRateHz;
        return ed116TgtRepUpdateRateHz;
    }

    return var;
}

inline float ed116SrvMsgUpdateRate()
{
    QString key = QLatin1String("MOPS.ed116SrvMsgUpdateRate");

    QSettings settings;

    if (!settings.contains(key))
    {
        return ed116SrvMsgUpdateRateHz;
    }

    float var = settings.value(key).toFloat();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed116SrvMsgUpdateRateHz;
        return ed116SrvMsgUpdateRateHz;
    }

    return var;
}

inline float ed117TgtRepUpdateRate()
{
    QString key = QLatin1String("MOPS.ed117TgtRepUpdateRate");

    QSettings settings;

    if (!settings.contains(key))
    {
        return ed117TgtRepUpdateRateHz;
    }

    float var = settings.value(key).toFloat();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed117TgtRepUpdateRateHz;
        return ed117TgtRepUpdateRateHz;
    }

    return var;
}

inline float ed117SrvMsgUpdateRate()
{
    QString key = QLatin1String("MOPS.ed117SrvMsgUpdateRate");

    QSettings settings;

    if (!settings.contains(key))
    {
        return ed117SrvMsgUpdateRateHz;
    }

    float var = settings.value(key).toFloat();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed117SrvMsgUpdateRateHz;
        return ed117SrvMsgUpdateRateHz;
    }

    return var;
}

inline float silencePeriod()
{
    QString key = QLatin1String("MOPS.SilencePeriod");

    QSettings settings;

    if (!settings.contains(key))
    {
        return silencePeriodSeconds;
    }

    float var = settings.value(key).toFloat();

    if (var <= 0)
    {
        qWarning() << "Invalid Silence Period, using default value: " << silencePeriodSeconds;
        return silencePeriodSeconds;
    }

    return var;
}

};  // namespace Configuration

#endif  // ASTMOPS_ASTMOPS_H
