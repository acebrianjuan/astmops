/*!
 * \file trackassociator.cpp
 * \brief Implementation of the TrackAssociator class.
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

#include "trackassociator.h"

TrackAssociator::TrackAssociator()
{
}

void TrackAssociator::addData(const Track &t)
{
    // Test tracks.
    if (t.system_type() == SystemType::Smr ||
        t.system_type() == SystemType::Mlat)
    {
        tstTracks_.insert(t.track_number(), t);
    }

    // Reference tracks.
    if (t.system_type() == SystemType::Adsb ||
        t.system_type() == SystemType::Dgps)
    {
        if (t.mode_s().has_value())
        {
            ModeS mode_s = t.mode_s().value();

            if (!refTracks_.contains(mode_s))
            {
                refTracks_.insert(mode_s, TrackCollection(mode_s, t.system_type()));
            }

            refTracks_[mode_s] << t;
        }
    }
}

void TrackAssociator::run()
{
    double dmax = 30.0;
    double threshold = 0.7;

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

    auto calculateScore = [dmax](const QVector<double> &dist) {
        int n_tot = dist.size();
        int n_ok = 0;

        for (double d : dist)
        {
            if (d <= dmax)
            {
                ++n_ok;
            }
        }

        return static_cast<double>(n_ok) / static_cast<double>(n_tot);
    };

    // Iterate for each target address in the reference.
    for (const TrackCollection &tc : qAsConst(refTracks_))
    {
        if (!tc.mode_s().has_value())
        {
            continue;
        }

        ModeS mode_s = tc.mode_s().value();
        sets_.insert(mode_s, TrackCollectionSet(mode_s));

        // Iterate for each REF track in the track collection.
        for (const Track &t_ref : tc.tracks())  // clazy:exclude=range-loop,range-loop-detach
        {
            // If track is empty, skip it.
            if (t_ref.isEmpty())
            {
                // qWarning();
                continue;
            }

            // Add REF Track to set.
            sets_[mode_s] << t_ref;

            // Iterate for each TST track.
            QHash<TrackNum, Track>::iterator it = tstTracks_.begin();
            while (it != tstTracks_.end())
            {
                Track t_tst = it.value();

                // If track is empty, skip it.
                if (t_tst.isEmpty())
                {
                    it = tstTracks_.erase(it);
                    continue;
                }

                if (t_tst.system_type() == SystemType::Mlat &&
                    t_tst.mode_s().has_value())
                {
                    if (t_tst.mode_s().value() == mode_s)
                    {
                        sets_[mode_s] << t_tst;
                        it = tstTracks_.erase(it);
                    }
                    else
                    {
                        ++it;
                    }

                    continue;
                }

                // Check if there is space-time overlap between reference
                // track and test track.
                if (haveSpaceTimeIntersection(t_tst, t_ref))
                {
                    // Extract TST track portion that matches in time with the
                    // reference track.
                    Track t_t = intersect(t_tst, t_ref).value();

                    // Simplify track by extracting feature points with the
                    // Ramer-Douglas-Peucker algorithm. (Use only for very
                    // densely populated tracks).

                    // Interpolate the reference track at the times of the
                    // TST track points.
                    Track t_r = resample(t_ref, t_t.data().keys().toVector());

                    // TST and REF trajectories should match in size.
                    //Q_ASSERT(t_tst.size() == t_ref.size());

                    // TST and REF pairs should have exact same times.
                    //Q_ASSERT(t_tst.data().keys() == t_ref.data().keys());

                    // Calculate Euclidean distance between TST-REF pairs.
                    QVector<double> dist = euclideanDistance(t_t.data(), t_r.data());

                    // Check if distances are within the maximum allowed value.
                    // Compute the overall matching score as the ratio between
                    // the number of positive matches and the total elements.
                    double score = calculateScore(dist);

                    // If score satisfies the threshold we have a match.
                    if (score >= threshold)
                    {
                        sets_[mode_s] << t_tst;
                        it = tstTracks_.erase(it);
                    }
                }

                ++it;
            }
        }
    }

    QHash<ModeS, TrackCollectionSet>::iterator it = sets_.begin();
    while (it != sets_.end())
    {
        TrackCollectionSet s = it.value();
        if (s.size() <= 1)
        {
            it = sets_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool TrackAssociator::hasPendingData() const
{
    return !sets_.isEmpty();
}

std::optional<TrackCollectionSet> TrackAssociator::takeData()
{
    QHash<ModeS, TrackCollectionSet>::iterator it = sets_.begin();
    if (it != sets_.end())
    {
        sets_.erase(it);
        return it.value();
    }

    return std::nullopt;
}
