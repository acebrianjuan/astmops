/*!
 * \file asterix.h
 * \brief Interface of ASTERIX protocol abstractions.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * Contains a set of abstractions for handling ASTERIX protocol data.
 *
 * -----------------------------------------------------------------------
 *
 * Copyright (C) 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
 *
 * ASTMOPS is a surveillance performance analyzer
 *      for Air Traffic Control
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
    DataElement() = default;
    DataElement(const QString &name, const QString &value);

    bool operator==(const DataElement &other) const;

    inline bool isNull()
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
    DataItem() = default;
    DataItem(const QString &name,
        const QVector<DataElement> &data = QVector<DataElement>());

    bool operator==(const DataItem &other) const;

    inline bool isNull()
    {
        return name_.isEmpty() || data_.isEmpty();
    }

    DataElement element(QLatin1String elStr) const;

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
    Record() = default;
    Record(const quint8 cat, const QDateTime &dateTime,
        const QVector<DataItem> &dataItems = QVector<DataItem>());

    bool operator==(const Record &other) const;

    DataItem dataItem(QLatin1String diName) const;

    quint8 cat_ = 0;
    quint16 len_ = 0;
    quint32 crc_ = 0xFFFFFFFF;
    QDateTime datetime_;
    QHash<QString, DataItem> dataItems_;
};

bool containsDataItem(const Record &rec, QLatin1String diName);
bool containsElement(const Record &rec, QLatin1String diName, QLatin1String deName);
QString getElementValue(const Record &rec, QLatin1String diName, QLatin1String deName);
RecordType getRecordType(const Asterix::Record &rec);

};  // namespace Asterix

/*
bool operator==(const Asterix::DataElement &lhs, const Asterix::DataElement &rhs);
bool operator==(const Asterix::DataItem &lhs, const Asterix::DataItem &rhs);
bool operator==(const Asterix::Record &lhs, const Asterix::Record &rhs);
*/

// TODO: Implement streaming operator for debugging.
//QDebug operator<<(QDebug dbg, const Asterix::Record &record);

Q_DECLARE_METATYPE(Asterix::Record);
Q_DECLARE_METATYPE(Asterix::DataItem);
Q_DECLARE_METATYPE(Asterix::DataElement);


#endif  // ASTMOPS_ASTERIX_H
