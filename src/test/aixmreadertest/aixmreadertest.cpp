/*!
 * \file aixmreadertest.cpp
 * \brief Implements unit tests for the AixmReader class.
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

#include "aixmreader.h"
#include <QObject>
#include <QtTest>

// TODO: Verify coordinates.

class AixmReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();
};

void AixmReaderTest::test_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("runwayElements");
    QTest::addColumn<int>("taxiwayElements");
    QTest::addColumn<int>("apronElements");

    QTest::newRow("KCLT") << "AIXM51_KCLT_272B.xml" << 10 << 241 << 34;
    QTest::newRow("KORD") << "AIXM51_KORD_Runways.xml" << 25 << 0 << 0;
}

void AixmReaderTest::test()
{
    AixmReader reader;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(int, runwayElements);
    QFETCH(int, taxiwayElements);
    QFETCH(int, apronElements);

    reader.read(&file);
    //Aerodrome aerodrome = reader.aerodrome();

    QCOMPARE(reader.m_runwayElements.size(), runwayElements);
    QCOMPARE(reader.m_taxiwayElements.size(), taxiwayElements);
    QCOMPARE(reader.m_apronElements.size(), apronElements);
}

QTEST_APPLESS_MAIN(AixmReaderTest)
#include "aixmreadertest.moc"
