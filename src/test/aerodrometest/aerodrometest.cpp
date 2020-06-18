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
    Aerodrome dummyAerodrome();
};

void AerodromeTest::test_data()
{
    QTest::addColumn<QPolygonF>("runway");
    QTest::addColumn<QPolygonF>("taxiway");
    QTest::addColumn<QPolygonF>("apron");
    QTest::addColumn<QPolygonF>("stand");
    QTest::addColumn<QPolygonF>("approach1L");
    QTest::addColumn<QPolygonF>("approach2L");
    QTest::addColumn<QPolygonF>("approach1R");
    QTest::addColumn<QPolygonF>("approach2R");

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
    apronPolygon = apronPolygon.subtracted(standPolygon);

    QPolygonF approach1LPolygon;
    QPolygonF approach2LPolygon;
    QPolygonF approach1RPolygon;
    QPolygonF approach2RPolygon;

    QTest::newRow("Dummy Aerodrome")
        << runwayPolygon
        << taxiwayPolygon
        << apronPolygon
        << standPolygon
        << approach1LPolygon
        << approach2LPolygon
        << approach1RPolygon
        << approach2RPolygon;
}

void AerodromeTest::test()
{
    QFETCH(QPolygonF, runway);
    QFETCH(QPolygonF, taxiway);
    QFETCH(QPolygonF, apron);
    QFETCH(QPolygonF, stand);
    QFETCH(QPolygonF, approach1L);
    QFETCH(QPolygonF, approach2L);
    QFETCH(QPolygonF, approach1R);
    QFETCH(QPolygonF, approach2R);

    Aerodrome aerodrome;

    QVERIFY(aerodrome.hasAnyElements() == false);
    QVERIFY(aerodrome.hasAllElements() == false);

    aerodrome.addRunwayElement(runway);
    aerodrome.addTaxiwayElement(taxiway);

    QVERIFY(aerodrome.hasAnyElements() == true);
    QVERIFY(aerodrome.hasAllElements() == false);

    aerodrome.addApronElement(apron);
    aerodrome.addStandElement(stand);
    aerodrome.addStandElement(approach1L);
    aerodrome.addStandElement(approach2L);
    aerodrome.addStandElement(approach1R);
    aerodrome.addStandElement(approach2R);

    QVERIFY(aerodrome.hasAnyElements() == true);
    QVERIFY(aerodrome.hasAllElements() == true);
}

Aerodrome AerodromeTest::dummyAerodrome()
{
    Aerodrome aerodrome;

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
    apronPolygon = apronPolygon.subtracted(standPolygon);

    QPolygonF approach1LPolygon;
    QPolygonF approach2LPolygon;
    QPolygonF approach1RPolygon;
    QPolygonF approach2RPolygon;

    return aerodrome;
}

QTEST_APPLESS_MAIN(AerodromeTest)
#include "aerodrometest.moc"
