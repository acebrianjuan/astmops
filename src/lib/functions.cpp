/*!
 * \file functions.cpp
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

#include "functions.h"
#include <cmath>
#include <numeric>

double percentile(QVector<double> v, double percent)
{
    //Q_ASSERT(!v.isEmpty() && percent <= 100);

    if (v.isEmpty() || percent < 0 || percent > 100)
    {
        return qSNaN();  // TODO: Should throw a warning here.
    }

    if (v.size() == 1)
    {
        return v.first();
    }

    std::sort(v.begin(), v.end());

    if (percent == 0)
    {
        return v.first();
    }
    if (percent == 100)
    {
        return v.last();
    }

    const int numRecords = v.size();
    double rank = percent / 100.0 * numRecords;

    if (rank >= numRecords - 1)
    {
        return v.last();
    }

    double intPart;
    double fractPart = std::modf(rank, &intPart);

    int idx = static_cast<int>(intPart) - 1;

    if (fractPart != 0)
    {
        return v.at(idx + 1);
    }
    else
    {
        return (v.at(idx) + v.at(idx + 1)) / 2.0;
    }
}

double mean(const QVector<double> &v)
{
    int N = v.size();

    if (N == 0)
    {
        return qSNaN();
    }

    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / N;

    return mean;
}

double stdDev(const QVector<double> &v)
{
    int N = v.size();

    if (N == 0)
    {
        return qSNaN();
    }

    if (N == 1)
    {
        return 0.0;
    }

    double mn = mean(v);

    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), [mn](double x) { return x - mn; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / (N - 1));

    return stdev;
}
