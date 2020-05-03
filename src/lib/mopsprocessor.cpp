#include "mopsprocessor.h"

void MopsProcessor::setLocatePointCallback(std::function<Aerodrome::Area(const QPointF &)> callback)
{
    m_locatePoint = callback;
}

MopsProcessor::MopsProcessor(QObject *parent) : QObject(parent)
{
}

void MopsProcessor::processRecord(const AsterixRecord &record)
{
    if (record.cat == 10)
    {
        // TODO: Handle case when none of the following Data Items are present:
        // * I010/I000 Message Type
        // * I010/I020 Target Report Descriptor

        AsterixDataItem di010_000 = record.dataItems[QLatin1String("I000")];
        int msgType = di010_000.fields[0].value<AsterixDataElement>().value.toInt();

        /*
        QVariantList varlist = di010_000.fields;
        QVariant var = varlist[0];
        AsterixDataElement dele = var.value<AsterixDataElement>();
        QString str = dele.value;
        int msgType = str.toInt();
        */

        if (msgType == 1)  // Target Report.
        {
            AsterixDataItem di010_020 = record.dataItems[QLatin1String("I020")];
            int sysType = di010_020.fields[0].value<AsterixDataElement>().value.toInt();

            if (sysType == 1)  // Mode S Multilateration.
            {
                ++m_ed117TgtRepCounter.total;
                if (checkDataItems(record, ed117TargetReportsMinimumFieldsCollection()))
                {
                    // Target Report is valid. Update surveillance state.
                    ++m_ed117TgtRepCounter.n;
                }
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
                ++m_ed116TgtRepCounter.total;
                if (checkDataItems(record, ed116TargetReportsMinimumFieldsCollection()))
                {
                    // Target Report is valid. Update surveillance state.
                    ++m_ed116TgtRepCounter.n;
                }
            }
        }
        else if (msgType == 3)  // Periodic Status Message.
        {
            ++m_srvMsgCounter.total;
            if (checkDataItems(record, ed117ServiceMessagesMinimumFieldsCollection()))
            {
                // Status Message is valid. Update surveillance state.
                ++m_srvMsgCounter.n;
            }
        }
    }
}

double MopsProcessor::ed116TargetReportsMinimumFields()
{
    double num = static_cast<double>(m_ed116TgtRepCounter.n);
    double den = static_cast<double>(m_ed116TgtRepCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::ed117TargetReportsMinimumFields()
{
    double num = static_cast<double>(m_ed117TgtRepCounter.n);
    double den = static_cast<double>(m_ed117TgtRepCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::ed117ServiceMessagesMinimumFields()
{
    double num = static_cast<double>(m_srvMsgCounter.n);
    double den = static_cast<double>(m_srvMsgCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

bool MopsProcessor::checkDataItems(const AsterixRecord &record,
    const QVector<DataItemList> &collections)
{
    Q_ASSERT(!record.dataItems.isEmpty() && !collections.isEmpty());

    int count = collections.size();
    for (DataItemList diList : collections)
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
    Q_ASSERT(!record.dataItems.isEmpty() && !list.isEmpty());

    if (type == MopsProcessor::Disjunctive)
    {
        for (const AsterixDataItem &di : record.dataItems)
        {
            QString diName = di.name;
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
                // All items found.
                return true;
            }
        }
    }

    return false;
}

QVector<MopsProcessor::DataItemList> MopsProcessor::ed116TargetReportsMinimumFieldsCollection()
{
    QVector<DataItemList> minFields;

    DataItemList mandatory;
    mandatory.type = DataItemListType::Mandatory;
    mandatory.items = QStringList() << QLatin1String("I000")   // Message Type
                                    << QLatin1String("I010")   // Data Source Identifier
                                    << QLatin1String("I020")   // Target Report Descriptor
                                    << QLatin1String("I140")   // Time of Day
                                    << QLatin1String("I270");  // Target Size & Orientation
    //<< QLatin1String("I550");  // System Status

    DataItemList disjunctive;
    disjunctive.type = DataItemListType::Disjunctive;
    disjunctive.items = QStringList() << QLatin1String("I040")   // Position in Polar Co-ordinates
                                      << QLatin1String("I041")   // Position in WGS-84 Coordinates
                                      << QLatin1String("I042");  // Position in Cartesian Coordinates

    minFields << mandatory << disjunctive;
    return minFields;
}

QVector<MopsProcessor::DataItemList> MopsProcessor::ed117TargetReportsMinimumFieldsCollection()
{
    QVector<DataItemList> minFields;

    DataItemList mandatory;
    mandatory.type = DataItemListType::Mandatory;
    mandatory.items = QStringList() << QLatin1String("I000")   // Message Type
                                    << QLatin1String("I010")   // Data Source Identifier
                                    << QLatin1String("I020")   // Target Report Descriptor
                                    << QLatin1String("I060")   // Mode 3/A Code in Octal
                                    << QLatin1String("I091")   // Measured Height
                                    << QLatin1String("I140")   // Time of Day
                                    << QLatin1String("I161")   // Track Number
                                    << QLatin1String("I170")   // Track Status
                                    << QLatin1String("I220")   // Target Address
                                    << QLatin1String("I500");  // Standard Deviation of Position

    DataItemList disjunctive;
    disjunctive.type = DataItemListType::Disjunctive;
    disjunctive.items = QStringList() << QLatin1String("I040")   // Position in Polar Co-ordinates
                                      << QLatin1String("I041")   // Position in WGS-84 Coordinates
                                      << QLatin1String("I042");  // Position in Cartesian Coordinates

    minFields << mandatory << disjunctive;
    return minFields;
}

QVector<MopsProcessor::DataItemList> MopsProcessor::ed117ServiceMessagesMinimumFieldsCollection()
{
    QVector<DataItemList> minFields;

    DataItemList mandatory;
    mandatory.type = DataItemListType::Mandatory;
    mandatory.items = QStringList() << QLatin1String("I000")   // Message Type
                                    << QLatin1String("I010")   // Data Source Identifier
                                    << QLatin1String("I140")   // Time of Day
                                    << QLatin1String("I550");  // System Status

    minFields << mandatory;
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
