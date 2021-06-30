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
#include "trackassociator.h"

struct PdCounter
{
    quint32 n_trp_ = 0;
    quint32 n_up_ = 0;
};

struct PfdCounter
{
    quint32 n_ftr_ = 0;
    quint32 n_tr_ = 0;
};

struct PidCounter
{
    quint32 n_citr_ = 0;
    quint32 n_itr_ = 0;
};

struct PfidCounter
{
    quint32 n_eitr_ = 0;
    quint32 n_itr_ = 0;
};

struct PlgCounter
{
    quint32 n_g_ = 0;
    quint32 n_tr_ = 0;
};

class PerfEvaluator
{
public:
    PerfEvaluator();

    void addData(const Track &t);
    void run();

    void printPosAccResultsSmr() const;
    void printPosAccResultsMlat() const;

private:
    void computePicThreshold(double prctl);
    QVector<QPair<TargetReport, double>> euclideanDistance(const TgtRepMap &ref, const TgtRepMap &tst) const;
    Track filterTrackByQuality(const Track &trk, quint8 ver, quint8 pic) const;

    void evalED116RPA(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED116PD(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED116PFD(const Track &trk_ref, const TrackCollection &col_tst);

    void evalED117RPA(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED117PD(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED117PFD(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED117PID(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED117PFID(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED117PLG(const TrackCollection &col_tst);

    TrackAssociator trkAssoc;

    quint8 pic_p95 = 0;

    AreaHash<QVector<double>> smrRpaErrors_;
    AreaHash<QVector<double>> mlatRpaErrors_;

    AreaHash<PdCounter> smrPd_;
    AreaHash<PdCounter> mlatPd_;

    AreaHash<PfdCounter> mlatPfd_;

    AreaHash<PidCounter> mlatPidIdent_;
    AreaHash<PidCounter> mlatPidMode3A_;

    AreaHash<PfidCounter> mlatPfidIdent_;
    AreaHash<PfidCounter> mlatPfidMode3A_;

    AreaHash<PlgCounter> mlatPlg_;
};

#endif  // ASTMOPS_PERFEVALUATOR_H
