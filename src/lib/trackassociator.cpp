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
    SystemType st = t.system_type();

    // Test tracks.
    if (st == SystemType::Smr || st == SystemType::Mlat)
    {
        tstTracks_[st][t.track_number()] = t;
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

int TrackAssociator::run()
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

        Q_ASSERT(n_tot > 0);
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
        SystemType ref_sys_type = tc.system_type();
        sets_.insert(mode_s, TrackCollectionSet(mode_s, ref_sys_type));

        // Iterate for each REF track in the track collection.
        for (const Track &t_ref : tc.tracks())  // clazy:exclude=range-loop,range-loop-detach
        {
            // If track is empty, skip it.
            if (t_ref.isEmpty())
            {
                // qWarning();
                continue;
            }

            // Insert REF track in the set.
            sets_[mode_s] << t_ref;

            // Iterate for each TST track.
            for (QHash<TrackNum, Track> hash : qAsConst(tstTracks_))
            {
                QHash<TrackNum, Track>::iterator it = hash.begin();
                while (it != hash.end())
                {
                    Track t_tst = it.value();

                    // If track is empty, skip it.
                    if (t_tst.isEmpty())
                    {
                        it = hash.erase(it);
                        continue;
                    }

                    // For MLAT, track association is done with the Mode-S address.
                    if (t_tst.system_type() == SystemType::Mlat &&
                        t_tst.mode_s().has_value())
                    {
                        if (t_tst.mode_s().value() == mode_s)
                        {
                            // Insert MLAT TST track in the set.
                            sets_[mode_s] << t_tst;
                            tn2ms_[t_tst.system_type()][t_tst.track_number()] << mode_s;

                            // Only register a match if a time intersection
                            // exists between TST and REF tracks.
                            if (haveTimeIntersection(t_tst, t_ref))
                            {
                                sets_[mode_s].addMatch(t_ref, t_tst);
                            }
                        }
                    }
                    else if (haveTimeIntersection(t_tst, t_ref))
                    {
                        // Otherwise check if there is a time overlap between
                        // reference track and test track and calculate a
                        // track similarity score.

                        // Extract TST track portion that matches in time with the
                        // reference track.
                        std::optional<Track> t_t_opt = intersect(t_tst, t_ref);

                        if (!t_t_opt.has_value())
                        {
                            continue;
                        }

                        Track t_t = t_t_opt.value();

                        // Interpolate the reference track at the times of the
                        // TST track points.
                        Track t_r = resample(t_ref, t_t.timestamps());

                        // Calculate Euclidean distance between TST-REF pairs.
                        QVector<double> dist = euclideanDistance(t_t.rdata(), t_r.rdata());

                        // Check if distances are within the maximum allowed value.
                        // Compute the overall matching score as the ratio between
                        // the number of positive matches and the total elements.
                        double score = calculateScore(dist);

                        // If score satisfies the threshold we have a match.
                        if (score >= threshold)
                        {
                            sets_[mode_s].addMatch(t_ref, t_tst);
                            tn2ms_[t_tst.system_type()][t_tst.track_number()] << mode_s;
                        }
                    }

                    ++it;
                }
            }
        }
    }

    QHash<ModeS, TrackCollectionSet>::iterator it = sets_.begin();
    while (it != sets_.end())
    {
        TrackCollectionSet s = it.value();
        if (!s.isValid())
        {
            it = sets_.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return sets_.size();
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
        return sets_.take(it.key());
    }

    return std::nullopt;
}

const QHash<SystemType, QHash<TrackNum, Track>> &TrackAssociator::tstTracks() const
{
    return tstTracks_;
}

const QHash<ModeS, TrackCollection> &TrackAssociator::refTracks() const
{
    return refTracks_;
}

const QHash<ModeS, TrackCollectionSet> &TrackAssociator::sets() const
{
    return sets_;
}
