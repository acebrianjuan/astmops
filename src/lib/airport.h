#ifndef ASTMOPS_AIRPORT_H
#define ASTMOPS_AIRPORT_H

#include <QPolygonF>
#include <QVector>

struct Airport
{
    QVector<QPolygonF> runways;
    QVector<QPolygonF> taxiways;
    QVector<QPolygonF> aprons;
    QVector<QPolygonF> stands;
};

#endif  // ASTMOPS_AIRPORT_H
