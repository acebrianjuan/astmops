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
        bool isInitialized = false;
        uint n = 0;
        QDateTime firstTod;
        QDateTime lastTod;
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
    double ed116TargetReportsUpdateRate();

    double ed117TargetReportsMinimumFields();


    double serviceMessagesMinimumFields();
    double serviceMessagesUpdateRate();

signals:

private:
    static bool checkDataItems(const AsterixRecord& record, const QVector<DataItemList>& collections);
    static bool checkDataItemsList(const AsterixRecord& record, const QStringList& list, DataItemListType type = Mandatory);

    static QVector<DataItemList> ed116TargetReportsMinimumDataItems();
    static QVector<DataItemList> ed117TargetReportsMinimumDataItems();
    static QVector<DataItemList> serviceMessagesMinimumDataItems();

    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);
    static QDateTime getDateTimefromTod(const double& tod);

    std::function<Aerodrome::Area(const QPointF&)> m_locatePoint;

    QVector<DataItemList> m_ed116TgtRepMinDataItems;
    QVector<DataItemList> m_ed117TgtRepMinDataItems;
    QVector<DataItemList> m_srvMsgMinDataItems;

    Counter m_ed116TgtRepCounter;
    Counter m_ed117TgtRepCounter;
    Counter m_srvMsgCounter;

    QDateTime m_asterixDateTime;

    QHash<uint, UpdateRateCounter> m_ed116TgtRepUpdateRateCounters;
    QHash<uint, UpdateRateCounter> m_ed117TgtRepUpdateRateCounters;
    UpdateRateCounter m_srvMsgUpdateRateCounter;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
