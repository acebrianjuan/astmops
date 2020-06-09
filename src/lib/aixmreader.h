#ifndef ASTMOPS_AIXMREADER_H
#define ASTMOPS_AIXMREADER_H

#include "aerodrome.h"
#include <QGeoCoordinate>
#include <QObject>
#include <QXmlStreamReader>

class AixmReader : public QObject
{
    Q_OBJECT

    friend class AixmReaderTest;

public:
    explicit AixmReader(QObject *parent = nullptr);

    bool read(QIODevice *device);
    //Aerodrome aerodrome() const { return m_aerodrome; }

public slots:

signals:

private:
    void readAixm();
    QStringList getPosList(const QStringList &tokens);
    QGeoCoordinate posListToCoord(QStringList list);
    QVector<QGeoCoordinate> posListToCoordVector(QStringList list);
    static QStringList arpPosXmlPath();
    static QStringList runwayPosListXmlPath();
    static QStringList taxiwayPosListXmlPath();
    static QStringList apronPosListXmlPath();
    // TODO: Add standPosListXmlPath() function;

    QXmlStreamReader m_xml;
    //Aerodrome m_aerodrome;

    QGeoCoordinate m_arp;
    QVector<QVector<QGeoCoordinate>> m_runwayElements;
    QVector<QVector<QGeoCoordinate>> m_taxiwayElements;
    QVector<QVector<QGeoCoordinate>> m_apronElements;
    QVector<QVector<QGeoCoordinate>> m_standElements;
    QVector<QVector<QGeoCoordinate>> m_approach1Elements;
    QVector<QVector<QGeoCoordinate>> m_approach2Elements;
};

#endif  // ASTMOPS_AIXMREADER_H
