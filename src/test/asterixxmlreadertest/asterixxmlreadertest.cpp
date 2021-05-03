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
    void initTestCase();
    void test_data();
    void test();
};

void AsterixXmlReaderTest::initTestCase()
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops-asterixxmlreadertest"));

    QSettings settings;
    settings.clear();

    settings.setValue(QLatin1String("SMR.SIC"), 7);
    settings.setValue(QLatin1String("MLAT.SIC"), 107);
    settings.setValue(QLatin1String("ADSB.SIC"), 109);

    settings.setValue(QLatin1String("Date"), QLatin1String("2020-05-05"));
}

void AsterixXmlReaderTest::test_data()
{
    // TARGET REPORTS.

    // SMR:

    // Record 0: [ToD: 23:59:58.000], CAT010 SMR Target Report.
    Asterix::Record cat010Smr0(quint8(10),
        QDateTime::fromString(QLatin1String("2020-05-05T23:59:58.000Z"), Qt::ISODateWithMs));

    cat010Smr0.dataItems_[QLatin1String("I010")] =
        Asterix::DataItem(QLatin1String("I010"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("SAC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("SIC"), QLatin1String("7")));

    cat010Smr0.dataItems_[QLatin1String("I000")] =
        Asterix::DataItem(QLatin1String("I000"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("MsgTyp"), QLatin1String("1")));

    cat010Smr0.dataItems_[QLatin1String("I020")] =
        Asterix::DataItem(QLatin1String("I020"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("TYP"), QLatin1String("3"))
                                            << Asterix::DataElement(QLatin1String("DCR"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CHN"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("GBS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CRT"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("FX"), QLatin1String("0")));

    cat010Smr0.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("86398.0000000")));

    cat010Smr0.dataItems_[QLatin1String("I040")] =
        Asterix::DataItem(QLatin1String("I040"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("RHO"), QLatin1String("638.0000000"))
                                            << Asterix::DataElement(QLatin1String("Theta"), QLatin1String("117.6196289")));

    cat010Smr0.dataItems_[QLatin1String("I042")] =
        Asterix::DataItem(QLatin1String("I042"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("X"), QLatin1String("565.0000000"))
                                            << Asterix::DataElement(QLatin1String("Y"), QLatin1String("-295.0000000")));

    cat010Smr0.dataItems_[QLatin1String("I200")] =
        Asterix::DataItem(QLatin1String("I200"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("GS"), QLatin1String("0.0026245"))
                                            << Asterix::DataElement(QLatin1String("TA"), QLatin1String("104.1668701")));

    cat010Smr0.dataItems_[QLatin1String("I202")] =
        Asterix::DataItem(QLatin1String("I202"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Vx"), QLatin1String("4.7500000"))
                                            << Asterix::DataElement(QLatin1String("Vy"), QLatin1String("-1.0000000")));

    cat010Smr0.dataItems_[QLatin1String("I161")] =
        Asterix::DataItem(QLatin1String("I161"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("spare"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TrkNb"), QLatin1String("1391")));

    cat010Smr0.dataItems_[QLatin1String("I170")] =
        Asterix::DataItem(QLatin1String("I170"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("CNF"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TRE"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("CST"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("MAH"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("TCC"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("STH"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("FX"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("TOM"), QLatin1String("3"))
                                            << Asterix::DataElement(QLatin1String("DOU"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("MRS"), QLatin1String("0"))
                                            << Asterix::DataElement(QLatin1String("FX"), QLatin1String("0")));

    cat010Smr0.dataItems_[QLatin1String("I270")] =
        Asterix::DataItem(QLatin1String("I270"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Length"), QLatin1String("38.0000000"))
                                            << Asterix::DataElement(QLatin1String("FX"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("Ori"), QLatin1String("98.3500000"))
                                            << Asterix::DataElement(QLatin1String("FX"), QLatin1String("1"))
                                            << Asterix::DataElement(QLatin1String("Width"), QLatin1String("33.0000000"))
                                            << Asterix::DataElement(QLatin1String("FX"), QLatin1String("0")));

    cat010Smr0.dataItems_[QLatin1String("I210")] =
        Asterix::DataItem(QLatin1String("I210"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("Ax"), QLatin1String("3.0000000"))
                                            << Asterix::DataElement(QLatin1String("Ay"), QLatin1String("0.0000000")));

    // Record 1: [ToD: 23:59:59.000], CAT010 SMR Target Report.
    Asterix::Record cat010Smr1(quint8(10),
        QDateTime::fromString(QLatin1String("2020-05-05T23:59:59.000Z"), Qt::ISODateWithMs));

    cat010Smr1.dataItems_ = cat010Smr0.dataItems_;

    cat010Smr1.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("86399.0000000")));

    // Record 2: [ToD: 23:59:59.500], CAT010 SMR Target Report.
    Asterix::Record cat010Smr2(quint8(10),
        QDateTime::fromString(QLatin1String("2020-05-05T23:59:59.500Z"), Qt::ISODateWithMs));

    cat010Smr2.dataItems_ = cat010Smr0.dataItems_;

    cat010Smr2.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("86399.5000000")));

    // Record 3: [ToD: 00:00:00.000], CAT010 SMR Target Report.
    Asterix::Record cat010Smr3(quint8(10),
        QDateTime::fromString(QLatin1String("2020-05-06T00:00:00.000Z"), Qt::ISODateWithMs));

    cat010Smr3.dataItems_ = cat010Smr0.dataItems_;

    cat010Smr3.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("00000.0000000")));

    // Record 4: [ToD: 00:00:01.000], CAT010 SMR Target Report.
    Asterix::Record cat010Smr4(quint8(10),
        QDateTime::fromString(QLatin1String("2020-05-06T00:00:01.000Z"), Qt::ISODateWithMs));

    cat010Smr4.dataItems_ = cat010Smr0.dataItems_;

    cat010Smr4.dataItems_[QLatin1String("I140")] =
        Asterix::DataItem(QLatin1String("I140"),
            QVector<Asterix::DataElement>() << Asterix::DataElement(QLatin1String("ToD"), QLatin1String("00001.0000000")));


    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QVector<Asterix::Record>>("recordsIn");

    QTest::newRow("CAT010") << "cat010_incomplete.xml" << (QVector<Asterix::Record>() << cat010Smr0 << cat010Smr1);
    QTest::newRow("CAT010 (Incomplete chunk)") << "cat010_incomplete.xml" << (QVector<Asterix::Record>() << cat010Smr0 << cat010Smr1);
    QTest::newRow("CAT010 (MIDNIGHT ROLLOVER)") << "cat010_rollover.xml" << (QVector<Asterix::Record>() << cat010Smr0 << cat010Smr1 << cat010Smr3 << cat010Smr4);
    QTest::newRow("CAT010 (MIDNIGHT ROLLOVER DELAYED SAMPLE)") << "cat010_rollover_delayed.xml" << (QVector<Asterix::Record>() << cat010Smr0 << cat010Smr1 << cat010Smr3 << cat010Smr2 << cat010Smr4);


    //QTest::newRow("CAT010") << "cat010.xml" << (QVector<int>() << 12 << 12);
    //QTest::newRow("CAT010 (Empty Data Items)") << "cat010_empty.xml" << (QVector<int>() << 12 << 12);
    //QTest::newRow("CAT010 (Missing attributes)") << "cat010_missing_attributes.xml" << QVector<int>();
}

void AsterixXmlReaderTest::test()
{
    AsterixXmlReader reader;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(QVector<Asterix::Record>, recordsIn);

    //    const QByteArray contents = file.readAll();
    //    qDebug() << contents;
    //    reader.addData(contents);

    do
    {
        QByteArray line = file.readLine();

        // Split at EOT control character to simulate a buffer interruption.
        QByteArrayList list = line.split('\x04');

        for (const auto &l : list)
        {
            reader.addData(l);
        }
    } while (file.canReadLine());


    QVector<Asterix::Record> recordsOut;
    while (reader.hasPendingRecords())
    {
        recordsOut.append(reader.record());
    }

    QCOMPARE(recordsOut.size(), recordsIn.size());

    for (int i = 0; i < recordsOut.size(); ++i)
    {
        QCOMPARE(recordsOut.at(i), recordsIn.at(i));
    }
}

QTEST_APPLESS_MAIN(AsterixXmlReaderTest)
#include "asterixxmlreadertest.moc"
