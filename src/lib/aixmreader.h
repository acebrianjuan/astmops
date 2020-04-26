#ifndef ASTMOPS_AIXMREADER_H
#define ASTMOPS_AIXMREADER_H

#include "aerodrome.h"
#include <QObject>
#include <QXmlStreamReader>

class AixmReader : public QObject
{
    Q_OBJECT

public:
    explicit AixmReader(QObject *parent = nullptr);

    bool read(QIODevice *device);
    Aerodrome aerodrome() const { return m_aerodrome; }

public slots:

signals:

private:
    void readAixm();
    QPolygonF getPosList(const QStringList &tokens);
    QPolygonF posListToPolygon(QStringList list);
    static QStringList runwayPosListXmlPath();
    static QStringList taxiwayPosListXmlPath();
    static QStringList apronPosListXmlPath();
    // TODO: Add standPosListXmlPath() function;

    QXmlStreamReader m_xml;
    Aerodrome m_aerodrome;
};

#endif  // ASTMOPS_AIXMREADER_H
