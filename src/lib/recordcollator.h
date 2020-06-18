/*!
 * \file recordcollator.h
 * \brief Interface of a class that filters, sorts and classifies
 * AsterixRecord objects into different queues.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * The purpose of this class is threefold:
 *
 * Filters records to be excluded from the analysis based on the ICAO
 * address of the target. The list of addresses to be filtered is read
 * from a text file.
 *
 * Classifies records into different queues based on their nature.
 * Factors such as the Category, Data Source Identifier, Message Type
 * and Target Report Descriptor are taken into account for this
 * classification.
 *
 * Makes sure that the records are sorted in chronological order within
 * each queue.
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
#include <QObject>
#include <QQueue>
#include <QVector>

class RecordCollator : public QObject
{
    Q_OBJECT

public:
    struct Counter
    {
        uint in = 0;
        uint out = 0;
    };

    explicit RecordCollator(QObject* parent = nullptr);

    void processRecord(const AsterixRecord& record);

    void loadExcludedAddresses(QIODevice* device);
    void loadExcludedAddresses(QVector<IcaoAddr> excladdr);
    void clearExcludedAddresses();
    void addExcludedAddress(const IcaoAddr addr);
    void removeExcludedAddress(const IcaoAddr addr);

    bool hasPendingSmrTgtRepBatches();
    bool hasPendingSmrSrvMsgBatches();
    bool hasPendingMlatTgtRepBatches();
    bool hasPendingMlatSrvMsgBatches();
    bool hasPendingAdsbTgtRepBatches();

    void readSettings();

    QVector<IcaoAddr> excludedAddresses() const;

    QQueue<AsterixRecord> smrTgtRepQueue() const;
    QQueue<AsterixRecord> smrSrvMsgQueue() const;
    QQueue<AsterixRecord> mlatTgtRepQueue() const;
    QQueue<AsterixRecord> mlatSrvMsgQueue() const;
    QQueue<AsterixRecord> adsbTgtRepQueue() const;

    QVector<AsterixRecord> smrTgtRepBatch(int size);
    QVector<AsterixRecord> smrSrvMsgBatch(int size);
    QVector<AsterixRecord> mlatTgtRepBatch(int size);
    QVector<AsterixRecord> mlatSrvMsgBatch(int size);
    QVector<AsterixRecord> adsbTgtRepBatch(int size);

    Counter smrTgtRepCounter() const;
    Counter smrSrvMsgCounter() const;
    Counter mlatTgtRepCounter() const;
    Counter mlatSrvMsgCounter() const;
    Counter adsbTgtRepCounter() const;

public slots:

signals:
    void readyRead();

private:
    QVector<AsterixRecord> extractBatch(QQueue<AsterixRecord> queue, int batchSize);

    int m_minBatchSize;

    const QString m_smrSicKey = QLatin1String("SMR.SIC");
    const QString m_mlatSicKey = QLatin1String("MLAT.SIC");
    const QString m_adsbSicKey = QLatin1String("ADS-B.SIC");

    QVector<IcaoAddr> m_excludedAddresses;

    quint8 m_smrSic;
    quint8 m_mlatSic;
    quint8 m_adsbSic;

    QHash<SystemType, QQueue<AsterixRecord>> m_catMap;
    QQueue<AsterixRecord> m_smrTgtRepQueue;
    QQueue<AsterixRecord> m_smrSrvMsgQueue;
    QQueue<AsterixRecord> m_mlatTgtRepQueue;
    QQueue<AsterixRecord> m_mlatSrvMsgQueue;
    QQueue<AsterixRecord> m_adsbTgtRepQueue;

    Counter m_smrTgtRepCounter;
    Counter m_smrSrvMsgCounter;
    Counter m_mlatTgtRepCounter;
    Counter m_mlatSrvMsgCounter;
    Counter m_adsbTgtRepCounter;
};

#endif  // ASTMOPS_RECORDCOLLATOR_H
