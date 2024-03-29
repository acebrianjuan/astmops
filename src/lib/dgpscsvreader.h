/*!
 * \file dgpscsvreader.h
 * \brief
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * -----------------------------------------------------------------------
 *
 * Copyright (C) 2020-2021 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
 *
 * ASTMOPS is a command line tool for evaluating 
 * the performance of A-SMGCS sensors at airports
 *
 * This file is part of ASTMOPS.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------
 */

#ifndef ASTMOPS_DGPSCSVREADER_H
#define ASTMOPS_DGPSCSVREADER_H

#include "targetreport.h"
#include <QGeoPositionInfo>
#include <QIODevice>

enum ErrorType
{
    NoError,
    NotWellFormedHeaderError,
    OtherError
};

Q_DECLARE_METATYPE(ErrorType);

QVector<QGeoPositionInfo> readDgpsCsv(QIODevice *file, ErrorType *error = nullptr);

#endif  // ASTMOPS_DGPSCSVREADER_H
