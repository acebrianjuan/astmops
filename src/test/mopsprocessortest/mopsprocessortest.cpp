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
    QTest::addColumn<double>("ed116TgtRepResult");
    QTest::addColumn<double>("ed117TgtRepResult");
    QTest::addColumn<double>("srvMsgResult");

    QTest::newRow("SMR") << "ASTERIX_LEBL_SMR.xml" << 0.0 << 0.0 << 1.0;
    QTest::newRow("MLAT") << "ASTERIX_LEBL_MLAT.xml" << 0.0 << 0.0 << 1.0;
    //QTest::newRow("ADSB") << "ASTERIX_LEBL_ADSB.xml" << 0.0 << 0.0 << 0.0;
}

void MopsProcessorTest::testMinimumFields()
{
    AsterixXmlReader reader;
    MopsProcessor processor;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(double, ed116TgtRepResult);
    QFETCH(double, ed117TgtRepResult);
    QFETCH(double, srvMsgResult);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    if (fileName == QLatin1String("ASTERIX_LEBL_SMR.xml"))
    {
        QCOMPARE(processor.ed116TargetReportsMinimumFields(), ed116TgtRepResult);
    }
    else if (fileName == QLatin1String("ASTERIX_LEBL_MLAT.xml"))
    {
        QCOMPARE(processor.ed117TargetReportsMinimumFields(), ed117TgtRepResult);
    }

    QCOMPARE(processor.ed117ServiceMessagesMinimumFields(), srvMsgResult);
}

QTEST_APPLESS_MAIN(MopsProcessorTest)
#include "mopsprocessortest.moc"
