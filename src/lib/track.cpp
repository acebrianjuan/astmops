/*!
 * \file track.cpp
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
        // Insert target report into the track.
        data_.insert(tr.tod_, tr);

        // Begin/end timestamps.
        QDateTime tod = tr.tod_;
        if (!beginTimestamp_.isValid())
        {
            beginTimestamp_ = tod;
        }
        else if (tod < beginTimestamp_)
        {
            beginTimestamp_ = tod;
        }

        if (!endTimestamp_.isValid())
        {
            endTimestamp_ = tod;
        }
        else if (tod > endTimestamp_)
        {
            endTimestamp_ = tod;
        }

        // XYZ bounds.
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

        if (!qIsNaN(tr.z_))
        {
            // Z min.
            if (qIsNaN(z_bounds_.first))
            {
                z_bounds_.first = tr.z_;
            }
            else if (tr.z_ < z_bounds_.first)
            {
                z_bounds_.first = tr.z_;
            }

            // Z max.
            if (qIsNaN(z_bounds_.second))
            {
                z_bounds_.second = tr.z_;
            }
            else if (tr.z_ > z_bounds_.second)
            {
                z_bounds_.second = tr.z_;
            }
        }

        // Crossed areas.
        if (!nareas_.contains(tr.narea_))
        {
            nareas_ << tr.narea_;
        }

        // Target types.
        if (!tgt_typs_.contains(tr.tgt_typ_))
        {
            tgt_typs_ << tr.tgt_typ_;
        }

        // Asign first detected Mode-S address to Track object.
        if (!mode_s_.has_value() && tr.mode_s_.has_value())
        {
            mode_s_ = tr.mode_s_;
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

TgtRepMap &Track::rdata()
{
    return data_;
}

const TgtRepMap &Track::data() const
{
    return data_;
}

std::optional<ModeS> Track::mode_s() const
{
    return mode_s_;
}

QSet<Aerodrome::NamedArea> Track::nareas() const
{
    return nareas_;
}

const QSet<TargetType> &Track::tgt_typs() const
{
    return tgt_typs_;
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

QDateTime Track::beginTimestamp() const
{
    return beginTimestamp_;
}

QDateTime Track::endTimestamp() const
{
    return endTimestamp_;
}

double Track::duration() const
{
    double dur = qSNaN();

    QDateTime tbegin = beginTimestamp_;
    QDateTime tend = endTimestamp_;

    if (tbegin.isValid() && tend.isValid())
    {
        dur = tbegin.msecsTo(tend) / 1000.0;
    }

    return dur;
}

bool Track::coversTimestamp(const QDateTime &tod) const
{
    if (!tod.isValid())
    {
        return false;
    }

    QDateTime tbegin = beginTimestamp_;
    QDateTime tend = endTimestamp_;

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
    while (it != data_.lowerBound(other.beginTimestamp()))
    {
        it = data_.erase(it);
    }

    it = data_.upperBound(other.endTimestamp());
    while (it != data_.end())
    {
        it = data_.erase(it);
    }
}

void Track::setMode_s(ModeS ms)
{
    if (ms <= 0xFFFFFF)
    {
        mode_s_ = ms;
    }
}

void Track::clear()
{
    data_.clear();

    beginTimestamp_ = QDateTime();
    endTimestamp_ = QDateTime();

    nareas_.clear();

    x_bounds_ = {qSNaN(), qSNaN()};
    y_bounds_ = {qSNaN(), qSNaN()};
    z_bounds_ = {qSNaN(), qSNaN()};
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
    if (system_type_ == SystemType::Unknown ||
        t.system_type() == SystemType::Unknown || t.isEmpty())
    {
        return *this;
    }

    if (t.system_type() == system_type_)
    {
        // Insert Track and register track number.
        tracks_.insert(t.beginTimestamp(), t);
        track_numbers_ << t.track_number();

        // Begin/end timestamps.
        QDateTime beginTod = t.beginTimestamp();
        if (!beginTimestamp_.isValid())
        {
            beginTimestamp_ = beginTod;
        }
        else if (beginTod.isValid() && beginTod < beginTimestamp_)
        {
            beginTimestamp_ = beginTod;
        }

        QDateTime endTod = t.endTimestamp();
        if (!endTimestamp_.isValid())
        {
            endTimestamp_ = endTod;
        }
        else if (endTod.isValid() && endTod > endTimestamp_)
        {
            endTimestamp_ = endTod;
        }

        // Crossed areas.
        QSet<Aerodrome::NamedArea> nareas = t.nareas();
        for (const Aerodrome::NamedArea &narea : nareas)
        {
            if (!nareas_.contains(narea))
            {
                nareas_ << narea;
            }
        }

        // Target types.
        QSet<TargetType> types = t.tgt_typs();
        for (const TargetType &typ : types)
        {
            if (!tgt_typs_.contains(typ))
            {
                tgt_typs_ << typ;
            }
        }

        // Asign first detected Mode-S address to Collection object.
        if (!mode_s_.has_value() && t.mode_s().has_value())
        {
            mode_s_ = t.mode_s();
        }
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

QSet<TrackNum> TrackCollection::track_numbers() const
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

TrackCollection TrackCollection::makeSubColForTracks(const QVector<TrackNum> &v) const
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

bool TrackCollection::isEmpty() const
{
    return tracks_.isEmpty();
}

int TrackCollection::size() const
{
    return tracks_.size();
}

QDateTime TrackCollection::beginTimestamp() const
{
    return beginTimestamp_;
}

QDateTime TrackCollection::endTimestamp() const
{
    return endTimestamp_;
}

bool TrackCollection::coversTimestamp(const QDateTime &tod) const
{
    if (!tod.isValid())
    {
        return false;
    }

    for (const Track &t : tracks_)
    {
        if (t.coversTimestamp(tod))
        {
            return true;
        }
    }

    return false;
}

std::optional<Track> TrackCollection::getTrackAtTimestamp(const QDateTime &tod) const
{
    if (!tod.isValid())
    {
        return std::nullopt;
    }

    for (const Track &t : tracks_)
    {
        if (t.coversTimestamp(tod))
        {
            return t;
        }
    }

    return std::nullopt;
}

QSet<Aerodrome::NamedArea> TrackCollection::nareas() const
{
    return nareas_;
}

const QSet<TargetType> &TrackCollection::tgt_typs() const
{
    return tgt_typs_;
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

TrackCollectionSet &TrackCollectionSet::operator<<(const Track &t)
{
    if (ref_sys_type_ == SystemType::Unknown ||
        t.system_type() == SystemType::Unknown || t.isEmpty())
    {
        return *this;
    }

    SystemType st = t.system_type();
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

TrackCollectionSet &TrackCollectionSet::operator<<(const TrackCollection &c)
{
    if (c.system_type() != SystemType::Unknown)
    {
        for (const Track &trk : c)
        {
            *this << trk;
        }
    }

    return *this;
}

void TrackCollectionSet::addMatch(const Track &t_ref, const Track &t_tst)
{
    if (ref_sys_type_ == SystemType::Unknown ||
        t_ref.system_type() == SystemType::Unknown || t_ref.isEmpty() ||
        t_tst.system_type() == SystemType::Unknown || t_tst.isEmpty() ||
        t_ref.system_type() != ref_sys_type_ ||
        t_ref.system_type() == t_tst.system_type())
    {
        return;
    }

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

        return col.track(lhs).value().beginTimestamp() <
               col.track(rhs).value().beginTimestamp();
    };

    std::sort(match_vec.begin(), match_vec.end(), sortfun);
}

QVector<TrackCollection> TrackCollectionSet::tstTrackCols() const
{
    QVector<TrackCollection> vec;
    for (const TrackCollection &c : tst_cols_)
    {
        vec << c;
    }

    std::sort(vec.begin(), vec.end());

    return vec;
}

TrackCollection TrackCollectionSet::refTrackCol() const
{
    return ref_col_;
}

MatchHash TrackCollectionSet::matches() const
{
    return matches_;
}

QVector<TrackCollection> TrackCollectionSet::matchesForRefTrack(TrackNum ref_tn) const
{
    QVector<TrackCollection> vec;

    MatchHash::const_iterator it;
    for (it = matches_.begin(); it != matches_.end(); ++it)
    {
        SystemType st = it.key();
        QHash<TrackNum, QVector<TrackNum>> hash = it.value();

        if (hash.contains(ref_tn))
        {
            vec << tst_cols_[st].makeSubColForTracks(hash[ref_tn]);
        }
    }

    std::sort(vec.begin(), vec.end());

    return vec;
}

std::optional<TrackCollection> TrackCollectionSet::matchesForRefTrackAndSystem(TrackNum ref_tn, SystemType st) const
{
    if (matches_.contains(st) && matches_[st].contains(ref_tn))
    {
        return tst_cols_[st].makeSubColForTracks(matches_[st][ref_tn]);
    }

    return std::nullopt;
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
    if (t_ref.system_type() != ref_sys_type_)
    {
        return false;
    }

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
    // TrackCollection of known SystemType.

    return (ref_sys_type_ != SystemType::Unknown && hasRefData());
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

bool operator==(const TrackCollectionSet &lhs, const TrackCollectionSet &rhs)
{
    return lhs.mode_s() == rhs.mode_s() &&
           lhs.ref_sys_type() == rhs.ref_sys_type() &&
           lhs.refTrackCol() == rhs.refTrackCol() &&
           lhs.tstTrackCols() == rhs.tstTrackCols() &&
           lhs.matches() == rhs.matches();
}

bool operator<(const Track &lhs, const Track &rhs)
{
    return lhs.beginTimestamp() < rhs.beginTimestamp();
}

bool operator<(const TrackCollection &lhs, const TrackCollection &rhs)
{
    return lhs.beginTimestamp() < rhs.beginTimestamp();
}

bool haveTimeIntersection(const Track &lhs, const Track &rhs)
{
    return lhs.beginTimestamp() < rhs.endTimestamp() &&
           rhs.beginTimestamp() < lhs.endTimestamp();
}

bool haveSpaceIntersection(const Track &lhs, const Track &rhs)
{
    return lhs.x_bounds().first <= rhs.x_bounds().second &&
           rhs.x_bounds().first <= lhs.x_bounds().second &&
           lhs.y_bounds().first <= rhs.y_bounds().second &&
           rhs.y_bounds().first <= lhs.y_bounds().second &&
           lhs.z_bounds().first <= rhs.z_bounds().second &&
           rhs.z_bounds().first <= lhs.z_bounds().second;
}

bool haveSpaceTimeIntersection(const Track &lhs, const Track &rhs)
{
    return haveTimeIntersection(lhs, rhs) &&
           haveSpaceIntersection(lhs, rhs);
}

std::optional<Track> intersect(const Track &intersectee, const Track &intersector)
{
    if (!haveTimeIntersection(intersectee, intersector) ||
        intersector.size() < 2)
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

    const TgtRepMap &data = intersectee.data();
    TgtRepMap::const_iterator it_from = data.lowerBound(intersector.beginTimestamp());
    TgtRepMap::const_iterator it_to = data.upperBound(intersector.endTimestamp());

    // Only insert elements of intersectee that satisfy intersection with
    // intersector.
    TgtRepMap::const_iterator it;
    for (it = it_from; it != it_to; ++it)
    {
        t << it.value();
    }

    if (t.isEmpty())
    {
        return std::nullopt;
    }

    return t;
}

Track resample(const Track &track, const QVector<QDateTime> &dtimes)
{
    // Create a new empty Track object with same SystemType and TrackNum as
    // input track.
    Track t(track.system_type(), track.track_number());

    const TgtRepMap &data = track.data();
    for (const QDateTime &tod : dtimes)
    {
        if (track.coversTimestamp(tod))
        {
            if (data.contains(tod))
            {
                // Exact match. No need to interpolate.
                t << data.value(tod);
            }
            else  // Linear interpolation.
            {
                TgtRepMap::const_iterator it_u = data.end();  // Upper.
                TgtRepMap::const_iterator it_l = data.end();  // Lower.

                TgtRepMap::const_iterator it = data.lowerBound(tod);
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

                    if (it_l != data.end() && it_u != data.end())
                    {
                        Q_ASSERT(it_u.key() > it_l.key());

                        TargetReport tr_u = it_u.value();  // Upper.
                        TargetReport tr_l = it_l.value();  // Lower.

                        // Create interpolated TgtRep as a copy of the
                        // TgtRep that precedes it with a new timestamp and
                        // position.
                        TargetReport tr_i = tr_l;
                        tr_i.tod_ = tod;

                        double dt_t = tr_l.tod_.msecsTo(tr_u.tod_) / 1000.0;
                        double dt_i = tr_l.tod_.msecsTo(tod) / 1000.0;

                        Q_ASSERT(dt_t > 0 && dt_i > 0);

                        double f = dt_i / dt_t;

                        Q_ASSERT(f > 0);

                        double dx = tr_u.x_ - tr_l.x_;
                        double dy = tr_u.y_ - tr_l.y_;
                        double dz = tr_u.z_ - tr_l.z_;

                        double x_i = tr_l.x_ + f * dx;
                        double y_i = tr_l.y_ + f * dy;
                        double z_i = tr_l.z_ + f * dz;

                        tr_i.x_ = x_i;
                        tr_i.y_ = y_i;
                        tr_i.z_ = z_i;

                        // Add interpolated TargetReport to track.
                        t << tr_i;
                    }
                }
            }
        }
    }

    return t;
}

Track average(const Track &track, double tw)
{
    auto averagePos = [](const QVector<TargetReport> &vec) {
        double sum_x = 0.0;
        double sum_y = 0.0;
        //double sum_z = 0.0;

        quint32 N = 0;
        for (const TargetReport &tr : vec)
        {
            if (!qIsNaN(tr.x_) && !qIsNaN(tr.y_))
            {
                sum_x += tr.x_;
                sum_y += tr.y_;
                //sum_z += tr.z_;
                ++N;
            }
        }

        double x = sum_x / N;
        double y = sum_y / N;
        //double z = sum_z / N;

        return QPointF(x, y);
    };


    // Copy input track.
    Track trk = track;

    // Half time window.
    double h_tw = tw / 2;

    TgtRepMap::iterator it;
    for (it = trk.begin(); it != trk.end(); ++it)
    {
        QDateTime ts_pivot = it.key();
        QDateTime ts_from = ts_pivot.addMSecs(-h_tw * 1000);
        QDateTime ts_to = ts_pivot.addMSecs(h_tw * 1000);

        TgtRepMap::const_iterator cit_from = track.data().lowerBound(ts_from);
        TgtRepMap::const_iterator cit = cit_from;
        QVector<TargetReport> vec;
        while (cit != track.end() && cit.key() <= ts_to)
        {
            vec << cit.value();
            ++cit;
        }

        if (vec.size() >= 2)
        {
            QPointF pos = averagePos(vec);

            it->x_ = pos.x();
            it->y_ = pos.y();
            //it->z_ = pos.z();
        }

        // TODO: Update the X, Y and Z track bounds.
    }

    return trk;
}

QVector<Track> splitTrackByArea(const Track &trk, TrackSplitMode mode)
{
    auto areaChanged = [mode](const Aerodrome::NamedArea &lhs, const Aerodrome::NamedArea &rhs) {
        if (mode == TrackSplitMode::SplitByNamedArea)
        {
            if (lhs != rhs)
            {
                return true;
            }
        }
        else if (mode == TrackSplitMode::SplitByArea)
        {
            if (lhs.area_ != rhs.area_)
            {
                return true;
            }
        }

        return false;
    };

    QVector<Track> sub_trk_vec;

    Track sub_trk = trk.mode_s().has_value()
                        ? Track(trk.mode_s().value(), trk.system_type(), trk.track_number())
                        : Track(trk.system_type(), trk.track_number());

    Aerodrome::NamedArea last_na;

    bool first = true;
    for (const TargetReport &tr : trk)
    {
        if (first)
        {
            sub_trk << tr;
            last_na = tr.narea_;

            first = false;
            continue;
        }

        if (areaChanged(tr.narea_, last_na))
        {
            sub_trk_vec << sub_trk;
            sub_trk.clear();
        }

        sub_trk << tr;
        last_na = tr.narea_;
    }

    // Manual insertion of the last subtrack.
    sub_trk_vec << sub_trk;

    int n_tr = 0;
    for (const Track &sub_trk : qAsConst(sub_trk_vec))
    {
        Q_ASSERT(sub_trk.nareas().size() == 1);
        n_tr += sub_trk.size();
    }

    Q_ASSERT(n_tr == trk.size());

    return sub_trk_vec;
}
