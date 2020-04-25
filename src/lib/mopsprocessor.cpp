#include "mopsprocessor.h"

MopsProcessor::MopsProcessor(QObject *parent) : QObject(parent)
{
}

void MopsProcessor::processRecord(const AsterixRecord &record)
{
    if (record.cat == 10)
    {
    }
}

double MopsProcessor::ed117TargetReportsMinimumFields()
{
    double num = static_cast<double>(tgtRepCounter.n);
    double den = static_cast<double>(tgtRepCounter.total);

    double p = num / den;
    return p;
}

double MopsProcessor::ed117ServiceMessagesMinimumFields()
{
    double num = static_cast<double>(srvMsgCounter.n);
    double den = static_cast<double>(srvMsgCounter.total);

    double p = num / den;
    return p;
}

bool MopsProcessor::checkDataItems(const AsterixRecord &record, const QStringList &list)
{
    /*
     * Count how many false there are at the start,
     * decrement that number when you flip one.
     * Stop when there's none left.
     */

    QHash<QString, bool> hash = makeHash(list);
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

QStringList MopsProcessor::ed116TargetReportsList()
{
    static const QStringList list = QStringList()
                                    << QLatin1String("I000")   // Message Type
                                    << QLatin1String("I010")   // Data Source Identifier
                                    << QLatin1String("I020")   // Target Report Descriptor
                                    << QLatin1String("I140")   // Time of Day
                                    << QLatin1String("I041")   // Position in WGS-84 Coordinates
                                    << QLatin1String("I042")   // Position in Cartesian Coordinates
                                    << QLatin1String("I270")   // Target Size & Orientation
                                    << QLatin1String("I550");  // System Status

    return list;
}

QStringList MopsProcessor::ed117TargetReportsList()
{
    static const QStringList list = QStringList()
                                    << QLatin1String("I000")   // Message Type
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

    return list;
}

QStringList MopsProcessor::ed117ServiceMessagesList()
{
    static const QStringList list = QStringList()
                                    << QLatin1String("I000")   // Message Type
                                    << QLatin1String("I010")   // Data Source Identifier
                                    << QLatin1String("I140")   // Time of Day
                                    << QLatin1String("I550");  // System Status

    return list;
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
