/*!
 * \file evaluator.cpp
 * \brief
 * \author Álvaro Cebrián Juan, 2020. acebrianjuan(at)gmail.com
 *
 * -----------------------------------------------------------------------
 *
 * Copyright (C) 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
 *
 * ASTMOPS is a surveillance performance analyzer
 *      for Air Traffic Control
 *
 * This file is part of ASTMOPS.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------
 */

#include "evaluator.h"
#include "geofunctions.h"
#include <QMetaEnum>
#include <QtMath>
#include <iostream>

Evaluator::Evaluator(QObject *parent) : QObject(parent)
{
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();
    for (int i = 0; i < e.keyCount(); ++i)
    {
        m_cat010MlatPosAccuracy.insert(static_cast<Aerodrome::Area>(e.value(i)), Counters::BasicCounter());
    }
}

double Evaluator::evalPosAccDgps()
{
    QGeoCoordinate leblArp(41.297076579982225, 2.0784629201158662, 4.3200000000000003);
    QGeoCoordinate leblSmr(41.297076579982225, 2.0784629201158662, 4.3200000000000003);

    calculateTestDataMappings();

    QDateTime tod;
    QGeoPositionInfo testPos;
    QGeoPositionInfo refPos;

    for (QMultiMap<QDateTime, AsterixRecord>::const_iterator it = m_testData.constBegin(); it != m_testData.constEnd(); ++it)
    {
        AsterixRecord testRec = it.value();

        tod = it.key();

        double xTest = testRec.valStrFromDitem(QLatin1String("I042"), QLatin1String("X")).toDouble();
        double yTest = testRec.valStrFromDitem(QLatin1String("I042"), QLatin1String("Y")).toDouble();

        bool gndBit = testRec.valStrFromDitem(QLatin1String("I020"), QLatin1String("GBS")).toUInt();

        TestDataMapping mapping = m_testDataMappings.value(tod);

        if (!mapping.hasRefPosInfo)
        {
            continue;
        }

        QGeoPositionInfo refPosInfo = mapping.refPosInfo;

        QVector3D cartRef = geoToLocalEnu(refPosInfo.coordinate(), leblArp);

        Aerodrome::Area area = m_locatePoint(cartRef, gndBit);

        double errX = cartRef.x() - xTest;
        double errY = cartRef.y() - yTest;

        double dist = qSqrt(qPow(errX, 2) + qPow(errY, 2));

        ++m_cat010MlatPosAccuracy[area].countTotal;
        if (dist <= 7.5)
        {
            ++m_cat010MlatPosAccuracy[area].countValid;
        }

        /*
        qDebug() << tod
                 << "REF_X:" << cartRef.x() << "REF_Y:" << cartRef.y()
                 << "TST_X:" << xTest << "TST_Y:" << yTest
                 << "ERR: " << dist;
        */

        //std::cout << tod.toString(Qt::ISODateWithMs).toStdString() << " ERR: " << dist << '\n';
    }

    for (QHash<Aerodrome::Area, Counters::BasicCounter>::const_iterator it = m_cat010MlatPosAccuracy.constBegin(); it != m_cat010MlatPosAccuracy.constEnd(); ++it)
    {
        double result = it.value().countTotal == 0 ? qSNaN()
                                                   : it.value().countValid / static_cast<double>(it.value().countTotal);

        qDebug() << it.key() << "\t\t" << it.value().countValid << "/" << it.value().countTotal << "=" << result;
    }

    return 0.0;
}

void Evaluator::setTestData(const QMultiMap<QDateTime, AsterixRecord> &testData)
{
    m_testData = testData;
}

void Evaluator::setLocatePointCallback(const std::function<Aerodrome::Area(const QVector3D &, const std::optional<bool>)> &callback)
{
    m_locatePoint = callback;
}

QGeoPositionInfo Evaluator::interpolatedRefPosForTime(const QDateTime &tod, double dtMax, bool *ok)
{
    if (ok)
    {
        *ok = false;
    }

    Q_ASSERT(m_testDataMappings.contains(tod));

    const TestDataMapping &mapping = m_testDataMappings.value(tod);

    if (!mapping.hasRef1 || !mapping.hasRef2)  // No ref data.
    {
        return QGeoPositionInfo();
    }

    Q_ASSERT(mapping.refPosInfo1.timestamp() <= tod);
    Q_ASSERT(mapping.refPosInfo2.timestamp() >= tod);

    if (mapping.refPosInfo1.timestamp().msecsTo(tod) > dtMax)  // Lower too far.
    {
        return QGeoPositionInfo();
    }

    if (tod.msecsTo(mapping.refPosInfo2.timestamp()) > dtMax)  // Upper too far.
    {
        return QGeoPositionInfo();
    }

    if (!mapping.hasRefPosInfo)
    {
        return QGeoPositionInfo();
    }

    if (ok)
    {
        *ok = true;
    }

    return mapping.refPosInfo;
}

