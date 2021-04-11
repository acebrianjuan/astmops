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
#include "aerodrome.h"
#include "geofunctions.h"
#include <QMetaEnum>
#include <QtMath>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

Evaluator::Evaluator(QObject *parent) : QObject(parent)
{
    /*
    QMetaEnum e = QMetaEnum::fromType<Aerodrome::Area>();
    for (int i = 0; i < e.keyCount(); ++i)
    {
        m_cat010MlatPosAccuracyCounters.insert(static_cast<Aerodrome::Area>(e.value(i)), Counters::BasicCounter());
        m_cat010MlatPosAccuracyErrors.insert(static_cast<Aerodrome::Area>(e.value(i)), QVector<double>());
    }
    */
}

double Evaluator::evalPosAccDgps()
{
    // TODO: These coordinates should be read from the KML file.
    QGeoCoordinate leblArp(41.297076579982225, 2.0784629201158662, 4.3200000000000003);
    QGeoCoordinate leblSmr(41.2961573950623, 2.086652827750635, 4.3200000000000003);

    calculateTestDataMappings();

    QDateTime tod;
    QGeoPositionInfo testPos;
    QGeoPositionInfo refPos;

    double tdiffMax = 2.0;  // seconds.
    for (QMultiMap<QDateTime, Asterix::Record>::const_iterator it = m_testData.constBegin(); it != m_testData.constEnd(); ++it)
    {
        Asterix::Record testRec = it.value();

        tod = it.key();

        if (!hasRefDataForTime(tod, tdiffMax))
        {
            //qWarning() << tod.toString(Qt::ISODateWithMs) << "Ref data beyond time limit of" << tdiffMax << "seconds";
            continue;
        }

        double xTest = Asterix::getElementValue(testRec, QLatin1String("I042"), QLatin1String("X")).toDouble();
        double yTest = Asterix::getElementValue(testRec, QLatin1String("I042"), QLatin1String("Y")).toDouble();

        bool gbOk;
        bool gndBit = Asterix::getElementValue(testRec, QLatin1String("I020"), QLatin1String("GBS")).toUInt(&gbOk);

        TestDataMapping mapping = m_testDataMappings.value(tod);

        if (!mapping.hasRefPosInfo)
        {
            continue;
        }

        QGeoPositionInfo refPosInfo = mapping.refPosInfo;

        QVector3D cartRef = geoToLocalEnu(refPosInfo.coordinate(), leblArp);

        Aerodrome::NamedArea namedArea;
        if (gbOk)
        {
            namedArea = m_locatePoint(cartRef, gndBit);
        }
        else
        {
            continue;
        }

        Aerodrome::Area area = namedArea.area_;
        if (area == Aerodrome::Area::None)
        {
            continue;
        }

        double errX = cartRef.x() - xTest;
        double errY = cartRef.y() - yTest;

        double dist = qSqrt(qPow(errX, 2) + qPow(errY, 2));

        double minDist;
        switch (area)
        {
        case Aerodrome::Area::Runway:
            minDist = 7.5;
            break;
        case Aerodrome::Area::Taxiway:
            minDist = 7.5;
            break;
        case Aerodrome::Area::Apron:
            minDist = 20;
            break;
        case Aerodrome::Area::Stand:
            minDist = 20;
            break;
        case Aerodrome::Area::Airborne1:
            minDist = 20;
            break;
        case Aerodrome::Area::Airborne2:
            minDist = 40;
            break;
        default:
            continue;
        }

        ++m_cat010MlatPosAccuracyCounters[namedArea].countTotal;
        if (dist <= minDist)
        {
            ++m_cat010MlatPosAccuracyCounters[namedArea].countValid;
        }

        m_cat010MlatPosAccuracyErrors[namedArea].append(dist);

        /*
        int ncheck = 10;
        QVector<double> errVec = m_cat010MlatPosAccuracyErrors.value(area);
        if (errVec.size() > ncheck)
        {
            double lastErr = errVec.constLast();

            bool found = false;
            auto itErr = errVec.crbegin();
            while (!found && qAbs(*itErr - lastErr) < 1e-4)
            {
                ++itErr;
                --ncheck;
                if (ncheck == 0)
                {
                    qDebug() << "FOUND ERROR REPETITION!";
                    found = true;
                }
            }

            if (found)
            {
                int i = 10;

                auto itMapping = m_testDataMappings.find(tod);
                while (i > 0)
                {
                    qDebug()
                        << itMapping.key().toString(Qt::ISODateWithMs)
                        << itMapping.value().tod.toString(Qt::ISODateWithMs)
                        << itMapping.value().refPosInfo1.timestamp().toString(Qt::ISODateWithMs)
                        << itMapping.value().refPosInfo2.timestamp().toString(Qt::ISODateWithMs)
                        << "-->"
                        << itMapping.value().refPosInfo;
                    --itMapping;
                    --i;
                }
            }
        }
        */


        /*
        qDebug() << tod
                 << "REF_X:" << cartRef.x() << "REF_Y:" << cartRef.y()
                 << "TST_X:" << xTest << "TST_Y:" << yTest
                 << "ERR: " << dist;
        */

        //std::cout << tod.toString(Qt::ISODateWithMs).toStdString() << " ERR: " << dist << '\n';
    }

    /*
    for (QHash<Aerodrome::Area, Counters::BasicCounter>::const_iterator it = m_cat010MlatPosAccuracyCounters.constBegin(); it != m_cat010MlatPosAccuracyCounters.constEnd(); ++it)
    {
        double result = it.value().countTotal == 0 ? qSNaN()
                                                   : it.value().countValid / static_cast<double>(it.value().countTotal);

        qDebug() << it.key() << "\t\t" << it.value().countValid << "/" << it.value().countTotal << "=" << result;
    }
    */


    printPosAccResuls();


    return 0.0;
}

