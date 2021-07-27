/*!
 * \file trafficperiod.cpp
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


#include "trafficperiod.h"

/* ----------------------------- TrafficPeriod ---------------------------- */

TrafficPeriod::TrafficPeriod(const Track &trk)
{
    if (trk.beginTimestamp().isValid() &&
        trk.endTimestamp().isValid() &&
        trk.mode_s().has_value())
    {
        beginTimestamp_ = trk.beginTimestamp();
        endTimestamp_ = trk.endTimestamp();

        traffic_ << trk.mode_s().value();
    }
}

TrafficPeriod::TrafficPeriod(const QDateTime &begin, const QDateTime &end)
{
    if (begin.isValid() && end.isValid() && begin < end)
    {
        beginTimestamp_ = begin;
        endTimestamp_ = end;
    }
}

TrafficPeriod::TrafficPeriod(const QDateTime &begin, const QDateTime &end, const QSet<ModeS> &s)
    : traffic_(s)
{
    if (begin.isValid() && end.isValid() && begin < end)
    {
        beginTimestamp_ = begin;
        endTimestamp_ = end;
    }
}

TrafficPeriod &TrafficPeriod::operator<<(ModeS addr)
{
    traffic_ << addr;

    return *this;
}

TrafficPeriod &TrafficPeriod::operator<<(const QVector<ModeS> &l)
{
    for (ModeS addr : l)
    {
        *this << addr;
    }

    return *this;
}

TrafficPeriod &TrafficPeriod::operator<<(const QSet<ModeS> &l)
{
    for (ModeS addr : l)
    {
        *this << addr;
    }

    return *this;
}

void TrafficPeriod::shrinkFront(const QDateTime &dt)
{
    if (beginTimestamp_.isValid() && endTimestamp_.isValid() && dt.isValid() &&
        dt > beginTimestamp_ && dt < endTimestamp_)
    {
        beginTimestamp_ = dt;
    }
}

void TrafficPeriod::shrinkBack(const QDateTime &dt)
{
    if (beginTimestamp_.isValid() && endTimestamp_.isValid() && dt.isValid() &&
        dt > beginTimestamp_ && dt < endTimestamp_)
    {
        endTimestamp_ = dt;
    }
}

QDateTime TrafficPeriod::beginTimestamp() const
{
    return beginTimestamp_;
}

QDateTime TrafficPeriod::endTimestamp() const
{
    return endTimestamp_;
}

double TrafficPeriod::duration() const
{
    return beginTimestamp_.msecsTo(endTimestamp_) / 1000.0;
}

quint32 TrafficPeriod::trafficCount() const
{
    return traffic_.count();
}

quint32 TrafficPeriod::expectedUpdates(double freq) const
{
    return freq * duration();
}

quint32 TrafficPeriod::expectedTgtReps(double freq) const
{
    return freq * duration() * trafficCount();
}

bool TrafficPeriod::isValid() const
{
    return beginTimestamp_.isValid() && endTimestamp_.isValid() && !traffic_.isEmpty();
}

bool TrafficPeriod::coversTimestamp(const QDateTime &dt) const
{
    if (!dt.isValid())
    {
        return false;
    }

    if (dt >= beginTimestamp_ && dt <= endTimestamp_)
    {
        return true;
    }

    return false;
}

bool TrafficPeriod::overlaps(const TrafficPeriod &other) const
{
    return beginTimestamp_ < other.endTimestamp() &&
           other.beginTimestamp() < endTimestamp_;
}

bool TrafficPeriod::hasTarget(ModeS addr)
{
    return traffic_.contains(addr);
}

QSet<ModeS> TrafficPeriod::traffic() const
{
    return traffic_;
}

/* ----------------------- TrafficPeriodCollection ------------------------ */

TrafficPeriodCollection &TrafficPeriodCollection::operator<<(const TrafficPeriod &tp)
{
    if (!tp.isValid())
    {
        return *this;
    }

    // Make a mutable (non-const) copy of the input period.
    TrafficPeriod mtp = tp;

    auto findFirstOverlap = [this](const TrafficPeriod &tp) {
        QVector<TrafficPeriod>::iterator it;
        for (it = periods_.begin(); it != periods_.end(); ++it)
        {
            if (tp.overlaps(*it))
            {
                return it;
            }
        }

        return periods_.end();
    };


    // Add first period directly.
    if (periods_.isEmpty())
    {
        periods_ << mtp;
    }
    else
    {
        if (overlaps(mtp))
        {
            QVector<TrafficPeriod>::iterator it = findFirstOverlap(mtp);
            Q_ASSERT(it != periods_.end());

            dealOverlap(*it, mtp);

            //periods_.insert(it, );
        }
        else
        {
            periods_ << mtp;
        }

        // Sort time periods chronologically.
        sort();
    }

    return *this;
}

