/*!
 * \file asterix.cpp
 * \brief Implementation of ASTERIX protocol abstractions.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * Contains a set of abstractions for handling ASTERIX protocol data.
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

#include "asterix.h"
#include "config.h"

/* ----------------------------- DataElement ------------------------------ */

Asterix::DataElement::DataElement(const QString &name, const QString &value)
    : name_(name), value_(value)
{
}

/* ------------------------------ DataItem -------------------------------- */

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

/* ------------------------------- Record --------------------------------- */

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

/* --------------------------- Free functions ----------------------------- */

bool Asterix::containsDataItem(const Asterix::Record &rec, QLatin1String diName)
{
    return rec.dataItem(diName).has_value();
}

bool Asterix::containsDataItem(const Asterix::Record &rec, const QVector<QLatin1String> &diNames)
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

    if (!Asterix::isCategorySupported(rec.cat_))
    {
        return RecordType(st, mt);
    }

    // Read only once.
    static const QSet<Sic> smrSic = Configuration::smrSic();
    static const QSet<Sic> mlatSic = Configuration::mlatSic();
    static const QSet<Sic> adsbSic = Configuration::adsbSic();

    // SICs assigned to SMR should not be assigned to any other sensors.
    Q_ASSERT(!mlatSic.intersects(smrSic) && !adsbSic.intersects(smrSic));

    bool ok;
    switch (rec.cat_)
    {
    case 10:  // CAT010: Monosensor Surface Movement Data.
    {
        // System Identification Code (SIC).
        bool has_sic = Asterix::containsElement(rec, QLatin1String("I010"), QLatin1String("SIC"));

        if (!has_sic)
        {
            qDebug() << "CAT010 record" << rec.crc_
                     << "without SIC information";
            break;
        }

        quint8 sic = Asterix::getElementValue(rec, QLatin1String("I010"), QLatin1String("SIC")).value().toUInt(&ok);

        if (!ok)
        {
            qDebug() << "CAT010 record" << rec.crc_
                     << "with invalid SIC";
            break;
        }

        if (smrSic.contains(sic))
        {
            st = SystemType::Smr;
        }
        else if (mlatSic.contains(sic))
        {
            st = SystemType::Mlat;
        }

        // Message type.
        bool has_msg_typ = Asterix::containsElement(rec, QLatin1String("I000"), QLatin1String("MsgTyp"));

        if (!has_msg_typ)
        {
            qDebug() << "CAT010 record" << rec.crc_
                     << "without message type information";
            break;
        }

        quint8 msg_typ = Asterix::getElementValue(rec, QLatin1String("I000"), QLatin1String("MsgTyp")).value().toUInt(&ok);

        if (!ok)
        {
            qDebug() << "CAT010 record" << rec.crc_
                     << "with invalid message type";
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
            qDebug() << "CAT010 record" << rec.crc_
                     << "of unknown message type:" << msg_typ;
            break;
        }

        // Validate System Type for Target reports.
        if (mt == MessageType::TargetReport)
        {
            // System type.
            bool has_sys_typ = Asterix::containsElement(rec, QLatin1String("I020"), QLatin1String("TYP"));

            if (!has_sys_typ)
            {
                qDebug() << "CAT010 TgtRep" << rec.crc_
                         << "without system type information";
                break;
            }

            /* TYP:
             * 0b000 (0) SSR multilateration
             * 0b001 (1) Mode S multilateration
             * 0b010 (2) ADS-B
             * 0b011 (3) PSR
             * 0b100 (4) Magnetic Loop System
             * 0b101 (5) HF multilateration
             * 0b110 (6) Not defined
             * 0b111 (7) Other types
             */
            quint8 sys_typ = Asterix::getElementValue(rec, QLatin1String("I020"), QLatin1String("TYP")).value().toUInt(&ok);

            if (!ok)
            {
                qDebug() << "CAT010 TgtRep" << rec.crc_
                         << "with invalid system type";
                break;
            }

            if (st == SystemType::Mlat)
            {
                if (sys_typ != 1)
                {
                    qDebug() << "CAT010 TgtRep" << rec.crc_
                             << "with disagreeing system type";
                    break;
                }
            }
            else if (st == SystemType::Smr)
            {
                if (sys_typ != 3)
                {
                    qDebug() << "CAT010 TgtRep" << rec.crc_
                             << "with disagreeing system type ";
                    break;
                }
            }
        }

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
            qDebug() << "CAT021 TgtRep" << rec.crc_
                     << "without SIC information";
            break;
        }

        quint8 sic = Asterix::getElementValue(rec, QLatin1String("I010"), QLatin1String("SIC")).value().toUInt(&ok);

        if (!ok)
        {
            qDebug() << "CAT021 TgtRep" << rec.crc_
                     << "with invalid SIC";
            break;
        }

        if (!adsbSic.contains(sic))
        {
            qDebug() << "CAT021 TgtRep" << rec.crc_
                     << "with disagreeing SIC";
            break;
        }
        break;
    }
    default:
    {
        qDebug("ASTERIX category %03d not supported", rec.cat_);
    }
    }

    return RecordType(st, mt);
}

