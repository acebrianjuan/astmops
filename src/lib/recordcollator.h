#ifndef ASTMOPS_RECORDCOLLATOR_H
#define ASTMOPS_RECORDCOLLATOR_H

#include "asterix.h"
#include "astmops.h"
#include <QIODevice>
#include <QObject>
#include <QQueue>
#include <QVector>

class RecordCollator : public QObject
{
    Q_OBJECT

public:
    struct Counter
    {
        uint in = 0;
        uint out = 0;
    };

    explicit RecordCollator(QObject* parent = nullptr);

    void processRecord(const AsterixRecord& record);

    void loadExcludedAddresses(QIODevice* device);
    void loadExcludedAddresses(QVector<IcaoAddr> excladdr);
    void clearExcludedAddresses();
    void addExcludedAddress(const IcaoAddr addr);
    void removeExcludedAddress(const IcaoAddr addr);

    // TODO: Implement hasPendingBatches() for each queue.

    bool hasPendingBatches();

    void readSettings();

    QVector<IcaoAddr> excludedAddresses() const;

    // TODO: Implement getters that return the first n elements of the queue.

    QQueue<AsterixRecord> smrTgtRepQueue() const;
    QQueue<AsterixRecord> smrSrvMsgQueue() const;
    QQueue<AsterixRecord> mlatTgtRepQueue() const;
    QQueue<AsterixRecord> mlatSrvMsgQueue() const;
    QQueue<AsterixRecord> adsbTgtRepQueue() const;

    Counter smrTgtRepCounter() const;
    Counter smrSrvMsgCounter() const;
    Counter mlatTgtRepCounter() const;
    Counter mlatSrvMsgCounter() const;
    Counter adsbTgtRepCounter() const;

public slots:

signals:
    void readyRead();

private:
    const QString m_smrSicKey = QLatin1String("SMR.SIC");
    const QString m_mlatSicKey = QLatin1String("MLAT.SIC");
    const QString m_adsbSicKey = QLatin1String("ADS-B.SIC");

    QVector<IcaoAddr> m_excludedAddresses;

    quint8 m_smrSic;
    quint8 m_mlatSic;
    quint8 m_adsbSic;

    QHash<SystemType, QQueue<AsterixRecord>> m_catMap;
    QQueue<AsterixRecord> m_smrTgtRepQueue;
    QQueue<AsterixRecord> m_smrSrvMsgQueue;
    QQueue<AsterixRecord> m_mlatTgtRepQueue;
    QQueue<AsterixRecord> m_mlatSrvMsgQueue;
    QQueue<AsterixRecord> m_adsbTgtRepQueue;

    Counter m_smrTgtRepCounter;
    Counter m_smrSrvMsgCounter;
    Counter m_mlatTgtRepCounter;
    Counter m_mlatSrvMsgCounter;
    Counter m_adsbTgtRepCounter;
};

#endif  // ASTMOPS_RECORDCOLLATOR_H
