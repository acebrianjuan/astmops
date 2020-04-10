#include "mopsprocessor.h"
#include "asterix.h"
#include <QObject>
#include <QtTest>

class MopsProcessorTest : public QObject
{
    Q_OBJECT

private slots:
    void ed116TargetReportsTest_data();
    void ed116TargetReportsTest();

    void ed117TargetReportsTest_data();
    void ed117TargetReportsTest();

private:
    AsterixRecord makeRecord(const quint8& cat, const QStringList& di);
};

void MopsProcessorTest::ed116TargetReportsTest_data()
{
    QStringList itemsList;
    itemsList << QLatin1String("I000")
              << QLatin1String("I010")
              << QLatin1String("I020")
              << QLatin1String("I140")
              << QLatin1String("I041")
              << QLatin1String("I042")
              << QLatin1String("I270")
              << QLatin1String("I550");

    AsterixRecord recordPass = makeRecord(10, itemsList);

    AsterixRecord recordFail = recordPass;
    recordFail.dataItems.removeLast();

    QTest::addColumn<AsterixRecord>("record");
    QTest::addColumn<bool>("passed");

    QTest::newRow("All DataItems") << recordPass << true;
    QTest::newRow("Missing DataItem") << recordFail << false;
}

void MopsProcessorTest::ed116TargetReportsTest()
{
    QFETCH(AsterixRecord, record);
    QFETCH(bool, passed);

    MopsProcessor mp;
    QCOMPARE(mp.ed116TargetReports(record), passed);
}

void MopsProcessorTest::ed117TargetReportsTest_data()
{
    QStringList itemsList;
    itemsList << QLatin1String("I000")
              << QLatin1String("I010")
              << QLatin1String("I020")
              << QLatin1String("I041")
              << QLatin1String("I042")
              << QLatin1String("I060")
              << QLatin1String("I091")
              << QLatin1String("I140")
              << QLatin1String("I161")
              << QLatin1String("I170")
              << QLatin1String("I220")
              << QLatin1String("I500");

    AsterixRecord recordPass = makeRecord(10, itemsList);

    AsterixRecord recordFail = recordPass;
    recordFail.dataItems.removeLast();

    QTest::addColumn<AsterixRecord>("record");
    QTest::addColumn<bool>("passed");

    QTest::newRow("All DataItems") << recordPass << true;
    QTest::newRow("Missing DataItem") << recordFail << false;
}

void MopsProcessorTest::ed117TargetReportsTest()
{
    QFETCH(AsterixRecord, record);
    QFETCH(bool, passed);

    MopsProcessor mp;
    QCOMPARE(mp.ed117TargetReports(record), passed);
}

AsterixRecord MopsProcessorTest::makeRecord(const quint8& cat, const QStringList& itemsList)
{
    AsterixRecord record;
    record.cat = cat;

    for (const QString& it : itemsList)
    {
        AsterixDataItem di;
        di.name = it;

        record.dataItems.append(QVariant::fromValue(di));
    }

    return record;
}

QTEST_APPLESS_MAIN(MopsProcessorTest)
#include "mopsprocessortest.moc"
