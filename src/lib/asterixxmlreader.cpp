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
    for (const auto& line : lines)
    {
        if (line.isEmpty())
        {
            continue;
        }

        xml_.addData(line);

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

                if (xml_.attributes().hasAttribute(QLatin1String("cat")))
                {
                    readRecord();
                    emit readyRead();
                }
                else
                {
                    xml_.raiseError(QLatin1String("Invalid ASTERIX file."));
                }
            }
        }

        xml_.clear();
    }
}

Asterix::Record AsterixXmlReader::record()
{
    return records_.dequeue();
}

bool AsterixXmlReader::hasPendingRecords()
{
    return !records_.isEmpty();
}

void AsterixXmlReader::setStartDate(QDate date)
{
    if (date.isValid())
    {
        startDate_ = date;
    }
}

void AsterixXmlReader::readRecord()
{
    Q_ASSERT(xml_.isStartElement() &&
             xml_.name() == QLatin1String("ASTERIX") &&
             xml_.attributes().hasAttribute(QLatin1String("cat")) &&
             xml_.attributes().hasAttribute(QLatin1String("crc")) &&
             xml_.attributes().hasAttribute(QLatin1String("timestamp")));

    Asterix::Record record;
    bool catOk, timeStampOk;
    quint8 cat = xml_.attributes().value(QLatin1String("cat")).toUInt(&catOk);
    quint64 timeStamp = xml_.attributes().value(QLatin1String("timestamp")).toUInt(&timeStampOk);

    if (!catOk || !timeStampOk)
    {
        /* Missing ASTERIX Category and/or date and time information is not
         * allowed. Do not continue parsing this record.
         */
        return;
    }

    record.cat_ = cat;
    record.timestamp_ = QDateTime(startDate_, QTime::fromMSecsSinceStartOfDay(timeStamp), Qt::UTC);

    while (xml_.readNextStartElement())
    {
        QString diName = xml_.name().toString();
        if (isValidDataItem(diName))
        {
            //qDebug() << diName;
            record.dataItems_[diName] = readDataItem();
        }
        else
        {
            xml_.skipCurrentElement();
        }
    }

    records_.enqueue(record);
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
        di.data_.insert(de.name_, de);
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
