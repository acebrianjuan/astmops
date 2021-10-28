/*!
 * \file perfevaluator.h
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

#include "perfevaluator.h"
#include "config.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QMetaEnum>

#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
#include <QTextStream>
namespace Qt
{
static const auto &endl = &::endl;
static const auto &left = &::left;
static const auto &center = &::center;
static const auto &right = &::right;
}  // namespace Qt
#endif

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
    computePicThreshold(Configuration::rpaPicPercentile());

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

    // Print results.

    const QStringList args = QCoreApplication::arguments();
    const int idx = args.indexOf(QLatin1String("--json"));

    if (idx != -1)  // Print JSON output.
    {
        QJsonObject jsonRoot;
        jsonRoot.insert(QLatin1String("type"), QLatin1String("astmops 1.0"));

        jsonRoot.insert(QLatin1String("ED116RPA"), printED116RPA_json());
        jsonRoot.insert(QLatin1String("ED116UR"), printED116UR_json());
        jsonRoot.insert(QLatin1String("ED116PD"), printED116PD_json());
        jsonRoot.insert(QLatin1String("ED116PFD"), printED116PFD_json());

        jsonRoot.insert(QLatin1String("ED117RPA"), printED117RPA_json());
        jsonRoot.insert(QLatin1String("ED117UR"), printED117UR_json());
        jsonRoot.insert(QLatin1String("ED117PD"), printED117PD_json());
        jsonRoot.insert(QLatin1String("ED117PFD"), printED117PFD_json());
        jsonRoot.insert(QLatin1String("ED117PID_Ident"), printED117PID_Ident_json());
        jsonRoot.insert(QLatin1String("ED117PID_Mode3A"), printED117PID_Mode3A_json());
        jsonRoot.insert(QLatin1String("ED117PFID_Ident"), printED117PFID_Ident_json());
        jsonRoot.insert(QLatin1String("ED117PFID_Mode3A"), printED117PFID_Mode3A_json());
        jsonRoot.insert(QLatin1String("ED117PLG"), printED117PLG_json());

        QJsonDocument document(jsonRoot);
        QTextStream(stdout) << QString::fromLatin1(document.toJson(QJsonDocument::Indented));
    }
    else  // Print plain text tables.
    {
        printED116RPA();
        printED116UR();
        printED116PD();
        printED116PFD();

        printED117RPA();
        printED117UR();
        printED117PD();
        printED117PFD();
        printED117PID_Ident();
        printED117PID_Mode3A();
        printED117PFID_Ident();
        printED117PFID_Mode3A();
        printED117PLG();
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

                    // Local copy of the TST track.
                    Track t_t = trk_tst;

                    // Local copy of the REF sub-track.
                    // Only keep target reports with MOPS version 2 and PIC above the
                    // 95th percentile threshold.
                    Track t_r = filterTrackByQuality(sub_trk_ref, 2, pic_p95_);

                    if (t_r.isEmpty())
                    {
                        continue;
                    }

                    // Resample the REF sub-track at the times of the TST track.
                    t_r = resample(t_r, t_t.timestamps());

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
                    // REF sub-track.
                    std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                    if (!sub_trk_tst_opt.has_value())
                    {
                        continue;
                    }

                    Track sub_trk_tst = sub_trk_tst_opt.value();

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
                    std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                    if (!sub_trk_tst_opt.has_value())
                    {
                        continue;
                    }

                    Track sub_trk_tst = sub_trk_tst_opt.value();

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

                // Note to avoid confusion:
                // Beware that, unlike the actual target reports count, here
                // we are overwriting the updates count and expected target
                // reports count with updated values from the traffic periods
                // counter after each track insertion, so no data is being
                // duplicated.
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
                    std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                    if (!sub_trk_tst_opt.has_value())
                    {
                        continue;
                    }

                    Track sub_trk_tst = sub_trk_tst_opt.value();

                    smrPfd_[narea].n_tr_ += sub_trk_tst.size();
                }
            }
        }
        else
        {
            for (const Track &sub_trk_ref : sub_trk_vec)
            {
                if (sub_trk_ref.duration() < 1)
                {
                    continue;
                }

                Aerodrome::NamedArea narea = sub_trk_ref.begin()->narea_;

                // Note to avoid confusion:
                // Beware that, unlike the actual target reports count, here
                // we are overwriting the updates count and expected target
                // reports count with updated values from the traffic periods
                // counter after each track insertion, so no data is being
                // duplicated.
                trafficPeriods_[narea] << sub_trk_ref;
                smrPfd_[narea].n_u_ = trafficPeriods_[narea].expectedUpdates(freq);
                smrPfd_[narea].n_etr_ = trafficPeriods_[narea].expectedTgtReps(freq);
            }
        }
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

                    // Local copy of the TST track.
                    Track t_t = trk_tst;

                    // Local copy of the REF sub-track.
                    // Only keep target reports with MOPS version 2 and PIC above the
                    // 95th percentile threshold.
                    Track t_r = filterTrackByQuality(sub_trk_ref, 2, pic_p95_);

                    if (t_r.isEmpty())
                    {
                        continue;
                    }

                    // Resample the REF sub-track at the times of the TST track.
                    t_r = resample(t_r, t_t.timestamps());

                    // On Stand, average TST track positions over a period of 5 s.
                    if (narea.area_ == Aerodrome::Stand)
                    {
                        t_t = average(t_t, 5.0);
                    }

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
                    std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                    if (!sub_trk_tst_opt.has_value())
                    {
                        continue;
                    }

                    Track sub_trk_tst = sub_trk_tst_opt.value();

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
        Aerodrome::Area area = narea.area_;

        double period = 2.0;
        if (area == Aerodrome::Area::Stand)
        {
            period = 5.0;
        }
        else if (area == Aerodrome::Area::ApronLane)
        {
            period = 2.0;
        }
        else if (area == Aerodrome::Area::Taxiway)
        {
            period = 2.0;
        }
        else if (area == Aerodrome::Area::Runway)
        {
            period = 1.0;
        }
        else if (area == Aerodrome::Area::Airborne1 ||
                 area == Aerodrome::Area::Airborne2)
        {
            period = 1.0;
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
                    std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                    if (!sub_trk_tst_opt.has_value())
                    {
                        continue;
                    }

                    Track sub_trk_tst = sub_trk_tst_opt.value();

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

                // Resample the REF sub-track at the times of the TST track.
                const Track sub_trk_ref_i = resample(sub_trk_ref, trk_tst.timestamps());

                // Calculate Euclidean distance between TST-REF pairs.
                QVector<QPair<TargetReport, double>> dists = euclideanDistance(sub_trk_ref_i.data(), trk_tst.data());

                for (const QPair<TargetReport, double> &p : dists)
                {
                    double dist = p.second;

                    if (dist > 50.0)
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
                std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                if (!sub_trk_tst_opt.has_value())
                {
                    continue;
                }

                Track sub_trk_tst = sub_trk_tst_opt.value();

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
                std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                if (!sub_trk_tst_opt.has_value())
                {
                    continue;
                }

                Track sub_trk_tst = sub_trk_tst_opt.value();

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
                std::optional<Track> sub_trk_tst_opt = intersect(trk_tst, sub_trk_ref);

                if (!sub_trk_tst_opt.has_value())
                {
                    continue;
                }

                Track sub_trk_tst = sub_trk_tst_opt.value();

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

void PerfEvaluator::printED116RPA() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(56) << "[ ED-116 RPA ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "P95 [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "P99 [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "Mean [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "SDev [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "--------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "--------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;

    auto printStats = [&out, &e](AreaHash<QVector<double>>::const_iterator it, const QVector<double> &errors) {
        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errors, 95) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errors, 99) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << mean(errors) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << stdDev(errors) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << errors.size()
            << qSetFieldWidth(0) << Qt::endl;
    };

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

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errorsTotal, 95) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errorsTotal, 99) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << mean(errorsTotal) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << stdDev(errorsTotal) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << errorsTotal.size()
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED116UR() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-116 UR ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "UR [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;

    auto printStats = [&out, &e](AreaHash<Counters::UrCounter>::const_iterator it, Counters::UrCounter ctr) {
        double ur = ctr.n_trp_ / static_cast<double>(ctr.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << ur * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_trp_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = smrUr_.findByArea(area);

        Counters::UrCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::UrCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_etrp_ += ctr.n_etrp_;

            printStats(it, ctr);
        }

        double ur = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << ur * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_trp_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED116PD() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-116 PD ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "PD [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;

    auto printStats = [&out, &e](AreaHash<Counters::PdCounter>::const_iterator it, Counters::PdCounter ctr) {
        double pd = ctr.n_trp_ / static_cast<double>(ctr.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_trp_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = smrPd_.findByArea(area);

        Counters::PdCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PdCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_up_ += ctr.n_up_;

            printStats(it, ctr);
        }

        double pd = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_trp_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED116PFD() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-116 PFD ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "PFD [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;

    auto printStats = [&out, &e](AreaHash<Counters::PfdCounter2>::const_iterator it, Counters::PfdCounter2 ctr) {
        double pfd = (static_cast<double>(ctr.n_tr_) - static_cast<double>(ctr.n_etr_)) / static_cast<double>(ctr.n_u_);
        if (pfd < 0)
        {
            pfd = 0;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pfd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_tr_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = smrPfd_.findByArea(area);

        TrafficPeriodCollection tpcolTotal;
        Counters::PfdCounter2 ctrTotal;
        for (const auto &it : subAreas)
        {
            Aerodrome::NamedArea narea = it.key();
            tpcolTotal << trafficPeriods_[narea];

            Counters::PfdCounter2 ctr = it.value();
            ctrTotal.n_tr_ += ctr.n_tr_;
            ctrTotal.n_etr_ = tpcolTotal.expectedTgtReps(1.0);
            ctrTotal.n_u_ = tpcolTotal.expectedUpdates(1.0);

            printStats(it, ctr);
        }

        double pfd = (static_cast<double>(ctrTotal.n_tr_) - static_cast<double>(ctrTotal.n_etr_)) / static_cast<double>(ctrTotal.n_u_);
        if (pfd < 0)
        {
            pfd = 0;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pfd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_tr_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117RPA() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(56) << "[ ED-117 RPA ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "P95 [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "P99 [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "Mean [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "SDev [m]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "--------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "--------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<QVector<double>>::const_iterator it, const QVector<double> &errors) {
        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errors, 95) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errors, 99) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << mean(errors) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << stdDev(errors) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << errors.size()
            << qSetFieldWidth(0) << Qt::endl;
    };

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

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errorsTotal, 95) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << percentile(errorsTotal, 99) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << mean(errorsTotal) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << stdDev(errorsTotal) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << errorsTotal.size()
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117UR() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-117 UR ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "UR [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::UrCounter>::const_iterator it, Counters::UrCounter ctr) {
        double ur = ctr.n_trp_ / static_cast<double>(ctr.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << ur * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_trp_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatUr_.findByArea(area);

        Counters::UrCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::UrCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_etrp_ += ctr.n_etrp_;

            printStats(it, ctr);
        }

        double ur = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << ur * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_trp_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117PD() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-117 PD ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "PD [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::PdCounter>::const_iterator it, Counters::PdCounter ctr) {
        double pd = ctr.n_trp_ / static_cast<double>(ctr.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_trp_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatPd_.findByArea(area);

        Counters::PdCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PdCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_up_ += ctr.n_up_;

            printStats(it, ctr);
        }

        double pd = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_trp_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117PFD() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-117 PFD ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "PFD [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::PfdCounter>::const_iterator it, Counters::PfdCounter ctr) {
        double pfd = ctr.n_ftr_ / static_cast<double>(ctr.n_tr_);
        if (pfd > 1)
        {
            pfd = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pfd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_tr_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatPfd_.findByArea(area);

        Counters::PfdCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PfdCounter ctr = it.value();
            ctrTotal.n_ftr_ += ctr.n_ftr_;
            ctrTotal.n_tr_ += ctr.n_tr_;

            printStats(it, ctr);
        }

        double pfd = ctrTotal.n_ftr_ / static_cast<double>(ctrTotal.n_tr_);
        if (pfd > 1)
        {
            pfd = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pfd * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_tr_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117PID_Ident() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-117 PID (IDENT) ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "PID [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::PidCounter>::const_iterator it, Counters::PidCounter ctr) {
        double pid = ctr.n_citr_ / static_cast<double>(ctr.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_itr_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatPidIdent_.findByArea(area);

        Counters::PidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PidCounter ctr = it.value();
            ctrTotal.n_citr_ += ctr.n_citr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            printStats(it, ctr);
        }

        double pid = ctrTotal.n_citr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_itr_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117PID_Mode3A() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-117 PID (MODE3A) ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "PID [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::PidCounter>::const_iterator it, Counters::PidCounter ctr) {
        double pid = ctr.n_citr_ / static_cast<double>(ctr.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_itr_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatPidMode3A_.findByArea(area);

        Counters::PidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PidCounter ctr = it.value();
            ctrTotal.n_citr_ += ctr.n_citr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            printStats(it, ctr);
        }

        double pid = ctrTotal.n_citr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << pid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_itr_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117PFID_Ident() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(31) << "[ ED-117 PFID (IDENT) ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "PFID [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "--------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::PfidCounter>::const_iterator it, Counters::PfidCounter ctr) {
        double pfid = ctr.n_eitr_ / static_cast<double>(ctr.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << pfid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_itr_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatPfidIdent_.findByArea(area);

        Counters::PfidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PfidCounter ctr = it.value();
            ctrTotal.n_eitr_ += ctr.n_eitr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            printStats(it, ctr);
        }

        double pfid = ctrTotal.n_eitr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << pfid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_itr_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117PFID_Mode3A() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(31) << "[ ED-117 PFID (MODE3A) ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "PFID [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(8) << "--------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::PfidCounter>::const_iterator it, Counters::PfidCounter ctr) {
        double pfid = ctr.n_eitr_ / static_cast<double>(ctr.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << pfid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_itr_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatPfidMode3A_.findByArea(area);

        Counters::PfidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PfidCounter ctr = it.value();
            ctrTotal.n_eitr_ += ctr.n_eitr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            printStats(it, ctr);
        }

        double pfid = ctrTotal.n_eitr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(8) << Qt::right << pfid * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_itr_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

void PerfEvaluator::printED117PLG() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QTextStream out(stdout);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::FixedNotation);

    out << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignCenter);
    out.setPadChar(QLatin1Char('-'));
    out << qSetFieldWidth(30) << "[ ED-117A PLG ]" << qSetFieldWidth(0) << Qt::endl;
    out.setPadChar(QLatin1Char(' '));

    out << qSetFieldWidth(15) << "AREA" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "PLG [%]" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "N"
        << qSetFieldWidth(0) << Qt::endl;

    out.setFieldAlignment(QTextStream::AlignRight);

    out << qSetFieldWidth(15) << "---------------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(7) << "-------" << qSetFieldWidth(1) << ""
        << qSetFieldWidth(6) << "------"
        << qSetFieldWidth(0) << Qt::endl;

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;

    auto printStats = [&out, &e](AreaHash<Counters::PlgCounter>::const_iterator it, Counters::PlgCounter ctr) {
        double plg = ctr.n_g_ / static_cast<double>(ctr.n_tr_);
        if (plg > 1)
        {
            plg = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << it.key().fullName() << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << plg * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctr.n_tr_
            << qSetFieldWidth(0) << Qt::endl;
    };

    for (const Aerodrome::Area area : qAsConst(areas))
    {
        const auto subAreas = mlatPlg_.findByArea(area);

        Counters::PlgCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PlgCounter ctr = it.value();
            ctrTotal.n_g_ += ctr.n_g_;
            ctrTotal.n_tr_ += ctr.n_tr_;

            printStats(it, ctr);
        }

        double plg = ctrTotal.n_g_ / static_cast<double>(ctrTotal.n_tr_);
        if (plg > 1)
        {
            plg = 1;
        }

        out << qSetFieldWidth(15) << Qt::left << e.valueToKey(area) << qSetFieldWidth(1) << ""
            << qSetFieldWidth(7) << Qt::right << plg * 100.0 << qSetFieldWidth(1) << ""
            << qSetFieldWidth(6) << Qt::right << ctrTotal.n_tr_
            << qSetFieldWidth(0) << Qt::endl;

        out << Qt::endl;
    }
}

QJsonObject PerfEvaluator::printED116RPA_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;


    auto sa_results = [](const QVector<double> &errors) {
        QJsonObject obj;
        obj.insert(QLatin1String("P95"), percentile(errors, 95));
        obj.insert(QLatin1String("P99"), percentile(errors, 99));
        obj.insert(QLatin1String("Mean"), mean(errors));
        obj.insert(QLatin1String("StdDev"), stdDev(errors));
        obj.insert(QLatin1String("N"), errors.size());

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = smrRpaErrors_.findByArea(area);

        QJsonObject subAreasObj;
        QVector<double> errorsTotal;
        for (const auto &it : subAreas)
        {
            QVector<double> errors = it.value();
            errorsTotal.append(errors);

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(errors));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        areaObj.insert(QLatin1String("P95"), percentile(errorsTotal, 95));
        areaObj.insert(QLatin1String("P99"), percentile(errorsTotal, 99));
        areaObj.insert(QLatin1String("Mean"), mean(errorsTotal));
        areaObj.insert(QLatin1String("StdDev"), stdDev(errorsTotal));
        areaObj.insert(QLatin1String("N"), errorsTotal.size());


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED116UR_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;


    auto sa_results = [](Counters::UrCounter ctr) {
        double ur = ctr.n_trp_ / static_cast<double>(ctr.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("UR"), ur * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_trp_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = smrUr_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::UrCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::UrCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_etrp_ += ctr.n_etrp_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double ur = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        areaObj.insert(QLatin1String("UR"), ur * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_trp_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED116PD_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;


    auto sa_results = [](Counters::PdCounter ctr) {
        double pd = ctr.n_trp_ / static_cast<double>(ctr.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PD"), pd * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_trp_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = smrPd_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PdCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PdCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_up_ += ctr.n_up_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pd = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        areaObj.insert(QLatin1String("PD"), pd * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_trp_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED116PFD_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering;


    auto sa_results = [](Counters::PfdCounter2 ctr) {
        double pfd = (static_cast<double>(ctr.n_tr_) - static_cast<double>(ctr.n_etr_)) / static_cast<double>(ctr.n_u_);
        if (pfd < 0)
        {
            pfd = 0;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PFD"), pfd * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_tr_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = smrPfd_.findByArea(area);

        QJsonObject subAreasObj;
        TrafficPeriodCollection tpcolTotal;
        Counters::PfdCounter2 ctrTotal;
        for (const auto &it : subAreas)
        {
            Aerodrome::NamedArea narea = it.key();
            tpcolTotal << trafficPeriods_[narea];

            Counters::PfdCounter2 ctr = it.value();
            ctrTotal.n_tr_ += ctr.n_tr_;
            ctrTotal.n_etr_ = tpcolTotal.expectedTgtReps(1.0);
            ctrTotal.n_u_ = tpcolTotal.expectedUpdates(1.0);

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pfd = (static_cast<double>(ctrTotal.n_tr_) - static_cast<double>(ctrTotal.n_etr_)) / static_cast<double>(ctrTotal.n_u_);
        if (pfd < 0)
        {
            pfd = 0;
        }

        areaObj.insert(QLatin1String("PFD"), pfd * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_tr_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117RPA_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](const QVector<double> &errors) {
        QJsonObject obj;
        obj.insert(QLatin1String("P95"), percentile(errors, 95));
        obj.insert(QLatin1String("P99"), percentile(errors, 99));
        obj.insert(QLatin1String("Mean"), mean(errors));
        obj.insert(QLatin1String("StdDev"), stdDev(errors));
        obj.insert(QLatin1String("N"), errors.size());

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatRpaErrors_.findByArea(area);

        QJsonObject subAreasObj;
        QVector<double> errorsTotal;
        for (const auto &it : subAreas)
        {
            QVector<double> errors = it.value();
            errorsTotal.append(errors);

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(errors));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        areaObj.insert(QLatin1String("P95"), percentile(errorsTotal, 95));
        areaObj.insert(QLatin1String("P99"), percentile(errorsTotal, 99));
        areaObj.insert(QLatin1String("Mean"), mean(errorsTotal));
        areaObj.insert(QLatin1String("StdDev"), stdDev(errorsTotal));
        areaObj.insert(QLatin1String("N"), errorsTotal.size());


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117UR_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::UrCounter ctr) {
        double ur = ctr.n_trp_ / static_cast<double>(ctr.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("UR"), ur * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_trp_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatUr_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::UrCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::UrCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_etrp_ += ctr.n_etrp_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double ur = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_etrp_);
        if (ur > 1)
        {
            ur = 1;
        }

        areaObj.insert(QLatin1String("UR"), ur * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_trp_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117PD_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::PdCounter ctr) {
        double pd = ctr.n_trp_ / static_cast<double>(ctr.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PD"), pd * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_trp_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatPd_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PdCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PdCounter ctr = it.value();
            ctrTotal.n_trp_ += ctr.n_trp_;
            ctrTotal.n_up_ += ctr.n_up_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pd = ctrTotal.n_trp_ / static_cast<double>(ctrTotal.n_up_);
        if (pd > 1)
        {
            pd = 1;
        }

        areaObj.insert(QLatin1String("PD"), pd * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_trp_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117PFD_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::PfdCounter ctr) {
        double pfd = ctr.n_ftr_ / static_cast<double>(ctr.n_tr_);
        if (pfd > 1)
        {
            pfd = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PFD"), pfd * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_tr_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatPfd_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PfdCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PfdCounter ctr = it.value();
            ctrTotal.n_ftr_ += ctr.n_ftr_;
            ctrTotal.n_tr_ += ctr.n_tr_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pfd = ctrTotal.n_ftr_ / static_cast<double>(ctrTotal.n_tr_);
        if (pfd > 1)
        {
            pfd = 1;
        }

        areaObj.insert(QLatin1String("PFD"), pfd * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_tr_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117PID_Ident_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::PidCounter ctr) {
        double pid = ctr.n_citr_ / static_cast<double>(ctr.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PID"), pid * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_itr_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatPidIdent_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PidCounter ctr = it.value();
            ctrTotal.n_citr_ += ctr.n_citr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pid = ctrTotal.n_citr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        areaObj.insert(QLatin1String("PID"), pid * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_itr_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117PID_Mode3A_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::PidCounter ctr) {
        double pid = ctr.n_citr_ / static_cast<double>(ctr.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PID"), pid * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_itr_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatPidMode3A_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PidCounter ctr = it.value();
            ctrTotal.n_citr_ += ctr.n_citr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pid = ctrTotal.n_citr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pid > 1)
        {
            pid = 1;
        }

        areaObj.insert(QLatin1String("PID"), pid * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_itr_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117PFID_Ident_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::PfidCounter ctr) {
        double pfid = ctr.n_eitr_ / static_cast<double>(ctr.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PFID"), pfid * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_itr_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatPfidIdent_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PfidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PfidCounter ctr = it.value();
            ctrTotal.n_eitr_ += ctr.n_eitr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pfid = ctrTotal.n_eitr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        areaObj.insert(QLatin1String("PFID"), pfid * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_itr_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117PFID_Mode3A_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::PfidCounter ctr) {
        double pfid = ctr.n_eitr_ / static_cast<double>(ctr.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PFID"), pfid * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_itr_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatPfidMode3A_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PfidCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PfidCounter ctr = it.value();
            ctrTotal.n_eitr_ += ctr.n_eitr_;
            ctrTotal.n_itr_ += ctr.n_itr_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double pfid = ctrTotal.n_eitr_ / static_cast<double>(ctrTotal.n_itr_);
        if (pfid > 1)
        {
            pfid = 1;
        }

        areaObj.insert(QLatin1String("PFID"), pfid * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_itr_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}

QJsonObject PerfEvaluator::printED117PLG_json() const
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Movement << Aerodrome::Area::Airborne;


    auto sa_results = [](Counters::PlgCounter ctr) {
        double plg = ctr.n_g_ / static_cast<double>(ctr.n_tr_);
        if (plg > 1)
        {
            plg = 1;
        }

        QJsonObject obj;
        obj.insert(QLatin1String("PLG"), plg * 100);
        obj.insert(QLatin1String("N"), static_cast<qint32>(ctr.n_tr_));

        return obj;
    };


    QJsonObject resultObj;
    for (const Aerodrome::Area area : qAsConst(areas))
    {
        QJsonObject areaObj;
        const auto subAreas = mlatPlg_.findByArea(area);

        QJsonObject subAreasObj;
        Counters::PlgCounter ctrTotal;
        for (const auto &it : subAreas)
        {
            Counters::PlgCounter ctr = it.value();
            ctrTotal.n_g_ += ctr.n_g_;
            ctrTotal.n_tr_ += ctr.n_tr_;

            QString sa_name = it.key().fullName();
            subAreasObj.insert(sa_name, sa_results(ctr));
        }

        areaObj.insert(QLatin1String("subAreas"), subAreasObj);

        double plg = ctrTotal.n_g_ / static_cast<double>(ctrTotal.n_tr_);
        if (plg > 1)
        {
            plg = 1;
        }

        areaObj.insert(QLatin1String("PLG"), plg * 100);
        areaObj.insert(QLatin1String("N"), static_cast<qint32>(ctrTotal.n_tr_));


        QString a_name = QLatin1String(e.valueToKey(area));
        resultObj.insert(a_name, areaObj);
    }

    return resultObj;
}
