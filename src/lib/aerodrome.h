/*!
 * \file aerodrome.h
 * \brief Interface of the Aerodrome class.
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

#ifndef ASTMOPS_AERODROME_H
#define ASTMOPS_AERODROME_H

#include "astmops.h"
#include <QGeoCoordinate>
#include <QHash>
#include <QMetaEnum>
#include <QPolygonF>
#include <QVector3D>
#include <QVector>
#include <optional>

/*!
 * \brief The Aerodrome class contains the geometric assets that define an
 * aerodrome expressed in cartesian coordinates relative to the ARP.
 *
 * Contains all the assets related to an aerodrome, among which are the
 * coordinates of the different physical elements that form it such as
 * runways, taxiways, aprons, stands as well as non-physical elements
 * like airborne areas.
 */
class Aerodrome
{
    Q_GADGET

public:
    using Polygons = QVector<QPolygonF>;

    enum Area
    {
        None = 0,

        Airborne1 = 0b0000'0000'0000'0001,
        Airborne2 = 0b0000'0000'0000'0010,
        Airborne = Airborne1 | Airborne2,

        Runway = 0b0000'0001'0000'0000,
        Taxiway = 0b0000'0010'0000'0000,
        Manoeuvering = Runway | Taxiway,

        Stand = 0b0001'0000'0000'0000,
        ApronLane = 0b0010'0000'0000'0000,
        Apron = Stand | ApronLane,

        Movement = Manoeuvering | Apron,
        Ground = Movement,

        All = Airborne | Ground,
    };
    Q_ENUM(Area);
    Q_DECLARE_FLAGS(Areas, Area);
    Q_FLAG(Areas);

    struct NamedArea
    {
        NamedArea() = default;

        explicit NamedArea(const Aerodrome::Area area)
            : area_(area)
        {
        }

        NamedArea(const Aerodrome::Area area, const QString &name)
            : area_(area), name_(name)
        {
        }

        QString fullName() const
        {
            QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();

            QString fn = QLatin1String(e.valueToKey(area_));
            if (!name_.isEmpty())
            {
                fn.append(QLatin1Char('_'));
                fn.append(name_);
            }

            return fn;
        }

        Aerodrome::Area area_ = Aerodrome::Area::None;
        QString name_;
    };

    Aerodrome() = default;
    Aerodrome(const QGeoCoordinate &arp);

    void setArp(const QGeoCoordinate &pt);

    void addSmr(Sic sic, QVector3D pt);
    void addRunwayElement(const QString &name, const QPolygonF &pgn);
    void addTaxiwayElement(const QString &name, const QPolygonF &pgn);
    void addApronLaneElement(const QString &name, const QPolygonF &pgn);
    void addStandElement(const QString &name, const QPolygonF &pgn);
    void addAirborne1Element(const QString &name, const QPolygonF &pgn);
    void addAirborne2Element(const QString &name, const QPolygonF &pgn);

    QGeoCoordinate arp() const;
    QHash<Sic, QVector3D> smr() const;

    bool hasAnyElements() const;
    bool hasAllElements() const;
    NamedArea locatePoint(const QVector3D cartPos, const bool gndBit) const;

private:
    bool collectionContainsPoint(const Polygons &col, QPointF pt) const;
    bool collectionContainsPoint(const QHash<QString, Polygons> &col, QPointF pt) const;
    std::optional<QString> areasContainingPoint(const QHash<QString, Polygons> &col, QPointF pt) const;

    QGeoCoordinate arp_;
    QHash<Sic, QVector3D> smr_;

    // TODO: Consider merging all the elements into a single AreaHash container.
    QHash<QString, Polygons> runwayElements_;
    QHash<QString, Polygons> taxiwayElements_;
    QHash<QString, Polygons> apronLaneElements_;
    QHash<QString, Polygons> standElements_;
    QHash<QString, Polygons> airborne1Elements_;
    QHash<QString, Polygons> airborne2Elements_;
};

Q_DECLARE_METATYPE(Aerodrome);
Q_DECLARE_METATYPE(Aerodrome::Area);
Q_DECLARE_METATYPE(Aerodrome::NamedArea);


#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(const Aerodrome::NamedArea &narea, uint seed);
#else
size_t qHash(const Aerodrome::NamedArea &narea, size_t seed);
#endif

bool operator==(const Aerodrome::NamedArea &lhs, const Aerodrome::NamedArea &rhs);
bool operator!=(const Aerodrome::NamedArea &lhs, const Aerodrome::NamedArea &rhs);


bool areaBelongsToAreaGroup(Aerodrome::Area area, Aerodrome::Area group);
bool areaBelongsToAreaGroup(const Aerodrome::NamedArea &narea, Aerodrome::Area group);

#endif  // ASTMOPS_AERODROME_H
