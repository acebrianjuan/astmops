/*!
 * \file evaluatortest.cpp
 * \brief Implements unit tests for the Evaluator class.
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

#include "evaluator.h"
#include "asterixxmlreader.h"
#include "dgpscsvreader.h"
#include <QObject>
#include <QtTest>

class EvaluatorTest : public QObject
{
    Q_OBJECT

private slots:
    // called before the first test function
    void initTestCase();
    // called before every test function
    //void init();
    // called after every test function
    //void cleanup();
    // called after the last test function
    //void cleanupTestCase();

    void testPosAccDgps_data();
    void testPosAccDgps();

private:
    QMultiMap<QDateTime, AsterixRecord> dgpsTestDataMap(const QMultiMap<QDateTime, AsterixRecord> &testData, IcaoAddr dgpsAddr);
};


void EvaluatorTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-evaluatortest"));

    QSettings settings;
    settings.clear();

    settings.setValue(QLatin1String("Date"), QLatin1String("2020-05-05"));

    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 109);

    settings.beginGroup(QLatin1String("DGPS"));
    settings.setValue(QLatin1String("TargetAddress"), 0x34304F);
    settings.endGroup();
}

void EvaluatorTest::testPosAccDgps_data()
{
    QTest::addColumn<QString>("refFileName");
    QTest::addColumn<QString>("testFileName");
    QTest::addColumn<double>("result");

    QTest::newRow("TEST") << "eckjq_dgps_200505.csv"
                          << "eckjq_mlat_200505.xml"
                          << 0.0;
}

void EvaluatorTest::testPosAccDgps()
{
    AsterixXmlReader reader;
    reader.setOverrideDate(Configuration::asterixDate());

    Evaluator evaluator;

    IcaoAddr dgpsAddr = Configuration::dgpsTargetAddress();

    QFETCH(QString, refFileName);
    QFile refFile(QFINDTESTDATA(refFileName));
    QVERIFY(refFile.open(QIODevice::ReadOnly | QIODevice::Text));

    QFETCH(QString, testFileName);
    QFile testFile(QFINDTESTDATA(testFileName));
    QVERIFY(testFile.open(QIODevice::ReadOnly | QIODevice::Text));

    QMultiMap<QDateTime, QGeoPositionInfo> refData = readDgpsCsv(&refFile);

    const QByteArray contents = testFile.readAll();
    reader.addData(contents);
    QMultiMap<QDateTime, AsterixRecord> testData = reader.getMultiMap();

    evaluator.setRefData(refData);
    evaluator.setTestData(dgpsTestDataMap(testData, dgpsAddr));

    double rpa = evaluator.evalPosAccDgps();

    QFETCH(double, result);
    QCOMPARE(rpa, result);
}

QMultiMap<QDateTime, AsterixRecord> EvaluatorTest::dgpsTestDataMap(const QMultiMap<QDateTime, AsterixRecord> &testData, IcaoAddr dgpsAddr)
{
    QMultiMap<QDateTime, AsterixRecord> mmap;

    for (QMultiMap<QDateTime, AsterixRecord>::const_iterator it = testData.constBegin(); it != testData.constEnd(); ++it)
    {
        AsterixRecord rec = it.value();
        QString addrStr = rec.valStrFromDitem(QLatin1String("I220"), QLatin1String("TAddr"));
        if (!addrStr.isEmpty())
        {
            bool ok;
            IcaoAddr addr = addrStr.toUInt(&ok, 16);

            if (ok && addr == dgpsAddr)
            {
                mmap.insert(it.key(), it.value());
            }
        }
    }

    return mmap;
}


QTEST_APPLESS_MAIN(EvaluatorTest)
#include "evaluatortest.moc"
