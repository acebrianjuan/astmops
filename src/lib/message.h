/*!
 * \file message.h
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

#ifndef ASTMOPS_MESSAGE_H
#define ASTMOPS_MESSAGE_H

#include "asterix.h"
#include "astmops.h"
#include <QDateTime>
#include <optional>

namespace Messages
{
class Message
{
public:
    Message();

    DataSrcId ds_id_;
    RecordType rec_typ_;
    QDateTime tod_;
};

class ServiceMessage : public Message
{
public:
    ServiceMessage();

    ServiceMessageType srv_msg_type_;
};

class TargetReport : public Message
{
public:
    TargetReport();

    bool hasPosition() const;

    std::optional<Mode3A> mode_3A_;
    std::optional<ModeS> mode_S_;
    std::optional<Ident> ident_;

    std::optional<bool> on_gnd_;
    std::optional<double> x_;
    std::optional<double> y_;
    std::optional<double> z_;
};

// FREE OPERATORS.
bool operator==(const TargetReport &lhs, const TargetReport &rhs);

};  // namespace Messages

#endif  // ASTMOPS_MESSAGE_H
