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

    return checkDataItems(record, ed116TargetReportsHash());
}

bool MopsProcessor::ed117TargetReports(const AsterixRecord &record)
{
    // TODO: Add assertion that checks if record is a CAT010 MLAT target report.

    return checkDataItems(record, ed117TargetReportsHash());
}

double MopsProcessor::ed117UpdateRate(const AsterixRecord &record)
{
    // TODO: Add assertion that checks if record is a CAT010 MLAT target report.

    uint address = 0;
    uint tod = 0;

    for (const AsterixDataItem &di : record.dataItems)
    {
        AsterixDataElement el = di.fields.value(0).value<AsterixDataElement>();

        if (di.name == QLatin1String("I140"))
        {
            tod = el.value.toUInt();
        }
        if (di.name == QLatin1String("I220"))
        {
            address = el.value.toUInt();
        }
    }

    TargetData oldStatus = m_targetStatusHash.value(address);

    QDateTime newToD = QDateTime::fromSecsSinceEpoch(tod / 128);
    QDateTime firstToD = oldStatus.firstToD;
    qint64 diffToD = firstToD.secsTo(newToD);

    uint nRecords = oldStatus.nRecords + 1;

    double ur = (double)(nRecords / diffToD);
    return ur;
}

bool MopsProcessor::checkDataItems(const AsterixRecord &record,
    QHash<QString, bool> hash)
{
    /*
     * Count how many false there are at the start,
     * decrement that number when you flip one.
     * Stop when there's none left.
     */

    int count = hash.values().size();
    if (record.dataItems.size() >= count)
    {
        for (const AsterixDataItem &di : record.dataItems)
        {
            if (count == 0) break;

            QString diName = di.name;
            QHash<QString, bool>::iterator it = hash.find(diName);
            if (it != hash.end())
            {
                *it = true;
                --count;
            }
        }

        if (count == 0)
        {
            return true;
        }
    }

    return false;
}

QHash<QString, bool> MopsProcessor::ed116TargetReportsHash()
{
    QStringList list;
    list << QLatin1String("I000")   // Message Type
         << QLatin1String("I010")   // Data Source Identifier
         << QLatin1String("I020")   // Target Report Descriptor
         << QLatin1String("I140")   // Time of Day
         << QLatin1String("I041")   // Position in WGS-84 Coordinates
         << QLatin1String("I042")   // Position in Cartesian Coordinates
         << QLatin1String("I270")   // Target Size & Orientation
         << QLatin1String("I550");  // System Status

    return makeHash(list);
}

QHash<QString, bool> MopsProcessor::ed117TargetReportsHash()
{
    QStringList list;
    list << QLatin1String("I000")   // Message Type
         << QLatin1String("I010")   // Data Source Identifier
         << QLatin1String("I020")   // Target Report Descriptor
         << QLatin1String("I041")   // Position in WGS-84 Coordinates
         << QLatin1String("I042")   // Position in Cartesian Coordinates
         << QLatin1String("I060")   // Mode 3/A Code in Octal
         << QLatin1String("I091")   // Measured Height
         << QLatin1String("I140")   // Time of Day
         << QLatin1String("I161")   // Track Number
         << QLatin1String("I170")   // Track Status
         << QLatin1String("I220")   // Target Address
         << QLatin1String("I500");  // Standard Deviation of Position

    return makeHash(list);
}

QHash<QString, bool> MopsProcessor::ed117ServiceMessagesHash()
{
    static QStringList list = QStringList()
                              << QLatin1String("I000")   // Message Type
                              << QLatin1String("I010")   // Data Source Identifier
                              << QLatin1String("I140")   // Time of Day
                              << QLatin1String("I550");  // System Status
    static QHash<QString, bool> hash = makeHash(list);
    return hash;
}

QHash<QString, bool> MopsProcessor::makeHash(const QStringList &list, bool state)
{
    QHash<QString, bool> hash;

    for (const QString &key : list)
    {
        hash[key] = state;
    }

    return hash;
}
