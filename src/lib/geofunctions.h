/*!
 * \file geofunctions.h
 * \brief Interface of a set of functions for converting between
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

#ifndef ASTMOPS_GEOFUNCTIONS_H
#define ASTMOPS_GEOFUNCTIONS_H

#include <QGeoCoordinate>
#include <QVector3D>
#include <QtMath>

constexpr double WGS84_A = 6378137.0;          // Semi-major axis of the ellipsoid, a [m].
constexpr double WGS84_E2 = 0.00669437999013;  // Eccentricity of the ellipsoid squared.
//constexpr double WGS84_IF = 298.257223563;                       // Inverse flattening of the ellipsoid.
//constexpr double WGS84_F = (1.0 / WGS84_IF);                     // Flattening of the ellipsoid.
//constexpr double WGS84_B = (WGS84_A * (1 - WGS84_F));            // Semi-minor axis of the ellipsoid, b [m].
//const double WGS84_E = (sqrt(2 * WGS84_F - WGS84_F * WGS84_F));  // Eccentricity of the ellipsoid.

double wgs84TransverseRadius(const double latGeoRad);
QVector3D geoToEcef(const QGeoCoordinate &llh);
QVector3D ecefToLocalEnu(const QVector3D ecef, const QGeoCoordinate &llhRef);
QVector3D geoToLocalEnu(const QGeoCoordinate &llh, const QGeoCoordinate &llhRef);
double dmsToDeg(const double deg, const double min, const double sec);
double dmsToDeg(const double deg, const double min, const double sec, const QString &hemisphere);

#endif  // ASTMOPS_GEOFUNCTIONS_H
