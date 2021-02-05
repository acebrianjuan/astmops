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
        !m_approach1Elements.isEmpty() || !m_approach2Elements.isEmpty())
    {
        return true;
    }
    return false;
}

bool Aerodrome::hasAllElements()
{
    if (!m_runwayElements.isEmpty() && !m_taxiwayElements.isEmpty() &&
        !m_apronElements.isEmpty() && !m_standElements.isEmpty() &&
        !m_approach1Elements.isEmpty() && !m_approach2Elements.isEmpty())
    {
        return true;
    }
    return false;
}

void Aerodrome::setArp(QVector3D point)
{
    m_arp = point;
}

void Aerodrome::addRunwayElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_runwayElements.append(polygon);
}

void Aerodrome::addTaxiwayElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_taxiwayElements.append(polygon);
}

void Aerodrome::addApronElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_apronElements.append(polygon);
}

void Aerodrome::addStandElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_standElements.append(polygon);
}

void Aerodrome::addApproach1Element(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_approach1Elements.append(polygon);
}

void Aerodrome::addApproach2Element(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    m_approach2Elements.append(polygon);
}

Aerodrome::Area Aerodrome::locatePoint(const QVector3D cartPos, const std::optional<bool> &gndBit)
{
    Q_ASSERT(hasAnyElements());

    QPointF pos2D = cartPos.toPointF();
    double alt = cartPos.z();

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
        if (collectionContainsPoint(m_approach1Elements, pos2D))
        {
            return Area::Approach1;
        }
        else if (collectionContainsPoint(m_approach2Elements, pos2D))
        {
            return Area::Approach2;
        }
        else
        {
            return Area::Airborne;
        }
    }

    return Area::None;
}

bool Aerodrome::collectionContainsPoint(const QVector<QPolygonF> &collection, QPointF point)
{
    //Q_ASSERT(!collection.isEmpty());

    for (const QPolygonF &element : collection)
    {
        if (element.containsPoint(point, Qt::FillRule::OddEvenFill))
        {
            return true;
        }
    }
    return false;
}

QVector<QPolygonF> Aerodrome::getRunwayElements() const
{
    return m_runwayElements;
}

QVector<QPolygonF> Aerodrome::getTaxiwayElements() const
{
    return m_taxiwayElements;
}

QVector<QPolygonF> Aerodrome::getApronElements() const
{
    return m_apronElements;
}

QVector<QPolygonF> Aerodrome::getStandElements() const
{
    return m_standElements;
}

QVector<QPolygonF> Aerodrome::getApproach1Elements() const
{
    return m_approach1Elements;
}

QVector<QPolygonF> Aerodrome::getApproach2Elements() const
{
    return m_approach2Elements;
}
