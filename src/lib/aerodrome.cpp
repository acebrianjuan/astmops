/*!
 * \file aerodrome.cpp
 * \brief Implementation of the Aerodrome class.
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

#include "aerodrome.h"
#include "astmops.h"
#include <QVector2D>

Aerodrome::Aerodrome(const QGeoCoordinate &arp) : arp_(arp)
{
}

void Aerodrome::setArp(const QGeoCoordinate &pt)
{
    arp_ = pt;
}

void Aerodrome::addSmr(Sic sic, QVector3D pt)
{
    smr_.insert(sic, pt);
}

void Aerodrome::addRunwayElement(const QString &name, const QPolygonF &pgn)
{
    Q_ASSERT(!pgn.isEmpty() && pgn.isClosed());
    runwayElements_[name] << pgn;
}

void Aerodrome::addTaxiwayElement(const QString &name, const QPolygonF &pgn)
{
    Q_ASSERT(!pgn.isEmpty() && pgn.isClosed());
    taxiwayElements_[name] << pgn;
}

void Aerodrome::addApronLaneElement(const QString &name, const QPolygonF &pgn)
{
    Q_ASSERT(!pgn.isEmpty() && pgn.isClosed());
    apronLaneElements_[name] << pgn;
}

void Aerodrome::addStandElement(const QString &name, const QPolygonF &pgn)
{
    Q_ASSERT(!pgn.isEmpty() && pgn.isClosed());
    standElements_[name] << pgn;
}

void Aerodrome::addAirborne1Element(const QString &name, const QPolygonF &pgn)
{
    Q_ASSERT(!pgn.isEmpty() && pgn.isClosed());
    airborne1Elements_[name] << pgn;
}

void Aerodrome::addAirborne2Element(const QString &name, const QPolygonF &pgn)
{
    Q_ASSERT(!pgn.isEmpty() && pgn.isClosed());
    airborne2Elements_[name] << pgn;
}

QGeoCoordinate Aerodrome::arp() const
{
    return arp_;
}

QHash<Sic, QVector3D> Aerodrome::smr() const
{
    return smr_;
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

Aerodrome::NamedArea Aerodrome::locatePoint(const QVector3D cartPos, const bool gndBit) const
{
    Q_ASSERT(hasAnyElements());  // Asserting for "any" elements is enough.
    // It should not be mandatory for an aerodrome to have "all" elements.

    QPointF pos2D = cartPos.toPointF();
    double hgt = cartPos.z();  // TODO: Revise altitude/height values in local radar cartesian frame.

    Area layer = gndBit ? Area::Ground     // GBS = 1
                        : Area::Airborne;  // GBS = 0

    if (layer == Area::Ground)
    {
        if (auto name = areasContainingPoint(runwayElements_, pos2D))
        {
            return NamedArea(Area::Runway, name.value());
        }

        if (auto name = areasContainingPoint(taxiwayElements_, pos2D))
        {
            return NamedArea(Area::Taxiway, name.value());
        }

        if (auto name = areasContainingPoint(apronLaneElements_, pos2D))
        {
            return NamedArea(Area::ApronLane, name.value());
        }

        if (auto name = areasContainingPoint(standElements_, pos2D))
        {
            return NamedArea(Area::Stand, name.value());
        }
    }
    else if (layer == Area::Airborne)
    {
        if (auto name = areasContainingPoint(airborne1Elements_, pos2D); name && hgt <= 762)
        {
            return NamedArea(Area::Airborne1, name.value());
        }

        if (auto name = areasContainingPoint(airborne2Elements_, pos2D); name && hgt <= 762)
        {
            return NamedArea(Area::Airborne2, name.value());
        }
    }

    return NamedArea();
}

bool Aerodrome::collectionContainsPoint(const Polygons &col, QPointF pt) const
{
    for (const QPolygonF &element : col)
    {
        if (element.containsPoint(pt, Qt::FillRule::OddEvenFill))
        {
            return true;
        }
    }

    return false;
}

bool Aerodrome::collectionContainsPoint(const QHash<QString, Polygons> &col, QPointF pt) const
{
    for (auto it = col.begin(); it != col.end(); ++it)
    {
        Polygons pgns = it.value();

        if (collectionContainsPoint(pgns, pt))
        {
            return true;
        }
    }

    return false;
}

std::optional<QString> Aerodrome::areasContainingPoint(const QHash<QString, Polygons> &col, QPointF point) const
{
    for (auto it = col.begin(); it != col.end(); ++it)
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
