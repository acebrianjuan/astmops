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
    explicit RecordCollator(QObject* parent = nullptr);

    void processRecord(const AsterixRecord& record);

    void loadExcludedAddresses(QIODevice* device);
    void loadExcludedAddresses(QVector<IcaoAddr> excladdr);
    void clearExcludedAddresses();
    void addExcludedAddress(const IcaoAddr addr);
    void removeExcludedAddress(const IcaoAddr addr);

    bool hasPendingBatches();

    QVector<IcaoAddr> excludedAddresses() const;

    QQueue<AsterixRecord> smrQueue() const;
    QQueue<AsterixRecord> mlatQueue() const;
    QQueue<AsterixRecord> adsbQueue() const;
    QQueue<AsterixRecord> srvMsgQueue() const;

public slots:

signals:
    void readyRead();

private:
    QVector<IcaoAddr> m_excludedAddresses;

    QHash<SystemType, QQueue<AsterixRecord>> m_catMap;
    QQueue<AsterixRecord> m_smrQueue;
    QQueue<AsterixRecord> m_mlatQueue;
    QQueue<AsterixRecord> m_adsbQueue;
    QQueue<AsterixRecord> m_srvMsgQueue;
};

#endif  // ASTMOPS_RECORDCOLLATOR_H
