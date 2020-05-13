#include "mopsprocessor.h"

void MopsProcessor::setLocatePointCallback(std::function<Aerodrome::Area(const QPointF &)> callback)
{
    m_locatePoint = callback;
}

MopsProcessor::MopsProcessor(QObject *parent) : QObject(parent)
{
    m_ed116TgtRepMinDataItems = ed116TargetReportsMinimumDataItems();
    m_ed117TgtRepMinDataItems = ed117TargetReportsMinimumDataItems();
    m_srvMsgMinDataItems = serviceMessagesMinimumDataItems();
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

        if (msgType == 1)  // Target Report.
        {
            AsterixDataItem di010_020 = record.dataItems[QLatin1String("I020")];
            int sysType = di010_020.fields[0].value<AsterixDataElement>().value.toInt();

            if (sysType == 1)  // Mode S Multilateration. ED-117 Norm.
            {
                // Minimum Data Items.
                ++m_ed117TgtRepCounter.total;
                if (checkDataItems(record, m_ed117TgtRepMinDataItems))
                {
                    // Target Report is valid. Update surveillance state.
                    ++m_ed117TgtRepCounter.n;
                }

                // Update Rate.
                AsterixDataItem di010_220 = record.dataItems[QLatin1String("I220")];
                IcaoAddr icaoAddr = di010_220.fields[0].value<AsterixDataElement>().value.toUInt();

                AsterixDataItem di010_140 = record.dataItems[QLatin1String("I140")];
                double tod = di010_140.fields[0].value<AsterixDataElement>().value.toDouble();
                QDateTime todDateTime = getDateTimefromTod(tod);

                AsterixDataItem di010_042 = record.dataItems[QLatin1String("I042")];
                double x = di010_042.fields[0].value<AsterixDataElement>().value.toDouble();
                double y = di010_042.fields[1].value<AsterixDataElement>().value.toDouble();

                Aerodrome::Area area = m_locatePoint(QPointF(x, y));

                QHash<uint, Aerodrome::Area>::iterator itArea = m_ed117TgtRepAreas.find(icaoAddr);
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
                    m_ed117TgtRepAreas[icaoAddr] = area;
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
            else if (sysType == 3)  // Primary Surveillance Radar. ED-116 Norm.
            {
                // Minimum Data Items.
                ++m_ed116TgtRepCounter.total;
                if (checkDataItems(record, m_ed116TgtRepMinDataItems))
                {
                    // Target Report is valid. Update surveillance state.
                    ++m_ed116TgtRepCounter.n;
                }

                // Update Rate.
                AsterixDataItem di010_161 = record.dataItems[QLatin1String("I161")];
                TrackNum trkNum = di010_161.fields[1].value<AsterixDataElement>().value.toUInt();

                AsterixDataItem di010_140 = record.dataItems[QLatin1String("I140")];
                double tod = di010_140.fields[0].value<AsterixDataElement>().value.toDouble();
                QDateTime todDateTime = getDateTimefromTod(tod);

                QHash<uint, UpdateRateCounter>::iterator it = m_ed116TgtRepUpdateRateCounters.find(trkNum);
                if (it == m_ed116TgtRepUpdateRateCounters.end())
                {
                    // Unknown target. Create a new counter for it.
                    UpdateRateCounter urCounter;
                    ++urCounter.n;
                    urCounter.firstTod = todDateTime;
                    urCounter.isInitialized = true;

                    // Add it to the hash map.
                    m_ed116TgtRepUpdateRateCounters[trkNum] = urCounter;
                }
                else
                {
                    // Known target. Update counter.
                    ++it->n;
                    it->lastTod = todDateTime;
                }
            }
        }
        else if (msgType == 3)  // Periodic Status Message.
        {
            // Minimum Data Items.
            ++m_srvMsgCounter.total;
            if (checkDataItems(record, m_srvMsgMinDataItems))
            {
                // Status Message is valid. Update surveillance state.
                ++m_srvMsgCounter.n;
            }

            // Update Rate.
            ++m_srvMsgUpdateRateCounter.n;

            AsterixDataItem di010_140 = record.dataItems[QLatin1String("I140")];
            double tod = di010_140.fields[0].value<AsterixDataElement>().value.toDouble();
            QDateTime todDateTime = getDateTimefromTod(tod);

            if (!m_srvMsgUpdateRateCounter.isInitialized)
            {
                // First-time counter initialization.
                m_srvMsgUpdateRateCounter.firstTod = todDateTime;
                m_srvMsgUpdateRateCounter.isInitialized = true;
            }
            else
            {
                // Update counter.
                m_srvMsgUpdateRateCounter.lastTod = todDateTime;
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

double MopsProcessor::ed116TargetReportsUpdateRate()
{
    double num = 0.0;
    double den = 0.0;

    for (UpdateRateCounter urCounter : m_ed116TgtRepUpdateRateCounters)
    {
        if (urCounter.isInitialized)
        {
            QDateTime firstTod = urCounter.firstTod;
            QDateTime lastTod = urCounter.lastTod;

            if (!firstTod.isNull() && !lastTod.isNull())
            {
                num += static_cast<double>(urCounter.n);
                den += static_cast<double>(firstTod.secsTo(lastTod) + 1);
            }
        }
    }

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

double MopsProcessor::ed117TargetReportsUpdateRate(Aerodrome::Area area)
{
    double num = 0.0;
    double den = 0.0;

    QList<uint> addressList = m_ed117TgtRepAreas.keys(area);

    if (addressList.isEmpty())
    {
        // Cannot perform calculation.
        // TODO: Handle this case. Consider using std::optional.
    }

    for (uint addr : addressList)
    {
        UpdateRateCounter urCounter = m_ed117TgtRepUpdateRateCounters.value(addr);

        if (urCounter.isInitialized)
        {
            QDateTime firstTod = urCounter.firstTod;
            QDateTime lastTod = urCounter.lastTod;

            if (!firstTod.isNull() && !lastTod.isNull())
            {
                num += static_cast<double>(urCounter.n);
                den += static_cast<double>(firstTod.secsTo(lastTod) + 1);
            }
        }
    }

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::serviceMessagesMinimumFields()
{
    double num = static_cast<double>(m_srvMsgCounter.n);
    double den = static_cast<double>(m_srvMsgCounter.total);

    Q_ASSERT(den > 0);

    double p = num / den;
    return p;
}

double MopsProcessor::serviceMessagesUpdateRate()
{
    double num = static_cast<double>(m_srvMsgUpdateRateCounter.n);
    double den = static_cast<double>(m_srvMsgUpdateRateCounter.firstTod.secsTo(m_srvMsgUpdateRateCounter.lastTod) + 1);

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
                                        << QLatin1String("I220");  // Target Address (ICAO)

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
