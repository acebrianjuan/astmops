/*!
 * \file geofunctions.cpp
 * \brief Implementation of a set of functions for converting between
 * different coordinate systems.
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

double wgs84TransverseRadius(double latGeoRad)
{
    double N = WGS84_A / sqrt(1 - WGS84_E2 * sin(latGeoRad) * sin(latGeoRad));
    return N;
}

QVector3D geoToEcef(QGeoCoordinate llh)
{
    double phi = qDegreesToRadians(llh.latitude());
    double lambda = qDegreesToRadians(llh.longitude());
    double h = llh.altitude();

    double sinPhi = sin(phi);
    double cosPhi = cos(phi);

    double sinLambda = sin(lambda);
    double cosLambda = cos(lambda);

    double N = wgs84TransverseRadius(phi);

    double x = (h + N) * cosPhi * cosLambda;
    double y = (h + N) * cosPhi * sinLambda;
    double z = (h + (1 - WGS84_E2) * N) * sinPhi;

    QVector3D ecef(x, y, z);
    return ecef;
}

QVector3D ecefToLocalEnu(QVector3D ecef, QGeoCoordinate llhRef)
{
    QVector3D ecefRef = geoToEcef(llhRef);

    double phi = qDegreesToRadians(llhRef.latitude());
    double lambda = qDegreesToRadians(llhRef.longitude());

    double sinPhi = sin(phi);
    double cosPhi = cos(phi);

    double sinLambda = sin(lambda);
    double cosLambda = cos(lambda);

    double xd = ecef.x() - ecefRef.x();
    double yd = ecef.y() - ecefRef.y();
    double zd = ecef.z() - ecefRef.z();

    // Matrix multiplication.
    double xEast = -sinLambda * xd + cosLambda * yd;
    double yNorth = -sinPhi * cosLambda * xd - sinPhi * sinLambda * yd + cosPhi * zd;
    double zUp = cosPhi * cosLambda * xd + cosPhi * sinLambda * yd + sinPhi * zd;

    QVector3D enu(xEast, yNorth, zUp);
    return enu;
}

QVector3D geoToLocalEnu(QGeoCoordinate geo, QGeoCoordinate geoRef)
{
    QVector3D ecef = geoToEcef(geo);
    QVector3D enu = ecefToLocalEnu(ecef, geoRef);
    return enu;
}