QTime Asterix::getTimeOfDay(const Asterix::Record &rec)
{
    if (!Asterix::isCategorySupported(rec.cat_))
    {
        return QTime();
    }

    switch (rec.cat_)
    {
    case 10:  // CAT010: Monosensor Surface Movement Data.
    {
        bool has_tod = Asterix::containsElement(rec, QLatin1String("I140"), QLatin1String("ToD"));
        if (!has_tod)
        {
            return QTime();
        }

        bool tod_ok = false;
        double tod = Asterix::getElementValue(rec, QLatin1String("I140"), QLatin1String("ToD")).value().toDouble(&tod_ok);
        if (!tod_ok)
        {
            return QTime();
        }

        return QTime::fromMSecsSinceStartOfDay(tod * 1000);
    }
    case 21:  // CAT021: ADS-B Messages.
    {
        // Cascade down through the different time-related Data Items in order
        // of preference until a valid TOD is read. Otherwise return an
        // invalid QTime object.

        bool tod_ok = false;
        double tod;
        if (Asterix::containsElement(rec, QLatin1String("I071"), QLatin1String("time_applicability_position")))
        {
            tod = Asterix::getElementValue(rec, QLatin1String("I071"), QLatin1String("time_applicability_position")).value().toDouble(&tod_ok);
        }

        if (!tod_ok && Asterix::containsElement(rec, QLatin1String("I073"), QLatin1String("time_reception_position")))
        {
            tod = Asterix::getElementValue(rec, QLatin1String("I073"), QLatin1String("time_reception_position")).value().toDouble(&tod_ok);

            if (tod_ok && Asterix::containsDataItem(rec, QLatin1String("I074")))
            {
                bool fsi_ok = false;
                quint8 fsi;
                if (Asterix::containsElement(rec, QLatin1String("I074"), QLatin1String("FSI")))
                {
                    fsi = Asterix::getElementValue(rec, QLatin1String("I074"), QLatin1String("FSI")).value().toUInt(&fsi_ok);
                }

                if (fsi_ok)
                {
                    bool frac_ok = false;
                    double fracPart;
                    if (Asterix::containsElement(rec, QLatin1String("I074"), QLatin1String("time_reception_position_highprecision")))
                    {
                        fracPart = Asterix::getElementValue(rec, QLatin1String("I074"), QLatin1String("time_reception_position_highprecision")).value().toDouble(&frac_ok);
                    }

                    if (frac_ok)
                    {
                        double intPart;
                        std::modf(tod, &intPart);

                        if (fsi != 3)
                        {
                            if (fsi == 1)
                            {
                                intPart += 1;
                            }
                            else if (fsi == 2)
                            {
                                intPart -= 1;
                            }

                            tod = intPart + (fracPart * qPow(2, -30));
                        }
                    }
                }
            }
        }

        if (!tod_ok && Asterix::containsElement(rec, QLatin1String("I077"), QLatin1String("time_report_transmission")))
        {
            tod = Asterix::getElementValue(rec, QLatin1String("I077"), QLatin1String("time_report_transmission")).value().toDouble(&tod_ok);
        }

        if (!tod_ok)
        {
            return QTime();
        }

        return QTime::fromMSecsSinceStartOfDay(tod * 1000);
    }
    }

    return QTime();
}

bool Asterix::isCategorySupported(const Cat cat)
{
    /* SUPPORTED ASTERIX CATEGORIES:
     * CAT010: Monosensor Surface Movement Data
     * CAT021: ADS-B Messages
     */
    if (cat == 10 || cat == 21)
    {
        return true;
    }

    return false;
}

