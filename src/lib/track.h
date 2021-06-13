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
 * A Track object contains a history of TargetReport objects indexed
 * chronologically by their timestamp. By definition, all TargetReport objects
 * that belong to the same Track have the same track number in common.
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

    TgtRepMap::iterator begin() { return data_.begin(); }
    TgtRepMap::iterator end() { return data_.end(); }
    TgtRepMap::const_iterator begin() const { return data_.constBegin(); }
    TgtRepMap::const_iterator end() const { return data_.constEnd(); }

    SystemType system_type() const;
    TrackNum track_number() const;
    TgtRepMap &rdata();
    const TgtRepMap &data() const;
    std::optional<ModeS> mode_s() const;

    QPair<double, double> x_bounds() const;
    QPair<double, double> y_bounds() const;
    QPair<double, double> z_bounds() const;

    bool isEmpty() const;
    int size() const;

    QVector<QDateTime> timestamps() const;
    QDateTime beginDateTime() const;
    QDateTime endDateTime() const;
    double duration() const;
    bool coversDateTime(const QDateTime &tod) const;

    void intersect(const Track &other);

    void setMode_s(ModeS ms);

private:
    SystemType system_type_;

    TrackNum track_number_;
    TgtRepMap data_;

    QDateTime beginDateTime_;
    QDateTime endDateTime_;

    QPair<double, double> x_bounds_ = {qSNaN(), qSNaN()};
    QPair<double, double> y_bounds_ = {qSNaN(), qSNaN()};
    QPair<double, double> z_bounds_ = {qSNaN(), qSNaN()};

    std::optional<ModeS> mode_s_;
};

Q_DECLARE_METATYPE(Track);
Q_DECLARE_METATYPE(QVector<Track>);

/*!
 * \brief The TrackCollection class is an abstraction for collecting a series
 * of Track objects of a given SystemType that belong to the same target.
 *
 * A TrackCollection object contains a history of Track objects indexed
 * chronologically by the timestamp of the first TargetReport of the Track.
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

    QMultiMap<QDateTime, Track>::iterator begin() { return tracks_.begin(); }
    QMultiMap<QDateTime, Track>::iterator end() { return tracks_.end(); }
    QMultiMap<QDateTime, Track>::const_iterator begin() const { return tracks_.constBegin(); }
    QMultiMap<QDateTime, Track>::const_iterator end() const { return tracks_.constEnd(); }

    SystemType system_type() const;
    QVector<TrackNum> track_numbers() const;
    QVector<Track> tracks() const;

    bool isEmpty() const;
    int size() const;

    std::optional<Track> track(const TrackNum tn) const;
    TrackCollection tracks(const QVector<TrackNum> &v) const;
    bool containsTrackNumber(const TrackNum tn) const;

    QDateTime beginDateTime() const;
    QDateTime endDateTime() const;
    bool coversDateTime(const QDateTime &tod) const;

    std::optional<ModeS> mode_s() const;
    void setMode_s(ModeS ms);

private:
    SystemType system_type_;

    QVector<TrackNum> track_numbers_;
    QMultiMap<QDateTime, Track> tracks_;

    QDateTime beginDateTime_;
    QDateTime endDateTime_;

    std::optional<ModeS> mode_s_;
};

Q_DECLARE_METATYPE(TrackCollection);
Q_DECLARE_METATYPE(QVector<TrackCollection>);

/*!
 * \brief The TrackCollectionSet class is an abstraction for grouping a
 * reference TrackCollection object with the associated test TrackCollection
 * objects of different SystemType that belong to the same target.
 */
class TrackCollectionSet
{
public:
    TrackCollectionSet() = default;
    TrackCollectionSet(ModeS mode_s, SystemType ref_st);
    TrackCollectionSet(ModeS mode_s, SystemType ref_st, const QVector<TrackCollection> &cols);

    TrackCollectionSet &operator<<(const Track &t);
    TrackCollectionSet &operator<<(const TrackCollection &c);

    void addMatch(const Track &t_ref, const Track &t_tst);

    QVector<TrackCollection> tstTrackCols() const;
    TrackCollection refTrackCol() const;

    QVector<TrackCollection> getMatches(TrackNum ref_tn) const;

    std::optional<TrackCollection> collection(SystemType st) const;
    bool hasCollection(SystemType st) const;
    bool hasRefData() const;
    bool hasTestData() const;
    bool isValid() const;
    bool isEmpty() const;
    int size() const;

    ModeS mode_s() const;
    SystemType ref_sys_type() const;

    void setMode_s(ModeS mode_s);
    void setRef_sys_type(SystemType ref_sys_type);

private:
    bool containsTrack(SystemType st, TrackNum tn) const;
    bool containsMatch(SystemType st, TrackNum ref_tn, TrackNum tst_tn);
    bool containsMatch(const Track &t_ref, const Track &t_tst);

    ModeS mode_s_ = 0xFFFFFF;
    SystemType ref_sys_type_ = SystemType::Unknown;

    QHash<SystemType, TrackCollection> tst_cols_;
    TrackCollection ref_col_;

    QHash<SystemType, QHash<TrackNum, QVector<TrackNum>>> matches_;
};

Q_DECLARE_METATYPE(TrackCollectionSet);
Q_DECLARE_METATYPE(QVector<TrackCollectionSet>);


bool operator==(const Track &lhs, const Track &rhs);
bool operator==(const TrackCollection &lhs, const TrackCollection &rhs);

bool haveTimeIntersection(const Track &lhs, const Track &rhs);
bool haveSpaceIntersection(const Track &lhs, const Track &rhs);
bool haveSpaceTimeIntersection(const Track &lhs, const Track &rhs);
std::optional<Track> intersect(const Track &intersectee, const Track &intersector);
Track resample(const Track &track, const QVector<QDateTime> &dtimes);

#endif  // ASTMOPS_TRACK_H
