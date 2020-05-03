#include "aixmreader.h"
#include <QDebug>

AixmReader::AixmReader(QObject *parent) : QObject(parent)
{
}

bool AixmReader::read(QIODevice *device)
{
    m_xml.setDevice(device);

    if (m_xml.readNextStartElement())
    {
        if (m_xml.name() == QLatin1String("AIXMBasicMessage"))
        {
            readAixm();
        }
        else
        {
            m_xml.raiseError(QObject::tr("The file is not an AIXM file."));
        }
    }

    return !m_xml.error();
}

void AixmReader::readAixm()
{
    Q_ASSERT(m_xml.isStartElement() &&
             m_xml.name() == QLatin1String("AIXMBasicMessage"));

    while (!m_xml.atEnd())
    {
        if (m_xml.readNextStartElement())
        {
            // If the element start that we are it right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (m_xml.name() != QLatin1String("hasMember"))
            {
                m_xml.skipCurrentElement();
                continue;
            }

            if (m_xml.readNextStartElement())
            {
                if (m_xml.name() == QLatin1String("AirportHeliport"))
                {
                    m_aerodrome.setArp(posListToPoint(getPosList(arpPosXmlPath())));
                }
                else if (m_xml.name() == QLatin1String("RunwayElement"))
                {
                    m_aerodrome.addRunwayElement(posListToPolygon(getPosList(runwayPosListXmlPath())));
                }
                else if (m_xml.name() == QLatin1String("TaxiwayElement"))
                {
                    m_aerodrome.addTaxiwayElement(posListToPolygon(getPosList(taxiwayPosListXmlPath())));
                }
                else if (m_xml.name() == QLatin1String("ApronElement"))
                {
                    m_aerodrome.addApronElement(posListToPolygon(getPosList(apronPosListXmlPath())));
                }
                else if (m_xml.name() == QLatin1String("StandElement"))
                {
                    //m_aerodrome.addStandElement(getPosList(standPosListXmlPath()));
                }
                else
                {
                    m_xml.skipCurrentElement();
                }
            }
        }
    }
}

QStringList AixmReader::getPosList(const QStringList &tokens)
{
    int level = 0;
    while (!m_xml.atEnd())
    {
        if (m_xml.readNextStartElement())
        {
            // If the element start that we are it right now is not the one we want,
            // skip it entirely. Otherwise, "drill down" till the end.
            if (m_xml.name() != tokens[level])
            {
                m_xml.skipCurrentElement();
                continue;
            }
            ++level;

            // We are at the wanted DocFileRef element.
            if (level >= tokens.size())
            {
                QStringList list = m_xml.readElementText().split(QLatin1String(" "));
                return list;
            }
        }
        if (m_xml.hasError())
        {
            break;
        }
    }

    // Something went wrong. Return empty list.
    return QStringList();
}

QPointF AixmReader::posListToPoint(QStringList list)
{
    Q_ASSERT(list.size() == 2);

    double lon = list.takeFirst().toDouble();
    double lat = list.takeFirst().toDouble();
    QPointF point(lon, lat);

    return point;
}

QPolygonF AixmReader::posListToPolygon(QStringList list)
{
    int sizeIn = list.size();
    Q_ASSERT(sizeIn % 2 == 0);

    QPolygonF polygon;
    double lon, lat;

    while (list.size() > 0)
    {
        lon = list.takeFirst().toDouble();
        lat = list.takeFirst().toDouble();
        polygon << QPointF(lon, lat);
    }

    int sizeOut = polygon.size();
    Q_ASSERT(sizeOut == sizeIn / 2);

    return polygon;
}

QStringList AixmReader::arpPosXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("AirportHeliportTimeSlice")
                                                    << QLatin1String("ARP")
                                                    << QLatin1String("ElevatedPoint")
                                                    << QLatin1String("pos");
    return tokens;
}

QStringList AixmReader::runwayPosListXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("RunwayElementTimeSlice")
                                                    << QLatin1String("extent")
                                                    << QLatin1String("ElevatedSurface")
                                                    << QLatin1String("patches")
                                                    << QLatin1String("PolygonPatch")
                                                    << QLatin1String("exterior")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("posList");
    return tokens;
}

QStringList AixmReader::taxiwayPosListXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("TaxiwayElementTimeSlice")
                                                    << QLatin1String("extent")
                                                    << QLatin1String("ElevatedSurface")
                                                    << QLatin1String("patches")
                                                    << QLatin1String("PolygonPatch")
                                                    << QLatin1String("exterior")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("posList");
    return tokens;
}

QStringList AixmReader::apronPosListXmlPath()
{
    static const QStringList tokens = QStringList() << QLatin1String("timeSlice")
                                                    << QLatin1String("ApronElementTimeSlice")
                                                    << QLatin1String("extent")
                                                    << QLatin1String("ElevatedSurface")
                                                    << QLatin1String("patches")
                                                    << QLatin1String("PolygonPatch")
                                                    << QLatin1String("exterior")
                                                    << QLatin1String("LinearRing")
                                                    << QLatin1String("posList");
    return tokens;
}