TrafficPeriodCollection &TrafficPeriodCollection::operator<<(const QVector<TrafficPeriod> &l)
{
    for (const TrafficPeriod &tp : l)
    {
        *this << tp;
    }

    return *this;
}

TrafficPeriodCollection &TrafficPeriodCollection::operator<<(const TrafficPeriodCollection &col)
{
    for (const TrafficPeriod &tp : col)
    {
        *this << tp;
    }

    return *this;
}

TrafficPeriodCollection &TrafficPeriodCollection::operator<<(const Track &trk)
{
    *this << TrafficPeriod(trk);

    return *this;
}

TrafficPeriodCollection &TrafficPeriodCollection::operator<<(const TrackCollection &col)
{
    for (const Track &trk : col)
    {
        *this << trk;
    }

    return *this;
}

double TrafficPeriodCollection::duration() const
{
    double dur = 0;
    for (const TrafficPeriod &tp : periods_)
    {
        dur += tp.duration();
    }

    return dur;
}

quint32 TrafficPeriodCollection::expectedUpdates(double freq) const
{
    return freq * duration();
}

quint32 TrafficPeriodCollection::expectedTgtReps(double freq) const
{
    quint32 etr = 0;
    for (const TrafficPeriod &tp : periods_)
    {
        etr += tp.expectedTgtReps(freq);
    }

    return etr;
}

bool TrafficPeriodCollection::coversTimestamp(const QDateTime &dt) const
{
    if (!dt.isValid())
    {
        return false;
    }

    for (const TrafficPeriod &tp : periods_)
    {
        if (tp.coversTimestamp(dt))
        {
            return true;
        }
    }

    return false;
}

bool TrafficPeriodCollection::overlaps(const TrafficPeriod &tp) const
{
    for (const TrafficPeriod &otp : periods_)
    {
        if (tp.overlaps(otp))
        {
            return true;
        }
    }

    return false;
}

bool TrafficPeriodCollection::isEmpty() const
{
    return periods_.isEmpty();
}

int TrafficPeriodCollection::size() const
{
    return periods_.size();
}

QDateTime TrafficPeriodCollection::beginTimestamp() const
{
    if (periods_.isEmpty())
    {
        return QDateTime();
    }

    return periods_.first().beginTimestamp();
}

QDateTime TrafficPeriodCollection::endTimestamp() const
{
    if (periods_.isEmpty())
    {
        return QDateTime();
    }

    return periods_.last().endTimestamp();
}

