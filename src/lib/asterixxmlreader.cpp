#include "asterixxmlreader.h"
#include <QRegularExpression>

AsterixXmlReader::AsterixXmlReader(QObject* parent) : QObject(parent)
{
}

void AsterixXmlReader::addData(const QByteArray& data)
{
    m_xml.addData(data);

    if (m_xml.readNextStartElement())
    {
        if (m_xml.name() == QLatin1String("ASTERIX") &&
            m_xml.attributes().hasAttribute("cat"))
        {
            // TODO: Handle case when excluded target is read.
            readRecord();
            emit readyRead();
        }
        else
        {
            m_xml.raiseError("Invalid ASTERIX file.");
        }
    }
}

AsterixRecord AsterixXmlReader::record()
{
    return m_recordsQueue.dequeue();
}

bool AsterixXmlReader::hasPendingRecords()
{
    return !m_recordsQueue.isEmpty();
}

void AsterixXmlReader::readRecord()
{
    Q_ASSERT(m_xml.isStartElement() &&
             m_xml.name() == QLatin1String("ASTERIX") &&
             m_xml.attributes().hasAttribute("cat"));

    AsterixRecord record;
    record.cat = m_xml.attributes().value("cat").toUInt();

    while (m_xml.readNextStartElement())
    {
        if (isValidDataItem(m_xml.name().toString()))
        {
            //qDebug() << m_xml.name().toString();
            record.dataItems.append(QVariant::fromValue(readDataItem()));
        }
        else
        {
            m_xml.skipCurrentElement();
        }
    }

    m_recordsQueue.enqueue(record);
}

AsterixDataItem AsterixXmlReader::readDataItem()
{
    Q_ASSERT(m_xml.isStartElement() && isValidDataItem(m_xml.name().toString()));

    AsterixDataItem di;
    di.name = m_xml.name().toString();

    while (m_xml.readNextStartElement())
    {
        di.fields.append(QVariant::fromValue(readDataField()));
    }

    return di;
}

AsterixDataElement AsterixXmlReader::readDataField()
{
    Q_ASSERT(m_xml.isStartElement());

    AsterixDataElement df;
    df.name = m_xml.name().toString();
    df.value = m_xml.readElementText();

    return df;
}

bool AsterixXmlReader::isValidDataItem(const QString& di)
{
    QRegularExpression re("I(\\d{3}|RE|SP)\\b");
    QRegularExpressionMatch match = re.match(di);
    return match.hasMatch();
}
