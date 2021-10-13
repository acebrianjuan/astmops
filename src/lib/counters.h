/*!
 * \file counters.h
 * \brief Interface of a set of counters to assist in the calculation
 * of the performance indicators.
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

#ifndef ASTMOPS_COUNTERS_H
#define ASTMOPS_COUNTERS_H

#include "aerodrome.h"
#include <QDateTime>

namespace Counters
{
struct UrCounter
{
    quint32 n_trp_ = 0;
    quint32 n_etrp_ = 0;
};

struct PdCounter
{
    quint32 n_trp_ = 0;
    quint32 n_up_ = 0;
};

struct PfdCounter
{
    quint32 n_ftr_ = 0;
    quint32 n_tr_ = 0;
};

struct PfdCounter2
{
    quint32 n_tr_ = 0;
    quint32 n_etr_ = 0;
    quint32 n_u_ = 0;
};

struct PidCounter
{
    quint32 n_citr_ = 0;
    quint32 n_itr_ = 0;
};

struct PfidCounter
{
    quint32 n_eitr_ = 0;
    quint32 n_itr_ = 0;
};

struct PlgCounter
{
    quint32 n_g_ = 0;
    quint32 n_tr_ = 0;
};

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

// FREE OPERATORS.
bool operator==(Counters::UrCounter lhs, Counters::UrCounter rhs);
bool operator==(Counters::PdCounter lhs, Counters::PdCounter rhs);
bool operator==(Counters::PfdCounter lhs, Counters::PfdCounter rhs);
bool operator==(Counters::PfdCounter2 lhs, Counters::PfdCounter2 rhs);
bool operator==(Counters::PidCounter lhs, Counters::PidCounter rhs);
bool operator==(Counters::PfidCounter lhs, Counters::PfidCounter rhs);
bool operator==(Counters::PlgCounter lhs, Counters::PlgCounter rhs);

}  // namespace Counters

Q_DECLARE_METATYPE(Counters::UrCounter);
Q_DECLARE_METATYPE(Counters::PdCounter);
Q_DECLARE_METATYPE(Counters::PfdCounter);
Q_DECLARE_METATYPE(Counters::PfdCounter2);
Q_DECLARE_METATYPE(Counters::PidCounter);
Q_DECLARE_METATYPE(Counters::PfidCounter);
Q_DECLARE_METATYPE(Counters::PlgCounter);


#endif  // ASTMOPS_COUNTERS_H
