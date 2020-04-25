#ifndef ASTMOPS_MOPSPROCESSOR_H
#define ASTMOPS_MOPSPROCESSOR_H

#include "asterix.h"
#include <QObject>
#include <QQueue>

class MopsProcessor : public QObject
{
    Q_OBJECT

public:
    struct Counter
    {
        uint n;
        uint total;
    };

    explicit MopsProcessor(QObject* parent = nullptr);
    void processRecord(const AsterixRecord& record);

public slots:
    double ed117TargetReportsMinimumFields();
    double ed117ServiceMessagesMinimumFields();

signals:

private:
    static bool checkDataItems(const AsterixRecord& record, const QStringList& list);
    static QStringList ed116TargetReportsList();
    static QStringList ed117TargetReportsList();
    static QStringList ed117ServiceMessagesList();
    static QHash<QString, bool> makeHash(const QStringList& list, bool state = false);

    Counter tgtRepCounter;
    Counter srvMsgCounter;
};

#endif  // ASTMOPS_MOPSPROCESSOR_H
