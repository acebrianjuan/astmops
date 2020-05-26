#include "asterix.h"

AsterixDataElement::AsterixDataElement(const QString name, const QString value)
    : m_name(name), m_value(value)
{
}

AsterixDataItem::AsterixDataItem(const QString name, const QVariantList fields)
    : m_name(name), m_fields(fields)
{
}

AsterixRecord::AsterixRecord(const quint8 cat, const QDateTime dateTime, const SystemType sysType,
    const QHash<QString, AsterixDataItem> dataItems)
    : m_cat(cat), m_dateTime(dateTime), m_sysType(sysType), m_dataItems(dataItems)
{
}
