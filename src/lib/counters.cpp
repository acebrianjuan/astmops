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

void Counters::BasicCounter::reset()
{
    countValid = 0;
    countTotal = 0;
}

Counters::IntervalCounter::IntervalCounter(double period)
{
    setPeriod(period);
}

Counters::IntervalCounter::IntervalCounter(double period, QDateTime tod)
{
    setPeriod(period);
    init(tod);
}

bool Counters::IntervalCounter::isInitialized() const { return m_intervalStart.isValid(); }

void Counters::IntervalCounter::reset()
{
    m_intervalStart = QDateTime();
    m_counter.reset();
}

void Counters::IntervalCounter::setPeriod(double period)
{
    if (period <= 0)
    {
        return;
    }

    m_period = period;
}

void Counters::IntervalCounter::init(QDateTime tod)
{
    m_intervalStart = tod;
    ++m_counter.countValid;
    advance();
}

void Counters::IntervalCounter::advance()
{
    m_intervalStart = intervalEnd();
    ++m_counter.countTotal;
}

bool Counters::IntervalCounter::contains(QDateTime tod) const
{
    if (intervalStart() <= tod && tod <= intervalEnd())
    {
        return true;
    }
    return false;
}

void Counters::IntervalCounter::update(QDateTime newTod)
{
    if (!isInitialized())
    {
        init(newTod);
        return;
    }

    if (newTod >= intervalStart())
    {
        while (!contains(newTod))
        {
            advance();
        }
        ++m_counter.countValid;

        advance();
    }
}

Counters::BasicCounter Counters::IntervalCounter::read()
{
    BasicCounter counter = m_counter;
    m_counter.reset();

    return counter;
}

QDateTime Counters::IntervalCounter::intervalStart() const
{
    return m_intervalStart;
}

QDateTime Counters::IntervalCounter::intervalEnd() const
{
    return m_intervalStart.addSecs(m_period);
}

Counters::IntervalCounter::operator bool() const
{
    return isInitialized();
}
