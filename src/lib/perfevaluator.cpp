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
    trkAssoc.addData(t);
}

void PerfEvaluator::run()
{
    // Run track association.
    trkAssoc.run();

    // Set PIC threshold value.
    computePicThreshold(95);

    // Iterate through each target set.
    for (const TrackCollectionSet &s : qAsConst(trkAssoc.sets()))
    {
        //ModeS mode_s = s.mode_s();
        TrackCollection c_ref = s.refTrackCol();

        // Iterate through each track in the reference collection.
        for (const Track &t_ref : c_ref)
        {
            TrackNum ref_tn = t_ref.track_number();
            QVector<TrackCollection> c_tst = s.matchesForRefTrack(ref_tn);

            // Iterate through each test collection that matches with the
            // reference track.
            for (const TrackCollection &c_tst_i : c_tst)
            {
                if (c_tst_i.system_type() == SystemType::Smr)
                {
                    // SMR ED-116.

                    evalED116RPA(t_ref, c_tst_i);
                    evalED116UR(t_ref, c_tst_i);
                    evalED116PD(t_ref, c_tst_i);
                }
                else if (c_tst_i.system_type() == SystemType::Mlat)
                {
                    // MLAT ED-117.

                    evalED117RPA(t_ref, c_tst_i);
                    evalED117UR(t_ref, c_tst_i);
                    evalED117PD(t_ref, c_tst_i);
                    evalED117PFD(t_ref, c_tst_i);
                    evalED117PID(t_ref, c_tst_i);
                    evalED117PFID(t_ref, c_tst_i);
                }
            }
        }

        if (s.collection(SystemType::Mlat).has_value())
        {
            evalED117PLG(s.collection(SystemType::Mlat).value());
        }
    }
}

void PerfEvaluator::printPosAccResultsSmr() const
{
    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::All;

    auto printStats = [](AreaHash<QVector<double>>::const_iterator it, const QVector<double> &errors) {
        qDebug() << it.key().area_ << it.key().name_
                 << "\t"
                 << "P95:"
                 << "\t" << percentile(errors, 95) << "m"
                 << "\t"
                 << "P99:"
                 << "\t" << percentile(errors, 99) << "m"
                 << "\t"
                 << "Mean:"
                 << "\t" << mean(errors) << "m"
                 << "\t"
                 << "StdDev:"
                 << "\t" << stdDev(errors) << "m"
                 << "\t"
                 << "N:"
                 << "\t" << errors.size();
    };

    qDebug() << "--------------- SMR ---------------";
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = smrRpaErrors_.findByArea(area);

        QVector<double> errorsTotal;
        for (const auto &it : subAreas)
        {
            QVector<double> errors = it.value();
            errorsTotal.append(errors);

            printStats(it, errors);
        }

        qDebug() << area
                 << "\t"
                 << "P95:"
                 << "\t" << percentile(errorsTotal, 95) << "m"
                 << "\t"
                 << "P99:"
                 << "\t" << percentile(errorsTotal, 99) << "m"
                 << "\t"
                 << "Mean:"
                 << "\t" << mean(errorsTotal) << "m"
                 << "\t"
                 << "StdDev:"
                 << "\t" << stdDev(errorsTotal) << "m"
                 << "\t"
                 << "N:"
                 << "\t" << errorsTotal.size();
    }
}

void PerfEvaluator::printPosAccResultsMlat() const
{
    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering
          << Aerodrome::Area::Apron
          << Aerodrome::Area::Airborne;

    auto printStats = [](AreaHash<QVector<double>>::const_iterator it, const QVector<double> &errors) {
        qDebug() << it.key().area_ << it.key().name_
                 << "\t"
                 << "P95:"
                 << "\t" << percentile(errors, 95) << "m"
                 << "\t"
                 << "P99:"
                 << "\t" << percentile(errors, 99) << "m"
                 << "\t"
                 << "Mean:"
                 << "\t" << mean(errors) << "m"
                 << "\t"
                 << "StdDev:"
                 << "\t" << stdDev(errors) << "m"
                 << "\t"
                 << "N:"
                 << "\t" << errors.size();
    };

    qDebug() << "--------------- MLAT ---------------";
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatRpaErrors_.findByArea(area);

        QVector<double> errorsTotal;
        for (const auto &it : subAreas)
        {
            QVector<double> errors = it.value();
            errorsTotal.append(errors);

            printStats(it, errors);
        }

        qDebug() << area
                 << "\t"
                 << "P95:"
                 << "\t" << percentile(errorsTotal, 95) << "m"
                 << "\t"
                 << "P99:"
                 << "\t" << percentile(errorsTotal, 99) << "m"
                 << "\t"
                 << "Mean:"
                 << "\t" << mean(errorsTotal) << "m"
                 << "\t"
                 << "StdDev:"
                 << "\t" << stdDev(errorsTotal) << "m"
                 << "\t"
                 << "N:"
                 << "\t" << errorsTotal.size();
    }
}

