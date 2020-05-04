#include "mopsprocessor.h"
#include "asterix.h"
#include "asterixxmlreader.h"
#include <QObject>
#include <QtTest>

class MopsProcessorTest : public QObject
{
    Q_OBJECT

private slots:
    //void test();

    void testMinimumFields_data();
    void testMinimumFields();

    void testUpdateRate_data();
    void testUpdateRate();

private:
};

/*
void MopsProcessorTest::test()
{
    Aerodrome charlotte;

    auto runwayCallback = [](const QPointF& point) {
        Q_UNUSED(point);
        return Aerodrome::Runway;
    };

    auto airborneCallback = [](const QPointF& point) {
        Q_UNUSED(point);
        return Aerodrome::Airborne;
    };

    Aerodrome barcelona;
    auto barcelonaCallback = [&barcelona](const QPointF& point) {
        return barcelona.locatePoint(point);
    };

    MopsProcessor processor;
    processor.setLocatePointCallback(barcelonaCallback);

    processor.setLocatePointCallback(runwayCallback);
    // QCOMPARE
}
*/

void MopsProcessorTest::testMinimumFields_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<double>("tgtRepResult");
    QTest::addColumn<double>("srvMsgResult");

    QTest::newRow("SMR") << "ASTERIX_SMR.xml" << 0.5 << 0.5;
    QTest::newRow("MLAT") << "ASTERIX_MLAT.xml" << 0.5 << 0.5;
}

void MopsProcessorTest::testMinimumFields()
{
    AsterixXmlReader reader;
    MopsProcessor processor;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(double, tgtRepResult);
    QFETCH(double, srvMsgResult);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    if (fileName == QLatin1String("ASTERIX_SMR.xml"))
    {
        QCOMPARE(processor.ed116TargetReportsMinimumFields(), tgtRepResult);
    }
    else if (fileName == QLatin1String("ASTERIX_MLAT.xml"))
    {
        QCOMPARE(processor.ed117TargetReportsMinimumFields(), tgtRepResult);
    }

    QCOMPARE(processor.ed117ServiceMessagesMinimumFields(), srvMsgResult);
}

void MopsProcessorTest::testUpdateRate_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<double>("srvMsgResult");

    QTest::newRow("SMR") << "ASTERIX_SMR.xml" << 1.0;
    QTest::newRow("MLAT") << "ASTERIX_MLAT.xml" << 1.0;
}

void MopsProcessorTest::testUpdateRate()
{
    AsterixXmlReader reader;
    MopsProcessor processor;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(double, srvMsgResult);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    QCOMPARE(processor.serviceMessagesUpdateRate(), srvMsgResult);
}

QTEST_APPLESS_MAIN(MopsProcessorTest)
#include "mopsprocessortest.moc"
