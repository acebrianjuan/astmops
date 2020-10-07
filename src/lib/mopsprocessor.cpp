/*!
 * \file mopsprocessor.cpp
 * \brief Implementation of the MopsProcessor class.
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

#include "mopsprocessor.h"
#include <QDebug>
#include <QSettings>

void MopsProcessor::setLocatePointCallback(std::function<Aerodrome::Area(const QPointF &)> callback)
{
    m_locatePoint = callback;
}

MopsProcessor::MopsProcessor(QObject *parent) : QObject(parent),
                                                m_smrSic(Configuration::smrSic()),
                                                m_mlatSic(Configuration::mlatSic()),
                                                m_adsbSic(Configuration::adsbSic()),
                                                m_ed116TgtRepMinDataItems(ed116TargetReportsMinimumDataItems()),
                                                m_ed117TgtRepMinDataItems(ed117TargetReportsMinimumDataItems()),
                                                m_srvMsgMinDataItems(serviceMessagesMinimumDataItems())
{
}

void MopsProcessor::processRecord(const AsterixRecord &record)
{
    if (record.m_cat == 10)
    {
        // TODO: Handle case when none of the following Data Items are present:
        // * I010/I000 Message Type
        // * I010/I020 Target Report Descriptor

        AsterixDataItem di010_010 = record.m_dataItems[QLatin1String("I010")];
        //quint8 sac = di010_010.m_fields[0].value<AsterixDataElement>().m_value.toUInt();
        quint8 sic = di010_010.m_fields[1].value<AsterixDataElement>().m_value.toUInt();

        AsterixDataItem di010_000 = record.m_dataItems[QLatin1String("I000")];
        int msgType = di010_000.m_fields[0].value<AsterixDataElement>().m_value.toInt();

        // TODO: Consider using enums instead of hardcoded numbers for the record triage.
        if (sic == m_smrSic && msgType == 1)
        {
            processCat010SmrTgtRep(record);
        }
        else if (sic == m_smrSic && msgType == 3)
        {
            processCat010SmrSrvMsg(record);
        }
        else if (sic == m_mlatSic && msgType == 1)
        {
            processCat010MlatTgtRep(record);
        }
        else if (sic == m_mlatSic && msgType == 3)
        {
            processCat010MlatSrvMsg(record);
        }
    }
}

double MopsProcessor::ed116TargetReportsMinimumFields()
{
    double num = static_cast<double>(m_cat010SmrTgtRepCounter.n);
    double den = static_cast<double>(m_cat010SmrTgtRepCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::ed116TargetReportsUpdateRate(Aerodrome::Area area)
{
    return calculateUpdateRate(m_cat010SmrTgtRepAreas.keys(area), m_ed116TgtRepUpdateRateCounters);
}

double MopsProcessor::ed116ServiceMessagesMinimumFields()
{
    double num = static_cast<double>(m_cat010SmrSrvMsgCounter.n);
    double den = static_cast<double>(m_cat010SmrSrvMsgCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::ed116ServiceMessagesUpdateRate()
{
    return calculateUpdateRate(m_cat010SmrSrvMsgUpdateRateCounter);
}

double MopsProcessor::ed117TargetReportsMinimumFields()
{
    double num = static_cast<double>(m_cat010MlatTgtRepCounter.n);
    double den = static_cast<double>(m_cat010MlatTgtRepCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::ed117TargetReportsUpdateRate(Aerodrome::Area area)
{
    return calculateUpdateRate(m_cat010MlatTgtRepAreas.keys(area), m_ed117TgtRepUpdateRateCounters);
}

double MopsProcessor::ed117ServiceMessagesMinimumFields()
{
    double num = static_cast<double>(m_cat010MlatSrvMsgCounter.n);
    double den = static_cast<double>(m_cat010MlatSrvMsgCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::ed117ServiceMessagesUpdateRate()
{
    return calculateUpdateRate(m_cat010MlatSrvMsgUpdateRateCounter);
}

void MopsProcessor::processCat010SmrTgtRep(const AsterixRecord &record)
{
    ++m_cat010SmrTgtRepCounter.total;

    // Minimum Data Items.
    if (!checkDataItems(record, m_ed116TgtRepMinDataItems))
    {
        // Target Report is invalid. Do not continue.
        return;
    }

    // Target Report is valid. Update surveillance state.
    ++m_cat010SmrTgtRepCounter.n;

    // Update Rate.
    AsterixDataItem di010_161 = record.m_dataItems[QLatin1String("I161")];
    TrackNum trkNum = di010_161.m_fields[1].value<AsterixDataElement>().m_value.toUInt();

    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    AsterixDataItem di010_042 = record.m_dataItems[QLatin1String("I042")];
    double x = di010_042.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    double y = di010_042.m_fields[1].value<AsterixDataElement>().m_value.toDouble();

    Aerodrome::Area area = m_locatePoint(QPointF(x, y));

    QHash<uint, Aerodrome::Area>::iterator itArea = m_cat010SmrTgtRepAreas.find(trkNum);
    QHash<uint, UpdateRateCounter>::iterator itCounter = m_ed116TgtRepUpdateRateCounters.find(trkNum);
    if (itCounter == m_ed116TgtRepUpdateRateCounters.end())
    {
        // Unknown target. Create a new counter for it.
        UpdateRateCounter urCounter;
        ++urCounter.n;
        urCounter.firstTod = todDateTime;
        urCounter.isInitialized = true;

        // Add it to the hash maps.
        m_ed116TgtRepUpdateRateCounters[trkNum] = urCounter;
        m_cat010SmrTgtRepAreas[trkNum] = area;
    }
    else
    {
        // Known target. Check area.
        Aerodrome::Area oldArea = itArea.value();
        if (area != oldArea)
        {
            // Area changed. Reset counter.
            *itArea = area;
            itCounter->reset();
            ++itCounter->n;
            itCounter->firstTod = todDateTime;
        }
        else
        {
            // Update counter.
            ++itCounter->n;
            itCounter->lastTod = todDateTime;
        }
    }
}

void MopsProcessor::processCat010SmrSrvMsg(const AsterixRecord &record)
{
    ++m_cat010SmrSrvMsgCounter.total;

    // Minimum Data Items.
    if (!checkDataItems(record, m_srvMsgMinDataItems))
    {
        // Status Message is invalid. Do not continue.
        return;
    }

    // Status Message is valid. Update surveillance state.
    ++m_cat010SmrSrvMsgCounter.n;

    // Update Rate.
    ++m_cat010SmrSrvMsgUpdateRateCounter.n;

    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    if (!m_cat010SmrSrvMsgUpdateRateCounter.isInitialized)
    {
        // First-time counter initialization.
        m_cat010SmrSrvMsgUpdateRateCounter.firstTod = todDateTime;
        m_cat010SmrSrvMsgUpdateRateCounter.isInitialized = true;
    }
    else
    {
        // Update counter.
        m_cat010SmrSrvMsgUpdateRateCounter.lastTod = todDateTime;
    }
}

void MopsProcessor::processCat010MlatTgtRep(const AsterixRecord &record)
{
    ++m_cat010MlatTgtRepCounter.total;

    // Minimum Data Items.
    if (!checkDataItems(record, m_ed117TgtRepMinDataItems))
    {
        // Target Report is invalid. Do not continue.
        return;
    }

    // Target Report is valid. Update surveillance state.
    ++m_cat010MlatTgtRepCounter.n;

    // Update Rate.
    AsterixDataItem di010_220 = record.m_dataItems[QLatin1String("I220")];
    IcaoAddr icaoAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    AsterixDataItem di010_042 = record.m_dataItems[QLatin1String("I042")];
    double x = di010_042.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    double y = di010_042.m_fields[1].value<AsterixDataElement>().m_value.toDouble();

    Aerodrome::Area area = m_locatePoint(QPointF(x, y));

    QHash<uint, Aerodrome::Area>::iterator itArea = m_cat010MlatTgtRepAreas.find(icaoAddr);
    QHash<uint, UpdateRateCounter>::iterator itCounter = m_ed117TgtRepUpdateRateCounters.find(icaoAddr);

    if (itCounter == m_ed117TgtRepUpdateRateCounters.end())
    {
        // Unknown target. Create a new counter for it.
        UpdateRateCounter urCounter;
        ++urCounter.n;
        urCounter.firstTod = todDateTime;
        urCounter.isInitialized = true;

        // Add it to the hash maps.
        m_ed117TgtRepUpdateRateCounters[icaoAddr] = urCounter;
        m_cat010MlatTgtRepAreas[icaoAddr] = area;
    }
    else
    {
        // Known target. Check area.
        Aerodrome::Area oldArea = itArea.value();
        if (area != oldArea)
        {
            // Area changed. Reset counter.
            *itArea = area;
            itCounter->reset();
            ++itCounter->n;
            itCounter->firstTod = todDateTime;
        }
        else
        {
            // Update counter.
            ++itCounter->n;
            itCounter->lastTod = todDateTime;
        }
    }
}

void MopsProcessor::processCat010MlatSrvMsg(const AsterixRecord &record)
{
    ++m_cat010MlatSrvMsgCounter.total;

    // Minimum Data Items.
    if (!checkDataItems(record, m_srvMsgMinDataItems))
    {
        // Status Message is invalid. Do not continue.
        return;
    }

    // Status Message is valid. Update surveillance state.
    ++m_cat010MlatSrvMsgCounter.n;

    // Update Rate.
    ++m_cat010MlatSrvMsgUpdateRateCounter.n;

    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    if (!m_cat010MlatSrvMsgUpdateRateCounter.isInitialized)
    {
        // First-time counter initialization.
        m_cat010MlatSrvMsgUpdateRateCounter.firstTod = todDateTime;
        m_cat010MlatSrvMsgUpdateRateCounter.isInitialized = true;
    }
    else
    {
        // Update counter.
        m_cat010MlatSrvMsgUpdateRateCounter.lastTod = todDateTime;
    }
}

bool MopsProcessor::checkDataItems(const AsterixRecord &record,
    const QVector<DataItemList> &collections)
{
    Q_ASSERT(!record.m_dataItems.isEmpty() && !collections.isEmpty());

    int count = collections.size();
    for (const DataItemList &diList : collections)
    {
        if (checkDataItemsList(record, diList.items, diList.type))
        {
            // Check succeeded. Decrease counter.
            --count;
        }
        else
        {
            // Check failed. Exit the loop.
            break;
        }
    }

    if (count == 0)
    {
        // All checks succeeded.
        return true;
    }

    return false;
}

bool MopsProcessor::checkDataItemsList(const AsterixRecord &record,
    const QStringList &list, DataItemListType type)
{
    Q_ASSERT(!record.m_dataItems.isEmpty() && !list.isEmpty());

    if (type == MopsProcessor::Disjunctive)
    {
        for (const AsterixDataItem &di : record.m_dataItems)
        {
            QString diName = di.m_name;
            if (list.contains(diName))
            {
                // One match is enough. Return true.
                return true;
            }
        }
    }
    else  // Assume type is MopsProcessor::Mandatory.
    {
        /*
         * Count how many false there are at the start,
         * decrement that number when you flip one.
         * Stop when there's none left.
         */
        QHash<QString, bool> hash = makeHash(list);
        int count = hash.values().size();
        if (record.m_dataItems.size() >= count)
        {
            for (const AsterixDataItem &di : record.m_dataItems)
            {
                if (count == 0) break;

                QString diName = di.m_name;
                QHash<QString, bool>::iterator it = hash.find(diName);
                if (it != hash.end())
                {
                    *it = true;
                    --count;
                }
            }

            if (count == 0)
            {
                // All items found.
                return true;
            }
        }
    }

    return false;
}

