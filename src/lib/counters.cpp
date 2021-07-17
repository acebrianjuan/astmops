/*!
 * \file counters.cpp
 * \brief Implementation of a set of counters to assist in the calculation
 * of the performance indicators.
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

#include "counters.h"

/* ----------------------------- BasicCounter ----------------------------- */

void Counters::BasicCounter::reset()
{
    valid_ = 0;
    total_ = 0;
}

/* ---------------------------- IntervalCounter --------------------------- */

Counters::IntervalCounter::IntervalCounter(double period)
{
    setPeriod(period);
}

Counters::IntervalCounter::IntervalCounter(double period, const QDateTime &tod)
{
    setPeriod(period);
    init(tod);
}

bool Counters::IntervalCounter::isInitialized() const
{
    return intervalStart_.isValid();
}

QDateTime Counters::IntervalCounter::intervalStart() const
{
    return intervalStart_;
}

QDateTime Counters::IntervalCounter::intervalEnd() const
{
    return intervalStart_.addMSecs(period_ * 1000.0);
}

Counters::IntervalCounter::operator bool() const
{
    return isInitialized();
}

void Counters::IntervalCounter::setPeriod(double period)
{
    if (period <= 0)
    {
        return;
    }

    period_ = period;
}

void Counters::IntervalCounter::init(const QDateTime &tod)
{
    intervalStart_ = tod;
}

void Counters::IntervalCounter::update(const QDateTime &tod)
{
    if (!isInitialized())
    {
        // qWarning();
        return;
    }

    if (tod >= intervalStart())
    {
        while (!contains(tod))
        {
            advance();
        }

        ++counter_.valid_;
        advance();
    }
}

void Counters::IntervalCounter::finish(const QDateTime &tod)
{
    if (!isInitialized())
    {
        // qWarning();
        return;
    }

    if (tod >= intervalStart())
    {
        while (!contains(tod))
        {
            advance();
        }
    }
}

void Counters::IntervalCounter::reset()
{
    intervalStart_ = QDateTime();
    counter_.reset();
}

Counters::BasicCounter Counters::IntervalCounter::read()
{
    BasicCounter counter = counter_;
    counter_.reset();

    return counter;
}

bool Counters::IntervalCounter::contains(const QDateTime &tod) const
{
    if (intervalStart() <= tod && tod < intervalEnd())
    {
        return true;
    }

    return false;
}

void Counters::IntervalCounter::advance()
{
    intervalStart_ = intervalEnd();
    ++counter_.total_;
}

/* ---------------------------- Free functions ---------------------------- */

bool Counters::operator==(Counters::UrCounter lhs, Counters::UrCounter rhs)
{
    return lhs.n_etrp_ == rhs.n_etrp_ &&
           lhs.n_trp_ == rhs.n_trp_;
}

bool Counters::operator==(Counters::PdCounter lhs, Counters::PdCounter rhs)
{
    return lhs.n_trp_ == rhs.n_trp_ &&
           lhs.n_up_ == rhs.n_up_;
}
