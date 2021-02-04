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

#include "asterix.h"
#include <QDateTime>
#include <QGeoPositionInfo>
#include <QMultiMap>
#include <QObject>

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
    void setLocatePointCallback(const std::function<Aerodrome::Area(const QPointF &)> &callback);

private:
    QGeoPositionInfo interpolatedRefPosForTime(const QDateTime &tod, double dtMax, bool *ok = nullptr);

    //QMultiMap<QDateTime, AsterixRecord> dgpsTestDataMap(IcaoAddr dgpsAddr);
    void calculateTestDataMappings();
    TestDataMapping calculateTestDataMapping(const QDateTime &tod) const;
    void addRefPosToMapping(TestDataMapping &mapping) const;

    std::function<Aerodrome::Area(const QPointF &)> m_locatePoint;

    IcaoAddr m_dgpsAddr = Configuration::dgpsTargetAddress();

    QMultiMap<QDateTime, QGeoPositionInfo> m_refData;
    QMultiMap<QDateTime, AsterixRecord> m_testData;
    QMap<QDateTime, TestDataMapping> m_testDataMappings;
};

#endif  // ASTMOPS_EVALUATOR_H
