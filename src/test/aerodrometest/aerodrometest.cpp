/*!
 * \file aerodrometest.cpp
 * \brief Implements unit tests for the Aerodrome class.
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

#include "aerodrome.h"
#include <QObject>
#include <QtTest>

class AerodromeTest : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();

    //void testLocatePoint_data();
    //void testLocatePoint();

private:
    //Aerodrome dummyAerodrome();
};

void AerodromeTest::test_data()
{
    QTest::addColumn<QPolygonF>("runway");
    QTest::addColumn<QPolygonF>("taxiway");
    QTest::addColumn<QPolygonF>("apron");
    QTest::addColumn<QPolygonF>("stand");
    QTest::addColumn<QPolygonF>("airborne1L");
    QTest::addColumn<QPolygonF>("airborne2L");
    QTest::addColumn<QPolygonF>("airborne1R");
    QTest::addColumn<QPolygonF>("airborne2R");

    QPolygonF runwayPolygon;
    runwayPolygon << QPointF(4.0, 0.5)
                  << QPointF(-4.0, 0.5)
                  << QPointF(-4.0, -0.5)
                  << QPointF(4.0, -0.5)
                  << QPointF(4.0, 0.5);

    QPolygonF taxiwayPolygon;
    taxiwayPolygon << QPointF(4.0, 2.0)
                   << QPointF(-4.0, 2.0)
                   << QPointF(-4.0, 0.5)
                   << QPointF(4.0, 0.5)
                   << QPointF(4.0, 2.0);

    QPolygonF standPolygon;
    standPolygon << QPointF(3.0, 4.0)
                 << QPointF(-3.0, 4.0)
                 << QPointF(-3.0, 3.0)
                 << QPointF(3.0, 3.0)
                 << QPointF(3.0, 4.0);

    QPolygonF apronPolygon;
    apronPolygon << QPointF(4.0, 4.0)
                 << QPointF(-4.0, 4.0)
                 << QPointF(-4.0, 2.0)
                 << QPointF(4.0, 2.0)
                 << QPointF(4.0, 4.0);
    //apronPolygon = apronPolygon.subtracted(standPolygon);

    QPolygonF airborne1LPolygon;
    airborne1LPolygon << QPointF(4.0, 0.5)
                      << QPointF(8.0, 1.0)
                      << QPointF(8.0, -1.0)
                      << QPointF(4.0, -0.5)
                      << QPointF(4.0, 0.5);

    QPolygonF airborne2LPolygon;
    airborne2LPolygon << QPointF(8.0, 1.0)
                      << QPointF(12.0, 1.5)
                      << QPointF(12.0, -1.5)
                      << QPointF(8.0, -1.0)
                      << QPointF(8.0, 1.0);

    QPolygonF airborne1RPolygon;
    airborne1RPolygon << QPointF(-4.0, 0.5)
                      << QPointF(-8.0, 1.0)
                      << QPointF(-8.0, -1.0)
                      << QPointF(-4.0, -0.5)
                      << QPointF(-4.0, 0.5);

    QPolygonF airborne2RPolygon;
    airborne2RPolygon << QPointF(-8.0, 1.0)
                      << QPointF(-12.0, 1.5)
                      << QPointF(-12.0, -1.5)
                      << QPointF(-8.0, -1.0)
                      << QPointF(-8.0, 1.0);

    QTest::newRow("Dummy Aerodrome")
        << runwayPolygon
        << taxiwayPolygon
        << apronPolygon
        << standPolygon
        << airborne1LPolygon
        << airborne2LPolygon
        << airborne1RPolygon
        << airborne2RPolygon;
}

void AerodromeTest::test()
{
    QFETCH(QPolygonF, runway);
    QFETCH(QPolygonF, taxiway);
    QFETCH(QPolygonF, apron);
    QFETCH(QPolygonF, stand);
    QFETCH(QPolygonF, airborne1L);
    QFETCH(QPolygonF, airborne2L);
    QFETCH(QPolygonF, airborne1R);
    QFETCH(QPolygonF, airborne2R);

    Aerodrome aerodrome;

    QVERIFY(aerodrome.hasAnyElements() == false);
    QVERIFY(aerodrome.hasAllElements() == false);

    aerodrome.addRunwayElement(runway);
    aerodrome.addTaxiwayElement(taxiway);

    QVERIFY(aerodrome.hasAnyElements() == true);
    QVERIFY(aerodrome.hasAllElements() == false);

    aerodrome.addApronElement(apron);
    aerodrome.addStandElement(stand);
    aerodrome.addAirborne1Element(airborne1L);
    aerodrome.addAirborne2Element(airborne2L);
    aerodrome.addAirborne1Element(airborne1R);
    aerodrome.addAirborne2Element(airborne2R);

    QVERIFY(aerodrome.hasAnyElements() == true);
    QVERIFY(aerodrome.hasAllElements() == true);
}

QTEST_APPLESS_MAIN(AerodromeTest)
#include "aerodrometest.moc"
