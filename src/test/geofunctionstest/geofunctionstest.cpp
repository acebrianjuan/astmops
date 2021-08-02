/*!
 * \file geofunctionstest.cpp
 * \brief Implements unit tests for the coordinate system conversion
 * functions.
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

#include "geofunctions.h"
#include <QGeoCoordinate>
#include <QObject>
#include <QtTest>

class GeoFunctionsTest : public QObject
{
    Q_OBJECT

private slots:

    void wgs84TransverseRadiusTest_data();
    void wgs84TransverseRadiusTest();

    void geoToEcefTest_data();
    void geoToEcefTest();

    void ecefToLocalEnuTest_data();
    void ecefToLocalEnuTest();

    void geoToLocalEcefTest_data();
    void geoToLocalEcefTest();

    // TODO: Implement missing tests.
    //void dmsToDegTest_data();
    //void dmsToDegTest();
};

void GeoFunctionsTest::wgs84TransverseRadiusTest_data()
{
    QTest::addColumn<double>("latRadIn");
    QTest::addColumn<double>("radiusOut");

    QTest::newRow("Equator (Lat: 0º)") << 0.0 << WGS84_A;
    double M_90 = WGS84_A / sqrt(1 - WGS84_E2);
    QTest::newRow("Poles (Lat: 90º)") << M_PI_2 << M_90;

    /* Reference:
     *
     * Krakiwsky, E. J., & Thomson, D. B. (1974).
     * Geodetic position computations (No. NB-DSE-TR-39).
     * Department of Surveying Engineering, University of New Brunswick.
     *
     * https://www2.unb.ca/gge/Pubs/LN39.pdf
     */
}

void GeoFunctionsTest::wgs84TransverseRadiusTest()
{
    QFETCH(double, latRadIn);
    QFETCH(double, radiusOut);

    QCOMPARE(wgs84TransverseRadius(latRadIn), radiusOut);
}

void GeoFunctionsTest::geoToEcefTest_data()
{
    QTest::addColumn<QGeoCoordinate>("geoIn");
    QTest::addColumn<QVector3D>("ecefOutValid");

    QGeoCoordinate leblArpGeo(41.297076579982225, 2.0784629201158662, 4.3200000000000003);
    QVector3D leblArpEcef(4795728, 174046, 4187269);

    QTest::newRow("LEBL_ARP") << leblArpGeo << leblArpEcef;
}

void GeoFunctionsTest::geoToEcefTest()
{
    QFETCH(QGeoCoordinate, geoIn);
    QFETCH(QVector3D, ecefOutValid);

    QVector3D ecefOut = geoToEcef(geoIn);

    /*
     * TODO: Consider checking the error between expected and actual values
     * using QVERIFY, instead of checking for an exact match of the values.
     */
    QCOMPARE(ecefOut.x(), ecefOutValid.x());
    QCOMPARE(ecefOut.y(), ecefOutValid.y());
    QCOMPARE(ecefOut.z(), ecefOutValid.z());
}

void GeoFunctionsTest::ecefToLocalEnuTest_data()
{
    QTest::addColumn<QGeoCoordinate>("geoRef");
    QTest::addColumn<QVector3D>("ecefIn");
    QTest::addColumn<QVector3D>("enuOutValid");

    QGeoCoordinate leblArpGeo(41.297076579982225, 2.0784629201158662, 4.3200000000000003);
    QVector3D leblArpEcef(4795728, 174046, 4187269);
    QVector3D leblArpEnu(0, 0, 0);

    QGeoCoordinate leblSmrGeo(41.29561944, 2.095113889, 4.3200000000000003);
    //QVector3D leblSmrEcef(000, 000, 000);
    //QVector3D leblSmrEnu(1394.60223, -162.34771, 0);

    QTest::newRow("LEBL_ARP") << leblArpGeo << leblArpEcef << leblArpEnu;
    //QTest::newRow("LEBL_SMR") << leblArpGeo << leblSmrEcef << leblSmrEnu;
}

void GeoFunctionsTest::ecefToLocalEnuTest()
{
    QFETCH(QGeoCoordinate, geoRef);
    QFETCH(QVector3D, ecefIn);
    QFETCH(QVector3D, enuOutValid);

    QVector3D enuOut = ecefToLocalEnu(ecefIn, geoRef);

    QVERIFY(std::fabs(enuOut.x() - enuOutValid.x() < 1));
    QVERIFY(std::fabs(enuOut.y() - enuOutValid.y() < 1));
    QVERIFY(std::fabs(enuOut.z() - enuOutValid.z() < 1));
}

void GeoFunctionsTest::geoToLocalEcefTest_data()
{
    QTest::addColumn<QGeoCoordinate>("geoRef");
    QTest::addColumn<QGeoCoordinate>("geoIn");
    QTest::addColumn<QVector3D>("enuOutValid");

    QGeoCoordinate leblArpGeo(41.297076579982225, 2.0784629201158662, 4.3200000000000003);
    QVector3D leblArpEnu(0, 0, 0);

    QGeoCoordinate leblSmrGeo(41.29561944, 2.095113889, 4.3200000000000003);
    QVector3D leblSmrEnu(1394.60223, -162.34771, 0);

    QTest::newRow("LEBL_ARP") << leblArpGeo << leblArpGeo << leblArpEnu;
    QTest::newRow("LEBL_SMR") << leblArpGeo << leblSmrGeo << leblSmrEnu;
}

void GeoFunctionsTest::geoToLocalEcefTest()
{
    QFETCH(QGeoCoordinate, geoRef);
    QFETCH(QGeoCoordinate, geoIn);
    QFETCH(QVector3D, enuOutValid);

    QVector3D enuOut = geoToLocalEnu(geoIn, geoRef);

    QVERIFY(std::fabs(enuOut.x() - enuOutValid.x() < 1));
    QVERIFY(std::fabs(enuOut.y() - enuOutValid.y() < 1));
}

QTEST_APPLESS_MAIN(GeoFunctionsTest)
#include "geofunctionstest.moc"
