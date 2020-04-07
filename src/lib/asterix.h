#ifndef ASTMOPS_ASTERIX_H
#define ASTMOPS_ASTERIX_H

#include <QDateTime>
#include <QString>
#include <QVariantList>

struct AsterixRecord
{
    QDateTime date;
    quint8 cat;
    QVariantList dataItems;
};

struct AsterixDataItem
{
    QString name;
    QVariantList fields;
};

struct AsterixDataElement
{
    QString name;
    QString value;
};

Q_DECLARE_METATYPE(AsterixRecord);
Q_DECLARE_METATYPE(AsterixDataItem);
Q_DECLARE_METATYPE(AsterixDataElement);

#endif  // ASTMOPS_ASTERIX_H
