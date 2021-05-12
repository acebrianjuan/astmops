/*!
 * \file track.cpp
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

#include "track.h"

Track::Track(SystemType st, TrackNum tn)
    : system_type_(st), track_number_(tn)
{
}

Track::Track(SystemType st, TrackNum tn, const QVector<TargetReport> &tr)
    : system_type_(st), track_number_(tn)
{
    *this << tr;
}

Track &Track::operator<<(const TargetReport &tr)
{
    if (tr.sys_typ_ == system_type_ && tr.trk_nb_ == track_number_)
    {
        data_.insert(tr.tod_, tr);
    }

    return *this;
}

Track &Track::operator<<(const QVector<TargetReport> &l)
{
    for (const TargetReport &tr : l)
    {
        *this << tr;
    }

    return *this;
}

void Track::add(const TargetReport &tr)
{
    *this << tr;
}

void Track::add(const QVector<TargetReport> &l)
{
    *this << l;
}

SystemType Track::system_type() const
{
    return system_type_;
}

TrackNum Track::track_number() const
{
    return track_number_;
}

QMultiMap<QDateTime, TargetReport> Track::data() const
{
    return data_;
}

QDateTime Track::startDateTime() const
{
    QDateTime t;

    if (!data_.isEmpty())
    {
        t = data_.firstKey();
        Q_ASSERT(t.isValid());
    }

    return t;
}

QDateTime Track::endDateTime() const
{
    QDateTime t;

    if (!data_.isEmpty())
    {
        t = data_.lastKey();
        Q_ASSERT(t.isValid());
    }

    return t;
}

double Track::duration() const
{
    double dur = qSNaN();

    QDateTime tstart = startDateTime();
    QDateTime tend = endDateTime();

    if (tstart.isValid() && tend.isValid())
    {
        dur = tstart.msecsTo(tend) / 1000;
    }

    return dur;
}

bool Track::coversDateTime(const QDateTime &tod) const
{
    QDateTime tstart = startDateTime();
    QDateTime tend = endDateTime();

    if (tstart.isValid() && tend.isValid())
    {
        if (tod >= tstart && tod <= tend)
        {
            return true;
        }
    }

    return false;
}

TrackCollection::TrackCollection(SystemType type)
    : system_type_(type)
{
}

TrackCollection::TrackCollection(SystemType type, QVector<Track> &l)
    : system_type_(type)
{
    *this << l;
}

TrackCollection &TrackCollection::operator<<(const Track &t)
{
    track_numbers_ << t.track_number();
    tracks_ << t;

    return *this;
}

TrackCollection &TrackCollection::operator<<(const QVector<Track> &l)
{
    QVector<TrackNum> tnums;
    tnums.reserve(l.size());

    for (const Track &t : l)
    {
        tnums << t.track_number();
    }

    track_numbers_ << tnums;
    tracks_ << l;

    Q_ASSERT(track_numbers_.size() == tracks_.size());

    return *this;
}

void TrackCollection::add(const Track &t)
{
    *this << t;
}

void TrackCollection::add(const QVector<Track> &l)
{
    *this << l;
}

std::optional<Track> TrackCollection::track(const TrackNum tn) const
{
    if (containsTrackNumber(tn))
    {
        for (const Track &track : tracks_)
        {
            if (track.track_number() == tn)
            {
                return track;
            }
        }
    }

    return std::nullopt;
}

bool TrackCollection::containsTrackNumber(const TrackNum tn) const
{
    return track_numbers_.contains(tn);
}

bool TrackCollection::coversDateTime(const QDateTime &tod) const
{
    for (const Track &t : tracks_)
    {
        if (t.coversDateTime(tod))
        {
            return true;
        }
    }

    return false;
}

SystemType TrackCollection::system_type() const
{
    return system_type_;
}

QVector<TrackNum> TrackCollection::track_numbers() const
{
    return track_numbers_;
}

QVector<Track> TrackCollection::tracks() const
{
    return tracks_;
}

bool operator==(const Track &lhs, const Track &rhs)
{
    return lhs.system_type() == rhs.system_type() &&
           lhs.track_number() == rhs.track_number() &&
           lhs.data() == rhs.data();
}

bool operator==(const TrackCollection &lhs, const TrackCollection &rhs)
{
    return lhs.system_type() == rhs.system_type() &&
           lhs.track_numbers() == rhs.track_numbers() &&
           lhs.tracks() == rhs.tracks();
}
