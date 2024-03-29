/*!
 * \file geofunctions.cpp
 * \brief Implementation of a set of functions for converting between
 * different coordinate systems.
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

#include "geofunctions.h"
#include <QDebug>
#include <QRegularExpression>

/*!
 * \brief Computes the prime vertical radius of curvature for the given
 * latitude.
 *
 * \param latGeoRad Latitude in radians.
 *
 * \return Prime vertical radius of curvature in meters.
 *
 * The prime vertical radius of curvature (also called transverse radius of
 * curvature or normal earth radius level) equals to the distance between the
 * surface of the earth ellipsoid at latitude of reference point and the
 * intersection between the perpendicular to the earth ellipsoid tangent and
 * the vertical joining the poles.
 */
double wgs84TransverseRadius(const double latGeoRad)
{
    double N = WGS84_A / sqrt(1 - WGS84_E2 * sin(latGeoRad) * sin(latGeoRad));
    return N;
}

/*!
 * \brief Converts from geographic to ECEF coordinates.
 *
 * \param llh QGeoCoordinate object with the geographic coordinates to be
 * converted. Latitude in degrees, longitude in degrees and height in meters.
 *
 * \return QVector3D object with the converted XYZ ECEF coordinates in meters.
 */
QVector3D geoToEcef(const QGeoCoordinate &llh)
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

/*!
 * \brief Converts from ECEF to local ENU coordinates.
 *
 * \param ecef QVector3D object with the XYZ ECEF coordinates to be converted
 * in meters.
 *
 * \param llhRef QGeoCoordinate object with the geographic coordinates of the
 * local tangent plane origin. Latitude in degrees, longitude in degrees and
 * height in meters.
 *
 * \return QVector3D object with the converted XYZ local ENU coordinates in
 * meters.
 */
QVector3D ecefToLocalEnu(const QVector3D ecef, const QGeoCoordinate &llhRef)
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

/*!
 * \brief Converts from geographic to local ENU coordinates.
 *
 * \param llh QGeoCoordinate object with the geographic coordinates to be
 * converted. Latitude in degrees, longitude in degrees and height in meters.
 *
 * \param llhRef QGeoCoordinate object with the geographic coordinates of the
 * local tangent plane origin. Latitude in degrees, longitude in degrees and
 * height in meters.
 *
 * \return QVector3D object with the converted XYZ local ENU coordinates in
 * meters.
 */
QVector3D geoToLocalEnu(const QGeoCoordinate &llh, const QGeoCoordinate &llhRef)
{
    QVector3D ecef = geoToEcef(llh);
    QVector3D enu = ecefToLocalEnu(ecef, llhRef);
    return enu;
}

double dmsToDeg(const double deg, const double min, const double sec)
{
    double d = deg + min / 60.0 + sec / 3600.0;
    return d;
}

double dmsToDeg(const double deg, const double min, const double sec, const QString &hemisphere)
{
    if (hemisphere.size() != 1)
    {
        qDebug() << "Invalid hemisphere indicator:" << hemisphere;
        return qQNaN();
    }

    if (!hemisphere.at(0).isLetter())
    {
        qDebug() << "Invalid hemisphere indicator:" << hemisphere;
        return qQNaN();
    }

    // TODO: Consider making regEx object static for performance.
    QRegularExpression regEx(QLatin1String("[NSEW]"), QRegularExpression::CaseInsensitiveOption);
    if (!regEx.match(hemisphere).hasMatch())
    {
        qDebug() << "Invalid hemisphere indicator:" << hemisphere;
        return qQNaN();
    }

    double d = dmsToDeg(deg, min, sec);

    regEx.setPattern(QLatin1String("[SW]"));
    if (regEx.match(hemisphere).hasMatch())
    {
        d = -d;
    }

    return d;
}
