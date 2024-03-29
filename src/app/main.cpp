/*!
 * \file main.cpp
 * \brief Entry point of the ASTMOPS program.
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * -----------------------------------------------------------------------
 *
 * Copyright (C) 2020-2021 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
 *
 * ASTMOPS is a command line tool for evaluating 
 * the performance of A-SMGCS sensors at airports
 *
 * This file is part of ASTMOPS.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------
 */

#include "asterixxmlreader.h"
#include "dgpscsvreader.h"
#include "kmlreader.h"
#include "perfevaluator.h"
#include "targetreportextractor.h"
#include "trackextractor.h"
#include <QCoreApplication>
#include <QFile>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    QCoreApplication::setOrganizationName(QLatin1String("astmops"));
    QCoreApplication::setApplicationName(QLatin1String("astmops"));

    static ProcessingMode mode = Configuration::processingMode();

    std::optional<QString> logRules_opt = Configuration::logRules();
    if (logRules_opt.has_value())
    {
        QLoggingCategory::setFilterRules(logRules_opt.value());
    }
    else
    {
        // Debug log messages are turned off by default.
        QLoggingCategory::setFilterRules(QLatin1String("*.debug=false"));
    }

    std::optional<QString> logPattern_opt = Configuration::logPattern();
    if (logPattern_opt.has_value())
    {
        qSetMessagePattern(logPattern_opt.value());
    }

    const QStringList args = application.arguments();
    qDebug() << "args" << args;

    qInfo() << "Configuration file:" << Configuration::fileName();

    if (qEnvironmentVariableIsSet("APPDIR"))
    {
        // When running from an AppImage, we need to find out the location of
        // the GeographicLib assets at runtime.

        QByteArray arr(qgetenv("APPDIR"));
        arr.append("/usr/share/GeographicLib/");

        setenv("GEOGRAPHICLIB_DATA", arr.constData(), 1);
    }

    QString kmlFilePath = Configuration::kmlFile();
    QFile kmlFile(kmlFilePath);
    kmlFile.open(QIODevice::ReadOnly);

    KmlReader kmlReader;
    kmlReader.read(&kmlFile);

    Aerodrome aerodrome = kmlReader.makeAerodrome();

    auto leblCallback = [&aerodrome](const QVector3D cartPos, const bool gndBit) {
        return aerodrome.locatePoint(cartPos, gndBit);
    };

    AsterixXmlReader astXmlReader;

    TargetReportExtractor tgtRepExtr(aerodrome.arp(), aerodrome.smr());
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

    PerfEvaluator perfEval;

    // DGPS CSV reference.
    if (mode == ProcessingMode::Dgps)
    {
        QString dgpsPath = Configuration::dgpsFile();
        ModeS mode_s = Configuration::dgpsModeS();
        Mode3A mode_3a = Configuration::dgpsMode3A();
        Ident ident = Configuration::dgpsIdent();
        qint32 tod_offset = Configuration::dgpsTodOffset();

        QFile dgpsFile(dgpsPath);
        dgpsFile.open(QIODevice::ReadOnly);

        DgpsTargetData dgps;
        dgps.mode_s_ = mode_s;
        dgps.mode_3a_ = mode_3a;
        dgps.ident_ = ident;
        dgps.tod_offset_ = tod_offset;
        dgps.data_ = readDgpsCsv(&dgpsFile);

        tgtRepExtr.addDgpsData(dgps);
    }


    // ASTERIX XML.
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

    while (trackExtr.hasPendingData())
    {
        std::optional<Track> trk_opt = trackExtr.takeData();
        if (trk_opt.has_value())
        {
            perfEval.addData(trk_opt.value());
        }
    }

    perfEval.run();

    qDebug() << "\nFinished!";
}
