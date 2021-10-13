/*!
 * \file trackextractor.h
 * \brief Interface of the TrackExtractor class.
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

#ifndef ASTMOPS_TRACKEXTRACTOR_H
#define ASTMOPS_TRACKEXTRACTOR_H

#include "astmops.h"
#include "targetreport.h"
#include "track.h"

class TrackExtractor
{
public:
    TrackExtractor();

    void addData(const TargetReport& tr);

    QVector<Track> tracks(SystemType st) const;
    bool hasPendingData() const;

    std::optional<Track> takeData();

private:
    QHash<SystemType, QMap<TrackNum, Track>> tracks_;
};

#endif  // ASTMOPS_TRACKEXTRACTOR_H
