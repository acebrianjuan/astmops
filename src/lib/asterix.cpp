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

// DataElement:
Asterix::DataElement::DataElement(const QString &name, const QString &value)
    : name_(name), value_(value)
{
}

// DataItem:
Asterix::DataItem::DataItem(const QString &name, const QVector<DataElement> &data)
    : name_(name)
{
    for (const Asterix::DataElement &de : data)
    {
        data_.insert(de.name_, de);
    }
}

std::optional<Asterix::DataElement> Asterix::DataItem::element(QLatin1String deName) const
{
    if (data_.contains(deName))
    {
        return data_.value(deName);
    }

    return std::nullopt;
}

// Record:
Asterix::Record::Record(const quint8 cat, const QDateTime &dateTime,
    const QVector<Asterix::DataItem> &dataItems)
    : cat_(cat), timestamp_(dateTime)
{
    for (const Asterix::DataItem &di : dataItems)
    {
        dataItems_.insert(di.name_, di);
    }
}

std::optional<Asterix::DataItem> Asterix::Record::dataItem(QLatin1String diName) const
{
    if (dataItems_.contains(diName))
    {
        return dataItems_.value(diName);
    }

    return std::nullopt;
}

// Functions:
bool Asterix::containsDataItem(const Asterix::Record &rec, QLatin1String diName)
{
    return rec.dataItem(diName).has_value();
}

bool Asterix::containsDataItem(const Asterix::Record &rec, QVector<QLatin1String> diNames)
{
    for (QLatin1String diName : diNames)
    {
        if (!containsDataItem(rec, diName))
        {
            return false;
        }
    }

    // Returns true only if Record contains ALL Data Items.
    return true;
}

bool Asterix::containsElement(const Asterix::Record &rec, QLatin1String diName, QLatin1String deName)
{
    return containsDataItem(rec, diName) && rec.dataItem(diName).value().element(deName).has_value();
}

std::optional<QString> Asterix::getElementValue(const Asterix::Record &rec, QLatin1String diName, QLatin1String deName)
{
    std::optional<Asterix::DataItem> di = rec.dataItem(diName);
    if (di.has_value())  // Data Item exists.
    {
        std::optional<Asterix::DataElement> el = di.value().element(deName);
        if (el.has_value())  // Data Element exists.
        {
            return el.value().value_;
        }
    }

    return std::nullopt;
}

