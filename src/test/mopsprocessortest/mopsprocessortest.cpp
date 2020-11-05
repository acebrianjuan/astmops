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
        cat010SmrTgtRep,
        cat010SmrSrvMsg,
        cat010MlatTgtRep,
        cat010MlatSrvMsg,
        cat021Adsb
    };

private slots:
    void initTestCase();

    void testMinimumFields_data();
    void testMinimumFields();

    void testUpdateRate_data();
    void testUpdateRate();

    void testProbDetection_data();
    void testProbDetection();

private:
};

Q_DECLARE_METATYPE(MopsProcessorTest::TestType);

void MopsProcessorTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-mopsprocessortest"));

    QSettings settings;
    settings.clear();

    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 109);
}

void MopsProcessorTest::testMinimumFields_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<double>("result");

    QTest::newRow("SMR TgtRep") << cat010SmrTgtRep << "cat010-smr-TgtRep.xml" << 0.5;
    QTest::newRow("SMR SrvMsg") << cat010SmrSrvMsg << "cat010-smr-SrvMsg.xml" << 0.5;

    QTest::newRow("MLAT TgtRep") << cat010MlatTgtRep << "cat010-mlat-TgtRep.xml" << 0.5;
    QTest::newRow("MLAT SrvMsg") << cat010MlatSrvMsg << "cat010-mlat-SrvMsg.xml" << 0.5;
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
    QFETCH(double, result);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    if (testType == cat010SmrTgtRep)
    {
        QCOMPARE(processor.ed116TgtRepMinimumFields(), result);
    }
    else if (testType == cat010SmrSrvMsg)
    {
        QCOMPARE(processor.ed116SrvMsgMinimumFields(), result);
    }
    else if (testType == cat010MlatTgtRep)
    {
        QCOMPARE(processor.ed117TgtRepMinimumFields(), result);
    }
    else if (testType == cat010MlatSrvMsg)
    {
        QCOMPARE(processor.ed117SrvMsgMinimumFields(), result);
    }
}

void MopsProcessorTest::testUpdateRate_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<double>("result");

    QTest::newRow("SMR TgtRep") << cat010SmrTgtRep << "cat010-smr-TgtRep-ur.xml" << 1.0;
    QTest::newRow("SMR SrvMsg") << cat010SmrSrvMsg << "cat010-smr-SrvMsg-ur.xml" << 1.0;

    QTest::newRow("MLAT TgtRep") << cat010MlatTgtRep << "cat010-mlat-TgtRep-ur.xml" << 1.0;
    QTest::newRow("MLAT SrvMsg") << cat010MlatSrvMsg << "cat010-mlat-SrvMsg-ur.xml" << 1.0;
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
    QFETCH(double, result);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    if (testType == cat010SmrTgtRep)
    {
        QCOMPARE(processor.ed116TgtRepUpdateRate(Aerodrome::Runway), result);
    }
    else if (testType == cat010SmrSrvMsg)
    {
        QCOMPARE(processor.ed116SrvMsgUpdateRate(), result);
    }
    else if (testType == cat010MlatTgtRep)
    {
        QCOMPARE(processor.ed117TgtRepUpdateRate(Aerodrome::Runway), result);
    }
    else if (testType == cat010MlatSrvMsg)
    {
        QCOMPARE(processor.ed117SrvMsgUpdateRate(), result);
    }
}

void MopsProcessorTest::testProbDetection_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<double>("result");

    QTest::newRow("SMR TgtRep") << cat010SmrTgtRep << "cat010-smr-TgtRep-ur.xml" << 1.0;
    QTest::newRow("MLAT TgtRep") << cat010MlatTgtRep << "cat010-mlat-TgtRep-ur.xml" << 1.0;
}

void MopsProcessorTest::testProbDetection()
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
    QFETCH(double, result);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());
    }

    if (testType == cat010SmrTgtRep)
    {
        QCOMPARE(processor.ed116TgtRepProbDetection(Aerodrome::Runway), result);
    }
    else if (testType == cat010MlatTgtRep)
    {
        QCOMPARE(processor.ed117TgtRepProbDetection(Aerodrome::Runway), result);
    }
}

QTEST_APPLESS_MAIN(MopsProcessorTest)
#include "mopsprocessortest.moc"
