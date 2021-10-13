/*!
 * \file targetreport.cpp
 * \brief
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

#include "targetreport.h"

bool operator==(const TargetReport &lhs, const TargetReport &rhs)
{
    int tol = 1;  // 1 m.

    auto closeEnough = [tol](double x1, double y1, double z1,
                           double x2, double y2, double z2) {
        return qSqrt(qPow(x1 - x2, 2) +
                     qPow(y1 - y2, 2) +
                     qPow(z1 - z2, 2)) < tol;
    };

    return lhs.ds_id_ == rhs.ds_id_ &&
           lhs.sys_typ_ == rhs.sys_typ_ &&
           lhs.tod_ == rhs.tod_ &&
           lhs.trk_nb_ == rhs.trk_nb_ &&
           lhs.mode_3a_ == rhs.mode_3a_ &&
           lhs.mode_s_ == rhs.mode_s_ &&
           lhs.ident_ == rhs.ident_ &&
           lhs.tgt_typ_ == rhs.tgt_typ_ &&
           lhs.on_gnd_ == rhs.on_gnd_ &&
           closeEnough(lhs.x_, lhs.y_, lhs.z_, rhs.x_, rhs.y_, rhs.z_) &&
           lhs.ver_ == rhs.ver_ &&
           lhs.pic_ == rhs.pic_ &&
           lhs.narea_ == rhs.narea_;
}
