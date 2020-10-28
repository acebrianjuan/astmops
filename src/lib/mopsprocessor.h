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

    struct Counter
    {
        quint32 n = 0;
        quint32 total = 0;
    };

    struct UpdateRateCounter
    {
        QDateTime firstTod;
        QDateTime lastTod;
        Aerodrome::Area area = Aerodrome::Area::None;

        bool isInitialized() { return firstTod.isValid(); }

        void reset()
        {
            firstTod = QDateTime();
            lastTod = QDateTime();
            area = Aerodrome::Area::None;
        }

        void update(QDateTime newTod)
        {
            if (firstTod.isNull())
            {
                firstTod = newTod;
            }
            else if (lastTod.isNull())
            {
                lastTod = newTod;
            }
            else if (firstTod.isValid() && lastTod.isValid())
            {
                firstTod = lastTod;
                lastTod = newTod;
            }
        }

        double timeDiff()
        {
            if (firstTod.isValid() && lastTod.isValid())
            {
                return firstTod.msecsTo(lastTod) / 1000;
            }

            return qQNaN();
        }

        operator bool() const
        {
            return firstTod.isValid() && lastTod.isValid();
        }
    };

    struct UpdateRateResult
    {
        quint32 updates = 0;
        quint32 expected = 0;
    };

    void setLocatePointCallback(std::function<Aerodrome::Area(const QPointF&)> callback);

    explicit MopsProcessor(QObject* parent = nullptr);
    void processRecord(const AsterixRecord& record);

public slots:
    double ed116TargetReportsMinimumFields();
    double ed116TargetReportsUpdateRate(Aerodrome::Area area = Aerodrome::All);

    double ed116ServiceMessagesMinimumFields();
    double ed116ServiceMessagesUpdateRate();

    double ed117TargetReportsMinimumFields();
    double ed117TargetReportsUpdateRate(Aerodrome::Area area = Aerodrome::All);

    double ed117ServiceMessagesMinimumFields();
    double ed117ServiceMessagesUpdateRate();

signals:

private:
    void processCat010SmrTgtRep(const AsterixRecord& record);
    void processCat010SmrSrvMsg(const AsterixRecord& record);

    void processCat010MlatTgtRep(const AsterixRecord& record);
    void processCat010MlatSrvMsg(const AsterixRecord& record);

    static bool checkDataItems(const AsterixRecord& record, const QVector<DataItemList>& collections);
    static bool checkDataItemsList(const AsterixRecord& record, const QStringList& list, DataItemListType type = Mandatory);

    static bool containsPosition(const AsterixRecord& record);

    static QVector<DataItemList> ed116TargetReportsMinimumDataItems();
    static QVector<DataItemList> ed117TargetReportsMinimumDataItems();
    static QVector<DataItemList> serviceMessagesMinimumDataItems();

    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);
    static QDateTime getDateTimefromTod(const double& tod);


    std::function<Aerodrome::Area(const QPointF&)> m_locatePoint;

    quint8 m_smrSic = Configuration::smrSic();
    quint8 m_mlatSic = Configuration::mlatSic();
    quint8 m_adsbSic = Configuration::adsbSic();

    float m_ed116SrvMsgUpdateRateHz = Configuration::ed116SrvMsgUpdateRate();
    float m_ed116TgtRepUpdateRateHz = Configuration::ed116TgtRepUpdateRate();

    float m_ed117SrvMsgUpdateRateHz = Configuration::ed117SrvMsgUpdateRate();
    float m_ed117TgtRepUpdateRateHz = Configuration::ed117TgtRepUpdateRate();

    float m_silencePeriod = Configuration::silencePeriod();

    QVector<DataItemList> m_ed116TgtRepMinDataItems = ed116TargetReportsMinimumDataItems();
    QVector<DataItemList> m_ed117TgtRepMinDataItems = ed117TargetReportsMinimumDataItems();
    QVector<DataItemList> m_srvMsgMinDataItems = serviceMessagesMinimumDataItems();

    Counter m_cat010SmrTgtRepCounter;
    Counter m_cat010MlatTgtRepCounter;

    Counter m_cat010SmrSrvMsgCounter;
    Counter m_cat010MlatSrvMsgCounter;

    QHash<TrackNum, UpdateRateCounter> m_cat010SmrTgtRepUpdateRateCounters;
    QHash<IcaoAddr, UpdateRateCounter> m_cat010MlatTgtRepUpdateRateCounters;

    UpdateRateCounter m_cat010SmrSrvMsgUpdateRateCounter;
    UpdateRateCounter m_cat010MlatSrvMsgUpdateRateCounter;

    UpdateRateResult m_cat010SmrSrvMsgUpdateRateTable;
    UpdateRateResult m_cat010MlatSrvMsgUpdateRateTable;

    QHash<Aerodrome::Area, UpdateRateResult> m_cat010SmrTgtRepUpdateRateTable;
    QHash<Aerodrome::Area, UpdateRateResult> m_cat010MlatTgtRepUpdateRateTable;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
