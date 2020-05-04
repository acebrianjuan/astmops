#ifndef ASTMOPS_MOPSPROCESSOR_H
#define ASTMOPS_MOPSPROCESSOR_H

#include "aerodrome.h"
#include "asterix.h"
#include <QObject>
#include <QQueue>

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
        uint n = 0;
        QDateTime firstToD;
        QDateTime lastToD;
    };

    struct TargetData
    {
        uint address;
        QDateTime firstToD;
        QDateTime lastToD;
        QPointF position;
        Aerodrome::Area area;
    };

    void setLocatePointCallback(std::function<Aerodrome::Area(const QPointF&)> callback);

    explicit MopsProcessor(QObject* parent = nullptr);
    void processRecord(const AsterixRecord& record);

public slots:
    double ed116TargetReportsMinimumFields();
    //double ed116DataRenewalRate();

    double ed117TargetReportsMinimumFields();
    double ed117ServiceMessagesMinimumFields();

    double serviceMessagesUpdateRate();

signals:

private:
    static bool checkDataItems(const AsterixRecord& record, const QVector<DataItemList>& collections);
    static bool checkDataItemsList(const AsterixRecord& record, const QStringList& list, DataItemListType type = Mandatory);
    static QVector<DataItemList> ed116TargetReportsMinimumFieldsCollection();
    static QVector<DataItemList> ed117TargetReportsMinimumFieldsCollection();
    static QVector<DataItemList> ed117ServiceMessagesMinimumFieldsCollection();
    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);

    std::function<Aerodrome::Area(const QPointF&)> m_locatePoint;

    Counter m_ed116TgtRepCounter;
    Counter m_ed117TgtRepCounter;
    Counter m_srvMsgCounter;

    UpdateRateCounter m_srvMsgUpdateRateCounter;
    bool m_srvMsgUpdateRateCounterInitialized;

    //QHash<uint, TargetData> m_ed116TgtData;
    //QHash<uint, TargetData> m_ed117TgtData;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
