#include "asterix.h"

AsterixDataElement::AsterixDataElement()
    : m_name(QString()), m_value(QString())
{
}

AsterixDataElement::AsterixDataElement(const QString name, const QString value)
    : m_name(name), m_value(value)
{
}

AsterixDataItem::AsterixDataItem()
    : m_name(QString()), m_fields(QVariantList())
{
}

AsterixDataItem::AsterixDataItem(const QString name, const QVariantList fields)
    : m_name(name), m_fields(fields)
{
}

AsterixRecord::AsterixRecord()
    : m_cat(0), m_dateTime(QDateTime()), m_sysType(Unknown), m_dataItems(QHash<QString, AsterixDataItem>())
{
}

AsterixRecord::AsterixRecord(const quint8 cat, const QDateTime dateTime, const SystemType sysType,
    const QHash<QString, AsterixDataItem> dataItems)
    : m_cat(cat), m_dateTime(dateTime), m_sysType(sysType), m_dataItems(dataItems)
{
}
