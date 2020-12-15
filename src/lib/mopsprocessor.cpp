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

void MopsProcessor::setLocatePointCallback(const std::function<Aerodrome::Area(const QPointF &)> &callback)
{
    m_locatePoint = callback;
}

MopsProcessor::MopsProcessor(QObject *parent) : QObject(parent)
{
}

void MopsProcessor::processRecord(const AsterixRecord &record)
{
    if (record.m_cat == 10)
    {
        // TODO: Handle case when any of the following Data Items are missing:
        // * I010/000 Message Type
        // * I010/010 Data Source Identifier
        // * I010/020 Target Report Descriptor

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

double MopsProcessor::ed116TgtRepMinimumFields()
{
    double num = m_cat010SmrTgtRepCounter.countValid;
    double den = m_cat010SmrTgtRepCounter.countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double p = num / den;
    return p;
}

double MopsProcessor::ed116TgtRepUpdateRate(Aerodrome::Area area)
{
    double num = m_cat010SmrTgtRepUpdateRateTable.value(area).countValid;
    double den = m_cat010SmrTgtRepUpdateRateTable.value(area).countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double ur = num / den;
    return ur;
}

double MopsProcessor::ed116TgtRepProbDetection(Aerodrome::Area area)
{
    double num = m_cat010SmrTgtRepProbDetectionTable.value(area).countValid;
    double den = m_cat010SmrTgtRepProbDetectionTable.value(area).countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double pd = num / den;
    return pd;
}

double MopsProcessor::ed116SrvMsgMinimumFields()
{
    double num = m_cat010SmrSrvMsgCounter.countValid;
    double den = m_cat010SmrSrvMsgCounter.countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double p = num / den;
    return p;
}

double MopsProcessor::ed116SrvMsgUpdateRate()
{
    double num = m_cat010SmrSrvMsgUpdateRateTable.countValid;
    double den = m_cat010SmrSrvMsgUpdateRateTable.countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double ur = num / den;
    return ur;
}

double MopsProcessor::ed117TgtRepMinimumFields()
{
    double num = m_cat010MlatTgtRepCounter.countValid;
    double den = m_cat010MlatTgtRepCounter.countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double p = num / den;
    return p;
}

double MopsProcessor::ed117TgtRepUpdateRate(Aerodrome::Area area)
{
    double num = m_cat010MlatTgtRepUpdateRateTable.value(area).countValid;
    double den = m_cat010MlatTgtRepUpdateRateTable.value(area).countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double ur = num / den;
    return ur;
}

double MopsProcessor::ed117TgtRepProbDetection(Aerodrome::Area area)
{
    double num = m_cat010MlatTgtRepProbDetectionTable.value(area).countValid;
    double den = m_cat010MlatTgtRepProbDetectionTable.value(area).countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double pd = num / den;
    return pd;
}

double MopsProcessor::ed117SrvMsgMinimumFields()
{
    double num = m_cat010MlatSrvMsgCounter.countValid;
    double den = m_cat010MlatSrvMsgCounter.countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double p = num / den;
    return p;
}

double MopsProcessor::ed117SrvMsgUpdateRate()
{
    double num = m_cat010MlatSrvMsgUpdateRateTable.countValid;
    double den = m_cat010MlatSrvMsgUpdateRateTable.countTotal;

    if (den == 0)
    {
        return qQNaN();
    }

    double ur = num / den;
    return ur;
}

void MopsProcessor::processCat010SmrTgtRep(const AsterixRecord &record)
{
    if (!cat010SmrTgtRepMinDataItems(record))
    {
        return;
    }

    cat010SmrTgtRepUpdateRate(record);
    cat010SmrTgtRepProbDetection(record);
}

void MopsProcessor::processCat010SmrSrvMsg(const AsterixRecord &record)
{
    if (!cat010SmrSrvMsgMinDataItems(record))
    {
        return;
    }

    cat010SmrSrvMsgUpdateRate(record);
}

void MopsProcessor::processCat010MlatTgtRep(const AsterixRecord &record)
{
    if (!cat010MlatTgtRepMinDataItems(record))
    {
        return;
    }

    cat010MlatTgtRepUpdateRate(record);
    cat010MlatTgtRepProbDetection(record);
}

void MopsProcessor::processCat010MlatSrvMsg(const AsterixRecord &record)
{
    if (!cat010MlatSrvMsgMinDataItems(record))
    {
        return;
    }

    cat010MlatSrvMsgUpdateRate(record);
}

bool MopsProcessor::cat010SmrTgtRepMinDataItems(const AsterixRecord &record)
{
    ++m_cat010SmrTgtRepCounter.countTotal;

    // Minimum Data Items.
    if (!checkDataItems(record, m_ed116TgtRepMinDataItems))
    {
        // Target Report is invalid. Do not continue.
        return false;
    }

    // Target Report is valid. Update surveillance state.
    ++m_cat010SmrTgtRepCounter.countValid;

    return true;
}

void MopsProcessor::cat010SmrTgtRepUpdateRate(const AsterixRecord &record)
{
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

    QHash<TrackNum, TargetData>::iterator itTarget = m_cat010SmrTgtRepUpdateRateCounters.find(trkNum);

    if (itTarget == m_cat010SmrTgtRepUpdateRateCounters.end())
    {
        // Unknown target. Create a new counter for it.
        TargetData targetData;
        targetData.area = area;

        // Add it to the hash maps.
        itTarget = m_cat010SmrTgtRepUpdateRateCounters.insert(trkNum, targetData);
    }
    else
    {
        // Known target. Check area.
        Aerodrome::Area oldArea = itTarget->area;
        if (area != oldArea || itTarget->updateRateCounter.intervalStart().msecsTo(todDateTime) / 1000 > m_silencePeriod)
        {
            // Area changed or Silence Period. Reset counter.
            itTarget->updateRateCounter.reset();
            itTarget->area = area;
        }
    }

    // Update counter.
    itTarget->updateRateCounter.update(todDateTime);
    Counters::BasicCounter counter = itTarget->updateRateCounter.read();

    m_cat010SmrTgtRepUpdateRateTable[area].countValid += counter.countValid;
    m_cat010SmrTgtRepUpdateRateTable[area].countTotal += counter.countTotal;
}

void MopsProcessor::cat010SmrTgtRepProbDetection(const AsterixRecord &record)
{
    // Probability of Detection.
    if (!containsPosition(record))
    {
        return;
    }

    AsterixDataItem di010_161 = record.m_dataItems[QLatin1String("I161")];
    TrackNum trkNum = di010_161.m_fields[1].value<AsterixDataElement>().m_value.toUInt();

    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    AsterixDataItem di010_042 = record.m_dataItems[QLatin1String("I042")];
    double x = di010_042.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    double y = di010_042.m_fields[1].value<AsterixDataElement>().m_value.toDouble();

    Aerodrome::Area area = m_locatePoint(QPointF(x, y));

    if (area == Aerodrome::Area::None)
    {
        return;
    }

    double period = Configuration::probDetectionPeriod(area);

    QHash<IcaoAddr, TargetData>::iterator itTarget = m_cat010SmrTgtRepProbDetectionCounters.find(trkNum);

    if (itTarget == m_cat010SmrTgtRepProbDetectionCounters.end())
    {
        // Unknown target. Create a new counter for it.
        TargetData targetData;
        targetData.probDetectionCounter.setPeriod(period);
        targetData.area = area;

        // Add it to the hash maps.
        itTarget = m_cat010SmrTgtRepProbDetectionCounters.insert(trkNum, targetData);
    }
    else
    {
        // Known target. Check area.
        Aerodrome::Area oldArea = itTarget->area;
        if (area != oldArea || itTarget->probDetectionCounter.intervalStart().msecsTo(todDateTime) / 1000 > m_silencePeriod)
        {
            // Area changed or Silence Period. Reset counter.
            itTarget->probDetectionCounter.reset();
            itTarget->probDetectionCounter.setPeriod(period);
            itTarget->area = area;
        }
    }

    // Update counter.
    itTarget->probDetectionCounter.update(todDateTime);
    Counters::BasicCounter counter = itTarget->probDetectionCounter.read();

    m_cat010SmrTgtRepProbDetectionTable[area].countValid += counter.countValid;
    m_cat010SmrTgtRepProbDetectionTable[area].countTotal += counter.countTotal;
}

bool MopsProcessor::cat010SmrSrvMsgMinDataItems(const AsterixRecord &record)
{
    ++m_cat010SmrSrvMsgCounter.countTotal;

    // Minimum Data Items.
    if (!checkDataItems(record, m_srvMsgMinDataItems))
    {
        // Status Message is invalid. Do not continue.
        return false;
    }

    // Status Message is valid. Update surveillance state.
    ++m_cat010SmrSrvMsgCounter.countValid;

    return true;
}

void MopsProcessor::cat010SmrSrvMsgUpdateRate(const AsterixRecord &record)
{
    // Update Rate.
    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    // Update counter.
    m_cat010SmrSrvMsgUpdateRateCounter.update(todDateTime);
    Counters::BasicCounter counter = m_cat010SmrSrvMsgUpdateRateCounter.read();

    m_cat010SmrSrvMsgUpdateRateTable.countValid += counter.countValid;
    m_cat010SmrSrvMsgUpdateRateTable.countTotal += counter.countTotal;
}

bool MopsProcessor::cat010MlatTgtRepMinDataItems(const AsterixRecord &record)
{
    ++m_cat010MlatTgtRepCounter.countTotal;

    // Minimum Data Items.
    if (!checkDataItems(record, m_ed117TgtRepMinDataItems))
    {
        // Target Report is invalid. Do not continue.
        return false;
    }

    // Target Report is valid. Update surveillance state.
    ++m_cat010MlatTgtRepCounter.countValid;

    return true;
}

void MopsProcessor::cat010MlatTgtRepUpdateRate(const AsterixRecord &record)
{
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

    QHash<IcaoAddr, TargetData>::iterator itTarget = m_cat010MlatTgtRepUpdateRateCounters.find(icaoAddr);

    if (itTarget == m_cat010MlatTgtRepUpdateRateCounters.end())
    {
        // Unknown target. Create a new counter for it.
        TargetData targetData;
        targetData.area = area;

        // Add it to the hash maps.
        itTarget = m_cat010MlatTgtRepUpdateRateCounters.insert(icaoAddr, targetData);
    }
    else
    {
        // Known target. Check area.
        Aerodrome::Area oldArea = itTarget->area;
        if (area != oldArea || itTarget->updateRateCounter.intervalStart().msecsTo(todDateTime) / 1000 > m_silencePeriod)
        {
            // Area changed or Silence Period. Reset counter.
            itTarget->updateRateCounter.reset();
            itTarget->area = area;
        }
    }

    // Update counter.
    itTarget->updateRateCounter.update(todDateTime);
    Counters::BasicCounter counter = itTarget->updateRateCounter.read();

    m_cat010MlatTgtRepUpdateRateTable[area].countValid += counter.countValid;
    m_cat010MlatTgtRepUpdateRateTable[area].countTotal += counter.countTotal;
}

void MopsProcessor::cat010MlatTgtRepProbDetection(const AsterixRecord &record)
{
    // Probability of Detection.
    if (!containsPosition(record))
    {
        return;
    }

    AsterixDataItem di010_220 = record.m_dataItems[QLatin1String("I220")];
    IcaoAddr icaoAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    AsterixDataItem di010_042 = record.m_dataItems[QLatin1String("I042")];
    double x = di010_042.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    double y = di010_042.m_fields[1].value<AsterixDataElement>().m_value.toDouble();

    Aerodrome::Area area = m_locatePoint(QPointF(x, y));

    if (area == Aerodrome::Area::None)
    {
        return;
    }

    double period = Configuration::probDetectionPeriod(area);

    QHash<IcaoAddr, TargetData>::iterator itTarget = m_cat010MlatTgtRepProbDetectionCounters.find(icaoAddr);

    if (itTarget == m_cat010MlatTgtRepProbDetectionCounters.end())
    {
        // Unknown target. Create a new counter for it.
        TargetData targetData;
        targetData.probDetectionCounter.setPeriod(period);
        targetData.area = area;

        // Add it to the hash maps.
        itTarget = m_cat010MlatTgtRepProbDetectionCounters.insert(icaoAddr, targetData);
    }
    else
    {
        // Known target. Check area.
        Aerodrome::Area oldArea = itTarget->area;
        if (area != oldArea || itTarget->probDetectionCounter.intervalStart().msecsTo(todDateTime) / 1000 > m_silencePeriod)
        {
            // Area changed or Silence Period. Reset counter.
            itTarget->probDetectionCounter.reset();
            itTarget->probDetectionCounter.setPeriod(period);
            itTarget->area = area;
        }
    }

    // Update counter.
    itTarget->probDetectionCounter.update(todDateTime);
    Counters::BasicCounter counter = itTarget->probDetectionCounter.read();

    m_cat010MlatTgtRepProbDetectionTable[area].countValid += counter.countValid;
    m_cat010MlatTgtRepProbDetectionTable[area].countTotal += counter.countTotal;
}

bool MopsProcessor::cat010MlatSrvMsgMinDataItems(const AsterixRecord &record)
{
    ++m_cat010MlatSrvMsgCounter.countTotal;

    // Minimum Data Items.
    if (!checkDataItems(record, m_srvMsgMinDataItems))
    {
        // Status Message is invalid. Do not continue.
        return false;
    }

    // Status Message is valid. Update surveillance state.
    ++m_cat010MlatSrvMsgCounter.countValid;

    return true;
}

void MopsProcessor::cat010MlatSrvMsgUpdateRate(const AsterixRecord &record)
{
    // Update Rate.
    AsterixDataItem di010_140 = record.m_dataItems[QLatin1String("I140")];
    double tod = di010_140.m_fields[0].value<AsterixDataElement>().m_value.toDouble();
    QDateTime todDateTime = getDateTimefromTod(tod);

    // Update counter.
    m_cat010MlatSrvMsgUpdateRateCounter.update(todDateTime);
    Counters::BasicCounter counter = m_cat010MlatSrvMsgUpdateRateCounter.read();

    m_cat010MlatSrvMsgUpdateRateTable.countValid += counter.countValid;
    m_cat010MlatSrvMsgUpdateRateTable.countTotal += counter.countTotal;
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
        int count = hash.size();
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

bool MopsProcessor::containsPosition(const AsterixRecord &record)
{
    QStringList cat010PosDItems;
    cat010PosDItems << QLatin1String("I040")   // Position in Polar Co-ordinates
                    << QLatin1String("I041")   // Position in WGS-84 Coordinates
                    << QLatin1String("I042");  // Position in Cartesian Coordinates

    return checkDataItemsList(record, cat010PosDItems, DataItemListType::Disjunctive);
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

QDateTime MopsProcessor::getDateTimefromTod(double tod)
{
    QDateTime todDateTime = QDateTime::fromMSecsSinceEpoch(tod * 1000, Qt::UTC);
    return todDateTime;
}
