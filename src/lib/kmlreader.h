/*!
 * \file kmlreader.h
 * \brief Interface of the KmlReader class.
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

#ifndef ASTMOPS_KMLREADER_H
#define ASTMOPS_KMLREADER_H

#include "aerodrome.h"
#include "aerodromereaderinterface.h"
#include <QGeoCoordinate>
#include <QXmlStreamReader>

/*!
 * \brief The KmlReader class reads the description of an aerodrome in
 * Keyhole Markup Language (KML) format.
 *
 * Reads the aerodrome description in KML format from a file and extracts
 * the elements of interest to generate a digital representation of the
 * aerodrome expressed in local east, north, up (ENU) coordinates.
 */
class KmlReader : AerodromeReaderInterface
{
    using Collection = QVector<QVector<QGeoCoordinate>>;

    friend class KmlReaderTest;

public:
    KmlReader() = default;

    bool read(QIODevice *device) override;
    Aerodrome makeAerodrome() const override;

private:
    void readKml();
    void readPlacemark();

    QStringList getPosList(const QStringList &tokens);
    QGeoCoordinate posListToCoord(QStringList list);
    QVector<QGeoCoordinate> posListToCoordVector(QStringList list);

    static QStringList pointCoordinatesXmlPath();
    static QStringList polygonCoordinatesXmlPath();

    QXmlStreamReader m_xml;

    QGeoCoordinate m_arp;

    QHash<QString, Collection> m_runwayElements;
    QHash<QString, Collection> m_taxiwayElements;
    QHash<QString, Collection> m_apronElements;
    QHash<QString, Collection> m_standElements;
    QHash<QString, Collection> m_airborne1Elements;
    QHash<QString, Collection> m_airborne2Elements;
};

#endif  // ASTMOPS_KMLREADER_H
