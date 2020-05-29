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

    bool hasPendingBatches();

    void readSettings();

    QVector<IcaoAddr> excludedAddresses() const;

    QQueue<AsterixRecord> smrQueue() const;
    QQueue<AsterixRecord> mlatQueue() const;
    QQueue<AsterixRecord> adsbQueue() const;
    QQueue<AsterixRecord> srvMsgQueue() const;

    Counter smrCounter() const;
    Counter mlatCounter() const;
    Counter adsbCounter() const;
    Counter srvMsgCounter() const;

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
    QQueue<AsterixRecord> m_smrQueue;
    QQueue<AsterixRecord> m_mlatQueue;
    QQueue<AsterixRecord> m_adsbQueue;
    QQueue<AsterixRecord> m_srvMsgQueue;
    //QQueue<AsterixRecord> m_smrSrvMsgQueue;
    //QQueue<AsterixRecord> m_mlatSrvMsgQueue;

    Counter m_smrCounter;
    Counter m_mlatCounter;
    Counter m_adsbCounter;
    Counter m_srvMsgCounter;
    //Counter m_smrSrvMsgCounter;
    //Counter m_mlatSrvMsgCounter;
};

#endif  // ASTMOPS_RECORDCOLLATOR_H
