/*!
 * \file targetreport.h
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

#ifndef ASTMOPS_TARGETREPORT_H
#define ASTMOPS_TARGETREPORT_H

#include "asterix.h"
#include "astmops.h"
#include <QDateTime>
#include <optional>

struct TargetReport
{
    DataSrcId ds_id_;
    RecordType rec_typ_;
    QDateTime tod_;

    TrackNum trk_nb_;

    std::optional<Mode3A> mode_3A_;
    std::optional<ModeS> mode_S_;
    std::optional<Ident> ident_;

    std::optional<bool> on_gnd_;
    double x_;
    double y_;
    double z_;
};

Q_DECLARE_METATYPE(TargetReport);

// FREE OPERATORS.
bool operator==(const TargetReport &lhs, const TargetReport &rhs);

#endif  // ASTMOPS_TARGETREPORT_H
