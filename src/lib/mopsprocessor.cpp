#include "mopsprocessor.h"

MopsProcessor::MopsProcessor(QObject *parent) : QObject(parent)
{
}

void MopsProcessor::addRecord(const AsterixRecord &record)
{
    m_recordMap[record.cat].enqueue(record);
}

void MopsProcessor::addTarget(const MopsProcessor::TargetData &target)
{
    m_targetStatusHash[target.address] = target;
}

bool MopsProcessor::ed116TargetReports()
{
    return targetReports(ed116TargetReportsHash());
}

bool MopsProcessor::ed117TargetReports()
{
    return targetReports(ed117TargetReportsHash());
}

bool MopsProcessor::targetReports(QHash<QString, bool> hash)
{
    const quint8 cat = 10;

    Q_ASSERT(!m_recordMap[cat].isEmpty());

    AsterixRecord rcat10 = m_recordMap[cat].dequeue();
    QVariantList items = rcat10.dataItems;

    QVariantList::const_iterator it = items.constBegin();
    while (hash.values().contains(false) && it != items.constEnd())
    {
        const auto &diVariant = *it;
        AsterixDataItem di = diVariant.value<AsterixDataItem>();
        if (hash.contains(di.name))
        {
            hash[di.name] = true;
        }
        it++;
    }

    // TODO: Remove range-based for loop alternative (for discussion).
    /*
    for (const QVariant &it : items)
    {
        AsterixDataItem di = it.value<AsterixDataItem>();
        if (hash.contains(di.name))
        {
            hash[di.name] = true;
        }
    }
    */

    bool passed = !hash.values().contains(false);
    return passed;
}

QHash<QString, bool> MopsProcessor::ed116TargetReportsHash()
{
    QStringList list;
    list << "I000"   // Message Type
         << "I010"   // Data Source Identifier
         << "I020"   // Target Report Descriptor
         << "I140"   // Time of Day
         << "I041"   // Position in WGS-84 Coordinates
         << "I042"   // Position in Cartesian Coordinates
         << "I270"   // Target Size & Orientation
         << "I550";  // System Status

    return makeHash(list);
}

QHash<QString, bool> MopsProcessor::ed117TargetReportsHash()
{
    QStringList list;
    list << "I000"   // Message Type
         << "I010"   // Data Source Identifier
         << "I020"   // Target Report Descriptor
         << "I041"   // Position in WGS-84 Coordinates
         << "I042"   // Position in Cartesian Coordinates
         << "I060"   // Mode 3/A Code in Octal
         << "I091"   // Measured Height
         << "I140"   // Time of Day
         << "I161"   // Track Number
         << "I170"   // Track Status
         << "I220"   // Target Address
         << "I500";  // Standard Deviation of Position

    return makeHash(list);
}

QHash<QString, bool> MopsProcessor::ed117ServiceMessagesHash()
{
    QStringList list;
    list << "I000"   // Message Type
         << "I010"   // Data Source Identifier
         << "I140"   // Time of Day
         << "I550";  // System Status

    return makeHash(list);
}

QHash<QString, bool> MopsProcessor::makeHash(const QStringList &list, bool state)
{
    QHash<QString, bool> hash;

    for (QString key : list)
    {
        hash[key] = state;
    }

    return hash;
}
