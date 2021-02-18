/*!
 * \file aerodromereaderinterface.h
 * \brief Header file of the interface to an aerodrome reader.
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

#ifndef ASTMOPS_AERODROMEREADERINTERFACE_H
#define ASTMOPS_AERODROMEREADERINTERFACE_H

#include "aerodrome.h"

class QIODevice;

class AerodromeReaderInterface
{
public:
    virtual bool read(QIODevice *device) = 0;
    virtual Aerodrome makeAerodrome() const = 0;
};


#endif  // ASTMOPS_AERODROMEREADERINTERFACE_H
