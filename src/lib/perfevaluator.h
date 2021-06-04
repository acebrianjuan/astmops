/*!
 * \file perfevaluator.h
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

#ifndef ASTMOPS_PERFEVALUATOR_H
#define ASTMOPS_PERFEVALUATOR_H

#include "areahash.h"
#include "astmops.h"
#include "functions.h"
#include "track.h"

class PerfEvaluator
{
public:
    PerfEvaluator();

    void addData(const TrackCollectionSet &s);
    void run();

    void printPosAccResults() const;

private:
    void evalED116RPA(const Track &t_ref, const Track &t_tst);
    void evalED116PD(const Track &t_ref, const Track &t_tst);

    void evalED117RPA(const Track &t_ref, const Track &t_tst);
    void evalED117PD(const Track &t_ref, const Track &t_tst);
    void evalED117PID(const Track &t_ref, const Track &t_tst);
    void evalED117PLG(const Track &t_ref, const Track &t_tst);

    quint8 pic_p95;
    QHash<ModeS, TrackCollectionSet> sets_;
    AreaHash<QVector<double>> errors_;

    QVector<double> smrErrors_;
};

#endif  // ASTMOPS_PERFEVALUATOR_H
