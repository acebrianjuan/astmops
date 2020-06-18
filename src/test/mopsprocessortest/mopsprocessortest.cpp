/*!
 * \file mopsprocessortest.cpp
 * \brief Implements unit tests for the MopsProcessor class.
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

#include "mopsprocessor.h"
#include "asterix.h"
#include "asterixxmlreader.h"
#include <QObject>
#include <QtTest>

class MopsProcessorTest : public QObject
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
    void testMinimumFields_data();
    void testMinimumFields();

    void testUpdateRate_data();
    void testUpdateRate();

private:
};

Q_DECLARE_METATYPE(MopsProcessorTest::TestType);

void MopsProcessorTest::testMinimumFields_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<double>("tgtRepResult");
    QTest::addColumn<double>("srvMsgResult");

    QTest::newRow("SMR") << Smr << "ASTERIX_SMR.xml" << 0.5 << 0.5;
    QTest::newRow("MLAT") << Mlat << "ASTERIX_MLAT.xml" << 0.5 << 0.5;
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

    QFETCH(TestType, testType);
    QFETCH(double, tgtRepResult);
    QFETCH(double, srvMsgResult);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    if (testType == Smr)
    {
        QCOMPARE(processor.ed116TargetReportsMinimumFields(), tgtRepResult);
    }
    else if (testType == Mlat)
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

    QFETCH(TestType, testType);
    QFETCH(double, tgtRepResult);
    QFETCH(double, srvMsgResult);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    if (testType == Smr)
    {
        QCOMPARE(processor.ed116TargetReportsUpdateRate(), tgtRepResult);
    }
    else if (testType == Mlat)
    {
        QCOMPARE(processor.ed117TargetReportsUpdateRate(), tgtRepResult);
    }

    QCOMPARE(processor.serviceMessagesUpdateRate(), srvMsgResult);
}

QTEST_APPLESS_MAIN(MopsProcessorTest)
#include "mopsprocessortest.moc"
