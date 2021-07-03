/*!
 * \file config.h
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

#ifndef ASTMOPS_CONFIG_H
#define ASTMOPS_CONFIG_H

#include "aerodrome.h"
#include "astmops.h"
#include <QGeoShape>

Sic readSic(const QString& key);

namespace Configuration
{
QDate asterixDate();

bool useXmlTimestamp();

Sic smrSic();

Sic mlatSic();

Sic adsbSic();

QString kmlFile();

double ed116TgtRepUpdateRate();

double ed116SrvMsgUpdateRate();

double ed117TgtRepUpdateRate();

double ed117SrvMsgUpdateRate();

double silencePeriod();

double probDetectionPeriod(Aerodrome::Area area);

qint32 dgpsTimeOfDayOffset();

//Mode3A dgpsMode3ACode();

ModeS dgpsTargetAddress();

//Ident dgpsIdentification();

QGeoShape evalSector();

};  // namespace Configuration

#endif  // ASTMOPS_CONFIG_H
