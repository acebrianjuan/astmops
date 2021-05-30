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

Aerodrome::Aerodrome()
{
}

bool Aerodrome::hasAnyElements()
{
    if (!m_runwayElements.isEmpty() || !m_taxiwayElements.isEmpty() ||
        !m_apronLaneElements.isEmpty() || !m_standElements.isEmpty() ||
        !m_airborne1Elements.isEmpty() || !m_airborne2Elements.isEmpty())
    {
        return true;
    }
    return false;
}

bool Aerodrome::hasAllElements()
{
    if (!m_runwayElements.isEmpty() && !m_taxiwayElements.isEmpty() &&
        !m_apronLaneElements.isEmpty() && !m_standElements.isEmpty() &&
        !m_airborne1Elements.isEmpty() && !m_airborne2Elements.isEmpty())
    {
        return true;
    }
    return false;
}

void Aerodrome::setArp(QVector3D point)
{
    m_arp = point;
}

void Aerodrome::addRunwayElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_runwayElements[name] << polygon;
}

void Aerodrome::addTaxiwayElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_taxiwayElements[name] << polygon;
}

void Aerodrome::addApronLaneElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_apronLaneElements[name] << polygon;
}

void Aerodrome::addStandElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_standElements[name] << polygon;
}

void Aerodrome::addAirborne1Element(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_airborne1Elements[name] << polygon;
}

void Aerodrome::addAirborne2Element(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_airborne2Elements[name] << polygon;
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
        if (auto name = areaContainsPoint(m_runwayElements, pos2D))
        {
            return NamedArea(Area::Runway, name.value());
        }

        if (auto name = areaContainsPoint(m_taxiwayElements, pos2D))
        {
            return NamedArea(Area::Taxiway, name.value());
        }

        if (auto name = areaContainsPoint(m_apronLaneElements, pos2D))
        {
            return NamedArea(Area::ApronLane, name.value());
        }

        if (auto name = areaContainsPoint(m_standElements, pos2D))
        {
            return NamedArea(Area::Stand, name.value());
        }
    }
    else if (layer == Layer::AirborneLayer)
    {
        if (auto name = areaContainsPoint(m_airborne1Elements, pos2D); name && alt <= 762)
        {
            return NamedArea(Area::Airborne1, name.value());
        }

        if (auto name = areaContainsPoint(m_airborne2Elements, pos2D); name && alt <= 762)
        {
            return NamedArea(Area::Airborne2, name.value());
        }
    }

    return NamedArea();
}

bool Aerodrome::collectionContainsPoint(const QVector<QPolygonF> &collection, QPointF point)
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

bool Aerodrome::collectionContainsPoint(const QHash<QString, QVector<QPolygonF>> &collection, QPointF point)
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

std::optional<QString> Aerodrome::areaContainsPoint(const QHash<QString, QVector<QPolygonF>> &collection, QPointF point)
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

bool operator==(const Aerodrome::NamedArea &a, const Aerodrome::NamedArea &b)
{
    return a.area_ == b.area_ &&
           a.name_ == b.name_;
}
