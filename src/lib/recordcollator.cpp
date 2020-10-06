/*!
 * \file recordcollator.cpp
 * \brief Implementation of the RecordCollator class.
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

#include "recordcollator.h"
#include <QDebug>
#include <QSettings>
#include <algorithm>

RecordCollator::RecordCollator(QObject *parent) : QObject(parent),
                                                  m_smrSic(Configuration::smrSic()),
                                                  m_mlatSic(Configuration::mlatSic()),
                                                  m_adsbSic(Configuration::adsbSic())
{
}

void RecordCollator::processRecord(const AsterixRecord &record)
{
    // Classify record by System Type (smr, mlat, adsb).
    // Filter out if record is an excluded address.
    // Sort records by Time of Day.

    // TODO: Classify Records based on System Identification Code (SIC).
    // TODO: Handle cases with no Target Address information.
    // TODO: Consider sorting only the last n elements of the queues for performance.

    auto sorter = [](const AsterixRecord &lhs, const AsterixRecord &rhs) {
        return lhs.m_dateTime < rhs.m_dateTime;
    };

    if (record.m_cat == 10)  // Monosensor Surface Movement Data.
    {
        AsterixDataItem di010_000 = record.m_dataItems[QLatin1String("I000")];
        quint8 msgType = di010_000.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

        AsterixDataItem di010_010 = record.m_dataItems[QLatin1String("I010")];
        //quint8 sac = di010_010.m_fields[0].value<AsterixDataElement>().m_value.toUInt();
        quint8 sic = di010_010.m_fields[1].value<AsterixDataElement>().m_value.toUInt();

        if (msgType == 1)  // Target Report.
        {
            AsterixDataItem di010_020 = record.m_dataItems[QLatin1String("I020")];
            quint8 sysType = di010_020.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            if (sysType == 1)  // Mode S Multilateration.
            {
                ++m_mlatTgtRepCounter.in;
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
                ++m_smrTgtRepCounter.in;
            }

            bool ok;
            AsterixDataItem di010_220 = record.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt(&ok, 16);

            if (ok)  // Valid Target Address.
            {
                // Do not continue if Target Address is an excluded address.
                if (m_excludedAddresses.contains(tgtAddr))
                {
                    return;
                }
            }

            /* Continue if address is a valid non-excluded address
             * or if there is no address information at all.
             */
            if (sysType == 1)  // Mode S Multilateration.
            {
                m_mlatTgtRepQueue.enqueue(record);
                std::sort(m_mlatTgtRepQueue.begin(), m_mlatTgtRepQueue.end(), sorter);
                ++m_mlatTgtRepCounter.out;
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
                m_smrTgtRepQueue.enqueue(record);
                std::sort(m_smrTgtRepQueue.begin(), m_smrTgtRepQueue.end(), sorter);
                ++m_smrTgtRepCounter.out;
            }
        }
        else if (msgType == 3)  // Periodic Status Message.
        {
            // Check the SIC to determine which system is the service message informing about.
            // Service Messages are always enqueued.

            if (sic == m_mlatSic)  // Mode S Multilateration.
            {
                ++m_mlatSrvMsgCounter.in;

                m_mlatSrvMsgQueue.enqueue(record);
                std::sort(m_mlatSrvMsgQueue.begin(), m_mlatSrvMsgQueue.end(), sorter);
                ++m_mlatSrvMsgCounter.out;
            }
            else if (sic == m_smrSic)  // Primary Surveillance Radar.
            {
                ++m_smrSrvMsgCounter.in;

                m_smrSrvMsgQueue.enqueue(record);
                std::sort(m_smrSrvMsgQueue.begin(), m_smrSrvMsgQueue.end(), sorter);
                ++m_smrSrvMsgCounter.out;
            }
        }
    }
    else if (record.m_cat == 21)  // ADS-B Reports.
    {
        ++m_adsbTgtRepCounter.in;
        bool ok;
        AsterixDataItem di021_080 = record.m_dataItems[QLatin1String("I080")];
        IcaoAddr tgtAddr = di021_080.m_fields[0].value<AsterixDataElement>().m_value.toUInt(&ok, 16);

        if (ok)  // Valid Target Address.
        {
            // Do not continue if Target Address is an excluded address.
            if (m_excludedAddresses.contains(tgtAddr))
            {
                return;
            }
        }

        /* Continue if address is a valid non-excluded address
         * or if there is no address information at all.
         */
        m_adsbTgtRepQueue.enqueue(record);
        std::sort(m_adsbTgtRepQueue.begin(), m_adsbTgtRepQueue.end(), sorter);
        ++m_adsbTgtRepCounter.out;
    }
}

