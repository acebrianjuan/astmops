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

AsterixDataElement::AsterixDataElement(const QString &name, const QString &value)
    : m_name(name), m_value(value)
{
}

AsterixDataItem::AsterixDataItem(const QString &name, const QVariantList &fields)
    : m_name(name), m_fields(fields)
{
}

AsterixRecord::AsterixRecord(const quint8 cat, const QDateTime &dateTime, const SystemType sysType,
    const QHash<QString, AsterixDataItem> &dataItems)
    : m_cat(cat), m_dateTime(dateTime), m_sysType(sysType), m_dataItems(dataItems)
{
}
