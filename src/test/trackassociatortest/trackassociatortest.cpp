/*!
 * \file trackassociatortest.cpp
 * \brief Implements unit tests for the TrackAssociator class.
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

#include "trackassociator.h"
#include <QObject>
#include <QtTest>

class TrackAssociatorTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_data();
    void test();
};

void TrackAssociatorTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-trackassociatortest"));

    QSettings settings;
    settings.clear();

    settings.setValue(QLatin1String("Date"), QLatin1String("2020-05-05"));
    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 219);
}

void TrackAssociatorTest::test_data()
{
    QTest::addColumn<QVector<Track>>("tracksIn");
    QTest::addColumn<QVector<TrackCollectionSet>>("setsOut");

    // ADS-B track 101
    Track trk_adsb_101(SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:00.000Z"), Qt::ISODateWithMs);
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_S_ = 0x000001;
    tr_adsb_101_1.mode_3A_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = 0.0;
    tr_adsb_101_1.y_ = 0.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:01.000Z"), Qt::ISODateWithMs);
    tr_adsb_101_2.trk_nb_ = 101;
    tr_adsb_101_2.mode_S_ = 0x000001;
    tr_adsb_101_2.mode_3A_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 50.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:02.000Z"), Qt::ISODateWithMs);
    tr_adsb_101_3.trk_nb_ = 101;
    tr_adsb_101_3.mode_S_ = 0x000001;
    tr_adsb_101_3.mode_3A_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 100.0;
    tr_adsb_101_3.y_ = 0.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_4;
    tr_adsb_101_4.ds_id_.sac_ = 0;
    tr_adsb_101_4.ds_id_.sic_ = 219;
    tr_adsb_101_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_4.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:03.000Z"), Qt::ISODateWithMs);
    tr_adsb_101_4.trk_nb_ = 101;
    tr_adsb_101_4.mode_S_ = 0x000001;
    tr_adsb_101_4.mode_3A_ = 0001;
    tr_adsb_101_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_4.on_gnd_ = true;
    tr_adsb_101_4.x_ = 150.0;
    tr_adsb_101_4.y_ = 0.0;
    tr_adsb_101_4.z_ = 0.0;
    tr_adsb_101_4.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_5;
    tr_adsb_101_5.ds_id_.sac_ = 0;
    tr_adsb_101_5.ds_id_.sic_ = 219;
    tr_adsb_101_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_5.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:04.000Z"), Qt::ISODateWithMs);
    tr_adsb_101_5.trk_nb_ = 101;
    tr_adsb_101_5.mode_S_ = 0x000001;
    tr_adsb_101_5.mode_3A_ = 0001;
    tr_adsb_101_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_5.on_gnd_ = true;
    tr_adsb_101_5.x_ = 200.0;
    tr_adsb_101_5.y_ = 0.0;
    tr_adsb_101_5.z_ = 0.0;
    tr_adsb_101_5.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3 << tr_adsb_101_4 << tr_adsb_101_5;


    // ADS-B track 102
    Track trk_adsb_102(SystemType::Adsb, 102);

    TargetReport tr_adsb_102_1;
    tr_adsb_102_1.ds_id_.sac_ = 0;
    tr_adsb_102_1.ds_id_.sic_ = 219;
    tr_adsb_102_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:08.000Z"), Qt::ISODateWithMs);
    tr_adsb_102_1.trk_nb_ = 102;
    tr_adsb_102_1.mode_S_ = 0x000001;
    tr_adsb_102_1.mode_3A_ = 0001;
    tr_adsb_102_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_102_1.on_gnd_ = true;
    tr_adsb_102_1.x_ = 400.0;
    tr_adsb_102_1.y_ = 0.0;
    tr_adsb_102_1.z_ = 0.0;
    tr_adsb_102_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_2;
    tr_adsb_102_2.ds_id_.sac_ = 0;
    tr_adsb_102_2.ds_id_.sic_ = 219;
    tr_adsb_102_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:09.000Z"), Qt::ISODateWithMs);
    tr_adsb_102_2.trk_nb_ = 102;
    tr_adsb_102_2.mode_S_ = 0x000001;
    tr_adsb_102_2.mode_3A_ = 0001;
    tr_adsb_102_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_102_2.on_gnd_ = true;
    tr_adsb_102_2.x_ = 450.0;
    tr_adsb_102_2.y_ = 0.0;
    tr_adsb_102_2.z_ = 0.0;
    tr_adsb_102_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_3;
    tr_adsb_102_3.ds_id_.sac_ = 0;
    tr_adsb_102_3.ds_id_.sic_ = 219;
    tr_adsb_102_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:10.000Z"), Qt::ISODateWithMs);
    tr_adsb_102_3.trk_nb_ = 102;
    tr_adsb_102_3.mode_S_ = 0x000001;
    tr_adsb_102_3.mode_3A_ = 0001;
    tr_adsb_102_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_102_3.on_gnd_ = true;
    tr_adsb_102_3.x_ = 500.0;
    tr_adsb_102_3.y_ = 0.0;
    tr_adsb_102_3.z_ = 0.0;
    tr_adsb_102_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_4;
    tr_adsb_102_4.ds_id_.sac_ = 0;
    tr_adsb_102_4.ds_id_.sic_ = 219;
    tr_adsb_102_4.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_4.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:11.000Z"), Qt::ISODateWithMs);
    tr_adsb_102_4.trk_nb_ = 102;
    tr_adsb_102_4.mode_S_ = 0x000001;
    tr_adsb_102_4.mode_3A_ = 0001;
    tr_adsb_102_4.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_102_4.on_gnd_ = true;
    tr_adsb_102_4.x_ = 550.0;
    tr_adsb_102_4.y_ = 0.0;
    tr_adsb_102_4.z_ = 0.0;
    tr_adsb_102_4.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_5;
    tr_adsb_102_5.ds_id_.sac_ = 0;
    tr_adsb_102_5.ds_id_.sic_ = 219;
    tr_adsb_102_5.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_5.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:12.000Z"), Qt::ISODateWithMs);
    tr_adsb_102_5.trk_nb_ = 102;
    tr_adsb_102_5.mode_S_ = 0x000001;
    tr_adsb_102_5.mode_3A_ = 0001;
    tr_adsb_102_5.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_102_5.on_gnd_ = true;
    tr_adsb_102_5.x_ = 600.0;
    tr_adsb_102_5.y_ = 0.0;
    tr_adsb_102_5.z_ = 0.0;
    tr_adsb_102_5.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_adsb_102 << tr_adsb_102_1 << tr_adsb_102_2 << tr_adsb_102_3 << tr_adsb_102_4 << tr_adsb_102_5;


    // MLAT track 201.
    Track trk_mlat_201(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T09:59:59.000Z"), Qt::ISODateWithMs);
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_S_ = 0x000001;
    tr_mlat_201_1.mode_3A_ = 0001;
    tr_mlat_201_1.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = -50.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.ds_id_.sac_ = 0;
    tr_mlat_201_2.ds_id_.sic_ = 107;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:00.000Z"), Qt::ISODateWithMs);
    tr_mlat_201_2.trk_nb_ = 201;
    tr_mlat_201_2.mode_S_ = 0x000001;
    tr_mlat_201_2.mode_3A_ = 0001;
    tr_mlat_201_2.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_2.on_gnd_ = true;
    tr_mlat_201_2.x_ = 0.0;
    tr_mlat_201_2.y_ = 0.0;
    tr_mlat_201_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.ds_id_.sac_ = 0;
    tr_mlat_201_3.ds_id_.sic_ = 107;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:01.000Z"), Qt::ISODateWithMs);
    tr_mlat_201_3.trk_nb_ = 201;
    tr_mlat_201_3.mode_S_ = 0x000001;
    tr_mlat_201_3.mode_3A_ = 0001;
    tr_mlat_201_3.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_3.on_gnd_ = true;
    tr_mlat_201_3.x_ = 50.0;
    tr_mlat_201_3.y_ = 0.0;
    tr_mlat_201_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_4;
    tr_mlat_201_4.ds_id_.sac_ = 0;
    tr_mlat_201_4.ds_id_.sic_ = 107;
    tr_mlat_201_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_4.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:02.000Z"), Qt::ISODateWithMs);
    tr_mlat_201_4.trk_nb_ = 201;
    tr_mlat_201_4.mode_S_ = 0x000001;
    tr_mlat_201_4.mode_3A_ = 0001;
    tr_mlat_201_4.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_201_4.on_gnd_ = true;
    tr_mlat_201_4.x_ = 100.0;
    tr_mlat_201_4.y_ = 0.0;
    tr_mlat_201_4.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_mlat_201 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3 << tr_mlat_201_4;


    // MLAT track 202.
    Track trk_mlat_202(SystemType::Mlat, 202);

    TargetReport tr_mlat_202_1;
    tr_mlat_202_1.ds_id_.sac_ = 0;
    tr_mlat_202_1.ds_id_.sic_ = 107;
    tr_mlat_202_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_202_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:05.000Z"), Qt::ISODateWithMs);
    tr_mlat_202_1.trk_nb_ = 202;
    tr_mlat_202_1.mode_S_ = 0x000001;
    tr_mlat_202_1.mode_3A_ = 0001;
    tr_mlat_202_1.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_202_1.on_gnd_ = true;
    tr_mlat_202_1.x_ = 250.0;
    tr_mlat_202_1.y_ = 0.0;
    tr_mlat_202_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_202_2;
    tr_mlat_202_2.ds_id_.sac_ = 0;
    tr_mlat_202_2.ds_id_.sic_ = 107;
    tr_mlat_202_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_202_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:06.000Z"), Qt::ISODateWithMs);
    tr_mlat_202_2.trk_nb_ = 202;
    tr_mlat_202_2.mode_S_ = 0x000001;
    tr_mlat_202_2.mode_3A_ = 0001;
    tr_mlat_202_2.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_202_2.on_gnd_ = true;
    tr_mlat_202_2.x_ = 300.0;
    tr_mlat_202_2.y_ = 0.0;
    tr_mlat_202_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_202_3;
    tr_mlat_202_3.ds_id_.sac_ = 0;
    tr_mlat_202_3.ds_id_.sic_ = 107;
    tr_mlat_202_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_202_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:07.000Z"), Qt::ISODateWithMs);
    tr_mlat_202_3.trk_nb_ = 202;
    tr_mlat_202_3.mode_S_ = 0x000001;
    tr_mlat_202_3.mode_3A_ = 0001;
    tr_mlat_202_3.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_202_3.on_gnd_ = true;
    tr_mlat_202_3.x_ = 350.0;
    tr_mlat_202_3.y_ = 0.0;
    tr_mlat_202_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_mlat_202 << tr_mlat_202_1 << tr_mlat_202_2 << tr_mlat_202_3;


    // MLAT track 203.
    Track trk_mlat_203(SystemType::Mlat, 203);

    TargetReport tr_mlat_203_1;
    tr_mlat_203_1.ds_id_.sac_ = 0;
    tr_mlat_203_1.ds_id_.sic_ = 107;
    tr_mlat_203_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_203_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:10.000Z"), Qt::ISODateWithMs);
    tr_mlat_203_1.trk_nb_ = 203;
    tr_mlat_203_1.mode_S_ = 0x000001;
    tr_mlat_203_1.mode_3A_ = 0001;
    tr_mlat_203_1.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_203_1.on_gnd_ = true;
    tr_mlat_203_1.x_ = 500.0;
    tr_mlat_203_1.y_ = 0.0;
    tr_mlat_203_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_203_2;
    tr_mlat_203_2.ds_id_.sac_ = 0;
    tr_mlat_203_2.ds_id_.sic_ = 107;
    tr_mlat_203_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_203_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:11.000Z"), Qt::ISODateWithMs);
    tr_mlat_203_2.trk_nb_ = 203;
    tr_mlat_203_2.mode_S_ = 0x000001;
    tr_mlat_203_2.mode_3A_ = 0001;
    tr_mlat_203_2.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_203_2.on_gnd_ = true;
    tr_mlat_203_2.x_ = 550.0;
    tr_mlat_203_2.y_ = 0.0;
    tr_mlat_203_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_203_3;
    tr_mlat_203_3.ds_id_.sac_ = 0;
    tr_mlat_203_3.ds_id_.sic_ = 107;
    tr_mlat_203_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_203_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:12.000Z"), Qt::ISODateWithMs);
    tr_mlat_203_3.trk_nb_ = 203;
    tr_mlat_203_3.mode_S_ = 0x000001;
    tr_mlat_203_3.mode_3A_ = 0001;
    tr_mlat_203_3.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_203_3.on_gnd_ = true;
    tr_mlat_203_3.x_ = 600.0;
    tr_mlat_203_3.y_ = 0.0;
    tr_mlat_203_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_203_4;
    tr_mlat_203_4.ds_id_.sac_ = 0;
    tr_mlat_203_4.ds_id_.sic_ = 107;
    tr_mlat_203_4.sys_typ_ = SystemType::Mlat;
    tr_mlat_203_4.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:13.000Z"), Qt::ISODateWithMs);
    tr_mlat_203_4.trk_nb_ = 203;
    tr_mlat_203_4.mode_S_ = 0x000001;
    tr_mlat_203_4.mode_3A_ = 0001;
    tr_mlat_203_4.ident_ = QLatin1String("FOO1234 ");
    tr_mlat_203_4.on_gnd_ = true;
    tr_mlat_203_4.x_ = 650.0;
    tr_mlat_203_4.y_ = 0.0;
    tr_mlat_203_4.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_mlat_203 << tr_mlat_203_1 << tr_mlat_203_2 << tr_mlat_203_3 << tr_mlat_203_4;


    // SMR track 301.
    Track trk_smr_301(SystemType::Smr, 301);

    TargetReport tr_smr_301_1;
    tr_smr_301_1.ds_id_.sac_ = 0;
    tr_smr_301_1.ds_id_.sic_ = 7;
    tr_smr_301_1.sys_typ_ = SystemType::Smr;
    tr_smr_301_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T09:59:57.000Z"), Qt::ISODateWithMs);
    tr_smr_301_1.trk_nb_ = 301;
    tr_smr_301_1.on_gnd_ = true;
    tr_smr_301_1.x_ = -150.0;
    tr_smr_301_1.y_ = 0.0;
    tr_smr_301_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_2;
    tr_smr_301_2.ds_id_.sac_ = 0;
    tr_smr_301_2.ds_id_.sic_ = 7;
    tr_smr_301_2.sys_typ_ = SystemType::Smr;
    tr_smr_301_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T09:59:58.000Z"), Qt::ISODateWithMs);
    tr_smr_301_2.trk_nb_ = 301;
    tr_smr_301_2.on_gnd_ = true;
    tr_smr_301_2.x_ = -100.0;
    tr_smr_301_2.y_ = 0.0;
    tr_smr_301_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_301_3;
    tr_smr_301_3.ds_id_.sac_ = 0;
    tr_smr_301_3.ds_id_.sic_ = 7;
    tr_smr_301_3.sys_typ_ = SystemType::Smr;
    tr_smr_301_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T09:59:59.000Z"), Qt::ISODateWithMs);
    tr_smr_301_3.trk_nb_ = 301;
    tr_smr_301_3.on_gnd_ = true;
    tr_smr_301_3.x_ = -50.0;
    tr_smr_301_3.y_ = 0.0;
    tr_smr_301_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_smr_301 << tr_smr_301_1 << tr_smr_301_2 << tr_smr_301_3;


    // SMR track 302.
    Track trk_smr_302(SystemType::Smr, 302);

    TargetReport tr_smr_302_1;
    tr_smr_302_1.ds_id_.sac_ = 0;
    tr_smr_302_1.ds_id_.sic_ = 7;
    tr_smr_302_1.sys_typ_ = SystemType::Smr;
    tr_smr_302_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:02.000Z"), Qt::ISODateWithMs);
    tr_smr_302_1.trk_nb_ = 302;
    tr_smr_302_1.on_gnd_ = true;
    tr_smr_302_1.x_ = 100.0;
    tr_smr_302_1.y_ = 0.0;
    tr_smr_302_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_2;
    tr_smr_302_2.ds_id_.sac_ = 0;
    tr_smr_302_2.ds_id_.sic_ = 7;
    tr_smr_302_2.sys_typ_ = SystemType::Smr;
    tr_smr_302_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:03.000Z"), Qt::ISODateWithMs);
    tr_smr_302_2.trk_nb_ = 302;
    tr_smr_302_2.on_gnd_ = true;
    tr_smr_302_2.x_ = 150.0;
    tr_smr_302_2.y_ = 0.0;
    tr_smr_302_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_3;
    tr_smr_302_3.ds_id_.sac_ = 0;
    tr_smr_302_3.ds_id_.sic_ = 7;
    tr_smr_302_3.sys_typ_ = SystemType::Smr;
    tr_smr_302_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:04.000Z"), Qt::ISODateWithMs);
    tr_smr_302_3.trk_nb_ = 302;
    tr_smr_302_3.on_gnd_ = true;
    tr_smr_302_3.x_ = 200.0;
    tr_smr_302_3.y_ = 0.0;
    tr_smr_302_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_4;
    tr_smr_302_4.ds_id_.sac_ = 0;
    tr_smr_302_4.ds_id_.sic_ = 7;
    tr_smr_302_4.sys_typ_ = SystemType::Smr;
    tr_smr_302_4.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:05.000Z"), Qt::ISODateWithMs);
    tr_smr_302_4.trk_nb_ = 302;
    tr_smr_302_4.on_gnd_ = true;
    tr_smr_302_4.x_ = 250.0;
    tr_smr_302_4.y_ = 0.0;
    tr_smr_302_4.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_5;
    tr_smr_302_5.ds_id_.sac_ = 0;
    tr_smr_302_5.ds_id_.sic_ = 7;
    tr_smr_302_5.sys_typ_ = SystemType::Smr;
    tr_smr_302_5.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:06.000Z"), Qt::ISODateWithMs);
    tr_smr_302_5.trk_nb_ = 302;
    tr_smr_302_5.on_gnd_ = true;
    tr_smr_302_5.x_ = 300.0;
    tr_smr_302_5.y_ = 0.0;
    tr_smr_302_5.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_6;
    tr_smr_302_6.ds_id_.sac_ = 0;
    tr_smr_302_6.ds_id_.sic_ = 7;
    tr_smr_302_6.sys_typ_ = SystemType::Smr;
    tr_smr_302_6.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:07.000Z"), Qt::ISODateWithMs);
    tr_smr_302_6.trk_nb_ = 302;
    tr_smr_302_6.on_gnd_ = true;
    tr_smr_302_6.x_ = 350.0;
    tr_smr_302_6.y_ = 0.0;
    tr_smr_302_6.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_7;
    tr_smr_302_7.ds_id_.sac_ = 0;
    tr_smr_302_7.ds_id_.sic_ = 7;
    tr_smr_302_7.sys_typ_ = SystemType::Smr;
    tr_smr_302_7.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:08.000Z"), Qt::ISODateWithMs);
    tr_smr_302_7.trk_nb_ = 302;
    tr_smr_302_7.on_gnd_ = true;
    tr_smr_302_7.x_ = 400.0;
    tr_smr_302_7.y_ = 0.0;
    tr_smr_302_7.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_8;
    tr_smr_302_8.ds_id_.sac_ = 0;
    tr_smr_302_8.ds_id_.sic_ = 7;
    tr_smr_302_8.sys_typ_ = SystemType::Smr;
    tr_smr_302_8.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:09.000Z"), Qt::ISODateWithMs);
    tr_smr_302_8.trk_nb_ = 302;
    tr_smr_302_8.on_gnd_ = true;
    tr_smr_302_8.x_ = 450.0;
    tr_smr_302_8.y_ = 0.0;
    tr_smr_302_8.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_302_9;
    tr_smr_302_9.ds_id_.sac_ = 0;
    tr_smr_302_9.ds_id_.sic_ = 7;
    tr_smr_302_9.sys_typ_ = SystemType::Smr;
    tr_smr_302_9.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:10.000Z"), Qt::ISODateWithMs);
    tr_smr_302_9.trk_nb_ = 302;
    tr_smr_302_9.on_gnd_ = true;
    tr_smr_302_9.x_ = 500.0;
    tr_smr_302_9.y_ = 0.0;
    tr_smr_302_9.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_smr_302 << tr_smr_302_1 << tr_smr_302_2 << tr_smr_302_3
                << tr_smr_302_4 << tr_smr_302_5 << tr_smr_302_6
                << tr_smr_302_7 << tr_smr_302_8 << tr_smr_302_9;


    // SMR track 303.
    Track trk_smr_303(SystemType::Smr, 303);

    TargetReport tr_smr_303_1;
    tr_smr_303_1.ds_id_.sac_ = 0;
    tr_smr_303_1.ds_id_.sic_ = 7;
    tr_smr_303_1.sys_typ_ = SystemType::Smr;
    tr_smr_303_1.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:13.000Z"), Qt::ISODateWithMs);
    tr_smr_303_1.trk_nb_ = 303;
    tr_smr_303_1.on_gnd_ = true;
    tr_smr_303_1.x_ = 650.0;
    tr_smr_303_1.y_ = 0.0;
    tr_smr_303_1.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_303_2;
    tr_smr_303_2.ds_id_.sac_ = 0;
    tr_smr_303_2.ds_id_.sic_ = 7;
    tr_smr_303_2.sys_typ_ = SystemType::Smr;
    tr_smr_303_2.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:14.000Z"), Qt::ISODateWithMs);
    tr_smr_303_2.trk_nb_ = 303;
    tr_smr_303_2.on_gnd_ = true;
    tr_smr_303_2.x_ = 700.0;
    tr_smr_303_2.y_ = 0.0;
    tr_smr_303_2.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_smr_303_3;
    tr_smr_303_3.ds_id_.sac_ = 0;
    tr_smr_303_3.ds_id_.sic_ = 7;
    tr_smr_303_3.sys_typ_ = SystemType::Smr;
    tr_smr_303_3.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T10:00:15.000Z"), Qt::ISODateWithMs);
    tr_smr_303_3.trk_nb_ = 303;
    tr_smr_303_3.on_gnd_ = true;
    tr_smr_303_3.x_ = 750.0;
    tr_smr_303_3.y_ = 0.0;
    tr_smr_303_3.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    trk_smr_303 << tr_smr_303_1 << tr_smr_303_2 << tr_smr_303_3;


    QVector<Track> tracksIn;
    tracksIn << trk_adsb_101 << trk_adsb_102
             << trk_mlat_201 << trk_mlat_202 << trk_mlat_203
             << trk_smr_301 << trk_smr_302 << trk_smr_303;

    TrackCollectionSet set_000001(0x000001, SystemType::Adsb);
    set_000001 << trk_adsb_101 << trk_adsb_102;
    set_000001 << trk_mlat_201 << trk_mlat_202 << trk_mlat_203;
    set_000001 << trk_smr_302;

    set_000001.addMatch(trk_adsb_101, trk_mlat_201);
    set_000001.addMatch(trk_adsb_102, trk_mlat_203);
    set_000001.addMatch(trk_adsb_101, trk_smr_302);
    set_000001.addMatch(trk_adsb_102, trk_smr_302);

    QVector<TrackCollectionSet> setsOut;
    setsOut << set_000001;

    QTest::newRow("FOO") << tracksIn << setsOut;
}

void TrackAssociatorTest::test()
{
    QFETCH(QVector<Track>, tracksIn);
    QFETCH(QVector<TrackCollectionSet>, setsOut);

    TrackAssociator trackAssoc;

    // Right after instantiation there should not be any pending data in the output.
    QVERIFY(!trackAssoc.hasPendingData());

    // Attempting to retrieve data from the output at this stage should give
    // an empty optional value.
    QVERIFY(!trackAssoc.takeData().has_value());

    // Add input tracks.
    for (const Track &trk : tracksIn)
    {
        trackAssoc.addData(trk);

        // There still should not be any pending data in the output at this stage.
        QVERIFY(!trackAssoc.hasPendingData());

        // Attempting to retrieve data from the output at this stage should give
        // an empty optional value.
        QVERIFY(!trackAssoc.takeData().has_value());
    }

    // Run the association procedure and save the number of unique targets
    // that have been successfully associated.
    int n = trackAssoc.run();

    // Once the association has been performed the number of associated
    // targets should match the expected number of sets in the output.
    QCOMPARE(n, setsOut.size());

    // Now there should be some pending data in the output.
    QVERIFY(trackAssoc.hasPendingData());

    // Extract the output sets.
    QVector<TrackCollectionSet> sets;
    while (trackAssoc.hasPendingData())
    {
        // The optional values retrieved from the output should have a value now.
        std::optional opt = trackAssoc.takeData();
        QVERIFY(opt.has_value());

        // Extract set from optional value and save it.
        sets << opt.value();
    }

    // The number of output sets should match the expected value.
    QCOMPARE(sets.size(), setsOut.size());

    // After all sets have been extracted there should not be any pending data
    // left in the output.
    QVERIFY(!trackAssoc.hasPendingData());

    // Verify that each output set matches with the expected value.
    for (int i = 0; i < setsOut.size(); ++i)
    {
        QCOMPARE(sets.at(i), setsOut.at(i));
    }
}

QTEST_GUILESS_MAIN(TrackAssociatorTest);
#include "trackassociatortest.moc"
