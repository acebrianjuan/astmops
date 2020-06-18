/*!
 * \file asterixxmlreader.h
 * \brief Interface of a class that reads ASTERIX data in XML format.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * Reads ASTERIX data in XML format and generates AsterixRecord objects
 * from the data stream. These objects are placed in a queue to be
 * consumed upstream in the processing chain.
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

#ifndef ASTMOPS_ASTERIXXMLREADER_H
#define ASTMOPS_ASTERIXXMLREADER_H

#include "asterix.h"
#include <QObject>
#include <QQueue>
#include <QXmlStreamReader>

class AsterixXmlReader : public QObject
{
    Q_OBJECT

public:
    explicit AsterixXmlReader(QObject* parent = nullptr);

    void addData(const QByteArray& data);
    AsterixRecord record();
    bool hasPendingRecords();

public slots:

signals:
    void readyRead();

private:
    void readRecord();
    AsterixDataItem readDataItem();
    AsterixDataElement readDataField();
    bool isValidDataItem(const QString& di);

    QXmlStreamReader m_xml;
    QQueue<AsterixRecord> m_recordsQueue;
};

#endif  // ASTMOPS_ASTERIXXMLREADER_H
