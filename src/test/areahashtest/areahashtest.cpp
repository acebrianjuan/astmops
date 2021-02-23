/*!
 * \file areahashtest.cpp
 * \brief Implements unit tests for the AreaHash container.
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

#include "areahash.h"
#include <QObject>
#include <QtTest>

class AreaHashTest : public QObject
{
    Q_OBJECT

private slots:
    void test();
};

void AreaHashTest::test()
{
    AreaHash<int> areaHash;

    Aerodrome::NamedArea runway1 = Aerodrome::NamedArea(Aerodrome::Area::Runway, QLatin1String("18/36"));
    Aerodrome::NamedArea runway2 = Aerodrome::NamedArea(Aerodrome::Area::Runway, QLatin1String("09/27"));
    Aerodrome::NamedArea taxiway = Aerodrome::NamedArea(Aerodrome::Area::Taxiway);
    Aerodrome::NamedArea apronLane = Aerodrome::NamedArea(Aerodrome::Area::ApronLane);
    Aerodrome::NamedArea stand = Aerodrome::NamedArea(Aerodrome::Area::Stand);
    Aerodrome::NamedArea airborne1 = Aerodrome::NamedArea(Aerodrome::Area::Airborne1);
    Aerodrome::NamedArea airborne2 = Aerodrome::NamedArea(Aerodrome::Area::Airborne2);

    areaHash.insert(runway1, 100);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::All).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne1).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne2).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Ground).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Movement).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Manoeuvering).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Runway).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Taxiway).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Apron).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::ApronLane).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Stand).size() == 0);

    areaHash.insert(runway2, 100);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::All).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne1).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne2).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Ground).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Movement).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Manoeuvering).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Runway).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Taxiway).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Apron).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::ApronLane).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Stand).size() == 0);

    areaHash.insert(taxiway, 100);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::All).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne1).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne2).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Ground).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Movement).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Manoeuvering).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Runway).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Taxiway).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Apron).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::ApronLane).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Stand).size() == 0);

    areaHash.insert(apronLane, 100);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::All).size() == 4);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne1).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne2).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Ground).size() == 4);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Movement).size() == 4);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Manoeuvering).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Runway).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Taxiway).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Apron).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::ApronLane).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Stand).size() == 0);

    areaHash.insert(stand, 100);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::All).size() == 5);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne1).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne2).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Ground).size() == 5);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Movement).size() == 5);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Manoeuvering).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Runway).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Taxiway).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Apron).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::ApronLane).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Stand).size() == 1);

    areaHash.insert(airborne1, 100);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::All).size() == 6);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne1).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne2).size() == 0);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Ground).size() == 5);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Movement).size() == 5);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Manoeuvering).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Runway).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Taxiway).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Apron).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::ApronLane).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Stand).size() == 1);

    areaHash.insert(airborne2, 100);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::All).size() == 7);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne1).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Airborne2).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Ground).size() == 5);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Movement).size() == 5);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Manoeuvering).size() == 3);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Runway).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Taxiway).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Apron).size() == 2);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::ApronLane).size() == 1);
    QVERIFY(areaHash.findByArea(Aerodrome::Area::Stand).size() == 1);
}

QTEST_APPLESS_MAIN(AreaHashTest)
#include "areahashtest.moc"
