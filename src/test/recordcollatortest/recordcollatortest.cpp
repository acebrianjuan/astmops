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
    void test_data();
    void test();

private:
};

Q_DECLARE_METATYPE(RecordCollatorTest::TestType);

void RecordCollatorTest::test_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QVector<AsterixRecord>>("recordsIn");
    //QTest::addColumn<QVector<AsterixRecord>>("recordsOut");
    QTest::addColumn<int>("sizeOut");

    QTest::newRow("SMR") << Smr;
    QTest::newRow("MLAT") << Mlat;
    QTest::newRow("ADS-B") << Adsb;
    QTest::newRow("SrvMsg") << SrvMsg;
}

void RecordCollatorTest::test()
{
    // Check that excluded addresses are filtered out correctly.
    // Check that records are sorted in chronological order.

    RecordCollator collator;

    QFile file(QFINDTESTDATA("excluded.txt"));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(TestType, testType);
    QFETCH(QVector<AsterixRecord>, recordsIn);
    //QFETCH(QVector<AsterixRecord>, recordsOut);
    QFETCH(int, sizeOut);

    collator.loadExcludedAddresses(&file);

    for (AsterixRecord rin : recordsIn)
    {
        collator.processRecord(rin);
    }

    if (testType == Smr)
    {
        QCOMPARE(collator.smrQueue().size(), sizeOut);

        for (AsterixRecord rout : collator.smrQueue())
        {
            AsterixDataItem di010_220 = rout.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            QVERIFY(!collator.excludedAddresses().contains(tgtAddr));
        }
    }
    else if (testType == Mlat)
    {
        QCOMPARE(collator.mlatQueue().size(), sizeOut);

        for (AsterixRecord rout : collator.mlatQueue())
        {
            AsterixDataItem di010_220 = rout.m_dataItems[QLatin1String("I220")];
            IcaoAddr tgtAddr = di010_220.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            QVERIFY(!collator.excludedAddresses().contains(tgtAddr));
        }
    }
    else if (testType == Adsb)
    {
        QCOMPARE(collator.adsbQueue().size(), sizeOut);

        for (AsterixRecord rout : collator.adsbQueue())
        {
            AsterixDataItem di021_080 = rout.m_dataItems[QLatin1String("I080")];
            IcaoAddr tgtAddr = di021_080.m_fields[0].value<AsterixDataElement>().m_value.toUInt();

            QVERIFY(!collator.excludedAddresses().contains(tgtAddr));
        }
    }
    else if (testType == SrvMsg)
    {
        QCOMPARE(collator.srvMsgQueue().size(), sizeOut);
    }

    /*
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

QTEST_APPLESS_MAIN(RecordCollatorTest)
#include "recordcollatortest.moc"
