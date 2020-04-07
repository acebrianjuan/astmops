#ifndef ASTMOPS_ASTERIXXMLREADER_H
#define ASTMOPS_ASTERIXXMLREADER_H

#include "asterix.h"
#include <QObject>
#include <QXmlStreamReader>

class AsterixXmlReader : public QObject
{
    Q_OBJECT

public:
    explicit AsterixXmlReader(QObject* parent = nullptr);

    void addData(const QByteArray& data);
    AsterixRecord record() const { return m_record; }
    // TODO: Add function for reading excluded addresses from file.
    //void loadExcludedAddresses();

public slots:

signals:
    void newRecord(AsterixRecord record);

private:
    void readRecord();
    AsterixDataItem readDataItem();
    AsterixDataElement readDataField();
    bool isValidDataItem(const QString& di);

    // TODO: Add container of excluded addresses.
    //QVector<uint> m_excludedAddresses;
    QXmlStreamReader m_xml;
    AsterixRecord m_record;
};

#endif  // ASTMOPS_ASTERIXXMLREADER_H
