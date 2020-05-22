#ifndef ASTMOPS_ASTERIXXMLREADER_H
#define ASTMOPS_ASTERIXXMLREADER_H

#include "asterix.h"
#include <QObject>
#include <QQueue>
#include <QXmlStreamReader>

class AsterixXmlReader : public QObject
{
    Q_OBJECT

public:
    explicit AsterixXmlReader(QObject* parent = nullptr);

    void addData(const QByteArray& data);
    AsterixRecord record();
    bool hasPendingRecords();

public slots:

signals:
    void readyRead();

private:
    void readRecord();
    AsterixDataItem readDataItem();
    AsterixDataElement readDataField();
    bool isValidDataItem(const QString& di);

    QXmlStreamReader m_xml;
    QQueue<AsterixRecord> m_recordsQueue;
};

#endif  // ASTMOPS_ASTERIXXMLREADER_H
