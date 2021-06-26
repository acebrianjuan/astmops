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
            QVector<TrackCollection> c_tst = s.matchesForRefTrack(ref_tn);

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
                        evalED117RPA(t_ref, t_tst);
                        //evalED117PD(t_ref, t_tst);
                        evalED117PID(t_ref, t_tst);
                    }
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

void PerfEvaluator::evalED116RPA(const Track &trk_ref, const Track &trk_tst)
{
    if (!haveSpaceIntersection(trk_tst, trk_ref))
    {
        return;
    }

    // Make a copy of the reference track.
    Track t_r = trk_ref;

    qDebug() << "Before:" << t_r.data().size();
    TgtRepMap::iterator it = t_r.begin();
    while (it != t_r.end())
    {
        TargetReport tr = it.value();

        if (tr.ver_.has_value() && tr.pic_.has_value())
        {
            if (tr.ver_.value() != 2 || tr.pic_.value() < pic_p95)
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
        Aerodrome::NamedArea narea = p.first.area_;
        double dist = p.second;
        smrRpaErrors_[narea] << dist;
    }
}

void PerfEvaluator::evalED116PD(const Track &trk_ref, const TrackCollection &col_tst)
{
    if (trk_ref.duration() < 1.0)
    {
        return;
    }

    //

    for (const Track &trk_tst : col_tst)
    {
        if (!haveSpaceIntersection(trk_tst, trk_ref))
        {
            return;
        }
    }
}

void PerfEvaluator::evalED117RPA(const Track &trk_ref, const Track &trk_tst)
{
    if (!haveSpaceIntersection(trk_tst, trk_ref))
    {
        return;
    }

    // Make a copy of the reference track.
    Track t_r = trk_ref;

    qDebug() << "Before:" << t_r.data().size();
    TgtRepMap::iterator it = t_r.begin();
    while (it != t_r.end())
    {
        TargetReport tr = it.value();

        if (tr.ver_.has_value() && tr.pic_.has_value())
        {
            if (tr.ver_.value() != 2 || tr.pic_.value() < pic_p95)
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
        Aerodrome::NamedArea narea = p.first.area_;
        double dist = p.second;
        mlatRpaErrors_[narea] << dist;
    }
}

void PerfEvaluator::evalED117PD(const Track &trk_ref, const TrackCollection &col_tst)
{
    if (trk_ref.duration() < 1.0)
    {
        return;
    }

    //

    for (const Track &trk_tst : col_tst)
    {
        if (!haveSpaceIntersection(trk_tst, trk_ref))
        {
            return;
        }
    }
}

void PerfEvaluator::evalED117PID(const Track &trk_ref, const Track &trk_tst)
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

        QMultiMap<QDateTime, TargetReport>::const_iterator it_u = ref_data.end();  // Upper.
        QMultiMap<QDateTime, TargetReport>::const_iterator it_l = ref_data.end();  // Lower.

        QMultiMap<QDateTime, TargetReport>::const_iterator it = ref_data.lowerBound(tod);
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

                    ++mlatPidIdentCounter_.total;
                    if (ident_ok)
                    {
                        ++mlatPidIdentCounter_.correct;
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

                    ++mlatPidMode3ACounter_.total;
                    if (mode_3a_ok)
                    {
                        ++mlatPidMode3ACounter_.correct;
                    }
                }
            }
        }
    }
}

void PerfEvaluator::evalED117PFID(const Track &trk_ref, const Track &trk_tst)
{
    if (!haveSpaceIntersection(trk_tst, trk_ref))
    {
        return;
    }
}

void PerfEvaluator::evalED117PLG(const TrackCollection &col_tst)
{
    QDateTime last_tod;
    bool first = true;
    for (const Track &trk : col_tst)
    {
        QVector<Track> sub_trk_vec = splitTrackByArea(trk);

        for (const Track &sub_trk : sub_trk_vec)
        {
            for (const TargetReport &tr : sub_trk)
            {
                Aerodrome::NamedArea narea = tr.area_;
                QDateTime new_tod = tr.tod_;

                if (first)
                {
                    last_tod = new_tod;
                    ++plg_[narea].n_tr;

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
                    ++plg_[narea].n_gaps;
                }

                last_tod = new_tod;
                ++plg_[narea].n_tr;
            }
        }
    }
}
