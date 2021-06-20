/*!
 * \file kmlreader.cpp
 * \brief Implementation of the KmlReader class.
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

#include "kmlreader.h"
#include "geofunctions.h"
#include <QRegularExpression>

bool KmlReader::read(QIODevice *device)
{
    xml_.setDevice(device);

    if (xml_.readNextStartElement())
    {
        if (xml_.name() == QLatin1String("kml"))
        {
            readKml();
        }
        else
        {
            xml_.raiseError(QLatin1String("The file is not a KML file."));
        }
    }

    return !xml_.error();
}

/*!
 * Generates an Aerodrome projected in local tangent plane coordinates.
 */
Aerodrome KmlReader::makeAerodrome() const
{
    Aerodrome aerodrome;

    // Coordinates of the local tangent plane origin.
    QGeoCoordinate geoOrigin = arp_;

    // Airport Reference Point.
    aerodrome.setArp(geoToLocalEnu(arp_, geoOrigin));

    // Runway elements.
    for (auto it = runwayElements_.begin(); it != runwayElements_.end(); ++it)
    {
        QString idStr = it.key();
        Collection rwyElements = it.value();

        for (const QVector<QGeoCoordinate> &rwyEleGeo : rwyElements)
        {
            QPolygonF polygon;
            polygon.reserve(rwyEleGeo.size());

            for (const QGeoCoordinate &coord : rwyEleGeo)
            {
                polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
            }

            aerodrome.addRunwayElement(idStr, polygon);
        }
    }

    // Taxiway elements.
    for (auto it = taxiwayElements_.begin(); it != taxiwayElements_.end(); ++it)
    {
        QString idStr = it.key();
        Collection twyElements = it.value();

        for (const QVector<QGeoCoordinate> &twyEleGeo : twyElements)
        {
            QPolygonF polygon;
            polygon.reserve(twyEleGeo.size());

            for (const QGeoCoordinate &coord : twyEleGeo)
            {
                polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
            }

            aerodrome.addTaxiwayElement(idStr, polygon);
        }
    }

    // ApronLane elements.
    for (auto it = apronLaneElements_.begin(); it != apronLaneElements_.end(); ++it)
    {
        QString idStr = it.key();
        Collection apronLaneElements = it.value();

        for (const QVector<QGeoCoordinate> &apronLaneEleGeo : apronLaneElements)
        {
            QPolygonF polygon;
            polygon.reserve(apronLaneEleGeo.size());

            for (const QGeoCoordinate &coord : apronLaneEleGeo)
            {
                polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
            }

            aerodrome.addApronLaneElement(idStr, polygon);
        }
    }

    // Stand elements.
    for (auto it = standElements_.begin(); it != standElements_.end(); ++it)
    {
        QString idStr = it.key();
        Collection standElements = it.value();

        for (const QVector<QGeoCoordinate> &standEleGeo : standElements)
        {
            QPolygonF polygon;
            polygon.reserve(standEleGeo.size());

            for (const QGeoCoordinate &coord : standEleGeo)
            {
                polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
            }

            aerodrome.addStandElement(idStr, polygon);
        }
    }

    // Airborne 1 elements.
    for (auto it = airborne1Elements_.begin(); it != airborne1Elements_.end(); ++it)
    {
        QString idStr = it.key();
        Collection airborne1Elements = it.value();

        for (const QVector<QGeoCoordinate> &airborne1EleGeo : airborne1Elements)
        {
            QPolygonF polygon;
            polygon.reserve(airborne1EleGeo.size());

            for (const QGeoCoordinate &coord : airborne1EleGeo)
            {
                polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
            }

            aerodrome.addAirborne1Element(idStr, polygon);
        }
    }

    // Airborne 2 elements.
    for (auto it = airborne2Elements_.begin(); it != airborne2Elements_.end(); ++it)
    {
        QString idStr = it.key();
        Collection airborne2Elements = it.value();

        for (const QVector<QGeoCoordinate> &airborne2EleGeo : airborne2Elements)
        {
            QPolygonF polygon;
            polygon.reserve(airborne2EleGeo.size());

            for (const QGeoCoordinate &coord : airborne2EleGeo)
            {
                polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
            }

            aerodrome.addAirborne2Element(idStr, polygon);
        }
    }

    return aerodrome;
}

void KmlReader::readKml()
{
    Q_ASSERT(xml_.isStartElement() &&
             xml_.name() == QLatin1String("kml"));

    if (xml_.readNextStartElement())
    {
        if (xml_.name() != QLatin1String("Document"))
        {
            xml_.raiseError(QLatin1String("Missing <Document> root element."));
        }
    }

    while (!xml_.atEnd())
    {
        if (xml_.readNextStartElement())
        {
            if (xml_.name() == QLatin1String("Folder"))
            {
                continue;
            }

            // If the element start that we are in right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (xml_.name() != QLatin1String("Placemark"))
            {
                xml_.skipCurrentElement();
                continue;
            }

            readPlacemark();
        }
    }
}

