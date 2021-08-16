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
#include <GeographicLib/Geoid.hpp>
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

bool KmlReader::canMakeAerodrome() const
{
    return arp_.isValid() && !qIsNaN(arp_.altitude());
}

/*!
 * Generates an Aerodrome projected in local tangent plane coordinates
 * centered at the ARP.
 */
Aerodrome KmlReader::makeAerodrome() const
{
    if (!canMakeAerodrome())
    {
        qFatal("Fatal error: No ARP coordinates found in the KML file!");
    }

    Aerodrome aerodrome(arp_);

    // Coordinates of the local tangent plane origin.
    QGeoCoordinate originGeo = arp_;

    // SMR origins.
    if (smr_.isEmpty())
    {
        qWarning() << "No SMR coordinates found in the KML file!"
                   << "Results for SMR sensors will be disabled";
    }
    else
    {
        for (auto it = smr_.begin(); it != smr_.end(); ++it)
        {
            Sic sic = it.key();
            QGeoCoordinate smrGeo = it.value();

            aerodrome.addSmr(sic, geoToLocalEnu(smrGeo, originGeo));
        }
    }

    // Runway elements.
    if (runwayElements_.isEmpty())
    {
        qWarning() << "No Runway elements found in the KML file!"
                   << "Results for Runway areas will be disabled";
    }
    else
    {
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
                    polygon << geoToLocalEnu(coord, originGeo).toPointF();
                }

                aerodrome.addRunwayElement(idStr, polygon);
            }
        }
    }

    // Taxiway elements.
    if (taxiwayElements_.isEmpty())
    {
        qWarning() << "No Taxiway elements found in the KML file!"
                   << "Results for Taxiway areas will be disabled";
    }
    else
    {
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
                    polygon << geoToLocalEnu(coord, originGeo).toPointF();
                }

                aerodrome.addTaxiwayElement(idStr, polygon);
            }
        }
    }

    // ApronLane elements.
    if (apronLaneElements_.isEmpty())
    {
        qWarning() << "No ApronLane elements found in the KML file!"
                   << "Results for ApronLane areas will be disabled";
    }
    else
    {
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
                    polygon << geoToLocalEnu(coord, originGeo).toPointF();
                }

                aerodrome.addApronLaneElement(idStr, polygon);
            }
        }
    }

    // Stand elements.
    if (standElements_.isEmpty())
    {
        qWarning() << "No Stand elements found in the KML file!"
                   << "Results for Stand areas will be disabled";
    }
    else
    {
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
                    polygon << geoToLocalEnu(coord, originGeo).toPointF();
                }

                aerodrome.addStandElement(idStr, polygon);
            }
        }
    }

    // Airborne1 elements.
    if (airborne1Elements_.isEmpty())
    {
        qWarning() << "No Airborne1 elements found in the KML file!"
                   << "Results for Airborne1 areas will be disabled";
    }
    else
    {
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
                    polygon << geoToLocalEnu(coord, originGeo).toPointF();
                }

                aerodrome.addAirborne1Element(idStr, polygon);
            }
        }
    }

    // Airborne2 elements.
    if (airborne2Elements_.isEmpty())
    {
        qWarning() << "No Airborne2 elements found in the KML file!"
                   << "Results for Airborne2 areas will be disabled";
    }
    else
    {
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
                    polygon << geoToLocalEnu(coord, originGeo).toPointF();
                }

                aerodrome.addAirborne2Element(idStr, polygon);
            }
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

    // Once we have finished reading the KML file, we apply the geometric
    // altitude of the ARP to all aerodrome elements.

    double alt = arp_.altitude();

    for (QGeoCoordinate &c : smr_)
    {
        c.setAltitude(alt);
    }

    for (Collection &col : runwayElements_)
    {
        for (QVector<QGeoCoordinate> &ele : col)
        {
            for (QGeoCoordinate &c : ele)
            {
                c.setAltitude(alt);
            }
        }
    }

    for (Collection &col : taxiwayElements_)
    {
        for (QVector<QGeoCoordinate> &ele : col)
        {
            for (QGeoCoordinate &c : ele)
            {
                c.setAltitude(alt);
            }
        }
    }

    for (Collection &col : apronLaneElements_)
    {
        for (QVector<QGeoCoordinate> &ele : col)
        {
            for (QGeoCoordinate &c : ele)
            {
                c.setAltitude(alt);
            }
        }
    }

    for (Collection &col : standElements_)
    {
        for (QVector<QGeoCoordinate> &ele : col)
        {
            for (QGeoCoordinate &c : ele)
            {
                c.setAltitude(alt);
            }
        }
    }

    for (Collection &col : airborne1Elements_)
    {
        for (QVector<QGeoCoordinate> &ele : col)
        {
            for (QGeoCoordinate &c : ele)
            {
                c.setAltitude(alt);
            }
        }
    }

    for (Collection &col : airborne2Elements_)
    {
        for (QVector<QGeoCoordinate> &ele : col)
        {
            for (QGeoCoordinate &c : ele)
            {
                c.setAltitude(alt);
            }
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
    if (!desc.isEmpty() && !coords.isEmpty())
    {
        // Example 1:
        // desc: RunwayElement_07R/25L
        //     -> token: RunwayElement
        //     -> suffix: 07R/25L

        // Example 2:
        // desc: TaxiwayElement
        //     -> token: TaxiwayElement
        //     -> suffix: (empty)

        // Example 3:
        // desc: SMR_7
        //     -> token: SMR
        //     -> suffix: 7

        QStringList descParts = desc.split(QLatin1Char('_'));
        QString token = descParts.first();
        QString suffix;  // suffix is an empty string by default.
        if (descParts.size() == 2)
        {
            suffix = descParts.at(1);
        }

        if (token == QLatin1String("ARP"))
        {
            // ARP is required to have the ELEV as suffix.
            if (suffix.isEmpty())
            {
                //qWarning();
                return;
            }

            Q_ASSERT(coords.size() == 1);

            bool ok = false;
            double elev = suffix.toDouble(&ok);

            if (ok)
            {
                QGeoCoordinate c = coords.first();

                GeographicLib::Geoid geoid("egm96-5");
                double H = elev;
                double N = geoid(c.latitude(), c.longitude());
                double h = H + N;

                c.setAltitude(h);

                arp_ = c;
            }
        }
        else if (token == QLatin1String("SMR"))
        {
            // SMR is required to have the SIC as suffix.
            if (suffix.isEmpty())
            {
                //qWarning();
                return;
            }

            Q_ASSERT(coords.size() == 1);

            bool ok = false;
            Sic sic = suffix.toUInt(&ok);

            if (ok)
            {
                smr_[sic] = coords.first();
            }
        }
        else if (token == QLatin1String("RunwayElement"))
        {
            runwayElements_[suffix] << coords;
        }
        else if (token == QLatin1String("TaxiwayElement"))
        {
            taxiwayElements_[suffix] << coords;
        }
        else if (token == QLatin1String("ApronElement"))
        {
            apronLaneElements_[suffix] << coords;
        }
        else if (token == QLatin1String("AircraftStand"))
        {
            standElements_[suffix] << coords;
        }
        else if (token == QLatin1String("Airborne1Element"))
        {
            airborne1Elements_[suffix] << coords;
        }
        else if (token == QLatin1String("Airborne2Element"))
        {
            airborne2Elements_[suffix] << coords;
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
