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

class PerfEvaluator
{
    friend class PerfEvaluatorTest;

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
    void evalED116UR(const TrackCollectionSet &s);
    void evalED116PD(const TrackCollectionSet &s);
    void evalED116PFD(const Track &trk_ref, const TrackCollection &col_tst);


    void evalED117RPA(const Track &trk_ref, const TrackCollection &col_tst);
    void evalED117UR(const TrackCollectionSet &s);
    void evalED117PD(const TrackCollectionSet &s);
    void evalED117PFD(const TrackCollectionSet &s);
    void evalED117PID(const TrackCollectionSet &s);
    void evalED117PFID(const TrackCollectionSet &s);
    void evalED117PLG(const Track &trk_ref, const TrackCollection &col_tst);

    TrackAssociator trkAssoc;

    quint8 pic_p95 = 0;

    AreaHash<QVector<double>> smrRpaErrors_;
    AreaHash<QVector<double>> mlatRpaErrors_;

    AreaHash<Counters::UrCounter> smrUr_;
    AreaHash<Counters::UrCounter> mlatUr_;

    AreaHash<Counters::PdCounter> smrPd_;
    AreaHash<Counters::PdCounter> mlatPd_;

    Counters::PfdCounter2 smrPfd_;
    AreaHash<Counters::PfdCounter> mlatPfd_;

    AreaHash<Counters::PidCounter> mlatPidIdent_;
    AreaHash<Counters::PidCounter> mlatPidMode3A_;

    AreaHash<Counters::PfidCounter> mlatPfidIdent_;
    AreaHash<Counters::PfidCounter> mlatPfidMode3A_;

    AreaHash<Counters::PlgCounter> mlatPlg_;
};

#endif  // ASTMOPS_PERFEVALUATOR_H
