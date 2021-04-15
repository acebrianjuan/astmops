/*!
 * \file recordfilter.h
 * \brief Interface of the RecordFilter class.
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

#ifndef ASTMOPS_RECORDFILTER_H
#define ASTMOPS_RECORDFILTER_H

#include "asterix.h"
#include "astmops.h"
#include <QQueue>

class RecordFilter
{
public:
    struct Counter
    {
        quint32 in = 0;
        quint32 out = 0;
    };

    RecordFilter();

    void addData(const Asterix::Record& rec);
    Asterix::Record record();
    bool hasPendingRecords() const;

    void loadExcludedAddresses(QIODevice* device);
    QVector<ModeS> excludedAddresses() const;

    QQueue<Asterix::Record> records() const;

    QHash<RecordType, Counter> counters() const;
    Counter counter(RecordType rt) const;

private:
    bool isRecordToBeKept(RecordType rt, const Asterix::Record& rec);

    QQueue<Asterix::Record> records_;
    QVector<ModeS> excluded_addresses_;

    QHash<RecordType, Counter> counters_;
};

#endif  // ASTMOPS_RECORDFILTER_H
