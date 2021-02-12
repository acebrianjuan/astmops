/*!
 * \file recordcollator.h
 * \brief Interface of the RecordCollator class.
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

#ifndef ASTMOPS_RECORDCOLLATOR_H
#define ASTMOPS_RECORDCOLLATOR_H

#include "asterix.h"
#include "astmops.h"
#include <QIODevice>
#include <QMultiMap>
#include <QObject>
#include <QQueue>
#include <QVector>

/*!
 * \brief The RecordCollator class filters, sorts and classifies
 * AsterixRecord objects into different multimaps.
 *
 * The purpose of this class is threefold:
 *
 * Filters records to be excluded from the analysis based on the ICAO address
 * of the target. The list of addresses to be filtered is read from a text
 * file.
 *
 * Classifies records into different multimaps based on their nature.
 * Factors such as the Category, Data Source Identifier, Message Type and
 * Target Report Descriptor are taken into account for this classification.
 *
 * Makes sure that the records are sorted in chronological order within
 * each multimap.
 */
class RecordCollator : public QObject
{
    Q_OBJECT

public:
    struct Counter
    {
        quint32 in = 0;
        quint32 out = 0;
    };

    explicit RecordCollator(QObject* parent = nullptr);

    void processRecord(AsterixRecord record);

    void loadExcludedAddresses(QIODevice* device);
    void loadExcludedAddresses(const QVector<IcaoAddr>& excladdr);
    void clearExcludedAddresses();
    void addExcludedAddress(const IcaoAddr addr);
    void removeExcludedAddress(const IcaoAddr addr);

    QVector<IcaoAddr> excludedAddresses() const;

    QMultiMap<QDateTime, AsterixRecord> smrTgtRepMultiMap() const;
    QMultiMap<QDateTime, AsterixRecord> smrSrvMsgMultiMap() const;
    QMultiMap<QDateTime, AsterixRecord> mlatTgtRepMultiMap() const;
    QMultiMap<QDateTime, AsterixRecord> mlatSrvMsgMultiMap() const;
    QMultiMap<QDateTime, AsterixRecord> adsbTgtRepMultiMap() const;

    Counter smrTgtRepCounter() const;
    Counter smrSrvMsgCounter() const;
    Counter mlatTgtRepCounter() const;
    Counter mlatSrvMsgCounter() const;
    Counter adsbTgtRepCounter() const;

public slots:

signals:

private:
    void monotonicTimeCheck(QDateTime& lastdt, QDateTime& newdt);

    QVector<IcaoAddr> m_excludedAddresses;

    quint8 m_smrSic;
    quint8 m_mlatSic;
    quint8 m_adsbSic;

    QDateTime m_lastSmrTgtRepDateTime;
    QDateTime m_lastSmrSrvMsgDateTime;
    QDateTime m_lastMlatTgtRepDateTime;
    QDateTime m_lastMlatSrvMsgDateTime;
    QDateTime m_lastAdsbTgtRepDateTime;

    QMultiMap<QDateTime, AsterixRecord> m_smrTgtRepMultiMap;
    QMultiMap<QDateTime, AsterixRecord> m_smrSrvMsgMultiMap;
    QMultiMap<QDateTime, AsterixRecord> m_mlatTgtRepMultiMap;
    QMultiMap<QDateTime, AsterixRecord> m_mlatSrvMsgMultiMap;
    QMultiMap<QDateTime, AsterixRecord> m_adsbTgtRepMultiMap;

    Counter m_smrTgtRepCounter;
    Counter m_smrSrvMsgCounter;
    Counter m_mlatTgtRepCounter;
    Counter m_mlatSrvMsgCounter;
    Counter m_adsbTgtRepCounter;
};

#endif  // ASTMOPS_RECORDCOLLATOR_H
