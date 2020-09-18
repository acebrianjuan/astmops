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
#include <QRegularExpression>

KmlReader::KmlReader(QObject *parent) : QObject(parent)
{
}

bool KmlReader::read(QIODevice *device)
{
    m_xml.setDevice(device);

    if (m_xml.readNextStartElement())
    {
        if (m_xml.name() == QLatin1String("kml"))
        {
            readKml();
        }
        else
        {
            m_xml.raiseError(QLatin1String("The file is not a KML file."));
        }
    }

    return !m_xml.error();
}

void KmlReader::readKml()
{
    Q_ASSERT(m_xml.isStartElement() &&
             m_xml.name() == QLatin1String("kml"));

    if (m_xml.readNextStartElement())
    {
        if (m_xml.name() != QLatin1String("Document"))
        {
            m_xml.raiseError(QLatin1String("Missing <Document> root element."));
        }
    }

    while (!m_xml.atEnd())
    {
        if (m_xml.readNextStartElement())
        {
            // If the element start that we are in right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (m_xml.name() != QLatin1String("Placemark"))
            {
                m_xml.skipCurrentElement();
                continue;
            }

            readPlacemark();
        }
    }
}

void KmlReader::readPlacemark()
{
    Q_ASSERT(m_xml.isStartElement() &&
             m_xml.name() == QLatin1String("Placemark"));

    bool readDescription = false;
    bool readCoordinates = false;
    QString desc;
    QVector<QGeoCoordinate> coords;

    while (!m_xml.atEnd() && (!readDescription || !readCoordinates))
    {
        if (m_xml.readNextStartElement())
        {
            if (m_xml.name() == QLatin1String("description"))
            {
                desc = m_xml.readElementText();
                readDescription = true;
            }
            else if (m_xml.name() == QLatin1String("Polygon"))
            {
                coords << posListToCoordVector(getPosList(polygonCoordinatesXmlPath()));
                readCoordinates = true;
            }
            else if (m_xml.name() == QLatin1String("Point"))
            {
                coords << posListToCoord(getPosList(pointCoordinatesXmlPath()));
                readCoordinates = true;
            }
            else
            {
                m_xml.skipCurrentElement();
            }
        }
    }

    // Finished reading the Placemark contents.
    // Found description and coordinates?
    if (!desc.isNull() && !coords.isEmpty())
    {
        // TODO: Implement ARP case.
        if (desc == QLatin1String("RunwayElement"))
        {
            m_runwayElements << coords;
        }
        else if (desc == QLatin1String("TaxiwayElement"))
        {
            m_taxiwayElements << coords;
        }
        else if (desc == QLatin1String("ApronElement"))
        {
            m_apronElements << coords;
        }
    }
}

QStringList KmlReader::getPosList(const QStringList &tokens)
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
                QStringList list = m_xml.readElementText().split(QRegularExpression(QLatin1String(",|\\s")));
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

QGeoCoordinate KmlReader::posListToCoord(QStringList list)
{
    Q_ASSERT(list.size() == 2 || list.size() % 3 == 0);

    double lon = list.takeFirst().toDouble();
    double lat = list.takeFirst().toDouble();
    QGeoCoordinate coord(lat, lon);

    return coord;
}

QVector<QGeoCoordinate> KmlReader::posListToCoordVector(QStringList list)
{
    int sizeIn = list.size();
    Q_ASSERT(sizeIn % 3 == 0);

    QVector<QGeoCoordinate> coordVector;
    double lon, lat;

    while (list.size() > 0)
    {
        lon = list.takeFirst().toDouble();
        lat = list.takeFirst().toDouble();
        list.takeFirst().toDouble();  // Discard altitude component.
        coordVector << QGeoCoordinate(lat, lon);
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
