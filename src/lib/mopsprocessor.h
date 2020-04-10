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
        QDateTime lastToD;
        uint nRecords;
    };

    explicit MopsProcessor(QObject* parent = nullptr);

    void addTarget(const TargetData& target);

    bool ed116TargetReports(const AsterixRecord& record);
    bool ed117TargetReports(const AsterixRecord& record);
    void ed117UpdateRate(const AsterixRecord& record);

public slots:

signals:

private:
    bool targetReports(const AsterixRecord& record, QHash<QString, bool> hash);
    QHash<QString, bool> ed116TargetReportsHash();
    QHash<QString, bool> ed117TargetReportsHash();
    QHash<QString, bool> ed117ServiceMessagesHash();
    QHash<QString, bool> makeHash(const QStringList& list, bool state = false);

    QHash<uint, TargetData> m_targetStatusHash;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
