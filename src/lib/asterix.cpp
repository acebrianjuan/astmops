/*!
 * \file asterix.cpp
 * \brief Implementation of ASTERIX protocol abstractions.
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

#include "asterix.h"

Asterix::DataElement::DataElement(const QString &name, const QString &value)
    : name_(name), value_(value)
{
}

Asterix::DataItem::DataItem(const QString &name, const QVector<DataElement> &data)
    : name_(name)
{
    for (const Asterix::DataElement &de : data)
    {
        data_.insert(de.name_, de);
    }
}

Asterix::DataElement Asterix::DataItem::element(QLatin1String elStr) const
{
    return data_.value(elStr);
}

Asterix::Record::Record(const quint8 cat, const QDateTime &dateTime,
    const QVector<Asterix::DataItem> &dataItems)
    : cat_(cat), datetime_(dateTime)
{
    for (const Asterix::DataItem &di : dataItems)
    {
        dataItems_.insert(di.name_, di);
    }
}

Asterix::DataItem Asterix::Record::dataItem(QLatin1String diName) const
{
    return dataItems_.value(diName);
}

QString Asterix::getElementValue(const Asterix::Record &rec, QLatin1String diName, QLatin1String deName)
{
    Asterix::DataItem di = rec.dataItem(diName);
    if (di.isNull())
    {
        return QString();
    }

    Asterix::DataElement el = di.element(deName);
    if (el.isNull())
    {
        return QString();
    }

    return el.value_;
}
