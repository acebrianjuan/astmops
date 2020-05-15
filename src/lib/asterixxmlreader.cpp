#include "asterixxmlreader.h"
#include <QRegularExpression>

AsterixXmlReader::AsterixXmlReader(QObject* parent) : QObject(parent)
{
}

void AsterixXmlReader::addData(const QByteArray& data)
{
    for (auto line : data.split('\n'))
    {
        if (line.isEmpty())
        {
            continue;
        }

        m_xml.addData(line);

        while (!m_xml.atEnd())
        {
            if (m_xml.readNextStartElement())
            {
                // If the element start that we are in right now is not the one we want,
                // skip it entirely. Otherwise, "drill down" till the end.
                if (m_xml.name() != QLatin1String("ASTERIX"))
                {
                    m_xml.skipCurrentElement();
                    continue;
                }

                if (m_xml.attributes().hasAttribute(QLatin1String("cat")))
                {
                    readRecord();
                    emit readyRead();
                }
                else
                {
                    m_xml.raiseError(QLatin1String("Invalid ASTERIX file."));
                }
            }
        }

        m_xml.clear();
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
             m_xml.attributes().hasAttribute(QLatin1String("cat")));

    AsterixRecord record;
    record.cat = m_xml.attributes().value(QLatin1String("cat")).toUInt();

    while (m_xml.readNextStartElement())
    {
        QString diName = m_xml.name().toString();
        if (isValidDataItem(diName))
        {
            //qDebug() << diName;
            record.dataItems[diName] = readDataItem();
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
    QRegularExpression re(QLatin1String("I(\\d{3}|RE|SP)\\b"));
    QRegularExpressionMatch match = re.match(di);
    return match.hasMatch();
}
