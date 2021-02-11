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
#include "kmlreader.h"
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

    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 109);
}

void EvaluatorTest::testPosAccDgps_data()
{
    QTest::addColumn<QString>("date");
    QTest::addColumn<IcaoAddr>("targetAddr");
    QTest::addColumn<QString>("refFileName");
    QTest::addColumn<QString>("testFileName");
    QTest::addColumn<double>("result");

    QTest::newRow("TEST 34304F") << "2020-05-05"
                                 << 0x34304FU
                                 << "dgps_34304F_20200505.csv"
                                 << "mlat_34304F_20200505.xml"
                                 << 0.0;

    QTest::newRow("TEST 344399") << "2019-05-21"
                                 << 0x344399U
                                 << "dgps_344399_20190521.csv"
                                 << "mlat_344399_20190521.xml"
                                 << 0.0;
}

void EvaluatorTest::testPosAccDgps()
{
    QSettings settings;

    QFETCH(QString, date);
    QFETCH(IcaoAddr, targetAddr);
    settings.setValue(QLatin1String("Date"), date);

    settings.beginGroup(QLatin1String("DGPS"));
    settings.setValue(QLatin1String("TargetAddress"), targetAddr);
    settings.endGroup();

    AsterixXmlReader asterixReader;
    asterixReader.setOverrideDate(Configuration::asterixDate());

    QFile kmlFile(QFINDTESTDATA("lebl_insignia.kml"));
    QVERIFY(kmlFile.open(QIODevice::ReadOnly));

    KmlReader kmlReader;
    kmlReader.read(&kmlFile);

    Aerodrome lebl = kmlReader.makeAerodrome();
    auto leblCallback = [&lebl](const QVector3D cartPos, const std::optional<bool> &gndBit = std::nullopt) {
        return lebl.locatePoint(cartPos, gndBit);
    };

    Evaluator evaluator;
    evaluator.setLocatePointCallback(leblCallback);

    IcaoAddr dgpsAddr = Configuration::dgpsTargetAddress();

    QFETCH(QString, refFileName);
    QFile refFile(QFINDTESTDATA(refFileName));
    QVERIFY(refFile.open(QIODevice::ReadOnly | QIODevice::Text));

    QFETCH(QString, testFileName);
    QFile testFile(QFINDTESTDATA(testFileName));
    QVERIFY(testFile.open(QIODevice::ReadOnly | QIODevice::Text));

    QMultiMap<QDateTime, QGeoPositionInfo> refData = readDgpsCsv(&refFile);

    const QByteArray contents = testFile.readAll();
    asterixReader.addData(contents);
    QMultiMap<QDateTime, AsterixRecord> testData = asterixReader.getMultiMap();

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
