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

#include <QPolygonF>
#include <QVector3D>
#include <QVector>

/*!
 * \brief The Aerodrome class contains the assets of an aerodrome.
 *
 * Contains all the assets related to an aerodrome, among which are the
 * coordinates of the different physical elements that form it such as
 * runways, taxiways, aprons, stands as well as non-physical elements
 * like approach areas.
 */
class Aerodrome
{
    Q_GADGET

public:
    enum Area
    {
        None = 0,

        Enroute = 0b0000'0000'0000'0001,
        Approach1 = 0b0000'0000'0000'0010,
        Approach2 = 0b0000'0000'0000'0100,
        Approach = 0b0000'0000'0000'0110,
        //    Approach          = Approach1|Approach2,

        Airborne = 0b0000'0000'0000'0111,
        //    Airborne          = Approach|Enroute;
        //    Airborne          = Approach1|Approach2|Enroute;


        Runway = 0b0000'0001'0000'0000,
        Taxiway = 0b0000'0010'0000'0000,
        Manoeuvering = 0b0000'0011'0000'0000,
        //    Manoeuvering      = Runway|Taxiway,

        Stand = 0b0001'0000'0000'0000,
        ApronLane = 0b0010'0000'0000'0000,
        Apron = 0b0011'0000'0000'0000,
        //    Apron             = Stand|ApronLane,

        MovementArea = 0b0011'0011'0000'0000,
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

    void setArp(const QVector3D &point);
    void addRunwayElement(const QPolygonF &polygon);
    void addTaxiwayElement(const QPolygonF &polygon);
    void addApronElement(const QPolygonF &polygon);
    void addStandElement(const QPolygonF &polygon);
    void addApproach1Element(const QPolygonF &polygon);
    void addApproach2Element(const QPolygonF &polygon);

    Aerodrome::Area locatePoint(const QPointF &point);

    QVector<QPolygonF> getRunwayElements() const;
    QVector<QPolygonF> getTaxiwayElements() const;
    QVector<QPolygonF> getApronElements() const;
    QVector<QPolygonF> getStandElements() const;
    QVector<QPolygonF> getApproach1Elements() const;
    QVector<QPolygonF> getApproach2Elements() const;

private:
    bool collectionContainsPoint(const QVector<QPolygonF> &collection, const QPointF &point);

    QVector3D m_arp;
    QVector3D m_smr;
    QVector<QPolygonF> m_runwayElements;
    QVector<QPolygonF> m_taxiwayElements;
    QVector<QPolygonF> m_apronElements;
    QVector<QPolygonF> m_standElements;
    QVector<QPolygonF> m_approach1Elements;
    QVector<QPolygonF> m_approach2Elements;
};

#endif  // ASTMOPS_AERODROME_H