void Evaluator::setTestData(const QMultiMap<QDateTime, Asterix::Record> &testData)
{
    m_testData = testData;
}

void Evaluator::setLocatePointCallback(const std::function<Aerodrome::NamedArea(const QVector3D &, const bool)> &callback)
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

    for (QMultiMap<QDateTime, Asterix::Record>::const_iterator it = m_testData.constBegin(); it != m_testData.constEnd(); ++it)
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

bool Evaluator::hasRefDataForTime(const QDateTime &tod, double tdiffMax) const
{
    Q_ASSERT(m_testDataMappings.contains(tod));

    TestDataMapping mapping = m_testDataMappings.value(tod);

    if (!mapping.hasRef1 && !mapping.hasRef2)  // No ref data.
    {
        return false;
    }

    if (mapping.hasRef1 && mapping.hasRef2)  // Interpolated.
    {
        Q_ASSERT(mapping.refPosInfo1.timestamp() <= tod);
        Q_ASSERT(mapping.refPosInfo2.timestamp() >= tod);

        if (mapping.refPosInfo1.timestamp().msecsTo(tod) / 1000.0 > tdiffMax)  // Lower too far.
        {
            return false;
        }

        if (tod.msecsTo(mapping.refPosInfo2.timestamp()) / 1000.0 > tdiffMax)  // Upper too far.
        {
            return false;
        }

        return true;
    }

    return false;
}

double Evaluator::percentile(QVector<double> vec, const double percent)
{
    //Q_ASSERT(!vec.isEmpty() && percent <= 100);

    if (vec.isEmpty() || percent < 0 || percent > 100)
    {
        return qSNaN();  // TODO: Should throw a warning here.
    }

    if (vec.size() == 1)
    {
        return vec.first();
    }

    std::sort(vec.begin(), vec.end());

    if (percent == 0)
    {
        return vec.first();
    }
    if (percent == 100)
    {
        return vec.last();
    }

    const int numRecords = vec.size();
    double rank = percent / 100.0 * numRecords;

    if (rank >= numRecords - 1)
    {
        return vec.last();
    }

    double intPart;
    double fractPart = std::modf(rank, &intPart);

    int idx = static_cast<int>(intPart) - 1;

    if (fractPart != 0)
    {
        return vec.at(idx + 1);
    }
    else
    {
        return (vec.at(idx) + vec.at(idx + 1)) / 2.0;
    }
}

double Evaluator::mean(const QVector<double> &v)
{
    int N = v.size();

    if (N == 0)
    {
        return qSNaN();
    }

    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / N;

    return mean;
}

double Evaluator::stdDev(const QVector<double> &v)
{
    int N = v.size();

    if (N == 0)
    {
        return qSNaN();
    }

    if (N == 1)
    {
        return 0.0;
    }

    double mean = Evaluator::mean(v);

    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / (N - 1));

    return stdev;
}

void Evaluator::printPosAccResuls()
{
    QVector<Aerodrome::Area> areas;
    areas << Aerodrome::Area::Manoeuvering
          << Aerodrome::Area::Apron
          << Aerodrome::Area::Airborne;

    auto printStats = [this](AreaHash<QVector<double>>::iterator it, const QVector<double> &errors) {
        qDebug() << it.key().area_ << it.key().name_
                 << "\t"
                 << "P95:"
                 << "\t" << percentile(errors, 95) << "m"
                 << "\t"
                 << "P99:"
                 << "\t" << percentile(errors, 99) << "m"
                 << "\t"
                 << "Mean:"
                 << "\t" << mean(errors) << "m"
                 << "\t"
                 << "StdDev:"
                 << "\t" << stdDev(errors) << "m"
                 << "\t"
                 << "N:"
                 << "\t" << errors.size();
    };


    for (const Aerodrome::Area area : qAsConst(areas))
    {
        auto subAreas = m_cat010MlatPosAccuracyErrors.findByArea(area);

        QVector<double> errorsTotal;
        for (const auto &it : subAreas)
        {
            QVector<double> errors = it.value();
            errorsTotal.append(errors);

            printStats(it, errors);
        }

        qDebug() << area
                 << "\t"
                 << "P95:"
                 << "\t" << percentile(errorsTotal, 95) << "m"
                 << "\t"
                 << "P99:"
                 << "\t" << percentile(errorsTotal, 99) << "m"
                 << "\t"
                 << "Mean:"
                 << "\t" << mean(errorsTotal) << "m"
                 << "\t"
                 << "StdDev:"
                 << "\t" << stdDev(errorsTotal) << "m"
                 << "\t"
                 << "N:"
                 << "\t" << errorsTotal.size();
    }
}

void Evaluator::setRefData(const QMultiMap<QDateTime, QGeoPositionInfo> &refData)
{
    m_refData = refData;
}
