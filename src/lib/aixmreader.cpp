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

AixmReader::AixmReader(QObject *parent) : QObject(parent)
{
}

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
            m_xml.raiseError(QObject::tr("The file is not an AIXM file."));
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
    for (QVector<QGeoCoordinate> rwyEleGeo : m_runwayElements)
    {
        QPolygonF polygon;
        polygon.reserve(rwyEleGeo.size());

        for (QGeoCoordinate coord : rwyEleGeo)
        {
            polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
        }

        aerodrome.addRunwayElement(polygon);
    }

    // Taxiway elements.
    for (QVector<QGeoCoordinate> twyEleGeo : m_taxiwayElements)
    {
        QPolygonF polygon;
        polygon.reserve(twyEleGeo.size());

        for (QGeoCoordinate coord : twyEleGeo)
        {
            polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
        }

        aerodrome.addTaxiwayElement(polygon);
    }

    // Apron elements.
    for (QVector<QGeoCoordinate> apronEleGeo : m_apronElements)
    {
        QPolygonF polygon;
        polygon.reserve(apronEleGeo.size());

        for (QGeoCoordinate coord : apronEleGeo)
        {
            polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
        }

        aerodrome.addApronElement(polygon);
    }

    // Stand elements.
    for (QVector<QGeoCoordinate> standEleGeo : m_standElements)
    {
        QPolygonF polygon;
        polygon.reserve(standEleGeo.size());

        for (QGeoCoordinate coord : standEleGeo)
        {
            polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
        }

        aerodrome.addStandElement(polygon);
    }

    // Approach 1 elements.
    for (QVector<QGeoCoordinate> app1EleGeo : m_approach1Elements)
    {
        QPolygonF polygon;
        polygon.reserve(app1EleGeo.size());

        for (QGeoCoordinate coord : app1EleGeo)
        {
            polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
        }

        aerodrome.addApproach1Element(polygon);
    }

    // Approach 2 elements.
    for (QVector<QGeoCoordinate> app2EleGeo : m_approach2Elements)
    {
        QPolygonF polygon;
        polygon.reserve(app2EleGeo.size());

        for (QGeoCoordinate coord : app2EleGeo)
        {
            polygon << geoToLocalEnu(coord, geoOrigin).toPointF();
        }

        aerodrome.addApproach2Element(polygon);
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

            if (m_xml.readNextStartElement())
            {
                if (m_xml.name() == QLatin1String("AirportHeliport"))
                {
                    m_arp = posListToCoord(getPosList(arpPosXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("RunwayElement"))
                {
                    m_runwayElements << posListToCoordVector(getPosList(runwayPosListXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("TaxiwayElement"))
                {
                    m_taxiwayElements << posListToCoordVector(getPosList(taxiwayPosListXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("ApronElement"))
                {
                    m_apronElements << posListToCoordVector(getPosList(apronPosListXmlPath()));
                }
                else if (m_xml.name() == QLatin1String("StandElement"))
                {
                    //m_aerodrome.addStandElement(getPosList(standPosListXmlPath()));
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
            // If the element start that we are it right now is not the one we want,
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
