/*!
 * \file functions.h
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

#ifndef ASTMOPS_FUNCTIONS_H
#define ASTMOPS_FUNCTIONS_H

#include <QVector>

double percentile(QVector<double> v, double percent);
double mean(const QVector<double> &v);
double stdDev(const QVector<double> &v);

#endif  // ASTMOPS_FUNCTIONS_H
