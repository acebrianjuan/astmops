/*!
 * \file recordcollatortest.cpp
 * \brief Implements unit tests for the RecordCollator class.
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

#include "recordcollator.h"
#include <QObject>
#include <QtTest>

class RecordCollatorTest : public QObject
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

private:
    QVector<IcaoAddr> excludedAddresses();
};

Q_DECLARE_METATYPE(RecordCollatorTest::TestType);

void RecordCollatorTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-recordcollatortest"));

    QSettings settings;
    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADS-B.SIC"), 109);
}

void RecordCollatorTest::test_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QVector<AsterixRecord>>("recordsIn");
    //QTest::addColumn<QVector<AsterixRecord>>("recordsOut");
    QTest::addColumn<int>("sizeOut");


    // TARGET REPORTS.

    // SMR:

    // Record 1: [ToD: 00:00:00], CAT010 SMR Target Report (Excluded Vehicle).
    AsterixRecord cat010Smr1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC), SystemType::Smr);

    cat010Smr1.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("1"))));

    cat010Smr1.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("7"))));

    cat010Smr1.m_dataItems[QLatin1String("I020")] =
        AsterixDataItem(QLatin1String("I020"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TYP"), QLatin1String("3"))));

    cat010Smr1.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("0.0"))));

    cat010Smr1.m_dataItems[QLatin1String("I220")] =
        AsterixDataItem(QLatin1String("I220"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("3443C3"))));

    // Record 2: [ToD: 00:00:05], CAT010 SMR Target Report.
    AsterixRecord cat010Smr2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC), SystemType::Smr);

    cat010Smr2.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("1"))));

    cat010Smr2.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("7"))));

    cat010Smr2.m_dataItems[QLatin1String("I020")] =
        AsterixDataItem(QLatin1String("I020"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TYP"), QLatin1String("3"))));

    cat010Smr2.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("5.0"))));

    cat010Smr2.m_dataItems[QLatin1String("I220")] =
        AsterixDataItem(QLatin1String("I220"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));

    // Record 3: [ToD: 00:00:10], CAT010 SMR Target Report.
    AsterixRecord cat010Smr3(quint8(10), QDateTime::fromMSecsSinceEpoch(10000, Qt::UTC), SystemType::Smr);

    cat010Smr3.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("1"))));

    cat010Smr3.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("7"))));

    cat010Smr3.m_dataItems[QLatin1String("I020")] =
        AsterixDataItem(QLatin1String("I020"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TYP"), QLatin1String("3"))));

    cat010Smr3.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("10.0"))));

    cat010Smr3.m_dataItems[QLatin1String("I220")] =
        AsterixDataItem(QLatin1String("I220"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));


    // MLAT:

    // Record 1: [ToD: 00:00:00], CAT010 MLAT Target Report (Excluded Vehicle).
    AsterixRecord cat010Mlat1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC), SystemType::Mlat);

    cat010Mlat1.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("1"))));

    cat010Mlat1.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("107"))));

    cat010Mlat1.m_dataItems[QLatin1String("I020")] =
        AsterixDataItem(QLatin1String("I020"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TYP"), QLatin1String("1"))));

    cat010Mlat1.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("0.0"))));

    cat010Mlat1.m_dataItems[QLatin1String("I220")] =
        AsterixDataItem(QLatin1String("I220"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("3443C4"))));

    // Record 2: [ToD: 00:00:05], CAT010 MLAT Target Report.
    AsterixRecord cat010Mlat2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC), SystemType::Mlat);

    cat010Mlat2.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("1"))));

    cat010Mlat2.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("107"))));

    cat010Mlat2.m_dataItems[QLatin1String("I020")] =
        AsterixDataItem(QLatin1String("I020"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TYP"), QLatin1String("1"))));

    cat010Mlat2.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("5.0"))));

    cat010Mlat2.m_dataItems[QLatin1String("I220")] =
        AsterixDataItem(QLatin1String("I220"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));

    // Record 3: [ToD: 00:00:10], CAT010 MLAT Target Report.
    AsterixRecord cat010Mlat3(quint8(10), QDateTime::fromMSecsSinceEpoch(10000, Qt::UTC), SystemType::Mlat);

    cat010Mlat3.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("1"))));

    cat010Mlat3.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("107"))));

    cat010Mlat3.m_dataItems[QLatin1String("I020")] =
        AsterixDataItem(QLatin1String("I020"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TYP"), QLatin1String("1"))));

    cat010Mlat3.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("10.0"))));

    cat010Mlat3.m_dataItems[QLatin1String("I220")] =
        AsterixDataItem(QLatin1String("I220"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));


    // ADS-B:

    // Record 1: [ToD: 00:00:00], CAT021 ADS-B Target Report (Excluded Vehicle).
    AsterixRecord cat021Adsb1(quint8(21), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC), SystemType::Adsb);

    cat021Adsb1.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("14")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("219"))));

    cat021Adsb1.m_dataItems[QLatin1String("I073")] =
        AsterixDataItem(QLatin1String("I073"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("time_reception_position"), QLatin1String("0.0"))));

    cat021Adsb1.m_dataItems[QLatin1String("I080")] =
        AsterixDataItem(QLatin1String("I080"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("3443C5"))));

    // Record 2: [ToD: 00:00:05], CAT021 ADS-B Target Report.
    AsterixRecord cat021Adsb2(quint8(21), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC), SystemType::Adsb);

    cat021Adsb2.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("14")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("219"))));

    cat021Adsb2.m_dataItems[QLatin1String("I073")] =
        AsterixDataItem(QLatin1String("I073"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("time_reception_position"), QLatin1String("5.0"))));

    cat021Adsb2.m_dataItems[QLatin1String("I080")] =
        AsterixDataItem(QLatin1String("I080"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));

    // Record 3: [ToD: 00:00:10], CAT021 ADS-B Target Report.
    AsterixRecord cat021Adsb3(quint8(21), QDateTime::fromMSecsSinceEpoch(10000, Qt::UTC), SystemType::Adsb);

    cat021Adsb3.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("14")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("219"))));

    cat021Adsb3.m_dataItems[QLatin1String("I073")] =
        AsterixDataItem(QLatin1String("I073"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("time_reception_position"), QLatin1String("10.0"))));

    cat021Adsb3.m_dataItems[QLatin1String("I080")] =
        AsterixDataItem(QLatin1String("I080"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));


    // SERVICE MESSAGES:

    // SMR:

    // Record 1: [ToD: 00:00:00], CAT010 SMR Service Message.
    AsterixRecord cat010SmrSrvMsg1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC), SystemType::Unknown);

    cat010SmrSrvMsg1.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("3"))));

    cat010SmrSrvMsg1.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("7"))));

    cat010SmrSrvMsg1.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("0.0"))));

    // Record 2: [ToD: 00:00:05], CAT010 SMR Service Message.
    AsterixRecord cat010SmrSrvMsg2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC), SystemType::Unknown);

    cat010SmrSrvMsg2.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("3"))));

    cat010SmrSrvMsg2.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("7"))));

    cat010SmrSrvMsg2.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("5.0"))));

    // MLAT:

    // Record 1: [ToD: 00:00:00], CAT010 MLAT Service Message.
    AsterixRecord cat010MlatSrvMsg1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC), SystemType::Unknown);

    cat010MlatSrvMsg1.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("3"))));

    cat010MlatSrvMsg1.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("107"))));

    cat010MlatSrvMsg1.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("0.0"))));

    // Record 2: [ToD: 00:00:05], CAT010 MLAT Service Message.
    AsterixRecord cat010MlatSrvMsg2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC), SystemType::Unknown);

    cat010MlatSrvMsg2.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("3"))));

    cat010MlatSrvMsg2.m_dataItems[QLatin1String("I010")] =
        AsterixDataItem(QLatin1String("I010"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("SAC"), QLatin1String("0")))
                           << QVariant::fromValue(AsterixDataElement(QLatin1String("SIC"), QLatin1String("107"))));

    cat010MlatSrvMsg2.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("5.0"))));


    QVector<AsterixRecord> smrTgtRepRecordsIn({cat010Smr1, cat010Smr3, cat010Smr2});
    QVector<AsterixRecord> smrSrvMsgRecordsIn({cat010SmrSrvMsg2, cat010SmrSrvMsg1});
    QVector<AsterixRecord> mlatTgtRepRecordsIn({cat010Mlat1, cat010Mlat3, cat010Mlat2});
    QVector<AsterixRecord> mlatSrvMsgRecordsIn({cat010MlatSrvMsg2, cat010MlatSrvMsg1});
    QVector<AsterixRecord> adsbRecordsIn({cat021Adsb1, cat021Adsb3, cat021Adsb2});

    //QVector<AsterixRecord> smrRecordsOut({cat010Smr2, cat010Smr3});
    //QVector<AsterixRecord> mlatRecordsOut({cat010Mlat2, cat010Mlat3});
    //QVector<AsterixRecord> adsbRecordsOut({cat010Adsb2, cat010Adsb3});

    QTest::newRow("CAT010 SMR TgtRep") << cat010SmrTgtRep << smrTgtRepRecordsIn << 2;
    QTest::newRow("CAT010 SMR SrvMsg") << cat010SmrSrvMsg << smrSrvMsgRecordsIn << 2;
    QTest::newRow("CAT010 MLAT TgtRep") << cat010MlatTgtRep << mlatTgtRepRecordsIn << 2;
    QTest::newRow("CAT010 MLAT SrvMsg") << cat010MlatSrvMsg << mlatSrvMsgRecordsIn << 2;
    QTest::newRow("CAT021 ADS-B") << cat021Adsb << adsbRecordsIn << 2;
}

void RecordCollatorTest::test()
{
    // Check that excluded addresses are filtered out correctly.
    // Check that records are sorted in chronological order.
    // Check that counters are correctly initialized and updated.

    RecordCollator collator;

    QFile file(QFINDTESTDATA("excluded.txt"));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(TestType, testType);
    QFETCH(QVector<AsterixRecord>, recordsIn);
    //QFETCH(QVector<AsterixRecord>, recordsOut);
    QFETCH(int, sizeOut);

    // Load list of excluded addresses from text file.
    collator.loadExcludedAddresses(&file);
    QCOMPARE(collator.excludedAddresses().size(), 5);

    // Check that counters are initialized to zero.
    RecordCollator::Counter counter;

    switch (testType)
    {
    case cat010SmrTgtRep:
        counter = collator.smrTgtRepCounter();
        break;
    case cat010SmrSrvMsg:
        counter = collator.smrSrvMsgCounter();
        break;
    case cat010MlatTgtRep:
        counter = collator.mlatTgtRepCounter();
        break;
    case cat010MlatSrvMsg:
        counter = collator.mlatSrvMsgCounter();
        break;
    case cat021Adsb:
        counter = collator.adsbTgtRepCounter();
        break;
    default:
        QSKIP("Test not implemented for this testType");
    }

    QCOMPARE(counter.in, 0u);
    QCOMPARE(counter.out, 0u);

    // Feed Records to the RecordCollator.
    for (const AsterixRecord &rin : recordsIn)
    {
        collator.processRecord(rin);
    }

    QVector<quint64> msecs;
    msecs.clear();
    if (testType == cat010SmrTgtRep)
    {
        // Check that counter is correctly updated.
        counter = collator.smrTgtRepCounter();
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.smrTgtRepQueue().size(), sizeOut);

        for (const AsterixRecord &rout : collator.smrTgtRepQueue())
        {
            AsterixDataItem di010_220 = rout.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            // Check that excluded addresses have been filtered out.
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }
    else if (testType == cat010MlatTgtRep)
    {
        // Check that counter is correctly updated.
        counter = collator.mlatTgtRepCounter();
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.mlatTgtRepQueue().size(), sizeOut);

        for (const AsterixRecord &rout : collator.mlatTgtRepQueue())
        {
            AsterixDataItem di010_220 = rout.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            // Check that excluded addresses have been filtered out.
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }
    else if (testType == cat021Adsb)
    {
        // Check that counter is correctly updated.
        counter = collator.adsbTgtRepCounter();
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.adsbTgtRepQueue().size(), sizeOut);

        for (const AsterixRecord &rout : collator.adsbTgtRepQueue())
        {
            AsterixDataItem di021_080 = rout.m_dataItems[QLatin1String("I080")];
            IcaoAddr tgtAddr = di021_080.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            // Check that excluded addresses have been filtered out.
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }
    else if (testType == cat010SmrSrvMsg)
    {
        // Check that counter is correctly updated.
        counter = collator.smrSrvMsgCounter();
        QCOMPARE(counter.in, 2u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.smrSrvMsgQueue().size(), sizeOut);

        for (const AsterixRecord &rout : collator.smrSrvMsgQueue())
        {
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }
    else if (testType == cat010MlatSrvMsg)
    {
        // Check that counter is correctly updated.
        counter = collator.mlatSrvMsgCounter();
        QCOMPARE(counter.in, 2u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.mlatSrvMsgQueue().size(), sizeOut);

        for (const AsterixRecord &rout : collator.mlatSrvMsgQueue())
        {
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }

    // Check that Records are sorted in chronological order.
    auto it = std::adjacent_find(msecs.begin(), msecs.end(), std::greater<int>());
    QVERIFY2(it == msecs.end(), "Records are not sorted in chronological order.");

    /*
    // Check that the Records at the other end are the ones expected.
    for (AsterixRecord rout : recordsOut)
    {
        if (testType == Smr)
        {
            QCOMPARE(collator.smrQueue().dequeue(), rout);
        }
        else if (testType == Mlat)
        {
            QCOMPARE(collator.mlatQueue().dequeue(), rout);
        }
        else if (testType == Adsb)
        {
            QCOMPARE(collator.adsbQueue().dequeue(), rout);
        }
        else if (testType == SrvMsg)
        {
            QCOMPARE(collator.srvMsgQueue().dequeue(), rout);
        }
    }
    */
}

QVector<IcaoAddr> RecordCollatorTest::excludedAddresses()
{
    return QVector<IcaoAddr>({0x3443C3, 0x3443C4, 0x3443C5, 0x3443C2, 0x3443C6});
}

QTEST_GUILESS_MAIN(RecordCollatorTest)
#include "recordcollatortest.moc"
