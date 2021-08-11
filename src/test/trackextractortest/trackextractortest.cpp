/*!
 * \file trackextractortest.cpp
 * \brief Implements unit tests for the TrackExtractor class.
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

#include "trackextractor.h"
#include "config.h"
#include <QObject>
#include <QtTest>

class TrackExtractorTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_data();
    void test();
};

void TrackExtractorTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-trackextractortest"));

    Settings settings;
    settings.clear();

    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 219);
}

void TrackExtractorTest::test_data()
{
    using namespace Literals;

    // SMR Track 1:
    TargetReport smrTgtRep1;
    smrTgtRep1.sys_typ_ = SystemType::Smr;
    smrTgtRep1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    smrTgtRep1.trk_nb_ = 1001;
    smrTgtRep1.x_ = 0.0;
    smrTgtRep1.y_ = 0.0;

    TargetReport smrTgtRep2;
    smrTgtRep2.sys_typ_ = SystemType::Smr;
    smrTgtRep2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    smrTgtRep2.trk_nb_ = 1001;
    smrTgtRep2.x_ = 0.0;
    smrTgtRep2.y_ = 0.0;

    TargetReport smrTgtRep3;
    smrTgtRep3.sys_typ_ = SystemType::Smr;
    smrTgtRep3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    smrTgtRep3.trk_nb_ = 1001;
    smrTgtRep3.x_ = 0.0;
    smrTgtRep3.y_ = 0.0;

    Track smrTrack1(SystemType::Smr, 1001, {smrTgtRep1, smrTgtRep2, smrTgtRep3});

    // SMR Track 2:
    TargetReport smrTgtRep4;
    smrTgtRep4.sys_typ_ = SystemType::Smr;
    smrTgtRep4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    smrTgtRep4.trk_nb_ = 1002;
    smrTgtRep4.x_ = 0.0;
    smrTgtRep4.y_ = 0.0;

    TargetReport smrTgtRep5;
    smrTgtRep5.sys_typ_ = SystemType::Smr;
    smrTgtRep5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    smrTgtRep5.trk_nb_ = 1002;
    smrTgtRep5.x_ = 0.0;
    smrTgtRep5.y_ = 0.0;

    TargetReport smrTgtRep6;
    smrTgtRep6.sys_typ_ = SystemType::Smr;
    smrTgtRep6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    smrTgtRep6.trk_nb_ = 1002;
    smrTgtRep6.x_ = 0.0;
    smrTgtRep6.y_ = 0.0;

    Track smrTrack2(SystemType::Smr, 1002, {smrTgtRep4, smrTgtRep5, smrTgtRep6});

    QVector<Track> smrTracksOut;
    smrTracksOut << smrTrack1 << smrTrack2;

    QVector<TargetReport> smrTgtRepIn;
    smrTgtRepIn << smrTgtRep1 << smrTgtRep2 << smrTgtRep3
                << smrTgtRep4 << smrTgtRep5 << smrTgtRep6;


    // MLAT Track 1:
    TargetReport mlatTgtRep1;
    mlatTgtRep1.sys_typ_ = SystemType::Mlat;
    mlatTgtRep1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    mlatTgtRep1.trk_nb_ = 2001;
    mlatTgtRep1.x_ = 0.0;
    mlatTgtRep1.y_ = 0.0;

    TargetReport mlatTgtRep2;
    mlatTgtRep2.sys_typ_ = SystemType::Mlat;
    mlatTgtRep2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    mlatTgtRep2.trk_nb_ = 2001;
    mlatTgtRep2.x_ = 0.0;
    mlatTgtRep2.y_ = 0.0;

    TargetReport mlatTgtRep3;
    mlatTgtRep3.sys_typ_ = SystemType::Mlat;
    mlatTgtRep3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    mlatTgtRep3.trk_nb_ = 2001;
    mlatTgtRep3.x_ = 0.0;
    mlatTgtRep3.y_ = 0.0;

    Track mlatTrack1(SystemType::Mlat, 2001, {mlatTgtRep1, mlatTgtRep2, mlatTgtRep3});

    // MLAT Track 2:
    TargetReport mlatTgtRep4;
    mlatTgtRep4.sys_typ_ = SystemType::Mlat;
    mlatTgtRep4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    mlatTgtRep4.trk_nb_ = 2002;
    mlatTgtRep4.x_ = 0.0;
    mlatTgtRep4.y_ = 0.0;

    TargetReport mlatTgtRep5;
    mlatTgtRep5.sys_typ_ = SystemType::Mlat;
    mlatTgtRep5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    mlatTgtRep5.trk_nb_ = 2002;
    mlatTgtRep5.x_ = 0.0;
    mlatTgtRep5.y_ = 0.0;

    TargetReport mlatTgtRep6;
    mlatTgtRep6.sys_typ_ = SystemType::Mlat;
    mlatTgtRep6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    mlatTgtRep6.trk_nb_ = 2002;
    mlatTgtRep6.x_ = 0.0;
    mlatTgtRep6.y_ = 0.0;

    Track mlatTrack2(SystemType::Mlat, 2002, {mlatTgtRep4, mlatTgtRep5, mlatTgtRep6});

    QVector<Track> mlatTracksOut;
    mlatTracksOut << mlatTrack1 << mlatTrack2;

    QVector<TargetReport> mlatTgtRepIn;
    mlatTgtRepIn << mlatTgtRep1 << mlatTgtRep2 << mlatTgtRep3
                 << mlatTgtRep4 << mlatTgtRep5 << mlatTgtRep6;


    // ADS-B Track 1:
    TargetReport adsbTgtRep1;
    adsbTgtRep1.sys_typ_ = SystemType::Adsb;
    adsbTgtRep1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    adsbTgtRep1.trk_nb_ = 3001;
    adsbTgtRep1.x_ = 0.0;
    adsbTgtRep1.y_ = 0.0;

    TargetReport adsbTgtRep2;
    adsbTgtRep2.sys_typ_ = SystemType::Adsb;
    adsbTgtRep2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    adsbTgtRep2.trk_nb_ = 3001;
    adsbTgtRep2.x_ = 0.0;
    adsbTgtRep2.y_ = 0.0;

    TargetReport adsbTgtRep3;
    adsbTgtRep3.sys_typ_ = SystemType::Adsb;
    adsbTgtRep3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    adsbTgtRep3.trk_nb_ = 3001;
    adsbTgtRep3.x_ = 0.0;
    adsbTgtRep3.y_ = 0.0;

    Track adsbTrack1(SystemType::Adsb, 3001, {adsbTgtRep1, adsbTgtRep2, adsbTgtRep3});

    // ADS-B Track 2:
    TargetReport adsbTgtRep4;
    adsbTgtRep4.sys_typ_ = SystemType::Adsb;
    adsbTgtRep4.tod_ = "2020-05-05T10:00:03.000Z"_ts;
    adsbTgtRep4.trk_nb_ = 3002;
    adsbTgtRep4.x_ = 0.0;
    adsbTgtRep4.y_ = 0.0;

    TargetReport adsbTgtRep5;
    adsbTgtRep5.sys_typ_ = SystemType::Adsb;
    adsbTgtRep5.tod_ = "2020-05-05T10:00:04.000Z"_ts;
    adsbTgtRep5.trk_nb_ = 3002;
    adsbTgtRep5.x_ = 0.0;
    adsbTgtRep5.y_ = 0.0;

    TargetReport adsbTgtRep6;
    adsbTgtRep6.sys_typ_ = SystemType::Adsb;
    adsbTgtRep6.tod_ = "2020-05-05T10:00:05.000Z"_ts;
    adsbTgtRep6.trk_nb_ = 3002;
    adsbTgtRep6.x_ = 0.0;
    adsbTgtRep6.y_ = 0.0;

    Track adsbTrack2(SystemType::Adsb, 3002, {adsbTgtRep4, adsbTgtRep5, adsbTgtRep6});

    QVector<Track> adsbTracksOut;
    adsbTracksOut << adsbTrack1 << adsbTrack2;

    QVector<TargetReport> adsbTgtRepIn;
    adsbTgtRepIn << adsbTgtRep1 << adsbTgtRep2 << adsbTgtRep3
                 << adsbTgtRep4 << adsbTgtRep5 << adsbTgtRep6;


    // DGPS Track:
    TargetReport dgpsTgtRep1;
    dgpsTgtRep1.sys_typ_ = SystemType::Dgps;
    dgpsTgtRep1.tod_ = "2020-05-05T10:00:00.000Z"_ts;
    dgpsTgtRep1.trk_nb_ = 4001;
    dgpsTgtRep1.x_ = 0.0;
    dgpsTgtRep1.y_ = 0.0;

    TargetReport dgpsTgtRep2;
    dgpsTgtRep2.sys_typ_ = SystemType::Dgps;
    dgpsTgtRep2.tod_ = "2020-05-05T10:00:01.000Z"_ts;
    dgpsTgtRep2.trk_nb_ = 4001;
    dgpsTgtRep2.x_ = 0.0;
    dgpsTgtRep2.y_ = 0.0;

    TargetReport dgpsTgtRep3;
    dgpsTgtRep3.sys_typ_ = SystemType::Dgps;
    dgpsTgtRep3.tod_ = "2020-05-05T10:00:02.000Z"_ts;
    dgpsTgtRep3.trk_nb_ = 4001;
    dgpsTgtRep3.x_ = 0.0;
    dgpsTgtRep3.y_ = 0.0;

    Track dgpsTrack1(SystemType::Dgps, 4001, {dgpsTgtRep1, dgpsTgtRep2, dgpsTgtRep3});

    QVector<Track> dgpsTracksOut;
    dgpsTracksOut << dgpsTrack1;

    QVector<TargetReport> dgpsTgtRepIn;
    dgpsTgtRepIn << dgpsTgtRep1 << dgpsTgtRep2 << dgpsTgtRep3;


    QTest::addColumn<SystemType>("sysType");
    QTest::addColumn<QVector<TargetReport>>("tgtRepsIn");
    QTest::addColumn<QVector<Track>>("tracksOut");

    QTest::newRow("SMR") << SystemType::Smr << smrTgtRepIn << smrTracksOut;
    QTest::newRow("MLAT") << SystemType::Mlat << mlatTgtRepIn << mlatTracksOut;
    QTest::newRow("ADS-B") << SystemType::Adsb << adsbTgtRepIn << adsbTracksOut;
    QTest::newRow("DGPS") << SystemType::Dgps << dgpsTgtRepIn << dgpsTracksOut;
}

void TrackExtractorTest::test()
{
    TrackExtractor trackExtr;

    QFETCH(SystemType, sysType);
    QFETCH(QVector<TargetReport>, tgtRepsIn);
    QFETCH(QVector<Track>, tracksOut);

    // Feed Target Reports.
    for (const TargetReport &tr : tgtRepsIn)
    {
        trackExtr.addData(tr);
    }

    QVector<Track> tracks = trackExtr.tracks(sysType);

    QVERIFY(tracks.size() == tracksOut.size());

    for (int i = 0; i < tracksOut.size(); ++i)
    {
        QCOMPARE(tracks.at(i).duration(), 2.0);
        QCOMPARE(tracks.at(i), tracksOut.at(i));
    }
}

QTEST_GUILESS_MAIN(TrackExtractorTest);
#include "trackextractortest.moc"
