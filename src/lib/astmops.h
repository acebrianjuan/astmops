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

namespace Configuration
{
inline quint8 smrSic()
{
    QSettings settings;
    return settings.value(QLatin1String("SMR.SIC")).toUInt();
}

inline quint8 mlatSic()
{
    QSettings settings;
    return settings.value(QLatin1String("MLAT.SIC")).toUInt();
}

inline quint8 adsbSic()
{
    QSettings settings;
    return settings.value(QLatin1String("ADSB.SIC")).toUInt();
}

};  // namespace Configuration

#endif  // ASTMOPS_ASTMOPS_H
