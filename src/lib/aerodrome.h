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
public:
    enum Area
    {
        Manoeuvering,
        Runway,
        Taxiway,
        Apron,
        Stand,
        Approach1,
        Approach2,
        Airborne,
        All,
        None
    };

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
    QVector<QPolygonF> m_runwayElements;
    QVector<QPolygonF> m_taxiwayElements;
    QVector<QPolygonF> m_apronElements;
    QVector<QPolygonF> m_standElements;
    QVector<QPolygonF> m_approach1Elements;
    QVector<QPolygonF> m_approach2Elements;
};

#endif  // ASTMOPS_AERODROME_H
