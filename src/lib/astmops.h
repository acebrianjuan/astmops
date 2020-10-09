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


const float ed116TgtRepUpdateRate = 1.0;  // Hz
const float ed116SrvMsgUpdateRate = 1.0;  // Hz

const float ed117TgtRepUpdateRate = 1.0;  // Hz
const float ed117SrvMsgUpdateRate = 1.0;  // Hz


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

inline float tgtRepUpdateRate()
{
    QString key = QLatin1String("MOPS.TgtRepUpdateRate");

    QSettings settings;

    if (!settings.contains(key))
    {
        return ed117TgtRepUpdateRate;
    }

    float var = settings.value(key).toFloat();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed117TgtRepUpdateRate;
        return ed117TgtRepUpdateRate;
    }

    return var;
}

inline float srvMsgUpdateRate()
{
    QString key = QLatin1String("MOPS.SrvMsgUpdateRate");

    QSettings settings;

    if (!settings.contains(key))
    {
        return ed117SrvMsgUpdateRate;
    }

    float var = settings.value(key).toFloat();

    if (var <= 0)
    {
        qWarning() << "Invalid Update Rate, using default value: " << ed117SrvMsgUpdateRate;
        return ed117SrvMsgUpdateRate;
    }

    return var;
}

};  // namespace Configuration

#endif  // ASTMOPS_ASTMOPS_H
