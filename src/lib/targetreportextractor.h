/*!
 * \file targetreportextractor.h
 * \brief Interface of the TargetReportExtractor class.
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

#ifndef ASTMOPS_TARGETREPORTEXTRACTOR_H
#define ASTMOPS_TARGETREPORTEXTRACTOR_H

#include "asterix.h"
#include "astmops.h"
#include "counters.h"
#include "targetreport.h"
#include <QQueue>
#include <optional>

class TargetReportExtractor
{
public:
    TargetReportExtractor(const QGeoCoordinate& arp = QGeoCoordinate(),
        const QGeoCoordinate& smr = QGeoCoordinate());

    void addData(const Asterix::Record& rec);
    QQueue<TargetReport> targetReports(SystemType st) const;
    Counters::InOutCounter counters(SystemType st) const;

    void loadExcludedAddresses(QIODevice* device);

    void setArp(const QGeoCoordinate& arp);
    void setSmr(const QGeoCoordinate& smr);

private:
    bool isRecordToBeKept(const Asterix::Record& rec) const;
    std::optional<TargetReport> makeTargetReport(const Asterix::Record& rec) const;

    QGeoCoordinate arp_;
    QGeoCoordinate smr_;

    QSet<ModeS> excluded_addresses_;
    QHash<SystemType, Counters::InOutCounter> counters_;
    QHash<SystemType, QQueue<TargetReport>> tgt_reports_;
};

#endif  // ASTMOPS_TARGETREPORTEXTRACTOR_H
