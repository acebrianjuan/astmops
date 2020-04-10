#ifndef ASTMOPS_ASTERIX_H
#define ASTMOPS_ASTERIX_H

#include <QDateTime>
#include <QString>
#include <QVariantList>
#include <QVector>

struct AsterixDataElement
{
    QString name;
    QString value;
};

struct AsterixDataItem
{
    QString name;
    QVariantList fields;
};

struct AsterixRecord
{
    QDateTime date;
    quint8 cat;
    QVector<AsterixDataItem> dataItems;
};

Q_DECLARE_METATYPE(AsterixRecord);
Q_DECLARE_METATYPE(AsterixDataItem);
Q_DECLARE_METATYPE(AsterixDataElement);

#endif  // ASTMOPS_ASTERIX_H
