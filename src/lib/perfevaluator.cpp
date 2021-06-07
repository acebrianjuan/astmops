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

#include "perfevaluator.h"

PerfEvaluator::PerfEvaluator()
{
}

void PerfEvaluator::addData(const TrackCollectionSet &s)
{
    if (!s.isValid())
    {
        return;
    }

    ModeS mode_s = s.mode_s();
    sets_.insert(mode_s, s);
}

void PerfEvaluator::run()
{
    // Set PIC threshold value.
    computePicThreshold(95);

    // Iterate through each target set.
    for (const TrackCollectionSet &s : qAsConst(sets_))
    {
        //ModeS mode_s = s.mode_s();
        TrackCollection c_ref = s.refTrackCol();

        // Iterate through each track in the reference collection.
        for (const Track &t_ref : c_ref)
        {
            TrackNum ref_tn = t_ref.track_number();
            QVector<TrackCollection> c_tst = s.getMatches(ref_tn);

            // Iterate through each test collection that matches with the
            // reference track.
            for (const TrackCollection &c_tst_i : c_tst)
            {
                if (c_tst_i.system_type() == SystemType::Smr)
                {
                    // SMR ED-116.

                    // Iterate through each test track in the collection.
                    for (const Track &t_tst : c_tst_i)
                    {
                        evalED116RPA(t_ref, t_tst);
                        //evalED116PD(t_ref, t_tst);
                    }
                }
                else if (c_tst_i.system_type() == SystemType::Mlat)
                {
                    // MLAT ED-117.

                    // Iterate through each test track in the collection.
                    for (const Track &t_tst : c_tst_i)
                    {
                        Q_UNUSED(t_tst);

                        //evalED117RPA(t_ref, t_tst);
                        //evalED117PD(t_ref, t_tst);
                        //evalED117PID(t_ref, t_tst);
                        //evalED117PLG(t_ref, t_tst);
                    }
                }
            }
        }
    }
}

void PerfEvaluator::printPosAccResults() const
{
    qDebug() << "P95:"
             << "\t" << percentile(smrErrors_, 95) << "m"
             << "\t"
             << "P99:"
             << "\t" << percentile(smrErrors_, 99) << "m"
             << "\t"
             << "Mean:"
             << "\t" << mean(smrErrors_) << "m"
             << "\t"
             << "StdDev:"
             << "\t" << stdDev(smrErrors_) << "m"
             << "\t"
             << "N:"
             << "\t" << smrErrors_.size();
}

void PerfEvaluator::computePicThreshold(double prctl)
{
    QVector<double> vec;
    for (const TrackCollectionSet &s : qAsConst(sets_))
    {
        for (const Track &t : s.refTrackCol())
        {
            for (const TargetReport &tr : t)
            {
                if (tr.ver_.has_value() && tr.pic_.has_value())
                {
                    if (tr.ver_ == 2)
                    {
                        vec << tr.pic_.value();
                    }
                }
            }
        }
    }

    pic_p95 = percentile(vec, prctl);
}

void PerfEvaluator::evalED116RPA(const Track &t_ref, const Track &t_tst)
{
    if (!haveSpaceIntersection(t_tst, t_ref))
    {
        return;
    }

    auto euclideanDistance = [](const TgtRepMap &lhs, const TgtRepMap &rhs) {
        QVector<double> dist;
        for (TgtRepMap::const_iterator it = lhs.begin(); it != lhs.end(); ++it)
        {
            QDateTime tod = it.key();
            if (!rhs.contains(tod))
            {
                continue;
            }

            TargetReport tr_lhs = it.value();
            TargetReport tr_rhs = rhs.value(tod);

            Q_ASSERT(tr_lhs.tod_ == tr_rhs.tod_);

            dist << qSqrt(qPow(tr_lhs.x_ - tr_rhs.x_, 2) +
                          qPow(tr_lhs.y_ - tr_rhs.y_, 2));
        }

        return dist;
    };

    // Make a copy of the reference track.
    Track t_r = t_ref;

    qDebug() << "Before:" << t_r.data().size();
    TgtRepMap::iterator it = t_r.begin();
    while (it != t_r.end())
    {
        TargetReport tr = it.value();

        if (tr.ver_.has_value() && tr.pic_.has_value())
        {
            if (tr.ver_.value() < 2 || tr.pic_.value() < pic_p95)
            {
                it = t_r.rdata().erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            it = t_r.rdata().erase(it);
        }
    }
    qDebug() << "After:" << t_r.data().size();

    // Extract TST track portion that matches in time with the
    // reference track.
    //Track t_t = intersect(t_tst, t_ref).value();

    // Interpolate the TST track at the times of the REF track points.
    Track t_t = resample(t_tst, t_r.timestamps());

    // TST and REF trajectories should match in size.
    //Q_ASSERT(t_t.size() == t_r.size());

    // TST and REF pairs should have exact same times.
    //Q_ASSERT(t_t.data().keys() == t_r.data().keys());

    // Calculate Euclidean distance between TST-REF pairs.
    QVector<double> dist = euclideanDistance(t_r.rdata(), t_t.rdata());

    smrErrors_ << dist;
}
