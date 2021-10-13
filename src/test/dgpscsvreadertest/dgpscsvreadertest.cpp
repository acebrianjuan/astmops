/*!
 * \file dgpsreadertest.cpp
 * \brief Implements unit tests for the readDgpsCsv() function.
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

#include "dgpscsvreader.h"
#include "config.h"
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
};

Q_DECLARE_METATYPE(QVector<QGeoPositionInfo>);

void DgpsCsvReaderTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-dgpscsvreadertest"));

    Settings settings;
    settings.clear();
}

void DgpsCsvReaderTest::testReadDgpsCsv_data()
{
    using namespace Literals;

    // Expected output.
    QVector<QGeoPositionInfo> posInfo;
    posInfo
        << QGeoPositionInfo(QGeoCoordinate(41.2854687222, 2.0835099167, 188.6163796653 * ft_to_m),
               "2020-05-05T08:06:05.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2857005833, 2.0841903056, 189.9052917801 * ft_to_m),
               "2020-05-05T08:06:06.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2859409167, 2.0848886667, 193.4340869974 * ft_to_m),
               "2020-05-05T08:06:07.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2861901389, 2.0856012778, 198.6026889070 * ft_to_m),
               "2020-05-05T08:06:08.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2864456111, 2.0863271389, 206.4554209471 * ft_to_m),
               "2020-05-05T08:06:09.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2867038333, 2.0870648056, 218.5923658957 * ft_to_m),
               "2020-05-05T08:06:10.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2869634167, 2.0878136667, 232.5341582611 * ft_to_m),
               "2020-05-05T08:06:11.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2872252778, 2.0885727222, 246.6456595994 * ft_to_m),
               "2020-05-05T08:06:12.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2874895278, 2.0893413889, 261.7956146481 * ft_to_m),
               "2020-05-05T08:06:13.351Z"_ts)
        << QGeoPositionInfo(QGeoCoordinate(41.2877548889, 2.0901189722, 278.9660826756 * ft_to_m),
               "2020-05-05T08:06:14.351Z"_ts);


    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QVector<QGeoPositionInfo>>("posInfo");
    QTest::addColumn<ErrorType>("errorType");

    QTest::newRow("DGPS: UNIX; DD; DD; ft") << "dgps_unix.csv"
                                            << posInfo
                                            << ErrorType::NoError;

    QTest::newRow("DGPS: ISO; DD; DD; ft") << "dgps_iso.csv"
                                           << posInfo
                                           << ErrorType::NoError;

    QTest::newRow("DGPS (bad header)") << "dgps_bad_header.csv"
                                       << QVector<QGeoPositionInfo>()
                                       << ErrorType::NotWellFormedHeaderError;
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

QTEST_APPLESS_MAIN(DgpsCsvReaderTest)
#include "dgpscsvreadertest.moc"
