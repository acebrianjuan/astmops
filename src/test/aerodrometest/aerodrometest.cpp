/*!
 * \file aerodrometest.cpp
 * \brief Implements unit tests for the Aerodrome class.
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

#include "aerodrome.h"
#include "areahash.h"
#include <QObject>
#include <QtTest>

class AerodromeTest : public QObject
{
    Q_OBJECT

private slots:
    void testElements_data();
    void testElements();

    void testLocatePoint_data();
    void testLocatePoint();
};

void AerodromeTest::testElements_data()
{
    QTest::addColumn<QPolygonF>("runway");
    QTest::addColumn<QPolygonF>("taxiway");
    QTest::addColumn<QPolygonF>("apronLane");
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

    QPolygonF apronPolygon;  // Union of ApronLane + Stand.
    apronPolygon << QPointF(4.0, 4.0)
                 << QPointF(-4.0, 4.0)
                 << QPointF(-4.0, 2.0)
                 << QPointF(4.0, 2.0)
                 << QPointF(4.0, 4.0);

    QPolygonF standPolygon;
    standPolygon << QPointF(3.0, 4.0)
                 << QPointF(-3.0, 4.0)
                 << QPointF(-3.0, 3.0)
                 << QPointF(3.0, 3.0)
                 << QPointF(3.0, 4.0);

    QPolygonF apronLanePolygon = apronPolygon.subtracted(standPolygon);

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
        << apronLanePolygon
        << standPolygon
        << airborne1LPolygon
        << airborne2LPolygon
        << airborne1RPolygon
        << airborne2RPolygon;
}

void AerodromeTest::testElements()
{
    QFETCH(QPolygonF, runway);
    QFETCH(QPolygonF, taxiway);
    QFETCH(QPolygonF, apronLane);
    QFETCH(QPolygonF, stand);
    QFETCH(QPolygonF, airborne1L);
    QFETCH(QPolygonF, airborne2L);
    QFETCH(QPolygonF, airborne1R);
    QFETCH(QPolygonF, airborne2R);

    Aerodrome aerodrome;  // Empty aerodrome.

    // An empty aerodrome has no elements.
    QVERIFY(aerodrome.hasAnyElements() == false);
    QVERIFY(aerodrome.hasAllElements() == false);

    // Add some elements.
    aerodrome.addRunwayElement({}, runway);
    aerodrome.addTaxiwayElement({}, taxiway);

    // Aerodrome is still incomplete.
    QVERIFY(aerodrome.hasAnyElements() == true);
    QVERIFY(aerodrome.hasAllElements() == false);

    // Add elements of remaining areas.
    aerodrome.addApronLaneElement({}, apronLane);
    aerodrome.addStandElement({}, stand);
    aerodrome.addAirborne1Element({}, airborne1L);
    aerodrome.addAirborne2Element({}, airborne2L);
    aerodrome.addAirborne1Element({}, airborne1R);
    aerodrome.addAirborne2Element({}, airborne2R);

    // Aerodrome is now complete as it has at least one element for each area.
    QVERIFY(aerodrome.hasAnyElements() == true);
    QVERIFY(aerodrome.hasAllElements() == true);
}

void AerodromeTest::testLocatePoint_data()
{
    QTest::addColumn<Aerodrome>("aerodrome");
    QTest::addColumn<QVector<QPair<QVector3D, uint>>>("points");
    QTest::addColumn<QVector<Aerodrome::NamedArea>>("result");

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

    QPolygonF apronPolygon;  // Union of ApronLane + Stand.
    apronPolygon << QPointF(4.0, 4.0)
                 << QPointF(-4.0, 4.0)
                 << QPointF(-4.0, 2.0)
                 << QPointF(4.0, 2.0)
                 << QPointF(4.0, 4.0);

    QPolygonF standPolygon;
    standPolygon << QPointF(3.0, 4.0)
                 << QPointF(-3.0, 4.0)
                 << QPointF(-3.0, 3.0)
                 << QPointF(3.0, 3.0)
                 << QPointF(3.0, 4.0);

    QPolygonF apronLanePolygon = apronPolygon.subtracted(standPolygon);

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

    Aerodrome aerodrome;
    aerodrome.addRunwayElement({}, runwayPolygon);
    aerodrome.addTaxiwayElement({}, taxiwayPolygon);
    aerodrome.addApronLaneElement({}, apronLanePolygon);
    aerodrome.addStandElement({}, standPolygon);
    aerodrome.addAirborne1Element({}, airborne1LPolygon);
    aerodrome.addAirborne2Element({}, airborne2LPolygon);
    aerodrome.addAirborne1Element({}, airborne1RPolygon);
    aerodrome.addAirborne2Element({}, airborne2RPolygon);

    QVector<QPair<QVector3D, uint>> points;
    points << QPair<QVector3D, uint>(QVector3D(10, 0, 1), 0)
           << QPair<QVector3D, uint>(QVector3D(6, 0, 1), 0)
           << QPair<QVector3D, uint>(QVector3D(0, 0, 0), 1)
           << QPair<QVector3D, uint>(QVector3D(0, 1, 0), 1)
           << QPair<QVector3D, uint>(QVector3D(0, 2, 0), 1)
           << QPair<QVector3D, uint>(QVector3D(0, 3.5, 0), 1);

    QVector<Aerodrome::NamedArea> nareas;
    nareas << Aerodrome::NamedArea(Aerodrome::Area::Airborne2)
           << Aerodrome::NamedArea(Aerodrome::Area::Airborne1)
           << Aerodrome::NamedArea(Aerodrome::Area::Runway)
           << Aerodrome::NamedArea(Aerodrome::Area::Taxiway)
           << Aerodrome::NamedArea(Aerodrome::Area::ApronLane)
           << Aerodrome::NamedArea(Aerodrome::Area::Stand);

    QTest::newRow("Dummy Aerodrome") << aerodrome
                                     << points
                                     << nareas;
}

void AerodromeTest::testLocatePoint()
{
    using PairVec = QVector<QPair<QVector3D, uint>>;

    QFETCH(Aerodrome, aerodrome);
    QFETCH(PairVec, points);
    QFETCH(QVector<Aerodrome::NamedArea>, result);

    QCOMPARE(points.size(), result.size());

    auto it = result.begin();
    for (QPair<QVector3D, uint> pair : points)
    {
        Aerodrome::NamedArea narea = aerodrome.locatePoint(pair.first, pair.second);
        QCOMPARE(narea, *it);
        ++it;
    }
}

QTEST_APPLESS_MAIN(AerodromeTest)
#include "aerodrometest.moc"
