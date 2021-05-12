/*!
 * \file astmops.h
 * \brief Interface of essential definitions to be used in the scope
 * of the ASTMOPS program.
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

#ifndef ASTMOPS_ASTMOPS_H
#define ASTMOPS_ASTMOPS_H

#include "aerodrome.h"
//#include "track.h"
#include <QDate>
#include <QDebug>
#include <QGeoCircle>
#include <QGeoPolygon>
#include <QGeoShape>
#include <QPair>
#include <QSettings>
#include <QtGlobal>
#include <QtMath>

constexpr double ft_to_m = 0.3048;
constexpr double FL_to_m = ft_to_m * 100;

using Utn = quint32;
using ModeS = quint32;
using Mode3A = quint16;
using TrackNum = quint16;
using Ident = QString;

using Cat = quint8;
using Sac = quint8;
using Sic = quint8;

struct DataSrcId
{
    Sac sac_ = 0;
    Sic sic_ = 0;
};
Q_DECLARE_TYPEINFO(DataSrcId, Q_PRIMITIVE_TYPE);

bool operator==(DataSrcId lhs, DataSrcId rhs);

enum class SystemType
{
    Unknown,
    Smr,
    Mlat,
    Adsb,
    Dgps
};
Q_DECLARE_METATYPE(SystemType);

enum class MessageType
{
    Unknown,
    TargetReport,
    ServiceMessage
};
Q_DECLARE_METATYPE(MessageType);

enum class ServiceMessageType
{
    Unknown,
    StartOfUpdateCycle,
    PeriodicStatusMessage,
    EventTriggeredStatusMessage
};
Q_DECLARE_METATYPE(ServiceMessageType);

enum class TargetReportType
{
    Unknown,
    Surface,
    Airborne
};
Q_DECLARE_METATYPE(TargetReportType);

struct RecordType
{
    RecordType();
    RecordType(SystemType st, MessageType mt);

    bool isUnknown() const;

    SystemType sys_typ_ = SystemType::Unknown;
    MessageType msg_typ_ = MessageType::Unknown;
};

bool operator==(RecordType lhs, RecordType rhs);

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
uint qHash(const SystemType st, uint seed);
uint qHash(const MessageType mt, uint seed);
uint qHash(const RecordType rt, uint seed);
#else
size_t qHash(const SystemType st, size_t seed);
size_t qHash(const MessageType mt, size_t seed);
size_t qHash(const RecordType rt, size_t seed);
#endif

enum class Layer
{
    UnknownLayer,
    SurfaceLayer,
    AirborneLayer
};

//using EvalSet = QPair<Utn, QHash<SystemType, TrackCollection>>;

namespace MOPS
{
const double defaultSilencePeriodSeconds = 60.0;  // s
const int defaultDgpsTimeOfDayOffset = 0;         // s

namespace ED116
{
const double defaultTgtRepUpdateRateHz = 1.0;  // Hz
const double defaultSrvMsgUpdateRateHz = 1.0;  // Hz
}  // namespace ED116

namespace ED117
{
const double defaultTgtRepUpdateRateHz = 1.0;  // Hz
const double defaultSrvMsgUpdateRateHz = 1.0;  // Hz

const double defaultProbDetectionPeriodRunway = 1.0;
const double defaultProbDetectionPeriodApron = 5.0;
const double defaultProbDetectionPeriodOther = 2.0;
}  // namespace ED117
}  // namespace MOPS


quint8 readSic(const QString &key);

namespace Configuration
{
QDate asterixDate();

bool useAsterixTodAsTimestamp();

quint8 smrSic();

quint8 mlatSic();

quint8 adsbSic();

double ed116TgtRepUpdateRate();

double ed116SrvMsgUpdateRate();

double ed117TgtRepUpdateRate();

double ed117SrvMsgUpdateRate();

double silencePeriod();

double probDetectionPeriod(Aerodrome::Area area);

qint32 dgpsTimeOfDayOffset();

//Mode3A dgpsMode3ACode();

ModeS dgpsTargetAddress();

//Ident dgpsIdentification();

QGeoShape evalSector();

};  // namespace Configuration

#endif  // ASTMOPS_ASTMOPS_H