QVector<MopsProcessor::DataItemList> MopsProcessor::ed116TargetReportsMinimumDataItems()
{
    static QVector<DataItemList> minFields;

    static bool initialized = false;
    if (!initialized)
    {
        DataItemList mandatory;
        mandatory.type = DataItemListType::Mandatory;
        mandatory.items = QStringList() << QLatin1String("I010")   // Data Source Identifier
                                        << QLatin1String("I020")   // Target Report Descriptor
                                        << QLatin1String("I140")   // Time of Day
                                        << QLatin1String("I270");  // Target Size & Orientation

        DataItemList disjunctive;
        disjunctive.type = DataItemListType::Disjunctive;
        disjunctive.items = QStringList() << QLatin1String("I040")   // Position in Polar Co-ordinates
                                          << QLatin1String("I041")   // Position in WGS-84 Coordinates
                                          << QLatin1String("I042");  // Position in Cartesian Coordinates

        minFields << mandatory << disjunctive;
        initialized = true;
    }

    return minFields;
}

QVector<MopsProcessor::DataItemList> MopsProcessor::ed117TargetReportsMinimumDataItems()
{
    static QVector<DataItemList> minFields;

    static bool initialized = false;
    if (!initialized)
    {
        DataItemList mandatory;
        mandatory.type = DataItemListType::Mandatory;
        mandatory.items = QStringList() << QLatin1String("I010")   // Data Source Identifier
                                        << QLatin1String("I020")   // Target Report Descriptor
                                        << QLatin1String("I140")   // Time of Day
                                        << QLatin1String("I220");  // Mode S Target Address (ICAO)

        DataItemList disjunctive1;
        disjunctive1.type = DataItemListType::Disjunctive;
        disjunctive1.items = QStringList() << QLatin1String("I041")   // Position in WGS-84 Coordinates
                                           << QLatin1String("I042");  // Position in Cartesian Coordinates

        DataItemList disjunctive2;
        disjunctive2.type = DataItemListType::Disjunctive;
        disjunctive2.items = QStringList() << QLatin1String("I060")   // Mode 3/A Code in Octal (SQUAWK)
                                           << QLatin1String("I245");  // Target Identification (CALLSIGN)

        minFields << mandatory << disjunctive1 << disjunctive2;
        initialized = true;
    }

    return minFields;
}

