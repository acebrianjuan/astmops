/*!
 * \file track.h
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

#ifndef ASTMOPS_TRACK_H
#define ASTMOPS_TRACK_H

#include "astmops.h"
#include "targetreport.h"
#include <QDateTime>
#include <QMultiMap>

class Track
{
public:
    Track() = default;
    Track(SystemType st, TrackNum tn);
    Track(SystemType st, TrackNum tn, const QVector<TargetReport> &tr);

    Track &operator<<(const TargetReport &tr);
    Track &operator<<(const QVector<TargetReport> &l);
    void add(const TargetReport &tr);
    void add(const QVector<TargetReport> &l);

    SystemType system_type() const;
    TrackNum track_number() const;
    QMultiMap<QDateTime, TargetReport> data() const;

    QDateTime startDateTime() const;
    QDateTime endDateTime() const;
    double duration() const;
    bool coversDateTime(const QDateTime &tod) const;

private:
    SystemType system_type_;
    TrackNum track_number_;
    QMultiMap<QDateTime, TargetReport> data_;
};

Q_DECLARE_METATYPE(Track);

class TrackCollection
{
public:
    TrackCollection() = default;
    TrackCollection(SystemType type);
    TrackCollection(SystemType type, QVector<Track> &l);

    TrackCollection &operator<<(const Track &t);
    TrackCollection &operator<<(const QVector<Track> &l);
    void add(const Track &t);
    void add(const QVector<Track> &l);

    std::optional<Track> track(const TrackNum tn) const;
    bool containsTrackNumber(const TrackNum tn) const;
    bool coversDateTime(const QDateTime &tod) const;

    SystemType system_type() const;
    QVector<TrackNum> track_numbers() const;
    QVector<Track> tracks() const;

private:
    SystemType system_type_;

    QVector<TrackNum> track_numbers_;
    QVector<Track> tracks_;
};

Q_DECLARE_METATYPE(TrackCollection);

bool operator==(const Track &lhs, const Track &rhs);
bool operator==(const TrackCollection &lhs, const TrackCollection &rhs);

#endif  // ASTMOPS_TRACK_H
