#include "recordcollator.h"
#include <QDebug>
#include <QSettings>
#include <algorithm>

RecordCollator::RecordCollator(QObject *parent) : QObject(parent)
{
    readSettings();
}

void RecordCollator::processRecord(const AsterixRecord &record)
{
    // Classify record by System Type (smr, mlat, adsb).
    // Filter out if record is an excluded address.
    // Sort records by Time of Day.

    // TODO: Classify Records based on System Identification Code (SIC).
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

            if (sysType == 1)  // Mode S Multilateration.
            {
                ++m_mlatCounter.in;
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
                ++m_smrCounter.in;
            }

            bool ok;
            AsterixDataItem di010_220 = record.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt(&ok, 16);

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
                m_mlatQueue.enqueue(record);
                std::sort(m_mlatQueue.begin(), m_mlatQueue.end(), sorter);
                ++m_mlatCounter.out;
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
                m_smrQueue.enqueue(record);
                std::sort(m_smrQueue.begin(), m_smrQueue.end(), sorter);
                ++m_smrCounter.out;
            }
        }
        else if (msgType == 3)  // Periodic Status Message.
        {
            ++m_srvMsgCounter.in;

            // TODO: Use the SIC to determine which system is the service message informing about.

            /*
            if (sysType == 1)  // Mode S Multilateration.
            {
            }
            else if (sysType == 3)  // Primary Surveillance Radar.
            {
            }
            */

            // TODO: We need 2 queues, one for SMR and one for MLAT.

            // Service Messages are always enqueued.
            m_srvMsgQueue.enqueue(record);
            std::sort(m_srvMsgQueue.begin(), m_srvMsgQueue.end(), sorter);
            ++m_srvMsgCounter.out;
        }
    }
    else if (record.m_cat == 21)  // ADS-B Reports.
    {
        ++m_adsbCounter.in;
        bool ok;
        AsterixDataItem di021_080 = record.m_dataItems[QLatin1String("I080")];
        IcaoAddr tgtAddr = di021_080.m_fields[0].value<AsterixDataElement>().m_value.toUInt(&ok, 16);

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
        m_adsbQueue.enqueue(record);
        std::sort(m_adsbQueue.begin(), m_adsbQueue.end(), sorter);
        ++m_adsbCounter.out;
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

void RecordCollator::readSettings()
{
    QSettings settings;

    auto initIfSet = [&settings](QString key, quint8 &var) {
        if (settings.contains(key))
        {
            var = settings.value(key).toUInt();
        }
        else
        {
            qCritical() << key << "is not set in the configuration file!";
        }
    };

    initIfSet(m_smrSicKey, m_smrSic);
    initIfSet(m_mlatSicKey, m_mlatSic);
    initIfSet(m_adsbSicKey, m_adsbSic);
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

RecordCollator::Counter RecordCollator::smrCounter() const
{
    return m_smrCounter;
}

RecordCollator::Counter RecordCollator::mlatCounter() const
{
    return m_mlatCounter;
}

RecordCollator::Counter RecordCollator::adsbCounter() const
{
    return m_adsbCounter;
}

RecordCollator::Counter RecordCollator::srvMsgCounter() const
{
    return m_srvMsgCounter;
}
