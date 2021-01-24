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


    /*
    auto sorter = [](const AsterixRecord &lhs, const AsterixRecord &rhs) {
        return lhs.m_dateTime < rhs.m_dateTime;
    };
    */

    if (record.m_cat == 10)  // Monosensor Surface Movement Data.
    {
        quint8 msgType = record.valStrFromDitem(QLatin1String("I000"), QLatin1String("MsgTyp")).toUInt();
        quint8 sic = record.valStrFromDitem(QLatin1String("I010"), QLatin1String("SIC")).toUInt();

        if (msgType == 1)  // Target Report.
        {
            quint8 sysType = record.valStrFromDitem(QLatin1String("I020"), QLatin1String("TYP")).toUInt();

            if (sysType == 1)  // Mode S Multilateration.
            {
                ++m_mlatTgtRepCounter.in;
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
                ++m_smrTgtRepCounter.in;
            }

            bool ok;
            IcaoAddr tgtAddr = record.valStrFromDitem(QLatin1String("I220"), QLatin1String("TAddr")).toUInt(&ok, 16);

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
                m_mlatTgtRepMultiMap.insert(record.m_dateTime, record);
                //std::sort(m_mlatTgtRepMultiMap.begin(), m_mlatTgtRepMultiMap.end(), sorter);
                ++m_mlatTgtRepCounter.out;
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
                m_smrTgtRepMultiMap.insert(record.m_dateTime, record);
                //std::sort(m_smrTgtRepMultiMap.begin(), m_smrTgtRepMultiMap.end(), sorter);
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

                m_mlatSrvMsgMultiMap.insert(record.m_dateTime, record);
                //std::sort(m_mlatSrvMsgMultiMap.begin(), m_mlatSrvMsgMultiMap.end(), sorter);
                ++m_mlatSrvMsgCounter.out;
            }
            else if (sic == m_smrSic)  // Primary Surveillance Radar.
            {
                ++m_smrSrvMsgCounter.in;

                m_smrSrvMsgMultiMap.insert(record.m_dateTime, record);
                //std::sort(m_smrSrvMsgMultiMap.begin(), m_smrSrvMsgMultiMap.end(), sorter);
                ++m_smrSrvMsgCounter.out;
            }
        }
    }
    else if (record.m_cat == 21)  // ADS-B Reports.
    {
        ++m_adsbTgtRepCounter.in;
        bool ok;
        IcaoAddr tgtAddr = record.valStrFromDitem(QLatin1String("I080"), QLatin1String("TAddr")).toUInt(&ok, 16);

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
        m_adsbTgtRepMultiMap.insert(record.m_dateTime, record);
        //std::sort(m_adsbTgtRepMultiMap.begin(), m_adsbTgtRepMultiMap.end(), sorter);
        ++m_adsbTgtRepCounter.out;
    }
}

void RecordCollator::loadExcludedAddresses(QIODevice *device)
{
    const QByteArray data = device->readAll();
    const QByteArrayList lines = data.split('\n');
    for (const auto &line : lines)
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

void RecordCollator::loadExcludedAddresses(const QVector<IcaoAddr> &excladdr)
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

QVector<IcaoAddr> RecordCollator::excludedAddresses() const
{
    return m_excludedAddresses;
}

QMultiMap<QDateTime, AsterixRecord> RecordCollator::smrTgtRepMultiMap() const
{
    return m_smrTgtRepMultiMap;
}

QMultiMap<QDateTime, AsterixRecord> RecordCollator::smrSrvMsgMultiMap() const
{
    return m_smrSrvMsgMultiMap;
}

QMultiMap<QDateTime, AsterixRecord> RecordCollator::mlatTgtRepMultiMap() const
{
    return m_mlatTgtRepMultiMap;
}

QMultiMap<QDateTime, AsterixRecord> RecordCollator::mlatSrvMsgMultiMap() const
{
    return m_mlatSrvMsgMultiMap;
}

QMultiMap<QDateTime, AsterixRecord> RecordCollator::adsbTgtRepMultiMap() const
{
    return m_adsbTgtRepMultiMap;
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
