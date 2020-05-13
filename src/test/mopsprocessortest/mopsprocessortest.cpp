#include "mopsprocessor.h"
#include "asterix.h"
#include "asterixxmlreader.h"
#include <QObject>
#include <QtTest>

class MopsProcessorTest : public QObject
{
    Q_OBJECT

private slots:
    void testMinimumFields_data();
    void testMinimumFields();

    void testUpdateRate_data();
    void testUpdateRate();

private:
};

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

    auto runwayCallback = [](const QPointF& point) {
        Q_UNUSED(point);
        return Aerodrome::Runway;
    };

    processor.setLocatePointCallback(runwayCallback);

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

    QString currentDataTag = QString::fromLatin1(QTest::currentDataTag());
    if (currentDataTag == QLatin1String("SMR"))
    {
        QCOMPARE(processor.ed116TargetReportsMinimumFields(), tgtRepResult);
    }
    else if (currentDataTag == QLatin1String("MLAT"))
    {
        QCOMPARE(processor.ed117TargetReportsMinimumFields(), tgtRepResult);
    }

    QCOMPARE(processor.serviceMessagesMinimumFields(), srvMsgResult);
}

void MopsProcessorTest::testUpdateRate_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<double>("tgtRepResult");
    QTest::addColumn<double>("srvMsgResult");

    QTest::newRow("SMR") << "ASTERIX_SMR.xml" << 1.0 << 1.0;
    QTest::newRow("MLAT") << "ASTERIX_MLAT.xml" << 1.0 << 1.0;
}

void MopsProcessorTest::testUpdateRate()
{
    AsterixXmlReader reader;
    MopsProcessor processor;

    auto runwayCallback = [](const QPointF& point) {
        Q_UNUSED(point);
        return Aerodrome::Runway;
    };

    processor.setLocatePointCallback(runwayCallback);

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

    QString currentDataTag = QString::fromLatin1(QTest::currentDataTag());
    if (currentDataTag == QLatin1String("SMR"))
    {
        QCOMPARE(processor.ed116TargetReportsUpdateRate(), tgtRepResult);
    }
    else if (currentDataTag == QLatin1String("MLAT"))
    {
        QCOMPARE(processor.ed117TargetReportsUpdateRate(), tgtRepResult);
    }

    QCOMPARE(processor.serviceMessagesUpdateRate(), srvMsgResult);
}

QTEST_APPLESS_MAIN(MopsProcessorTest)
#include "mopsprocessortest.moc"
