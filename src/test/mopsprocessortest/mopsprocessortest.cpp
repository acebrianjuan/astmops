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
Q_DECLARE_METATYPE(Aerodrome::Area);

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

    QVector<double> expectedVals1 = QVector<double>()
                                    << 1.0 << 1.0 << 1.0 << 1.0 << 1.0
                                    << 1.0 << 1.0 << 1.0 << 1.0 << 1.0;

    QVector<double> expectedVals2 = QVector<double>()
                                    << 1.0 / 1.0 << 1.0 / 2.0 << 2.0 / 3.0 << 2.0 / 4.0 << 3.0 / 5.0
                                    << 3.0 / 6.0 << 4.0 / 7.0 << 4.0 / 8.0 << 5.0 / 9.0 << 5.0 / 10.0;

    QTest::newRow("SMR TgtRep (case A)") << cat010SmrTgtRep
                                         << "CAT010_SMR_TgtRep_MF_case_A.xml"
                                         << expectedVals1;

    QTest::newRow("SMR TgtRep (case B)") << cat010SmrTgtRep
                                         << "CAT010_SMR_TgtRep_MF_case_B.xml"
                                         << expectedVals2;

    QTest::newRow("SMR SrvMsg (case A)") << cat010SmrSrvMsg
                                         << "CAT010_SMR_SrvMsg_MF_case_A.xml"
                                         << expectedVals1;

    QTest::newRow("SMR SrvMsg (case B)") << cat010SmrSrvMsg
                                         << "CAT010_SMR_SrvMsg_MF_case_B.xml"
                                         << expectedVals2;

    QTest::newRow("MLAT TgtRep (case A)") << cat010MlatTgtRep
                                          << "CAT010_MLAT_TgtRep_MF_case_A.xml"
                                          << expectedVals1;

    QTest::newRow("MLAT TgtRep (case B)") << cat010MlatTgtRep
                                          << "CAT010_MLAT_TgtRep_MF_case_B.xml"
                                          << expectedVals2;

    QTest::newRow("MLAT SrvMsg (case A)") << cat010MlatSrvMsg
                                          << "CAT010_MLAT_SrvMsg_MF_case_A.xml"
                                          << expectedVals1;

    QTest::newRow("MLAT SrvMsg (case B)") << cat010MlatSrvMsg
                                          << "CAT010_MLAT_SrvMsg_MF_case_B.xml"
                                          << expectedVals2;
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

    QVector<double> expectedVals1 = QVector<double>()
                                    << 1.0 << 1.0 << 1.0 << 1.0 << 1.0
                                    << 1.0 << 1.0 << 1.0 << 1.0 << 1.0;

    QVector<double> expectedVals2 = QVector<double>()
                                    << 1.0 << 2.0 / 3.0 << 3.0 / 6.0 << 4.0 / 10.0;

    QTest::newRow("SMR TgtRep (case A)") << cat010SmrTgtRep
                                         << "CAT010_SMR_TgtRep_UR_case_A.xml"
                                         << expectedVals1;

    QTest::newRow("SMR TgtRep (case B)") << cat010SmrTgtRep
                                         << "CAT010_SMR_TgtRep_UR_case_B.xml"
                                         << expectedVals2;

    QTest::newRow("SMR SrvMsg (case A)") << cat010SmrSrvMsg
                                         << "CAT010_SMR_SrvMsg_UR_case_A.xml"
                                         << expectedVals1;

    QTest::newRow("SMR SrvMsg (case B)") << cat010SmrSrvMsg
                                         << "CAT010_SMR_SrvMsg_UR_case_B.xml"
                                         << expectedVals2;

    QTest::newRow("MLAT TgtRep (case A)") << cat010MlatTgtRep
                                          << "CAT010_MLAT_TgtRep_UR_case_A.xml"
                                          << expectedVals1;

    QTest::newRow("MLAT TgtRep (case B)") << cat010MlatTgtRep
                                          << "CAT010_MLAT_TgtRep_UR_case_B.xml"
                                          << expectedVals2;

    QTest::newRow("MLAT SrvMsg (case A)") << cat010MlatSrvMsg
                                          << "CAT010_MLAT_SrvMsg_UR_case_A.xml"
                                          << expectedVals1;

    QTest::newRow("MLAT SrvMsg (case B)") << cat010MlatSrvMsg
                                          << "CAT010_MLAT_SrvMsg_UR_case_B.xml"
                                          << expectedVals2;
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
    QTest::addColumn<QVector<Aerodrome::Area>>("areas");
    QTest::addColumn<QVector<double>>("result");

    QVector<double> expectedVals1 = QVector<double>() << 1.0 << 1.0 << 1.0 << 1.0 << 1.0
                                                      << 1.0 << 1.0 << 1.0 << 1.0 << 1.0;

    QVector<double> expectedVals2 = QVector<double>() << 1.0 << 0.5 << 1.0 << 0.5 << 1.0 << 0.5;

    QVector<Aerodrome::Area> areas;
    areas << QVector<Aerodrome::Area>(2, Aerodrome::Runway)
          << QVector<Aerodrome::Area>(2, Aerodrome::Taxiway)
          << QVector<Aerodrome::Area>(2, Aerodrome::Apron);

    QTest::newRow("SMR TgtRep (case A)") << cat010SmrTgtRep
                                         << "CAT010_SMR_TgtRep_PD_case_A.xml"
                                         << QVector<Aerodrome::Area>(10, Aerodrome::Runway)
                                         << expectedVals1;

    QTest::newRow("SMR TgtRep (case B)") << cat010SmrTgtRep
                                         << "CAT010_SMR_TgtRep_PD_case_B.xml"
                                         << areas
                                         << expectedVals2;

    QTest::newRow("MLAT TgtRep (case A)") << cat010MlatTgtRep
                                          << "CAT010_MLAT_TgtRep_PD_case_A.xml"
                                          << QVector<Aerodrome::Area>(10, Aerodrome::Runway)
                                          << expectedVals1;

    QTest::newRow("MLAT TgtRep (case B)") << cat010MlatTgtRep
                                          << "CAT010_MLAT_TgtRep_PD_case_B.xml"
                                          << areas
                                          << expectedVals2;
}

void MopsProcessorTest::testProbDetection()
{
    AsterixXmlReader reader;
    MopsProcessor processor;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(TestType, testType);
    QFETCH(QVector<Aerodrome::Area>, areas);
    QFETCH(QVector<double>, result);

    const QByteArray contents = file.readAll();
    reader.addData(contents);

    int i = 0;

    auto areaCallback = [&areas, &i](const QPointF& point) {
        Q_UNUSED(point);
        return areas.at(i);
    };

    processor.setLocatePointCallback(areaCallback);

    while (reader.hasPendingRecords())
    {
        processor.processRecord(reader.record());

        double expectedVal = result.at(i);
        double actualVal = 0.0;

        if (testType == cat010SmrTgtRep)
        {
            actualVal = processor.ed116TgtRepProbDetection(areas.at(i));
        }
        else if (testType == cat010MlatTgtRep)
        {
            actualVal = processor.ed117TgtRepProbDetection(areas.at(i));
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
