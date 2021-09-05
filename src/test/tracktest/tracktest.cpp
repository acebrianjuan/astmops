/*!
 * \file tracktest.cpp
 * \brief Implements unit tests for the track-related classes.
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

#include "track.h"
#include "config.h"
#include <QObject>
#include <QtTest>
#include <algorithm>

using namespace Literals;

class TrackTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testTrack();
    void testTrackCollection();
    void testTrackCollectionSet();

    // TODO: Add tests for intersect(), resample() and average() functions.
};

void TrackTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-tracktest"));

    Settings settings;
    settings.clear();

    settings.beginGroup(QLatin1String("Asterix"));
    settings.setValue(QLatin1String("Date"), QLatin1String("2020-05-05"));
    settings.setValue(QLatin1String("SmrSic"), 7);
    settings.setValue(QLatin1String("MlatSic"), 107);
    settings.setValue(QLatin1String("AdsbSic"), 219);
    settings.endGroup();
}

void TrackTest::testTrack()
{
    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.ds_id_.sac_ = 0;
    tr_adsb_101_1.ds_id_.sic_ = 219;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T09:59:55.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;
    tr_adsb_101_1.mode_s_ = 0x000001;
    tr_adsb_101_1.mode_3a_ = 0001;
    tr_adsb_101_1.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_1.on_gnd_ = true;
    tr_adsb_101_1.x_ = -50.0;
    tr_adsb_101_1.y_ = -25.0;
    tr_adsb_101_1.z_ = 0.0;
    tr_adsb_101_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.ds_id_.sac_ = 0;
    tr_adsb_101_2.ds_id_.sic_ = 219;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;
    //tr_adsb_101_2.mode_S_ = 0x000001;
    //tr_adsb_101_2.mode_3A_ = 0001;
    tr_adsb_101_2.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_2.on_gnd_ = true;
    tr_adsb_101_2.x_ = 0.0;
    tr_adsb_101_2.y_ = 0.0;
    tr_adsb_101_2.z_ = 0.0;
    tr_adsb_101_2.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.ds_id_.sac_ = 0;
    tr_adsb_101_3.ds_id_.sic_ = 219;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;
    //tr_adsb_101_3.mode_S_ = 0x000001;
    //tr_adsb_101_3.mode_3A_ = 0001;
    tr_adsb_101_3.ident_ = QLatin1String("FOO1234 ");
    tr_adsb_101_3.on_gnd_ = true;
    tr_adsb_101_3.x_ = 50.0;
    tr_adsb_101_3.y_ = 25.0;
    tr_adsb_101_3.z_ = 0.0;
    tr_adsb_101_3.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_adsb_102_1;
    tr_adsb_102_1.ds_id_.sac_ = 0;
    tr_adsb_102_1.ds_id_.sic_ = 219;
    tr_adsb_102_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_102_1.trk_nb_ = 102;
    tr_adsb_102_1.mode_s_ = 0x000002;
    tr_adsb_102_1.mode_3a_ = 0002;
    tr_adsb_102_1.ident_ = QLatin1String("FOO5678 ");
    tr_adsb_102_1.on_gnd_ = true;
    tr_adsb_102_1.x_ = 0.0;
    tr_adsb_102_1.y_ = 0.0;
    tr_adsb_102_1.z_ = 0.0;
    tr_adsb_102_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.ds_id_.sac_ = 0;
    tr_mlat_201_1.ds_id_.sic_ = 107;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;
    tr_mlat_201_1.mode_s_ = 0x000002;
    tr_mlat_201_1.mode_3a_ = 0002;
    tr_mlat_201_1.ident_ = QLatin1String("FOO5678 ");
    tr_mlat_201_1.on_gnd_ = true;
    tr_mlat_201_1.x_ = 0.0;
    tr_mlat_201_1.y_ = 0.0;
    tr_mlat_201_1.z_ = 0.0;
    tr_mlat_201_1.narea_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);


    /* -------------------------------------------------------------------- */
    // Test default-constructed track.
    /* -------------------------------------------------------------------- */

    Track trk_dflt;

    // For a default-constructed track the system type must be unknown and the
    // track number set to 0.
    QCOMPARE(trk_dflt.system_type(), SystemType::Unknown);
    QCOMPARE(trk_dflt.track_number(), static_cast<TrackNum>(0));

    // A default-constructed track must be empty.
    QVERIFY(trk_dflt.isEmpty() == true);
    QCOMPARE(trk_dflt.size(), 0);

    // This condition must always hold true for all tracks.
    QCOMPARE(trk_dflt.begin(), trk_dflt.rdata().begin());
    QCOMPARE(trk_dflt.end(), trk_dflt.rdata().end());

    // The begin and end timestamps must be in the default invalid state.
    QCOMPARE(trk_dflt.beginTimestamp(), QDateTime());
    QCOMPARE(trk_dflt.endTimestamp(), QDateTime());

    QVERIFY(trk_dflt.timestamps().isEmpty() == true);

    QVERIFY(qIsNaN(trk_dflt.duration()));

    QVERIFY(trk_dflt.coversTimestamp(QDateTime()) == false);

    // XYZ bounds must be in the default NaN-valued state.
    QVERIFY(qIsNaN(trk_dflt.x_bounds().first));
    QVERIFY(qIsNaN(trk_dflt.x_bounds().second));
    QVERIFY(qIsNaN(trk_dflt.y_bounds().first));
    QVERIFY(qIsNaN(trk_dflt.y_bounds().second));
    QVERIFY(qIsNaN(trk_dflt.z_bounds().first));
    QVERIFY(qIsNaN(trk_dflt.z_bounds().second));

    // ModeS address must have no value assigned.
    QVERIFY(trk_dflt.mode_s().has_value() == false);


    // INVALID INSERTION ATTEMPT:
    // Attempting to add target reports to a default-constructed track should
    // have no effect so all the previous checks should still hold.
    trk_dflt << tr_adsb_101_1;

    QCOMPARE(trk_dflt.system_type(), SystemType::Unknown);
    QCOMPARE(trk_dflt.track_number(), static_cast<TrackNum>(0));

    QVERIFY(trk_dflt.isEmpty() == true);
    QCOMPARE(trk_dflt.size(), 0);

    QCOMPARE(trk_dflt.begin(), trk_dflt.rdata().begin());
    QCOMPARE(trk_dflt.end(), trk_dflt.rdata().end());

    QCOMPARE(trk_dflt.beginTimestamp(), QDateTime());
    QCOMPARE(trk_dflt.endTimestamp(), QDateTime());

    QVERIFY(trk_dflt.timestamps().isEmpty() == true);

    QVERIFY(qIsNaN(trk_dflt.duration()));

    QVERIFY(trk_dflt.coversTimestamp(QDateTime()) == false);

    QVERIFY(qIsNaN(trk_dflt.x_bounds().first));
    QVERIFY(qIsNaN(trk_dflt.x_bounds().second));
    QVERIFY(qIsNaN(trk_dflt.y_bounds().first));
    QVERIFY(qIsNaN(trk_dflt.y_bounds().second));
    QVERIFY(qIsNaN(trk_dflt.z_bounds().first));
    QVERIFY(qIsNaN(trk_dflt.z_bounds().second));

    QVERIFY(trk_dflt.mode_s().has_value() == false);


    /* -------------------------------------------------------------------- */
    // Test track constructed with system type and track number.
    /* -------------------------------------------------------------------- */

    // Initialize an empty ADS-B track with track number 101.
    Track trk_adsb_101(SystemType::Adsb, 101);

    // Verify that system type and track number match with the values provided
    // to the constructor. These properties must remain constant during the
    // lifetime of the track.
    QCOMPARE(trk_adsb_101.system_type(), SystemType::Adsb);
    QCOMPARE(trk_adsb_101.track_number(), static_cast<TrackNum>(101));

    // No target reports have been added yet so the track must be empty.
    QVERIFY(trk_adsb_101.isEmpty() == true);
    QCOMPARE(trk_adsb_101.size(), 0);

    // This condition must always hold true for all tracks.
    QCOMPARE(trk_adsb_101.begin(), trk_adsb_101.rdata().begin());
    QCOMPARE(trk_adsb_101.end(), trk_adsb_101.rdata().end());

    // No target reports have been added yet so begin and end datetimes must
    // be invalid.
    QCOMPARE(trk_adsb_101.beginTimestamp(), QDateTime());
    QCOMPARE(trk_adsb_101.endTimestamp(), QDateTime());

    // No target reports have been added yet so timestamps must be empty.
    QVERIFY(trk_adsb_101.timestamps().isEmpty() == true);

    // No target reports have been added yet so duration must be NaN-valued.
    QVERIFY(qIsNaN(trk_adsb_101.duration()));

    QVERIFY(trk_adsb_101.coversTimestamp(QDateTime()) == false);

    // No target reports have been added yet so XYZ bounds must be in the
    // default NaN-valued state.
    QVERIFY(qIsNaN(trk_adsb_101.x_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.x_bounds().second));
    QVERIFY(qIsNaN(trk_adsb_101.y_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.y_bounds().second));
    QVERIFY(qIsNaN(trk_adsb_101.z_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.z_bounds().second));

    // No target reports with ModeS address have been added yet so ModeS
    // address must have no value assigned.
    QVERIFY(trk_adsb_101.mode_s().has_value() == false);


    // INVALID INSERTION ATTEMPT #1: MLAT target report.
    // Attempting to add target reports of a different system type to that of
    // the track should have no effect so all the previous checks should still
    // hold.
    trk_adsb_101 << tr_mlat_201_1;

    QCOMPARE(trk_adsb_101.system_type(), SystemType::Adsb);
    QCOMPARE(trk_adsb_101.track_number(), static_cast<TrackNum>(101));

    QVERIFY(trk_adsb_101.isEmpty() == true);
    QCOMPARE(trk_adsb_101.size(), 0);

    QCOMPARE(trk_adsb_101.begin(), trk_adsb_101.rdata().begin());
    QCOMPARE(trk_adsb_101.end(), trk_adsb_101.rdata().end());

    QCOMPARE(trk_adsb_101.beginTimestamp(), QDateTime());
    QCOMPARE(trk_adsb_101.endTimestamp(), QDateTime());

    QVERIFY(trk_adsb_101.timestamps().isEmpty() == true);

    QVERIFY(qIsNaN(trk_adsb_101.duration()));

    QVERIFY(trk_adsb_101.coversTimestamp(QDateTime()) == false);

    QVERIFY(qIsNaN(trk_adsb_101.x_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.x_bounds().second));
    QVERIFY(qIsNaN(trk_adsb_101.y_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.y_bounds().second));
    QVERIFY(qIsNaN(trk_adsb_101.z_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.z_bounds().second));

    QVERIFY(trk_adsb_101.mode_s().has_value() == false);


    // INVALID INSERTION ATTEMPT #2: ADS-B target report with track number 102.
    // Attempting to add target reports of a different track number to that
    // of the track should have no effect so all the previous checks should
    // still hold.
    trk_adsb_101 << tr_adsb_102_1;

    QCOMPARE(trk_adsb_101.system_type(), SystemType::Adsb);
    QCOMPARE(trk_adsb_101.track_number(), static_cast<TrackNum>(101));

    QVERIFY(trk_adsb_101.isEmpty() == true);
    QCOMPARE(trk_adsb_101.size(), 0);

    QCOMPARE(trk_adsb_101.begin(), trk_adsb_101.rdata().begin());
    QCOMPARE(trk_adsb_101.end(), trk_adsb_101.rdata().end());

    QCOMPARE(trk_adsb_101.beginTimestamp(), QDateTime());
    QCOMPARE(trk_adsb_101.endTimestamp(), QDateTime());

    QVERIFY(trk_adsb_101.timestamps().isEmpty() == true);

    QVERIFY(qIsNaN(trk_adsb_101.duration()));

    QVERIFY(trk_adsb_101.coversTimestamp(QDateTime()) == false);

    QVERIFY(qIsNaN(trk_adsb_101.x_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.x_bounds().second));
    QVERIFY(qIsNaN(trk_adsb_101.y_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.y_bounds().second));
    QVERIFY(qIsNaN(trk_adsb_101.z_bounds().first));
    QVERIFY(qIsNaN(trk_adsb_101.z_bounds().second));

    QVERIFY(trk_adsb_101.mode_s().has_value() == false);


    // VALID INSERTION #1: ADS-B target report with track number 101 recorded
    // at 10:00:00 UTC.
    // The track must accept all target reports of same system type and track
    // number.
    trk_adsb_101 << tr_adsb_101_2;

    QCOMPARE(trk_adsb_101.system_type(), SystemType::Adsb);
    QCOMPARE(trk_adsb_101.track_number(), static_cast<TrackNum>(101));

    // The track must contain one target report now.
    QVERIFY(trk_adsb_101.isEmpty() == false);
    QCOMPARE(trk_adsb_101.size(), 1);

    QCOMPARE(trk_adsb_101.begin(), trk_adsb_101.rdata().begin());
    QCOMPARE(trk_adsb_101.end(), trk_adsb_101.rdata().end());

    // Since the track only contains one target report, the begin and end
    // timestamps must be equal to the TOD of said target report.
    QCOMPARE(trk_adsb_101.beginTimestamp(), "2020-05-05T10:00:00.000Z"_ts);
    QCOMPARE(trk_adsb_101.endTimestamp(), "2020-05-05T10:00:00.000Z"_ts);

    // The only timestamp must be the TOD of the only target report in the track.
    QCOMPARE(trk_adsb_101.timestamps(),
        QVector<QDateTime>({tr_adsb_101_2.tod_}));

    // The track contains only one target report so the duration must be 0.
    QCOMPARE(trk_adsb_101.duration(), 0.0);

    // Even if the track has no duration it must at least cover the timestamp
    // of the only target report in the track.
    QVERIFY(trk_adsb_101.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == true);

    // Since the track only contains one target report, the XYZ bounds must be
    // equal to the position of said target report.
    QCOMPARE(trk_adsb_101.x_bounds().first, 0.0);
    QCOMPARE(trk_adsb_101.x_bounds().second, 0.0);
    QCOMPARE(trk_adsb_101.y_bounds().first, 0.0);
    QCOMPARE(trk_adsb_101.y_bounds().second, 0.0);
    QCOMPARE(trk_adsb_101.z_bounds().first, 0.0);
    QCOMPARE(trk_adsb_101.z_bounds().second, 0.0);

    // No target reports with ModeS address have been added yet so ModeS
    // address must have no value assigned.
    QVERIFY(trk_adsb_101.mode_s().has_value() == false);


    // VALID INSERTION #2: A second ADS-B target report with track number 101
    // recorded at 10:00:05 UTC (5 seconds AFTER the first one).
    // The track must accept all target reports of same system type and track
    // number.
    trk_adsb_101 << tr_adsb_101_3;

    QCOMPARE(trk_adsb_101.system_type(), SystemType::Adsb);
    QCOMPARE(trk_adsb_101.track_number(), static_cast<TrackNum>(101));

    // The track must contain two target reports now.
    QVERIFY(trk_adsb_101.isEmpty() == false);
    QCOMPARE(trk_adsb_101.size(), 2);

    QCOMPARE(trk_adsb_101.begin(), trk_adsb_101.rdata().begin());
    QCOMPARE(trk_adsb_101.end(), trk_adsb_101.rdata().end());

    // Since the track now contains more than one target report, the begin and
    // end timestamps must be equal to the TODs of the earliest and latest
    // target reports respectively.
    QCOMPARE(trk_adsb_101.beginTimestamp(), "2020-05-05T10:00:00.000Z"_ts);
    QCOMPARE(trk_adsb_101.endTimestamp(), "2020-05-05T10:00:05.000Z"_ts);

    // The timestamps must be the TODs of the two target reports in the track.
    QCOMPARE(trk_adsb_101.timestamps(),
        QVector<QDateTime>({tr_adsb_101_2.tod_, tr_adsb_101_3.tod_}));

    // The duration must be equal to the time span in seconds between the
    // earliest and latest target reports in the track.
    QCOMPARE(trk_adsb_101.duration(), 5.0);

    QDateTime dt = trk_adsb_101.beginTimestamp();
    while (dt <= trk_adsb_101.endTimestamp())
    {
        QVERIFY(trk_adsb_101.coversTimestamp(dt) == true);
        dt = dt.addSecs(1);
    }
    QVERIFY(trk_adsb_101.coversTimestamp(dt) == false);

    // The upper XY bounds must be updated with the position of the newly
    // inserted target report.
    QCOMPARE(trk_adsb_101.x_bounds().first, 0.0);
    QCOMPARE(trk_adsb_101.x_bounds().second, 50.0);
    QCOMPARE(trk_adsb_101.y_bounds().first, 0.0);
    QCOMPARE(trk_adsb_101.y_bounds().second, 25.0);
    QCOMPARE(trk_adsb_101.z_bounds().first, 0.0);
    QCOMPARE(trk_adsb_101.z_bounds().second, 0.0);

    // No target reports with ModeS address have been added yet so ModeS
    // address must have no value assigned.
    QVERIFY(trk_adsb_101.mode_s().has_value() == false);


    // VALID INSERTION #3: A third ADS-B target report with track number 101
    // recorded at 09:59:55 UTC (5 seconds BEFORE the first one) which
    // contains ModeS address 0x000001.
    // A target report of same SystemType and track number as the track should
    // be added to the track.
    trk_adsb_101 << tr_adsb_101_1;

    QCOMPARE(trk_adsb_101.system_type(), SystemType::Adsb);
    QCOMPARE(trk_adsb_101.track_number(), static_cast<TrackNum>(101));

    // The track must contain three target reports now.
    QVERIFY(trk_adsb_101.isEmpty() == false);
    QCOMPARE(trk_adsb_101.size(), 3);

    QCOMPARE(trk_adsb_101.begin(), trk_adsb_101.rdata().begin());
    QCOMPARE(trk_adsb_101.end(), trk_adsb_101.rdata().end());

    // The begin timestamp must be equal to the TOD of the newly inserted
    // target report since it precedes all the previously inserted target
    // reports. The end timestamp must remain as it was.
    QCOMPARE(trk_adsb_101.beginTimestamp(), "2020-05-05T09:59:55.000Z"_ts);
    QCOMPARE(trk_adsb_101.endTimestamp(), "2020-05-05T10:00:05.000Z"_ts);

    // The timestamps must be the TODs of the three target reports in the track.
    QCOMPARE(trk_adsb_101.timestamps(),
        QVector<QDateTime>({tr_adsb_101_1.tod_, tr_adsb_101_2.tod_, tr_adsb_101_3.tod_}));

    // With the newly added target report the duration must be increased by
    // 5 seconds.
    QCOMPARE(trk_adsb_101.duration(), 10.0);

    dt = trk_adsb_101.beginTimestamp();
    while (dt <= trk_adsb_101.endTimestamp())
    {
        QVERIFY(trk_adsb_101.coversTimestamp(dt) == true);
        dt = dt.addSecs(1);
    }
    QVERIFY(trk_adsb_101.coversTimestamp(dt) == false);

    // The lower XY bounds must be updated with the position of the newly
    // inserted target report.
    QCOMPARE(trk_adsb_101.x_bounds().first, -50.0);
    QCOMPARE(trk_adsb_101.x_bounds().second, 50.0);
    QCOMPARE(trk_adsb_101.y_bounds().first, -25.0);
    QCOMPARE(trk_adsb_101.y_bounds().second, 25.0);
    QCOMPARE(trk_adsb_101.z_bounds().first, 0.0);
    QCOMPARE(trk_adsb_101.z_bounds().second, 0.0);

    // The track must have the taken the ModeS address of the newly inserted
    // target report.
    QVERIFY(trk_adsb_101.mode_s().has_value() == true);
    QCOMPARE(trk_adsb_101.mode_s().value(), static_cast<quint32>(0x000001));


    /* -------------------------------------------------------------------- */
    // Test track constructed with system type, track number and vector of
    // target reports.
    /* -------------------------------------------------------------------- */

    QVector<TargetReport> vec({tr_adsb_101_1, tr_adsb_101_2, tr_adsb_101_3});
    QCOMPARE(Track(SystemType::Adsb, 101, vec), trk_adsb_101);
}

