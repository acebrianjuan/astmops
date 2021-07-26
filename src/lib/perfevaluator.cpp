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

void PerfEvaluator::addData(const Track &t)
{
    trkAssoc_.addData(t);
}

void PerfEvaluator::run()
{
    // Run track association.
    trkAssoc_.run();

    // Set PIC threshold value.
    computePicThreshold(95);

    // Iterate through each target set.
    for (const TrackCollectionSet &s : qAsConst(trkAssoc_.sets()))
    {
        // SMR ED-116.
        evalED116RPA(s);
        evalED116UR(s);
        evalED116PD(s);
        evalED116PFD(s);

        // MLAT ED-117.
        evalED117RPA(s);
        evalED117UR(s);
        evalED117PD(s);
        evalED117PFD(s);
        evalED117PID(s);
        evalED117PFID(s);
        evalED117PLG(s);
    }
}

void PerfEvaluator::computePicThreshold(double prctl)
{
    QVector<double> vec;
    for (const TrackCollectionSet &s : qAsConst(trkAssoc_.sets()))
    {
        for (const Track &t : s.refTrackCol())
        {
            for (const TargetReport &tr : t)
            {
                if (tr.narea_.area_ != Aerodrome::Area::None &&
                    tr.ver_.has_value() && tr.pic_.has_value())
                {
                    if (tr.ver_ == 2)
                    {
                        vec << tr.pic_.value();
                    }
                }
            }
        }
    }

    if (vec.isEmpty())
    {
        // qWarning() or qFatal();
        return;
    }

    double pctl = percentile(vec, prctl);
    if (qIsNaN(pctl))
    {
        // qWarning() or qFatal();
        return;
    }

    pic_p95_ = pctl;
}

QVector<QPair<TargetReport, double>> PerfEvaluator::euclideanDistance(const TgtRepMap &ref, const TgtRepMap &tst) const
{
    QVector<QPair<TargetReport, double>> v;

    for (TgtRepMap::const_iterator it = ref.begin(); it != ref.end(); ++it)
    {
        QDateTime tod = it.key();
        if (!tst.contains(tod))
        {
            continue;
        }

        TargetReport tr_ref = it.value();
        TargetReport tr_tst = tst.value(tod);

        Q_ASSERT(tr_ref.tod_ == tr_tst.tod_);

        double dist = qSqrt(qPow(tr_ref.x_ - tr_tst.x_, 2) +
                            qPow(tr_ref.y_ - tr_tst.y_, 2));

        QPair<TargetReport, double> p{tr_ref, dist};
        v << p;
    }

    return v;
}

Track PerfEvaluator::filterTrackByQuality(const Track &trk, quint8 ver, quint8 pic) const
{
    // Make a copy of the input track.
    Track trk_out = trk;

    TgtRepMap::iterator it = trk_out.begin();
    while (it != trk_out.end())
    {
        TargetReport tr = it.value();

        if (tr.ver_.has_value() && tr.pic_.has_value())
        {
            if (tr.ver_.value() != ver || tr.pic_.value() < pic)
            {
                it = trk_out.rdata().erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            it = trk_out.rdata().erase(it);
        }
    }

    return trk_out;
}

void PerfEvaluator::evalED116RPA(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Smr);

        if (col_tst_opt.has_value())
        {
            TrackCollection col_tst = col_tst_opt.value();

            // Iterate through each reference sub-track.
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                // Iterate through each track in the test data collection.
                for (const Track &trk_tst : col_tst)
                {
                    if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                    {
                        continue;
                    }

                    // Only keep target reports with MOPS version 2 and PIC above the
                    // 95th percentile threshold.
                    Track t_r = filterTrackByQuality(sub_trk_ref, 2, pic_p95_);

                    // Interpolate the TST track at the times of the REF track points.
                    Track t_t = resample(trk_tst, t_r.timestamps());

                    // Calculate Euclidean distance between TST-REF pairs.
                    QVector<QPair<TargetReport, double>> dists = euclideanDistance(t_r.rdata(), t_t.rdata());

                    for (const QPair<TargetReport, double> &p : dists)
                    {
                        Aerodrome::NamedArea narea = p.first.narea_;
                        double dist = p.second;
                        smrRpaErrors_[narea] << dist;
                    }
                }
            }
        }
    }
}

