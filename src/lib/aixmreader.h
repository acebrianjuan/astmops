/*!
 * \file aixmreader.h
 * \brief Interface of the AixmReader class.
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

#ifndef ASTMOPS_AIXMREADER_H
#define ASTMOPS_AIXMREADER_H

#include "aerodrome.h"
#include <QGeoCoordinate>
#include <QObject>
#include <QXmlStreamReader>

/*!
 * \brief The AixmReader class reads the description of an aerodrome in
 * Aeronautical Information Exchange Model (AIXM) format.
 *
 * Reads the aerodrome description in AIXM format from a file and extracts
 * the elements of interest to generate a digital representation of the
 * aerodrome expressed in local east, north, up (ENU) coordinates.
 */
class AixmReader : public QObject
{
    Q_OBJECT

    friend class AixmReaderTest;

public:
    explicit AixmReader(QObject *parent = nullptr);

    bool read(QIODevice *device);
    Aerodrome makeAerodrome() const;

public slots:

signals:

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

    QXmlStreamReader m_xml;

    QGeoCoordinate m_arp;
    QVector<QVector<QGeoCoordinate>> m_runwayElements;
    QVector<QVector<QGeoCoordinate>> m_taxiwayElements;
    QVector<QVector<QGeoCoordinate>> m_apronElements;
    QVector<QVector<QGeoCoordinate>> m_standElements;
    QVector<QVector<QGeoCoordinate>> m_approach1Elements;
    QVector<QVector<QGeoCoordinate>> m_approach2Elements;
};

#endif  // ASTMOPS_AIXMREADER_H
