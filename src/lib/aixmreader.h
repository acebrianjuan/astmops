/*!
 * \file aixmreader.h
 * \brief Interface of the AixmReader class.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * -----------------------------------------------------------------------
 *
 * Copyright (C) 2020-2021 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
 *
 * ASTMOPS is a command line tool for evaluating 
 * the performance of A-SMGCS sensors at airports
 *
 * This file is part of ASTMOPS.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------
 */

#ifndef ASTMOPS_AIXMREADER_H
#define ASTMOPS_AIXMREADER_H

#include "aerodrome.h"
#include <QGeoCoordinate>
#include <QXmlStreamReader>
#include <optional>

/*!
 * \brief The AixmReader class reads the description of an aerodrome in
 * Aeronautical Information Exchange Model (AIXM) format.
 *
 * Reads the aerodrome description in AIXM format from a file and extracts
 * the elements of interest to generate a digital representation of the
 * aerodrome expressed in local east, north, up (ENU) coordinates.
 */
class AixmReader
{
    using Collection = QVector<QVector<QGeoCoordinate>>;

    friend class AixmReaderTest;

public:
    AixmReader() = default;

    bool read(QIODevice *device);
    bool canMakeAerodrome() const;
    Aerodrome makeAerodrome() const;

private:
    void readAixm();

    QStringList getPosList(const QStringList &tokens);
    QGeoCoordinate posListToCoord(QStringList list);
    QVector<QGeoCoordinate> posListToCoordVector(QStringList list);

    static QStringList arpPosXmlPath();
    static QStringList runwayPosListXmlPath();
    static QStringList taxiwayPosListXmlPath();
    static QStringList apronPosListXmlPath();
    static QStringList standPosListXmlPath();

    QXmlStreamReader xml_;

    QGeoCoordinate arp_;

    QHash<QString, Collection> runwayElements_;
    QHash<QString, Collection> taxiwayElements_;
    QHash<QString, Collection> apronLaneElements_;
    QHash<QString, Collection> standElements_;
    QHash<QString, Collection> airborne1Elements_;
    QHash<QString, Collection> airborne2Elements_;
};

#endif  // ASTMOPS_AIXMREADER_H