bool Asterix::hasMinimumDataItems(const Asterix::Record &rec)
{
    Q_ASSERT(Asterix::isCategorySupported(rec.cat_) && !rec.rec_typ_.isUnknown());

    QVector<Asterix::DataItemList> ditems;

    switch (rec.cat_)
    {
    case 10:
    {
        if (rec.rec_typ_.msg_typ_ == MessageType::ServiceMessage)
        {
            Asterix::DataItemList mandatory;
            mandatory.type_ = Asterix::DataItemListType::Mandatory;
            mandatory.items_ = QStringList() << QLatin1String("I000")   // Message Type
                                             << QLatin1String("I010")   // Data Source Identifier
                                             << QLatin1String("I140")   // Time of Day
                                             << QLatin1String("I550");  // System Status

            ditems << mandatory;
        }
        else if (rec.rec_typ_.msg_typ_ == MessageType::TargetReport)
        {
            if (rec.rec_typ_.sys_typ_ == SystemType::Smr)
            {
                Asterix::DataItemList mandatory;
                mandatory.type_ = Asterix::DataItemListType::Mandatory;
                mandatory.items_ = QStringList() << QLatin1String("I000")   // Message Type
                                                 << QLatin1String("I010")   // Data Source Identifier
                                                 << QLatin1String("I020")   // Target Report Descriptor
                                                 << QLatin1String("I140")   // Time of Day
                                                 << QLatin1String("I161")   // Track Number
                                                 << QLatin1String("I270");  // Target Size & Orientation

                Asterix::DataItemList disjunctive;
                disjunctive.type_ = Asterix::DataItemListType::Disjunctive;
                disjunctive.items_ = QStringList() << QLatin1String("I040")   // Position in Polar Co-ordinates
                                                   << QLatin1String("I041")   // Position in WGS-84 Coordinates
                                                   << QLatin1String("I042");  // Position in Cartesian Coordinates

                ditems << mandatory << disjunctive;
            }
            else if (rec.rec_typ_.sys_typ_ == SystemType::Mlat)
            {
                Asterix::DataItemList mandatory;
                mandatory.type_ = Asterix::DataItemListType::Mandatory;
                mandatory.items_ = QStringList() << QLatin1String("I000")   // Message Type
                                                 << QLatin1String("I010")   // Data Source Identifier
                                                 << QLatin1String("I020")   // Target Report Descriptor
                                                 << QLatin1String("I140")   // Time of Day
                                                 << QLatin1String("I161")   // Track Number
                                                 << QLatin1String("I220");  // Mode S Target Address (ICAO)

                Asterix::DataItemList disjunctive1;
                disjunctive1.type_ = Asterix::DataItemListType::Disjunctive;
                disjunctive1.items_ = QStringList() << QLatin1String("I041")   // Position in WGS-84 Coordinates
                                                    << QLatin1String("I042");  // Position in Cartesian Coordinates

                Asterix::DataItemList disjunctive2;
                disjunctive2.type_ = Asterix::DataItemListType::Disjunctive;
                disjunctive2.items_ = QStringList() << QLatin1String("I060")   // Mode 3/A Code in Octal (SQUAWK)
                                                    << QLatin1String("I245");  // Target Identification (CALLSIGN)

                ditems << mandatory << disjunctive1 << disjunctive2;
            }
        }
    }
    case 21:
    {
    }
    }

    return Asterix::checkDataItems(rec, ditems);
}

bool Asterix::checkDataItems(const Asterix::Record &rec,
    const QVector<DataItemList> &col)
{
    // Return true if colection of Data Items to be found is empty.
    if (col.isEmpty())
    {
        return true;
    }

    // Return false if record has no Data Items.
    if (rec.dataItems_.isEmpty())
    {
        return false;
    }

    int count = col.size();
    for (const DataItemList &diList : col)
    {
        if (checkDataItemsList(rec, diList.items_, diList.type_))
        {
            // Check succeeded. Decrease counter.
            --count;
        }
        else
        {
            // Check failed. Exit the loop.
            break;
        }
    }

    if (count == 0)
    {
        // All checks succeeded.
        return true;
    }

    return false;
}

bool Asterix::checkDataItemsList(const Asterix::Record &rec,
    const QStringList &list, DataItemListType type)
{
    // Return true if list of Data Items to be found is empty.
    if (list.isEmpty())
    {
        return true;
    }

    // Return false if record has no Data Items.
    if (rec.dataItems_.isEmpty())
    {
        return false;
    }

    auto makeHash = [](const QStringList &list, bool state = false) {
        Q_ASSERT(!list.isEmpty());

        QHash<QString, bool> hash;

        for (const QString &key : list)
        {
            hash.insert(key, state);
        }

        return hash;
    };

    if (type == Asterix::DataItemListType::Disjunctive)
    {
        for (const Asterix::DataItem &di : rec.dataItems_)
        {
            if (di.isNull())
            {
                continue;
            }

            QString diName = di.name_;
            if (list.contains(diName))
            {
                // One match is enough. Return true.
                return true;
            }
        }
    }
    else  // Type is Mandatory.
    {
        /* Count how many false there are at the start,
         * decrement that number when you flip one.
         * Stop when there's none left.
         */
        QHash<QString, bool> hash = makeHash(list);
        int count = hash.size();
        if (rec.dataItems_.size() >= count)
        {
            for (const Asterix::DataItem &di : rec.dataItems_)
            {
                if (count == 0)
                {
                    break;
                }

                if (di.isNull())
                {
                    continue;
                }

                QString diName = di.name_;
                QHash<QString, bool>::iterator it = hash.find(diName);
                if (it != hash.end())
                {
                    *it = true;
                    --count;
                }
            }

            if (count == 0)
            {
                // All items found.
                return true;
            }
        }
    }

    return false;
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
           //lhs.len_ == rhs.len_ &&
           //lhs.crc_ == rhs.crc_ &&
           lhs.timestamp_ == rhs.timestamp_ &&
           lhs.dataItems_ == rhs.dataItems_;
}
