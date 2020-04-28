#include "aerodrome.h"

Aerodrome::Aerodrome()
{
}

bool Aerodrome::hasAnyElements()
{
    if (!runwayElements.isEmpty() || !taxiwayElements.isEmpty() ||
        !apronElements.isEmpty() || !standElements.isEmpty() ||
        !approach1Elements.isEmpty() || !approach2Elements.isEmpty())
    {
        return true;
    }
    return false;
}

bool Aerodrome::hasAllElements()
{
    if (!runwayElements.isEmpty() && !taxiwayElements.isEmpty() &&
        !apronElements.isEmpty() && !standElements.isEmpty() &&
        !approach1Elements.isEmpty() && !approach2Elements.isEmpty())
    {
        return true;
    }
    return false;
}

void Aerodrome::addRunwayElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    runwayElements.append(polygon);
}

void Aerodrome::addTaxiwayElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    taxiwayElements.append(polygon);
}

void Aerodrome::addApronElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    apronElements.append(polygon);
}

void Aerodrome::addStandElement(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    standElements.append(polygon);
}

void Aerodrome::addApproach1Element(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    approach1Elements.append(polygon);
}

void Aerodrome::addApproach2Element(const QPolygonF &polygon)
{
    Q_ASSERT(!polygon.isEmpty() && polygon.isClosed());
    approach2Elements.append(polygon);
}

Aerodrome::Area Aerodrome::locatePoint(const QPointF &point)
{
    Q_ASSERT(hasAnyElements());

    if (collectionContainsPoint(runwayElements, point))
    {
        return Area::Runway;
    }
    else if (collectionContainsPoint(taxiwayElements, point))
    {
        return Area::Taxiway;
    }
    else if (collectionContainsPoint(apronElements, point))
    {
        return Area::Apron;
    }
    else if (collectionContainsPoint(standElements, point))
    {
        return Area::Stand;
    }
    else if (collectionContainsPoint(approach1Elements, point))
    {
        return Area::Approach1;
    }
    else if (collectionContainsPoint(approach2Elements, point))
    {
        return Area::Approach2;
    }

    // TODO: Handle Area::Airborne case.

    return Area::None;
}

bool Aerodrome::collectionContainsPoint(const QVector<QPolygonF> &collection, const QPointF &point)
{
    Q_ASSERT(!collection.isEmpty());

    for (QPolygonF element : collection)
    {
        if (element.containsPoint(point, Qt::FillRule::OddEvenFill))
        {
            return true;
        }
    }
    return false;
}

QVector<QPolygonF> Aerodrome::getRunwayElements() const
{
    return runwayElements;
}

QVector<QPolygonF> Aerodrome::getTaxiwayElements() const
{
    return taxiwayElements;
}

QVector<QPolygonF> Aerodrome::getApronElements() const
{
    return apronElements;
}

QVector<QPolygonF> Aerodrome::getStandElements() const
{
    return standElements;
}

QVector<QPolygonF> Aerodrome::getApproach1Elements() const
{
    return approach1Elements;
}

QVector<QPolygonF> Aerodrome::getApproach2Elements() const
{
    return approach2Elements;
}
