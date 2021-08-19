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
#include "trafficperiod.h"

class PerfEvaluator
{
    friend class PerfEvaluatorTest;

public:
    PerfEvaluator();

    void addData(const Track &t);
    void run();
    void setDgpsOnly(bool b);

    void printED116RPA() const;
    void printED116UR() const;
    void printED116PD() const;
    void printED116PFD() const;

    void printED117RPA() const;
    void printED117UR() const;
    void printED117PD() const;
    void printED117PFD() const;
    void printED117PID_Ident() const;
    void printED117PID_Mode3A() const;
    void printED117PFID_Ident() const;
    void printED117PFID_Mode3A() const;
    void printED117PLG() const;

private:
    void computePicThreshold(double prctl);
    QVector<QPair<TargetReport, double>> euclideanDistance(const TgtRepMap &ref, const TgtRepMap &tst) const;
    Track filterTrackByQuality(const Track &trk, quint8 ver, quint8 pic) const;

    void evalED116RPA(const TrackCollectionSet &s);
    void evalED116UR(const TrackCollectionSet &s);
    void evalED116PD(const TrackCollectionSet &s);
    void evalED116PFD(const TrackCollectionSet &s);

    void evalED117RPA(const TrackCollectionSet &s);
    void evalED117UR(const TrackCollectionSet &s);
    void evalED117PD(const TrackCollectionSet &s);
    void evalED117PFD(const TrackCollectionSet &s);
    void evalED117PID(const TrackCollectionSet &s);
    void evalED117PFID(const TrackCollectionSet &s);
    void evalED117PLG(const TrackCollectionSet &s);

    TrackAssociator trkAssoc_;

    bool dgps_only_ = false;
    quint8 pic_p95_ = 0;

    AreaHash<TrafficPeriodCollection> trafficPeriods_;

    AreaHash<QVector<double>> smrRpaErrors_;
    AreaHash<QVector<double>> mlatRpaErrors_;

    AreaHash<Counters::UrCounter> smrUr_;
    AreaHash<Counters::UrCounter> mlatUr_;

    AreaHash<Counters::PdCounter> smrPd_;
    AreaHash<Counters::PdCounter> mlatPd_;

    AreaHash<Counters::PfdCounter2> smrPfd_;
    AreaHash<Counters::PfdCounter> mlatPfd_;

    AreaHash<Counters::PidCounter> mlatPidIdent_;
    AreaHash<Counters::PidCounter> mlatPidMode3A_;

    AreaHash<Counters::PfidCounter> mlatPfidIdent_;
    AreaHash<Counters::PfidCounter> mlatPfidMode3A_;

    AreaHash<Counters::PlgCounter> mlatPlg_;
};

#endif  // ASTMOPS_PERFEVALUATOR_H
