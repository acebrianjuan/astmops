/*!
 * \file mopsprocessor.h
 * \brief Interface of the MopsProcessor class.
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

#ifndef ASTMOPS_MOPSPROCESSOR_H
#define ASTMOPS_MOPSPROCESSOR_H

#include "aerodrome.h"
#include "asterix.h"
#include "astmops.h"
#include "counters.h"
#include <QGeoPositionInfo>
#include <QObject>
#include <QQueue>

/*!
 * \brief The MopsProcessor class calculates the performance indicators of
 * the surveillance systems.
 */
class MopsProcessor : public QObject
{
    Q_OBJECT

public:
    enum DataItemListType
    {
        Mandatory,
        Disjunctive
    };

    struct DataItemList
    {
        QStringList items;
        DataItemListType type;
    };

    struct TargetData
    {
        Aerodrome::Area area;
        Counters::IntervalCounter updateRateCounter;
        Counters::IntervalCounter probDetectionCounter;
    };

    void setLocatePointCallback(const std::function<Aerodrome::Area(const QPointF&)>& callback);

    explicit MopsProcessor(QObject* parent = nullptr);
    void processRecord(const Asterix::Record& record);
    void setPosRef(const QMultiMap<QDateTime, QGeoPositionInfo>& posRef);

public slots:
    double ed116TgtRepMinimumFields();
    double ed116TgtRepUpdateRate(Aerodrome::Area area = Aerodrome::All);
    double ed116TgtRepProbDetection(Aerodrome::Area area = Aerodrome::All);

    double ed116SrvMsgMinimumFields();
    double ed116SrvMsgUpdateRate();

    double ed117TgtRepMinimumFields();
    double ed117TgtRepUpdateRate(Aerodrome::Area area = Aerodrome::All);
    double ed117TgtRepProbDetection(Aerodrome::Area area = Aerodrome::All);

    double ed117SrvMsgMinimumFields();
    double ed117SrvMsgUpdateRate();

signals:

private:
    void processCat010SmrTgtRep(const Asterix::Record& record);
    void processCat010SmrSrvMsg(const Asterix::Record& record);

    void processCat010MlatTgtRep(const Asterix::Record& record);
    void processCat010MlatSrvMsg(const Asterix::Record& record);

    void processCat021AdsbTgtRep(const Asterix::Record& record);

    // CAT010 SMR Target Report
    bool cat010SmrTgtRepMinDataItems(const Asterix::Record& record);
    void cat010SmrTgtRepUpdateRate(const Asterix::Record& record);
    void cat010SmrTgtRepProbDetection(const Asterix::Record& record);

    // CAT010 SMR Service Message
    bool cat010SmrSrvMsgMinDataItems(const Asterix::Record& record);
    void cat010SmrSrvMsgUpdateRate(const Asterix::Record& record);

    // CAT010 MLAT Target Report
    bool cat010MlatTgtRepMinDataItems(const Asterix::Record& record);
    void cat010MlatTgtRepUpdateRate(const Asterix::Record& record);
    void cat010MlatTgtRepProbDetection(const Asterix::Record& record);

    // CAT010 MLAT Service Message
    bool cat010MlatSrvMsgMinDataItems(const Asterix::Record& record);
    void cat010MlatSrvMsgUpdateRate(const Asterix::Record& record);

    static bool checkDataItems(const Asterix::Record& record, const QVector<DataItemList>& collections);
    static bool checkDataItemsList(const Asterix::Record& record, const QStringList& list, DataItemListType type = Mandatory);

    static bool containsPosition(const Asterix::Record& record);

    static QVector<DataItemList> ed116TargetReportsMinimumDataItems();
    static QVector<DataItemList> ed117TargetReportsMinimumDataItems();
    static QVector<DataItemList> serviceMessagesMinimumDataItems();

    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);
    static QDateTime getDateTimefromTod(double tod);


    std::function<Aerodrome::Area(const QPointF&)> m_locatePoint;

    quint8 m_smrSic = Configuration::smrSic();
    quint8 m_mlatSic = Configuration::mlatSic();
    quint8 m_adsbSic = Configuration::adsbSic();

    double m_ed116SrvMsgUpdateRateHz = Configuration::ed116SrvMsgUpdateRate();
    double m_ed116TgtRepUpdateRateHz = Configuration::ed116TgtRepUpdateRate();

    double m_ed117SrvMsgUpdateRateHz = Configuration::ed117SrvMsgUpdateRate();
    double m_ed117TgtRepUpdateRateHz = Configuration::ed117TgtRepUpdateRate();

    double m_silencePeriod = Configuration::silencePeriod();

    QVector<DataItemList> m_ed116TgtRepMinDataItems = ed116TargetReportsMinimumDataItems();
    QVector<DataItemList> m_ed117TgtRepMinDataItems = ed117TargetReportsMinimumDataItems();
    QVector<DataItemList> m_srvMsgMinDataItems = serviceMessagesMinimumDataItems();

    Counters::BasicCounter m_cat010SmrTgtRepCounter;
    Counters::BasicCounter m_cat010MlatTgtRepCounter;

    Counters::BasicCounter m_cat010SmrSrvMsgCounter;
    Counters::BasicCounter m_cat010MlatSrvMsgCounter;

    QHash<TrackNum, TargetData> m_cat010SmrTgtRepUpdateRateCounters;
    QHash<TrackNum, TargetData> m_cat010SmrTgtRepProbDetectionCounters;

    QHash<IcaoAddr, TargetData> m_cat010MlatTgtRepUpdateRateCounters;
    QHash<IcaoAddr, TargetData> m_cat010MlatTgtRepProbDetectionCounters;

    Counters::IntervalCounter m_cat010SmrSrvMsgUpdateRateCounter = Counters::IntervalCounter(1 / m_ed116SrvMsgUpdateRateHz);
    Counters::IntervalCounter m_cat010MlatSrvMsgUpdateRateCounter = Counters::IntervalCounter(1 / m_ed117SrvMsgUpdateRateHz);

    Counters::BasicCounter m_cat010SmrSrvMsgUpdateRateTable;
    Counters::BasicCounter m_cat010MlatSrvMsgUpdateRateTable;

    QHash<Aerodrome::Area, Counters::BasicCounter> m_cat010SmrTgtRepUpdateRateTable;
    QHash<Aerodrome::Area, Counters::BasicCounter> m_cat010SmrTgtRepProbDetectionTable;

    QHash<Aerodrome::Area, Counters::BasicCounter> m_cat010MlatTgtRepUpdateRateTable;
    QHash<Aerodrome::Area, Counters::BasicCounter> m_cat010MlatTgtRepProbDetectionTable;

    QMultiMap<QDateTime, QGeoPositionInfo> m_posRef;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
