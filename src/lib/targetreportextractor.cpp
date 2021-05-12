/*!
 * \file targetreportextractor.cpp
 * \brief Implementation of the TargetReportExtractor class.
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

#include "targetreportextractor.h"
#include "geofunctions.h"

TargetReportExtractor::TargetReportExtractor(const QGeoCoordinate &arp, const QGeoCoordinate &smr)
    : arp_(arp), smr_(smr)
{
    // Initialize counters.
    counters_.insert(SystemType::Smr, Counters::InOutCounter());
    counters_.insert(SystemType::Mlat, Counters::InOutCounter());
    counters_.insert(SystemType::Adsb, Counters::InOutCounter());
}

void TargetReportExtractor::addData(const Asterix::Record &rec)
{
    if (rec.rec_typ_.isUnknown())
    {
        return;
    }

    ++counters_[rec.rec_typ_.sys_typ_].in;
    if (Asterix::hasMinimumDataItems(rec) && isRecordToBeKept(rec))
    {
        std::optional<TargetReport> tr_opt = makeTargetReport(rec);
        if (!tr_opt.has_value())
        {
            return;
        }

        TargetReport tr = tr_opt.value();

        Q_ASSERT(rec.rec_typ_.sys_typ_ == tr.sys_typ_);

        tgt_reports_[tr.sys_typ_].enqueue(tr);
        ++counters_[rec.rec_typ_.sys_typ_].out;
    }
}

QQueue<TargetReport> TargetReportExtractor::targetReports(SystemType st) const
{
    return tgt_reports_.value(st);
}

Counters::InOutCounter TargetReportExtractor::counters(SystemType st) const
{
    return counters_.value(st);
}

void TargetReportExtractor::loadExcludedAddresses(QIODevice *device)
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
            excluded_addresses_.insert(addr);
        }
    }
}

bool TargetReportExtractor::isRecordToBeKept(const Asterix::Record &rec) const
{
    // Discard records of unknown type.
    if (rec.rec_typ_.isUnknown())
    {
        return false;
    }

    // Discard all service messages regardless of the system type.
    if (rec.rec_typ_.msg_typ_ == MessageType::ServiceMessage)
    {
        /*
        if (rt.sys_typ_ == SystemType::Smr || rt.sys_typ_ == SystemType::Mlat)
        {
            // Only keep Service Messages from SMR and MLAT.
            return true;
        }
        */
        return false;
    }

    bool ok;
    if (rec.rec_typ_.msg_typ_ == MessageType::TargetReport)
    {
        if (rec.rec_typ_.sys_typ_ == SystemType::Smr)
        {
            // SMR Target Reports are always kept.
            return true;
        }
        else if (rec.rec_typ_.sys_typ_ == SystemType::Mlat)
        {
            // For MLAT Target Reports check if the Target Address is to be excluded.
            bool has_tgt_addr = Asterix::containsElement(rec, QLatin1String("I220"), QLatin1String("TAddr"));

            if (!has_tgt_addr)
            {
                qWarning("Skipping MLAT TgtRep without target address");
                return false;
            }

            ModeS tgt_addr = Asterix::getElementValue(rec, QLatin1String("I220"), QLatin1String("TAddr")).value().toUInt(&ok, 16);

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

            quint8 tot = Asterix::getElementValue(rec, QLatin1String("I020"), QLatin1String("TOT")).value().toUInt(&ok);

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
        else if (rec.rec_typ_.sys_typ_ == SystemType::Adsb)
        {
            // Check target address.
            bool has_tgt_addr = Asterix::containsElement(rec, QLatin1String("I080"), QLatin1String("TAddr"));

            if (!has_tgt_addr)
            {
                qWarning("Skipping ADS-B TgtRep without target address");
                return false;
            }

            ModeS tgt_addr = Asterix::getElementValue(rec, QLatin1String("I080"), QLatin1String("TAddr")).value().toUInt(&ok, 16);

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

            quint8 ecat = Asterix::getElementValue(rec, QLatin1String("I020"), QLatin1String("ECAT")).value().toUInt(&ok);

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

std::optional<TargetReport> TargetReportExtractor::makeTargetReport(const Asterix::Record &rec) const
{
    if (!Asterix::isCategorySupported(rec.cat_) ||
        rec.rec_typ_.isUnknown() ||
        rec.rec_typ_.msg_typ_ != MessageType::TargetReport ||
        !Asterix::hasMinimumDataItems(rec))
    {
        return std::nullopt;
    }

    TargetReport tr;
    tr.sys_typ_ = rec.rec_typ_.sys_typ_;

    // System Area Code (SAC).
    bool has_sac = Asterix::containsElement(rec, QLatin1String("I010"), QLatin1String("SAC"));
    if (!has_sac)
    {
        return std::nullopt;
    }

    bool sac_ok = false;
    Sac sac = Asterix::getElementValue(rec, QLatin1String("I010"), QLatin1String("SAC")).value().toUInt(&sac_ok);
    if (!sac_ok)
    {
        return std::nullopt;
    }

    tr.ds_id_.sac_ = sac;

    // System Identification Code (SIC).
    bool has_sic = Asterix::containsElement(rec, QLatin1String("I010"), QLatin1String("SIC"));
    if (!has_sic)
    {
        return std::nullopt;
    }

    bool sic_ok = false;
    Sic sic = Asterix::getElementValue(rec, QLatin1String("I010"), QLatin1String("SIC")).value().toUInt(&sic_ok);
    if (!sic_ok)
    {
        return std::nullopt;
    }

    tr.ds_id_.sic_ = sic;

    switch (rec.cat_)
    {
    case 10:
    {
        // TOD.
        bool has_tod = Asterix::containsElement(rec, QLatin1String("I140"), QLatin1String("ToD"));
        if (!has_tod)
        {
            return std::nullopt;
        }

        bool tod_ok = false;
        double tod = Asterix::getElementValue(rec, QLatin1String("I140"), QLatin1String("ToD")).value().toDouble(&tod_ok);
        Q_UNUSED(tod);
        if (!tod_ok)
        {
            return std::nullopt;
        }

        tr.tod_ = rec.timestamp_;

        // Track Number.
        bool has_trknb = Asterix::containsElement(rec, QLatin1String("I161"), QLatin1String("TrkNb"));
        if (!has_trknb)
        {
            return std::nullopt;
        }

        bool trknb_ok = false;
        TrackNum trknb = Asterix::getElementValue(rec, QLatin1String("I161"), QLatin1String("TrkNb")).value().toUInt(&trknb_ok);
        if (!trknb_ok)
        {
            return std::nullopt;
        }

        tr.trk_nb_ = trknb;

        // Ground bit.
        if (rec.rec_typ_.sys_typ_ == SystemType::Smr)
        {
            // By definition, SMR targets are ALWAYS on the ground.
            tr.on_gnd_ = true;
        }
        else
        {
            bool has_gbs = Asterix::containsElement(rec, QLatin1String("I020"), QLatin1String("GBS"));
            if (!has_gbs)
            {
                return std::nullopt;
            }

            bool gbs_ok = false;
            TrackNum gbs = Asterix::getElementValue(rec, QLatin1String("I020"), QLatin1String("GBS")).value().toUInt(&gbs_ok);
            if (!gbs_ok)
            {
                return std::nullopt;
            }

            tr.on_gnd_ = gbs;
        }

        // X
        bool has_x = Asterix::containsElement(rec, QLatin1String("I042"), QLatin1String("X"));
        if (!has_x)
        {
            return std::nullopt;
        }

        bool x_ok = false;
        double x = Asterix::getElementValue(rec, QLatin1String("I042"), QLatin1String("X")).value().toDouble(&x_ok);
        if (!x_ok)
        {
            return std::nullopt;
        }

        tr.x_ = x;

        // Y
        bool has_y = Asterix::containsElement(rec, QLatin1String("I042"), QLatin1String("Y"));
        if (!has_y)
        {
            return std::nullopt;
        }

        bool y_ok = false;
        double y = Asterix::getElementValue(rec, QLatin1String("I042"), QLatin1String("Y")).value().toDouble(&y_ok);
        if (!y_ok)
        {
            return std::nullopt;
        }

        tr.y_ = y;

        // Z

        if (rec.rec_typ_.sys_typ_ == SystemType::Mlat)
        {
            // Mode S.
            bool has_mode_s = Asterix::containsElement(rec, QLatin1String("I220"), QLatin1String("TAddr"));
            if (!has_mode_s)
            {
                return std::nullopt;
            }

            bool mode_s_ok = false;
            ModeS mode_s = Asterix::getElementValue(rec, QLatin1String("I220"), QLatin1String("TAddr")).value().toUInt(&mode_s_ok, 16);
            if (!mode_s_ok)
            {
                return std::nullopt;
            }

            tr.mode_S_ = mode_s;

            // Mode 3A.
            bool has_mode_3a = Asterix::containsElement(rec, QLatin1String("I060"), QLatin1String("Mod3A"));
            if (has_mode_3a)
            {
                bool mode_3a_ok = false;
                Mode3A mode_3a = Asterix::getElementValue(rec, QLatin1String("I060"), QLatin1String("Mod3A")).value().toUInt(&mode_3a_ok, 8);
                if (mode_3a_ok)
                {
                    tr.mode_3A_ = mode_3a;
                }
            }

            // Identification.
            bool has_ident = Asterix::containsElement(rec, QLatin1String("I245"), QLatin1String("TId"));
            if (has_ident)
            {
                Ident ident = Asterix::getElementValue(rec, QLatin1String("I245"), QLatin1String("TId")).value();
                if (!ident.isNull())
                {
                    tr.ident_ = ident;
                }
            }

            //            if (!has_mode_3a && !has_ident)
            //            {
            //                return std::nullopt;
            //            }
        }

        break;
    }
    case 21:
    {
        // Track Number.
        bool has_trknb = Asterix::containsElement(rec, QLatin1String("I161"), QLatin1String("TrackN"));
        if (!has_trknb)
        {
            return std::nullopt;
        }

        bool trknb_ok = false;
        TrackNum trknb = Asterix::getElementValue(rec, QLatin1String("I161"), QLatin1String("TrackN")).value().toUInt(&trknb_ok);
        if (!trknb_ok)
        {
            return std::nullopt;
        }

        tr.trk_nb_ = trknb;

        // Ground bit.
        bool has_gbs = Asterix::containsElement(rec, QLatin1String("I040"), QLatin1String("GBS"));
        if (!has_gbs)
        {
            return std::nullopt;
        }

        bool gbs_ok = false;
        TrackNum gbs = Asterix::getElementValue(rec, QLatin1String("I040"), QLatin1String("GBS")).value().toUInt(&gbs_ok);
        if (!gbs_ok)
        {
            return std::nullopt;
        }

        tr.on_gnd_ = gbs;

        // Latitude.
        double lat = qSNaN();

        bool lat_hp_ok = false;
        bool has_lat_hp = Asterix::containsElement(rec, QLatin1String("I131"), QLatin1String("Lat"));
        if (has_lat_hp)
        {
            double lat_hp = Asterix::getElementValue(rec, QLatin1String("I131"), QLatin1String("Lat")).value().toDouble(&lat_hp_ok);
            if (lat_hp_ok)
            {
                lat = lat_hp;
            }
        }

        if (!has_lat_hp || !lat_hp_ok)
        {
            bool has_lat_sp = Asterix::containsElement(rec, QLatin1String("I130"), QLatin1String("Lat"));
            if (!has_lat_sp)
            {
                return std::nullopt;
            }

            bool lat_sp_ok;
            double lat_sp = Asterix::getElementValue(rec, QLatin1String("I130"), QLatin1String("Lat")).value().toDouble(&lat_sp_ok);
            if (!lat_sp_ok)
            {
                return std::nullopt;
            }

            lat = lat_sp;
        }

        // Longitude.
        double lon = qSNaN();

        bool lon_hp_ok = false;
        bool has_lon_hp = Asterix::containsElement(rec, QLatin1String("I131"), QLatin1String("Lon"));
        if (has_lon_hp)
        {
            double lon_hp = Asterix::getElementValue(rec, QLatin1String("I131"), QLatin1String("Lon")).value().toDouble(&lon_hp_ok);
            if (lon_hp_ok)
            {
                lon = lon_hp;
            }
        }

        if (!has_lon_hp || !lon_hp_ok)
        {
            bool has_lon_sp = Asterix::containsElement(rec, QLatin1String("I130"), QLatin1String("Lon"));
            if (!has_lon_sp)
            {
                return std::nullopt;
            }

            bool lon_sp_ok;
            double lon_sp = Asterix::getElementValue(rec, QLatin1String("I130"), QLatin1String("Lon")).value().toDouble(&lon_sp_ok);
            if (!lon_sp_ok)
            {
                return std::nullopt;
            }

            lon = lon_sp;
        }

        // Height.
        double h = 0.0;

        bool mh_ok = false;
        bool has_mh = Asterix::containsElement(rec, QLatin1String("I091"), QLatin1String("MHeight"));
        if (has_mh)
        {
            double mh = Asterix::getElementValue(rec, QLatin1String("I091"), QLatin1String("MHeight")).value().toDouble(&mh_ok);
            if (mh_ok)
            {
                h = mh;
            }
        }

        if (!has_mh || !mh_ok)
        {
            bool fl_ok = false;
            bool has_fl = Asterix::containsElement(rec, QLatin1String("I145"), QLatin1String("FL"));
            if (!has_fl)
            {
                double fl = Asterix::getElementValue(rec, QLatin1String("I145"), QLatin1String("FL")).value().toDouble(&fl_ok);
                if (fl_ok)
                {
                    h = fl * FL_to_m;
                }
            }
        }

        QVector3D cart = geoToLocalEnu(QGeoCoordinate(lat, lon, h), arp_);

        tr.x_ = cart.x();
        tr.y_ = cart.y();
        tr.z_ = cart.z();

        // Mode S.
        bool mode_s_ok = false;
        bool has_mode_s = Asterix::containsElement(rec, QLatin1String("I080"), QLatin1String("TAddr"));
        if (!has_mode_s)
        {
            return std::nullopt;
        }

        ModeS mode_s = Asterix::getElementValue(rec, QLatin1String("I080"), QLatin1String("TAddr")).value().toUInt(&mode_s_ok, 16);
        if (!mode_s_ok)
        {
            return std::nullopt;
        }

        tr.mode_S_ = mode_s;

        // Mode 3A.
        bool mode_a_ok = false;
        bool has_mode_3a = Asterix::containsElement(rec, QLatin1String("I070"), QLatin1String("Mode3A"));
        if (has_mode_3a)
        {
            Mode3A mode_3a = Asterix::getElementValue(rec, QLatin1String("I070"), QLatin1String("Mode3A")).value().toUInt(&mode_a_ok, 8);
            if (mode_a_ok)
            {
                tr.mode_3A_ = mode_3a;
            }
        }

        // Identification.
        bool has_ident = Asterix::containsElement(rec, QLatin1String("I245"), QLatin1String("TId"));
        if (has_ident)
        {
            Ident ident = Asterix::getElementValue(rec, QLatin1String("I245"), QLatin1String("TId")).value();
            if (!ident.isNull())
            {
                tr.ident_ = ident;
            }
        }
    }
    }


    return tr;
}

void TargetReportExtractor::setSmr(const QGeoCoordinate &smr)
{
    smr_ = smr;
}

void TargetReportExtractor::setArp(const QGeoCoordinate &arp)
{
    arp_ = arp;
}
