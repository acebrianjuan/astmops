/*!
 * \file evaluator.h
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

#ifndef ASTMOPS_EVALUATOR_H
#define ASTMOPS_EVALUATOR_H

#include "areahash.h"
#include "asterix.h"
#include "counters.h"
#include <QDateTime>
#include <QGeoPositionInfo>
#include <QMultiMap>
#include <QObject>
#include <optional>

struct TestDataMapping  // mapping to respective ref data
{
    QDateTime tod;  // tod of test

    bool hasRef1{false};
    QGeoPositionInfo refPosInfo1;

    bool hasRef2{false};
    QGeoPositionInfo refPosInfo2;

    bool hasRefPosInfo{false};
    QGeoPositionInfo refPosInfo;
};

class Evaluator : public QObject
{
    Q_OBJECT

public:
    explicit Evaluator(QObject *parent = nullptr);

    double evalPosAccDgps();

    void setRefData(const QMultiMap<QDateTime, QGeoPositionInfo> &refData);
    void setTestData(const QMultiMap<QDateTime, AsterixRecord> &testData);
    void setLocatePointCallback(const std::function<Aerodrome::NamedArea(const QVector3D &, const bool)> &callback);

private:
    QGeoPositionInfo interpolatedRefPosForTime(const QDateTime &tod, double dtMax, bool *ok = nullptr);

    //QMultiMap<QDateTime, AsterixRecord> dgpsTestDataMap(IcaoAddr dgpsAddr);
    void calculateTestDataMappings();
    TestDataMapping calculateTestDataMapping(const QDateTime &tod) const;
    void addRefPosToMapping(TestDataMapping &mapping) const;
    bool hasRefDataForTime(const QDateTime &tod, double tdiffMax) const;
    double percentile(QVector<double> vec, const double percent);
    double mean(const QVector<double> &v);
    double stdDev(const QVector<double> &v);

    void printPosAccResuls();

    std::function<Aerodrome::NamedArea(const QVector3D &, const bool)> m_locatePoint;

    IcaoAddr m_dgpsAddr = Configuration::dgpsTargetAddress();

    QMultiMap<QDateTime, QGeoPositionInfo> m_refData;
    QMultiMap<QDateTime, AsterixRecord> m_testData;
    QMap<QDateTime, TestDataMapping> m_testDataMappings;

    AreaHash<Counters::BasicCounter> m_cat010MlatPosAccuracyCounters;
    AreaHash<QVector<double>> m_cat010MlatPosAccuracyErrors;
};

#endif  // ASTMOPS_EVALUATOR_H
