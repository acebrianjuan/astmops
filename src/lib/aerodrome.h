#ifndef ASTMOPS_AERODROME_H
#define ASTMOPS_AERODROME_H

#include <QPolygonF>
#include <QVector>

class Aerodrome
{
public:
    enum Area
    {
        Manoeuvering,
        Runway,
        Taxiway,
        Apron,
        Stand,
        Approach1,
        Approach2,
        Airborne,
        None
    };

    Aerodrome();

    bool hasAnyElements();
    bool hasAllElements();
    void addRunwayElement(const QPolygonF &polygon);
    void addTaxiwayElement(const QPolygonF &polygon);
    void addApronElement(const QPolygonF &polygon);
    void addStandElement(const QPolygonF &polygon);
    Aerodrome::Area locatePoint(const QPointF &point);

    QVector<QPolygonF> getRunwayElements() const;
    QVector<QPolygonF> getTaxiwayElements() const;
    QVector<QPolygonF> getApronElements() const;
    QVector<QPolygonF> getStandElements() const;
    QVector<QPolygonF> getApproach1Elements() const;
    QVector<QPolygonF> getApproach2Elements() const;

private:
    bool collectionContainsPoint(const QVector<QPolygonF> &collection, const QPointF &point);

    QVector<QPolygonF> runwayElements;
    QVector<QPolygonF> taxiwayElements;
    QVector<QPolygonF> apronElements;
    QVector<QPolygonF> standElements;
    QVector<QPolygonF> approach1Elements;
    QVector<QPolygonF> approach2Elements;
};

#endif  // ASTMOPS_AERODROME_H
