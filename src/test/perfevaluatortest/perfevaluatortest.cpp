/*!
 * \file perfevaluatortest.cpp
 * \brief Implements unit tests for the PerfEvaluator class.
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

#include "perfevaluator.h"
#include "config.h"
#include <QObject>
#include <QtTest>

using namespace Literals;

class PerfEvaluatorTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testED116RPA_data();
    void testED116RPA();

    void testED116UR_data();
    void testED116UR();

    void testED116PD_data();
    void testED116PD();

    void testED116PFD_data();
    void testED116PFD();

    void testED117RPA_data();
    void testED117RPA();

    void testED117UR_data();
    void testED117UR();

    void testED117PD_data();
    void testED117PD();

    void testED117PFD_data();
    void testED117PFD();

    void testED117PID_data();
    void testED117PID();

    void testED117PFID_data();
    void testED117PFID();

    void testED117PLG_data();
    void testED117PLG();
};

void PerfEvaluatorTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-perfevaluatortest"));

    Settings settings;
    settings.clear();

    settings.setValue(QLatin1String("Date"), QLatin1String("2020-05-05"));
    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 219);
}

void PerfEvaluatorTest::testED116RPA_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<AreaHash<QVector<double>>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_1.ver_ = 2;
    tr_adsb_101_1.pic_ = 14;

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_2.ver_ = 2;
    tr_adsb_101_2.pic_ = 14;

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_3.ver_ = 2;
    tr_adsb_101_3.pic_ = 14;

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_4.ver_ = 2;
    tr_adsb_101_4.pic_ = 14;

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_5.ver_ = 2;
    tr_adsb_101_5.pic_ = 14;

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_6.ver_ = 2;
    tr_adsb_101_6.pic_ = 14;

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);
    tr_adsb_101_7.ver_ = 2;
    tr_adsb_101_7.pic_ = 14;

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);
    tr_adsb_101_8.ver_ = 2;
    tr_adsb_101_8.pic_ = 14;

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);
    tr_adsb_101_9.ver_ = 2;
    tr_adsb_101_9.pic_ = 14;

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // SMR track 301.
    Track trk_smr_301(SystemType::Smr, 301);

    TargetReport tr_smr_301_1;
    tr_smr_301_1.ds_id_.sac_ = 0;
    tr_smr_301_1.ds_id_.sic_ = 7;
    tr_smr_301_1.sys_typ_ = SystemType::Smr;
    tr_smr_301_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_smr_301_1.trk_nb_ = 301;
    tr_smr_301_1.on_gnd_ = true;
    tr_smr_301_1.x_ = 0.0;
    tr_smr_301_1.y_ = 0.0;
    tr_smr_301_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_2;
    tr_smr_301_2.ds_id_.sac_ = 0;
    tr_smr_301_2.ds_id_.sic_ = 7;
    tr_smr_301_2.sys_typ_ = SystemType::Smr;
    tr_smr_301_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_smr_301_2.trk_nb_ = 301;
    tr_smr_301_2.on_gnd_ = true;
    tr_smr_301_2.x_ = 50.0;
    tr_smr_301_2.y_ = 5.0;
    tr_smr_301_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_3;
    tr_smr_301_3.ds_id_.sac_ = 0;
    tr_smr_301_3.ds_id_.sic_ = 7;
    tr_smr_301_3.sys_typ_ = SystemType::Smr;
    tr_smr_301_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_smr_301_3.trk_nb_ = 301;
    tr_smr_301_3.on_gnd_ = true;
    tr_smr_301_3.x_ = 100.0;
    tr_smr_301_3.y_ = 10.0;
    tr_smr_301_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_4;
    tr_smr_301_4.ds_id_.sac_ = 0;
    tr_smr_301_4.ds_id_.sic_ = 7;
    tr_smr_301_4.sys_typ_ = SystemType::Smr;
    tr_smr_301_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_smr_301_4.trk_nb_ = 301;
    tr_smr_301_4.on_gnd_ = true;
    tr_smr_301_4.x_ = 150.0;
    tr_smr_301_4.y_ = 15.0;
    tr_smr_301_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_5;
    tr_smr_301_5.ds_id_.sac_ = 0;
    tr_smr_301_5.ds_id_.sic_ = 7;
    tr_smr_301_5.sys_typ_ = SystemType::Smr;
    tr_smr_301_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_smr_301_5.trk_nb_ = 301;
    tr_smr_301_5.on_gnd_ = true;
    tr_smr_301_5.x_ = 200.0;
    tr_smr_301_5.y_ = 20.0;
    tr_smr_301_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_6;
    tr_smr_301_6.ds_id_.sac_ = 0;
    tr_smr_301_6.ds_id_.sic_ = 7;
    tr_smr_301_6.sys_typ_ = SystemType::Smr;
    tr_smr_301_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_smr_301_6.trk_nb_ = 301;
    tr_smr_301_6.on_gnd_ = true;
    tr_smr_301_6.x_ = 250.0;
    tr_smr_301_6.y_ = 25.0;
    tr_smr_301_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_7;
    tr_smr_301_7.ds_id_.sac_ = 0;
    tr_smr_301_7.ds_id_.sic_ = 7;
    tr_smr_301_7.sys_typ_ = SystemType::Smr;
    tr_smr_301_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_smr_301_7.trk_nb_ = 301;
    tr_smr_301_7.on_gnd_ = true;
    tr_smr_301_7.x_ = 300.0;
    tr_smr_301_7.y_ = 30.0;
    tr_smr_301_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_8;
    tr_smr_301_8.ds_id_.sac_ = 0;
    tr_smr_301_8.ds_id_.sic_ = 7;
    tr_smr_301_8.sys_typ_ = SystemType::Smr;
    tr_smr_301_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_smr_301_8.trk_nb_ = 301;
    tr_smr_301_8.on_gnd_ = true;
    tr_smr_301_8.x_ = 350.0;
    tr_smr_301_8.y_ = 30.0;
    tr_smr_301_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_9;
    tr_smr_301_9.ds_id_.sac_ = 0;
    tr_smr_301_9.ds_id_.sic_ = 7;
    tr_smr_301_9.sys_typ_ = SystemType::Smr;
    tr_smr_301_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_smr_301_9.trk_nb_ = 301;
    tr_smr_301_9.on_gnd_ = true;
    tr_smr_301_9.x_ = 400.0;
    tr_smr_301_9.y_ = 30.0;
    tr_smr_301_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_smr_301 << tr_smr_301_1 << tr_smr_301_2 << tr_smr_301_3
                << tr_smr_301_4 << tr_smr_301_5 << tr_smr_301_6
                << tr_smr_301_7 << tr_smr_301_8 << tr_smr_301_9;

    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_smr_301;

    AreaHash<QVector<double>> rpaErr_1;

    AreaHash<QVector<double>> rpaErr_2;
    rpaErr_2[Aerodrome::NamedArea(Aerodrome::Runway)]
        << 0 << 5 << 10 << 15 << 20 << 25;
    rpaErr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)]
        << 30 << 30 << 30;

    QTest::newRow("RPA NO DATA") << tracksIn_1 << rpaErr_1;
    QTest::newRow("RPA") << tracksIn_2 << rpaErr_2;
}

void PerfEvaluatorTest::testED116RPA()
{
    using RpaHash = AreaHash<QVector<double>>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(RpaHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.smrRpaErrors_, countersOut);
}

void PerfEvaluatorTest::testED116UR_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::UrCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // SMR track 301.
    Track trk_smr_301(SystemType::Smr, 301);

    TargetReport tr_smr_301_1;
    tr_smr_301_1.ds_id_.sac_ = 0;
    tr_smr_301_1.ds_id_.sic_ = 7;
    tr_smr_301_1.sys_typ_ = SystemType::Smr;
    tr_smr_301_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_smr_301_1.trk_nb_ = 301;
    tr_smr_301_1.on_gnd_ = true;
    tr_smr_301_1.x_ = 0.0;
    tr_smr_301_1.y_ = 0.0;
    tr_smr_301_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_2;
    tr_smr_301_2.ds_id_.sac_ = 0;
    tr_smr_301_2.ds_id_.sic_ = 7;
    tr_smr_301_2.sys_typ_ = SystemType::Smr;
    tr_smr_301_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_smr_301_2.trk_nb_ = 301;
    tr_smr_301_2.on_gnd_ = true;
    tr_smr_301_2.x_ = 50.0;
    tr_smr_301_2.y_ = 0.0;
    tr_smr_301_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_3;
    tr_smr_301_3.ds_id_.sac_ = 0;
    tr_smr_301_3.ds_id_.sic_ = 7;
    tr_smr_301_3.sys_typ_ = SystemType::Smr;
    tr_smr_301_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_smr_301_3.trk_nb_ = 301;
    tr_smr_301_3.on_gnd_ = true;
    tr_smr_301_3.x_ = 100.0;
    tr_smr_301_3.y_ = 0.0;
    tr_smr_301_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_4;
    tr_smr_301_4.ds_id_.sac_ = 0;
    tr_smr_301_4.ds_id_.sic_ = 7;
    tr_smr_301_4.sys_typ_ = SystemType::Smr;
    tr_smr_301_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_smr_301_4.trk_nb_ = 301;
    tr_smr_301_4.on_gnd_ = true;
    tr_smr_301_4.x_ = 150.0;
    tr_smr_301_4.y_ = 0.0;
    tr_smr_301_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_5;
    tr_smr_301_5.ds_id_.sac_ = 0;
    tr_smr_301_5.ds_id_.sic_ = 7;
    tr_smr_301_5.sys_typ_ = SystemType::Smr;
    tr_smr_301_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_smr_301_5.trk_nb_ = 301;
    tr_smr_301_5.on_gnd_ = true;
    tr_smr_301_5.x_ = 200.0;
    tr_smr_301_5.y_ = 0.0;
    tr_smr_301_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_6;
    tr_smr_301_6.ds_id_.sac_ = 0;
    tr_smr_301_6.ds_id_.sic_ = 7;
    tr_smr_301_6.sys_typ_ = SystemType::Smr;
    tr_smr_301_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_smr_301_6.trk_nb_ = 301;
    tr_smr_301_6.on_gnd_ = true;
    tr_smr_301_6.x_ = 250.0;
    tr_smr_301_6.y_ = 0.0;
    tr_smr_301_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_7;
    tr_smr_301_7.ds_id_.sac_ = 0;
    tr_smr_301_7.ds_id_.sic_ = 7;
    tr_smr_301_7.sys_typ_ = SystemType::Smr;
    tr_smr_301_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_smr_301_7.trk_nb_ = 301;
    tr_smr_301_7.on_gnd_ = true;
    tr_smr_301_7.x_ = 300.0;
    tr_smr_301_7.y_ = 0.0;
    tr_smr_301_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_8;
    tr_smr_301_8.ds_id_.sac_ = 0;
    tr_smr_301_8.ds_id_.sic_ = 7;
    tr_smr_301_8.sys_typ_ = SystemType::Smr;
    tr_smr_301_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_smr_301_8.trk_nb_ = 301;
    tr_smr_301_8.on_gnd_ = true;
    tr_smr_301_8.x_ = 350.0;
    tr_smr_301_8.y_ = 0.0;
    tr_smr_301_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_9;
    tr_smr_301_9.ds_id_.sac_ = 0;
    tr_smr_301_9.ds_id_.sic_ = 7;
    tr_smr_301_9.sys_typ_ = SystemType::Smr;
    tr_smr_301_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_smr_301_9.trk_nb_ = 301;
    tr_smr_301_9.on_gnd_ = true;
    tr_smr_301_9.x_ = 400.0;
    tr_smr_301_9.y_ = 0.0;
    tr_smr_301_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_smr_301 << tr_smr_301_1 << tr_smr_301_2 << tr_smr_301_3
                << tr_smr_301_4 << tr_smr_301_5 << tr_smr_301_6
                << tr_smr_301_7 << tr_smr_301_8 << tr_smr_301_9;

    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_smr_301;

    QHash<Aerodrome::NamedArea, Counters::UrCounter> urCtr_1;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_etrp_ = 5;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 0;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etrp_ = 2;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 0;

    QHash<Aerodrome::NamedArea, Counters::UrCounter> urCtr_2;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_etrp_ = 5;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 5;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etrp_ = 2;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 2;

    QTest::newRow("UR 0 %") << tracksIn_1 << urCtr_1;
    QTest::newRow("UR 100 %") << tracksIn_2 << urCtr_2;
}

void PerfEvaluatorTest::testED116UR()
{
    using UrHash = QHash<Aerodrome::NamedArea, Counters::UrCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(UrHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.smrUr_, countersOut);
}

void PerfEvaluatorTest::testED116PD_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::PdCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // SMR track 301.
    Track trk_smr_301(SystemType::Smr, 301);

    TargetReport tr_smr_301_1;
    tr_smr_301_1.ds_id_.sac_ = 0;
    tr_smr_301_1.ds_id_.sic_ = 7;
    tr_smr_301_1.sys_typ_ = SystemType::Smr;
    tr_smr_301_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_smr_301_1.trk_nb_ = 301;
    tr_smr_301_1.on_gnd_ = true;
    tr_smr_301_1.x_ = 0.0;
    tr_smr_301_1.y_ = 0.0;
    tr_smr_301_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_2;
    tr_smr_301_2.ds_id_.sac_ = 0;
    tr_smr_301_2.ds_id_.sic_ = 7;
    tr_smr_301_2.sys_typ_ = SystemType::Smr;
    tr_smr_301_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_smr_301_2.trk_nb_ = 301;
    tr_smr_301_2.on_gnd_ = true;
    tr_smr_301_2.x_ = 50.0;
    tr_smr_301_2.y_ = 0.0;
    tr_smr_301_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_3;
    tr_smr_301_3.ds_id_.sac_ = 0;
    tr_smr_301_3.ds_id_.sic_ = 7;
    tr_smr_301_3.sys_typ_ = SystemType::Smr;
    tr_smr_301_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_smr_301_3.trk_nb_ = 301;
    tr_smr_301_3.on_gnd_ = true;
    tr_smr_301_3.x_ = 100.0;
    tr_smr_301_3.y_ = 0.0;
    tr_smr_301_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_4;
    tr_smr_301_4.ds_id_.sac_ = 0;
    tr_smr_301_4.ds_id_.sic_ = 7;
    tr_smr_301_4.sys_typ_ = SystemType::Smr;
    tr_smr_301_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_smr_301_4.trk_nb_ = 301;
    tr_smr_301_4.on_gnd_ = true;
    tr_smr_301_4.x_ = 150.0;
    tr_smr_301_4.y_ = 0.0;
    tr_smr_301_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_5;
    tr_smr_301_5.ds_id_.sac_ = 0;
    tr_smr_301_5.ds_id_.sic_ = 7;
    tr_smr_301_5.sys_typ_ = SystemType::Smr;
    tr_smr_301_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_smr_301_5.trk_nb_ = 301;
    tr_smr_301_5.on_gnd_ = true;
    tr_smr_301_5.x_ = 200.0;
    tr_smr_301_5.y_ = 0.0;
    tr_smr_301_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_6;
    tr_smr_301_6.ds_id_.sac_ = 0;
    tr_smr_301_6.ds_id_.sic_ = 7;
    tr_smr_301_6.sys_typ_ = SystemType::Smr;
    tr_smr_301_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_smr_301_6.trk_nb_ = 301;
    tr_smr_301_6.on_gnd_ = true;
    tr_smr_301_6.x_ = 250.0;
    tr_smr_301_6.y_ = 0.0;
    tr_smr_301_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_7;
    tr_smr_301_7.ds_id_.sac_ = 0;
    tr_smr_301_7.ds_id_.sic_ = 7;
    tr_smr_301_7.sys_typ_ = SystemType::Smr;
    tr_smr_301_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_smr_301_7.trk_nb_ = 301;
    tr_smr_301_7.on_gnd_ = true;
    tr_smr_301_7.x_ = 300.0;
    tr_smr_301_7.y_ = 0.0;
    tr_smr_301_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_8;
    tr_smr_301_8.ds_id_.sac_ = 0;
    tr_smr_301_8.ds_id_.sic_ = 7;
    tr_smr_301_8.sys_typ_ = SystemType::Smr;
    tr_smr_301_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_smr_301_8.trk_nb_ = 301;
    tr_smr_301_8.on_gnd_ = true;
    tr_smr_301_8.x_ = 350.0;
    tr_smr_301_8.y_ = 0.0;
    tr_smr_301_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_9;
    tr_smr_301_9.ds_id_.sac_ = 0;
    tr_smr_301_9.ds_id_.sic_ = 7;
    tr_smr_301_9.sys_typ_ = SystemType::Smr;
    tr_smr_301_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_smr_301_9.trk_nb_ = 301;
    tr_smr_301_9.on_gnd_ = true;
    tr_smr_301_9.x_ = 400.0;
    tr_smr_301_9.y_ = 0.0;
    tr_smr_301_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_smr_301 << tr_smr_301_1 << tr_smr_301_2 << tr_smr_301_3
                << tr_smr_301_4 << tr_smr_301_5 << tr_smr_301_6
                << tr_smr_301_7 << tr_smr_301_8 << tr_smr_301_9;

    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_smr_301;

    QHash<Aerodrome::NamedArea, Counters::PdCounter> pdCtr_1;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_up_ = 5;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 0;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_up_ = 2;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 0;

    QHash<Aerodrome::NamedArea, Counters::PdCounter> pdCtr_2;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_up_ = 5;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 5;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_up_ = 2;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 2;


    QTest::newRow("PD 0 %") << tracksIn_1 << pdCtr_1;
    QTest::newRow("PD 100 %") << tracksIn_2 << pdCtr_2;
}

void PerfEvaluatorTest::testED116PD()
{
    using PdHash = QHash<Aerodrome::NamedArea, Counters::PdCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(PdHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.smrPd_, countersOut);
}

void PerfEvaluatorTest::testED116PFD_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::PfdCounter2>>("countersOut");


    // ADS-B track 101 (Target 1).
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_10;
    tr_adsb_101_10.ds_id_.sac_ = 0;
    tr_adsb_101_10.ds_id_.sic_ = 219;
    tr_adsb_101_10.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_10.tod_ = "2020-05-05T10:00:09.000Z"_ts;
    tr_adsb_101_10.trk_nb_ = 101;
    tr_adsb_101_10.mode_s_ = 0x000001;
    tr_adsb_101_10.mode_3a_ = 0001;
    tr_adsb_101_10.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_10.on_gnd_ = true;
    tr_adsb_101_10.x_ = 450.0;
    tr_adsb_101_10.y_ = 0.0;
    tr_adsb_101_10.z_ = 0.0;
    tr_adsb_101_10.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_11;
    tr_adsb_101_11.ds_id_.sac_ = 0;
    tr_adsb_101_11.ds_id_.sic_ = 219;
    tr_adsb_101_11.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_11.tod_ = "2020-05-05T10:00:10.000Z"_ts;
    tr_adsb_101_11.trk_nb_ = 101;
    tr_adsb_101_11.mode_s_ = 0x000001;
    tr_adsb_101_11.mode_3a_ = 0001;
    tr_adsb_101_11.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_11.on_gnd_ = true;
    tr_adsb_101_11.x_ = 500.0;
    tr_adsb_101_11.y_ = 0.0;
    tr_adsb_101_11.z_ = 0.0;
    tr_adsb_101_11.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_12;
    tr_adsb_101_12.ds_id_.sac_ = 0;
    tr_adsb_101_12.ds_id_.sic_ = 219;
    tr_adsb_101_12.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_12.tod_ = "2020-05-05T10:00:11.000Z"_ts;
    tr_adsb_101_12.trk_nb_ = 101;
    tr_adsb_101_12.mode_s_ = 0x000001;
    tr_adsb_101_12.mode_3a_ = 0001;
    tr_adsb_101_12.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_12.on_gnd_ = true;
    tr_adsb_101_12.x_ = 550.0;
    tr_adsb_101_12.y_ = 0.0;
    tr_adsb_101_12.z_ = 0.0;
    tr_adsb_101_12.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9
                 << tr_adsb_101_10 << tr_adsb_101_11 << tr_adsb_101_12;


    // ADS-B track 102 (Target 2).
    Track trk_adsb_102(SystemType::Adsb, 102);

    TargetReport tr_adsb_102_1;
    tr_adsb_102_1.ds_id_.sac_ = 0;
    tr_adsb_102_1.ds_id_.sic_ = 219;
    tr_adsb_102_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_1.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_102_1.trk_nb_ = 102;
    tr_adsb_102_1.mode_s_ = 0x000002;
    tr_adsb_102_1.mode_3a_ = 0002;
    tr_adsb_102_1.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_1.on_gnd_ = true;
    tr_adsb_102_1.x_ = 0.0;
    tr_adsb_102_1.y_ = 200.0;
    tr_adsb_102_1.z_ = 0.0;
    tr_adsb_102_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_2;
    tr_adsb_102_2.ds_id_.sac_ = 0;
    tr_adsb_102_2.ds_id_.sic_ = 219;
    tr_adsb_102_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_2.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_102_2.trk_nb_ = 102;
    tr_adsb_102_2.mode_s_ = 0x000002;
    tr_adsb_102_2.mode_3a_ = 0002;
    tr_adsb_102_2.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_2.on_gnd_ = true;
    tr_adsb_102_2.x_ = 50.0;
    tr_adsb_102_2.y_ = 200.0;
    tr_adsb_102_2.z_ = 0.0;
    tr_adsb_102_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_3;
    tr_adsb_102_3.ds_id_.sac_ = 0;
    tr_adsb_102_3.ds_id_.sic_ = 219;
    tr_adsb_102_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_3.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_102_3.trk_nb_ = 102;
    tr_adsb_102_3.mode_s_ = 0x000002;
    tr_adsb_102_3.mode_3a_ = 0002;
    tr_adsb_102_3.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_3.on_gnd_ = true;
    tr_adsb_102_3.x_ = 100.0;
    tr_adsb_102_3.y_ = 200.0;
    tr_adsb_102_3.z_ = 0.0;
    tr_adsb_102_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_4;
    tr_adsb_102_4.ds_id_.sac_ = 0;
    tr_adsb_102_4.ds_id_.sic_ = 219;
    tr_adsb_102_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_4.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_102_4.trk_nb_ = 102;
    tr_adsb_102_4.mode_s_ = 0x000002;
    tr_adsb_102_4.mode_3a_ = 0002;
    tr_adsb_102_4.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_4.on_gnd_ = true;
    tr_adsb_102_4.x_ = 150.0;
    tr_adsb_102_4.y_ = 200.0;
    tr_adsb_102_4.z_ = 0.0;
    tr_adsb_102_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_5;
    tr_adsb_102_5.ds_id_.sac_ = 0;
    tr_adsb_102_5.ds_id_.sic_ = 219;
    tr_adsb_102_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_5.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_102_5.trk_nb_ = 102;
    tr_adsb_102_5.mode_s_ = 0x000002;
    tr_adsb_102_5.mode_3a_ = 0002;
    tr_adsb_102_5.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_5.on_gnd_ = true;
    tr_adsb_102_5.x_ = 200.0;
    tr_adsb_102_5.y_ = 200.0;
    tr_adsb_102_5.z_ = 0.0;
    tr_adsb_102_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_6;
    tr_adsb_102_6.ds_id_.sac_ = 0;
    tr_adsb_102_6.ds_id_.sic_ = 219;
    tr_adsb_102_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_6.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_102_6.trk_nb_ = 102;
    tr_adsb_102_6.mode_s_ = 0x000002;
    tr_adsb_102_6.mode_3a_ = 0002;
    tr_adsb_102_6.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_6.on_gnd_ = true;
    tr_adsb_102_6.x_ = 250.0;
    tr_adsb_102_6.y_ = 200.0;
    tr_adsb_102_6.z_ = 0.0;
    tr_adsb_102_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_7;
    tr_adsb_102_7.ds_id_.sac_ = 0;
    tr_adsb_102_7.ds_id_.sic_ = 219;
    tr_adsb_102_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_7.tod_ = "2020-05-05T10:00:09.000Z"_ts;
    tr_adsb_102_7.trk_nb_ = 102;
    tr_adsb_102_7.mode_s_ = 0x000002;
    tr_adsb_102_7.mode_3a_ = 0002;
    tr_adsb_102_7.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_7.on_gnd_ = true;
    tr_adsb_102_7.x_ = 300.0;
    tr_adsb_102_7.y_ = 200.0;
    tr_adsb_102_7.z_ = 0.0;
    tr_adsb_102_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_102_8;
    tr_adsb_102_8.ds_id_.sac_ = 0;
    tr_adsb_102_8.ds_id_.sic_ = 219;
    tr_adsb_102_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_8.tod_ = "2020-05-05T10:00:10.000Z"_ts;
    tr_adsb_102_8.trk_nb_ = 102;
    tr_adsb_102_8.mode_s_ = 0x000002;
    tr_adsb_102_8.mode_3a_ = 0002;
    tr_adsb_102_8.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_8.on_gnd_ = true;
    tr_adsb_102_8.x_ = 350.0;
    tr_adsb_102_8.y_ = 200.0;
    tr_adsb_102_8.z_ = 0.0;
    tr_adsb_102_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_102_9;
    tr_adsb_102_9.ds_id_.sac_ = 0;
    tr_adsb_102_9.ds_id_.sic_ = 219;
    tr_adsb_102_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_9.tod_ = "2020-05-05T10:00:11.000Z"_ts;
    tr_adsb_102_9.trk_nb_ = 102;
    tr_adsb_102_9.mode_s_ = 0x000002;
    tr_adsb_102_9.mode_3a_ = 0002;
    tr_adsb_102_9.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_9.on_gnd_ = true;
    tr_adsb_102_9.x_ = 400.0;
    tr_adsb_102_9.y_ = 200.0;
    tr_adsb_102_9.z_ = 0.0;
    tr_adsb_102_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_102_10;
    tr_adsb_102_10.ds_id_.sac_ = 0;
    tr_adsb_102_10.ds_id_.sic_ = 219;
    tr_adsb_102_10.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_10.tod_ = "2020-05-05T10:00:12.000Z"_ts;
    tr_adsb_102_10.trk_nb_ = 102;
    tr_adsb_102_10.mode_s_ = 0x000002;
    tr_adsb_102_10.mode_3a_ = 0002;
    tr_adsb_102_10.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_10.on_gnd_ = true;
    tr_adsb_102_10.x_ = 450.0;
    tr_adsb_102_10.y_ = 200.0;
    tr_adsb_102_10.z_ = 0.0;
    tr_adsb_102_10.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_102_11;
    tr_adsb_102_11.ds_id_.sac_ = 0;
    tr_adsb_102_11.ds_id_.sic_ = 219;
    tr_adsb_102_11.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_11.tod_ = "2020-05-05T10:00:13.000Z"_ts;
    tr_adsb_102_11.trk_nb_ = 102;
    tr_adsb_102_11.mode_s_ = 0x000002;
    tr_adsb_102_11.mode_3a_ = 0002;
    tr_adsb_102_11.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_11.on_gnd_ = true;
    tr_adsb_102_11.x_ = 500.0;
    tr_adsb_102_11.y_ = 200.0;
    tr_adsb_102_11.z_ = 0.0;
    tr_adsb_102_11.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_102_12;
    tr_adsb_102_12.ds_id_.sac_ = 0;
    tr_adsb_102_12.ds_id_.sic_ = 219;
    tr_adsb_102_12.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_12.tod_ = "2020-05-05T10:00:14.000Z"_ts;
    tr_adsb_102_12.trk_nb_ = 102;
    tr_adsb_102_12.mode_s_ = 0x000002;
    tr_adsb_102_12.mode_3a_ = 0002;
    tr_adsb_102_12.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_12.on_gnd_ = true;
    tr_adsb_102_12.x_ = 550.0;
    tr_adsb_102_12.y_ = 200.0;
    tr_adsb_102_12.z_ = 0.0;
    tr_adsb_102_12.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_102 << tr_adsb_102_1 << tr_adsb_102_2 << tr_adsb_102_3
                 << tr_adsb_102_4 << tr_adsb_102_5 << tr_adsb_102_6
                 << tr_adsb_102_7 << tr_adsb_102_8 << tr_adsb_102_9
                 << tr_adsb_102_10 << tr_adsb_102_11 << tr_adsb_102_12;


    // SMR track 301 (Target 1).
    Track trk_smr_301(SystemType::Smr, 301);

    TargetReport tr_smr_301_1;
    tr_smr_301_1.ds_id_.sac_ = 0;
    tr_smr_301_1.ds_id_.sic_ = 7;
    tr_smr_301_1.sys_typ_ = SystemType::Smr;
    tr_smr_301_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_smr_301_1.trk_nb_ = 301;
    tr_smr_301_1.on_gnd_ = true;
    tr_smr_301_1.x_ = 0.0;
    tr_smr_301_1.y_ = 0.0;
    tr_smr_301_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_2;
    tr_smr_301_2.ds_id_.sac_ = 0;
    tr_smr_301_2.ds_id_.sic_ = 7;
    tr_smr_301_2.sys_typ_ = SystemType::Smr;
    tr_smr_301_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_smr_301_2.trk_nb_ = 301;
    tr_smr_301_2.on_gnd_ = true;
    tr_smr_301_2.x_ = 50.0;
    tr_smr_301_2.y_ = 0.0;
    tr_smr_301_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_3;
    tr_smr_301_3.ds_id_.sac_ = 0;
    tr_smr_301_3.ds_id_.sic_ = 7;
    tr_smr_301_3.sys_typ_ = SystemType::Smr;
    tr_smr_301_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_smr_301_3.trk_nb_ = 301;
    tr_smr_301_3.on_gnd_ = true;
    tr_smr_301_3.x_ = 100.0;
    tr_smr_301_3.y_ = 0.0;
    tr_smr_301_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_4;
    tr_smr_301_4.ds_id_.sac_ = 0;
    tr_smr_301_4.ds_id_.sic_ = 7;
    tr_smr_301_4.sys_typ_ = SystemType::Smr;
    tr_smr_301_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_smr_301_4.trk_nb_ = 301;
    tr_smr_301_4.on_gnd_ = true;
    tr_smr_301_4.x_ = 150.0;
    tr_smr_301_4.y_ = 0.0;
    tr_smr_301_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_5;
    tr_smr_301_5.ds_id_.sac_ = 0;
    tr_smr_301_5.ds_id_.sic_ = 7;
    tr_smr_301_5.sys_typ_ = SystemType::Smr;
    tr_smr_301_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_smr_301_5.trk_nb_ = 301;
    tr_smr_301_5.on_gnd_ = true;
    tr_smr_301_5.x_ = 200.0;
    tr_smr_301_5.y_ = 0.0;
    tr_smr_301_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_6;
    tr_smr_301_6.ds_id_.sac_ = 0;
    tr_smr_301_6.ds_id_.sic_ = 7;
    tr_smr_301_6.sys_typ_ = SystemType::Smr;
    tr_smr_301_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_smr_301_6.trk_nb_ = 301;
    tr_smr_301_6.on_gnd_ = true;
    tr_smr_301_6.x_ = 250.0;
    tr_smr_301_6.y_ = 0.0;
    tr_smr_301_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_7;
    tr_smr_301_7.ds_id_.sac_ = 0;
    tr_smr_301_7.ds_id_.sic_ = 7;
    tr_smr_301_7.sys_typ_ = SystemType::Smr;
    tr_smr_301_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_smr_301_7.trk_nb_ = 301;
    tr_smr_301_7.on_gnd_ = true;
    tr_smr_301_7.x_ = 300.0;
    tr_smr_301_7.y_ = 0.0;
    tr_smr_301_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_8;
    tr_smr_301_8.ds_id_.sac_ = 0;
    tr_smr_301_8.ds_id_.sic_ = 7;
    tr_smr_301_8.sys_typ_ = SystemType::Smr;
    tr_smr_301_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_smr_301_8.trk_nb_ = 301;
    tr_smr_301_8.on_gnd_ = true;
    tr_smr_301_8.x_ = 350.0;
    tr_smr_301_8.y_ = 0.0;
    tr_smr_301_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_9;
    tr_smr_301_9.ds_id_.sac_ = 0;
    tr_smr_301_9.ds_id_.sic_ = 7;
    tr_smr_301_9.sys_typ_ = SystemType::Smr;
    tr_smr_301_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_smr_301_9.trk_nb_ = 301;
    tr_smr_301_9.on_gnd_ = true;
    tr_smr_301_9.x_ = 400.0;
    tr_smr_301_9.y_ = 0.0;
    tr_smr_301_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_10;
    tr_smr_301_10.ds_id_.sac_ = 0;
    tr_smr_301_10.ds_id_.sic_ = 7;
    tr_smr_301_10.sys_typ_ = SystemType::Smr;
    tr_smr_301_10.tod_ = "2020-05-05T10:00:09.100Z"_ts;
    tr_smr_301_10.trk_nb_ = 301;
    tr_smr_301_10.on_gnd_ = true;
    tr_smr_301_10.x_ = 450.0;
    tr_smr_301_10.y_ = 0.0;
    tr_smr_301_10.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_11;
    tr_smr_301_11.ds_id_.sac_ = 0;
    tr_smr_301_11.ds_id_.sic_ = 7;
    tr_smr_301_11.sys_typ_ = SystemType::Smr;
    tr_smr_301_11.tod_ = "2020-05-05T10:00:10.100Z"_ts;
    tr_smr_301_11.trk_nb_ = 301;
    tr_smr_301_11.on_gnd_ = true;
    tr_smr_301_11.x_ = 500.0;
    tr_smr_301_11.y_ = 0.0;
    tr_smr_301_11.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_301_12;
    tr_smr_301_12.ds_id_.sac_ = 0;
    tr_smr_301_12.ds_id_.sic_ = 7;
    tr_smr_301_12.sys_typ_ = SystemType::Smr;
    tr_smr_301_12.tod_ = "2020-05-05T10:00:11.100Z"_ts;
    tr_smr_301_12.trk_nb_ = 301;
    tr_smr_301_12.on_gnd_ = true;
    tr_smr_301_12.x_ = 550.0;
    tr_smr_301_12.y_ = 0.0;
    tr_smr_301_12.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_smr_301 << tr_smr_301_1 << tr_smr_301_2 << tr_smr_301_3
                << tr_smr_301_4 << tr_smr_301_5 << tr_smr_301_6
                << tr_smr_301_7 << tr_smr_301_8 << tr_smr_301_9
                << tr_smr_301_10 << tr_smr_301_11 << tr_smr_301_12;


    // SMR track 302 (Target 2).
    Track trk_smr_302(SystemType::Smr, 302);

    TargetReport tr_smr_302_1;
    tr_smr_302_1.ds_id_.sac_ = 0;
    tr_smr_302_1.ds_id_.sic_ = 7;
    tr_smr_302_1.sys_typ_ = SystemType::Smr;
    tr_smr_302_1.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_smr_302_1.trk_nb_ = 302;
    tr_smr_302_1.on_gnd_ = true;
    tr_smr_302_1.x_ = 0.0;
    tr_smr_302_1.y_ = 200.0;
    tr_smr_302_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_2;
    tr_smr_302_2.ds_id_.sac_ = 0;
    tr_smr_302_2.ds_id_.sic_ = 7;
    tr_smr_302_2.sys_typ_ = SystemType::Smr;
    tr_smr_302_2.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_smr_302_2.trk_nb_ = 302;
    tr_smr_302_2.on_gnd_ = true;
    tr_smr_302_2.x_ = 50.0;
    tr_smr_302_2.y_ = 200.0;
    tr_smr_302_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_3;
    tr_smr_302_3.ds_id_.sac_ = 0;
    tr_smr_302_3.ds_id_.sic_ = 7;
    tr_smr_302_3.sys_typ_ = SystemType::Smr;
    tr_smr_302_3.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_smr_302_3.trk_nb_ = 302;
    tr_smr_302_3.on_gnd_ = true;
    tr_smr_302_3.x_ = 100.0;
    tr_smr_302_3.y_ = 200.0;
    tr_smr_302_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_4;
    tr_smr_302_4.ds_id_.sac_ = 0;
    tr_smr_302_4.ds_id_.sic_ = 7;
    tr_smr_302_4.sys_typ_ = SystemType::Smr;
    tr_smr_302_4.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_smr_302_4.trk_nb_ = 302;
    tr_smr_302_4.on_gnd_ = true;
    tr_smr_302_4.x_ = 150.0;
    tr_smr_302_4.y_ = 200.0;
    tr_smr_302_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_5;
    tr_smr_302_5.ds_id_.sac_ = 0;
    tr_smr_302_5.ds_id_.sic_ = 7;
    tr_smr_302_5.sys_typ_ = SystemType::Smr;
    tr_smr_302_5.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_smr_302_5.trk_nb_ = 302;
    tr_smr_302_5.on_gnd_ = true;
    tr_smr_302_5.x_ = 200.0;
    tr_smr_302_5.y_ = 200.0;
    tr_smr_302_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_6;
    tr_smr_302_6.ds_id_.sac_ = 0;
    tr_smr_302_6.ds_id_.sic_ = 7;
    tr_smr_302_6.sys_typ_ = SystemType::Smr;
    tr_smr_302_6.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_smr_302_6.trk_nb_ = 302;
    tr_smr_302_6.on_gnd_ = true;
    tr_smr_302_6.x_ = 250.0;
    tr_smr_302_6.y_ = 200.0;
    tr_smr_302_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_7;
    tr_smr_302_7.ds_id_.sac_ = 0;
    tr_smr_302_7.ds_id_.sic_ = 7;
    tr_smr_302_7.sys_typ_ = SystemType::Smr;
    tr_smr_302_7.tod_ = "2020-05-05T10:00:09.100Z"_ts;
    tr_smr_302_7.trk_nb_ = 302;
    tr_smr_302_7.on_gnd_ = true;
    tr_smr_302_7.x_ = 300.0;
    tr_smr_302_7.y_ = 200.0;
    tr_smr_302_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_302_8;
    tr_smr_302_8.ds_id_.sac_ = 0;
    tr_smr_302_8.ds_id_.sic_ = 7;
    tr_smr_302_8.sys_typ_ = SystemType::Smr;
    tr_smr_302_8.tod_ = "2020-05-05T10:00:10.100Z"_ts;
    tr_smr_302_8.trk_nb_ = 302;
    tr_smr_302_8.on_gnd_ = true;
    tr_smr_302_8.x_ = 350.0;
    tr_smr_302_8.y_ = 200.0;
    tr_smr_302_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_302_9;
    tr_smr_302_9.ds_id_.sac_ = 0;
    tr_smr_302_9.ds_id_.sic_ = 7;
    tr_smr_302_9.sys_typ_ = SystemType::Smr;
    tr_smr_302_9.tod_ = "2020-05-05T10:00:11.100Z"_ts;
    tr_smr_302_9.trk_nb_ = 302;
    tr_smr_302_9.on_gnd_ = true;
    tr_smr_302_9.x_ = 400.0;
    tr_smr_302_9.y_ = 200.0;
    tr_smr_302_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_302_10;
    tr_smr_302_10.ds_id_.sac_ = 0;
    tr_smr_302_10.ds_id_.sic_ = 7;
    tr_smr_302_10.sys_typ_ = SystemType::Smr;
    tr_smr_302_10.tod_ = "2020-05-05T10:00:12.100Z"_ts;
    tr_smr_302_10.trk_nb_ = 302;
    tr_smr_302_10.on_gnd_ = true;
    tr_smr_302_10.x_ = 450.0;
    tr_smr_302_10.y_ = 200.0;
    tr_smr_302_10.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_302_11;
    tr_smr_302_11.ds_id_.sac_ = 0;
    tr_smr_302_11.ds_id_.sic_ = 7;
    tr_smr_302_11.sys_typ_ = SystemType::Smr;
    tr_smr_302_11.tod_ = "2020-05-05T10:00:13.100Z"_ts;
    tr_smr_302_11.trk_nb_ = 302;
    tr_smr_302_11.on_gnd_ = true;
    tr_smr_302_11.x_ = 500.0;
    tr_smr_302_11.y_ = 200.0;
    tr_smr_302_11.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_302_12;
    tr_smr_302_12.ds_id_.sac_ = 0;
    tr_smr_302_12.ds_id_.sic_ = 7;
    tr_smr_302_12.sys_typ_ = SystemType::Smr;
    tr_smr_302_12.tod_ = "2020-05-05T10:00:14.100Z"_ts;
    tr_smr_302_12.trk_nb_ = 302;
    tr_smr_302_12.on_gnd_ = true;
    tr_smr_302_12.x_ = 550.0;
    tr_smr_302_12.y_ = 200.0;
    tr_smr_302_12.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_smr_302 << tr_smr_302_1 << tr_smr_302_2 << tr_smr_302_3
                << tr_smr_302_4 << tr_smr_302_5 << tr_smr_302_6
                << tr_smr_302_7 << tr_smr_302_8 << tr_smr_302_9
                << tr_smr_302_10 << tr_smr_302_11 << tr_smr_302_12;


    // SMR SPURIOUS track 401.
    Track trk_smr_401(SystemType::Smr, 401);

    TargetReport tr_smr_401_1;
    tr_smr_401_1.ds_id_.sac_ = 0;
    tr_smr_401_1.ds_id_.sic_ = 7;
    tr_smr_401_1.sys_typ_ = SystemType::Smr;
    tr_smr_401_1.tod_ = "2020-05-05T10:00:03.200Z"_ts;
    tr_smr_401_1.trk_nb_ = 401;
    tr_smr_401_1.on_gnd_ = true;
    tr_smr_401_1.x_ = 150.0;
    tr_smr_401_1.y_ = 5.0;
    tr_smr_401_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_401_2;
    tr_smr_401_2.ds_id_.sac_ = 0;
    tr_smr_401_2.ds_id_.sic_ = 7;
    tr_smr_401_2.sys_typ_ = SystemType::Smr;
    tr_smr_401_2.tod_ = "2020-05-05T10:00:04.200Z"_ts;
    tr_smr_401_2.trk_nb_ = 401;
    tr_smr_401_2.on_gnd_ = true;
    tr_smr_401_2.x_ = 200.0;
    tr_smr_401_2.y_ = 5.0;
    tr_smr_401_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_401_3;
    tr_smr_401_3.ds_id_.sac_ = 0;
    tr_smr_401_3.ds_id_.sic_ = 7;
    tr_smr_401_3.sys_typ_ = SystemType::Smr;
    tr_smr_401_3.tod_ = "2020-05-05T10:00:05.200Z"_ts;
    tr_smr_401_3.trk_nb_ = 401;
    tr_smr_401_3.on_gnd_ = true;
    tr_smr_401_3.x_ = 250.0;
    tr_smr_401_3.y_ = 5.0;
    tr_smr_401_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_smr_401 << tr_smr_401_1 << tr_smr_401_2 << tr_smr_401_3;


    // SMR SPURIOUS track 402.
    Track trk_smr_402(SystemType::Smr, 402);

    TargetReport tr_smr_402_1;
    tr_smr_402_1.ds_id_.sac_ = 0;
    tr_smr_402_1.ds_id_.sic_ = 7;
    tr_smr_402_1.sys_typ_ = SystemType::Smr;
    tr_smr_402_1.tod_ = "2020-05-05T10:00:08.200Z"_ts;
    tr_smr_402_1.trk_nb_ = 402;
    tr_smr_402_1.on_gnd_ = true;
    tr_smr_402_1.x_ = 250.0;
    tr_smr_402_1.y_ = 205.0;
    tr_smr_402_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_402_2;
    tr_smr_402_2.ds_id_.sac_ = 0;
    tr_smr_402_2.ds_id_.sic_ = 7;
    tr_smr_402_2.sys_typ_ = SystemType::Smr;
    tr_smr_402_2.tod_ = "2020-05-05T10:00:09.200Z"_ts;
    tr_smr_402_2.trk_nb_ = 402;
    tr_smr_402_2.on_gnd_ = true;
    tr_smr_402_2.x_ = 300.0;
    tr_smr_402_2.y_ = 205.0;
    tr_smr_402_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_smr_402_3;
    tr_smr_402_3.ds_id_.sac_ = 0;
    tr_smr_402_3.ds_id_.sic_ = 7;
    tr_smr_402_3.sys_typ_ = SystemType::Smr;
    tr_smr_402_3.tod_ = "2020-05-05T10:00:10.200Z"_ts;
    tr_smr_402_3.trk_nb_ = 402;
    tr_smr_402_3.on_gnd_ = true;
    tr_smr_402_3.x_ = 350.0;
    tr_smr_402_3.y_ = 205.0;
    tr_smr_402_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_smr_402 << tr_smr_402_1 << tr_smr_402_2 << tr_smr_402_3;


    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101
               << trk_smr_301;

    QVector<Track> tracksIn_3;
    tracksIn_3 << trk_adsb_101
               << trk_smr_301 << trk_smr_401;

    QVector<Track> tracksIn_4;
    tracksIn_4 << trk_adsb_101 << trk_adsb_102;

    QVector<Track> tracksIn_5;
    tracksIn_5 << trk_adsb_101 << trk_adsb_102
               << trk_smr_301 << trk_smr_302;

    QVector<Track> tracksIn_6;
    tracksIn_6 << trk_adsb_101 << trk_adsb_102
               << trk_smr_301 << trk_smr_302 << trk_smr_401 << trk_smr_402;


    QHash<Aerodrome::NamedArea, Counters::PfdCounter2> pfdCtr_1;
    pfdCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 0;
    pfdCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_etr_ = 5;
    pfdCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_u_ = 5;
    pfdCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 0;
    pfdCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etr_ = 5;
    pfdCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_u_ = 5;

    QHash<Aerodrome::NamedArea, Counters::PfdCounter2> pfdCtr_2;
    pfdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 5;
    pfdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_etr_ = 5;
    pfdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_u_ = 5;
    pfdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 5;
    pfdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etr_ = 5;
    pfdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_u_ = 5;

    QHash<Aerodrome::NamedArea, Counters::PfdCounter2> pfdCtr_3;
    pfdCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 7;
    pfdCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_etr_ = 5;
    pfdCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_u_ = 5;
    pfdCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 5;
    pfdCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etr_ = 5;
    pfdCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_u_ = 5;

    QHash<Aerodrome::NamedArea, Counters::PfdCounter2> pfdCtr_4;
    pfdCtr_4[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 0;
    pfdCtr_4[Aerodrome::NamedArea(Aerodrome::Runway)].n_etr_ = 10;
    pfdCtr_4[Aerodrome::NamedArea(Aerodrome::Runway)].n_u_ = 8;
    pfdCtr_4[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 0;
    pfdCtr_4[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etr_ = 10;
    pfdCtr_4[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_u_ = 8;

    QHash<Aerodrome::NamedArea, Counters::PfdCounter2> pfdCtr_5;
    pfdCtr_5[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 10;
    pfdCtr_5[Aerodrome::NamedArea(Aerodrome::Runway)].n_etr_ = 10;
    pfdCtr_5[Aerodrome::NamedArea(Aerodrome::Runway)].n_u_ = 8;
    pfdCtr_5[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 10;
    pfdCtr_5[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etr_ = 10;
    pfdCtr_5[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_u_ = 8;

    QHash<Aerodrome::NamedArea, Counters::PfdCounter2> pfdCtr_6;
    pfdCtr_6[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 12;
    pfdCtr_6[Aerodrome::NamedArea(Aerodrome::Runway)].n_etr_ = 10;
    pfdCtr_6[Aerodrome::NamedArea(Aerodrome::Runway)].n_u_ = 8;
    pfdCtr_6[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 12;
    pfdCtr_6[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etr_ = 10;
    pfdCtr_6[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_u_ = 8;


    QTest::newRow("PFD NO TEST DATA (1 target)") << tracksIn_1 << pfdCtr_1;
    QTest::newRow("PFD 0 % (1 target)") << tracksIn_2 << pfdCtr_2;
    QTest::newRow("PFD 20 % (1 target)") << tracksIn_3 << pfdCtr_3;

    QTest::newRow("PFD NO TEST DATA (2 targets)") << tracksIn_4 << pfdCtr_4;
    QTest::newRow("PFD 0 % (2 targets)") << tracksIn_5 << pfdCtr_5;
    QTest::newRow("PFD 28,57 % (2 targets)") << tracksIn_6 << pfdCtr_6;
}

void PerfEvaluatorTest::testED116PFD()
{
    using PfdHash = QHash<Aerodrome::NamedArea, Counters::PfdCounter2>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(PfdHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.smrPfd_, countersOut);
}

void PerfEvaluatorTest::testED117RPA_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<AreaHash<QVector<double>>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_1.ver_ = 2;
    tr_adsb_101_1.pic_ = 14;

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_2.ver_ = 2;
    tr_adsb_101_2.pic_ = 14;

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_3.ver_ = 2;
    tr_adsb_101_3.pic_ = 14;

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_4.ver_ = 2;
    tr_adsb_101_4.pic_ = 14;

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_5.ver_ = 2;
    tr_adsb_101_5.pic_ = 14;

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);
    tr_adsb_101_6.ver_ = 2;
    tr_adsb_101_6.pic_ = 14;

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);
    tr_adsb_101_7.ver_ = 2;
    tr_adsb_101_7.pic_ = 14;

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);
    tr_adsb_101_8.ver_ = 2;
    tr_adsb_101_8.pic_ = 14;

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);
    tr_adsb_101_9.ver_ = 2;
    tr_adsb_101_9.pic_ = 14;

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // MLAT track 201.
    Track trk_mlat_201(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000001;
    tr_mlat_201_1.mode_3a_ = 0001;
    tr_mlat_201_1.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_s_ = 0x000001;
    tr_mlat_201_2.mode_3a_ = 0001;
    tr_mlat_201_2.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 50.0;
    tr_mlat_201_2.y_ = 5.0;
    tr_mlat_201_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_s_ = 0x000001;
    tr_mlat_201_3.mode_3a_ = 0001;
    tr_mlat_201_3.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 100.0;
    tr_mlat_201_3.y_ = 10.0;
    tr_mlat_201_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_s_ = 0x000001;
    tr_mlat_201_4.mode_3a_ = 0001;
    tr_mlat_201_4.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 150.0;
    tr_mlat_201_4.y_ = 15.0;
    tr_mlat_201_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_5;
    tr_mlat_201_5.ds_id_.sac_ = 0;
    tr_mlat_201_5.ds_id_.sic_ = 107;
    tr_mlat_201_5.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_mlat_201_5.trk_nb_ = 201;
    tr_mlat_201_5.mode_s_ = 0x000001;
    tr_mlat_201_5.mode_3a_ = 0001;
    tr_mlat_201_5.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_5.on_gnd_ = true;
    tr_mlat_201_5.x_ = 200.0;
    tr_mlat_201_5.y_ = 20.0;
    tr_mlat_201_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_6;
    tr_mlat_201_6.ds_id_.sac_ = 0;
    tr_mlat_201_6.ds_id_.sic_ = 107;
    tr_mlat_201_6.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_mlat_201_6.trk_nb_ = 201;
    tr_mlat_201_6.mode_s_ = 0x000001;
    tr_mlat_201_6.mode_3a_ = 0001;
    tr_mlat_201_6.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_6.on_gnd_ = true;
    tr_mlat_201_6.x_ = 250.0;
    tr_mlat_201_6.y_ = 25.0;
    tr_mlat_201_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_7;
    tr_mlat_201_7.ds_id_.sac_ = 0;
    tr_mlat_201_7.ds_id_.sic_ = 107;
    tr_mlat_201_7.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_mlat_201_7.trk_nb_ = 201;
    tr_mlat_201_7.mode_s_ = 0x000001;
    tr_mlat_201_7.mode_3a_ = 0001;
    tr_mlat_201_7.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_7.on_gnd_ = true;
    tr_mlat_201_7.x_ = 300.0;
    tr_mlat_201_7.y_ = 30.0;
    tr_mlat_201_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_8;
    tr_mlat_201_8.ds_id_.sac_ = 0;
    tr_mlat_201_8.ds_id_.sic_ = 107;
    tr_mlat_201_8.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_mlat_201_8.trk_nb_ = 201;
    tr_mlat_201_8.mode_s_ = 0x000001;
    tr_mlat_201_8.mode_3a_ = 0001;
    tr_mlat_201_8.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_8.on_gnd_ = true;
    tr_mlat_201_8.x_ = 350.0;
    tr_mlat_201_8.y_ = 30.0;
    tr_mlat_201_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_9;
    tr_mlat_201_9.ds_id_.sac_ = 0;
    tr_mlat_201_9.ds_id_.sic_ = 107;
    tr_mlat_201_9.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_mlat_201_9.trk_nb_ = 201;
    tr_mlat_201_9.mode_s_ = 0x000001;
    tr_mlat_201_9.mode_3a_ = 0001;
    tr_mlat_201_9.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_9.on_gnd_ = true;
    tr_mlat_201_9.x_ = 400.0;
    tr_mlat_201_9.y_ = 30.0;
    tr_mlat_201_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_mlat_201 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3
                 << tr_mlat_201_4 << tr_mlat_201_5 << tr_mlat_201_6
                 << tr_mlat_201_7 << tr_mlat_201_8 << tr_mlat_201_9;


    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_mlat_201;

    AreaHash<QVector<double>> rpaErr_1;

    AreaHash<QVector<double>> rpaErr_2;
    rpaErr_2[Aerodrome::NamedArea(Aerodrome::Runway)]
        << 0 << 5 << 10 << 15 << 20 << 25;
    rpaErr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)]
        << 30 << 30 << 30;

    QTest::newRow("RPA NO DATA") << tracksIn_1 << rpaErr_1;
    QTest::newRow("RPA") << tracksIn_2 << rpaErr_2;
}

void PerfEvaluatorTest::testED117RPA()
{
    using RpaHash = AreaHash<QVector<double>>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(RpaHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.mlatRpaErrors_, countersOut);
}

void PerfEvaluatorTest::testED117UR_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::UrCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // MLAT track 201.
    Track trk_mlat_201(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000001;
    tr_mlat_201_1.mode_3a_ = 0001;
    tr_mlat_201_1.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_s_ = 0x000001;
    tr_mlat_201_2.mode_3a_ = 0001;
    tr_mlat_201_2.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 50.0;
    tr_mlat_201_2.y_ = 0.0;
    tr_mlat_201_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_s_ = 0x000001;
    tr_mlat_201_3.mode_3a_ = 0001;
    tr_mlat_201_3.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 100.0;
    tr_mlat_201_3.y_ = 0.0;
    tr_mlat_201_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_s_ = 0x000001;
    tr_mlat_201_4.mode_3a_ = 0001;
    tr_mlat_201_4.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 150.0;
    tr_mlat_201_4.y_ = 0.0;
    tr_mlat_201_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_5;
    tr_mlat_201_5.ds_id_.sac_ = 0;
    tr_mlat_201_5.ds_id_.sic_ = 107;
    tr_mlat_201_5.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_mlat_201_5.trk_nb_ = 201;
    tr_mlat_201_5.mode_s_ = 0x000001;
    tr_mlat_201_5.mode_3a_ = 0001;
    tr_mlat_201_5.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_5.on_gnd_ = true;
    tr_mlat_201_5.x_ = 200.0;
    tr_mlat_201_5.y_ = 0.0;
    tr_mlat_201_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_6;
    tr_mlat_201_6.ds_id_.sac_ = 0;
    tr_mlat_201_6.ds_id_.sic_ = 107;
    tr_mlat_201_6.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_mlat_201_6.trk_nb_ = 201;
    tr_mlat_201_6.mode_s_ = 0x000001;
    tr_mlat_201_6.mode_3a_ = 0001;
    tr_mlat_201_6.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_6.on_gnd_ = true;
    tr_mlat_201_6.x_ = 250.0;
    tr_mlat_201_6.y_ = 0.0;
    tr_mlat_201_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_7;
    tr_mlat_201_7.ds_id_.sac_ = 0;
    tr_mlat_201_7.ds_id_.sic_ = 107;
    tr_mlat_201_7.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_mlat_201_7.trk_nb_ = 201;
    tr_mlat_201_7.mode_s_ = 0x000001;
    tr_mlat_201_7.mode_3a_ = 0001;
    tr_mlat_201_7.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_7.on_gnd_ = true;
    tr_mlat_201_7.x_ = 300.0;
    tr_mlat_201_7.y_ = 0.0;
    tr_mlat_201_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_8;
    tr_mlat_201_8.ds_id_.sac_ = 0;
    tr_mlat_201_8.ds_id_.sic_ = 107;
    tr_mlat_201_8.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_mlat_201_8.trk_nb_ = 201;
    tr_mlat_201_8.mode_s_ = 0x000001;
    tr_mlat_201_8.mode_3a_ = 0001;
    tr_mlat_201_8.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_8.on_gnd_ = true;
    tr_mlat_201_8.x_ = 350.0;
    tr_mlat_201_8.y_ = 0.0;
    tr_mlat_201_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_9;
    tr_mlat_201_9.ds_id_.sac_ = 0;
    tr_mlat_201_9.ds_id_.sic_ = 107;
    tr_mlat_201_9.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_mlat_201_9.trk_nb_ = 201;
    tr_mlat_201_9.mode_s_ = 0x000001;
    tr_mlat_201_9.mode_3a_ = 0001;
    tr_mlat_201_9.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_9.on_gnd_ = true;
    tr_mlat_201_9.x_ = 400.0;
    tr_mlat_201_9.y_ = 0.0;
    tr_mlat_201_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_mlat_201 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3
                 << tr_mlat_201_4 << tr_mlat_201_5 << tr_mlat_201_6
                 << tr_mlat_201_7 << tr_mlat_201_8 << tr_mlat_201_9;

    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_mlat_201;

    QHash<Aerodrome::NamedArea, Counters::UrCounter> urCtr_1;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_etrp_ = 5;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 0;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etrp_ = 2;
    urCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 0;

    QHash<Aerodrome::NamedArea, Counters::UrCounter> urCtr_2;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_etrp_ = 5;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 5;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_etrp_ = 2;
    urCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 2;

    QTest::newRow("UR 0 %") << tracksIn_1 << urCtr_1;
    QTest::newRow("UR 100 %") << tracksIn_2 << urCtr_2;
}

void PerfEvaluatorTest::testED117UR()
{
    using UrHash = QHash<Aerodrome::NamedArea, Counters::UrCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(UrHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.mlatUr_, countersOut);
}

void PerfEvaluatorTest::testED117PD_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::PdCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // MLAT track 201.
    Track trk_mlat_201(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000001;
    tr_mlat_201_1.mode_3a_ = 0001;
    tr_mlat_201_1.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_s_ = 0x000001;
    tr_mlat_201_2.mode_3a_ = 0001;
    tr_mlat_201_2.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 50.0;
    tr_mlat_201_2.y_ = 0.0;
    tr_mlat_201_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_s_ = 0x000001;
    tr_mlat_201_3.mode_3a_ = 0001;
    tr_mlat_201_3.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 100.0;
    tr_mlat_201_3.y_ = 0.0;
    tr_mlat_201_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_s_ = 0x000001;
    tr_mlat_201_4.mode_3a_ = 0001;
    tr_mlat_201_4.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 150.0;
    tr_mlat_201_4.y_ = 0.0;
    tr_mlat_201_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_5;
    tr_mlat_201_5.ds_id_.sac_ = 0;
    tr_mlat_201_5.ds_id_.sic_ = 107;
    tr_mlat_201_5.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_mlat_201_5.trk_nb_ = 201;
    tr_mlat_201_5.mode_s_ = 0x000001;
    tr_mlat_201_5.mode_3a_ = 0001;
    tr_mlat_201_5.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_5.on_gnd_ = true;
    tr_mlat_201_5.x_ = 200.0;
    tr_mlat_201_5.y_ = 0.0;
    tr_mlat_201_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_6;
    tr_mlat_201_6.ds_id_.sac_ = 0;
    tr_mlat_201_6.ds_id_.sic_ = 107;
    tr_mlat_201_6.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_mlat_201_6.trk_nb_ = 201;
    tr_mlat_201_6.mode_s_ = 0x000001;
    tr_mlat_201_6.mode_3a_ = 0001;
    tr_mlat_201_6.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_6.on_gnd_ = true;
    tr_mlat_201_6.x_ = 250.0;
    tr_mlat_201_6.y_ = 0.0;
    tr_mlat_201_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_7;
    tr_mlat_201_7.ds_id_.sac_ = 0;
    tr_mlat_201_7.ds_id_.sic_ = 107;
    tr_mlat_201_7.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_mlat_201_7.trk_nb_ = 201;
    tr_mlat_201_7.mode_s_ = 0x000001;
    tr_mlat_201_7.mode_3a_ = 0001;
    tr_mlat_201_7.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_7.on_gnd_ = true;
    tr_mlat_201_7.x_ = 300.0;
    tr_mlat_201_7.y_ = 0.0;
    tr_mlat_201_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_8;
    tr_mlat_201_8.ds_id_.sac_ = 0;
    tr_mlat_201_8.ds_id_.sic_ = 107;
    tr_mlat_201_8.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_mlat_201_8.trk_nb_ = 201;
    tr_mlat_201_8.mode_s_ = 0x000001;
    tr_mlat_201_8.mode_3a_ = 0001;
    tr_mlat_201_8.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_8.on_gnd_ = true;
    tr_mlat_201_8.x_ = 350.0;
    tr_mlat_201_8.y_ = 0.0;
    tr_mlat_201_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_9;
    tr_mlat_201_9.ds_id_.sac_ = 0;
    tr_mlat_201_9.ds_id_.sic_ = 107;
    tr_mlat_201_9.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_mlat_201_9.trk_nb_ = 201;
    tr_mlat_201_9.mode_s_ = 0x000001;
    tr_mlat_201_9.mode_3a_ = 0001;
    tr_mlat_201_9.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_9.on_gnd_ = true;
    tr_mlat_201_9.x_ = 400.0;
    tr_mlat_201_9.y_ = 0.0;
    tr_mlat_201_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_mlat_201 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3
                 << tr_mlat_201_4 << tr_mlat_201_5 << tr_mlat_201_6
                 << tr_mlat_201_7 << tr_mlat_201_8 << tr_mlat_201_9;

    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_mlat_201;

    QHash<Aerodrome::NamedArea, Counters::PdCounter> pdCtr_1;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_up_ = 5;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 0;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_up_ = 1;
    pdCtr_1[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 0;

    QHash<Aerodrome::NamedArea, Counters::PdCounter> pdCtr_2;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_up_ = 5;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_trp_ = 5;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_up_ = 1;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_trp_ = 1;

    QTest::newRow("PD 0 %") << tracksIn_1 << pdCtr_1;
    QTest::newRow("PD 100 %") << tracksIn_2 << pdCtr_2;
}

void PerfEvaluatorTest::testED117PD()
{
    using PdHash = QHash<Aerodrome::NamedArea, Counters::PdCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(PdHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.mlatPd_, countersOut);
}

void PerfEvaluatorTest::testED117PFD_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::PfdCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // MLAT track 201.
    Track trk_mlat_201_1(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000001;
    tr_mlat_201_1.mode_3a_ = 0001;
    tr_mlat_201_1.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_s_ = 0x000001;
    tr_mlat_201_2.mode_3a_ = 0001;
    tr_mlat_201_2.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 50.0;
    tr_mlat_201_2.y_ = 0.0;
    tr_mlat_201_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_s_ = 0x000001;
    tr_mlat_201_3.mode_3a_ = 0001;
    tr_mlat_201_3.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 100.0;
    tr_mlat_201_3.y_ = 0.0;
    tr_mlat_201_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_s_ = 0x000001;
    tr_mlat_201_4.mode_3a_ = 0001;
    tr_mlat_201_4.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 150.0;
    tr_mlat_201_4.y_ = 0.0;
    tr_mlat_201_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_5;
    tr_mlat_201_5.ds_id_.sac_ = 0;
    tr_mlat_201_5.ds_id_.sic_ = 107;
    tr_mlat_201_5.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_mlat_201_5.trk_nb_ = 201;
    tr_mlat_201_5.mode_s_ = 0x000001;
    tr_mlat_201_5.mode_3a_ = 0001;
    tr_mlat_201_5.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_5.on_gnd_ = true;
    tr_mlat_201_5.x_ = 200.0;
    tr_mlat_201_5.y_ = 0.0;
    tr_mlat_201_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_6;
    tr_mlat_201_6.ds_id_.sac_ = 0;
    tr_mlat_201_6.ds_id_.sic_ = 107;
    tr_mlat_201_6.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_mlat_201_6.trk_nb_ = 201;
    tr_mlat_201_6.mode_s_ = 0x000001;
    tr_mlat_201_6.mode_3a_ = 0001;
    tr_mlat_201_6.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_6.on_gnd_ = true;
    tr_mlat_201_6.x_ = 250.0;
    tr_mlat_201_6.y_ = 0.0;
    tr_mlat_201_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_7;
    tr_mlat_201_7.ds_id_.sac_ = 0;
    tr_mlat_201_7.ds_id_.sic_ = 107;
    tr_mlat_201_7.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_mlat_201_7.trk_nb_ = 201;
    tr_mlat_201_7.mode_s_ = 0x000001;
    tr_mlat_201_7.mode_3a_ = 0001;
    tr_mlat_201_7.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_7.on_gnd_ = true;
    tr_mlat_201_7.x_ = 300.0;
    tr_mlat_201_7.y_ = 0.0;
    tr_mlat_201_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_8;
    tr_mlat_201_8.ds_id_.sac_ = 0;
    tr_mlat_201_8.ds_id_.sic_ = 107;
    tr_mlat_201_8.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_mlat_201_8.trk_nb_ = 201;
    tr_mlat_201_8.mode_s_ = 0x000001;
    tr_mlat_201_8.mode_3a_ = 0001;
    tr_mlat_201_8.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_8.on_gnd_ = true;
    tr_mlat_201_8.x_ = 350.0;
    tr_mlat_201_8.y_ = 0.0;
    tr_mlat_201_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_9;
    tr_mlat_201_9.ds_id_.sac_ = 0;
    tr_mlat_201_9.ds_id_.sic_ = 107;
    tr_mlat_201_9.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_mlat_201_9.trk_nb_ = 201;
    tr_mlat_201_9.mode_s_ = 0x000001;
    tr_mlat_201_9.mode_3a_ = 0001;
    tr_mlat_201_9.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_9.on_gnd_ = true;
    tr_mlat_201_9.x_ = 400.0;
    tr_mlat_201_9.y_ = 0.0;
    tr_mlat_201_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_mlat_201_1 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3
                   << tr_mlat_201_4 << tr_mlat_201_5 << tr_mlat_201_6
                   << tr_mlat_201_7 << tr_mlat_201_8 << tr_mlat_201_9;


    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;


    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_mlat_201_1;


    Track trk_mlat_201_3 = trk_mlat_201_1;
    for (TargetReport &tr : trk_mlat_201_3)
    {
        tr.y_ = 100.0;
    }
    QVector<Track> tracksIn_3;
    tracksIn_3 << trk_adsb_101 << trk_mlat_201_3;


    QHash<Aerodrome::NamedArea, Counters::PfdCounter> pdCtr_1;

    QHash<Aerodrome::NamedArea, Counters::PfdCounter> pdCtr_2;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 5;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_ftr_ = 0;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 2;
    pdCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_ftr_ = 0;

    QHash<Aerodrome::NamedArea, Counters::PfdCounter> pdCtr_3;
    pdCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 5;
    pdCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_ftr_ = 5;
    pdCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_tr_ = 2;
    pdCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_ftr_ = 2;


    QTest::newRow("PFD NO DATA") << tracksIn_1 << pdCtr_1;
    QTest::newRow("PFD 0 %") << tracksIn_2 << pdCtr_2;
    QTest::newRow("PFD 100 %") << tracksIn_3 << pdCtr_3;
}

void PerfEvaluatorTest::testED117PFD()
{
    using PfdHash = QHash<Aerodrome::NamedArea, Counters::PfdCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(PfdHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.mlatPfd_, countersOut);
}

void PerfEvaluatorTest::testED117PID_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::PidCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // MLAT track 201.
    Track trk_mlat_201_1(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000001;
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_s_ = 0x000001;
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 50.0;
    tr_mlat_201_2.y_ = 0.0;
    tr_mlat_201_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_s_ = 0x000001;
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 100.0;
    tr_mlat_201_3.y_ = 0.0;
    tr_mlat_201_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_s_ = 0x000001;
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 150.0;
    tr_mlat_201_4.y_ = 0.0;
    tr_mlat_201_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_5;
    tr_mlat_201_5.ds_id_.sac_ = 0;
    tr_mlat_201_5.ds_id_.sic_ = 107;
    tr_mlat_201_5.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_mlat_201_5.trk_nb_ = 201;
    tr_mlat_201_5.mode_s_ = 0x000001;
    tr_mlat_201_5.on_gnd_ = true;
    tr_mlat_201_5.x_ = 200.0;
    tr_mlat_201_5.y_ = 0.0;
    tr_mlat_201_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_6;
    tr_mlat_201_6.ds_id_.sac_ = 0;
    tr_mlat_201_6.ds_id_.sic_ = 107;
    tr_mlat_201_6.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_mlat_201_6.trk_nb_ = 201;
    tr_mlat_201_6.mode_s_ = 0x000001;
    tr_mlat_201_6.on_gnd_ = true;
    tr_mlat_201_6.x_ = 250.0;
    tr_mlat_201_6.y_ = 0.0;
    tr_mlat_201_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_7;
    tr_mlat_201_7.ds_id_.sac_ = 0;
    tr_mlat_201_7.ds_id_.sic_ = 107;
    tr_mlat_201_7.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_mlat_201_7.trk_nb_ = 201;
    tr_mlat_201_7.mode_s_ = 0x000001;
    tr_mlat_201_7.on_gnd_ = true;
    tr_mlat_201_7.x_ = 300.0;
    tr_mlat_201_7.y_ = 0.0;
    tr_mlat_201_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_8;
    tr_mlat_201_8.ds_id_.sac_ = 0;
    tr_mlat_201_8.ds_id_.sic_ = 107;
    tr_mlat_201_8.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_mlat_201_8.trk_nb_ = 201;
    tr_mlat_201_8.mode_s_ = 0x000001;
    tr_mlat_201_8.on_gnd_ = true;
    tr_mlat_201_8.x_ = 350.0;
    tr_mlat_201_8.y_ = 0.0;
    tr_mlat_201_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_9;
    tr_mlat_201_9.ds_id_.sac_ = 0;
    tr_mlat_201_9.ds_id_.sic_ = 107;
    tr_mlat_201_9.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_mlat_201_9.trk_nb_ = 201;
    tr_mlat_201_9.mode_s_ = 0x000001;
    tr_mlat_201_9.on_gnd_ = true;
    tr_mlat_201_9.x_ = 400.0;
    tr_mlat_201_9.y_ = 0.0;
    tr_mlat_201_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_mlat_201_1 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3
                   << tr_mlat_201_4 << tr_mlat_201_5 << tr_mlat_201_6
                   << tr_mlat_201_7 << tr_mlat_201_8 << tr_mlat_201_9;


    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101 << trk_mlat_201_1;


    Track trk_mlat_201_2 = trk_mlat_201_1;
    for (TargetReport &tr : trk_mlat_201_2)
    {
        //tr.mode_3a_ = 0001;
        tr.ident_ = QLatin1String("FOO5678 ");
    }
    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_mlat_201_2;


    Track trk_mlat_201_3 = trk_mlat_201_1;
    for (TargetReport &tr : trk_mlat_201_3)
    {
        //tr.mode_3a_ = 0001;
        tr.ident_ = QLatin1String("FOO1234 ");
    }
    QVector<Track> tracksIn_3;
    tracksIn_3 << trk_adsb_101 << trk_mlat_201_3;


    QHash<Aerodrome::NamedArea, Counters::PidCounter> pidCtr_1;

    QHash<Aerodrome::NamedArea, Counters::PidCounter> pidCtr_2;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_itr_ = 5;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_citr_ = 0;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_itr_ = 2;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_citr_ = 0;

    QHash<Aerodrome::NamedArea, Counters::PidCounter> pidCtr_3;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_itr_ = 5;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_citr_ = 5;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_itr_ = 2;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_citr_ = 2;

    QTest::newRow("PID NO DATA") << tracksIn_1 << pidCtr_1;
    QTest::newRow("PID 0 %") << tracksIn_2 << pidCtr_2;
    QTest::newRow("PID 100 %") << tracksIn_3 << pidCtr_3;
}

void PerfEvaluatorTest::testED117PID()
{
    using PidHash = QHash<Aerodrome::NamedArea, Counters::PidCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(PidHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.mlatPidIdent_, countersOut);
}

void PerfEvaluatorTest::testED117PFID_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::PfidCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_s_ = 0x000001;
    tr_adsb_101_5.mode_3a_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_6;
    tr_adsb_101_6.ds_id_.sac_ = 0;
    tr_adsb_101_6.ds_id_.sic_ = 219;
    tr_adsb_101_6.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_6.trk_nb_ = 101;
    tr_adsb_101_6.mode_s_ = 0x000001;
    tr_adsb_101_6.mode_3a_ = 0001;
    tr_adsb_101_6.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_6.on_gnd_ = true;
    tr_adsb_101_6.x_ = 250.0;
    tr_adsb_101_6.y_ = 0.0;
    tr_adsb_101_6.z_ = 0.0;
    tr_adsb_101_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_7;
    tr_adsb_101_7.ds_id_.sac_ = 0;
    tr_adsb_101_7.ds_id_.sic_ = 219;
    tr_adsb_101_7.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_7.tod_ = "2020-05-05T10:00:06.000Z"_ts;
    tr_adsb_101_7.trk_nb_ = 101;
    tr_adsb_101_7.mode_s_ = 0x000001;
    tr_adsb_101_7.mode_3a_ = 0001;
    tr_adsb_101_7.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_7.on_gnd_ = true;
    tr_adsb_101_7.x_ = 300.0;
    tr_adsb_101_7.y_ = 0.0;
    tr_adsb_101_7.z_ = 0.0;
    tr_adsb_101_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_8;
    tr_adsb_101_8.ds_id_.sac_ = 0;
    tr_adsb_101_8.ds_id_.sic_ = 219;
    tr_adsb_101_8.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_8.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_101_8.trk_nb_ = 101;
    tr_adsb_101_8.mode_s_ = 0x000001;
    tr_adsb_101_8.mode_3a_ = 0001;
    tr_adsb_101_8.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_8.on_gnd_ = true;
    tr_adsb_101_8.x_ = 350.0;
    tr_adsb_101_8.y_ = 0.0;
    tr_adsb_101_8.z_ = 0.0;
    tr_adsb_101_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_adsb_101_9;
    tr_adsb_101_9.ds_id_.sac_ = 0;
    tr_adsb_101_9.ds_id_.sic_ = 219;
    tr_adsb_101_9.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_9.tod_ = "2020-05-05T10:00:08.000Z"_ts;
    tr_adsb_101_9.trk_nb_ = 101;
    tr_adsb_101_9.mode_s_ = 0x000001;
    tr_adsb_101_9.mode_3a_ = 0001;
    tr_adsb_101_9.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_9.on_gnd_ = true;
    tr_adsb_101_9.x_ = 400.0;
    tr_adsb_101_9.y_ = 0.0;
    tr_adsb_101_9.z_ = 0.0;
    tr_adsb_101_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3
                 << tr_adsb_101_4 << tr_adsb_101_5 << tr_adsb_101_6
                 << tr_adsb_101_7 << tr_adsb_101_8 << tr_adsb_101_9;


    // MLAT track 201.
    Track trk_mlat_201_1(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000001;
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:01.100Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_s_ = 0x000001;
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 50.0;
    tr_mlat_201_2.y_ = 0.0;
    tr_mlat_201_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:02.100Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_s_ = 0x000001;
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 100.0;
    tr_mlat_201_3.y_ = 0.0;
    tr_mlat_201_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = "2020-05-05T10:00:03.100Z"_ts;
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_s_ = 0x000001;
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 150.0;
    tr_mlat_201_4.y_ = 0.0;
    tr_mlat_201_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_5;
    tr_mlat_201_5.ds_id_.sac_ = 0;
    tr_mlat_201_5.ds_id_.sic_ = 107;
    tr_mlat_201_5.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_5.tod_ = "2020-05-05T10:00:04.100Z"_ts;
    tr_mlat_201_5.trk_nb_ = 201;
    tr_mlat_201_5.mode_s_ = 0x000001;
    tr_mlat_201_5.on_gnd_ = true;
    tr_mlat_201_5.x_ = 200.0;
    tr_mlat_201_5.y_ = 0.0;
    tr_mlat_201_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_6;
    tr_mlat_201_6.ds_id_.sac_ = 0;
    tr_mlat_201_6.ds_id_.sic_ = 107;
    tr_mlat_201_6.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_6.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_mlat_201_6.trk_nb_ = 201;
    tr_mlat_201_6.mode_s_ = 0x000001;
    tr_mlat_201_6.on_gnd_ = true;
    tr_mlat_201_6.x_ = 250.0;
    tr_mlat_201_6.y_ = 0.0;
    tr_mlat_201_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_7;
    tr_mlat_201_7.ds_id_.sac_ = 0;
    tr_mlat_201_7.ds_id_.sic_ = 107;
    tr_mlat_201_7.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_7.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_mlat_201_7.trk_nb_ = 201;
    tr_mlat_201_7.mode_s_ = 0x000001;
    tr_mlat_201_7.on_gnd_ = true;
    tr_mlat_201_7.x_ = 300.0;
    tr_mlat_201_7.y_ = 0.0;
    tr_mlat_201_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_8;
    tr_mlat_201_8.ds_id_.sac_ = 0;
    tr_mlat_201_8.ds_id_.sic_ = 107;
    tr_mlat_201_8.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_8.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_mlat_201_8.trk_nb_ = 201;
    tr_mlat_201_8.mode_s_ = 0x000001;
    tr_mlat_201_8.on_gnd_ = true;
    tr_mlat_201_8.x_ = 350.0;
    tr_mlat_201_8.y_ = 0.0;
    tr_mlat_201_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    TargetReport tr_mlat_201_9;
    tr_mlat_201_9.ds_id_.sac_ = 0;
    tr_mlat_201_9.ds_id_.sic_ = 107;
    tr_mlat_201_9.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_9.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_mlat_201_9.trk_nb_ = 201;
    tr_mlat_201_9.mode_s_ = 0x000001;
    tr_mlat_201_9.on_gnd_ = true;
    tr_mlat_201_9.x_ = 400.0;
    tr_mlat_201_9.y_ = 0.0;
    tr_mlat_201_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);

    trk_mlat_201_1 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3
                   << tr_mlat_201_4 << tr_mlat_201_5 << tr_mlat_201_6
                   << tr_mlat_201_7 << tr_mlat_201_8 << tr_mlat_201_9;


    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101 << trk_mlat_201_1;


    Track trk_mlat_201_2 = trk_mlat_201_1;
    for (TargetReport &tr : trk_mlat_201_2)
    {
        //tr.mode_3a_ = 0001;
        tr.ident_ = QLatin1String("FOO5678 ");
    }
    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_mlat_201_2;


    Track trk_mlat_201_3 = trk_mlat_201_1;
    for (TargetReport &tr : trk_mlat_201_3)
    {
        //tr.mode_3a_ = 0001;
        tr.ident_ = QLatin1String("FOO1234 ");
    }
    QVector<Track> tracksIn_3;
    tracksIn_3 << trk_adsb_101 << trk_mlat_201_3;


    QHash<Aerodrome::NamedArea, Counters::PfidCounter> pidCtr_1;

    QHash<Aerodrome::NamedArea, Counters::PfidCounter> pidCtr_2;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_itr_ = 5;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_eitr_ = 5;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_itr_ = 2;
    pidCtr_2[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_eitr_ = 2;

    QHash<Aerodrome::NamedArea, Counters::PfidCounter> pidCtr_3;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_itr_ = 5;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Runway)].n_eitr_ = 0;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_itr_ = 2;
    pidCtr_3[Aerodrome::NamedArea(Aerodrome::Taxiway)].n_eitr_ = 0;

    QTest::newRow("PID NO DATA") << tracksIn_1 << pidCtr_1;
    QTest::newRow("PFID 100 %") << tracksIn_2 << pidCtr_2;
    QTest::newRow("PFID 0 %") << tracksIn_3 << pidCtr_3;
}

void PerfEvaluatorTest::testED117PFID()
{
    using PfidHash = QHash<Aerodrome::NamedArea, Counters::PfidCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(PfidHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.mlatPfidIdent_, countersOut);
}

void PerfEvaluatorTest::testED117PLG_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QHash<Aerodrome::NamedArea, Counters::PlgCounter>>("countersOut");


    // ADS-B track 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:09.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_s_ = 0x000001;
    tr_adsb_101_2.mode_3a_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:10.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_s_ = 0x000001;
    tr_adsb_101_3.mode_3a_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Stand);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = "2020-05-05T10:00:30.000Z"_ts;
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_s_ = 0x000001;
    tr_adsb_101_4.mode_3a_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Stand);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2
                 << tr_adsb_101_3 << tr_adsb_101_4;


    // MLAT track 201.
    Track trk_mlat_201_1(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.100Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000001;
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:05.100Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_s_ = 0x000001;
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 250.0;
    tr_mlat_201_2.y_ = 0.0;
    tr_mlat_201_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:06.100Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_s_ = 0x000001;
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 300.0;
    tr_mlat_201_3.y_ = 0.0;
    tr_mlat_201_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = "2020-05-05T10:00:07.100Z"_ts;
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_s_ = 0x000001;
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 350.0;
    tr_mlat_201_4.y_ = 0.0;
    tr_mlat_201_4.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_5;
    tr_mlat_201_5.ds_id_.sac_ = 0;
    tr_mlat_201_5.ds_id_.sic_ = 107;
    tr_mlat_201_5.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_5.tod_ = "2020-05-05T10:00:08.100Z"_ts;
    tr_mlat_201_5.trk_nb_ = 201;
    tr_mlat_201_5.mode_s_ = 0x000001;
    tr_mlat_201_5.on_gnd_ = true;
    tr_mlat_201_5.x_ = 400.0;
    tr_mlat_201_5.y_ = 0.0;
    tr_mlat_201_5.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_6;
    tr_mlat_201_6.ds_id_.sac_ = 0;
    tr_mlat_201_6.ds_id_.sic_ = 107;
    tr_mlat_201_6.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_6.tod_ = "2020-05-05T10:00:10.100Z"_ts;
    tr_mlat_201_6.trk_nb_ = 201;
    tr_mlat_201_6.mode_s_ = 0x000001;
    tr_mlat_201_6.on_gnd_ = true;
    tr_mlat_201_6.x_ = 400.0;
    tr_mlat_201_6.y_ = 0.0;
    tr_mlat_201_6.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Stand);

    TargetReport tr_mlat_201_7;
    tr_mlat_201_7.ds_id_.sac_ = 0;
    tr_mlat_201_7.ds_id_.sic_ = 107;
    tr_mlat_201_7.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_7.tod_ = "2020-05-05T10:00:26.100Z"_ts;
    tr_mlat_201_7.trk_nb_ = 201;
    tr_mlat_201_7.mode_s_ = 0x000001;
    tr_mlat_201_7.on_gnd_ = true;
    tr_mlat_201_7.x_ = 400.0;
    tr_mlat_201_7.y_ = 0.0;
    tr_mlat_201_7.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Stand);

    TargetReport tr_mlat_201_8;
    tr_mlat_201_8.ds_id_.sac_ = 0;
    tr_mlat_201_8.ds_id_.sic_ = 107;
    tr_mlat_201_8.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_8.tod_ = "2020-05-05T10:00:27.100Z"_ts;
    tr_mlat_201_8.trk_nb_ = 201;
    tr_mlat_201_8.mode_s_ = 0x000001;
    tr_mlat_201_8.on_gnd_ = true;
    tr_mlat_201_8.x_ = 400.0;
    tr_mlat_201_8.y_ = 0.0;
    tr_mlat_201_8.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Stand);

    TargetReport tr_mlat_201_9;
    tr_mlat_201_9.ds_id_.sac_ = 0;
    tr_mlat_201_9.ds_id_.sic_ = 107;
    tr_mlat_201_9.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_9.tod_ = "2020-05-05T10:00:28.100Z"_ts;
    tr_mlat_201_9.trk_nb_ = 201;
    tr_mlat_201_9.mode_s_ = 0x000001;
    tr_mlat_201_9.on_gnd_ = true;
    tr_mlat_201_9.x_ = 400.0;
    tr_mlat_201_9.y_ = 0.0;
    tr_mlat_201_9.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Stand);

    TargetReport tr_mlat_201_10;
    tr_mlat_201_10.ds_id_.sac_ = 0;
    tr_mlat_201_10.ds_id_.sic_ = 107;
    tr_mlat_201_10.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_10.tod_ = "2020-05-05T10:00:29.100Z"_ts;
    tr_mlat_201_10.trk_nb_ = 201;
    tr_mlat_201_10.mode_s_ = 0x000001;
    tr_mlat_201_10.on_gnd_ = true;
    tr_mlat_201_10.x_ = 400.0;
    tr_mlat_201_10.y_ = 0.0;
    tr_mlat_201_10.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Stand);

    trk_mlat_201_1 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3
                   << tr_mlat_201_4 << tr_mlat_201_5 << tr_mlat_201_6
                   << tr_mlat_201_7 << tr_mlat_201_8 << tr_mlat_201_9
                   << tr_mlat_201_10;


    QVector<Track> tracksIn_1;
    tracksIn_1 << trk_adsb_101;

    QVector<Track> tracksIn_2;
    tracksIn_2 << trk_adsb_101 << trk_mlat_201_1;

    QHash<Aerodrome::NamedArea, Counters::PlgCounter> plgCtr_1;

    QHash<Aerodrome::NamedArea, Counters::PlgCounter> plgCtr_2;
    plgCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_tr_ = 5;
    plgCtr_2[Aerodrome::NamedArea(Aerodrome::Runway)].n_g_ = 1;
    plgCtr_2[Aerodrome::NamedArea(Aerodrome::Stand)].n_tr_ = 5;
    plgCtr_2[Aerodrome::NamedArea(Aerodrome::Stand)].n_g_ = 1;

    QTest::newRow("PLG NO DATA") << tracksIn_1 << plgCtr_1;
    QTest::newRow("PLG 20 %") << tracksIn_2 << plgCtr_2;
}

void PerfEvaluatorTest::testED117PLG()
{
    using PlgHash = QHash<Aerodrome::NamedArea, Counters::PlgCounter>;

    QFETCH(QVector<Track>, tracksIn);
    QFETCH(PlgHash, countersOut);

    PerfEvaluator perfEval;
    for (const Track &trk : tracksIn)
    {
        perfEval.addData(trk);
    }
    perfEval.run();

    QCOMPARE(perfEval.mlatPlg_, countersOut);
}

QTEST_GUILESS_MAIN(PerfEvaluatorTest);
#include "perfevaluatortest.moc"
