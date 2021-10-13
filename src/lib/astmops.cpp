/*!
 * \file astmops.cpp
 * \brief Implementation of essential definitions to be used in the scope
 * of the ASTMOPS program.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
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

#include "astmops.h"

RecordType::RecordType() {}

RecordType::RecordType(SystemType st, MessageType mt)
    : sys_typ_(st), msg_typ_(mt){};

bool RecordType::isUnknown() const
{
    return sys_typ_ == SystemType::Unknown || msg_typ_ == MessageType::Unknown;
}

bool operator==(RecordType lhs, RecordType rhs)
{
    return lhs.sys_typ_ == rhs.sys_typ_ &&
           lhs.msg_typ_ == rhs.msg_typ_;
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(TargetType tt, uint seed)
#else
size_t qHash(TargetType tt, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, static_cast<int>(tt));
    return seed;
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(SystemType st, uint seed)
#else
size_t qHash(SystemType st, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, static_cast<int>(st));
    return seed;
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(MessageType mt, uint seed)
#else
size_t qHash(MessageType mt, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, static_cast<int>(mt));
    return seed;
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(RecordType rt, uint seed)
#else
size_t qHash(RecordType rt, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, static_cast<int>(rt.sys_typ_));
    seed = hash(seed, static_cast<int>(rt.msg_typ_));
    return seed;
}

bool operator==(DataSrcId lhs, DataSrcId rhs)
{
    return lhs.sac_ == rhs.sac_ &&
           lhs.sic_ == rhs.sic_;
}

namespace Literals
{
QDateTime operator""_ts(const char *text, size_t size)
{
    return QDateTime::fromString(QLatin1String(text, size), Qt::ISODateWithMs);
}
}  // namespace Literals