void KmlReader::readPlacemark()
{
    Q_ASSERT(xml_.isStartElement() &&
             xml_.name() == QLatin1String("Placemark"));

    bool readDescription = false;
    bool readCoordinates = false;
    QString desc;
    QVector<QGeoCoordinate> coords;

    while (!xml_.atEnd() && (!readDescription || !readCoordinates))
    {
        if (xml_.readNextStartElement())
        {
            if (xml_.name() == QLatin1String("description"))
            {
                desc = xml_.readElementText();
                readDescription = true;
            }
            else if (xml_.name() == QLatin1String("Polygon"))
            {
                coords << posListToCoordVector(getPosList(polygonCoordinatesXmlPath()));
                readCoordinates = true;
            }
            else if (xml_.name() == QLatin1String("Point"))
            {
                coords << posListToCoord(getPosList(pointCoordinatesXmlPath()));
                readCoordinates = true;
            }
            else
            {
                xml_.skipCurrentElement();
            }
        }
    }

    // Finished reading the Placemark contents.
    // Found description and coordinates?
    if (!desc.isNull() && !coords.isEmpty())
    {
        // Example 1:
        // desc: RunwayElement_07R/25L
        //     -> token: RunwayElement
        //     -> name: 07R/25L

        // Example 2:
        // desc: TaxiwayElement
        //     -> token: TaxiwayElement
        //     -> name: (empty)

        QStringList descParts = desc.split(QLatin1Char('_'));
        QString token = descParts.first();
        QString name;  // name is empty by default.
        if (descParts.size() == 2)
        {
            name = descParts.at(1);
        }

        if (token == QLatin1String("ARP"))
        {
            arp_ = coords.first();
        }
        else if (token == QLatin1String("RunwayElement"))
        {
            runwayElements_[name] << coords;
        }
        else if (token == QLatin1String("TaxiwayElement"))
        {
            taxiwayElements_[name] << coords;
        }
        else if (token == QLatin1String("ApronElement"))
        {
            apronLaneElements_[name] << coords;
        }
        else if (token == QLatin1String("AircraftStand"))
        {
            standElements_[name] << coords;
        }
        else if (token == QLatin1String("Airborne1Element"))
        {
            airborne1Elements_[name] << coords;
        }
        else if (token == QLatin1String("Airborne2Element"))
        {
            airborne2Elements_[name] << coords;
        }
    }
}

QStringList KmlReader::getPosList(const QStringList &tokens)
{
    int level = 0;
    while (!xml_.atEnd())
    {
        if (xml_.readNextStartElement())
        {
            // If the element start that we are in right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (xml_.name() != tokens[level])
            {
                xml_.skipCurrentElement();
                continue;
            }
            ++level;

            // We are at the wanted DocFileRef element.
            if (level >= tokens.size())
            {
                QStringList list = xml_.readElementText().split(QRegularExpression(QLatin1String(",|\\s")));
                list.removeAll(QLatin1String(""));
                return list;
            }
        }

        if (xml_.hasError())
        {
            break;
        }
    }

    // Something went wrong. Return empty list.
    return QStringList();
}

QGeoCoordinate KmlReader::posListToCoord(QStringList list)
{
    Q_ASSERT(/*list.size() == 2 ||*/ list.size() % 3 == 0);

    double lon = list.takeFirst().toDouble();
    double lat = list.takeFirst().toDouble();
    double alt = list.takeFirst().toDouble();
    QGeoCoordinate coord(lat, lon, alt);

    return coord;
}

QVector<QGeoCoordinate> KmlReader::posListToCoordVector(QStringList list)
{
    int sizeIn = list.size();
    Q_ASSERT(sizeIn % 3 == 0);

    QVector<QGeoCoordinate> coordVector;
    double lon, lat, alt;

    while (list.size() > 0)
    {
        lon = list.takeFirst().toDouble();
        lat = list.takeFirst().toDouble();
        alt = list.takeFirst().toDouble();
        coordVector << QGeoCoordinate(lat, lon, alt);
    }

    int sizeOut = coordVector.size();
    Q_ASSERT(sizeOut == sizeIn / 3);

    return coordVector;
}

QStringList KmlReader::pointCoordinatesXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("coordinates");
    return tokens;
}

QStringList KmlReader::polygonCoordinatesXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("outerBoundaryIs")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("coordinates");
    return tokens;
}
