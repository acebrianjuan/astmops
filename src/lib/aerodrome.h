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

#include <QHash>
#include <QPolygonF>
#include <QVector3D>
#include <QVector>
#include <optional>

/*!
 * \brief The Aerodrome class contains the assets of an aerodrome.
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
    enum Area
    {
        None = 0,

        // TODO: Revise enum values.

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
    Q_ENUM(Area)

    Aerodrome();

    bool hasAnyElements();
    bool hasAllElements();

    void setArp(QVector3D point);
    void addRunwayElement(const QPolygonF &polygon);
    void addTaxiwayElement(const QPolygonF &polygon);
    void addApronElement(const QPolygonF &polygon);
    void addStandElement(const QPolygonF &polygon);
    void addAirborne1Element(const QPolygonF &polygon);
    void addAirborne2Element(const QPolygonF &polygon);

    Aerodrome::Area locatePoint(const QVector3D cartPos, const std::optional<bool> &gndBit = std::nullopt);

    QVector<QPolygonF> getRunwayElements() const;
    QVector<QPolygonF> getTaxiwayElements() const;
    QVector<QPolygonF> getApronElements() const;
    QVector<QPolygonF> getStandElements() const;
    QVector<QPolygonF> getAirborne1Elements() const;
    QVector<QPolygonF> getAirborne2Elements() const;

private:
    bool collectionContainsPoint(const QVector<QPolygonF> &collection, QPointF point);

    QVector3D m_arp;
    QVector3D m_smr;

    QHash<QString, QVector<QPolygonF>> m_runwayElementsHash;
    QVector<QPolygonF> m_runwayElements;
    QVector<QPolygonF> m_taxiwayElements;
    QVector<QPolygonF> m_apronElements;
    QVector<QPolygonF> m_standElements;
    QVector<QPolygonF> m_airborne1Elements;
    QVector<QPolygonF> m_airborne2Elements;
};

#endif  // ASTMOPS_AERODROME_H