void TrackTest::testTrackCollection()
{
    Track trk_adsb_101(0x000001, SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T09:59:55.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T09:59:57.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T09:59:59.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3;


    Track trk_adsb_102(SystemType::Adsb, 102);

    TargetReport tr_adsb_102_1;
    tr_adsb_102_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_102_1.trk_nb_ = 102;

    TargetReport tr_adsb_102_2;
    tr_adsb_102_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_2.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_102_2.trk_nb_ = 102;

    TargetReport tr_adsb_102_3;
    tr_adsb_102_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_3.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_102_3.trk_nb_ = 102;

    trk_adsb_102 << tr_adsb_102_1 << tr_adsb_102_2 << tr_adsb_102_3;


    Track trk_adsb_103(SystemType::Adsb, 103);

    TargetReport tr_adsb_103_1;
    tr_adsb_103_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_103_1.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    tr_adsb_103_1.trk_nb_ = 103;

    TargetReport tr_adsb_103_2;
    tr_adsb_103_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_103_2.tod_ = "2020-05-05T10:00:07.000Z"_ts;
    tr_adsb_103_2.trk_nb_ = 103;

    TargetReport tr_adsb_103_3;
    tr_adsb_103_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_103_3.tod_ = "2020-05-05T10:00:09.000Z"_ts;
    tr_adsb_103_3.trk_nb_ = 103;

    trk_adsb_103 << tr_adsb_103_1 << tr_adsb_103_2 << tr_adsb_103_3;


    /* -------------------------------------------------------------------- */
    // Test default-constructed track collection.
    /* -------------------------------------------------------------------- */

    TrackCollection col_dflt;

    QCOMPARE(col_dflt.system_type(), SystemType::Unknown);
    QCOMPARE(col_dflt.track_numbers(), QSet<TrackNum>());

    QVERIFY(col_dflt.isEmpty() == true);
    QCOMPARE(col_dflt.size(), 0);

    QCOMPARE(col_dflt.beginTimestamp(), QDateTime());
    QCOMPARE(col_dflt.endTimestamp(), QDateTime());

    QVERIFY(col_dflt.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == false);

    QCOMPARE(col_dflt.tracks(), QVector<Track>());

    QVERIFY(col_dflt.mode_s().has_value() == false);


    // INVALID INSERTION ATTEMPT:
    // Attempting to add tracks to a default-constructed track collection
    // should have no effect so all the previous checks should still hold.
    col_dflt << Track(SystemType::Smr, 100);

    QCOMPARE(col_dflt.system_type(), SystemType::Unknown);
    QCOMPARE(col_dflt.track_numbers(), QSet<TrackNum>());

    QVERIFY(col_dflt.isEmpty() == true);
    QCOMPARE(col_dflt.size(), 0);

    QCOMPARE(col_dflt.beginTimestamp(), QDateTime());
    QCOMPARE(col_dflt.endTimestamp(), QDateTime());

    QVERIFY(col_dflt.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == false);

    QCOMPARE(col_dflt.tracks(), QVector<Track>());

    QVERIFY(col_dflt.mode_s().has_value() == false);


    /* -------------------------------------------------------------------- */
    // Test track collection constructed with system type.
    /* -------------------------------------------------------------------- */

    TrackCollection col_adsb(SystemType::Adsb);

    QCOMPARE(col_adsb.system_type(), SystemType::Adsb);
    QCOMPARE(col_adsb.track_numbers(), QSet<TrackNum>());

    QVERIFY(col_adsb.isEmpty() == true);
    QCOMPARE(col_adsb.size(), 0);

    QCOMPARE(col_adsb.beginTimestamp(), QDateTime());
    QCOMPARE(col_adsb.endTimestamp(), QDateTime());

    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:50.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:55.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:05.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:10.000Z"_ts) == false);

    QCOMPARE(col_adsb.tracks(), QVector<Track>());

    QVERIFY(col_adsb.mode_s().has_value() == false);


    // INVALID INSERTION ATTEMPT:
    // Attempting to add tracks of a different system type to that of the
    // collection should have no effect so all the previous checks should
    // still hold.
    col_adsb << Track(SystemType::Smr, 100);

    QCOMPARE(col_adsb.system_type(), SystemType::Adsb);
    QCOMPARE(col_adsb.track_numbers(), QSet<TrackNum>());

    QVERIFY(col_adsb.isEmpty() == true);
    QCOMPARE(col_adsb.size(), 0);

    QCOMPARE(col_adsb.beginTimestamp(), QDateTime());
    QCOMPARE(col_adsb.endTimestamp(), QDateTime());

    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:50.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:55.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:05.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:10.000Z"_ts) == false);

    QCOMPARE(col_adsb.tracks(), QVector<Track>());

    QVERIFY(col_adsb.mode_s().has_value() == false);


    // INVALID INSERTION ATTEMPT:
    // Attempting to add empty tracks to a collection should have no effect
    // so all the previous checks should still hold.
    col_adsb << Track(SystemType::Adsb, 100);

    QCOMPARE(col_adsb.system_type(), SystemType::Adsb);
    QCOMPARE(col_adsb.track_numbers(), QSet<TrackNum>());

    QVERIFY(col_adsb.isEmpty() == true);
    QCOMPARE(col_adsb.size(), 0);

    QCOMPARE(col_adsb.beginTimestamp(), QDateTime());
    QCOMPARE(col_adsb.endTimestamp(), QDateTime());

    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:50.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:55.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:05.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:10.000Z"_ts) == false);

    QCOMPARE(col_adsb.tracks(), QVector<Track>());

    QVERIFY(col_adsb.mode_s().has_value() == false);


    // VALID INSERTION #1: ADS-B track with track number 102 beginning at
    // 10:00:00 UTC.
    // The collection must accept all tracks of same system type.
    col_adsb << trk_adsb_102;

    QCOMPARE(col_adsb.system_type(), SystemType::Adsb);
    QCOMPARE(col_adsb.track_numbers(), QSet<TrackNum>({102}));

    QVERIFY(col_adsb.isEmpty() == false);
    QCOMPARE(col_adsb.size(), 1);

    QCOMPARE(col_adsb.beginTimestamp(), "2020-05-05T10:00:00.000Z"_ts);
    QCOMPARE(col_adsb.endTimestamp(), "2020-05-05T10:00:04.000Z"_ts);

    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:50.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:55.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == true);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:05.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:10.000Z"_ts) == false);

    QCOMPARE(col_adsb.tracks(), QVector<Track>({trk_adsb_102}));

    QVERIFY(col_adsb.mode_s().has_value() == false);


    // VALID INSERTION #2: ADS-B track with track number 103 beginning at
    // 10:00:05 UTC.
    // The collection must accept all tracks of same system type.
    col_adsb << trk_adsb_103;

    QCOMPARE(col_adsb.system_type(), SystemType::Adsb);
    QCOMPARE(col_adsb.track_numbers(), QSet<TrackNum>({102, 103}));

    QVERIFY(col_adsb.isEmpty() == false);
    QCOMPARE(col_adsb.size(), 2);

    QCOMPARE(col_adsb.beginTimestamp(), "2020-05-05T10:00:00.000Z"_ts);
    QCOMPARE(col_adsb.endTimestamp(), "2020-05-05T10:00:09.000Z"_ts);

    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:50.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:55.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == true);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:05.000Z"_ts) == true);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:10.000Z"_ts) == false);

    QCOMPARE(col_adsb.tracks(), QVector<Track>({trk_adsb_102, trk_adsb_103}));

    QVERIFY(col_adsb.mode_s().has_value() == false);


    // VALID INSERTION #3: A third ADS-B track with track number 101 beginning
    // at 09:59:55 UTC (5 seconds BEFORE track 102) which contains ModeS
    // address 0x000001.
    // The collection must accept all tracks of same system type.
    col_adsb << trk_adsb_101;

    QCOMPARE(col_adsb.system_type(), SystemType::Adsb);
    QCOMPARE(col_adsb.track_numbers(), QSet<TrackNum>({101, 102, 103}));

    QVERIFY(col_adsb.isEmpty() == false);
    QCOMPARE(col_adsb.size(), 3);

    QCOMPARE(col_adsb.beginTimestamp(), "2020-05-05T09:59:55.000Z"_ts);
    QCOMPARE(col_adsb.endTimestamp(), "2020-05-05T10:00:09.000Z"_ts);

    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:50.000Z"_ts) == false);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T09:59:55.000Z"_ts) == true);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:00.000Z"_ts) == true);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:05.000Z"_ts) == true);
    QVERIFY(col_adsb.coversTimestamp("2020-05-05T10:00:10.000Z"_ts) == false);

    QCOMPARE(col_adsb.tracks(), QVector<Track>({trk_adsb_101, trk_adsb_102, trk_adsb_103}));

    QVERIFY(col_adsb.mode_s().has_value() == true);
    QCOMPARE(col_adsb.mode_s().value(), static_cast<quint32>(0x000001));


    /* -------------------------------------------------------------------- */
    // Test track collection constructed with system type and vector of tracks.
    /* -------------------------------------------------------------------- */

    QVector<Track> vec({trk_adsb_101, trk_adsb_102, trk_adsb_103});
    QCOMPARE(TrackCollection(SystemType::Adsb, vec), col_adsb);
}