void TrafficPeriodCollection::removeSmallPeriods(double min_duration)
{
    QVector<TrafficPeriod>::iterator it = periods_.begin();
    while (it != periods_.end())
    {
        TrafficPeriod tp = *it;
        if (tp.duration() < min_duration)
        {
            it = periods_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void TrafficPeriodCollection::dealOverlap(TrafficPeriod &oldtp, TrafficPeriod &newtp)
{
    Q_ASSERT(newtp.overlaps(oldtp));

    if (oldtp.beginTimestamp() == newtp.beginTimestamp() &&
        oldtp.endTimestamp() == newtp.endTimestamp())
    {
        // CASE 0: Old period and new period have identical times.

        // Simply add the traffic contents of the new period to the old one.
        oldtp << newtp.traffic();
    }
    else if (oldtp.beginTimestamp() == newtp.beginTimestamp())
    {
        if (newtp.endTimestamp() < oldtp.endTimestamp())
        {
            // CASE 1: Old period and new period start at the same time.
            //         New period ends before old period.

            // Create intersection period.
            TrafficPeriod itp(newtp.beginTimestamp(), newtp.endTimestamp());
            itp << oldtp.traffic() << newtp.traffic();

            // Shrink old period from the front to match the end of the
            // intersection period.
            oldtp.shrinkFront(itp.endTimestamp());

            if (itp.isValid())
            {
                // Insert intersection period.
                *this << itp;
            }
        }
        else
        {
            // CASE 2: Old period and new period start at the same time.
            //         New period ends after old period.

            // Add the traffic contents of the new period to the old one.
            oldtp << newtp.traffic();

            // Create remainder of the new period by copying the new period
            // and shrinking it from the front to match the end of the
            // intersection period.
            TrafficPeriod rnewtp(newtp);
            rnewtp.shrinkFront(oldtp.endTimestamp());

            if (rnewtp.isValid())
            {
                // Insert remainder of the new period.
                *this << rnewtp;
            }
        }
    }
    else if (oldtp.endTimestamp() == newtp.endTimestamp())
    {
        if (oldtp.beginTimestamp() < newtp.beginTimestamp())
        {
            // CASE 3: Old period and new period end at the same time.
            //         New period starts after old period.

            // Create intersection period.
            TrafficPeriod itp(newtp.beginTimestamp(), newtp.endTimestamp());
            itp << oldtp.traffic() << newtp.traffic();

            // Shrink old period from the back to match the beginning of the
            // intersection period.
            oldtp.shrinkBack(itp.beginTimestamp());

            if (itp.isValid())
            {
                // Insert intersection period.
                *this << itp;
            }
        }
        else
        {
            // CASE 4: Old period and new period end at the same time.
            //         New period starts before old period.

            // Add the traffic contents of the new period to the old one.
            oldtp << newtp.traffic();

            // Create remainder of the new period by copying the new period
            // and shrinking it from the back to match the beginning of the
            // intersection period.
            TrafficPeriod rnewtp(newtp);
            rnewtp.shrinkBack(oldtp.beginTimestamp());

            if (rnewtp.isValid())
            {
                // Insert remainder of the new period.
                *this << rnewtp;
            }
        }
    }
    else if (oldtp.beginTimestamp() < newtp.beginTimestamp())
    {
        if (newtp.endTimestamp() > oldtp.endTimestamp())
        {
            // CASE 5: Old period starts before new period.
            //         New period ends after old period.

            // Create intersection period.
            TrafficPeriod itp(newtp.beginTimestamp(), oldtp.endTimestamp());
            itp << oldtp.traffic() << newtp.traffic();

            // Shrink old period from the back to match the beginning of the
            // intersection period.
            oldtp.shrinkBack(itp.beginTimestamp());

            // Shrink new period from the front to match the end of the
            // intersection period.
            newtp.shrinkFront(itp.endTimestamp());

            if (itp.isValid() && newtp.isValid())
            {
                // Insert intersection period and remainder of the new period.
                *this << itp << newtp;
            }
        }
        else
        {
            // CASE 6: Old period starts before and ends after new period.

            // Create intersection period by copying the new period and adding
            // the traffic contents of the old period.
            TrafficPeriod itp(newtp);
            itp << oldtp.traffic();

            // Create remainder of the old period by copying the old period
            // and shrinking it from the front to match the end of the
            // intersection period.
            TrafficPeriod roldtp(oldtp);
            roldtp.shrinkFront(itp.endTimestamp());

            // Shrink old period from the front to match the beginning of the
            // intersection period.
            oldtp.shrinkBack(newtp.beginTimestamp());

            if (itp.isValid() && roldtp.isValid())
            {
                // Insert intersection period and remainder of the old period.
                *this << itp << roldtp;
            }
        }
    }
    else if (newtp.beginTimestamp() < oldtp.beginTimestamp())
    {
        if (newtp.endTimestamp() < oldtp.endTimestamp())
        {
            // CASE 7: New period starts before old period.
            //         Old period ends after new period.

            // Create intersection period.
            TrafficPeriod itp(oldtp.beginTimestamp(), newtp.endTimestamp());
            itp << oldtp.traffic() << newtp.traffic();

            // Shrink old period from the back to match the beginning of the
            // intersection period.
            oldtp.shrinkFront(itp.endTimestamp());

            // Shrink new period from the front to match the beginning of the
            // intersection period.
            newtp.shrinkBack(itp.beginTimestamp());

            if (itp.isValid() && newtp.isValid())
            {
                // Insert intersection and remainder of the new period.
                *this << itp << newtp;
            }
        }
        else
        {
            // CASE 8: New period starts before and ends after old period.

            // Add the traffic contents of the new period to the old one.
            oldtp << newtp.traffic();

            // Create remainder of the new period by copying the new period
            // and shrinking it from the front to match the end of the
            // intersection period.
            TrafficPeriod rnewtp(newtp);
            rnewtp.shrinkFront(oldtp.endTimestamp());

            // Shrink new period from the back to match the beginning of the
            // old period.
            newtp.shrinkBack(oldtp.beginTimestamp());

            if (newtp.isValid() && rnewtp.isValid())
            {
                // Insert new period and remainder of the new period.
                *this << newtp << rnewtp;
            }
        }
    }
    else
    {
        // This should not happen.
        // qWarning();
    }
}

void TrafficPeriodCollection::sort()
{
    std::sort(periods_.begin(), periods_.end());
}

/* ---------------------------- Free functions ---------------------------- */

bool operator==(const TrafficPeriod &lhs, const TrafficPeriod &rhs)
{
    return lhs.beginTimestamp() == rhs.beginTimestamp() &&
           lhs.endTimestamp() == rhs.endTimestamp() &&
           lhs.traffic() == rhs.traffic();
}

bool operator<(const TrafficPeriod &lhs, const TrafficPeriod &rhs)
{
    return lhs.beginTimestamp() < rhs.beginTimestamp();
}
