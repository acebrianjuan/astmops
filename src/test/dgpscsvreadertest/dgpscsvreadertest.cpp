/*!
 * \file dgpsreadertest.cpp
 * \brief Implements unit tests for the readDgpsCsv() function.
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

#include "dgpscsvreader.h"
#include <QGeoCoordinate>
#include <QObject>
#include <QtTest>

class DgpsCsvReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testReadDgpsCsv_data();
    void testReadDgpsCsv();

    void testMakeDgpsTgtRep_data();
    void testMakeDgpsTgtRep();

private:
    double ft_to_m = 0.3048;
};
Q_DECLARE_METATYPE(QVector<QGeoPositionInfo>);
Q_DECLARE_METATYPE(QVector<TargetReport>);
Q_DECLARE_METATYPE(ErrorType);

void DgpsCsvReaderTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-dgpscsvreadertest"));

    QSettings settings;
    settings.clear();

    settings.beginGroup(QLatin1String("DGPS"));
    settings.setValue(QLatin1String("TargetAddress"), 0x34304F);
    settings.endGroup();
}

void DgpsCsvReaderTest::testReadDgpsCsv_data()
{
    // Expected output.
    QVector<QGeoPositionInfo> posInfo;
    posInfo
        << QGeoPositionInfo(QGeoCoordinate(41.2854687222, 2.0835099167, 188.6163796653 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:05.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2857005833, 2.0841903056, 189.9052917801 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:06.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2859409167, 2.0848886667, 193.4340869974 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:07.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2861901389, 2.0856012778, 198.6026889070 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:08.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2864456111, 2.0863271389, 206.4554209471 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:09.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2867038333, 2.0870648056, 218.5923658957 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:10.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2869634167, 2.0878136667, 232.5341582611 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:11.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2872252778, 2.0885727222, 246.6456595994 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:12.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2874895278, 2.0893413889, 261.7956146481 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:13.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2877548889, 2.0901189722, 278.9660826756 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:14.351Z"), Qt::ISODateWithMs));


    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QVector<QGeoPositionInfo>>("posInfo");
    QTest::addColumn<ErrorType>("errorType");

    QTest::newRow("DGPS: UNIX; DD; DD; ft") << "dgps_unix.csv" << posInfo << ErrorType::NoError;
    QTest::newRow("DGPS: ISO; DD; DD; ft") << "dgps_iso.csv" << posInfo << ErrorType::NoError;
    QTest::newRow("DGPS (bad header)") << "dgps_bad_header.csv" << QVector<QGeoPositionInfo>() << ErrorType::NotWellFormedHeaderError;
}

void DgpsCsvReaderTest::testReadDgpsCsv()
{
    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));

    QFETCH(QVector<QGeoPositionInfo>, posInfo);
    QFETCH(ErrorType, errorType);

    ErrorType error;
    QVector<QGeoPositionInfo> refData = readDgpsCsv(&file, &error);

    QCOMPARE(error, errorType);
    QVERIFY(refData.size() == posInfo.size());

    for (int i = 0; i < refData.size(); ++i)
    {
        QGeoPositionInfo p_actual = refData.at(i);
        QGeoPositionInfo p_expected = posInfo.at(i);

        QCOMPARE(p_actual.timestamp(), p_expected.timestamp());
        QVERIFY(p_actual.coordinate().distanceTo(p_expected.coordinate()) < 0.1);
        QCOMPARE(p_actual.coordinate().altitude(), p_expected.coordinate().altitude());
    }
}

void DgpsCsvReaderTest::testMakeDgpsTgtRep_data()
{
    // Input.
    QVector<QGeoPositionInfo> posInfo;
    posInfo
        << QGeoPositionInfo(QGeoCoordinate(41.2854687222, 2.0835099167, 188.6163796653 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:05.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2857005833, 2.0841903056, 189.9052917801 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:06.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2859409167, 2.0848886667, 193.4340869974 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:07.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2861901389, 2.0856012778, 198.6026889070 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:08.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2864456111, 2.0863271389, 206.4554209471 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:09.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2867038333, 2.0870648056, 218.5923658957 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:10.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2869634167, 2.0878136667, 232.5341582611 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:11.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2872252778, 2.0885727222, 246.6456595994 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:12.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2874895278, 2.0893413889, 261.7956146481 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:13.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2877548889, 2.0901189722, 278.9660826756 * ft_to_m),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:14.351Z"), Qt::ISODateWithMs));

    // Expected output.
    QVector<TargetReport> tgtRep;

    TargetReport tr;
    tr.rec_typ_ = RecordType(SystemType::Dgps, MessageType::TargetReport);
    tr.mode_S_ = 0x34304F;
    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:05.351Z"), Qt::ISODateWithMs);
    tr.x_ = 422.796423;
    tr.y_ = -1289.162720;
    tr.z_ = 53.025692;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:06.351Z"), Qt::ISODateWithMs);
    tr.x_ = 479.792215;
    tr.y_ = -1263.408667;
    tr.z_ = 53.419691;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:07.351Z"), Qt::ISODateWithMs);
    tr.x_ = 538.293154;
    tr.y_ = -1236.713247;
    tr.z_ = 54.495850;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:08.351Z"), Qt::ISODateWithMs);
    tr.x_ = 597.987385;
    tr.y_ = -1209.030149;
    tr.z_ = 56.071250;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:09.351Z"), Qt::ISODateWithMs);
    tr.x_ = 658.791174;
    tr.y_ = -1180.652468;
    tr.z_ = 58.464110;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:10.351Z"), Qt::ISODateWithMs);
    tr.x_ = 720.583612;
    tr.y_ = -1151.968979;
    tr.z_ = 62.162036;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:11.351Z"), Qt::ISODateWithMs);
    tr.x_ = 783.313441;
    tr.y_ = -1123.133751;
    tr.z_ = 66.409265;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:12.351Z"), Qt::ISODateWithMs);
    tr.x_ = 846.896818;
    tr.y_ = -1094.044887;
    tr.z_ = 70.707404;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:13.351Z"), Qt::ISODateWithMs);
    tr.x_ = 911.284910;
    tr.y_ = -1064.690067;
    tr.z_ = 75.321228;
    tgtRep << tr;

    tr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T08:06:14.351Z"), Qt::ISODateWithMs);
    tr.x_ = 976.419590;
    tr.y_ = -1035.211232;
    tr.z_ = 80.550027;
    tgtRep << tr;


    QTest::addColumn<QVector<QGeoPositionInfo>>("posInfo");
    QTest::addColumn<QVector<TargetReport>>("tgtRep");

    QTest::newRow("DGPS TGTREP") << posInfo << tgtRep;
}

void DgpsCsvReaderTest::testMakeDgpsTgtRep()
{
    QFETCH(QVector<QGeoPositionInfo>, posInfo);
    QFETCH(QVector<TargetReport>, tgtRep);

    QMultiMap<QDateTime, TargetReport> refData = makeDgpsTgtRep(posInfo);

    QVERIFY(refData.size() == tgtRep.size());

    int i = 0;
    for (const TargetReport &p_actual : refData)
    {
        TargetReport p_expected = tgtRep.at(i);

        /*
        qDebug() << p_actual.tod_ << p_expected.tod_;
        qDebug() << p_actual.x_ << p_actual.y_ << p_actual.z_;
        qDebug() << p_expected.x_ << p_expected.y_ << p_expected.z_;
        */

        QCOMPARE(p_actual, p_expected);

        ++i;
    }
}

QTEST_APPLESS_MAIN(DgpsCsvReaderTest)
#include "dgpscsvreadertest.moc"
