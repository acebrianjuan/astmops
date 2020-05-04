#ifndef ASTMOPS_ASTERIX_H
#define ASTMOPS_ASTERIX_H

#include <QDateTime>
#include <QString>
#include <QVariantList>
#include <QVector>

// TODO: Consider giving default values to the struct members.

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
    QHash<QString, AsterixDataItem> dataItems;
};

Q_DECLARE_METATYPE(AsterixRecord);
Q_DECLARE_METATYPE(AsterixDataItem);
Q_DECLARE_METATYPE(AsterixDataElement);

#endif  // ASTMOPS_ASTERIX_H
