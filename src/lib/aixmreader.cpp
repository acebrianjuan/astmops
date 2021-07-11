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
    xml_.setDevice(device);

    if (xml_.readNextStartElement())
    {
        if (xml_.name() == QLatin1String("AIXMBasicMessage"))
        {
            readAixm();
        }
        else
        {
            xml_.raiseError(QLatin1String("The file is not an AIXM file."));
        }
    }

    return !xml_.error();
}

/*!
 * Generates an Aerodrome projected in local tangent plane coordinates.
 */
std::optional<Aerodrome> AixmReader::makeAerodrome() const
{
    if (!arp_.isValid() || qIsNaN(arp_.altitude()))
    {
        return std::nullopt;
    }

    Aerodrome aerodrome(arp_);

    // Coordinates of the local tangent plane origin.
    QGeoCoordinate geoOrigin = arp_;

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

    // Apron elements.
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

void AixmReader::readAixm()
{
    Q_ASSERT(xml_.isStartElement() &&
             xml_.name() == QLatin1String("AIXMBasicMessage"));

    while (!xml_.atEnd())
    {
        if (xml_.readNextStartElement())
        {
            // If the element start that we are in right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (xml_.name() != QLatin1String("hasMember"))
            {
                xml_.skipCurrentElement();
                continue;
            }

            // TODO: Consider reading an appropriate AIXM field to fill up the empty "name".
            QString name = QString();  // Using an empty string as a temporary placeholder.
            if (xml_.readNextStartElement())
            {
                if (xml_.name() == QLatin1String("AirportHeliport"))
                {
                    arp_ = posListToCoord(getPosList(arpPosXmlPath()));
                }
                else if (xml_.name() == QLatin1String("RunwayElement"))
                {
                    runwayElements_[name] << posListToCoordVector(getPosList(runwayPosListXmlPath()));
                }
                else if (xml_.name() == QLatin1String("TaxiwayElement"))
                {
                    taxiwayElements_[name] << posListToCoordVector(getPosList(taxiwayPosListXmlPath()));
                }
                else if (xml_.name() == QLatin1String("ApronElement"))
                {
                    apronLaneElements_[name] << posListToCoordVector(getPosList(apronPosListXmlPath()));
                }
                else if (xml_.name() == QLatin1String("AircraftStand"))
                {
                    standElements_[name] << posListToCoordVector(getPosList(standPosListXmlPath()));
                }
                else
                {
                    xml_.skipCurrentElement();
                }
            }
        }
    }
}

QStringList AixmReader::getPosList(const QStringList &tokens)
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
                QStringList list = xml_.readElementText().split(QLatin1String(" "));
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
