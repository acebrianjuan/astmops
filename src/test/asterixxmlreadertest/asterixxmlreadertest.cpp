/*!
 * \file asterixxmlreadertest.cpp
 * \brief Implements unit tests for the AsterixXmlReader class.
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
    QTest::addColumn<QVector<int>>("items");

    QTest::newRow("CAT010") << "cat010.xml" << (QVector<int>() << 12 << 12);
    QTest::newRow("CAT010 (Empty Data Items)") << "cat010-empty.xml" << (QVector<int>() << 12 << 12);
    QTest::newRow("CAT010 (Incomplete chunk)") << "cat010-incomplete.xml" << (QVector<int>() << 12 << 2);
    QTest::newRow("CAT010 (Missing attributes)") << "cat010-missing-attributes.xml" << QVector<int>();
}

void AsterixXmlReaderTest::test()
{
    AsterixXmlReader reader;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(QVector<int>, items);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    QVector<AsterixRecord> records;
    while (reader.hasPendingRecords())
    {
        records.append(reader.record());
    }

    QCOMPARE(records.size(), items.size());

    for (int i = 0; i < records.size(); ++i)
    {
        QCOMPARE(records.at(i).m_dataItems.size(), items.at(i));
    }
}

QTEST_APPLESS_MAIN(AsterixXmlReaderTest)
#include "asterixxmlreadertest.moc"
