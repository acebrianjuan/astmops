/*!
 * \file aixmreader.cpp
 * \brief Implementation of the AixmReader class.
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

#include "aixmreader.h"
#include "geofunctions.h"
#include <QDebug>

bool AixmReader::read(QIODevice *device)
{
    m_xml.setDevice(device);

    if (m_xml.readNextStartElement())
    {
        if (m_xml.name() == QLatin1String("AIXMBasicMessage"))
        {
            readAixm();
        }
        else
        {
            m_xml.raiseError(QLatin1String("The file is not an AIXM file."));
        }
    }

    return !m_xml.error();
}

/*!
 * Generates an Aerodrome projected in local tangent plane coordinates.
 */
Aerodrome AixmReader::makeAerodrome() const
{
    Aerodrome aerodrome;

    // Coordinates of the local tangent plane origin.
    QGeoCoordinate geoOrigin = m_arp;

    // Airport Reference Point.
    aerodrome.setArp(geoToLocalEnu(m_arp, geoOrigin));

    // Runway elements.
    for (auto it = m_runwayElements.begin(); it != m_runwayElements.end(); ++it)
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
    for (auto it = m_taxiwayElements.begin(); it != m_taxiwayElements.end(); ++it)
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

    // Apron elements.
    for (auto it = m_apronLaneElements.begin(); it != m_apronLaneElements.end(); ++it)
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
    for (auto it = m_standElements.begin(); it != m_standElements.end(); ++it)
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
    for (auto it = m_airborne1Elements.begin(); it != m_airborne1Elements.end(); ++it)
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
    for (auto it = m_airborne2Elements.begin(); it != m_airborne2Elements.end(); ++it)
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

void AixmReader::readAixm()
{
    Q_ASSERT(m_xml.isStartElement() &&
             m_xml.name() == QLatin1String("AIXMBasicMessage"));

    while (!m_xml.atEnd())
    {
        if (m_xml.readNextStartElement())
        {
            // If the element start that we are in right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (m_xml.name() != QLatin1String("hasMember"))
            {
                m_xml.skipCurrentElement();
                continue;
            }

            // TODO: Consider reading an appropriate AIXM field to fill up the empty "name".
            QString name = QString();  // Using an empty string as a temporary placeholder.
            if (m_xml.readNextStartElement())
            {
                if (m_xml.name() == QLatin1String("AirportHeliport"))
                {
                    m_arp = posListToCoord(getPosList(arpPosXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("RunwayElement"))
                {
                    m_runwayElements[name] << posListToCoordVector(getPosList(runwayPosListXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("TaxiwayElement"))
                {
                    m_taxiwayElements[name] << posListToCoordVector(getPosList(taxiwayPosListXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("ApronElement"))
                {
                    m_apronLaneElements[name] << posListToCoordVector(getPosList(apronPosListXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("AircraftStand"))
                {
                    m_standElements[name] << posListToCoordVector(getPosList(standPosListXmlPath()));
                }
                else
                {
                    m_xml.skipCurrentElement();
                }
            }
        }
    }
}

QStringList AixmReader::getPosList(const QStringList &tokens)
{
    int level = 0;
    while (!m_xml.atEnd())
    {
        if (m_xml.readNextStartElement())
        {
            // If the element start that we are in right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (m_xml.name() != tokens[level])
            {
                m_xml.skipCurrentElement();
                continue;
            }
            ++level;

            // We are at the wanted DocFileRef element.
            if (level >= tokens.size())
            {
                QStringList list = m_xml.readElementText().split(QLatin1String(" "));
                list.removeAll(QLatin1String(""));
                return list;
            }
        }

        if (m_xml.hasError())
        {
            break;
        }
    }

    // Something went wrong. Return empty list.
    return QStringList();
}

QGeoCoordinate AixmReader::posListToCoord(QStringList list)
{
    Q_ASSERT(list.size() == 2);

    double lon = list.takeFirst().toDouble();
    double lat = list.takeFirst().toDouble();
    QGeoCoordinate coord(lat, lon);

    return coord;
}

QVector<QGeoCoordinate> AixmReader::posListToCoordVector(QStringList list)
{
    int sizeIn = list.size();
    Q_ASSERT(sizeIn % 2 == 0);

    QVector<QGeoCoordinate> coordVector;
    double lon, lat;

    while (list.size() > 0)
    {
        lon = list.takeFirst().toDouble();
        lat = list.takeFirst().toDouble();
        coordVector << QGeoCoordinate(lat, lon);
    }

    int sizeOut = coordVector.size();
    Q_ASSERT(sizeOut == sizeIn / 2);

    return coordVector;
}

QStringList AixmReader::arpPosXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("AirportHeliportTimeSlice")
                                                    << QLatin1String("ARP")
                                                    << QLatin1String("ElevatedPoint")
                                                    << QLatin1String("pos");
    return tokens;
}

QStringList AixmReader::runwayPosListXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("RunwayElementTimeSlice")
                                                    << QLatin1String("extent")
                                                    << QLatin1String("ElevatedSurface")
                                                    << QLatin1String("patches")
                                                    << QLatin1String("PolygonPatch")
                                                    << QLatin1String("exterior")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("posList");
    return tokens;
}

QStringList AixmReader::taxiwayPosListXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("TaxiwayElementTimeSlice")
                                                    << QLatin1String("extent")
                                                    << QLatin1String("ElevatedSurface")
                                                    << QLatin1String("patches")
                                                    << QLatin1String("PolygonPatch")
                                                    << QLatin1String("exterior")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("posList");
    return tokens;
}

QStringList AixmReader::apronPosListXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("ApronElementTimeSlice")
                                                    << QLatin1String("extent")
                                                    << QLatin1String("ElevatedSurface")
                                                    << QLatin1String("patches")
                                                    << QLatin1String("PolygonPatch")
                                                    << QLatin1String("exterior")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("posList");
    return tokens;
}

QStringList AixmReader::standPosListXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("AircraftStandTimeSlice")
                                                    << QLatin1String("extent")
                                                    << QLatin1String("ElevatedSurface")
                                                    << QLatin1String("patches")
                                                    << QLatin1String("PolygonPatch")
                                                    << QLatin1String("exterior")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("posList");
    return tokens;
}
