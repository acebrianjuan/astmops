#ifndef ASTMOPS_AIXMREADER_H
#define ASTMOPS_AIXMREADER_H

#include "airport.h"
#include <QObject>
#include <QXmlStreamReader>

class AixmReader : public QObject
{
    Q_OBJECT

public:
    explicit AixmReader(QObject *parent = nullptr);

    bool read(QIODevice *device);
    Airport airport() const { return m_airport; }

public slots:

signals:

private:
    void readAixm();
    QPolygonF getPosList(const QStringList &tokens);
    QPolygonF posListToPolygon(QStringList list);
    static QStringList runwayPosListXmlPath();
    static QStringList taxiwayPosListXmlPath();
    static QStringList apronPosListXmlPath();

    QXmlStreamReader m_xml;
    Airport m_airport;
};

#endif  // ASTMOPS_AIXMREADER_H
