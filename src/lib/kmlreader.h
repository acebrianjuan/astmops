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
#include <QGeoCoordinate>
#include <QObject>
#include <QXmlStreamReader>

/*!
 * \brief The KmlReader class reads the description of an aerodrome in
 * Keyhole Markup Language (KML) format.
 *
 * Reads the aerodrome description in KML format from a file and extracts
 * the elements of interest to generate a digital representation of the
 * aerodrome expressed in local east, north, up (ENU) coordinates.
 */
class KmlReader : public QObject
{
    Q_OBJECT

    friend class KmlReaderTest;

public:
    explicit KmlReader(QObject *parent = nullptr);

    bool read(QIODevice *device);
    Aerodrome makeAerodrome() const;

public slots:

signals:

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
    QVector<QVector<QGeoCoordinate>> m_runwayElements;
    QVector<QVector<QGeoCoordinate>> m_taxiwayElements;
    QVector<QVector<QGeoCoordinate>> m_apronElements;
    QVector<QVector<QGeoCoordinate>> m_standElements;
    QVector<QVector<QGeoCoordinate>> m_approach1Elements;
    QVector<QVector<QGeoCoordinate>> m_approach2Elements;
};

#endif  // ASTMOPS_KMLREADER_H