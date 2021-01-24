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
    void test_data();
    void test();
};
Q_DECLARE_METATYPE(QVector<QGeoPositionInfo>);
Q_DECLARE_METATYPE(ErrorType);

void DgpsCsvReaderTest::test_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QVector<QGeoPositionInfo>>("posInfo");
    QTest::addColumn<ErrorType>("errorType");

    double factor = 0.3048;

    QVector<QGeoPositionInfo> posInfo;
    posInfo
        << QGeoPositionInfo(QGeoCoordinate(41.2854687222, 2.0835099167, 188.6163796653 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:05.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2857005833, 2.0841903056, 189.9052917801 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:06.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2859409167, 2.0848886667, 193.4340869974 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:07.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2861901389, 2.0856012778, 198.6026889070 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:08.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2864456111, 2.0863271389, 206.4554209471 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:09.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2867038333, 2.0870648056, 218.5923658957 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:10.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2869634167, 2.0878136667, 232.5341582611 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:11.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2872252778, 2.0885727222, 246.6456595994 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:12.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2874895278, 2.0893413889, 261.7956146481 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:13.351Z"), Qt::ISODateWithMs))
        << QGeoPositionInfo(QGeoCoordinate(41.2877548889, 2.0901189722, 278.9660826756 * factor),
               QDateTime::fromString(QLatin1String("2020-05-05T08:06:14.351Z"), Qt::ISODateWithMs));

    QTest::newRow("DGPS: UNIX; DD; DD; ft") << "dgps_unix.csv" << posInfo << ErrorType::NoError;
    QTest::newRow("DGPS: ISO; DD; DD; ft") << "dgps_iso.csv" << posInfo << ErrorType::NoError;
    QTest::newRow("DGPS (bad header)") << "dgps_bad_header.csv" << QVector<QGeoPositionInfo>() << ErrorType::NotWellFormedHeaderError;
}

void DgpsCsvReaderTest::test()
{
    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));

    QFETCH(QVector<QGeoPositionInfo>, posInfo);
    QFETCH(ErrorType, errorType);

    ErrorType error;
    QMultiMap<QDateTime, QGeoPositionInfo> refData = readDgpsCsv(&file, &error);

    QCOMPARE(error, errorType);
    QVERIFY(refData.size() == posInfo.size());

    int i = 0;
    QMapIterator<QDateTime, QGeoPositionInfo> itRef(refData);
    while (itRef.hasNext())
    {
        itRef.next();

        QGeoPositionInfo p1 = itRef.value();
        //qDebug() << p1;

        QGeoPositionInfo p2 = posInfo.at(i);
        //qDebug() << p2;

        QCOMPARE(p1.timestamp(), p2.timestamp());
        QVERIFY(p1.coordinate().distanceTo(p2.coordinate()) <= 1);
        QCOMPARE(p1.coordinate().altitude(), p2.coordinate().altitude());

        ++i;
    }
}

QTEST_APPLESS_MAIN(DgpsCsvReaderTest)
#include "dgpscsvreadertest.moc"
