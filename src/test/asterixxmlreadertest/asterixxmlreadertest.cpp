#include "asterixxmlreader.h"
#include <QObject>
#include <QtTest>

class AsterixXmlReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();
};

void AsterixXmlReaderTest::test_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("nItems");

    QTest::newRow("CAT010") << "cat010.xml" << 12;
    QTest::newRow("CAT010 (Empty Data Items)") << "cat010-empty.xml" << 12;
    QTest::newRow("CAT010 (Incomplete Chunk)") << "cat010-incomplete.xml" << 12;
}

void AsterixXmlReaderTest::test()
{
    AsterixXmlReader reader;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(int, nItems);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    QVERIFY(reader.hasPendingRecords() == true);

    AsterixRecord record = reader.record();

    QVERIFY(reader.hasPendingRecords() == false);

    QCOMPARE(record.dataItems.size(), nItems);
}

QTEST_APPLESS_MAIN(AsterixXmlReaderTest)
#include "asterixxmlreadertest.moc"
