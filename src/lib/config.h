/*!
 * \file config.h
 * \brief
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

#ifndef ASTMOPS_CONFIG_H
#define ASTMOPS_CONFIG_H

#include "astmops.h"
#include <QSettings>
#include <optional>

class Settings : public QSettings
{
    Q_OBJECT

public:
    Settings();

private:
    QString configFilePath();
};

namespace Configuration
{
QSet<Sic> readSic(const QString& key);

QDate asterixDate();

bool useXmlTimestamp();

QSet<Sic> smrSic();
QSet<Sic> mlatSic();
QSet<Sic> adsbSic();

QString kmlFile();

QString dgpsFile();
ModeS dgpsModeS();
Mode3A dgpsMode3A();
Ident dgpsIdent();
qint32 dgpsTodOffset();

std::optional<QString> logRules();
std::optional<QString> logPattern();

};  // namespace Configuration

#endif  // ASTMOPS_CONFIG_H