void PerfEvaluator::evalED116UR(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Smr);

        if (col_tst_opt.has_value())
        {
            TrackCollection col_tst = col_tst_opt.value();

            // Iterate through each reference sub-track.
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double dur = sub_trk_ref.duration();
                double freq = 1.0;
                int n_etrp = qFloor(dur * freq);

                smrUr_[narea].n_etrp_ += n_etrp;

                // Iterate through each track in the test data collection.
                for (const Track &trk_tst : col_tst)
                {
                    if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                    {
                        continue;
                    }

                    // Extract TST track portion that matches in time with the
                    // REF track.
                    Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                    smrUr_[narea].n_trp_ += sub_trk_tst.size();
                }
            }
        }
        else
        {
            // If reference track has no matching collection of test tracks
            // then just count the expected target reports for each reference
            // sub-track and jump to the next reference track.

            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double dur = sub_trk_ref.duration();
                double freq = 1.0;
                int n_etrp = qFloor(dur * freq);

                smrUr_[narea].n_etrp_ += n_etrp;
            }
        }
    }
}

void PerfEvaluator::evalED116PD(const TrackCollectionSet &s)
{
    auto hasPosition = [](const TargetReport &tr) {
        return !qIsNaN(tr.x_) && !qIsNaN(tr.y_);
    };

    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Smr);

        if (col_tst_opt.has_value())
        {
            TrackCollection col_tst = col_tst_opt.value();

            // Iterate through each reference sub-track.
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double period = 1.0;
                Counters::IntervalCounter intervalCtr(period, sub_trk_ref.beginTimestamp());

                // Iterate through each track in the test data collection.
                for (const Track &trk_tst : col_tst)
                {
                    if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                    {
                        continue;
                    }

                    // Extract TST track portion that matches in time with the
                    // REF track.
                    Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                    // Iterate through every target report in the test sub-track.
                    for (const TargetReport &tr_tst : sub_trk_tst)
                    {
                        if (hasPosition(tr_tst))
                        {
                            intervalCtr.update(tr_tst.tod_);
                        }
                    }
                }

                intervalCtr.finish(sub_trk_ref.endTimestamp());

                Counters::BasicCounter ctr = intervalCtr.read();

                smrPd_[narea].n_trp_ += ctr.valid_;
                smrPd_[narea].n_up_ += ctr.total_;
            }
        }
        else
        {
            // If reference track has no matching collection of test tracks
            // then just count the expected target reports for each reference
            // sub-track and jump to the next reference track.

            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double period = 1.0;
                Counters::IntervalCounter intervalCtr(period, sub_trk_ref.beginTimestamp());
                intervalCtr.finish(sub_trk_ref.endTimestamp());

                Counters::BasicCounter ctr = intervalCtr.read();

                Q_ASSERT(ctr.valid_ == 0);

                smrPd_[narea].n_trp_ += ctr.valid_;
                smrPd_[narea].n_up_ += ctr.total_;
            }
        }
    }
}

void PerfEvaluator::evalED116PFD(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();
    double freq = 1.0;

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Smr);

        if (col_tst_opt.has_value())
        {
            TrackCollection col_tst = col_tst_opt.value();

            // Iterate through each reference sub-track.
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                trafficPeriods_[narea] << sub_trk_ref;
                smrPfd_[narea].n_u_ = trafficPeriods_[narea].expectedUpdates(freq);
                smrPfd_[narea].n_etr_ = trafficPeriods_[narea].expectedTgtReps(freq);

                // Iterate through each track in the test data collection.
                for (const Track &trk_tst : col_tst)
                {
                    if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                    {
                        continue;
                    }

                    // Extract TST track portion that matches in time with the
                    // REF track.
                    Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                    smrPfd_[narea].n_tr_ += sub_trk_tst.size();
                }
            }
        }
        /*
        else
        {
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                trafficPeriods_[narea] << sub_trk_ref;
                smrPfd_[narea].n_u_ = trafficPeriods_[narea].expectedUpdates(freq);
                smrPfd_[narea].n_etr_ = trafficPeriods_[narea].expectedTgtReps(freq);
            }
        }
        */
    }
}

