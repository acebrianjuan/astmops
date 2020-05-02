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

    void setLocatePointCallback(std::function<Aerodrome::Area(const QPointF&)> callback);

    explicit MopsProcessor(QObject* parent = nullptr);
    void processRecord(const AsterixRecord& record);

public slots:
    double ed116TargetReportsMinimumFields();
    double ed117TargetReportsMinimumFields();
    double ed117ServiceMessagesMinimumFields();

signals:

private:
    static bool checkDataItems(const AsterixRecord& record, const QVector<DataItemList>& collections);
    static bool checkDataItemsList(const AsterixRecord& record, const QStringList& list, DataItemListType type = Mandatory);
    static QVector<DataItemList> ed116TargetReportsMinimumFieldsCollection();
    static QVector<DataItemList> ed117TargetReportsMinimumFieldsCollection();
    static QVector<DataItemList> ed117ServiceMessagesMinimumFieldsCollection();
    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);

    std::function<Aerodrome::Area(const QPointF&)> m_locatePoint;

    Counter ed116TgtRepCounter;
    Counter ed117TgtRepCounter;
    Counter srvMsgCounter;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
