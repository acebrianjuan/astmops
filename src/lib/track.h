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

using TgtRepMap = QMultiMap<QDateTime, TargetReport>;

/*!
 * \brief The Track class is an abstraction that implements the concept of a
 * radar track, which is a continuous sequence of plots for a given target.
 *
 * A Track object contains a history of TargetReport objects indexed by their
 * timestamp. By definition, all TargetReport objects that belong to the same
 * Track have the same track number in common.
 */
class Track
{
public:
    Track() = default;
    Track(SystemType st, TrackNum tn);
    Track(SystemType st, TrackNum tn, const QVector<TargetReport> &tr);

    Track(ModeS ms, SystemType st, TrackNum tn);
    Track(ModeS ms, SystemType st, TrackNum tn, const QVector<TargetReport> &tr);

    Track &operator<<(const TargetReport &tr);
    Track &operator<<(const QVector<TargetReport> &l);

    SystemType system_type() const;
    TrackNum track_number() const;
    TgtRepMap data() const;
    std::optional<ModeS> mode_s() const;

    QPair<double, double> x_bounds() const;
    QPair<double, double> y_bounds() const;
    QPair<double, double> z_bounds() const;

    bool isEmpty() const;
    int size() const;

    QDateTime startDateTime() const;
    QDateTime endDateTime() const;
    double duration() const;
    bool coversDateTime(const QDateTime &tod) const;

    void intersect(const Track &other);

    void setMode_s(ModeS ms);

private:
    SystemType system_type_;

    TrackNum track_number_;
    TgtRepMap data_;

    QDateTime startDateTime_;
    QDateTime endDateTime_;

    QPair<double, double> x_bounds_ = {qSNaN(), qSNaN()};
    QPair<double, double> y_bounds_ = {qSNaN(), qSNaN()};
    QPair<double, double> z_bounds_ = {qSNaN(), qSNaN()};

    std::optional<ModeS> mode_s_;
};

Q_DECLARE_METATYPE(Track);

/*!
 * \brief The TrackCollection class is an abstraction for collecting a series
 * of Track objects of a given SystemType that are related because they belong
 * to the same target.
 */
class TrackCollection
{
public:
    TrackCollection() = default;
    TrackCollection(SystemType st);
    TrackCollection(SystemType st, const Track &t);
    TrackCollection(SystemType st, const QVector<Track> &l);

    TrackCollection(ModeS ms, SystemType st);
    TrackCollection(ModeS ms, SystemType st, const Track &t);
    TrackCollection(ModeS ms, SystemType st, const QVector<Track> &l);


    TrackCollection &operator<<(const Track &t);
    TrackCollection &operator<<(const QVector<Track> &l);

    SystemType system_type() const;
    QVector<TrackNum> track_numbers() const;
    QVector<Track> tracks() const;

    bool isEmpty() const;
    int size() const;

    std::optional<Track> track(const TrackNum tn) const;
    bool containsTrackNumber(const TrackNum tn) const;

    QDateTime startDateTime() const;
    QDateTime endDateTime() const;
    bool coversDateTime(const QDateTime &tod) const;

    std::optional<ModeS> mode_s() const;
    void setMode_s(ModeS ms);

private:
    SystemType system_type_;

    QVector<TrackNum> track_numbers_;
    QMultiMap<QDateTime, Track> tracks_;

    QDateTime startDateTime_;
    QDateTime endDateTime_;

    std::optional<ModeS> mode_s_;
};

Q_DECLARE_METATYPE(TrackCollection);

/*!
 * \brief The TrackCollectionSet class is an abstraction for collecting a
 * series of TrackCollection objects of different SystemType that are related
 * because they belong to the same target.
 */
class TrackCollectionSet
{
public:
    TrackCollectionSet() = default;
    TrackCollectionSet(ModeS mode_s);
    TrackCollectionSet(ModeS mode_s, const QVector<TrackCollection> &cols);

    TrackCollectionSet &operator<<(const Track &t);
    TrackCollectionSet &operator<<(const TrackCollection &c);

    bool isEmpty() const;
    int size() const;

    ModeS mode_s() const;

private:
    ModeS mode_s_ = 0xFFFFFF;
    QHash<SystemType, TrackCollection> cols_;
};

Q_DECLARE_METATYPE(TrackCollectionSet);

bool operator==(const Track &lhs, const Track &rhs);
bool operator==(const TrackCollection &lhs, const TrackCollection &rhs);

bool haveTimeIntersection(const Track &lhs, const Track &rhs);
bool haveSpaceIntersection(const Track &lhs, const Track &rhs);
bool haveSpaceTimeIntersection(const Track &lhs, const Track &rhs);
std::optional<Track> intersect(const Track &intersectee, const Track &intersector);
Track resample(const Track &track, const QVector<QDateTime> &dtimes);

#endif  // ASTMOPS_TRACK_H
