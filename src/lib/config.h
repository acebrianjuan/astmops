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
Sic readSic(const QString& key);

QDate asterixDate();

bool useXmlTimestamp();

Sic smrSic();
Sic mlatSic();
Sic adsbSic();

QString kmlFile();

qint32 dgpsTodOffset();
Mode3A dgpsMode3A();
ModeS dgpsModeS();
Ident dgpsIdent();

QString logRules();
QString logPattern();

};  // namespace Configuration

#endif  // ASTMOPS_CONFIG_H
