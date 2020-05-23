#ifndef ASTMOPS_ASTERIX_H
#define ASTMOPS_ASTERIX_H

#include "astmops.h"
#include <QDateTime>
#include <QString>
#include <QVariantList>

struct AsterixDataElement
{
    AsterixDataElement();
    AsterixDataElement(const QString name, const QString value);

    QString m_name;
    QString m_value;
};

struct AsterixDataItem
{
    AsterixDataItem();
    AsterixDataItem(const QString name, const QVariantList fields = QVariantList());

    QString m_name;
    QVariantList m_fields;
};

struct AsterixRecord
{
    AsterixRecord();
    AsterixRecord(const quint8 cat, const QDateTime dateTime, const SystemType sysType,
        const QHash<QString, AsterixDataItem> dataItems = QHash<QString, AsterixDataItem>());

    quint8 m_cat;
    QDateTime m_dateTime;
    SystemType m_sysType;
    QHash<QString, AsterixDataItem> m_dataItems;
};

Q_DECLARE_METATYPE(AsterixRecord);
Q_DECLARE_METATYPE(AsterixDataItem);
Q_DECLARE_METATYPE(AsterixDataElement);

#endif  // ASTMOPS_ASTERIX_H
