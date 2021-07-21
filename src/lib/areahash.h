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

template <typename T>
struct AreaHash : public QHash<Aerodrome::NamedArea, T>
{
    QVector<typename AreaHash::const_iterator> findByArea(Aerodrome::Area wantedArea) const
    {
        QVector<typename AreaHash::const_iterator> result;

        for (auto it = this->begin(), last = this->end(); it != last; ++it)
        {
            const Aerodrome::Area area = it.key().area_;

            if (area != Aerodrome::Area::None &&
                (area | wantedArea) == wantedArea)
            {
                result.append(it);
            }
        }

        return result;
    }
};

template <typename T>
bool operator==(const AreaHash<T> &lhs, const AreaHash<T> &rhs)
{
    auto lhs_h = static_cast<QHash<Aerodrome::NamedArea, T>>(lhs);
    auto rhs_h = static_cast<QHash<Aerodrome::NamedArea, T>>(rhs);

    return lhs_h == rhs_h;
}

Q_DECLARE_METATYPE(AreaHash<QVector<double>>);

#endif  // ASTMOPS_AREAHASH_H
