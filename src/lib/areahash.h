/*!
 * \file areahash.h
 * \brief
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
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

#ifndef ASTMOPS_AREAHASH_H
#define ASTMOPS_AREAHASH_H

#include "aerodrome.h"
#include <QHash>


#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(const Aerodrome::NamedArea& narea, uint seed)
#else
size_t qHash(const Aerodrome::NamedArea& narea, size_t seed)
#endif
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, narea.name_);
    seed = hash(seed, narea.area_);
    return seed;
}

bool operator==(const Aerodrome::NamedArea& a, const Aerodrome::NamedArea& b)
{
    return a.area_ == b.area_ &&
           a.name_ == b.name_;
}

template <typename T>
struct AreaHash : public QHash<Aerodrome::NamedArea, T>
{
    QVector<typename AreaHash::iterator> findByArea(Aerodrome::Area wantedArea)
    {
        QVector<typename AreaHash::iterator> result;

        for (auto it = this->begin(), last = this->end(); it != last; ++it)
        {
            const Aerodrome::Area itArea = it.key().area_;

            if ((itArea | wantedArea) == wantedArea)
            {
                result.append(it);
            }
        }

        return result;
    }
};

#endif  // ASTMOPS_AREAHASH_H
