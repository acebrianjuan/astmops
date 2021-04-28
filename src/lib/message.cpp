/*!
 * \file message.cpp
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

#include "message.h"

Messages::Message::Message()
{
}

Messages::ServiceMessage::ServiceMessage()
{
}

Messages::TargetReport::TargetReport()
{
}

bool Messages::TargetReport::hasPosition() const
{
    return x_.has_value() && y_.has_value() && z_.has_value();
}

bool Messages::operator==(const Messages::TargetReport &lhs, const Messages::TargetReport &rhs)
{
    int tol = 1;  // 1 m.
    auto compare = [tol](std::optional<double> d_lhs, std::optional<double> d_rhs) {
        return d_lhs.has_value() == d_rhs.has_value() &&
               (!d_lhs || qAbs(*d_lhs - *d_rhs) < tol);
    };

    return compare(lhs.x_, rhs.x_) &&
           compare(lhs.y_, rhs.y_) &&
           compare(lhs.z_, rhs.z_) &&
           lhs.on_gnd_ == rhs.on_gnd_ &&
           lhs.mode_3A_ == rhs.mode_3A_ &&
           lhs.mode_S_ == rhs.mode_S_ &&
           lhs.ident_ == rhs.ident_;
}
