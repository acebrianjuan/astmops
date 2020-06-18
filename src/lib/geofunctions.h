/*!
 * \file geofunctions.h
 * \brief Interface of a set of functions for converting between
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

#ifndef ASTMOPS_GEOFUNCTIONS_H
#define ASTMOPS_GEOFUNCTIONS_H

#include <QGeoCoordinate>
#include <QVector3D>
#include <QtMath>

constexpr double WGS84_A = 6378137.0;                            // Semi-major axis of the Earth, a [m].
constexpr double WGS84_IF = 298.257223563;                       // Inverse flattening of the Earth.
constexpr double WGS84_F = (1.0 / WGS84_IF);                     // Flattening of the Earth.
constexpr double WGS84_B = (WGS84_A * (1 - WGS84_F));            // Semi-minor axis of the Earth [m].
const double WGS84_E = (sqrt(2 * WGS84_F - WGS84_F * WGS84_F));  // Eccentricity of the Earth.

double wgs84TransverseRadius(double latGeoRad);
QVector3D geoToEcef(QGeoCoordinate llh);
QVector3D ecefToLocalEnu(QVector3D ecef, QGeoCoordinate llhRef);
QVector3D geoToLocalEnu(QGeoCoordinate geo, QGeoCoordinate geoRef);

#endif  // ASTMOPS_GEOFUNCTIONS_H
