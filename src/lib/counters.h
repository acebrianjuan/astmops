/*!
 * \file counters.h
 * \brief Interface of a set of counters to assist in the calculation
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

#ifndef ASTMOPS_COUNTERS_H
#define ASTMOPS_COUNTERS_H

#include "aerodrome.h"
#include <QDateTime>

namespace Counters
{
struct BasicCounter
{
    quint32 countValid = 0;
    quint32 countTotal = 0;

    void reset();
};

class IntervalCounter
{
public:
    IntervalCounter() = default;
    IntervalCounter(double period);
    IntervalCounter(double period, const QDateTime &tod);

    bool isInitialized() const;
    void reset();
    void setPeriod(double period);
    void init(const QDateTime &tod);
    void advance();
    bool contains(const QDateTime &tod) const;
    void update(const QDateTime &newTod);
    BasicCounter read();

    QDateTime intervalStart() const;
    QDateTime intervalEnd() const;

    operator bool() const;

private:
    double m_period = 1.0;
    QDateTime m_intervalStart;
    BasicCounter m_counter;
};

}  // namespace Counters

#endif  // ASTMOPS_COUNTERS_H
