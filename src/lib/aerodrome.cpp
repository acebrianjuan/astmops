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
        !m_apronElements.isEmpty() || !m_standElements.isEmpty() ||
        !m_airborne1Elements.isEmpty() || !m_airborne2Elements.isEmpty())
    {
        return true;
    }
    return false;
}

bool Aerodrome::hasAllElements()
{
    if (!m_runwayElements.isEmpty() && !m_taxiwayElements.isEmpty() &&
        !m_apronElements.isEmpty() && !m_standElements.isEmpty() &&
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

void Aerodrome::addApronElement(const QString &name, const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_apronElements[name] << polygon;
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

Aerodrome::Area Aerodrome::locatePoint(const QVector3D cartPos, const std::optional<bool> &gndBit)
{
    Q_ASSERT(hasAnyElements());

    QPointF pos2D = cartPos.toPointF();
    double alt = cartPos.z();  // TODO: Revise altitude/height values in local radar cartesian frame.

    Layer layer = UnknownLayer;

    if (gndBit.has_value())
    {
        layer = gndBit.value() ? Layer::GroundLayer
                               : Layer::AirborneLayer;
    }
    else
    {
        layer = alt > 0 ? Layer::AirborneLayer
                        : Layer::GroundLayer;
    }

    // TODO: Consider returning a NamedArea instead of an Aerodrome::Area.
    if (layer == Layer::GroundLayer)
    {
        if (collectionContainsPoint(m_runwayElements, pos2D))
        {
            return Area::Runway;
        }
        else if (collectionContainsPoint(m_taxiwayElements, pos2D))
        {
            return Area::Taxiway;
        }
        else if (collectionContainsPoint(m_apronElements, pos2D))
        {
            return Area::Apron;
        }
        else if (collectionContainsPoint(m_standElements, pos2D))
        {
            return Area::Stand;
        }
    }
    else if (layer == Layer::AirborneLayer)
    {
        if (collectionContainsPoint(m_airborne1Elements, pos2D) && alt <= 762)
        {
            return Area::Airborne1;
        }
        else if (collectionContainsPoint(m_airborne2Elements, pos2D) && alt <= 762)
        {
            return Area::Airborne2;
        }
    }

    return Area::None;
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