void PerfEvaluator::evalED117RPA(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Mlat);

        if (col_tst_opt.has_value())
        {
            TrackCollection col_tst = col_tst_opt.value();

            // Iterate through each reference sub-track.
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                // Iterate through each track in the test data collection.
                for (const Track &trk_tst : col_tst)
                {
                    if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                    {
                        continue;
                    }

                    // Only keep target reports with MOPS version 2 and PIC above the
                    // 95th percentile threshold.
                    Track t_r = filterTrackByQuality(sub_trk_ref, 2, pic_p95_);

                    // Interpolate the TST track at the times of the REF track points.
                    Track t_t = resample(trk_tst, t_r.timestamps());

                    // Calculate Euclidean distance between TST-REF pairs.
                    QVector<QPair<TargetReport, double>> dists = euclideanDistance(t_r.rdata(), t_t.rdata());

                    for (const QPair<TargetReport, double> &p : dists)
                    {
                        Aerodrome::NamedArea narea = p.first.narea_;
                        double dist = p.second;
                        mlatRpaErrors_[narea] << dist;
                    }
                }
            }
        }
    }
}

void PerfEvaluator::evalED117UR(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Mlat);

        if (col_tst_opt.has_value())
        {
            TrackCollection col_tst = col_tst_opt.value();

            // Iterate through each reference sub-track.
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double dur = sub_trk_ref.duration();
                double freq = 1.0;
                int n_etrp = qFloor(dur * freq);

                mlatUr_[narea].n_etrp_ += n_etrp;

                // Iterate through each track in the test data collection.
                for (const Track &trk_tst : col_tst)
                {
                    if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                    {
                        continue;
                    }

                    // Extract TST track portion that matches in time with the
                    // REF track.
                    Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                    mlatUr_[narea].n_trp_ += sub_trk_tst.size();
                }
            }
        }
        else
        {
            // If reference track has no matching collection of test tracks
            // then just count the expected target reports for each reference
            // sub-track and jump to the next reference track.

            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double dur = sub_trk_ref.duration();
                double freq = 1.0;
                int n_etrp = qFloor(dur * freq);

                mlatUr_[narea].n_etrp_ += n_etrp;
            }
        }
    }
}

void PerfEvaluator::evalED117PD(const TrackCollectionSet &s)
{
    auto hasPosition = [](const TargetReport &tr) {
        return !qIsNaN(tr.x_) && !qIsNaN(tr.y_);
    };

    auto getPeriodForArea = [](const Aerodrome::NamedArea &narea) {
        double period = 1.0;
        if (narea.area_ == Aerodrome::Area::Runway)
        {
            period = 1.0;
        }
        else if (narea.area_ == Aerodrome::Area::Taxiway)
        {
            period = 2.0;
        }
        else if (narea.area_ == Aerodrome::Area::Stand)
        {
            period = 5.0;
        }

        return period;
    };

    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Mlat);

        if (col_tst_opt.has_value())
        {
            TrackCollection col_tst = col_tst_opt.value();

            // Iterate through each reference sub-track.
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double period = getPeriodForArea(narea);

                Counters::IntervalCounter intervalCtr(period, sub_trk_ref.beginTimestamp());

                // Iterate through each track in the test data collection.
                for (const Track &trk_tst : col_tst)
                {
                    if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                    {
                        continue;
                    }

                    // Extract TST track portion that matches in time with the
                    // REF track.
                    Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                    // Iterate through every target report in the test sub-track.
                    for (const TargetReport &tr_tst : sub_trk_tst)
                    {
                        if (hasPosition(tr_tst))
                        {
                            intervalCtr.update(tr_tst.tod_);
                        }
                    }
                }

                intervalCtr.finish(sub_trk_ref.endTimestamp());

                Counters::BasicCounter ctr = intervalCtr.read();

                mlatPd_[narea].n_trp_ += ctr.valid_;
                mlatPd_[narea].n_up_ += ctr.total_;
            }
        }
        else
        {
            // If reference track has no matching collection of test tracks
            // then just count the expected target reports for each reference
            // sub-track and jump to the next reference track.

            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                double period = getPeriodForArea(narea);

                Counters::IntervalCounter intervalCtr(period, sub_trk_ref.beginTimestamp());
                intervalCtr.finish(sub_trk_ref.endTimestamp());

                Counters::BasicCounter ctr = intervalCtr.read();

                Q_ASSERT(ctr.valid_ == 0);

                mlatPd_[narea].n_trp_ += ctr.valid_;
                mlatPd_[narea].n_up_ += ctr.total_;
            }
        }
    }
}