void RecordCollator::loadExcludedAddresses(QIODevice *device)
{
    QByteArray data = device->readAll();
    for (const auto &line : data.split('\n'))
    {
        // TODO: Consider ignoring comment lines explicitly.
        bool ok;
        IcaoAddr addr = line.trimmed().toUInt(&ok, 16);
        if (ok)
        {
            m_excludedAddresses.append(addr);
        }
    }
}

void RecordCollator::loadExcludedAddresses(QVector<IcaoAddr> excladdr)
{
    m_excludedAddresses = excladdr;
}

void RecordCollator::clearExcludedAddresses()
{
    m_excludedAddresses.clear();
}

void RecordCollator::addExcludedAddress(const IcaoAddr addr)
{
    m_excludedAddresses.append(addr);
}

void RecordCollator::removeExcludedAddress(const IcaoAddr addr)
{
    m_excludedAddresses.removeOne(addr);
}

bool RecordCollator::hasPendingSmrTgtRepBatches()
{
    return m_smrTgtRepQueue.size() >= m_minBatchSize;
}

bool RecordCollator::hasPendingSmrSrvMsgBatches()
{
    return m_smrSrvMsgQueue.size() >= m_minBatchSize;
}

bool RecordCollator::hasPendingMlatTgtRepBatches()
{
    return m_mlatTgtRepQueue.size() >= m_minBatchSize;
}

bool RecordCollator::hasPendingMlatSrvMsgBatches()
{
    return m_mlatSrvMsgQueue.size() >= m_minBatchSize;
}

bool RecordCollator::hasPendingAdsbTgtRepBatches()
{
    return m_adsbTgtRepQueue.size() >= m_minBatchSize;
}

QVector<IcaoAddr> RecordCollator::excludedAddresses() const
{
    return m_excludedAddresses;
}

QQueue<AsterixRecord> RecordCollator::smrTgtRepQueue() const
{
    return m_smrTgtRepQueue;
}

QQueue<AsterixRecord> RecordCollator::smrSrvMsgQueue() const
{
    return m_smrSrvMsgQueue;
}

QQueue<AsterixRecord> RecordCollator::mlatTgtRepQueue() const
{
    return m_mlatTgtRepQueue;
}

QQueue<AsterixRecord> RecordCollator::mlatSrvMsgQueue() const
{
    return m_mlatSrvMsgQueue;
}

QQueue<AsterixRecord> RecordCollator::adsbTgtRepQueue() const
{
    return m_adsbTgtRepQueue;
}

QVector<AsterixRecord> RecordCollator::smrTgtRepBatch(int size)
{
    return extractBatch(m_smrTgtRepQueue, size);
}

QVector<AsterixRecord> RecordCollator::smrSrvMsgBatch(int size)
{
    return extractBatch(m_smrSrvMsgQueue, size);
}

QVector<AsterixRecord> RecordCollator::mlatTgtRepBatch(int size)
{
    return extractBatch(m_mlatTgtRepQueue, size);
}

QVector<AsterixRecord> RecordCollator::mlatSrvMsgBatch(int size)
{
    return extractBatch(m_mlatSrvMsgQueue, size);
}

QVector<AsterixRecord> RecordCollator::adsbTgtRepBatch(int size)
{
    return extractBatch(m_adsbTgtRepQueue, size);
}

RecordCollator::Counter RecordCollator::smrTgtRepCounter() const
{
    return m_smrTgtRepCounter;
}

RecordCollator::Counter RecordCollator::smrSrvMsgCounter() const
{
    return m_smrSrvMsgCounter;
}

RecordCollator::Counter RecordCollator::mlatTgtRepCounter() const
{
    return m_mlatTgtRepCounter;
}

RecordCollator::Counter RecordCollator::mlatSrvMsgCounter() const
{
    return m_mlatSrvMsgCounter;
}

RecordCollator::Counter RecordCollator::adsbTgtRepCounter() const
{
    return m_adsbTgtRepCounter;
}

QVector<AsterixRecord> RecordCollator::extractBatch(QQueue<AsterixRecord> queue, int batchSize)
{
    Q_ASSERT(queue.size() >= batchSize);

    QVector<AsterixRecord> batch;
    batch.reserve(batchSize);

    int sz = batchSize;
    while (sz > 0)
    {
        batch << queue.dequeue();
        --sz;
    }

    Q_ASSERT(batch.size() == batchSize);

    return batch;
}
