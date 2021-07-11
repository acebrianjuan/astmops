/*!
 * \file aixmreadertest.cpp
 * \brief Implements unit tests for the AixmReader class.
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

#include "aixmreader.h"
#include <QObject>
#include <QtTest>

class AixmReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();
};

void AixmReaderTest::test_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("runwayElements");
    QTest::addColumn<int>("taxiwayElements");
    QTest::addColumn<int>("apronLaneElements");
    //QTest::addColumn<int>("aircraftStands");

    QTest::addColumn<QGeoCoordinate>("arpCoordinates");
    QTest::addColumn<QGeoCoordinate>("runwayCoordinates");
    QTest::addColumn<QGeoCoordinate>("taxiwayCoordinates");
    QTest::addColumn<QGeoCoordinate>("apronLaneCoordinates");
    //QTest::addColumn<QGeoCoordinate>("standCoordinates");

    QTest::newRow("KCLT") << "kclt-test.xml"
                          << 1 << 1 << 1
                          << QGeoCoordinate(35.213740556000062, -80.949069166999948)
                          << QGeoCoordinate(35.203722345000074, -80.934106229999941)
                          << QGeoCoordinate(35.216137873000037, -80.93863754299997)
                          << QGeoCoordinate(35.209120710000036, -80.936863836999976);
}

void AixmReaderTest::test()
{
    AixmReader reader;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(int, runwayElements);
    QFETCH(int, taxiwayElements);
    QFETCH(int, apronLaneElements);
    //QFETCH(int, aircraftStands);

    QFETCH(QGeoCoordinate, arpCoordinates);
    QFETCH(QGeoCoordinate, runwayCoordinates);
    QFETCH(QGeoCoordinate, taxiwayCoordinates);
    QFETCH(QGeoCoordinate, apronLaneCoordinates);
    //QFETCH(QGeoCoordinate, standCoordinates);

    reader.read(&file);
    //Aerodrome aerodrome = reader.makeAerodrome();

    // Check number of elements of each collection.
    QCOMPARE(reader.runwayElements_.size(), runwayElements);
    QCOMPARE(reader.taxiwayElements_.size(), taxiwayElements);
    QCOMPARE(reader.apronLaneElements_.size(), apronLaneElements);
    //QCOMPARE(reader.m_standElements.size(), aircraftStands);

    // Check first coordinates of each collection.
    QCOMPARE(reader.arp_, arpCoordinates);
    QCOMPARE(reader.runwayElements_.value({}).first().first(), runwayCoordinates);
    QCOMPARE(reader.taxiwayElements_.value({}).first().first(), taxiwayCoordinates);
    QCOMPARE(reader.apronLaneElements_.value({}).first().first(), apronLaneCoordinates);
    //QCOMPARE(reader.m_standElements.value({}).first().first(), standCoordinates);
}

QTEST_APPLESS_MAIN(AixmReaderTest)
#include "aixmreadertest.moc"
