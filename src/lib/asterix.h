#ifndef ASTMOPS_ASTERIX_H
#define ASTMOPS_ASTERIX_H

#include "astmops.h"
#include <QDateTime>
#include <QString>
#include <QVariantList>

struct AsterixDataElement
{
    AsterixDataElement() = default;
    AsterixDataElement(const QString name, const QString value);

    QString m_name;
    QString m_value;
};

struct AsterixDataItem
{
    AsterixDataItem() = default;
    AsterixDataItem(const QString name, const QVariantList fields = QVariantList());

    QString m_name;
    QVariantList m_fields;
};

struct AsterixRecord
{
    AsterixRecord() = default;
    AsterixRecord(const quint8 cat, const QDateTime dateTime, const SystemType sysType,
        const QHash<QString, AsterixDataItem> dataItems = QHash<QString, AsterixDataItem>());

    quint8 m_cat = 0;
    QDateTime m_dateTime;
    SystemType m_sysType = Unknown;
    QHash<QString, AsterixDataItem> m_dataItems;
};

Q_DECLARE_METATYPE(AsterixRecord);
Q_DECLARE_METATYPE(AsterixDataItem);
Q_DECLARE_METATYPE(AsterixDataElement);

// TODO: Implement streaming operator for debugging.
//QDebug operator<<(QDebug dbg, const AsterixRecord &record);

#endif  // ASTMOPS_ASTERIX_H