void TrackTest::testTrackCollectionSet()
{
    Track trk_adsb_101(0x000001, SystemType::Adsb, 101);

    TargetReport tr_adsb_101_1;
    tr_adsb_101_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_1.tod_ = "2020-05-05T09:59:55.000Z"_ts;
    tr_adsb_101_1.trk_nb_ = 101;

    TargetReport tr_adsb_101_2;
    tr_adsb_101_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_2.tod_ = "2020-05-05T09:59:57.000Z"_ts;
    tr_adsb_101_2.trk_nb_ = 101;

    TargetReport tr_adsb_101_3;
    tr_adsb_101_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_101_3.tod_ = "2020-05-05T09:59:59.000Z"_ts;
    tr_adsb_101_3.trk_nb_ = 101;

    trk_adsb_101 << tr_adsb_101_1 << tr_adsb_101_2 << tr_adsb_101_3;


    Track trk_adsb_102(SystemType::Adsb, 102);

    TargetReport tr_adsb_102_1;
    tr_adsb_102_1.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    tr_adsb_102_1.trk_nb_ = 102;

    TargetReport tr_adsb_102_2;
    tr_adsb_102_2.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_2.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    tr_adsb_102_2.trk_nb_ = 102;

    TargetReport tr_adsb_102_3;
    tr_adsb_102_3.sys_typ_ = SystemType::Adsb;
    tr_adsb_102_3.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    tr_adsb_102_3.trk_nb_ = 102;

    trk_adsb_102 << tr_adsb_102_1 << tr_adsb_102_2 << tr_adsb_102_3;


    Track trk_mlat_201(SystemType::Mlat, 201);

    TargetReport tr_mlat_201_1;
    tr_mlat_201_1.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_1.tod_ = "2020-05-05T10:00:00.250Z"_ts;
    tr_mlat_201_1.trk_nb_ = 201;

    TargetReport tr_mlat_201_2;
    tr_mlat_201_2.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_2.tod_ = "2020-05-05T10:00:02.250Z"_ts;
    tr_mlat_201_2.trk_nb_ = 201;

    TargetReport tr_mlat_201_3;
    tr_mlat_201_3.sys_typ_ = SystemType::Mlat;
    tr_mlat_201_3.tod_ = "2020-05-05T10:00:04.250Z"_ts;
    tr_mlat_201_3.trk_nb_ = 201;

    trk_mlat_201 << tr_mlat_201_1 << tr_mlat_201_2 << tr_mlat_201_3;


    Track trk_smr_301(SystemType::Smr, 301);

    TargetReport tr_smr_201_1;
    tr_smr_201_1.sys_typ_ = SystemType::Smr;
    tr_smr_201_1.tod_ = "2020-05-05T10:00:00.500Z"_ts;
    tr_smr_201_1.trk_nb_ = 301;

    TargetReport tr_smr_201_2;
    tr_smr_201_2.sys_typ_ = SystemType::Smr;
    tr_smr_201_2.tod_ = "2020-05-05T10:00:02.500Z"_ts;
    tr_smr_201_2.trk_nb_ = 301;

    TargetReport tr_smr_201_3;
    tr_smr_201_3.sys_typ_ = SystemType::Smr;
    tr_smr_201_3.tod_ = "2020-05-05T10:00:04.500Z"_ts;
    tr_smr_201_3.trk_nb_ = 301;

    trk_smr_301 << tr_smr_201_1 << tr_smr_201_2 << tr_smr_201_3;


    /* -------------------------------------------------------------------- */
    // Test default-constructed set.
    /* -------------------------------------------------------------------- */

    TrackCollectionSet set_dflt;

    QCOMPARE(set_dflt.ref_sys_type(), SystemType::Unknown);
    QCOMPARE(set_dflt.mode_s(), static_cast<quint32>(0xFFFFFF));

    QVERIFY(set_dflt.hasRefData() == false);
    QVERIFY(set_dflt.hasTestData() == false);
    QVERIFY(set_dflt.isValid() == false);
    QVERIFY(set_dflt.isEmpty() == true);
    QCOMPARE(set_dflt.size(), 0);

    QCOMPARE(set_dflt.refTrackCol(), TrackCollection());
    QCOMPARE(set_dflt.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_dflt.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    // INVALID INSERTION ATTEMPT:
    // Attempting to add tracks to a default-constructed set should have no
    // effect so all the previous checks should still hold.
    set_dflt << Track(SystemType::Adsb, 100);

    QCOMPARE(set_dflt.ref_sys_type(), SystemType::Unknown);
    QCOMPARE(set_dflt.mode_s(), static_cast<quint32>(0xFFFFFF));

    QVERIFY(set_dflt.hasRefData() == false);
    QVERIFY(set_dflt.hasTestData() == false);
    QVERIFY(set_dflt.isValid() == false);
    QVERIFY(set_dflt.isEmpty() == true);
    QCOMPARE(set_dflt.size(), 0);

    QCOMPARE(set_dflt.refTrackCol(), TrackCollection());
    QCOMPARE(set_dflt.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_dflt.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    /* -------------------------------------------------------------------- */
    // Test set constructed with ModeS address and system type.
    /* -------------------------------------------------------------------- */

    TrackCollectionSet set_0x000001(0x000001, SystemType::Adsb);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == false);
    QVERIFY(set_0x000001.hasTestData() == false);
    QVERIFY(set_0x000001.isValid() == false);
    QVERIFY(set_0x000001.isEmpty() == true);
    QCOMPARE(set_0x000001.size(), 0);

    QCOMPARE(set_0x000001.refTrackCol(), TrackCollection(SystemType::Adsb));
    QCOMPARE(set_0x000001.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_0x000001.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    // INVALID INSERTION ATTEMPT:
    // Attempting to add empty tracks to the set should have no effect so all
    // the previous checks should still hold.
    set_0x000001 << Track(SystemType::Adsb, 100);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == false);
    QVERIFY(set_0x000001.hasTestData() == false);
    QVERIFY(set_0x000001.isValid() == false);
    QVERIFY(set_0x000001.isEmpty() == true);
    QCOMPARE(set_0x000001.size(), 0);

    QCOMPARE(set_0x000001.refTrackCol(), TrackCollection(SystemType::Adsb));
    QCOMPARE(set_0x000001.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_0x000001.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    // VALID INSERTION #1: ADS-B track with track number 101.
    // The set must accept all tracks of known system type.
    set_0x000001 << trk_adsb_101;

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == false);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 0);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, trk_adsb_101));
    QCOMPARE(set_0x000001.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_0x000001.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    // INVALID INSERTION ATTEMPT:
    // Attempting to add an already existing track to the set should have no
    // effect so all the previous checks should still hold.
    set_0x000001 << trk_adsb_101;

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == false);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 0);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, trk_adsb_101));
    QCOMPARE(set_0x000001.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_0x000001.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    // INVALID MATCH ATTEMPT:
    // Attempting to add a match of a track with itself should have no effect
    // so all the previous checks should still hold.
    set_0x000001.addMatch(trk_adsb_101, trk_adsb_101);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == false);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 0);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, trk_adsb_101));
    QCOMPARE(set_0x000001.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_0x000001.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    // INVALID MATCH ATTEMPT:
    // Attempting to add a match in reverse order (test track before ref track)
    // should have no effect so all the previous checks should still hold.
    set_0x000001.addMatch(trk_mlat_201, trk_adsb_101);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == false);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 0);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, trk_adsb_101));
    QCOMPARE(set_0x000001.tstTrackCols(), QVector<TrackCollection>());

    QVERIFY(set_0x000001.matches().isEmpty() == true);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
    }


    // VALID MATCH #1: ADS-B track 101 --> MLAT track 201.
    set_0x000001.addMatch(trk_adsb_101, trk_mlat_201);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == true);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 1);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, trk_adsb_101));
    QCOMPARE(set_0x000001.tstTrackCols(),
        QVector<TrackCollection>({TrackCollection(SystemType::Mlat, trk_mlat_201)}));

    QVERIFY(set_0x000001.matches().isEmpty() == false);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        if (tn == 101)
        {
            QCOMPARE(set_0x000001.matchesForRefTrack(tn),
                QVector<TrackCollection>({TrackCollection(SystemType::Mlat, trk_mlat_201)}));
        }
        else
        {
            QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
        }
    }


    // VALID MATCH #2: ADS-B track 102 --> MLAT track 201.
    set_0x000001.addMatch(trk_adsb_102, trk_mlat_201);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == true);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 1);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, {trk_adsb_101, trk_adsb_102}));
    QCOMPARE(set_0x000001.tstTrackCols(),
        QVector<TrackCollection>({TrackCollection(SystemType::Mlat, trk_mlat_201)}));

    QVERIFY(set_0x000001.matches().isEmpty() == false);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        if (tn == 101)
        {
            QCOMPARE(set_0x000001.matchesForRefTrack(tn),
                QVector<TrackCollection>({TrackCollection(SystemType::Mlat, trk_mlat_201)}));
        }
        else if (tn == 102)
        {
            QCOMPARE(set_0x000001.matchesForRefTrack(tn),
                QVector<TrackCollection>({TrackCollection(SystemType::Mlat, trk_mlat_201)}));
        }
        else
        {
            QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
        }
    }


    // VALID MATCH #3: ADS-B track 102 --> SMR track 301.
    set_0x000001.addMatch(trk_adsb_102, trk_smr_301);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == true);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 2);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, {trk_adsb_101, trk_adsb_102}));
    QCOMPARE(set_0x000001.tstTrackCols(),
        QVector<TrackCollection>(
            {TrackCollection(SystemType::Mlat, trk_mlat_201),
                TrackCollection(SystemType::Smr, trk_smr_301)}));

    QVERIFY(set_0x000001.matches().isEmpty() == false);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        if (tn == 101)
        {
            QCOMPARE(set_0x000001.matchesForRefTrack(tn),
                QVector<TrackCollection>({TrackCollection(SystemType::Mlat, trk_mlat_201)}));
        }
        else if (tn == 102)
        {
            QCOMPARE(set_0x000001.matchesForRefTrack(tn),
                QVector<TrackCollection>(
                    {TrackCollection(SystemType::Mlat, trk_mlat_201),
                        TrackCollection(SystemType::Smr, trk_smr_301)}));
        }
        else
        {
            QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
        }
    }


    // INVALID MATCH ATTEMPT:
    // Attempting to add an already existing match should have no effect so
    // all the previous checks should still hold.
    set_0x000001.addMatch(trk_adsb_102, trk_smr_301);

    QCOMPARE(set_0x000001.ref_sys_type(), SystemType::Adsb);
    QCOMPARE(set_0x000001.mode_s(), static_cast<quint32>(0x000001));

    QVERIFY(set_0x000001.hasRefData() == true);
    QVERIFY(set_0x000001.hasTestData() == true);
    QVERIFY(set_0x000001.isValid() == true);
    QVERIFY(set_0x000001.isEmpty() == false);
    QCOMPARE(set_0x000001.size(), 2);

    QCOMPARE(set_0x000001.refTrackCol(),
        TrackCollection(SystemType::Adsb, {trk_adsb_101, trk_adsb_102}));
    QCOMPARE(set_0x000001.tstTrackCols(),
        QVector<TrackCollection>(
            {TrackCollection(SystemType::Mlat, trk_mlat_201),
                TrackCollection(SystemType::Smr, trk_smr_301)}));

    QVERIFY(set_0x000001.matches().isEmpty() == false);

    for (TrackNum tn = 0; tn <= 4095; ++tn)
    {
        if (tn == 101)
        {
            QCOMPARE(set_0x000001.matchesForRefTrack(tn),
                QVector<TrackCollection>({TrackCollection(SystemType::Mlat, trk_mlat_201)}));
        }
        else if (tn == 102)
        {
            QCOMPARE(set_0x000001.matchesForRefTrack(tn),
                QVector<TrackCollection>(
                    {TrackCollection(SystemType::Mlat, trk_mlat_201),
                        TrackCollection(SystemType::Smr, trk_smr_301)}));
        }
        else
        {
            QCOMPARE(set_dflt.matchesForRefTrack(tn), QVector<TrackCollection>());
        }
    }
}

QTEST_GUILESS_MAIN(TrackTest);
#include "tracktest.moc"