void Evaluator::calculateTestDataMappings()
{
    Q_ASSERT(m_testDataMappings.isEmpty());

    for (QMultiMap<QDateTime, AsterixRecord>::const_iterator it = m_testData.constBegin(); it != m_testData.constEnd(); ++it)
    {
        QDateTime todTestData = it.key();
        m_testDataMappings.insert(todTestData, calculateTestDataMapping(todTestData));
    }
}

TestDataMapping Evaluator::calculateTestDataMapping(const QDateTime &tod) const
{
    TestDataMapping tdm;

    tdm.tod = tod;

    QMultiMap<QDateTime, QGeoPositionInfo>::const_iterator lbIt = m_refData.lowerBound(tod);

    if (lbIt != m_refData.end())  // Upper TOD found.
    {
        Q_ASSERT(lbIt.key() >= tod);

        // Save upper value.
        tdm.hasRef2 = true;
        tdm.refPosInfo2 = lbIt.value();

        // Search lower values by decrementing iterator.
        while (lbIt != m_refData.end() && (tod < lbIt.key() || lbIt.key() == tdm.refPosInfo2.timestamp()))
        {
            if (lbIt == m_refData.begin())  // Exit condition on first value.
            {
                if (tod < lbIt.key())  // Set as not found.
                {
                    lbIt = m_refData.end();
                }

                break;
            }

            lbIt--;
        }

        if (lbIt != m_refData.end() && lbIt.key() != tdm.refPosInfo2.timestamp())  // Lower TOD found.
        {
            Q_ASSERT(tod >= lbIt.key());

            // Add lower value.
            tdm.hasRef1 = true;
            tdm.refPosInfo1 = lbIt.value();
        }
        else  // Not found, clear previous.
        {
            tdm.hasRef2 = false;
            tdm.refPosInfo2 = QGeoPositionInfo();
        }
    }

    addRefPosToMapping(tdm);

    return tdm;
}

void Evaluator::addRefPosToMapping(TestDataMapping &mapping) const
{
    if (!mapping.hasRef1 || !mapping.hasRef2)  // Mapping must have two reference positions.
    {
        return;
    }

    QGeoPositionInfo posInfo1 = mapping.refPosInfo1;
    QGeoPositionInfo posInfo2 = mapping.refPosInfo2;

    double dtTotal = posInfo1.timestamp().msecsTo(posInfo2.timestamp()) / 1000.0;

    Q_ASSERT(dtTotal >= 0);

    if (posInfo1.coordinate().latitude() == posInfo2.coordinate().latitude() && posInfo1.coordinate().longitude() == posInfo2.coordinate().longitude())  // Same position.
    {
        /* If pos1 and pos2 are identical we directly assign this position
         * to the mapping object but it will not be possible to derive the
         * speed and heading.
         */
        mapping.hasRefPosInfo = true;
        mapping.refPosInfo = posInfo1;
    }
    else  // Different position.
    {
        if (posInfo1.timestamp() == posInfo2.timestamp())
        {
            qWarning() << "Same reference times";
        }
        else
        {
            double azimDeg = posInfo1.coordinate().azimuthTo(posInfo2.coordinate());
            double azimRad = qDegreesToRadians(azimDeg);
            double distTotal = posInfo1.coordinate().distanceTo(posInfo2.coordinate());

            double dx = distTotal * qSin(azimRad);
            double dy = distTotal * qCos(azimRad);

            double vx = dx / dtTotal;
            double vy = dy / dtTotal;
            double gs = qSqrt(qPow(vx, 2) + qPow(vy, 2));

            double dtInterp = posInfo1.timestamp().msecsTo(mapping.tod) / 1000.0;

            Q_ASSERT(dtInterp >= 0);

            double distInterp = distTotal * dtInterp / dtTotal;

            QGeoCoordinate posInterp = posInfo1.coordinate().atDistanceAndAzimuth(distInterp, azimDeg);

            // Calculate altitude.
            double altitude = qSNaN();

            if (!qIsNaN(posInfo1.coordinate().altitude()) && qIsNaN(posInfo2.coordinate().altitude()))
            {
                altitude = posInfo1.coordinate().altitude();
            }
            else if (qIsNaN(posInfo1.coordinate().altitude()) && !qIsNaN(posInfo2.coordinate().altitude()))
            {
                altitude = posInfo2.coordinate().altitude();
            }
            else if (!qIsNaN(posInfo1.coordinate().altitude()) && !qIsNaN(posInfo2.coordinate().altitude()))
            {
                double vAlt = (posInfo2.coordinate().altitude() - posInfo1.coordinate().altitude()) / dtTotal;
                altitude = posInfo1.coordinate().altitude() + vAlt * dtInterp;
            }

            posInterp.setAltitude(altitude);

            QGeoPositionInfo posInfoInterp = QGeoPositionInfo(posInterp, mapping.tod);
            posInfoInterp.setAttribute(QGeoPositionInfo::Attribute::Direction, azimDeg);
            posInfoInterp.setAttribute(QGeoPositionInfo::Attribute::GroundSpeed, gs);

            mapping.hasRefPosInfo = true;
            mapping.refPosInfo = posInfoInterp;
        }
    }
}

void Evaluator::setRefData(const QMultiMap<QDateTime, QGeoPositionInfo> &refData)
{
    m_refData = refData;
}
