/*!
 * \file asterix.h
 * \brief Interface of ASTERIX protocol abstractions.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * Contains a set of abstractions for handling ASTERIX protocol data.
 *
 * -----------------------------------------------------------------------
 *
 * Copyright (C) 2020-2021 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
 *
 * ASTMOPS is a command line tool for evaluating 
 * the performance of A-SMGCS sensors at airports
 *
 * This file is part of ASTMOPS.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------
 */

#ifndef ASTMOPS_ASTERIX_H
#define ASTMOPS_ASTERIX_H

#include "astmops.h"
#include <QDateTime>
#include <QString>
#include <optional>

namespace Asterix
{
/*!
 * \brief The AsterixDataElement struct is an abstraction of the unit of
 * information being transmitted.
 *
 * An AsterixDataElement object contains the name and value of a unit of
 * information.
 */
struct DataElement
{
public:
    DataElement() = default;
    DataElement(const QString &name, const QString &value);

    inline bool isNull() const
    {
        return name_.isEmpty() || value_.isEmpty();
    }

    QString name_;
    QString value_;
};

/*!
 * \brief The AsterixDataItem struct is an abstraction of the Data Item
 * concept in the ASTERIX protocol.
 *
 * An AsterixDataItem object contains a collection of AsterixDataElement objects.
 */
struct DataItem
{
public:
    DataItem() = default;
    DataItem(const QString &name,
        const QVector<DataElement> &data = QVector<DataElement>());

    inline bool isNull() const
    {
        return name_.isEmpty() || data_.isEmpty();
    }

    std::optional<DataElement> element(QLatin1String deName) const;

    QString name_;
    QHash<QString, DataElement> data_;
};

/*!
 * \brief The AsterixRecord struct is an abstraction of the Record concept
 * in the ASTERIX protocol.
 *
 * An AsterixRecord object contains a collection of AsterixDataItem objects
 * of the same category.
 */
struct Record
{
public:
    Record() = default;
    Record(const quint8 cat, const QDateTime &dateTime,
        const QVector<DataItem> &dataItems = QVector<DataItem>());

    std::optional<DataItem> dataItem(QLatin1String diName) const;

    quint8 cat_ = 0;
    quint16 len_ = 0;
    quint32 crc_ = 0xFFFFFFFF;
    QDateTime timestamp_;

    DataSrcId ds_id_;
    RecordType rec_typ_;

    QHash<QString, DataItem> dataItems_;
};

bool containsDataItem(const Asterix::Record &rec, QLatin1String diName);
bool containsDataItem(const Asterix::Record &rec, const QVector<QLatin1String> &diNames);
bool containsElement(const Asterix::Record &rec, QLatin1String diName, QLatin1String deName);
std::optional<QString> getElementValue(const Asterix::Record &rec, QLatin1String diName, QLatin1String deName);
RecordType getRecordType(const Asterix::Record &rec);
QTime getTimeOfDay(const Asterix::Record &rec);

bool isCategorySupported(const Cat cat);

enum DataItemListType
{
    Mandatory,
    Disjunctive
};

struct DataItemList
{
    QStringList items_;
    DataItemListType type_;
};

bool hasMinimumDataItems(const Asterix::Record &rec);
bool checkDataItems(const Asterix::Record &rec, const QVector<DataItemList> &col);
bool checkDataItemsList(const Asterix::Record &rec, const QStringList &list, DataItemListType type);

// FREE OPERATORS.
bool operator==(const Asterix::DataElement &lhs, const Asterix::DataElement &rhs);
bool operator==(const Asterix::DataItem &lhs, const Asterix::DataItem &rhs);
bool operator==(const Asterix::Record &lhs, const Asterix::Record &rhs);

};  // namespace Asterix

// TODO: Implement streaming operator for debugging.
//QDebug operator<<(QDebug dbg, const Asterix::Record &record);

Q_DECLARE_METATYPE(Asterix::Record);
Q_DECLARE_METATYPE(QVector<Asterix::Record>);

Q_DECLARE_METATYPE(Asterix::DataItem);
Q_DECLARE_METATYPE(QVector<Asterix::DataItem>);

Q_DECLARE_METATYPE(Asterix::DataElement);
Q_DECLARE_METATYPE(QVector<Asterix::DataElement>);


#endif  // ASTMOPS_ASTERIX_H
