#include "mopsprocessor.h"

MopsProcessor::MopsProcessor(QObject *parent) : QObject(parent)
{
}

void MopsProcessor::addTarget(const MopsProcessor::TargetData &target)
{
    m_targetStatusHash[target.address] = target;
}

bool MopsProcessor::ed116TargetReports(const AsterixRecord &record)
{
    // TODO: Add assertion that checks if record is a CAT010 SMR target report.
    return targetReports(record, ed116TargetReportsHash());
}

bool MopsProcessor::ed117TargetReports(const AsterixRecord &record)
{
    // TODO: Add assertion that checks if record is a CAT010 MLAT target report.
    return targetReports(record, ed117TargetReportsHash());
}

void MopsProcessor::ed117UpdateRate(const AsterixRecord &record)
{
    // TODO: Add assertion that checks if record is a CAT010 MLAT target report.
}

bool MopsProcessor::targetReports(const AsterixRecord &record,
    QHash<QString, bool> hash)
{
    /*
     * Count how many false there are at the start,
     * decrement that number when you flip one.
     * Stop when there's none left.
     */

    int count = hash.values().size();
    for (const QVariant &it : record.dataItems)
    {
        if (count == 0) break;

        QString diName = it.value<AsterixDataItem>().name;
        QHash<QString, bool>::iterator hit = hash.find(diName);
        if (hit != hash.end())
        {
            *hit = true;
            --count;
        }
    }

    if (count == 0)
    {
        return true;
    }

    return false;
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
