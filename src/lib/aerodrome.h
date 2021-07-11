/*!
 * \file aerodrome.h
 * \brief Interface of the Aerodrome class.
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

#ifndef ASTMOPS_AERODROME_H
#define ASTMOPS_AERODROME_H

#include "astmops.h"
#include <QGeoCoordinate>
#include <QHash>
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
    // TODO: Revise enum values.
    enum Area
    {
        None = 0,

        //Enroute = 0b0000'0000'0000'0001,
        Airborne1 = 0b0000'0000'0000'0010,
        Airborne2 = 0b0000'0000'0000'0100,
        //Airborne = 0b0000'0000'0000'0110,
        //    Airborne          = Airborne1|Airborne2,

        Airborne = 0b0000'0000'0000'0111,
        //    Airborne          = Airborne|Enroute;
        //    Airborne          = Airborne1|Airborne2|Enroute;


        Runway = 0b0000'0001'0000'0000,
        Taxiway = 0b0000'0010'0000'0000,
        Manoeuvering = 0b0000'0011'0000'0000,
        //    Manoeuvering      = Runway|Taxiway,

        Stand = 0b0001'0000'0000'0000,
        ApronLane = 0b0010'0000'0000'0000,
        Apron = 0b0011'0000'0000'0000,
        //    Apron             = Stand|ApronLane,

        Movement = 0b0011'0011'0000'0000,
        //    MovementArea      = Manoeuvering|Apron,

        Ground = 0b0011'0011'0000'0000,
        //    Ground            = MovementArea,

        All = 0b0011'0011'0000'0111,
        //    All               = Airborne|Ground,
    };

    Q_ENUM(Area);
    Q_DECLARE_FLAGS(Areas, Area);
    Q_FLAG(Areas);

    struct NamedArea
    {
        NamedArea() = default;

        NamedArea(const Aerodrome::Area area)
            : area_(area)
        {
        }

        NamedArea(const Aerodrome::Area area, const QString &name)
            : area_(area), name_(name)
        {
        }

        Aerodrome::Area area_ = Aerodrome::Area::None;
        QString name_;
    };

    Aerodrome() = default;
    Aerodrome(const QGeoCoordinate &arp);

    bool hasAnyElements() const;
    bool hasAllElements() const;

    void setArp(const QGeoCoordinate &point);

    void addSmr(Sic sic, QVector3D point);
    void addRunwayElement(const QString &name, const QPolygonF &polygon);
    void addTaxiwayElement(const QString &name, const QPolygonF &polygon);
    void addApronLaneElement(const QString &name, const QPolygonF &polygon);
    void addStandElement(const QString &name, const QPolygonF &polygon);
    void addAirborne1Element(const QString &name, const QPolygonF &polygon);
    void addAirborne2Element(const QString &name, const QPolygonF &polygon);

    NamedArea locatePoint(const QVector3D cartPos, const bool gndBit);

private:
    bool collectionContainsPoint(const QVector<QPolygonF> &collection, QPointF point) const;
    bool collectionContainsPoint(const QHash<QString, QVector<QPolygonF>> &collection, QPointF point) const;
    std::optional<QString> areaContainsPoint(const QHash<QString, QVector<QPolygonF>> &collection, QPointF point) const;

    QGeoCoordinate arp_;

    QHash<Sic, QVector3D> smr_;

    QHash<QString, QVector<QPolygonF>> runwayElements_;
    QHash<QString, QVector<QPolygonF>> taxiwayElements_;
    QHash<QString, QVector<QPolygonF>> apronLaneElements_;
    QHash<QString, QVector<QPolygonF>> standElements_;
    QHash<QString, QVector<QPolygonF>> airborne1Elements_;
    QHash<QString, QVector<QPolygonF>> airborne2Elements_;
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