RecordType Asterix::getRecordType(const Asterix::Record &rec)
{
    SystemType st = SystemType::Unknown;
    MessageType mt = MessageType::Unknown;
    bool ok;

    quint8 smrSic = Configuration::smrSic();
    quint8 mlatSic = Configuration::mlatSic();
    quint8 adsbSic = Configuration::adsbSic();

    /* SUPPORTED ASTERIX CATEGORIES:
     * CAT010: Monosensor Surface Movement Data
     * CAT020: MLT Messages
     * CAT021: ADS-B Messages
     * CAT023: CNS/ATM Ground Station Service Messages
     */

    switch (rec.cat_)
    {
    case 10:  // CAT010: Monosensor Surface Movement Data.
    {
        // System Identification Code (SIC).
        bool has_sic = Asterix::containsElement(rec, QLatin1String("I010"), QLatin1String("SIC"));

        if (!has_sic)
        {
            qWarning("CAT010 record without SIC");
            break;
        }

        quint8 sic = Asterix::getElementValue(rec, QLatin1String("I010"), QLatin1String("SIC")).value().toUInt(&ok);

        if (!ok)
        {
            qWarning("CAT010 record with invalid SIC");
            break;
        }

        if (sic == smrSic)
        {
            st = SystemType::Smr;
        }
        else if (sic == mlatSic)
        {
            st = SystemType::Mlat;
        }

        // Message type.
        bool has_msg_typ = Asterix::containsElement(rec, QLatin1String("I000"), QLatin1String("MsgTyp"));

        if (!has_msg_typ)
        {
            qWarning("CAT010 record without message type");
            break;
        }

        quint8 msg_typ = Asterix::getElementValue(rec, QLatin1String("I000"), QLatin1String("MsgTyp")).value().toUInt(&ok);

        if (!ok)
        {
            qWarning("CAT010 record with invalid message type");
            break;
        }

        /* MsgTyp:
         * 001 Target Report
         * 002 Start of Update Cycle
         * 003 Periodic Status Message
         * 004 Event-triggered Status Message
         */
        if (msg_typ == 1)
        {
            mt = MessageType::TargetReport;
        }
        else if (msg_typ > 1 && msg_typ <= 4)
        {
            mt = MessageType::ServiceMessage;
        }
        else
        {
            qWarning("CAT010 record of unknown message type: %03d", msg_typ);
            break;
        }

        // Validate System Type for Target reports.
        if (mt == MessageType::TargetReport)
        {
            // System type.
            bool has_sys_typ = Asterix::containsElement(rec, QLatin1String("I020"), QLatin1String("TYP"));

            if (!has_sys_typ)
            {
                qWarning("CAT010 TgtRep without system type");
                break;
            }

            /* TYP:
             * 000 SSR multilateration
             * 001 Mode S multilateration
             * 010 ADS-B
             * 011 PSR
             * 100 Magnetic Loop System
             * 101 HF multilateration
             * 110 Not defined
             * 111 Other types
             */
            quint8 sys_typ = Asterix::getElementValue(rec, QLatin1String("I020"), QLatin1String("TYP")).value().toUInt(&ok);

            if (!ok)
            {
                qWarning("CAT010 TgtRep with invalid system type");
                break;
            }

            if (st == SystemType::Mlat)
            {
                if (sys_typ != 1)
                {
                    qWarning("CAT010 TgtRep with disagreeing system type");
                    break;
                }
            }
            else if (st == SystemType::Smr)
            {
                if (sys_typ != 3)
                {
                    qWarning("CAT010 TgtRep with disagreeing system type");
                    break;
                }
            }
        }

        break;
    }
    case 20:  // CAT020: MLT Messages.
    {
        st = SystemType::Mlat;
        mt = MessageType::TargetReport;

        break;
    }
    case 21:  // CAT021: ADS-B Messages.
    {
        st = SystemType::Adsb;
        mt = MessageType::TargetReport;

        // System Identification Code (SIC).
        bool has_sic = Asterix::containsElement(rec, QLatin1String("I010"), QLatin1String("SIC"));

        if (!has_sic)
        {
            qWarning("CAT010 TgtRep without SIC");
            break;
        }

        quint8 sic = Asterix::getElementValue(rec, QLatin1String("I010"), QLatin1String("SIC")).value().toUInt(&ok);

        if (!ok)
        {
            qWarning("CAT021 TgtRep with invalid SIC");
            break;
        }

        if (sic != adsbSic)
        {
            qWarning("CAT021 TgtRep with disagreeing SIC");
            break;
        }
        break;
    }
    case 23:  // CAT023: CNS/ATM Ground Station Service Messages.
    {
        st = SystemType::Adsb;
        mt = MessageType::ServiceMessage;

        break;
    }
    default:
    {
        qWarning("ASTERIX category %03d not supported", rec.cat_);
    }
    }

    return RecordType(st, mt);
}

bool Asterix::operator==(const Asterix::DataElement &lhs, const Asterix::DataElement &rhs)
{
    return lhs.name_ == rhs.name_ &&
           lhs.value_ == rhs.value_;
}

bool Asterix::operator==(const Asterix::DataItem &lhs, const Asterix::DataItem &rhs)
{
    return lhs.name_ == rhs.name_ &&
           lhs.data_ == rhs.data_;
}

bool Asterix::operator==(const Asterix::Record &lhs, const Asterix::Record &rhs)
{
    return lhs.cat_ == rhs.cat_ &&
           lhs.len_ == rhs.len_ &&
           lhs.crc_ == rhs.crc_ &&
           lhs.timestamp_ == rhs.timestamp_ &&
           lhs.dataItems_ == rhs.dataItems_;
}
