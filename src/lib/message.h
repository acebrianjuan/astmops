/*!
 * \file message.h
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

#ifndef ASTMOPS_MESSAGE_H
#define ASTMOPS_MESSAGE_H


class Message
{
public:
    enum MessageType
    {
        UnknownType,
        ServiceMessageType,
        TargetReportType
    };

    Message();

    MessageType m_type;
};

class ServiceMessage : Message
{
public:
    enum ServiceMessageType
    {
        UnknownType,
        Cat010Type
    };

    ServiceMessage();

    ServiceMessageType m_type;
};

class Cat010ServiceMessage : ServiceMessage
{
public:
    enum Cat010ServiceMessageType
    {
        UnknownType,
        StartOfUpdateCycleType,
        PeriodicStatusMessageType,
        EventTriggeredStatusMessage
    };

    Cat010ServiceMessage();

    Cat010ServiceMessageType m_type;

    // I010/000 Message Type
    // I010/010 Data Source Identifier
    // I010/140 Time of Day
    // I010/550 System Status
};

class TargetReport : Message
{
public:
    enum TargetReportType
    {
        UnknownType,
        Cat010Type,
        Cat021Type
    };

    TargetReport();

    TargetReportType m_type;
};

class Cat010TargetReport : TargetReport
{
public:
    enum Cat010TargetReportType
    {
        UnknownType,
        SsrMultilaterationType,
        ModeSMultilaterationType,
        AdsbType,
        PsrType,
        MagneticLoopSystemType,
        HfMultilaterationType,
        NotDefinedType,
        OtherTypesType
    };

    Cat010TargetReport();

    // I010/000 Message Type
    // I010/010 Data Source Identifier
    // I010/020 Target Report Descriptor
    // I010/040 Measured Position in Polar Coordinates
    // I010/041 Position in WGS-84 Coordinates
    // I010/042 Position in Cartesian Coordinates
    // I010/060 Mode-3/A Code
    // I010/090 Flight Level in Binary Representation
    // I010/091 Measured Height
    // I010/131 Amplitude of Primary Plot
    // I010/140 Time of Day
    // I010/161 Track Number
    // I010/170 Track Status
    // I010/200 Calculated Track Velocity in Polar Coordinates
    // I010/202 Calculated Track Velocity in Cartesian Coordinates
    // I010/210 Calculated Acceleration
    // I010/220 Target Address
    // I010/245 Target Identification
    // I010/250 Mode S MB Data
    // I010/270 Target Size & Orientation
    // I010/280 Presence
    // I010/300 Vehicle Fleet Identification
    // I010/310 Pre-programmed Message
    // I010/500 Standard Deviation of Position
    // I010/550 System Status
};

class Cat021TargetReport : TargetReport
{
public:
    Cat021TargetReport();

    // I021/008 Aircraft Operational Status
    // I021/010 Data Source Identification
    // I021/015 Service Identification
    // I021/016 Service Management
    // I021/020 Emitter Category
    // I021/040 Target Report Descriptor
    // I021/070 Mode 3/A Code
    // I021/071 Time of Applicability for Position
    // I021/072 Time of Applicability for Velocity
    // I021/073 Time of Message Reception for Position
    // I021/074 Time of Message Reception for Position – High Precision
    // I021/075 Time of Message Reception for Velocity
    // I021/076 Time of Message Reception for Velocity – High Precision
    // I021/077 Time of Report Transmission
    // I021/080 Target Address
    // I021/090 Quality Indicators
    // I021/110 Trajectory Intent
    // I021/130 Position in WGS-84 co-ordinates
    // I021/131 Position in WGS-84 co-ordinates, high resolution
    // I021/132 Message Amplitude
    // I021/140 Geometric Height
    // I021/145 Flight Level
    // I021/146 Selected Altitude
    // I021/148 Final State Selected Altitude
    // I021/150 Air Speed
    // I021/151 True Air Speed
    // I021/152 Magnetic Heading
    // I021/155 Barometric Vertical Rate
    // I021/157 Geometric Vertical Rate
    // I021/160 Airborne Ground Vector
    // I021/161 Track Number
    // I021/165 Track Angle Rate
    // I021/170 Target Identification
    // I021/200 Target Status
    // I021/210 MOPS Version
    // I021/220 Met Information
    // I021/230 Roll Angle
    // I021/250 Mode S MB Data
    // I021/260 ACAS Resolution Advisory Report
    // I021/271 Surface Capabilities and Characteristics
    // I021/295 Data Ages
    // I021/400 Receiver ID
};

#endif  // ASTMOPS_MESSAGE_H
