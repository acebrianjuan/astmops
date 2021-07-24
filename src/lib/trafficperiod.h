/*!
 * \file trafficperiod.h
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

#ifndef ASTMOPS_TRAFFICPERIOD_H
#define ASTMOPS_TRAFFICPERIOD_H

#include "astmops.h"
#include "track.h"

/*!
 * \brief The TrafficPeriod class is an abstraction that implements the
 * concept of a finite time interval in which a number of targets are present.
 */
class TrafficPeriod
{
public:
    TrafficPeriod() = default;
    TrafficPeriod(const QDateTime &begin, const QDateTime &end);
    TrafficPeriod(const QDateTime &begin, const QDateTime &end, QSet<ModeS> s);
    TrafficPeriod(const Track &trk);

    TrafficPeriod &operator<<(ModeS addr);
    TrafficPeriod &operator<<(const QVector<ModeS> &l);
    TrafficPeriod &operator<<(const QSet<ModeS> &l);

    void shrinkFront(const QDateTime &dt);
    void shrinkBack(const QDateTime &dt);

    QDateTime begin() const;
    QDateTime end() const;
    double duration() const;
    bool isValid() const;
    bool coversTimestamp(const QDateTime &dt) const;
    bool overlaps(const TrafficPeriod &other) const;
    QSet<ModeS> traffic() const;

private:
    QDateTime begin_;
    QDateTime end_;

    QSet<ModeS> traffic_;
};

Q_DECLARE_METATYPE(TrafficPeriod);
Q_DECLARE_METATYPE(QVector<TrafficPeriod>);


/*!
 * \brief The TimePeriodCollection class is an abstraction for collecting a
 * series of TrafficPeriod objects.
 */
class TrafficPeriodCollection
{
public:
    TrafficPeriodCollection() = default;

    TrafficPeriodCollection &operator<<(const TrafficPeriod &tp);
    TrafficPeriodCollection &operator<<(const QVector<TrafficPeriod> &l);

    TrafficPeriodCollection &operator<<(const Track &trk);
    TrafficPeriodCollection &operator<<(const TrackCollection &col);

    QVector<TrafficPeriod>::iterator begin() { return periods_.begin(); }
    QVector<TrafficPeriod>::iterator end() { return periods_.end(); }
    QVector<TrafficPeriod>::const_iterator begin() const { return periods_.constBegin(); }
    QVector<TrafficPeriod>::const_iterator end() const { return periods_.constEnd(); }

    bool coversTimestamp(const QDateTime &dt) const;
    bool overlaps(const TrafficPeriod &tp) const;
    bool isEmpty() const;
    int size() const;

    QDateTime beginTimestamp() const;
    QDateTime endTimestamp() const;

    void removeSmallPeriods(double min_duration);

private:
    void dealOverlap(TrafficPeriod &oldtp, TrafficPeriod &newtp);
    void sort();

    QVector<TrafficPeriod> periods_;
};

Q_DECLARE_METATYPE(TrafficPeriodCollection);


// FREE OPERATORS.
bool operator==(const TrafficPeriod &lhs, const TrafficPeriod &rhs);
bool operator<(const TrafficPeriod &lhs, const TrafficPeriod &rhs);

#endif  // ASTMOPS_TRAFFICPERIOD_H
