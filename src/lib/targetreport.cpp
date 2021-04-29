/*!
 * \file targetreport.cpp
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

#include "targetreport.h"

bool operator==(const TargetReport &lhs, const TargetReport &rhs)
{
    int tol = 1;  // 1 m.

#if 0
    auto compare = [tol](std::optional<double> d_lhs, std::optional<double> d_rhs) {
        return d_lhs.has_value() == d_rhs.has_value() &&
               (!d_lhs || /*qAbs(*d_lhs - *d_rhs) < tol*/ qRound(*d_lhs * tol) == qRound(*d_rhs * tol));
    };
#endif

    auto closeEnough = [tol](double x1, double y1, double z1,
                           double x2, double y2, double z2) {
        return qSqrt(qPow(x1 - x2, 2) +
                     qPow(y1 - y2, 2) +
                     qPow(z1 - z2, 2)) < tol;
    };

    return closeEnough(lhs.x_, lhs.y_, lhs.z_, rhs.x_, rhs.y_, rhs.z_) &&
           lhs.on_gnd_ == rhs.on_gnd_ &&
           lhs.mode_3A_ == rhs.mode_3A_ &&
           lhs.mode_S_ == rhs.mode_S_ &&
           lhs.ident_ == rhs.ident_;
}
