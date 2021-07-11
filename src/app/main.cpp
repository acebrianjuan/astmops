/*!
 * \file main.cpp
 * \brief Entry point of the ASTMOPS program.
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
#include "kmlreader.h"
#include "perfevaluator.h"
#include "targetreportextractor.h"
#include "trackextractor.h"
#include <QCoreApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops"));

    QCoreApplication application(argc, argv);
    const QStringList args = application.arguments();

    qDebug() << "args" << args;


    QGeoCoordinate leblArpGeo(41.297076579982225, 2.0784629201158662, 4.3200000000000003);
    QGeoCoordinate leblSmrGeo(41.29561944, 2.095113889, 4.3200000000000003);

    QString kmlFilePath = Configuration::kmlFile();
    QFile kmlFile(kmlFilePath);
    kmlFile.open(QIODevice::ReadOnly);

    KmlReader kmlReader;
    kmlReader.read(&kmlFile);

    std::optional<Aerodrome> ad_opt = kmlReader.makeAerodrome();

    if (!ad_opt.has_value())
    {
        qFatal("Error creating Aerodrome!");
    }

    Aerodrome aerodrome = ad_opt.value();

    auto leblCallback = [&aerodrome](const QVector3D cartPos, const bool gndBit) {
        return aerodrome.locatePoint(cartPos, gndBit);
    };

    AsterixXmlReader astXmlReader;

    TargetReportExtractor tgtRepExtr(leblArpGeo, leblSmrGeo);
    tgtRepExtr.setLocatePointCallback(leblCallback);

    TrackExtractor trackExtr;

    QObject::connect(&astXmlReader, &AsterixXmlReader::readyRead, [&]() {
        while (astXmlReader.hasPendingData())
        {
            tgtRepExtr.addData(astXmlReader.takeData().value());
        }
    });

    QObject::connect(&tgtRepExtr, &TargetReportExtractor::readyRead, [&]() {
        while (tgtRepExtr.hasPendingData())
        {
            trackExtr.addData(tgtRepExtr.takeData().value());
        }
    });

    QFile astXmlFile;
    if (args.size() > 1)
    {
        astXmlFile.setFileName(args.at(1));
        astXmlFile.open(QIODevice::ReadOnly);
    }
    else  // stdin
    {
        astXmlFile.open(stdin, QIODevice::ReadOnly);
    }

    while (!astXmlFile.atEnd())
    {
        const QByteArray line = astXmlFile.readLine();
        astXmlReader.addData(line);
    }


    PerfEvaluator perfEval;
    while (trackExtr.hasPendingData())
    {
        perfEval.addData(trackExtr.takeData().value());
    }

    perfEval.run();

    qDebug() << "\nFinished!";
}
