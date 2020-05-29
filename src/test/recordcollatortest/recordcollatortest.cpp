#include "recordcollator.h"
#include <QObject>
#include <QtTest>

class RecordCollatorTest : public QObject
{
    Q_OBJECT

public:
    enum TestType
    {
        Smr,
        Mlat,
        Adsb,
        SrvMsg
    };

private slots:
    void initTestCase();
    void test_data();
    void test();

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


    // SMR:

    // Record 1: [ToD: 00:00:00], CAT010 SMR Target Report (Excluded Vehicle).
    AsterixRecord cat010Smr1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC), SystemType::Smr);

    cat010Smr1.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("1"))));

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

    cat021Adsb1.m_dataItems[QLatin1String("I073")] =
        AsterixDataItem(QLatin1String("I073"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("time_reception_position"), QLatin1String("0.0"))));

    cat021Adsb1.m_dataItems[QLatin1String("I080")] =
        AsterixDataItem(QLatin1String("I080"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("3443C5"))));

    // Record 2: [ToD: 00:00:05], CAT021 ADS-B Target Report.
    AsterixRecord cat021Adsb2(quint8(21), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC), SystemType::Adsb);

    cat021Adsb2.m_dataItems[QLatin1String("I073")] =
        AsterixDataItem(QLatin1String("I073"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("time_reception_position"), QLatin1String("5.0"))));

    cat021Adsb2.m_dataItems[QLatin1String("I080")] =
        AsterixDataItem(QLatin1String("I080"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));

    // Record 3: [ToD: 00:00:10], CAT021 ADS-B Target Report.
    AsterixRecord cat021Adsb3(quint8(21), QDateTime::fromMSecsSinceEpoch(10000, Qt::UTC), SystemType::Adsb);

    cat021Adsb3.m_dataItems[QLatin1String("I073")] =
        AsterixDataItem(QLatin1String("I073"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("time_reception_position"), QLatin1String("10.0"))));

    cat021Adsb3.m_dataItems[QLatin1String("I080")] =
        AsterixDataItem(QLatin1String("I080"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("TAddr"), QLatin1String("FFFFF1"))));


    // Service Message:

    // Record 1: [ToD: 00:00:00], CAT010 Service Message.
    AsterixRecord cat010SrvMsg1(quint8(10), QDateTime::fromMSecsSinceEpoch(0, Qt::UTC), SystemType::Unknown);

    cat010SrvMsg1.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("3"))));

    cat010SrvMsg1.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("0.0"))));

    // Record 2: [ToD: 00:00:05], CAT010 Service Message.
    AsterixRecord cat010SrvMsg2(quint8(10), QDateTime::fromMSecsSinceEpoch(5000, Qt::UTC), SystemType::Unknown);

    cat010SrvMsg2.m_dataItems[QLatin1String("I000")] =
        AsterixDataItem(QLatin1String("I000"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("MsgTyp"), QLatin1String("3"))));

    cat010SrvMsg2.m_dataItems[QLatin1String("I140")] =
        AsterixDataItem(QLatin1String("I140"),
            QVariantList() << QVariant::fromValue(AsterixDataElement(QLatin1String("ToD"), QLatin1String("5.0"))));


    QVector<AsterixRecord> smrRecordsIn({cat010Smr1, cat010Smr3, cat010Smr2});
    QVector<AsterixRecord> mlatRecordsIn({cat010Mlat1, cat010Mlat3, cat010Mlat2});
    QVector<AsterixRecord> adsbRecordsIn({cat021Adsb1, cat021Adsb3, cat021Adsb2});
    QVector<AsterixRecord> srvMsgRecordsIn({cat010SrvMsg2, cat010SrvMsg1});

    //QVector<AsterixRecord> smrRecordsOut({cat010Smr2, cat010Smr3});
    //QVector<AsterixRecord> mlatRecordsOut({cat010Mlat2, cat010Mlat3});
    //QVector<AsterixRecord> adsbRecordsOut({cat010Adsb2, cat010Adsb3});

    QTest::newRow("SMR") << Smr << smrRecordsIn << 2;
    QTest::newRow("MLAT") << Mlat << mlatRecordsIn << 2;
    QTest::newRow("ADS-B") << Adsb << adsbRecordsIn << 2;
    QTest::newRow("SrvMsg") << SrvMsg << srvMsgRecordsIn << 2;
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
    if (testType == Smr)
    {
        counter = collator.smrCounter();
    }
    else if (testType == Mlat)
    {
        counter = collator.mlatCounter();
    }
    else if (testType == Adsb)
    {
        counter = collator.adsbCounter();
    }
    else if (testType == SrvMsg)
    {
        counter = collator.srvMsgCounter();
    }

    QCOMPARE(counter.in, 0u);
    QCOMPARE(counter.out, 0u);

    // Feed Records to the RecordCollator.
    for (AsterixRecord rin : recordsIn)
    {
        collator.processRecord(rin);
    }

    QVector<quint64> msecs;
    msecs.clear();
    if (testType == Smr)
    {
        // Check that counter is correctly updated.
        counter = collator.smrCounter();
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.smrQueue().size(), sizeOut);

        for (AsterixRecord rout : collator.smrQueue())
        {
            AsterixDataItem di010_220 = rout.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            // Check that excluded addresses have been filtered out.
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }
    else if (testType == Mlat)
    {
        // Check that counter is correctly updated.
        counter = collator.mlatCounter();
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.mlatQueue().size(), sizeOut);

        for (AsterixRecord rout : collator.mlatQueue())
        {
            AsterixDataItem di010_220 = rout.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            // Check that excluded addresses have been filtered out.
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }
    else if (testType == Adsb)
    {
        // Check that counter is correctly updated.
        counter = collator.adsbCounter();
        QCOMPARE(counter.in, 3u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.adsbQueue().size(), sizeOut);

        for (AsterixRecord rout : collator.adsbQueue())
        {
            AsterixDataItem di021_080 = rout.m_dataItems[QLatin1String("I080")];
            IcaoAddr tgtAddr = di021_080.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            // Check that excluded addresses have been filtered out.
            QVERIFY2(!excludedAddresses().contains(tgtAddr), "Excluded addresses have not been filtered out.");
            msecs << rout.m_dateTime.toMSecsSinceEpoch();
        }
    }
    else if (testType == SrvMsg)
    {
        // Check that counter is correctly updated.
        counter = collator.srvMsgCounter();
        QCOMPARE(counter.in, 2u);
        QCOMPARE(counter.out, 2u);

        // Check that number of Records at the other end matches the expected value.
        QCOMPARE(collator.srvMsgQueue().size(), sizeOut);

        for (AsterixRecord rout : collator.srvMsgQueue())
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
