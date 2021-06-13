/*!
 * \file targetreportextractortest.cpp
 * \brief Implements unit tests for the TargetReportExtractor class.
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
#include <QObject>
#include <QtTest>

class TargetReportExtractorTest : public QObject
{
    Q_OBJECT

public:
    enum TestType
    {
        cat010SmrTgtRep,
        cat010SmrSrvMsg,
        cat010MlatTgtRep,
        cat010MlatSrvMsg,
        cat021Adsb
    };

private slots:
    void initTestCase();
    void test_data();
    void test();

    /* TODO: Consider adding a test that takes as input a sequence of
     * records of different nature (resembling a real stream of data).
     */
};

Q_DECLARE_METATYPE(TargetReportExtractorTest::TestType);

void TargetReportExtractorTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-targetreportextractortest"));

    QSettings settings;
    settings.clear();

    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 219);
}

void TargetReportExtractorTest::test_data()
{
    // TARGET REPORTS.

    // SMR:

    // Record 0: [ToD: 23:59:58.000], CAT010 SMR Target Report.
    Asterix::Record cat010Smr0(quint8(10),
        QDateTime::fromString(QLatin1String("2020-05-05T23:59:58.000Z"), Qt::ISODateWithMs));

    cat010Smr0.rec_typ_ = RecordType(SystemType::Smr, MessageType::TargetReport);

    cat010Smr0.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("7")));

    cat010Smr0.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010Smr0.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("3"))
                                            << Asterix::DataElement(QLatin1String("DCR"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CHN"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("GBS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CRT"), QLatin1String("0")));

    cat010Smr0.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("86398.0000000")));

    cat010Smr0.dataItems_[QLatin1String("I040")] =
        Asterix::DataItem(QLatin1String("I040"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("RHO"), QLatin1String("638.0000000"))
                                            << Asterix::DataElement(QLatin1String("Theta"), QLatin1String("117.6196289")));

    cat010Smr0.dataItems_[QLatin1String("I042")] =
        Asterix::DataItem(QLatin1String("I042"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("X"), QLatin1String("565.0000000"))
                                            << Asterix::DataElement(QLatin1String("Y"), QLatin1String("-295.0000000")));

    cat010Smr0.dataItems_[QLatin1String("I200")] =
        Asterix::DataItem(QLatin1String("I200"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("GS"), QLatin1String("0.0026245"))
                                            << Asterix::DataElement(QLatin1String("TA"), QLatin1String("104.1668701")));

    cat010Smr0.dataItems_[QLatin1String("I202")] =
        Asterix::DataItem(QLatin1String("I202"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Vx"), QLatin1String("4.7500000"))
                                            << Asterix::DataElement(QLatin1String("Vy"), QLatin1String("-1.0000000")));

    cat010Smr0.dataItems_[QLatin1String("I161")] =
        Asterix::DataItem(QLatin1String("I161"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TrkNb"), QLatin1String("1391")));

    cat010Smr0.dataItems_[QLatin1String("I170")] =
        Asterix::DataItem(QLatin1String("I170"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("CNF"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TRE"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CST"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("MAH"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TCC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("STH"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TOM"), QLatin1String("3"))
                                            << Asterix::DataElement(QLatin1String("DOU"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("MRS"), QLatin1String("0")));

    cat010Smr0.dataItems_[QLatin1String("I270")] =
        Asterix::DataItem(QLatin1String("I270"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Length"), QLatin1String("38.0000000"))
                                            << Asterix::DataElement(QLatin1String("Ori"), QLatin1String("98.3500000"))
                                            << Asterix::DataElement(QLatin1String("Width"), QLatin1String("33.0000000")));

    cat010Smr0.dataItems_[QLatin1String("I210")] =
        Asterix::DataItem(QLatin1String("I210"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Ax"), QLatin1String("3.0000000"))
                                            << Asterix::DataElement(QLatin1String("Ay"), QLatin1String("0.0000000")));

    // MLAT:

    // Record 0: [ToD: 23:59:58.000], CAT010 MLAT Target Report.
    Asterix::Record cat010Mlat0(quint8(10),
        QDateTime::fromString(QLatin1String("2020-05-05T23:59:58.000Z"), Qt::ISODateWithMs));

    cat010Mlat0.rec_typ_ = RecordType(SystemType::Mlat, MessageType::TargetReport);

    cat010Mlat0.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("107")));

    cat010Mlat0.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010Mlat0.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("DCR"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CHN"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("GBS"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("CRT"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIM"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TST"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("RAB"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("LOP"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TOT"), QLatin1String("1")));

    cat010Mlat0.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("86398.0000000")));

    cat010Mlat0.dataItems_[QLatin1String("I042")] =
        Asterix::DataItem(QLatin1String("I042"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("X"), QLatin1String("565.0000000"))
                                            << Asterix::DataElement(QLatin1String("Y"), QLatin1String("-295.0000000")));

    cat010Mlat0.dataItems_[QLatin1String("I200")] =
        Asterix::DataItem(QLatin1String("I200"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("GS"), QLatin1String("0.0026245"))
                                            << Asterix::DataElement(QLatin1String("TA"), QLatin1String("104.1668701")));

    cat010Mlat0.dataItems_[QLatin1String("I202")] =
        Asterix::DataItem(QLatin1String("I202"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Vx"), QLatin1String("4.7500000"))
                                            << Asterix::DataElement(QLatin1String("Vy"), QLatin1String("-1.0000000")));

    cat010Mlat0.dataItems_[QLatin1String("I161")] =
        Asterix::DataItem(QLatin1String("I161"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TrkNb"), QLatin1String("1391")));

    cat010Mlat0.dataItems_[QLatin1String("I170")] =
        Asterix::DataItem(QLatin1String("I170"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("CNF"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TRE"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CST"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("MAH"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TCC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("STH"), QLatin1String("0")));

    cat010Mlat0.dataItems_[QLatin1String("I060")] =
        Asterix::DataItem(QLatin1String("I060"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("V"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("G"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("L"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("Mod3A"), QLatin1String("0246")));

    cat010Mlat0.dataItems_[QLatin1String("I220")] =
        Asterix::DataItem(QLatin1String("I220"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("34304F")));

    cat010Mlat0.dataItems_[QLatin1String("I245")] =
        Asterix::DataItem(QLatin1String("I245"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("STI"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TId"), QLatin1String("ECKJQ   ")));

    cat010Mlat0.dataItems_[QLatin1String("I090")] =
        Asterix::DataItem(QLatin1String("I090"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("V"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("G"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("FL"), QLatin1String("-1.0000000")));

    // ADS-B:

    // Record 0: [ToD: 23:59:58.000], CAT010 ADS-B Target Report.
    Asterix::Record cat021Adsb0(quint8(21),
        QDateTime::fromString(QLatin1String("2020-05-05T23:59:58.000Z"), Qt::ISODateWithMs));

    cat021Adsb0.rec_typ_ = RecordType(SystemType::Adsb, MessageType::TargetReport);

    cat021Adsb0.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("20"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("219")));

    cat021Adsb0.dataItems_[QLatin1String("I040")] =
        Asterix::DataItem(QLatin1String("I040"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ATP"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("ARC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("RC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("RAB"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("DCR"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("GBS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIM"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TST"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SAA"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CL"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("IPC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("NOGO"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CPR"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("LDPJ"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("RCF"), QLatin1String("0")));

    cat021Adsb0.dataItems_[QLatin1String("I161")] =
        Asterix::DataItem(QLatin1String("I161"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TrackN"), QLatin1String("3389")));

    cat021Adsb0.dataItems_[QLatin1String("I015")] =
        Asterix::DataItem(QLatin1String("I015"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("id"), QLatin1String("1")));

    cat021Adsb0.dataItems_[QLatin1String("I130")] =
        Asterix::DataItem(QLatin1String("I130"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Lat"), QLatin1String("41.2847972"))
                                            << Asterix::DataElement(QLatin1String("Lon"), QLatin1String("2.0816517")));

    cat021Adsb0.dataItems_[QLatin1String("I131")] =
        Asterix::DataItem(QLatin1String("I131"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Lat"), QLatin1String("41.2848056"))
                                            << Asterix::DataElement(QLatin1String("Lon"), QLatin1String("2.0816596")));

    cat021Adsb0.dataItems_[QLatin1String("I080")] =
        Asterix::DataItem(QLatin1String("I080"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("70605A")));

    cat021Adsb0.dataItems_[QLatin1String("I073")] =
        Asterix::DataItem(QLatin1String("I073"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("time_reception_position"), QLatin1String("31032.2656250")));

    cat021Adsb0.dataItems_[QLatin1String("I075")] =
        Asterix::DataItem(QLatin1String("I075"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("time_reception_velocity"), QLatin1String("31032.4843750")));

    cat021Adsb0.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("geometric_height"), QLatin1String("200.0000000")));

    cat021Adsb0.dataItems_[QLatin1String("I090")] =
        Asterix::DataItem(QLatin1String("I090"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("NUCr_or_NACv"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("NUCp_or_NIC"), QLatin1String("8"))
                                            << Asterix::DataElement(QLatin1String("NICbaro"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("SIL"), QLatin1String("3"))
                                            << Asterix::DataElement(QLatin1String("NACp"), QLatin1String("9"))
                                            << Asterix::DataElement(QLatin1String("SIL_supplement"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SDA"), QLatin1String("2"))
                                            << Asterix::DataElement(QLatin1String("GVA"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("PIC"), QLatin1String("11")));

    cat021Adsb0.dataItems_[QLatin1String("I210")] =
        Asterix::DataItem(QLatin1String("I210"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("VNS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("VN"), QLatin1String("2"))
                                            << Asterix::DataElement(QLatin1String("LTT"), QLatin1String("2")));

    cat021Adsb0.dataItems_[QLatin1String("I070")] =
        Asterix::DataItem(QLatin1String("I070"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Mode3A"), QLatin1String("4553")));

    cat021Adsb0.dataItems_[QLatin1String("I145")] =
        Asterix::DataItem(QLatin1String("I145"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("FL"), QLatin1String("0.2500000")));

    cat021Adsb0.dataItems_[QLatin1String("I200")] =
        Asterix::DataItem(QLatin1String("I200"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ICF"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("LNAV"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("PS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SS"), QLatin1String("0")));

    cat021Adsb0.dataItems_[QLatin1String("I155")] =
        Asterix::DataItem(QLatin1String("I155"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("RE"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("BVR"), QLatin1String("62.5000000")));

    cat021Adsb0.dataItems_[QLatin1String("I160")] =
        Asterix::DataItem(QLatin1String("I160"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("RE"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("GS"), QLatin1String("0.0284424"))
                                            << Asterix::DataElement(QLatin1String("TA"), QLatin1String("65.1873779")));

    cat021Adsb0.dataItems_[QLatin1String("I077")] =
        Asterix::DataItem(QLatin1String("I077"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("time_report_transmission"), QLatin1String("31032.6484375")));

    cat021Adsb0.dataItems_[QLatin1String("I170")] =
        Asterix::DataItem(QLatin1String("I170"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TId"), QLatin1String("KAC1412 ")));

    cat021Adsb0.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ECAT"), QLatin1String("5")));

    cat021Adsb0.dataItems_[QLatin1String("I146")] =
        Asterix::DataItem(QLatin1String("I146"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAS"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("Source"), QLatin1String("2"))
                                            << Asterix::DataElement(QLatin1String("Alt"), QLatin1String("6025.0000000")));

    cat021Adsb0.dataItems_[QLatin1String("I016")] =
        Asterix::DataItem(QLatin1String("I016"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("RP"), QLatin1String("4.0000000")));

    cat021Adsb0.dataItems_[QLatin1String("I008")] =
        Asterix::DataItem(QLatin1String("I008"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("RA"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("ARV"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CDTI_A"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("not_TCAS"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("SA"), QLatin1String("0")));

    cat021Adsb0.dataItems_[QLatin1String("I271")] =
        Asterix::DataItem(QLatin1String("I271"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("POA"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CDTI_S"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("B2_low"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("RAS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("IDENT"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("length_width"), QLatin1String("12")));

    cat021Adsb0.dataItems_[QLatin1String("I132")] =
        Asterix::DataItem(QLatin1String("I132"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MAM"), QLatin1String("-20.0000000")));

    cat021Adsb0.dataItems_[QLatin1String("I400")] =
        Asterix::DataItem(QLatin1String("I400"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("RID"), QLatin1String("1")));

    cat021Adsb0.dataItems_[QLatin1String("I295")] =
        Asterix::DataItem(QLatin1String("I295"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("AOS"), QLatin1String("1.3000000"))
                                            << Asterix::DataElement(QLatin1String("TRD"), QLatin1String("0.4000000"))
                                            << Asterix::DataElement(QLatin1String("M3A"), QLatin1String("3.7000000"))
                                            << Asterix::DataElement(QLatin1String("QI"), QLatin1String("0.4000000"))
                                            << Asterix::DataElement(QLatin1String("MAM"), QLatin1String("0.2000000"))
                                            << Asterix::DataElement(QLatin1String("GH"), QLatin1String("0.2000000"))
                                            << Asterix::DataElement(QLatin1String("FL"), QLatin1String("0.4000000"))
                                            << Asterix::DataElement(QLatin1String("ISA"), QLatin1String("1.3000000"))
                                            << Asterix::DataElement(QLatin1String("BVR"), QLatin1String("0.2000000"))
                                            << Asterix::DataElement(QLatin1String("GV"), QLatin1String("0.2000000"))
                                            << Asterix::DataElement(QLatin1String("TI"), QLatin1String("1.8000000"))
                                            << Asterix::DataElement(QLatin1String("TS"), QLatin1String("3.7000000"))
                                            << Asterix::DataElement(QLatin1String("SCC"), QLatin1String("2.2000000")));

    TargetReport trSmr;
    trSmr.ds_id_.sac_ = 0;
    trSmr.ds_id_.sic_ = 7;
    trSmr.sys_typ_ = SystemType::Smr;
    trSmr.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T23:59:58.000Z"), Qt::ISODateWithMs);
    trSmr.trk_nb_ = 1391;
    trSmr.on_gnd_ = true;
    trSmr.x_ = 565.0000000 + 1394.655251;
    trSmr.y_ = -295.0000000 - 161.695809;
    trSmr.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport trMlat;
    trMlat.ds_id_.sac_ = 0;
    trMlat.ds_id_.sic_ = 107;
    trMlat.sys_typ_ = SystemType::Mlat;
    trMlat.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T23:59:58.000Z"), Qt::ISODateWithMs);
    trMlat.trk_nb_ = 1391;
    trMlat.mode_S_ = 0x34304F;
    trMlat.mode_3A_ = 0246;
    trMlat.ident_ = QLatin1String("ECKJQ   ");
    trMlat.on_gnd_ = true;
    trMlat.x_ = 565.0000000;
    trMlat.y_ = -295.0000000;
    trMlat.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    TargetReport trAdsb;
    trAdsb.ds_id_.sac_ = 0;
    trAdsb.ds_id_.sic_ = 219;
    trAdsb.sys_typ_ = SystemType::Adsb;
    trAdsb.tod_ = QDateTime::fromString(QLatin1String("2020-05-05T23:59:58.000Z"), Qt::ISODateWithMs);
    trAdsb.trk_nb_ = 1391;
    trAdsb.mode_S_ = 0x70605A;
    trAdsb.mode_3A_ = 4553;
    trAdsb.ident_ = QLatin1String("KAC1412 ");
    trAdsb.on_gnd_ = false;
    trAdsb.x_ = 565.0000000;
    trAdsb.y_ = -295.0000000;
    trAdsb.z_ = 0;
    trAdsb.area_ = Aerodrome::NamedArea(Aerodrome::Area::Runway);

    QVector<Asterix::Record> smrRecsIn;
    smrRecsIn << cat010Smr0;

    QVector<Asterix::Record> mlatRecsIn;
    mlatRecsIn << cat010Mlat0;

    QVector<Asterix::Record> adsbRecsIn;
    adsbRecsIn << cat021Adsb0;

    QVector<TargetReport> smrTgtRepsOut;
    smrTgtRepsOut << trSmr;

    QVector<TargetReport> mlatTgtRepsOut;
    mlatTgtRepsOut << trMlat;

    QVector<TargetReport> adsbTgtRepsOut;
    adsbTgtRepsOut << trAdsb;

    QTest::addColumn<SystemType>("sysType");
    QTest::addColumn<QVector<Asterix::Record>>("recsIn");
    QTest::addColumn<QVector<TargetReport>>("tgtRepsOut");

    QTest::newRow("SMR") << SystemType::Smr << smrRecsIn << smrTgtRepsOut;
    QTest::newRow("MLAT") << SystemType::Mlat << mlatRecsIn << mlatTgtRepsOut;
    //QTest::newRow("ADS-B") << SystemType::Adsb << adsbRecsIn << adsbTgtRepsOut;
}

void TargetReportExtractorTest::test()
{
    QFETCH(SystemType, sysType);
    QFETCH(QVector<Asterix::Record>, recsIn);
    QFETCH(QVector<TargetReport>, tgtRepsOut);

    QGeoCoordinate leblArpGeo(41.297076579982225, 2.0784629201158662, 4.3200000000000003);
    QGeoCoordinate leblSmrGeo(41.29561944, 2.095113889, 4.3200000000000003);

    auto runwayCallback = [](const QVector3D cartPos, const bool gndBit) {
        Q_UNUSED(cartPos);
        Q_UNUSED(gndBit);
        return Aerodrome::NamedArea(Aerodrome::Area::Runway);
    };

    TargetReportExtractor tgtRepExtr(leblArpGeo, leblSmrGeo);
    tgtRepExtr.setLocatePointCallback(runwayCallback);

    // Feed Records.
    for (Asterix::Record &rin : recsIn)
    {
        tgtRepExtr.addData(rin);
    }

    QQueue<TargetReport> trqueue = tgtRepExtr.targetReports(sysType);
    Counters::InOutCounter counter = tgtRepExtr.counters(sysType);

    QVERIFY(trqueue.size() == tgtRepsOut.size());

    QCOMPARE(counter.in, recsIn.size());
    QCOMPARE(counter.out, tgtRepsOut.size());

    for (int i = 0; i < tgtRepsOut.size(); ++i)
    {
        QCOMPARE(trqueue.at(i), tgtRepsOut.at(i));
    }
}

QTEST_GUILESS_MAIN(TargetReportExtractorTest);
#include "targetreportextractortest.moc"
