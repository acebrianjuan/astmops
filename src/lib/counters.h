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
struct InOutCounter
{
    quint32 in_ = 0;
    quint32 out_ = 0;
};

struct BasicCounter
{
    quint32 valid_ = 0;
    quint32 total_ = 0;

    void reset();
};

class IntervalCounter
{
public:
    IntervalCounter() = default;
    IntervalCounter(double period);
    IntervalCounter(double period, const QDateTime &tod);

    bool isInitialized() const;
    QDateTime intervalStart() const;
    QDateTime intervalEnd() const;

    operator bool() const;

    void setPeriod(double period);
    void init(const QDateTime &tod);
    void update(const QDateTime &tod);
    void finish(const QDateTime &tod);

    void reset();

    BasicCounter read();

private:
    bool contains(const QDateTime &tod) const;
    void advance();

    double period_ = 1.0;
    QDateTime intervalStart_;
    BasicCounter counter_;
};

}  // namespace Counters

#endif  // ASTMOPS_COUNTERS_H
