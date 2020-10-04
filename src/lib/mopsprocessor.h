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
        uint n = 0;
        uint total = 0;
    };

    struct UpdateRateCounter
    {
        bool isInitialized = false;
        uint n = 0;
        QDateTime firstTod;
        QDateTime lastTod;

        void reset()
        {
            isInitialized = false;
            n = 0;
            firstTod = QDateTime();
            lastTod = QDateTime();
        }

        operator bool() const
        {
            return n > 1 && firstTod.isValid() && lastTod.isValid();
        }
    };

    struct TargetData
    {
        uint address;
        QDateTime firstTod;
        QDateTime lastTod;
        QPointF position;
        Aerodrome::Area area;
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
    static bool checkDataItems(const AsterixRecord& record, const QVector<DataItemList>& collections);
    static bool checkDataItemsList(const AsterixRecord& record, const QStringList& list, DataItemListType type = Mandatory);

    static QVector<DataItemList> ed116TargetReportsMinimumDataItems();
    static QVector<DataItemList> ed117TargetReportsMinimumDataItems();
    static QVector<DataItemList> serviceMessagesMinimumDataItems();

    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);
    static QDateTime getDateTimefromTod(const double& tod);

    double calculateUpdateRate(const QList<quint32>& addresses, const QHash<quint32, UpdateRateCounter>& counters);
    double calculateUpdateRate(const UpdateRateCounter& counter);

    void readSettings();

    std::function<Aerodrome::Area(const QPointF&)> m_locatePoint;

    QVector<DataItemList> m_ed116TgtRepMinDataItems;
    QVector<DataItemList> m_ed117TgtRepMinDataItems;
    QVector<DataItemList> m_srvMsgMinDataItems;

    Counter m_cat010SmrTgtRepCounter;
    Counter m_cat010MlatTgtRepCounter;

    Counter m_cat010SmrSrvMsgCounter;
    Counter m_cat010MlatSrvMsgCounter;

    QHash<TrackNum, UpdateRateCounter> m_ed116TgtRepUpdateRateCounters;
    QHash<IcaoAddr, UpdateRateCounter> m_ed117TgtRepUpdateRateCounters;

    UpdateRateCounter m_cat010SmrSrvMsgUpdateRateCounter;
    UpdateRateCounter m_cat010MlatSrvMsgUpdateRateCounter;

    QHash<TrackNum, Aerodrome::Area> m_cat010SmrTgtRepAreas;
    QHash<IcaoAddr, Aerodrome::Area> m_cat010MlatTgtRepAreas;


    const QString m_smrSicKey = QLatin1String("SMR.SIC");
    const QString m_mlatSicKey = QLatin1String("MLAT.SIC");
    const QString m_adsbSicKey = QLatin1String("ADS-B.SIC");

    quint8 m_smrSic;
    quint8 m_mlatSic;
    quint8 m_adsbSic;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
