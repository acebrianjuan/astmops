/*!
 * \file astmops.h
 * \brief Header file with essential definitions to be used in the scope
 * of the project.
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

using TrackNum = unsigned int;
using IcaoAddr = unsigned int;

enum SystemType
{
    Smr,
    Mlat,
    Adsb,
    Unknown
};

#endif  // ASTMOPS_ASTMOPS_H
