/*!
 * \file recordfilter.cpp
 * \brief Implementation of the RecordFilter class.
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

#include "recordfilter.h"
#include <QMetaEnum>

RecordFilter::RecordFilter()
{
    // Initialize counters.
    counters_.insert(RecordType(SystemType::Smr, MessageType::TargetReport), Counter());
    counters_.insert(RecordType(SystemType::Smr, MessageType::ServiceMessage), Counter());
    counters_.insert(RecordType(SystemType::Mlat, MessageType::TargetReport), Counter());
    counters_.insert(RecordType(SystemType::Mlat, MessageType::ServiceMessage), Counter());
    counters_.insert(RecordType(SystemType::Adsb, MessageType::TargetReport), Counter());
}

void RecordFilter::addData(const Asterix::Record &rec)
{
    RecordType rt = Asterix::getRecordType(rec);

    ++counters_[rt].in;
    if (isRecordToBeKept(rt, rec))
    {
        records_.enqueue(rec);
        ++counters_[rt].out;
    }
}

Asterix::Record RecordFilter::record()
{
    return records_.dequeue();
}

bool RecordFilter::hasPendingRecords() const
{
    return !records_.isEmpty();
}

void RecordFilter::loadExcludedAddresses(QIODevice *device)
{
    const QByteArray data = device->readAll();
    const QByteArrayList lines = data.split('\n');
    for (const QByteArray &line : lines)
    {
        QByteArray text = line.trimmed();

        // Skip empty lines or comments.
        if (text.isEmpty() || text.startsWith('#'))
        {
            continue;
        }

        bool ok;
        ModeS addr = text.toUInt(&ok, 16);
        if (ok)
        {
            excluded_addresses_.append(addr);
        }
    }
}

QVector<ModeS> RecordFilter::excludedAddresses() const
{
    return excluded_addresses_;
}

bool RecordFilter::isRecordToBeKept(RecordType rt, const Asterix::Record &rec)
{
    if (rt.isUnknown())
    {
        return false;
    }

    if (rt.msg_typ_ == MessageType::ServiceMessage)
    {
        if (rt.sys_typ_ == SystemType::Smr || rt.sys_typ_ == SystemType::Mlat)
        {
            // Only keep Service Messages from SMR and MLAT.
            return true;
        }
        return false;
    }

    bool ok;
    if (rt.msg_typ_ == MessageType::TargetReport)
    {
        if (rt.sys_typ_ == SystemType::Smr)
        {
            // SMR Target Reports are always kept.
            return true;
        }
        else if (rt.sys_typ_ == SystemType::Mlat)
        {
            // For MLAT Target Reports check if the Target Address is to be excluded.
            bool has_tgt_addr = Asterix::containsElement(rec, QLatin1String("I220"), QLatin1String("TAddr"));

            if (!has_tgt_addr)
            {
                qWarning("Skipping MLAT TgtRep without target address");
                return false;
            }

            ModeS tgt_addr = Asterix::getElementValue(rec, QLatin1String("I220"), QLatin1String("TAddr")).toUInt(&ok, 16);

            if (!ok)
            {
                qWarning("Skipping MLAT TgtRep with invalid target address");
                return false;
            }

            // Do not keep record if it belongs to an excluded address.
            if (excluded_addresses_.contains(tgt_addr))
            {
                return false;
            }

            // Check the type of target (TOT).
            bool has_tot = Asterix::containsElement(rec, QLatin1String("I020"), QLatin1String("TOT"));

            if (!has_tot)
            {
                qWarning("Skipping MLAT TgtRep without TOT");
                return false;
            }

            quint8 tot = Asterix::getElementValue(rec, QLatin1String("I020"), QLatin1String("TOT")).toUInt(&ok);

            if (!ok)
            {
                qWarning("Skipping MLAT TgtRep with invalid TOT");
                return false;
            }

            /* TOT:
             * 00 Undetermined
             * 01 Aircraft
             * 10 Ground vehicle
             * 11 Helicopter
             */
            if (tot == 1 || tot == 3)
            {
                // Only keep aircraft and helicopters.
                return true;
            }
            return false;
        }
        else if (rt.sys_typ_ == SystemType::Adsb)
        {
            // Check target address.
            bool has_tgt_addr = Asterix::containsElement(rec, QLatin1String("I080"), QLatin1String("TAddr"));

            if (!has_tgt_addr)
            {
                qWarning("Skipping ADS-B TgtRep without target address");
                return false;
            }

            ModeS tgt_addr = Asterix::getElementValue(rec, QLatin1String("I080"), QLatin1String("TAddr")).toUInt(&ok, 16);

            if (!ok)
            {
                qWarning("Skipping ADS-B TgtRep with invalid target address");
                return false;
            }

            // Do not continue if Target Address is an excluded address.
            if (excluded_addresses_.contains(tgt_addr))
            {
                return false;
            }

            // Check emitter category (ECAT).
            bool has_ecat = Asterix::containsElement(rec, QLatin1String("I020"), QLatin1String("ECAT"));

            if (!has_ecat)
            {
                qWarning("Skipping ADS-B TgtRep without ECAT");
                return false;
            }

            quint8 ecat = Asterix::getElementValue(rec, QLatin1String("I020"), QLatin1String("ECAT")).toUInt(&ok);

            if (!ok)
            {
                qWarning("Skipping ADS-B TgtRep with invalid ECAT");
                return false;
            }

            /* ECAT:
             * 0 = No ADS-B Emitter Category Information
             * 1 = light aircraft <= 15500 lbs
             * 2 = 15500 lbs < small aircraft <75000 lbs
             * 3 = 75000 lbs < medium a/c < 300000 lbs
             * 4 = High Vortex Large
             * 5 = 300000 lbs <= heavy aircraft
             * 6 = highly manoeuvrable (5g acceleration
             * capability) and high speed (>400 knots
             * cruise)
             * 7 to 9 = reserved
             * 10 = rotocraft
             * 11 = glider / sailplane
             * 12 = lighter-than-air
             * 13 = unmanned aerial vehicle
             * 14 = space / transatmospheric vehicle
             * 15 = ultralight / handglider / paraglider
             * 16 = parachutist / skydiver
             * 17 to 19 = reserved
             * 20 = surface emergency vehicle
             * 21 = surface service vehicle
             * 22 = fixed ground or tethered obstruction
             * 23 = cluster obstacle
             * 24 = line obstacle
             */
            if ((ecat >= 1 && ecat <= 5) || ecat == 10)
            {
                // Only keep aircraft and rotorcraft.
                return true;
            }
            return false;
        }
    }

    return false;
}

QHash<RecordType, RecordFilter::Counter> RecordFilter::counters() const
{
    return counters_;
}

RecordFilter::Counter RecordFilter::counter(RecordType rt) const
{
    return counters_.value(rt);
}

QQueue<Asterix::Record> RecordFilter::records() const
{
    return records_;
}
