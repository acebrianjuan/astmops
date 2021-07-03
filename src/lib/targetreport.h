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

#include "aerodrome.h"
#include "asterix.h"
#include "astmops.h"
#include <QDateTime>
#include <optional>

struct TargetReport
{
    DataSrcId ds_id_;
    SystemType sys_typ_ = SystemType::Unknown;
    QDateTime tod_;

    TrackNum trk_nb_ = 0;

    std::optional<ModeS> mode_s_;
    std::optional<Mode3A> mode_3a_;
    std::optional<Ident> ident_;

    bool on_gnd_ = false;

    double x_ = qSNaN();
    double y_ = qSNaN();
    std::optional<double> z_;

    Aerodrome::NamedArea narea_;

    std::optional<quint8> ver_;
    std::optional<quint8> pic_;
};

Q_DECLARE_METATYPE(TargetReport);
Q_DECLARE_METATYPE(QVector<TargetReport>);

// FREE OPERATORS.
bool operator==(const TargetReport &lhs, const TargetReport &rhs);

#endif  // ASTMOPS_TARGETREPORT_H
