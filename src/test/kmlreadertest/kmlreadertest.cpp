/*!
 * \file kmlreadertest.cpp
 * \brief Implements unit tests for the KmlReader class.
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

#include "kmlreader.h"
#include <QObject>
#include <QtTest>

// TODO: Verify coordinates.

class KmlReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();
};

void KmlReaderTest::test_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("runwayElements");
    QTest::addColumn<int>("taxiwayElements");
    QTest::addColumn<int>("apronElements");

    QTest::newRow("LEBL") << "lebl-insignia.kml" << 48 << 480 << 17;
}

void KmlReaderTest::test()
{
    KmlReader reader;

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

QTEST_APPLESS_MAIN(KmlReaderTest)
#include "kmlreadertest.moc"
