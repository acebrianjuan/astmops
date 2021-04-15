/*!
 * \file recordfiltertest.cpp
 * \brief Implements unit tests for the RecordFilter class.
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
#include <QObject>
#include <QtTest>

class RecordFilterTest : public QObject
{
    Q_OBJECT

public:
    enum TestType
    {
        cat010SmrTgtRep,
        cat010SmrSrvMsg,
        cat010MlatTgtRep,
        cat010MlatSrvMsg,
        cat021AdsbTgtRep
    };

private slots:
    void initTestCase();
    void test_data();
    void test();

private:
    QVector<ModeS> excludedAddresses();
};

Q_DECLARE_METATYPE(RecordFilterTest::TestType);

void RecordFilterTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-recordfiltertest"));

    QSettings settings;
    settings.clear();

    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 109);
}

void RecordFilterTest::test_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QVector<Asterix::Record>>("recordsIn");
    QTest::addColumn<QVector<Asterix::Record>>("recordsOut");


    // TARGET REPORTS.

    // SMR:

    // Record 1: [ToD: 00:00:00], CAT010 SMR Target Report.
    Asterix::Record cat010SmrTgtRep1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC));

    cat010SmrTgtRep1.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010SmrTgtRep1.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("7")));

    cat010SmrTgtRep1.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("3")));

    cat010SmrTgtRep1.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("0.0")));

    // Record 2: [ToD: 00:00:05], CAT010 SMR Target Report.
    Asterix::Record cat010SmrTgtRep2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC));

    cat010SmrTgtRep2.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010SmrTgtRep2.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("7")));

    cat010SmrTgtRep2.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("3")));

    cat010SmrTgtRep2.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("5.0")));

    // Record 3: [ToD: 00:00:10], CAT010 SMR Target Report.
    Asterix::Record cat010SmrTgtRep3(quint8(10), QDateTime::fromMSecsSinceEpoch(10000, Qt::UTC));

    cat010SmrTgtRep3.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010SmrTgtRep3.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("7")));

    cat010SmrTgtRep3.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("3")));

    cat010SmrTgtRep3.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("10.0")));


    // MLAT:

    // Record 1: [ToD: 00:00:00], CAT010 MLAT Target Report (Excluded Address).
    Asterix::Record cat010MlatTgtRep1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC));

    cat010MlatTgtRep1.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010MlatTgtRep1.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("107")));

    cat010MlatTgtRep1.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("TOT"), QLatin1String("1")));

    cat010MlatTgtRep1.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("0.0")));

    cat010MlatTgtRep1.dataItems_[QLatin1String("I220")] =
        Asterix::DataItem(QLatin1String("I220"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("3443C4")));

    // Record 2: [ToD: 00:00:05], CAT010 MLAT Target Report (Excluded TOT).
    Asterix::Record cat010MlatTgtRep2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC));

    cat010MlatTgtRep2.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010MlatTgtRep2.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("107")));

    cat010MlatTgtRep2.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("TOT"), QLatin1String("2")));

    cat010MlatTgtRep2.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("5.0")));

    cat010MlatTgtRep2.dataItems_[QLatin1String("I220")] =
        Asterix::DataItem(QLatin1String("I220"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1")));

    // Record 3: [ToD: 00:00:10], CAT010 MLAT Target Report.
    Asterix::Record cat010MlatTgtRep3(quint8(10), QDateTime::fromMSecsSinceEpoch(10000, Qt::UTC));

    cat010MlatTgtRep3.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010MlatTgtRep3.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("107")));

    cat010MlatTgtRep3.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("TOT"), QLatin1String("1")));

    cat010MlatTgtRep3.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("10.0")));

    cat010MlatTgtRep3.dataItems_[QLatin1String("I220")] =
        Asterix::DataItem(QLatin1String("I220"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("FFFFF2")));


    // ADS-B:

    // Record 1: [ToD: 00:00:00], CAT021 ADS-B Target Report (Excluded Address).
    Asterix::Record cat021AdsbTgtRep1(quint8(21), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC));

    cat021AdsbTgtRep1.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("14"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("219")));

    cat021AdsbTgtRep1.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ECAT"), QLatin1String("5")));

    cat021AdsbTgtRep1.dataItems_[QLatin1String("I073")] =
        Asterix::DataItem(QLatin1String("I073"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("time_reception_position"), QLatin1String("0.0")));

    cat021AdsbTgtRep1.dataItems_[QLatin1String("I080")] =
        Asterix::DataItem(QLatin1String("I080"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("3443C5")));

    // Record 2: [ToD: 00:00:05], CAT021 ADS-B Target Report (Excluded ECAT).
    Asterix::Record cat021AdsbTgtRep2(quint8(21), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC));

    cat021AdsbTgtRep2.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("14"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("219")));

    cat021AdsbTgtRep2.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ECAT"), QLatin1String("21")));

    cat021AdsbTgtRep2.dataItems_[QLatin1String("I073")] =
        Asterix::DataItem(QLatin1String("I073"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("time_reception_position"), QLatin1String("5.0")));

    cat021AdsbTgtRep2.dataItems_[QLatin1String("I080")] =
        Asterix::DataItem(QLatin1String("I080"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1")));

    // Record 3: [ToD: 00:00:10], CAT021 ADS-B Target Report.
    Asterix::Record cat021AdsbTgtRep3(quint8(21), QDateTime::fromMSecsSinceEpoch(10000, Qt::UTC));

    cat021AdsbTgtRep3.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("14"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("219")));

    cat021AdsbTgtRep3.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ECAT"), QLatin1String("5")));

    cat021AdsbTgtRep3.dataItems_[QLatin1String("I073")] =
        Asterix::DataItem(QLatin1String("I073"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("time_reception_position"), QLatin1String("10.0")));

    cat021AdsbTgtRep3.dataItems_[QLatin1String("I080")] =
        Asterix::DataItem(QLatin1String("I080"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1")));


    // SERVICE MESSAGES:

    // SMR:

    // Record 1: [ToD: 00:00:00], CAT010 SMR Service Message.
    Asterix::Record cat010SmrSrvMsg1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC));

    cat010SmrSrvMsg1.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("3")));

    cat010SmrSrvMsg1.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("7")));

    cat010SmrSrvMsg1.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("0.0")));

    // Record 2: [ToD: 00:00:05], CAT010 SMR Service Message.
    Asterix::Record cat010SmrSrvMsg2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC));

    cat010SmrSrvMsg2.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("3")));

    cat010SmrSrvMsg2.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("7")));

    cat010SmrSrvMsg2.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("5.0")));

    // MLAT:

    // Record 1: [ToD: 00:00:00], CAT010 MLAT Service Message.
    Asterix::Record cat010MlatSrvMsg1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC));

    cat010MlatSrvMsg1.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("3")));

    cat010MlatSrvMsg1.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("107")));

    cat010MlatSrvMsg1.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("0.0")));

    // Record 2: [ToD: 00:00:05], CAT010 MLAT Service Message.
    Asterix::Record cat010MlatSrvMsg2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC));

    cat010MlatSrvMsg2.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("3")));

    cat010MlatSrvMsg2.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("107")));

    cat010MlatSrvMsg2.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("5.0")));


    QVector<Asterix::Record> smrTgtRepRecordsIn({cat010SmrTgtRep1, cat010SmrTgtRep2, cat010SmrTgtRep3});
    QVector<Asterix::Record> smrSrvMsgRecordsIn({cat010SmrSrvMsg1, cat010SmrSrvMsg2});
    QVector<Asterix::Record> mlatTgtRepRecordsIn({cat010MlatTgtRep1, cat010MlatTgtRep2, cat010MlatTgtRep3});
    QVector<Asterix::Record> mlatSrvMsgRecordsIn({cat010MlatSrvMsg1, cat010MlatSrvMsg2});
    QVector<Asterix::Record> adsbTgtRepRecordsIn({cat021AdsbTgtRep1, cat021AdsbTgtRep2, cat021AdsbTgtRep3});

    QVector<Asterix::Record> smrTgtRepRecordsOut({cat010SmrTgtRep1, cat010SmrTgtRep2, cat010SmrTgtRep3});
    QVector<Asterix::Record> smrSrvMsgRecordsOut({cat010SmrSrvMsg1, cat010SmrSrvMsg2});
    QVector<Asterix::Record> mlatTgtRepRecordsOut({cat010MlatTgtRep3});
    QVector<Asterix::Record> mlatSrvMsgRecordsOut({cat010MlatSrvMsg1, cat010MlatSrvMsg2});
    QVector<Asterix::Record> adsbTgtRepRecordsOut({cat021AdsbTgtRep3});

    QTest::newRow("CAT010 SMR TgtRep") << cat010SmrTgtRep << smrTgtRepRecordsIn << smrTgtRepRecordsOut;
    QTest::newRow("CAT010 SMR SrvMsg") << cat010SmrSrvMsg << smrSrvMsgRecordsIn << smrSrvMsgRecordsOut;
    QTest::newRow("CAT010 MLAT TgtRep") << cat010MlatTgtRep << mlatTgtRepRecordsIn << mlatTgtRepRecordsOut;
    QTest::newRow("CAT010 MLAT SrvMsg") << cat010MlatSrvMsg << mlatSrvMsgRecordsIn << mlatSrvMsgRecordsOut;
    QTest::newRow("CAT021 ADS-B TgtRep") << cat021AdsbTgtRep << adsbTgtRepRecordsIn << adsbTgtRepRecordsOut;
}

void RecordFilterTest::test()
{
    // Check that excluded addresses are filtered out correctly.
    // Check that counters are correctly initialized and updated.

    RecordFilter filter;

    QFile file(QFINDTESTDATA("excluded.txt"));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(TestType, testType);
    QFETCH(QVector<Asterix::Record>, recordsIn);
    QFETCH(QVector<Asterix::Record>, recordsOut);

    // Load list of excluded addresses from text file.
    filter.loadExcludedAddresses(&file);
    QCOMPARE(filter.excludedAddresses().size(), 5);

    // Check that counters are initialized to zero.
    RecordFilter::Counter counter;

    switch (testType)
    {
    case cat010SmrTgtRep:
        counter = filter.counter(RecordType(SystemType::Smr, MessageType::TargetReport));
        break;
    case cat010SmrSrvMsg:
        counter = filter.counter(RecordType(SystemType::Smr, MessageType::ServiceMessage));
        break;
    case cat010MlatTgtRep:
        counter = filter.counter(RecordType(SystemType::Mlat, MessageType::TargetReport));
        break;
    case cat010MlatSrvMsg:
        counter = filter.counter(RecordType(SystemType::Mlat, MessageType::ServiceMessage));
        break;
    case cat021AdsbTgtRep:
        counter = filter.counter(RecordType(SystemType::Adsb, MessageType::TargetReport));
        break;
    default:
        QSKIP("Test not implemented for this testType");
    }

    QCOMPARE(counter.in, 0u);
    QCOMPARE(counter.out, 0u);

    // Feed Records to the RecordFilter.
    for (Asterix::Record &rin : recordsIn)
    {
        filter.addData(rin);
    }

    if (testType == cat010SmrTgtRep)
    {
        // Check that counter is correctly updated.
        counter = filter.counter(RecordType(SystemType::Smr, MessageType::TargetReport));
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 3u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(filter.records().size(), 3u);

        int i = 0;
        while (filter.hasPendingRecords())
        {
            // Dequeue record from the filter queue.
            Asterix::Record rout = filter.record();

            // Check that the records at the other end are exactly as expected.
            QCOMPARE(rout, recordsOut.at(i));
            ++i;
        }
    }
    else if (testType == cat010MlatTgtRep)
    {
        // Check that counter is correctly updated.
        counter = filter.counter(RecordType(SystemType::Mlat, MessageType::TargetReport));
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 1u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(filter.records().size(), 1u);

        int i = 0;
        while (filter.hasPendingRecords())
        {
            // Dequeue record from the filter queue.
            Asterix::Record rout = filter.record();

            // Check that excluded addresses have been filtered out.
            ModeS tgtAddr = Asterix::getElementValue(rout, QLatin1String("I220"), QLatin1String("TAddr")).toUInt();
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");

            // Check that the records at the other end are exactly as expected.
            QCOMPARE(rout, recordsOut.at(i));
            ++i;
        }
    }
    else if (testType == cat021AdsbTgtRep)
    {
        // Check that counter is correctly updated.
        counter = filter.counter(RecordType(SystemType::Adsb, MessageType::TargetReport));
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 1u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(filter.records().size(), 1u);

        int i = 0;
        while (filter.hasPendingRecords())
        {
            // Dequeue record from the filter queue.
            Asterix::Record rout = filter.record();

            // Check that excluded addresses have been filtered out.
            ModeS tgtAddr = Asterix::getElementValue(rout, QLatin1String("I080"), QLatin1String("TAddr")).toUInt();
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");

            // Check that the records at the other end are exactly as expected.
            QCOMPARE(rout, recordsOut.at(i));
            ++i;
        }
    }
    else if (testType == cat010SmrSrvMsg)
    {
        // Check that counter is correctly updated.
        counter = filter.counter(RecordType(SystemType::Smr, MessageType::ServiceMessage));
        QCOMPARE(counter.in, 2u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(filter.records().size(), 2u);
    }
    else if (testType == cat010MlatSrvMsg)
    {
        // Check that counter is correctly updated.
        counter = filter.counter(RecordType(SystemType::Mlat, MessageType::ServiceMessage));
        QCOMPARE(counter.in, 2u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(filter.records().size(), 2u);
    }
}

QVector<ModeS> RecordFilterTest::excludedAddresses()
{
    return QVector<ModeS>({0x3443C3, 0x3443C4, 0x3443C5, 0x3443C2, 0x3443C6});
}

QTEST_GUILESS_MAIN(RecordFilterTest)
#include "recordfiltertest.moc"
