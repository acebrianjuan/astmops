/*!
 * \file asterixxmlreader.cpp
 * \brief Implementation of the AsterixXmlReader class.
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

#include "asterixxmlreader.h"
#include <QRegularExpression>

AsterixXmlReader::AsterixXmlReader(QObject* parent) : QObject(parent)
{
}

void AsterixXmlReader::addData(const QByteArray& data)
{
    const QByteArrayList lines = data.split('\n');

    for (int i = 0; i < lines.size(); ++i)
    {
        QByteArray line = lines.at(i);

        if (line.isEmpty())
        {
            continue;
        }

        xml_.addData(line);

        // Check that last line ends with a newline character.
        // If it doesn't, it means that data is missing.
        if (i == lines.size() - 1 && !data.endsWith('\n'))
        {
            // Premature end of document!
            break;
        }

        while (!xml_.atEnd())
        {
            if (xml_.readNextStartElement())
            {
                // If the element start that we are in right now is not the one we want,
                // skip it entirely. Otherwise, "drill down" till the end.
                if (xml_.name() != QLatin1String("ASTERIX"))
                {
                    xml_.skipCurrentElement();
                    continue;
                }

                readRecord();
            }
        }

        xml_.clear();
    }
}

void AsterixXmlReader::setStartDate(QDate date)
{
    if (date.isValid())
    {
        startDate_ = date;
    }
}

bool AsterixXmlReader::hasPendingData() const
{
    return !records_.isEmpty();
}

std::optional<Asterix::Record> AsterixXmlReader::takeData()
{
    if (!records_.isEmpty())
    {
        return records_.dequeue();
    }

    return std::nullopt;
}

void AsterixXmlReader::readRecord()
{
    Q_ASSERT(xml_.isStartElement() && xml_.name() == QLatin1String("ASTERIX"));

    auto hasMinimumAttributes = [this]() {
        if (xml_.attributes().hasAttribute(QLatin1String("cat")) &&
            xml_.attributes().hasAttribute(QLatin1String("crc")) &&
            xml_.attributes().hasAttribute(QLatin1String("timestamp")))
        {
            return true;
        }
        return false;
    };

    if (!hasMinimumAttributes())
    {
        // qWarning();
        return;
    }

    Asterix::Record record;
    bool catOk, tstampOk;
    quint8 cat = xml_.attributes().value(QLatin1String("cat")).toUInt(&catOk);
    quint64 tstamp = xml_.attributes().value(QLatin1String("timestamp")).toUInt(&tstampOk);
    quint32 crc = xml_.attributes().value(QLatin1String("crc")).toUInt(nullptr, 16);
    Q_UNUSED(crc);

    if (!catOk || !tstampOk)
    {
        /* Invalid ASTERIX category and/or date and time information is not
         * allowed. Do not continue parsing this record.
         */
        // qWarning();
        return;
    }

    // Skip unsupported categories.
    if (!Asterix::isCategorySupported(cat))
    {
        // qWarning();
        return;
    }

    record.cat_ = cat;

    // Read Data Items.
    while (xml_.readNextStartElement())
    {
        QString diName = xml_.name().toString();
        if (isValidDataItem(diName))
        {
            Asterix::DataItem di = readDataItem();
            if (!di.isNull())
            {
                record.dataItems_.insert(diName, di);
            }
        }
        else
        {
            xml_.skipCurrentElement();
        }
    }
    if (xml_.hasError())
    {
        // Discard corrupt record.
        // qWarning();
        return;
    }

    // Determine record type.
    RecordType rt = Asterix::getRecordType(record);
    if (rt.isUnknown())
    {
        // Skip unknown record types.
        // qWarning();
        return;
    }

    record.rec_typ_ = rt;

    QDateTime datetime;
    if (useXmlTimestamp_)
    {
        datetime = QDateTime(startDate_, QTime::fromMSecsSinceStartOfDay(tstamp), Qt::UTC);
    }
    else  // Use ASTERIX TOD for timestamp.
    {
        datetime = QDateTime(startDate_, Asterix::getTimeOfDay(record), Qt::UTC);
    }

    // Add ROLLOVER days.
    if (day_count_.contains(rt) && day_count_.value(rt) > 0)
    {
        datetime = datetime.addDays(day_count_.value(rt));
    }

    // Skip invalid timestamps.
    if (!datetime.isValid())
    {
        // qWarning();
        return;
    }

    record.timestamp_ = datetime;

    auto isCloseToMidnight = [](const QDateTime& dt) {
        QTime t = QTime::fromString(QLatin1String("23:59:50"), QLatin1String("hh:mm:ss"));
        if (dt.time() >= t)
        {
            return true;
        }

        return false;
    };

    // Keep track of timestamps.
    bool save_tod = true;
    quint32 day_tdiff = 24 * 3600 - 10;
    if (last_times_.contains(rt) && last_times_.value(rt).isValid())
    {
        QDateTime lastTod = last_times_.value(rt);
        QDateTime newTod = record.timestamp_;

        Q_ASSERT(lastTod.isValid() && newTod.isValid());

        // Calculate time difference in seconds between successive records.
        double tdiff = lastTod.msecsTo(newTod) / 1000.0;

        // Check for time difference of 24h (tolerance of 10 sec).
        if (qAbs(tdiff) >= day_tdiff)
        {
            // Check for negative time difference.
            if (tdiff < 0)
            {
                // Backward TOD jump of 24h.
                // Check if last known timestamp was close to midnight.
                if (isCloseToMidnight(lastTod))
                {
                    // MIDNIGHT TOD ROLLOVER! Increase day by one.
                    ++day_count_[rt];
                    record.timestamp_ = record.timestamp_.addDays(day_count_.value(rt));
                    // qWarning();
                }
                else
                {
                    // If this happens the data is unreliable!
                    // qWarning();
                }
            }
            else  // Positive time diference.
            {
                // Forward TOD jump of 24h.
                // Check if current timestamp is close to midnight.
                if (isCloseToMidnight(newTod))
                {
                    // This is likely a delayed sample before the midnight TOD rollover.
                    // Do not apply the day increment to this record.
                    record.timestamp_ = record.timestamp_.addDays(-1);
                    save_tod = false;
                }
            }
        }
        else if (qAbs(tdiff) <= 10)  // Small time difference (less than 10 sec).
        {
            // Check for TOD backjump: small negative time difference,
            // usually less than a second.
            if (tdiff < 0)
            {
                // This is OK. Only issues a warning.
                // qWarning();
            }
        }
        else
        {
            // If this happens the data is unreliable!
            // qWarning();
        }
    }
    else
    {
        // First timestamp insertion.
        last_times_.insert(rt, record.timestamp_);
        day_count_.insert(rt, 0);
        // qWarning();
    }

    // Save most recent TOD.
    if (save_tod)
    {
        last_times_.insert(rt, record.timestamp_);
    }

    records_.enqueue(record);
    emit readyRead();
}

Asterix::DataItem AsterixXmlReader::readDataItem()
{
    Q_ASSERT(xml_.isStartElement() && isValidDataItem(xml_.name().toString()));

    Asterix::DataItem di;
    di.name_ = xml_.name().toString();

    Asterix::DataElement de;
    while (xml_.readNextStartElement())
    {
        de = readDataElement();

        // Ignore Field Extension Indicator (FX) and spare bit DataElements.
        if (!de.isNull() &&
            de.name_ != QLatin1String("FX") &&
            de.name_ != QLatin1String("spare"))
        {
            di.data_.insert(de.name_, de);
        }
    }

    return di;
}

Asterix::DataElement AsterixXmlReader::readDataElement()
{
    Q_ASSERT(xml_.isStartElement());

    Asterix::DataElement de;
    de.name_ = xml_.name().toString();
    de.value_ = xml_.readElementText();

    return de;
}

bool AsterixXmlReader::isValidDataItem(const QString& di)
{
    QRegularExpression re(QLatin1String("I(\\d{3}|RE|SP)\\b"));
    QRegularExpressionMatch match = re.match(di);
    return match.hasMatch();
}