void PerfEvaluator::evalED117PFD(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Mlat);

        if (!col_tst_opt.has_value())
        {
            return;
        }

        TrackCollection col_tst = col_tst_opt.value();

        // Iterate through each reference sub-track.
        for (const Track &sub_trk_ref : sub_trk_vec)
        {
            if (sub_trk_ref.duration() < 1)
            {
                continue;
            }

            Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

            // Iterate through each track in the test data collection.
            for (const Track &trk_tst : col_tst)
            {
                if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                {
                    continue;
                }

                // Interpolate the REF track at the times of the TST track plots.
                const Track sub_trk_ref_i = resample(sub_trk_ref, trk_tst.timestamps());

                // Calculate Euclidean distance between TST-REF pairs.
                QVector<QPair<TargetReport, double>> dists = euclideanDistance(sub_trk_ref_i.data(), trk_tst.data());

                for (const QPair<TargetReport, double> &p : dists)
                {
                    double dist = p.second;

                    if (dist >= 50.0)
                    {
                        ++mlatPfd_[narea].n_ftr_;
                    }
                    ++mlatPfd_[narea].n_tr_;
                }
            }
        }
    }
}

void PerfEvaluator::evalED117PID(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Mlat);

        if (!col_tst_opt.has_value())
        {
            return;
        }

        TrackCollection col_tst = col_tst_opt.value();

        // Iterate through each reference sub-track.
        for (const Track &sub_trk_ref : sub_trk_vec)
        {
            if (sub_trk_ref.duration() < 1)
            {
                continue;
            }

            Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

            const TgtRepMap &sub_trk_ref_data = sub_trk_ref.data();

            // Iterate through each track in the test data collection.
            for (const Track &trk_tst : col_tst)
            {
                if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                {
                    continue;
                }

                // Extract TST track portion that matches in time with the
                // REF track.
                Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                // Iterate through every target report in the test sub-track.
                for (const TargetReport &tr_tst : sub_trk_tst)
                {
                    if (!tr_tst.ident_.has_value() && !tr_tst.mode_3a_.has_value())
                    {
                        // Skip if target report has no identification and no mode 3/A code.
                        continue;
                    }

                    QDateTime tod = tr_tst.tod_;

                    TgtRepMap::const_iterator it_u = sub_trk_ref_data.end();  // Upper.
                    TgtRepMap::const_iterator it_l = sub_trk_ref_data.end();  // Lower.

                    TgtRepMap::const_iterator it = sub_trk_ref_data.lowerBound(tod);
                    if (it != sub_trk_ref_data.end())  // Upper TOD found.
                    {
                        Q_ASSERT(it.key() >= tod);

                        // Save upper value.
                        it_u = it;

                        // Search lower values by decrementing iterator.
                        while (it != sub_trk_ref_data.end() && tod < it.key())
                        {
                            if (it == sub_trk_ref_data.begin())  // Exit condition on first value.
                            {
                                if (tod < it.key())  // Set as not found.
                                {
                                    it = sub_trk_ref_data.end();
                                }

                                break;
                            }

                            it--;
                        }

                        if (it != sub_trk_ref_data.end())  // Lower TOD found.
                        {
                            Q_ASSERT(tod >= it.key());

                            // Save lower value.
                            it_l = it;
                        }

                        if (it_l != sub_trk_ref_data.end() && it_u != sub_trk_ref_data.end())
                        {
                            TargetReport tr_l = it_l.value();
                            TargetReport tr_u = it_u.value();

                            if (!tr_l.ident_.has_value() &&
                                !tr_u.ident_.has_value())
                            {
                                continue;
                            }

                            Aerodrome::NamedArea narea = tr_tst.narea_;

                            if (tr_tst.ident_.has_value())
                            {
                                bool ident_ok = false;

                                if (tr_l.ident_.has_value())
                                {
                                    ident_ok = tr_tst.ident_.value() == tr_l.ident_.value();
                                }

                                if (!ident_ok && tr_u.ident_.has_value())
                                {
                                    ident_ok = tr_tst.ident_.value() == tr_u.ident_.value();
                                }

                                ++mlatPidIdent_[narea].n_itr_;
                                if (ident_ok)
                                {
                                    ++mlatPidIdent_[narea].n_citr_;
                                }
                            }

                            if (tr_tst.mode_3a_.has_value())
                            {
                                bool mode_3a_ok = false;

                                if (tr_l.mode_3a_.has_value())
                                {
                                    mode_3a_ok = tr_tst.mode_3a_.value() == tr_l.mode_3a_.value();
                                }

                                if (!mode_3a_ok && tr_u.mode_3a_.has_value())
                                {
                                    mode_3a_ok = tr_tst.mode_3a_.value() == tr_u.mode_3a_.value();
                                }

                                ++mlatPidMode3A_[narea].n_itr_;
                                if (mode_3a_ok)
                                {
                                    ++mlatPidMode3A_[narea].n_citr_;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void PerfEvaluator::evalED117PFID(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Mlat);

        if (!col_tst_opt.has_value())
        {
            return;
        }

        TrackCollection col_tst = col_tst_opt.value();

        // Iterate through each reference sub-track.
        for (const Track &sub_trk_ref : sub_trk_vec)
        {
            if (sub_trk_ref.duration() < 1)
            {
                continue;
            }

            Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

            const TgtRepMap &sub_trk_ref_data = sub_trk_ref.data();

            // Iterate through each track in the test data collection.
            for (const Track &trk_tst : col_tst)
            {
                if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                {
                    continue;
                }

                // Extract TST track portion that matches in time with the
                // REF track.
                Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                // Iterate through every target report in the test sub-track.
                for (const TargetReport &tr_tst : sub_trk_tst)
                {
                    if (!tr_tst.ident_.has_value() && !tr_tst.mode_3a_.has_value())
                    {
                        // Skip if target report has no identification and no mode 3/A code.
                        continue;
                    }

                    QDateTime tod = tr_tst.tod_;

                    TgtRepMap::const_iterator it_u = sub_trk_ref_data.end();  // Upper.
                    TgtRepMap::const_iterator it_l = sub_trk_ref_data.end();  // Lower.

                    TgtRepMap::const_iterator it = sub_trk_ref_data.lowerBound(tod);
                    if (it != sub_trk_ref_data.end())  // Upper TOD found.
                    {
                        Q_ASSERT(it.key() >= tod);

                        // Save upper value.
                        it_u = it;

                        // Search lower values by decrementing iterator.
                        while (it != sub_trk_ref_data.end() && tod < it.key())
                        {
                            if (it == sub_trk_ref_data.begin())  // Exit condition on first value.
                            {
                                if (tod < it.key())  // Set as not found.
                                {
                                    it = sub_trk_ref_data.end();
                                }

                                break;
                            }

                            it--;
                        }

                        if (it != sub_trk_ref_data.end())  // Lower TOD found.
                        {
                            Q_ASSERT(tod >= it.key());

                            // Save lower value.
                            it_l = it;
                        }

                        if (it_l != sub_trk_ref_data.end() && it_u != sub_trk_ref_data.end())
                        {
                            TargetReport tr_l = it_l.value();
                            TargetReport tr_u = it_u.value();

                            if (!tr_l.ident_.has_value() &&
                                !tr_u.ident_.has_value())
                            {
                                continue;
                            }

                            Aerodrome::NamedArea narea = tr_tst.narea_;

                            if (tr_tst.ident_.has_value())
                            {
                                bool ident_nok = false;

                                if (tr_l.ident_.has_value())
                                {
                                    ident_nok = tr_tst.ident_.value() != tr_l.ident_.value();
                                }

                                if (!ident_nok && tr_u.ident_.has_value())
                                {
                                    ident_nok = tr_tst.ident_.value() != tr_u.ident_.value();
                                }

                                ++mlatPfidIdent_[narea].n_itr_;
                                if (ident_nok)
                                {
                                    ++mlatPfidIdent_[narea].n_eitr_;
                                }
                            }

                            if (tr_tst.mode_3a_.has_value())
                            {
                                bool mode_3a_nok = false;

                                if (tr_l.mode_3a_.has_value())
                                {
                                    mode_3a_nok = tr_tst.mode_3a_.value() != tr_l.mode_3a_.value();
                                }

                                if (!mode_3a_nok && tr_u.mode_3a_.has_value())
                                {
                                    mode_3a_nok = tr_tst.mode_3a_.value() != tr_u.mode_3a_.value();
                                }

                                ++mlatPfidMode3A_[narea].n_itr_;
                                if (mode_3a_nok)
                                {
                                    ++mlatPfidMode3A_[narea].n_eitr_;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void PerfEvaluator::evalED117PLG(const TrackCollectionSet &s)
{
    TrackCollection col_ref = s.refTrackCol();

    // Iterate through each track in the reference data collection.
    for (const Track &trk_ref : col_ref)
    {
        TrackNum ref_tn = trk_ref.track_number();
        QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

        // Get collection of test tracks that match with the reference track.
        std::optional<TrackCollection> col_tst_opt = s.matchesForRefTrackAndSystem(ref_tn, SystemType::Mlat);

        if (!col_tst_opt.has_value())
        {
            return;
        }

        TrackCollection col_tst = col_tst_opt.value();

        // Iterate through each reference sub-track.
        for (const Track &sub_trk_ref : sub_trk_vec)
        {
            if (sub_trk_ref.duration() < 1)
            {
                continue;
            }

            Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

            double threshold = 3.0;
            if (narea.area_ == Aerodrome::Area::Stand)
            {
                threshold = 15.0;
            }

            QDateTime last_tod;
            bool first = true;

            // Iterate through each track in the test data collection.
            for (const Track &trk_tst : col_tst)
            {
                if (!haveTimeIntersection(trk_tst, sub_trk_ref))
                {
                    continue;
                }

                // Extract TST sub track that matches in time with the REF sub track.
                Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

                // Iterate through each target report in the TST sub track.
                for (const TargetReport &tr : sub_trk_tst)
                {
                    QDateTime new_tod = tr.tod_;

                    if (first)
                    {
                        last_tod = new_tod;
                        ++mlatPlg_[narea].n_tr_;

                        first = false;
                        continue;
                    }

                    double tdiff = last_tod.msecsTo(new_tod) / 1000.0;
                    if (tdiff >= threshold)
                    {
                        ++mlatPlg_[narea].n_g_;
                    }

                    last_tod = new_tod;
                    ++mlatPlg_[narea].n_tr_;
                }
            }
        }
    }
}
