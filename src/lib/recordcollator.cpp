#include "recordcollator.h"
#include <algorithm>

RecordCollator::RecordCollator(QObject *parent) : QObject(parent)
{
}

void RecordCollator::processRecord(const AsterixRecord &record)
{
    // Classify record by System Type (smr, mlat, adsb).
    // Filter out if record is an excluded address.
    // Sort records by Time of Day.

    // TODO: Handle cases with no Target Address information.

    auto sorter = [](const AsterixRecord &lhs, const AsterixRecord &rhs) {
        return lhs.m_dateTime < rhs.m_dateTime;
    };

    if (record.m_cat == 10)  // Monosensor Surface Movement Data.
    {
        AsterixDataItem di010_000 = record.m_dataItems[QLatin1String("I000")];
        quint8 msgType = di010_000.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

        if (msgType == 1)  // Target Report.
        {
            AsterixDataItem di010_020 = record.m_dataItems[QLatin1String("I020")];
            quint8 sysType = di010_020.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            bool ok;
            AsterixDataItem di010_220 = record.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt(&ok, 16);

            if (ok)  // Valid Target Address.
            {
                // Do not continue if Target Address is an Excluded Address.
                if (m_excludedAddresses.contains(tgtAddr))
                {
                    return;
                }

                if (sysType == 1)  // Mode S Multilateration.
                {
                    m_mlatQueue.enqueue(record);
                    std::sort(m_mlatQueue.begin(), m_mlatQueue.end(), sorter);
                }
                else if (sysType == 3)  // Primary Surveillance Radar.
                {
                    m_smrQueue.enqueue(record);
                    std::sort(m_smrQueue.begin(), m_smrQueue.end(), sorter);
                }
            }
            else  // Invalid Target Address.
            {
                // Add Record to the queue anyway.
                m_mlatQueue.enqueue(record);
                std::sort(m_mlatQueue.begin(), m_mlatQueue.end(), sorter);
            }
        }
        else if (msgType == 3)  // Periodic Status Message.
        {
            // TODO: Determine if there System Type concept applies to Service Messages.

            /*
            if (sysType == 1)  // Mode S Multilateration.
            {
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
            }
            */

            // Service Messages are always enqueued.
            m_srvMsgQueue.enqueue(record);
            std::sort(m_srvMsgQueue.begin(), m_srvMsgQueue.end(), sorter);
        }
    }
    else if (record.m_cat == 21)  // ADS-B Reports.
    {
        bool ok;
        AsterixDataItem di021_080 = record.m_dataItems[QLatin1String("I080")];
        IcaoAddr tgtAddr = di021_080.m_fields[0].value<AsterixDataElement>().m_value.toUInt(&ok, 16);

        if (ok)  // Valid Target Address.
        {
            // Do not continue if Target Address is an Excluded Address.
            if (m_excludedAddresses.contains(tgtAddr))
            {
                return;
            }

            m_adsbQueue.enqueue(record);
            std::sort(m_adsbQueue.begin(), m_adsbQueue.end(), sorter);
        }
        else  // Invalid Target Address.
        {
            // Add Record to the queue anyway.
            m_adsbQueue.enqueue(record);
            std::sort(m_adsbQueue.begin(), m_adsbQueue.end(), sorter);
        }
    }
}

void RecordCollator::loadExcludedAddresses(QIODevice *device)
{
    QByteArray data = device->readAll();
    for (auto line : data.split('\n'))
    {
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

QVector<IcaoAddr> RecordCollator::excludedAddresses() const
{
    return m_excludedAddresses;
}

QQueue<AsterixRecord> RecordCollator::smrQueue() const
{
    return m_smrQueue;
}

QQueue<AsterixRecord> RecordCollator::mlatQueue() const
{
    return m_mlatQueue;
}

QQueue<AsterixRecord> RecordCollator::adsbQueue() const
{
    return m_adsbQueue;
}

QQueue<AsterixRecord> RecordCollator::srvMsgQueue() const
{
    return m_srvMsgQueue;
}
