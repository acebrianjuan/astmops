/*!
 * \file aerodrome.cpp
 * \brief Implementation of the Aerodrome class.
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
#include "astmops.h"
#include <QVector2D>

Aerodrome::Aerodrome(const QGeoCoordinate &arp) : arp_(arp)
{
}

bool Aerodrome::hasAnyElements() const
{
    if (!runwayElements_.isEmpty() || !taxiwayElements_.isEmpty() ||
        !apronLaneElements_.isEmpty() || !standElements_.isEmpty() ||
        !airborne1Elements_.isEmpty() || !airborne2Elements_.isEmpty())
    {
        return true;
    }
    return false;
}

bool Aerodrome::hasAllElements() const
{
    if (!runwayElements_.isEmpty() && !taxiwayElements_.isEmpty() &&
        !apronLaneElements_.isEmpty() && !standElements_.isEmpty() &&
        !airborne1Elements_.isEmpty() && !airborne2Elements_.isEmpty())
    {
        return true;
    }
    return false;
}

void Aerodrome::setArp(const QGeoCoordinate &point)
{
    arp_ = point;
}

void Aerodrome::addSmr(Sic sic, QVector3D point)
{
    smr_.insert(sic, point);
}

void Aerodrome::addRunwayElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    runwayElements_[name] << polygon;
}

void Aerodrome::addTaxiwayElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    taxiwayElements_[name] << polygon;
}

void Aerodrome::addApronLaneElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    apronLaneElements_[name] << polygon;
}

void Aerodrome::addStandElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    standElements_[name] << polygon;
}

void Aerodrome::addAirborne1Element(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    airborne1Elements_[name] << polygon;
}

void Aerodrome::addAirborne2Element(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    airborne2Elements_[name] << polygon;
}

Aerodrome::NamedArea Aerodrome::locatePoint(const QVector3D cartPos, const bool gndBit)
{
    Q_ASSERT(hasAnyElements());  // Asserting for "any" elements is enough.
    // It should not be mandatory for an aerodrome to have "all" elements.

    QPointF pos2D = cartPos.toPointF();
    double alt = cartPos.z();  // TODO: Revise altitude/height values in local radar cartesian frame.

    Layer layer = gndBit ? Layer::SurfaceLayer    // GBS = 1
                         : Layer::AirborneLayer;  // GBS = 0
    /*
    else
    {
        layer = alt > 0 ? Layer::AirborneLayer
                        : Layer::GroundLayer;
    }*/

    if (layer == Layer::SurfaceLayer)
    {
        if (auto name = areaContainsPoint(runwayElements_, pos2D))
        {
            return NamedArea(Area::Runway, name.value());
        }

        if (auto name = areaContainsPoint(taxiwayElements_, pos2D))
        {
            return NamedArea(Area::Taxiway, name.value());
        }

        if (auto name = areaContainsPoint(apronLaneElements_, pos2D))
        {
            return NamedArea(Area::ApronLane, name.value());
        }

        if (auto name = areaContainsPoint(standElements_, pos2D))
        {
            return NamedArea(Area::Stand, name.value());
        }
    }
    else if (layer == Layer::AirborneLayer)
    {
        if (auto name = areaContainsPoint(airborne1Elements_, pos2D); name && alt <= 762)
        {
            return NamedArea(Area::Airborne1, name.value());
        }

        if (auto name = areaContainsPoint(airborne2Elements_, pos2D); name && alt <= 762)
        {
            return NamedArea(Area::Airborne2, name.value());
        }
    }

    return NamedArea();
}

bool Aerodrome::collectionContainsPoint(const QVector<QPolygonF> &collection, QPointF point) const
{
    for (const QPolygonF &element : collection)
    {
        if (element.containsPoint(point, Qt::FillRule::OddEvenFill))
        {
            return true;
        }
    }

    return false;
}

bool Aerodrome::collectionContainsPoint(const QHash<QString, QVector<QPolygonF>> &collection, QPointF point) const
{
    for (auto it = collection.begin(); it != collection.end(); ++it)
    {
        QVector<QPolygonF> vec = it.value();

        if (collectionContainsPoint(vec, point))
        {
            return true;
        }
    }

    return false;
}

std::optional<QString> Aerodrome::areaContainsPoint(const QHash<QString, QVector<QPolygonF>> &collection, QPointF point) const
{
    for (auto it = collection.begin(); it != collection.end(); ++it)
    {
        QVector<QPolygonF> vec = it.value();

        if (collectionContainsPoint(vec, point))
        {
            return it.key();
        }
    }

    return std::nullopt;
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(const Aerodrome::NamedArea &narea, uint seed)
#else
size_t qHash(const Aerodrome::NamedArea &narea, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, narea.name_);
    seed = hash(seed, narea.area_);
    return seed;
}

bool operator==(const Aerodrome::NamedArea &lhs, const Aerodrome::NamedArea &rhs)
{
    return lhs.area_ == rhs.area_ &&
           lhs.name_ == rhs.name_;
}

bool operator!=(const Aerodrome::NamedArea &lhs, const Aerodrome::NamedArea &rhs)
{
    return !(lhs == rhs);
}

bool areaBelongsToAreaGroup(Aerodrome::Area area, Aerodrome::Area group)
{
    if (area != Aerodrome::Area::None && (area | group) == group)
    {
        return true;
    }

    return false;
}

bool areaBelongsToAreaGroup(const Aerodrome::NamedArea &narea, Aerodrome::Area group)
{
    return areaBelongsToAreaGroup(narea.area_, group);
}
