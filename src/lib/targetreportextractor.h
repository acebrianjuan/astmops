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
#include <QGeoCoordinate>
#include <QObject>
#include <QQueue>
#include <optional>

using LocatePointCb = std::function<Aerodrome::NamedArea(const QVector3D&, const bool)>;

class TargetReportExtractor : public QObject
{
    Q_OBJECT

public:
    TargetReportExtractor(const QGeoCoordinate& arp,
        const QHash<Sic, QVector3D>& smr);

    void addData(const Asterix::Record& rec);
    void loadExcludedAddresses(QIODevice* device);
    void setLocatePointCallback(const LocatePointCb& cb);
    std::optional<TargetReport> takeData();

    QQueue<TargetReport> targetReports(SystemType st) const;
    Counters::InOutCounter counters(SystemType st) const;
    bool hasPendingData() const;

public slots:

signals:
    void readyRead();

private:
    bool isRecordToBeKept(const Asterix::Record& rec) const;
    std::optional<TargetReport> makeTargetReport(const Asterix::Record& rec) const;
    Aerodrome::NamedArea locatePoint(const QVector3D pos, const bool gbs) const;

    LocatePointCb locatePoint_cb_;

    QGeoCoordinate arp_;
    QHash<Sic, QVector3D> smr_;

    QSet<ModeS> excluded_addresses_;
    QHash<SystemType, Counters::InOutCounter> counters_;
    QHash<SystemType, QQueue<TargetReport>> tgt_reports_;
};

#endif  // ASTMOPS_TARGETREPORTEXTRACTOR_H
