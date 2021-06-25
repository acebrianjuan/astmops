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
#include "counters.h"
#include "functions.h"
#include "track.h"

struct PlgCounter
{
    quint32 n_gaps = 0;
    quint32 n_tr = 0;
};

struct PidCounter
{
    quint32 correct = 0;
    quint32 total = 0;
};

class PerfEvaluator
{
public:
    PerfEvaluator();

    void addData(const TrackCollectionSet &s);
    void run();

    void printPosAccResultsSmr() const;
    void printPosAccResultsMlat() const;

private:
    void computePicThreshold(double prctl);
    QVector<QPair<TargetReport, double>> euclideanDistance(const TgtRepMap &ref, const TgtRepMap &tst) const;

    void evalED116RPA(const Track &trk_ref, const Track &trk_tst);
    void evalED116PD(const Track &trk_ref, const TrackCollection &col_tst);

    void evalED117RPA(const Track &trk_ref, const Track &trk_tst);
    void evalED117PD(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED117PID(const Track &trk_ref, const Track &trk_tst);
    void evalED117PFID(const Track &trk_ref, const Track &trk_tst);
    void evalED117PLG(const TrackCollection &col_tst);

    quint8 pic_p95 = 0;
    QHash<ModeS, TrackCollectionSet> sets_;

    AreaHash<QVector<double>> smrRpaErrors_;
    AreaHash<QVector<double>> mlatRpaErrors_;
    AreaHash<PlgCounter> plg_;

    PidCounter mlatPidCounter_;
};

#endif  // ASTMOPS_PERFEVALUATOR_H
