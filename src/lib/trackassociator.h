/*!
 * \file trackassociator.h
 * \brief Interface of the TrackAssociator class.
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

#ifndef ASTMOPS_TRACKASSOCIATOR_H
#define ASTMOPS_TRACKASSOCIATOR_H

#include "asterix.h"
#include "astmops.h"
#include "track.h"
#include <QMap>

class TrackAssociator
{
public:
    TrackAssociator();

    void addData(const Track &t);
    int run();

    bool hasPendingData() const;
    std::optional<TrackCollectionSet> takeData();

    const QHash<SystemType, QHash<TrackNum, Track>> &tstTracks() const;
    const QHash<ModeS, TrackCollection> &refTracks() const;
    const QHash<ModeS, TrackCollectionSet> &sets() const;

private:
    QHash<SystemType, QMap<TrackNum, QSet<ModeS>>> tn2ms_;

    QHash<SystemType, QHash<TrackNum, Track>> tstTracks_;
    QHash<ModeS, TrackCollection> refTracks_;

    QHash<ModeS, TrackCollectionSet> sets_;
};

#endif  // ASTMOPS_TRACKASSOCIATOR_H
