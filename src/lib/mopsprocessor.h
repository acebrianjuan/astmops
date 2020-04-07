#ifndef ASTMOPS_MOPSPROCESSOR_H
#define ASTMOPS_MOPSPROCESSOR_H

#include "asterix.h"
#include <QObject>
#include <QQueue>

class MopsProcessor : public QObject
{
    Q_OBJECT

public:
    struct TargetData
    {
        uint address;
        QDateTime firstToD;
        QDateTime latToD;
        uint nRecords;
    };

    explicit MopsProcessor(QObject* parent = nullptr);

    void addRecord(const AsterixRecord& record);
    void addTarget(const TargetData& target);

    bool ed116TargetReports();
    bool ed117TargetReports();
    void ed117UpdateRate();

public slots:

signals:

private:
    bool targetReports(QHash<QString, bool> hash);
    QHash<QString, bool> ed116TargetReportsHash();
    QHash<QString, bool> ed117TargetReportsHash();
    QHash<QString, bool> ed117ServiceMessagesHash();
    QHash<QString, bool> makeHash(const QStringList& list, bool state = false);

    QMap<quint8, QQueue<AsterixRecord>> m_recordMap;
    QHash<uint, TargetData> m_targetStatusHash;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
