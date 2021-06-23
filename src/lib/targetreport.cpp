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

    auto closeEnough = [tol](double x1, double y1, std::optional<double> z1,
                           double x2, double y2, std::optional<double> z2) {
        if (z1.has_value() != z2.has_value())
        {
            return false;
        }

        if (z1.has_value() && z2.has_value())
        {
            return qSqrt(qPow(x1 - x2, 2) +
                         qPow(y1 - y2, 2) +
                         qPow(z1.value() - z2.value(), 2)) < tol;
        }
        else
        {
            return qSqrt(qPow(x1 - x2, 2) +
                         qPow(y1 - y2, 2)) < tol;
        }
    };

    return lhs.ds_id_ == rhs.ds_id_ &&
           lhs.sys_typ_ == rhs.sys_typ_ &&
           lhs.tod_ == rhs.tod_ &&
           lhs.trk_nb_ == rhs.trk_nb_ &&
           lhs.mode_3a_ == rhs.mode_3a_ &&
           lhs.mode_s_ == rhs.mode_s_ &&
           lhs.ident_ == rhs.ident_ &&
           lhs.on_gnd_ == rhs.on_gnd_ &&
           closeEnough(lhs.x_, lhs.y_, lhs.z_, rhs.x_, rhs.y_, rhs.z_) &&
           lhs.ver_ == rhs.ver_ &&
           lhs.pic_ == rhs.pic_ &&
           lhs.area_ == rhs.area_;
}
