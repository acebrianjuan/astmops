/*!
 * \file trackextractor.cpp
 * \brief Implementation of the TrackExtractor class.
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

#include "trackextractor.h"

TrackExtractor::TrackExtractor()
{
}

void TrackExtractor::addData(const TargetReport &tr)
{
    if (!tracks_.value(tr.sys_typ_).contains(tr.trk_nb_))
    {
        tracks_[tr.sys_typ_].insert(tr.trk_nb_, Track(tr.sys_typ_, tr.trk_nb_));
    }

    tracks_[tr.sys_typ_][tr.trk_nb_].add(tr);
}

QVector<Track> TrackExtractor::tracks(SystemType st) const
{
    return tracks_.value(st).values().toVector();
}
