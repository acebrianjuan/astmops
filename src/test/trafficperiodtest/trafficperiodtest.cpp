/*!
 * \file trafficperiodtest.cpp
 * \brief Implements unit tests for the TrafficPeriod and
 * TrafficPeriodCollection classes.
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

#include "trafficperiod.h"
#include "config.h"
#include <QObject>
#include <QtTest>

using namespace Literals;

class TrafficPeriodTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testTrafficPeriod_data();
    void testTrafficPeriod();

    void testTrafficPeriodCollection_data();
    void testTrafficPeriodCollection();
};

void TrafficPeriodTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-trafficperiodtest"));

    Settings settings;
    settings.clear();

    settings.beginGroup(QLatin1String("Asterix"));
    settings.setValue(QLatin1String("Date"), QLatin1String("2020-05-05"));
    settings.setValue(QLatin1String("SmrSic"), 7);
    settings.setValue(QLatin1String("MlatSic"), 107);
    settings.setValue(QLatin1String("AdsbSic"), 219);
    settings.endGroup();
}

void TrafficPeriodTest::testTrafficPeriod_data()
{
}

void TrafficPeriodTest::testTrafficPeriod()
{
}

void TrafficPeriodTest::testTrafficPeriodCollection_data()
{
    QTest::addColumn<QVector<TrafficPeriod>>("periodsIn");
    QTest::addColumn<QVector<TrafficPeriod>>("periodsOut");


    // CASE 0:
    QVector<TrafficPeriod> tp_in_vec_case_0;
    tp_in_vec_case_0
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_0;
    tp_out_vec_case_0
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001, 0x000002});


    // CASE 1:
    QVector<TrafficPeriod> tp_in_vec_case_1;
    tp_in_vec_case_1
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:05.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_1;
    tp_out_vec_case_1
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:05.000Z"_ts, {0x000001, 0x000002})
        << TrafficPeriod("2020-05-05T10:00:05.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001});


    // CASE 2:
    QVector<TrafficPeriod> tp_in_vec_case_2;
    tp_in_vec_case_2
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:15.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_2;
    tp_out_vec_case_2
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001, 0x000002})
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:15.000Z"_ts, {0x000002});


    // CASE 3:
    QVector<TrafficPeriod> tp_in_vec_case_3;
    tp_in_vec_case_3
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:15.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_3;
    tp_out_vec_case_3
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:15.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:15.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000001, 0x000002});


    // CASE 4:
    QVector<TrafficPeriod> tp_in_vec_case_4;
    tp_in_vec_case_4
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:05.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_4;
    tp_out_vec_case_4
        << TrafficPeriod("2020-05-05T10:00:05.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000002})
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000001, 0x000002});


    // CASE 5:
    QVector<TrafficPeriod> tp_in_vec_case_5;
    tp_in_vec_case_5
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:05.000Z"_ts, "2020-05-05T10:00:15.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_5;
    tp_out_vec_case_5
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:05.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:05.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001, 0x000002})
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:15.000Z"_ts, {0x000002});


    // CASE 6:
    QVector<TrafficPeriod> tp_in_vec_case_6;
    tp_in_vec_case_6
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:04.000Z"_ts, "2020-05-05T10:00:06.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_6;
    tp_out_vec_case_6
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:04.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:04.000Z"_ts, "2020-05-05T10:00:06.000Z"_ts, {0x000001, 0x000002})
        << TrafficPeriod("2020-05-05T10:00:06.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001});


    // CASE 7:
    QVector<TrafficPeriod> tp_in_vec_case_7;
    tp_in_vec_case_7
        << TrafficPeriod("2020-05-05T10:00:05.000Z"_ts, "2020-05-05T10:00:15.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_7;
    tp_out_vec_case_7
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:05.000Z"_ts, {0x000002})
        << TrafficPeriod("2020-05-05T10:00:05.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001, 0x000002})
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:15.000Z"_ts, {0x000001});


    // CASE 8:
    QVector<TrafficPeriod> tp_in_vec_case_8;
    tp_in_vec_case_8
        << TrafficPeriod("2020-05-05T10:00:04.000Z"_ts, "2020-05-05T10:00:06.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000002});

    QVector<TrafficPeriod> tp_out_vec_case_8;
    tp_out_vec_case_8
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:04.000Z"_ts, {0x000002})
        << TrafficPeriod("2020-05-05T10:00:04.000Z"_ts, "2020-05-05T10:00:06.000Z"_ts, {0x000001, 0x000002})
        << TrafficPeriod("2020-05-05T10:00:06.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000002});


    // MIX:
    QVector<TrafficPeriod> tp_in_vec_mix;
    tp_in_vec_mix
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:14.000Z"_ts, "2020-05-05T10:00:18.000Z"_ts, {0x000002})
        << TrafficPeriod("2020-05-05T10:00:08.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000003});

    QVector<TrafficPeriod> tp_out_vec_mix;
    tp_out_vec_mix
        << TrafficPeriod("2020-05-05T10:00:00.000Z"_ts, "2020-05-05T10:00:08.000Z"_ts, {0x000001})
        << TrafficPeriod("2020-05-05T10:00:08.000Z"_ts, "2020-05-05T10:00:10.000Z"_ts, {0x000001, 0x000003})
        << TrafficPeriod("2020-05-05T10:00:10.000Z"_ts, "2020-05-05T10:00:14.000Z"_ts, {0x000003})
        << TrafficPeriod("2020-05-05T10:00:14.000Z"_ts, "2020-05-05T10:00:18.000Z"_ts, {0x000002, 0x000003})
        << TrafficPeriod("2020-05-05T10:00:18.000Z"_ts, "2020-05-05T10:00:20.000Z"_ts, {0x000003});


    QTest::newRow("CASE 0") << tp_in_vec_case_0 << tp_out_vec_case_0;

    QTest::newRow("CASE 1") << tp_in_vec_case_1 << tp_out_vec_case_1;
    QTest::newRow("CASE 2") << tp_in_vec_case_2 << tp_out_vec_case_2;
    QTest::newRow("CASE 3") << tp_in_vec_case_3 << tp_out_vec_case_3;
    QTest::newRow("CASE 4") << tp_in_vec_case_4 << tp_out_vec_case_4;

    QTest::newRow("CASE 5") << tp_in_vec_case_5 << tp_out_vec_case_5;
    QTest::newRow("CASE 6") << tp_in_vec_case_6 << tp_out_vec_case_6;
    QTest::newRow("CASE 7") << tp_in_vec_case_7 << tp_out_vec_case_7;
    QTest::newRow("CASE 8") << tp_in_vec_case_8 << tp_out_vec_case_8;

    QTest::newRow("MIX") << tp_in_vec_mix << tp_out_vec_mix;
}

void TrafficPeriodTest::testTrafficPeriodCollection()
{
    QFETCH(QVector<TrafficPeriod>, periodsIn);
    QFETCH(QVector<TrafficPeriod>, periodsOut);

    TrafficPeriodCollection tpcol;
    for (const TrafficPeriod &tp_in : periodsIn)
    {
        tpcol << tp_in;
    }

    QCOMPARE(tpcol.size(), periodsOut.size());

    int i = 0;
    for (const TrafficPeriod &tp : tpcol)
    {
        QCOMPARE(tp, periodsOut.at(i));
        ++i;
    }
}

QTEST_GUILESS_MAIN(TrafficPeriodTest);
#include "trafficperiodtest.moc"
