#ifndef ASTMOPS_ASTERIX_H
#define ASTMOPS_ASTERIX_H

#include "astmops.h"
#include <QDateTime>
#include <QString>
#include <QVariantList>
#include <QVector>
#include <tuple>

// TODO: Consider giving default values to the struct members.

struct AsterixDataElement
{
    QString m_name;
    QString m_value;
};

struct AsterixDataItem
{
    QString m_name;
    QVariantList m_fields;
};

struct AsterixRecord
{
    quint8 m_cat;
    QDateTime m_dateTime;
    SystemType m_sysType;
    QHash<QString, AsterixDataItem> m_dataItems;
};

Q_DECLARE_METATYPE(AsterixRecord);
Q_DECLARE_METATYPE(AsterixDataItem);
Q_DECLARE_METATYPE(AsterixDataElement);

#endif  // ASTMOPS_ASTERIX_H
