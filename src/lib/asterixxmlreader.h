/*!
 * \file asterixxmlreader.h
 * \brief Interface of the AsterixXmlReader class.
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

#ifndef ASTMOPS_ASTERIXXMLREADER_H
#define ASTMOPS_ASTERIXXMLREADER_H

#include "asterix.h"
#include <QMultiMap>
#include <QObject>
#include <QQueue>
#include <QXmlStreamReader>

/*!
 * \brief The AsterixXmlReader class reads ASTERIX protocol data in
 * line-delimited XML format.
 *
 * Reads ASTERIX data in line-delimited XML format and generates AsterixRecord
 * objects from the data stream. These objects are placed in a queue to be
 * consumed upstream in the processing chain.
 */
class AsterixXmlReader : public QObject
{
    Q_OBJECT

public:
    explicit AsterixXmlReader(QObject* parent = nullptr);

    void addData(const QByteArray& data);
    Asterix::Record record();
    bool hasPendingRecords();
    void setStartDate(QDate date);

public slots:

signals:
    void readyRead();

private:
    void readRecord();
    Asterix::DataItem readDataItem();
    Asterix::DataElement readDataElement();
    bool isValidDataItem(const QString& di);

    QDate m_startDate;
    QXmlStreamReader m_xml;
    QQueue<Asterix::Record> m_recordsQueue;
};

#endif  // ASTMOPS_ASTERIXXMLREADER_H
