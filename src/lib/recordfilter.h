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
#include "counters.h"
#include <QQueue>
#include <optional>

class RecordFilter
{
public:
    RecordFilter();

    void addData(const Asterix::Record& rec);
    Asterix::Record record();
    bool hasPendingRecords() const;

    void loadExcludedAddresses(QIODevice* device);
    QSet<ModeS> excludedAddresses() const;

    QQueue<Asterix::Record> records() const;

    QHash<RecordType, Counters::InOutCounter> counters() const;
    std::optional<Counters::InOutCounter> counter(RecordType rt) const;

private:
    bool isRecordToBeKept(RecordType rt, const Asterix::Record& rec);

    QQueue<Asterix::Record> records_;
    QSet<ModeS> excluded_addresses_;
    QHash<RecordType, Counters::InOutCounter> counters_;
};

#endif  // ASTMOPS_RECORDFILTER_H
