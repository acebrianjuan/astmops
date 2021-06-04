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

/* -------------------------------- Track --------------------------------- */

Track::Track(SystemType st, TrackNum tn)
    : system_type_(st), track_number_(tn)
{
}

Track::Track(SystemType st, TrackNum tn, const QVector<TargetReport> &tr)
    : system_type_(st), track_number_(tn)
{
    *this << tr;
}

Track::Track(ModeS ms, SystemType st, TrackNum tn)
    : system_type_(st), track_number_(tn)
{
    setMode_s(ms);
}

Track::Track(ModeS ms, SystemType st, TrackNum tn, const QVector<TargetReport> &tr)
    : system_type_(st), track_number_(tn)
{
    setMode_s(ms);
    *this << tr;
}

Track &Track::operator<<(const TargetReport &tr)
{
    if (tr.sys_typ_ == system_type_ && tr.trk_nb_ == track_number_ &&
        tr.tod_.isValid())
    {
        // Start/End DateTimes.
        QDateTime tod = tr.tod_;
        if (beginDateTime_.isNull())
        {
            beginDateTime_ = tod;
        }
        else if (tod < beginDateTime_)
        {
            beginDateTime_ = tod;
        }

        if (endDateTime_.isNull())
        {
            endDateTime_ = tod;
        }
        else if (tod > endDateTime_)
        {
            endDateTime_ = tod;
        }

        // XYZ Bounds.
        if (!qIsNaN(tr.x_) && !qIsNaN(tr.y_))
        {
            // X min.
            if (qIsNaN(x_bounds_.first))
            {
                x_bounds_.first = tr.x_;
            }
            else if (tr.x_ < x_bounds_.first)
            {
                x_bounds_.first = tr.x_;
            }

            // X max.
            if (qIsNaN(x_bounds_.second))
            {
                x_bounds_.second = tr.x_;
            }
            else if (tr.x_ > x_bounds_.second)
            {
                x_bounds_.second = tr.x_;
            }

            // Y min.
            if (qIsNaN(y_bounds_.first))
            {
                y_bounds_.first = tr.y_;
            }
            else if (tr.y_ < y_bounds_.first)
            {
                y_bounds_.first = tr.y_;
            }

            // Y max.
            if (qIsNaN(y_bounds_.second))
            {
                y_bounds_.second = tr.y_;
            }
            else if (tr.y_ > y_bounds_.second)
            {
                y_bounds_.second = tr.y_;
            }
        }

        data_.insert(tr.tod_, tr);

        // Asign first detected Mode-S address to Track object.
        if (!mode_s_.has_value() && tr.mode_S_.has_value())
        {
            mode_s_ = tr.mode_S_;
        }
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

SystemType Track::system_type() const
{
    return system_type_;
}

TrackNum Track::track_number() const
{
    return track_number_;
}

TgtRepMap Track::data() const
{
    return data_;
}

std::optional<ModeS> Track::mode_s() const
{
    return mode_s_;
}

QPair<double, double> Track::x_bounds() const
{
    return x_bounds_;
}

QPair<double, double> Track::y_bounds() const
{
    return y_bounds_;
}

QPair<double, double> Track::z_bounds() const
{
    return z_bounds_;
}

bool Track::isEmpty() const
{
    return data_.isEmpty();
}

int Track::size() const
{
    return data_.size();
}

QVector<QDateTime> Track::timestamps() const
{
    QVector<QDateTime> tstamps;

    TgtRepMap::const_iterator it;
    for (it = data_.begin(); it != data_.end(); ++it)
    {
        tstamps << it.key();
    }

    return tstamps;
}

QDateTime Track::beginDateTime() const
{
    return beginDateTime_;
}

QDateTime Track::endDateTime() const
{
    return endDateTime_;
}

double Track::duration() const
{
    double dur = qSNaN();

    QDateTime tbegin = beginDateTime_;
    QDateTime tend = endDateTime_;

    if (tbegin.isValid() && tend.isValid())
    {
        dur = tbegin.msecsTo(tend) / 1000;
    }

    return dur;
}

bool Track::coversDateTime(const QDateTime &tod) const
{
    QDateTime tbegin = beginDateTime_;
    QDateTime tend = endDateTime_;

    if (tbegin.isValid() && tend.isValid())
    {
        if (tod >= tbegin && tod <= tend)
        {
            return true;
        }
    }

    return false;
}

void Track::intersect(const Track &other)
{
    QMap<QDateTime, TargetReport>::iterator it = data_.begin();
    while (it != data_.lowerBound(other.beginDateTime()))
    {
        data_.erase(it);
        ++it;
    }

    it = data_.upperBound(other.endDateTime());
    while (it != data_.end())
    {
        data_.erase(it);
        ++it;
    }
}

void Track::setMode_s(ModeS ms)
{
    if (ms <= 0xFFFFFF)
    {
        mode_s_ = ms;
    }
}

/* --------------------------- TrackCollection ---------------------------- */

TrackCollection::TrackCollection(SystemType st)
    : system_type_(st)
{
}

TrackCollection::TrackCollection(SystemType st, const Track &t)
    : system_type_(st)
{
    *this << t;
}

TrackCollection::TrackCollection(SystemType st, const QVector<Track> &l)
    : system_type_(st)
{
    *this << l;
}

TrackCollection::TrackCollection(ModeS ms, SystemType st)
    : system_type_(st)
{
    setMode_s(ms);
}

TrackCollection::TrackCollection(ModeS ms, SystemType st, const Track &t)
    : system_type_(st)
{
    setMode_s(ms);
    *this << t;
}

TrackCollection::TrackCollection(ModeS ms, SystemType st, const QVector<Track> &l)
    : system_type_(st)
{
    setMode_s(ms);
    *this << l;
}

TrackCollection &TrackCollection::operator<<(const Track &t)
{
    if (t.system_type() == system_type_)
    {
        // Start/End DateTimes.
        QDateTime beginTod = t.beginDateTime();
        if (beginDateTime_.isNull())
        {
            beginDateTime_ = beginTod;
        }
        else if (beginTod.isValid() && beginTod < beginDateTime_)
        {
            beginDateTime_ = beginTod;
        }

        QDateTime endTod = t.endDateTime();
        if (endDateTime_.isNull())
        {
            endDateTime_ = endTod;
        }
        else if (endTod.isValid() && endTod > endDateTime_)
        {
            endDateTime_ = endTod;
        }

        // Insert Track and register track number.
        tracks_.insert(t.beginDateTime(), t);
        track_numbers_ << t.track_number();
    }

    return *this;
}

TrackCollection &TrackCollection::operator<<(const QVector<Track> &l)
{
    for (const Track &t : l)
    {
        *this << t;
    }

    return *this;
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
    QVector<Track> vec;
    vec.reserve(tracks_.size());

    for (const Track &t : tracks_)
    {
        vec << t;
    }

    return vec;
}

bool TrackCollection::isEmpty() const
{
    return tracks_.isEmpty();
}

int TrackCollection::size() const
{
    return tracks_.size();
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

TrackCollection TrackCollection::tracks(const QVector<TrackNum> &v) const
{
    TrackCollection col = mode_s_.has_value()
                              ? TrackCollection(mode_s_.value(), system_type_)
                              : TrackCollection(system_type_);

    for (TrackNum tn : v)
    {
        if (containsTrackNumber(tn))
        {
            col << track(tn).value();
        }
    }

    return col;
}

bool TrackCollection::containsTrackNumber(const TrackNum tn) const
{
    return track_numbers_.contains(tn);
}

QDateTime TrackCollection::beginDateTime() const
{
    return beginDateTime_;
}

QDateTime TrackCollection::endDateTime() const
{
    return endDateTime_;
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

std::optional<ModeS> TrackCollection::mode_s() const
{
    return mode_s_;
}

void TrackCollection::setMode_s(ModeS ms)
{
    if (ms <= 0xFFFFFF)
    {
        mode_s_ = ms;
    }
}

/* -------------------------- TrackCollectionSet -------------------------- */

TrackCollectionSet::TrackCollectionSet(ModeS mode_s, SystemType ref_st)
    : mode_s_(mode_s), ref_sys_type_(ref_st), ref_col_(ref_st)
{
}

TrackCollectionSet::TrackCollectionSet(ModeS mode_s, SystemType ref_st, const QVector<TrackCollection> &cols)
    : mode_s_(mode_s), ref_sys_type_(ref_st), ref_col_(ref_st)
{
    for (const TrackCollection &c : cols)
    {
        *this << c;
    }
}

TrackCollectionSet &TrackCollectionSet::operator<<(const Track &t)
{
    SystemType st = t.system_type();

    if (st == SystemType::Unknown)
    {
        return *this;
    }

    TrackNum tn = t.track_number();

    if (st == ref_sys_type_)
    {
        if (!ref_col_.containsTrackNumber(tn))
        {
            ref_col_ << t;
        }
    }
    else
    {
        if (!tst_cols_.contains(st))
        {
            // Insert a new track collection containing the given track.
            tst_cols_.insert(st, TrackCollection(st, t));
        }
        else
        {
            if (!tst_cols_[st].containsTrackNumber(tn))
            {
                // Append track to already existing collection.
                tst_cols_[st] << t;
            }
        }
    }

    return *this;
}

void TrackCollectionSet::addMatch(const Track &t_ref, const Track &t_tst)
{
    if (containsMatch(t_ref, t_tst))
    {
        // This match is already registered. Skip to avoid duplication.
        return;
    }

    // Insert reference and test tracks. If any of the tracks already exists
    // in the collection it will not be added to avoid duplication.
    *this << t_ref << t_tst;

    // Register match.
    SystemType st = t_tst.system_type();
    TrackNum ref_tn = t_ref.track_number();
    TrackNum tst_tn = t_tst.track_number();

    QVector<TrackNum> &match_vec = matches_[st][ref_tn];
    match_vec << tst_tn;

    // Sort vector of matched track numbers based on first timestamp:
    TrackCollection col = collection(st).value();

    auto sortfun = [col](TrackNum lhs, TrackNum rhs) {
        Q_ASSERT(col.track(lhs).has_value());
        Q_ASSERT(col.track(rhs).has_value());

        return col.track(lhs).value().beginDateTime() <
               col.track(rhs).value().beginDateTime();
    };

    std::sort(match_vec.begin(), match_vec.end(), sortfun);
}

QVector<TrackCollection> TrackCollectionSet::tstTrackCols() const
{
    QVector<TrackCollection> cols;

    for (const TrackCollection &c : tst_cols_)
    {
        cols << c;
    }

    return cols;
}

TrackCollection TrackCollectionSet::refTrackCol() const
{
    return ref_col_;
}

QVector<TrackCollection> TrackCollectionSet::getMatches(TrackNum ref_tn) const
{
    QVector<TrackCollection> vec;

    QHash<SystemType, QHash<TrackNum, QVector<TrackNum>>>::const_iterator it;
    for (it = matches_.begin(); it != matches_.end(); ++it)
    {
        SystemType st = it.key();
        QHash<TrackNum, QVector<TrackNum>> hash = it.value();

        if (hash.contains(ref_tn))
        {
            vec << tst_cols_[st].tracks(hash[ref_tn]);
        }
    }

    return vec;
}

bool TrackCollectionSet::containsTrack(SystemType st, TrackNum tn) const
{
    if (st == SystemType::Unknown)
    {
        return false;
    }

    if (st == ref_sys_type_)
    {
        if (ref_col_.containsTrackNumber(tn))
        {
            return true;
        }
    }
    else
    {
        if (tst_cols_.contains(st))
        {
            if (tst_cols_[st].containsTrackNumber(tn))
            {
                return true;
            }
        }
    }

    return false;
}

bool TrackCollectionSet::containsMatch(SystemType st, TrackNum ref_tn, TrackNum tst_tn)
{
    if (matches_.contains(st))
    {
        if (matches_[st].contains(ref_tn))
        {
            if (matches_[st][ref_tn].contains(tst_tn))
            {
                return true;
            }
        }
    }

    return false;
}

bool TrackCollectionSet::containsMatch(const Track &t_ref, const Track &t_tst)
{
    SystemType st = t_tst.system_type();
    return containsMatch(st, t_ref.track_number(), t_tst.track_number());
}

std::optional<TrackCollection> TrackCollectionSet::collection(SystemType st) const
{
    if (st == ref_sys_type_)
    {
        return ref_col_;
    }
    else if (tst_cols_.contains(st))
    {
        return tst_cols_.value(st);
    }

    return std::nullopt;
}

bool TrackCollectionSet::hasCollection(SystemType st) const
{
    return tst_cols_.contains(st);
}

bool TrackCollectionSet::hasRefData() const
{
    return !ref_col_.isEmpty();
}

bool TrackCollectionSet::hasTestData() const
{
    return !tst_cols_.isEmpty();
}

bool TrackCollectionSet::isValid() const
{
    // For a set to be considered valid, it must have a reference
    // TrackCollection of known SystemType and at least a test TrackCollection.

    return (ref_sys_type_ == SystemType::Adsb ||
               ref_sys_type_ == SystemType::Dgps) &&
           hasRefData() && hasTestData();
}

TrackCollectionSet &TrackCollectionSet::operator<<(const TrackCollection &c)
{
    SystemType st = c.system_type();

    if (!tst_cols_.contains(st))
    {
        // Insert collection to the set.
        tst_cols_.insert(st, c);
    }
    else
    {
        // Append tracks to already existing collection in the set.
        tst_cols_[st] << c.tracks();
    }

    return *this;
}

bool TrackCollectionSet::isEmpty() const
{
    return ref_col_.isEmpty() && tst_cols_.isEmpty();
}

int TrackCollectionSet::size() const
{
    return tst_cols_.size();
}

ModeS TrackCollectionSet::mode_s() const
{
    return mode_s_;
}

SystemType TrackCollectionSet::ref_sys_type() const
{
    return ref_sys_type_;
}

void TrackCollectionSet::setMode_s(ModeS mode_s)
{
    mode_s_ = mode_s;
}

void TrackCollectionSet::setRef_sys_type(SystemType ref_sys_type)
{
    ref_sys_type_ = ref_sys_type;
}


/* ---------------------------- Free functions ---------------------------- */

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

bool haveTimeIntersection(const Track &lhs, const Track &rhs)
{
    return lhs.beginDateTime() < rhs.endDateTime() &&
           rhs.beginDateTime() < lhs.endDateTime();
}

bool haveSpaceIntersection(const Track &lhs, const Track &rhs)
{
    /*
    return lhs.x_bounds().first < rhs.x_bounds().second &&
           rhs.x_bounds().first < lhs.x_bounds().second &&
           lhs.y_bounds().first < rhs.y_bounds().second &&
           rhs.y_bounds().first < lhs.y_bounds().second &&
           lhs.z_bounds().first < rhs.z_bounds().second &&
           rhs.z_bounds().first < lhs.z_bounds().second;
    */

    return lhs.x_bounds().first < rhs.x_bounds().second &&
           rhs.x_bounds().first < lhs.x_bounds().second &&
           lhs.y_bounds().first < rhs.y_bounds().second &&
           rhs.y_bounds().first < lhs.y_bounds().second;
}

bool haveSpaceTimeIntersection(const Track &lhs, const Track &rhs)
{
    return haveTimeIntersection(lhs, rhs) &&
           haveSpaceIntersection(lhs, rhs);
}

std::optional<Track> intersect(const Track &intersectee, const Track &intersector)
{
    if (!haveSpaceTimeIntersection(intersectee, intersector))
    {
        return std::nullopt;
    }

    // Create a new empty Track object with same SystemType and TrackNum as
    // intersectee.
    Track t(intersectee.system_type(), intersectee.track_number());

    if (intersectee.mode_s().has_value())
    {
        t.mode_s() = intersectee.mode_s();
    }

    QMultiMap<QDateTime, TargetReport> data = intersectee.data();
    QMultiMap<QDateTime, TargetReport>::iterator it_from = data.lowerBound(intersector.beginDateTime());
    QMultiMap<QDateTime, TargetReport>::iterator it_to = data.upperBound(intersector.endDateTime());

    // Only insert elements of intersectee that satisfy intersection with
    // intersector.
    QMultiMap<QDateTime, TargetReport>::iterator it;
    for (it = it_from; it != it_to; ++it)
    {
        t << it.value();
    }

    Q_ASSERT(!t.isEmpty());

    return t;
}

Track resample(const Track &track, const QVector<QDateTime> &dtimes)
{
    // Create a new empty Track object with same SystemType and TrackNum as
    // input track.
    Track t(track.system_type(), track.track_number());

    QMultiMap<QDateTime, TargetReport> data = track.data();
    for (const QDateTime &tod : dtimes)
    {
        if (track.coversDateTime(tod))
        {
            if (data.contains(tod))
            {
                // Exact match. No need to interpolate.
                t << data.value(tod);
            }
            else  // Linear interpolation.
            {
                QMultiMap<QDateTime, TargetReport>::iterator it_u;  // Upper.
                QMultiMap<QDateTime, TargetReport>::iterator it_l;  // Lower.

                QMultiMap<QDateTime, TargetReport>::iterator it = data.lowerBound(tod);
                if (it != data.end())  // Upper TOD found.
                {
                    Q_ASSERT(it.key() >= tod);

                    // Save upper value.
                    it_u = it;

                    // Search lower values by decrementing iterator.
                    while (it != data.end() && tod < it.key())
                    {
                        if (it == data.begin())  // Exit condition on first value.
                        {
                            if (tod < it.key())  // Set as not found.
                            {
                                it = data.end();
                            }

                            break;
                        }

                        it--;
                    }

                    if (it != data.end())  // Lower TOD found.
                    {
                        Q_ASSERT(tod >= it.key());

                        // Save lower value.
                        it_l = it;
                    }

                    if (it_u != data.end() && it_l != data.end())
                    {
                        Q_ASSERT(it_u.key() > it_l.key());

                        TargetReport tr_u = it_u.value();  // Upper.
                        TargetReport tr_l = it_l.value();  // Lower.

                        TargetReport tr_i;  // Interpolated.
                        tr_i.ds_id_ = tr_l.ds_id_;
                        tr_i.sys_typ_ = tr_l.sys_typ_;
                        tr_i.tod_ = tod;
                        tr_i.trk_nb_ = tr_l.trk_nb_;
                        tr_i.mode_S_ = tr_l.mode_S_;
                        tr_i.mode_3A_ = tr_l.mode_3A_;
                        tr_i.ident_ = tr_l.ident_;
                        tr_i.on_gnd_ = tr_l.on_gnd_;

                        double dt_t = tr_l.tod_.msecsTo(tr_u.tod_) / 1000.0;
                        double dt_i = tr_l.tod_.msecsTo(tod) / 1000.0;

                        Q_ASSERT(dt_t > 0 && dt_i > 0);

                        double f = dt_i / dt_t;

                        Q_ASSERT(f > 0);

                        double dx = tr_u.x_ - tr_l.x_;
                        double dy = tr_u.y_ - tr_l.y_;

                        double x_i = tr_l.x_ + f * dx;
                        double y_i = tr_l.y_ + f * dy;

                        tr_i.x_ = x_i;
                        tr_i.y_ = y_i;

                        if (tr_u.z_.has_value() && tr_l.z_.has_value())
                        {
                            double dz = tr_u.z_.value() - tr_l.z_.value();
                            double z_i = tr_l.z_.value() + f * dz;
                            tr_i.z_ = z_i;
                        }

                        // Add interpolated TargetReport to track.
                        t << tr_i;
                    }
                }
            }
        }
        else  // Linear extrapolation.
        {
        }
    }

    return t;
}