QVector<MopsProcessor::DataItemList> MopsProcessor::serviceMessagesMinimumDataItems()
{
    static QVector<DataItemList> minFields;

    static bool initialized = false;
    if (!initialized)
    {
        DataItemList mandatory;
        mandatory.type = DataItemListType::Mandatory;
        mandatory.items = QStringList() << QLatin1String("I000")   // Message Type
                                        << QLatin1String("I010")   // Data Source Identifier
                                        << QLatin1String("I140")   // Time of Day
                                        << QLatin1String("I550");  // System Status

        minFields << mandatory;
        initialized = true;
    }

    return minFields;
}

QHash<QString, bool> MopsProcessor::makeHash(const QStringList &list, bool state)
{
    Q_ASSERT(!list.isEmpty());

    QHash<QString, bool> hash;

    for (const QString &key : list)
    {
        hash[key] = state;
    }

    return hash;
}

QDateTime MopsProcessor::getDateTimefromTod(const double &tod)
{
    QDateTime todDateTime = QDateTime::fromMSecsSinceEpoch(tod * 1000, Qt::UTC);
    return todDateTime;
}

double MopsProcessor::calculateUpdateRate(const QList<quint32> &addresses, const QHash<quint32, MopsProcessor::UpdateRateCounter> &counters)
{
    double num = 0.0;
    double den = 0.0;

    if (addresses.isEmpty())
    {
        // Cannot perform calculation.
        return qQNaN();
    }

    for (uint addr : addresses)
    {
        UpdateRateCounter urCounter = counters.value(addr);

        if (urCounter)
        {
            QDateTime firstTod = urCounter.firstTod;
            QDateTime lastTod = urCounter.lastTod;

            if (!firstTod.isNull() && !lastTod.isNull())
            {
                num += static_cast<double>(urCounter.n);
                den += static_cast<double>(firstTod.msecsTo(lastTod)) / 1000 + 1;
            }
        }
    }

    if (den == 0)
    {
        return qQNaN();
    }

    double p = num / den;
    return p;
}

double MopsProcessor::calculateUpdateRate(const MopsProcessor::UpdateRateCounter &counter)
{
    double num = 0.0;
    double den = 0.0;

    if (counter)
    {
        QDateTime firstTod = counter.firstTod;
        QDateTime lastTod = counter.lastTod;

        if (!firstTod.isNull() && !lastTod.isNull())
        {
            num += static_cast<double>(counter.n);
            den += static_cast<double>(firstTod.msecsTo(lastTod)) / 1000 + 1;
        }
    }

    if (den == 0)
    {
        return qQNaN();
    }

    double p = num / den;
    return p;
}
