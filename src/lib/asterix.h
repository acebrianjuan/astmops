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
#include <QVariantList>

/*!
 * \brief The AsterixDataElement struct is an abstraction of the unit of
 * information being transmitted.
 *
 * An AsterixDataElement object contains the name and value of a unit of
 * information.
 */
struct AsterixDataElement
{
    AsterixDataElement() = default;
    AsterixDataElement(const QString &name, const QString &value);

    inline bool isNull()
    {
        return m_name.isEmpty() || m_value.isEmpty();
    }

    QString m_name;
    QString m_value;
};

/*!
 * \brief The AsterixDataItem struct is an abstraction of the Data Item
 * concept in the ASTERIX protocol.
 *
 * An AsterixDataItem object contains a collection of AsterixDataElement objects.
 */
struct AsterixDataItem
{
    AsterixDataItem() = default;
    AsterixDataItem(const QString &name, const QVariantList &fields = QVariantList());

    inline bool isNull()
    {
        return m_name.isEmpty() || m_fields.isEmpty();
    }

    AsterixDataElement element(QLatin1String elStr) const;
    QString valStr(QLatin1String elStr) const;

    QString m_name;
    QVariantList m_fields;
};

/*!
 * \brief The AsterixRecord struct is an abstraction of the Record concept
 * in the ASTERIX protocol.
 *
 * An AsterixRecord object contains a collection of AsterixDataItem objects
 * of the same category.
 */
struct AsterixRecord
{
    AsterixRecord() = default;
    AsterixRecord(const quint8 cat, const QDateTime &dateTime, const SystemType sysType,
        const QHash<QString, AsterixDataItem> &dataItems = QHash<QString, AsterixDataItem>());

    AsterixDataItem dataItem(QLatin1String diStr) const;
    QString valStrFromDitem(QLatin1String diStr, QLatin1String elStr) const;

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