void PerfEvaluator::computePicThreshold(double prctl)
{
    QVector<double> vec;
    for (const TrackCollectionSet &s : qAsConst(trkAssoc.sets()))
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

    qDebug() << "Before:" << trk_out.data().size();

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

    qDebug() << "After:" << trk_out.data().size();

    return trk_out;
}

void PerfEvaluator::evalED116RPA(const Track &trk_ref, const TrackCollection &col_tst)
{
    // Iterate through each test track in the collection.
    for (const Track &trk_tst : col_tst)
    {
        if (!haveSpaceIntersection(trk_tst, trk_ref))
        {
            return;
        }

        // Only keep target reports with MOPS version 2 and PIC above the
        // 95th percentile threshold.
        Track t_r = filterTrackByQuality(trk_ref, 2, pic_p95);

        // Extract TST track portion that matches in time with the
        // reference track.
        //Track t_t = intersect(t_tst, t_r).value();

        // Interpolate the TST track at the times of the REF track points.
        Track t_t = resample(trk_tst, t_r.timestamps());

        // TST and REF trajectories should match in size.
        //Q_ASSERT(t_t.size() == t_r.size());

        // TST and REF pairs should have exact same times.
        //Q_ASSERT(t_t.data().keys() == t_r.data().keys());

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

void PerfEvaluator::evalED116UR(const Track &trk_ref, const TrackCollection &col_tst)
{
    QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

    for (const Track &sub_trk_ref : sub_trk_vec)
    {
        if (sub_trk_ref.size() < 2)
        {
            continue;
        }

        // Iterate through each test track in the collection.
        for (const Track &trk_tst : col_tst)
        {
            if (!haveTimeIntersection(trk_tst, sub_trk_ref))
            {
                return;
            }

            // Extract TST track portion that matches in time with the
            // reference track.
            Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

            Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

            double dur = sub_trk_tst.duration();
            double freq = 1.0;
            int n_etrp = qFloor(dur * freq);

            smrUr_[narea].n_trp_ += sub_trk_tst.size();
            smrUr_[narea].n_etrp_ += n_etrp;
        }
    }
}

void PerfEvaluator::evalED116PD(const Track &trk_ref, const TrackCollection &col_tst)
{
    auto hasPosition = [](const TargetReport &tr) {
        return !qIsNaN(tr.x_) && !qIsNaN(tr.y_);
    };

    QVector<Track> trk_ref_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

    for (const Track &trk_ref_i : trk_ref_vec)
    {
        if (trk_ref_i.duration() < 1.0)
        {
            return;
        }

        Aerodrome::NamedArea narea = trk_ref_i.begin()->narea_;

        double period = 1.0;

        Counters::IntervalCounter intervalCtr(period, trk_ref_i.beginTimestamp());

        for (const Track &trk_tst : col_tst)
        {
            if (!haveSpaceTimeIntersection(trk_tst, trk_ref_i))
            {
                return;
            }

            Track trk_tst_i = intersect(trk_tst, trk_ref_i).value();

            for (const TargetReport &tr : trk_tst_i)
            {
                if (hasPosition(tr))
                {
                    intervalCtr.update(tr.tod_);
                }
            }

            intervalCtr.finish(trk_ref_i.endTimestamp());

            Counters::BasicCounter ctr = intervalCtr.read();

            smrPd_[narea].n_trp_ += ctr.valid_;
            smrPd_[narea].n_up_ += ctr.total_;
        }
    }
}

void PerfEvaluator::evalED116PFD(const Track &trk_ref, const TrackCollection &col_tst)
{
    Q_UNUSED(trk_ref);
    Q_UNUSED(col_tst);
}

void PerfEvaluator::evalED117RPA(const Track &trk_ref, const TrackCollection &col_tst)
{
    // Iterate through each test track in the collection.
    for (const Track &trk_tst : col_tst)
    {
        if (!haveSpaceIntersection(trk_tst, trk_ref))
        {
            return;
        }

        // Only keep target reports with MOPS version 2 and PIC above the
        // 95th percentile threshold.
        Track t_r = filterTrackByQuality(trk_ref, 2, pic_p95);

        // Extract TST track portion that matches in time with the
        // reference track.
        //Track t_t = intersect(t_tst, t_r).value();

        // Interpolate the TST track at the times of the REF track points.
        Track t_t = resample(trk_tst, t_r.timestamps());

        // TST and REF trajectories should match in size.
        //Q_ASSERT(t_t.size() == t_r.size());

        // TST and REF pairs should have exact same times.
        //Q_ASSERT(t_t.data().keys() == t_r.data().keys());

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

void PerfEvaluator::evalED117UR(const Track &trk_ref, const TrackCollection &col_tst)
{
    QVector<Track> sub_trk_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

    for (const Track &sub_trk_ref : sub_trk_vec)
    {
        if (sub_trk_ref.size() < 2)
        {
            continue;
        }

        // Iterate through each test track in the collection.
        for (const Track &trk_tst : col_tst)
        {
            if (!haveTimeIntersection(trk_tst, sub_trk_ref))
            {
                return;
            }

            // Extract TST track portion that matches in time with the
            // reference track.
            Track sub_trk_tst = intersect(trk_tst, sub_trk_ref).value();

            Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

            double dur = sub_trk_tst.duration();
            double freq = 1.0;
            int n_etrp = qFloor(dur * freq);

            mlatUr_[narea].n_trp_ += sub_trk_tst.size();
            mlatUr_[narea].n_etrp_ += n_etrp;
        }
    }
}

void PerfEvaluator::evalED117PD(const Track &trk_ref, const TrackCollection &col_tst)
{
    auto hasPosition = [](const TargetReport &tr) {
        return !qIsNaN(tr.x_) && !qIsNaN(tr.y_);
    };

    QVector<Track> trk_ref_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

    for (const Track &trk_ref_i : trk_ref_vec)
    {
        if (trk_ref_i.duration() < 1.0)
        {
            return;
        }

        Aerodrome::NamedArea narea = trk_ref_i.begin()->narea_;

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

        Counters::IntervalCounter intervalCtr(period, trk_ref_i.beginTimestamp());

        for (const Track &trk_tst : col_tst)
        {
            if (!haveSpaceTimeIntersection(trk_tst, trk_ref_i))
            {
                return;
            }

            Track trk_tst_i = intersect(trk_tst, trk_ref_i).value();

            for (const TargetReport &tr : trk_tst_i)
            {
                if (hasPosition(tr))
                {
                    intervalCtr.update(tr.tod_);
                }
            }

            intervalCtr.finish(trk_ref_i.endTimestamp());

            Counters::BasicCounter ctr = intervalCtr.read();

            mlatPd_[narea].n_trp_ += ctr.valid_;
            mlatPd_[narea].n_up_ += ctr.total_;
        }
    }
}

void PerfEvaluator::evalED117PFD(const Track &trk_ref, const TrackCollection &col_tst)
{
    QVector<Track> trk_ref_vec = splitTrackByArea(trk_ref, TrackSplitMode::SplitByNamedArea);

    for (const Track &trk_ref_i : trk_ref_vec)
    {
        Aerodrome::NamedArea narea = trk_ref_i.begin()->narea_;

        for (const Track &trk_tst : col_tst)
        {
            if (!haveSpaceTimeIntersection(trk_tst, trk_ref_i))
            {
                return;
            }

            //Track trk_tst_i = intersect(trk_tst, trk_ref_i).value();

            // Interpolate the REF track at the times of the TST track plots.
            const Track t_r = resample(trk_ref_i, trk_tst.timestamps());

            // Calculate Euclidean distance between TST-REF pairs.
            QVector<QPair<TargetReport, double>> dists = euclideanDistance(t_r.data(), trk_tst.data());

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

void PerfEvaluator::evalED117PID(const Track &trk_ref, const TrackCollection &col_tst)
{
    // Iterate through each test track in the collection.
    for (const Track &trk_tst : col_tst)
    {
        if (!haveSpaceIntersection(trk_tst, trk_ref))
        {
            return;
        }

        const QMultiMap<QDateTime, TargetReport> &ref_data = trk_ref.data();

        for (const TargetReport &tr_tst : trk_tst)
        {
            if (!tr_tst.ident_.has_value() && !tr_tst.mode_3a_.has_value())
            {
                // Skip if target report has no identification and no mode 3/A code.
                continue;
            }

            QDateTime tod = tr_tst.tod_;

            TgtRepMap::const_iterator it_u = ref_data.end();  // Upper.
            TgtRepMap::const_iterator it_l = ref_data.end();  // Lower.

            TgtRepMap::const_iterator it = ref_data.lowerBound(tod);
            if (it != ref_data.end())  // Upper TOD found.
            {
                Q_ASSERT(it.key() >= tod);

                // Save upper value.
                it_u = it;

                // Search lower values by decrementing iterator.
                while (it != ref_data.end() && tod < it.key())
                {
                    if (it == ref_data.begin())  // Exit condition on first value.
                    {
                        if (tod < it.key())  // Set as not found.
                        {
                            it = ref_data.end();
                        }

                        break;
                    }

                    it--;
                }

                if (it != ref_data.end())  // Lower TOD found.
                {
                    Q_ASSERT(tod >= it.key());

                    // Save lower value.
                    it_l = it;
                }

                if (it_l != ref_data.end() && it_u != ref_data.end())
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

void PerfEvaluator::evalED117PFID(const Track &trk_ref, const TrackCollection &col_tst)
{
    // Iterate through each test track in the collection.
    for (const Track &trk_tst : col_tst)
    {
        if (!haveSpaceIntersection(trk_tst, trk_ref))
        {
            return;
        }

        const QMultiMap<QDateTime, TargetReport> &ref_data = trk_ref.data();

        for (const TargetReport &tr_tst : trk_tst)
        {
            if (!tr_tst.ident_.has_value() && !tr_tst.mode_3a_.has_value())
            {
                // Skip if target report has no identification and no mode 3/A code.
                continue;
            }

            QDateTime tod = tr_tst.tod_;

            TgtRepMap::const_iterator it_u = ref_data.end();  // Upper.
            TgtRepMap::const_iterator it_l = ref_data.end();  // Lower.

            TgtRepMap::const_iterator it = ref_data.lowerBound(tod);
            if (it != ref_data.end())  // Upper TOD found.
            {
                Q_ASSERT(it.key() >= tod);

                // Save upper value.
                it_u = it;

                // Search lower values by decrementing iterator.
                while (it != ref_data.end() && tod < it.key())
                {
                    if (it == ref_data.begin())  // Exit condition on first value.
                    {
                        if (tod < it.key())  // Set as not found.
                        {
                            it = ref_data.end();
                        }

                        break;
                    }

                    it--;
                }

                if (it != ref_data.end())  // Lower TOD found.
                {
                    Q_ASSERT(tod >= it.key());

                    // Save lower value.
                    it_l = it;
                }

                if (it_l != ref_data.end() && it_u != ref_data.end())
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

void PerfEvaluator::evalED117PLG(const TrackCollection &col_tst)
{
    QDateTime last_tod;
    bool first = true;
    for (const Track &trk : col_tst)
    {
        QVector<Track> sub_trk_vec = splitTrackByArea(trk, TrackSplitMode::SplitByNamedArea);

        for (const Track &sub_trk : sub_trk_vec)
        {
            for (const TargetReport &tr : sub_trk)
            {
                Aerodrome::NamedArea narea = tr.narea_;
                QDateTime new_tod = tr.tod_;

                if (first)
                {
                    last_tod = new_tod;
                    ++mlatPlg_[narea].n_tr_;

                    first = false;
                    continue;
                }

                double threshold = 3.0;
                if (narea.area_ == Aerodrome::Area::Stand)
                {
                    threshold = 15.0;
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
