/*!
 * \file kmlreadertest.cpp
 * \brief Implements unit tests for the KmlReader class.
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

#include "kmlreader.h"
#include <QObject>
#include <QtTest>

class KmlReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();
};

void KmlReaderTest::test_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("runwayElements");
    QTest::addColumn<int>("taxiwayElements");
    QTest::addColumn<int>("apronElements");
    QTest::addColumn<int>("aircraftStands");

    QTest::addColumn<QGeoCoordinate>("arpCoordinates");
    QTest::addColumn<QGeoCoordinate>("runwayCoordinates");
    QTest::addColumn<QGeoCoordinate>("taxiwayCoordinates");
    QTest::addColumn<QGeoCoordinate>("apronCoordinates");
    QTest::addColumn<QGeoCoordinate>("standCoordinates");

    QTest::newRow("LEBL") << "lebl_test.kml"
                          << 2 << 1 << 1 << 1
                          << QGeoCoordinate(41.29707777777777, 2.078462920115866, 0)
                          << QGeoCoordinate(41.2827740402838, 2.07656517967082, 0)
                          << QGeoCoordinate(41.2868783930656, 2.07892935775076, 0)
                          << QGeoCoordinate(41.2994286800839, 2.07137880758195, 0)
                          << QGeoCoordinate(41.28896414516679, 2.080953038858968, 0);
}

void KmlReaderTest::test()
{
    KmlReader reader;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(int, runwayElements);
    QFETCH(int, taxiwayElements);
    QFETCH(int, apronElements);
    QFETCH(int, aircraftStands);

    QFETCH(QGeoCoordinate, arpCoordinates);
    QFETCH(QGeoCoordinate, runwayCoordinates);
    QFETCH(QGeoCoordinate, taxiwayCoordinates);
    QFETCH(QGeoCoordinate, apronCoordinates);
    QFETCH(QGeoCoordinate, standCoordinates);

    reader.read(&file);
    //Aerodrome aerodrome = reader.makeAerodrome();

    // Check number of elements of each collection.
    QCOMPARE(reader.m_runwayElements.size(), runwayElements);
    QCOMPARE(reader.m_taxiwayElements.size(), taxiwayElements);
    QCOMPARE(reader.m_apronElements.size(), apronElements);
    QCOMPARE(reader.m_standElements.size(), aircraftStands);

    // Check first coordinates of each collection.
    QCOMPARE(reader.m_arp, arpCoordinates);
    QCOMPARE(reader.m_runwayElements.value(QLatin1String("07R/25L")).first().first(), runwayCoordinates);
    QCOMPARE(reader.m_taxiwayElements.value({}).first().first(), taxiwayCoordinates);
    QCOMPARE(reader.m_apronElements.value({}).first().first(), apronCoordinates);
    QCOMPARE(reader.m_standElements.value({}).first().first(), standCoordinates);
}

QTEST_APPLESS_MAIN(KmlReaderTest)
#include "kmlreadertest.moc"
