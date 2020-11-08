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
    QTest::addColumn<QVector<double>>("result");

    QVector<double> expectedVals = QVector<double>() << 1.0 << 0.5;

    QTest::newRow("SMR TgtRep") << cat010SmrTgtRep
                                << "CAT010_SMR_TgtRep_MF.xml"
                                << expectedVals;

    QTest::newRow("SMR SrvMsg") << cat010SmrSrvMsg
                                << "CAT010_SMR_SrvMsg_MF.xml"
                                << expectedVals;

    QTest::newRow("MLAT TgtRep") << cat010MlatTgtRep
                                 << "CAT010_MLAT_TgtRep_MF.xml"
                                 << expectedVals;

    QTest::newRow("MLAT SrvMsg") << cat010MlatSrvMsg
                                 << "CAT010_MLAT_SrvMsg_MF.xml"
                                 << expectedVals;
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
    QFETCH(QVector<double>, result);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    int i = 0;
    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());

        double expectedVal = result.at(i);
        double actualVal = 0.0;

        if (testType == cat010SmrTgtRep)
        {
            actualVal = processor.ed116TgtRepMinimumFields();
        }
        else if (testType == cat010SmrSrvMsg)
        {
            actualVal = processor.ed116SrvMsgMinimumFields();
        }
        else if (testType == cat010MlatTgtRep)
        {
            actualVal = processor.ed117TgtRepMinimumFields();
        }
        else if (testType == cat010MlatSrvMsg)
        {
            actualVal = processor.ed117SrvMsgMinimumFields();
        }

        if (qIsNaN(expectedVal))
        {
            QVERIFY(qIsNaN(actualVal));
        }
        else
        {
            QCOMPARE(actualVal, expectedVal);
        }

        ++i;
    }
}

void MopsProcessorTest::testUpdateRate_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QVector<double>>("result");

    QVector<double> expectedVals1 = QVector<double>() << 1.0 << 1.0 << 1.0 << 1.0 << 1.0
                                                      << 1.0 << 1.0 << 1.0 << 1.0 << 1.0;

    QVector<double> expectedVals2 = QVector<double>() << 1.0 << 2.0 / 3.0 << 3.0 / 6.0 << 4.0 / 10.0;

    QTest::newRow("SMR TgtRep") << cat010SmrTgtRep
                                << "CAT010_SMR_TgtRep_UR_case_A.xml"
                                << expectedVals1;

    QTest::newRow("SMR TgtRep") << cat010SmrTgtRep
                                << "CAT010_SMR_TgtRep_UR_case_B.xml"
                                << expectedVals2;

    QTest::newRow("SMR SrvMsg") << cat010SmrSrvMsg
                                << "CAT010_SMR_SrvMsg_UR_case_A.xml"
                                << expectedVals1;

    QTest::newRow("MLAT TgtRep") << cat010MlatTgtRep
                                 << "CAT010_MLAT_TgtRep_UR_case_A.xml"
                                 << expectedVals1;

    QTest::newRow("MLAT TgtRep") << cat010MlatTgtRep
                                 << "CAT010_MLAT_TgtRep_UR_case_B.xml"
                                 << expectedVals2;

    QTest::newRow("MLAT SrvMsg") << cat010MlatSrvMsg
                                 << "CAT010_MLAT_SrvMsg_UR_case_A.xml"
                                 << expectedVals1;
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
    QFETCH(QVector<double>, result);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    int i = 0;
    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());

        double expectedVal = result.at(i);
        double actualVal = 0.0;

        if (testType == cat010SmrTgtRep)
        {
            actualVal = processor.ed116TgtRepUpdateRate(Aerodrome::Runway);
        }
        else if (testType == cat010SmrSrvMsg)
        {
            actualVal = processor.ed116SrvMsgUpdateRate();
        }
        else if (testType == cat010MlatTgtRep)
        {
            actualVal = processor.ed117TgtRepUpdateRate(Aerodrome::Runway);
        }
        else if (testType == cat010MlatSrvMsg)
        {
            actualVal = processor.ed117SrvMsgUpdateRate();
        }

        if (qIsNaN(expectedVal))
        {
            QVERIFY(qIsNaN(actualVal));
        }
        else
        {
            QCOMPARE(actualVal, expectedVal);
        }

        ++i;
    }
}

void MopsProcessorTest::testProbDetection_data()
{
    QTest::addColumn<TestType>("testType");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QVector<double>>("result");

    QVector<double> expectedVals = QVector<double>() << 1.0 << 1.0 << 1.0 << 1.0 << 1.0
                                                     << 1.0 << 1.0 << 1.0 << 1.0 << 1.0;

    QTest::newRow("SMR TgtRep") << cat010SmrTgtRep
                                << "CAT010_SMR_TgtRep_PD.xml"
                                << expectedVals;

    QTest::newRow("MLAT TgtRep") << cat010MlatTgtRep
                                 << "CAT010_MLAT_TgtRep_PD.xml"
                                 << expectedVals;
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
    QFETCH(QVector<double>, result);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    int i = 0;
    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());

        double expectedVal = result.at(i);
        double actualVal = 0.0;

        if (testType == cat010SmrTgtRep)
        {
            actualVal = processor.ed116TgtRepProbDetection(Aerodrome::Runway);
        }
        else if (testType == cat010MlatTgtRep)
        {
            actualVal = processor.ed117TgtRepProbDetection(Aerodrome::Runway);
        }

        if (qIsNaN(expectedVal))
        {
            QVERIFY(qIsNaN(actualVal));
        }
        else
        {
            QCOMPARE(actualVal, expectedVal);
        }

        ++i;
    }
}

QTEST_APPLESS_MAIN(MopsProcessorTest)
#include "mopsprocessortest.moc"
