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

    static bool ed116TargetReports(const AsterixRecord& record);
    static bool ed117TargetReports(const AsterixRecord& record);
    double ed117UpdateRate(const AsterixRecord& record);

public slots:

signals:

private:
    static bool checkDataItems(const AsterixRecord& record, const QStringList& list);
    static QStringList ed116TargetReportsList();
    static QStringList ed117TargetReportsList();
    static QStringList ed117ServiceMessagesList();
    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);

    QHash<uint, TargetData> m_targetStatusHash;
    QDateTime m_lastServiceMessage;
};

Q_DECLARE_METATYPE(MopsProcessor::TargetData);

#endif  // ASTMOPS_MOPSPROCESSOR_H
