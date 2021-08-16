/*!
 * \file kmlreadertest.cpp
 * \brief Implements unit tests for the KmlReader class.
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

#include "kmlreader.h"
#include <GeographicLib/Geoid.hpp>
#include <QObject>
#include <QtTest>

using SmrMap = QHash<Sic, QGeoCoordinate>;

class KmlReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();
};

void KmlReaderTest::test_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addColumn<QGeoCoordinate>("arpCoordinates");
    QTest::addColumn<QHash<Sic, QGeoCoordinate>>("smrCoordinates");
    QTest::addColumn<KmlReader::Collection>("runwayCoordinates");
    QTest::addColumn<KmlReader::Collection>("taxiwayCoordinates");
    QTest::addColumn<KmlReader::Collection>("apronCoordinates");
    QTest::addColumn<KmlReader::Collection>("standCoordinates");
    QTest::addColumn<KmlReader::Collection>("airborne1Coordinates");
    QTest::addColumn<KmlReader::Collection>("airborne2Coordinates");


    GeographicLib::Geoid geoid = GeographicLib::Geoid("egm96-5");

    double lat_lemd = 40.4722303900746;
    double lon_lemd = -3.56094539038218;
    double H_lemd = 609.02099999999996;
    double N_lemd = geoid(lat_lemd, lon_lemd);
    double h_lemd = H_lemd + N_lemd;


    // ARP.
    QGeoCoordinate arpCoordinates(lat_lemd, lon_lemd, h_lemd);


    // SMR.
    SmrMap smrCoordinates;
    smrCoordinates.insert(3, {40.46814966666667, -3.568730277777777, h_lemd});
    smrCoordinates.insert(4, {40.49188877777778, -3.569062527777778, h_lemd});


    // RUNWAY.
    KmlReader::Collection runwayCol;

    QVector<QGeoCoordinate> runwayEl;
    runwayEl.append({40.4632489601465, -3.55361404980395, h_lemd});
    runwayEl.append({40.4646102402898, -3.55499599018929, h_lemd});
    runwayEl.append({40.4659714901115, -3.55637797998195, h_lemd});
    runwayEl.append({40.4673327396818, -3.5577600299617, h_lemd});
    runwayEl.append({40.4686939602959, -3.559142140128631, h_lemd});
    runwayEl.append({40.4700551697219, -3.560524299702871, h_lemd});
    runwayEl.append({40.471416359758, -3.56190652036254, h_lemd});
    runwayEl.append({40.4727775399706, -3.56328879042959, h_lemd});
    runwayEl.append({40.4741386898579, -3.56467111978541, h_lemd});
    runwayEl.append({40.4754998299199, -3.56605351022667, h_lemd});
    runwayEl.append({40.4768609601554, -3.5674359500753, h_lemd});
    runwayEl.append({40.4782220600639, -3.56881844022957, h_lemd});
    runwayEl.append({40.479583150144, -3.57020099955411, h_lemd});
    runwayEl.append({40.4809442297113, -3.57158361008262, h_lemd});
    runwayEl.append({40.4823052803163, -3.57296627001848, h_lemd});
    runwayEl.append({40.4836663197234, -3.574348990141469, h_lemd});
    runwayEl.append({40.4850273499812, -3.57573176955327, h_lemd});
    runwayEl.append({40.4846961501566, -3.57629089985073, h_lemd});
    runwayEl.append({40.4833351398299, -3.57490812043893, h_lemd});
    runwayEl.append({40.4819740998571, -3.57352540031594, h_lemd});
    runwayEl.append({40.4806130500534, -3.57214272960032, h_lemd});
    runwayEl.append({40.4792519897367, -3.57076011008863, h_lemd});
    runwayEl.append({40.4778908997756, -3.56937755974723, h_lemd});
    runwayEl.append({40.4765297999867, -3.567995059711489, h_lemd});
    runwayEl.append({40.4751686896878, -3.56661260998143, h_lemd});
    runwayEl.append({40.4738075497464, -3.56523022043846, h_lemd});
    runwayEl.append({40.4724463999802, -3.56384789018431, h_lemd});
    runwayEl.append({40.4710852301396, -3.56246561023583, h_lemd});
    runwayEl.append({40.4697240497931, -3.56108338957613, h_lemd});
    runwayEl.append({40.4683628500573, -3.55970122012042, h_lemd});
    runwayEl.append({40.4670016302498, -3.55831910995353, h_lemd});
    runwayEl.append({40.4656403896876, -3.55693705997373, h_lemd});
    runwayEl.append({40.4642791399901, -3.55555506029962, h_lemd});
    runwayEl.append({40.4629178702232, -3.55417311003284, h_lemd});
    runwayEl.append({40.4632489601465, -3.55361404980395, h_lemd});

    runwayCol << runwayEl;


    // TAXIWAY.
    KmlReader::Collection taxiwayCol;

    QVector<QGeoCoordinate> taxiwayEl;
    taxiwayEl.append({40.4955384683338, -3.56518013217484, h_lemd});
    taxiwayEl.append({40.4955353860524, -3.56599870131626, h_lemd});
    taxiwayEl.append({40.4944487698811, -3.56599262062011, h_lemd});
    taxiwayEl.append({40.4937298190636, -3.56598860604908, h_lemd});
    taxiwayEl.append({40.4935295627678, -3.56598796644861, h_lemd});
    taxiwayEl.append({40.4933783882938, -3.565987283728981, h_lemd});
    taxiwayEl.append({40.4933326690325, -3.56598697470855, h_lemd});
    taxiwayEl.append({40.4931479860996, -3.56598602429095, h_lemd});
    taxiwayEl.append({40.4927976712947, -3.565984224965431, h_lemd});
    taxiwayEl.append({40.4926035490399, -3.56598320268267, h_lemd});
    taxiwayEl.append({40.4925223739166, -3.56598249570852, h_lemd});
    taxiwayEl.append({40.4923167409442, -3.56598123716882, h_lemd});
    taxiwayEl.append({40.4921693587141, -3.56598030920912, h_lemd});
    taxiwayEl.append({40.4921638797384, -3.5659803370569, h_lemd});
    taxiwayEl.append({40.4921197698684, -3.56598010708817, h_lemd});
    taxiwayEl.append({40.4919716480201, -3.56597899587216, h_lemd});
    taxiwayEl.append({40.4918176663165, -3.565978276321621, h_lemd});
    taxiwayEl.append({40.4917731756943, -3.56597792687698, h_lemd});
    taxiwayEl.append({40.4916112223115, -3.56597701508696, h_lemd});
    taxiwayEl.append({40.4914561303107, -3.565976420402249, h_lemd});
    taxiwayEl.append({40.4914145873349, -3.56597621558638, h_lemd});
    taxiwayEl.append({40.4912556609109, -3.5659750531664, h_lemd});
    taxiwayEl.append({40.4910902043241, -3.56597421144499, h_lemd});
    taxiwayEl.append({40.4910596458987, -3.56597392039083, h_lemd});
    taxiwayEl.append({40.4908504798324, -3.56597281995458, h_lemd});
    taxiwayEl.append({40.4906133457855, -3.56597133503945, h_lemd});
    taxiwayEl.append({40.4903767888729, -3.56597013758515, h_lemd});
    taxiwayEl.append({40.4901772897864, -3.56596912877711, h_lemd});
    taxiwayEl.append({40.4901366232281, -3.56596903355569, h_lemd});
    taxiwayEl.append({40.4899443809201, -3.56596808134146, h_lemd});
    taxiwayEl.append({40.4895740920342, -3.565966262253041, h_lemd});
    taxiwayEl.append({40.4892550228892, -3.56596548970187, h_lemd});
    taxiwayEl.append({40.4892467405314, -3.565967359096, h_lemd});
    taxiwayEl.append({40.4892386508333, -3.56596993995579, h_lemd});
    taxiwayEl.append({40.4892308152825, -3.56597321162005, h_lemd});
    taxiwayEl.append({40.4892232933167, -3.56597714983426, h_lemd});
    taxiwayEl.append({40.4892125494067, -3.56598603866401, h_lemd});
    taxiwayEl.append({40.4892025734052, -3.56599578179159, h_lemd});
    taxiwayEl.append({40.4891934336322, -3.56600631274167, h_lemd});
    taxiwayEl.append({40.4891851922585, -3.56601756144565, h_lemd});
    taxiwayEl.append({40.4891779046233, -3.56602944885181, h_lemd});
    taxiwayEl.append({40.489171619917, -3.56604189680669, h_lemd});
    taxiwayEl.append({40.4891613781467, -3.56606901694513, h_lemd});
    taxiwayEl.append({40.4891533663225, -3.56609687819364, h_lemd});
    taxiwayEl.append({40.4891476370513, -3.56612529639765, h_lemd});
    taxiwayEl.append({40.4891459126676, -3.56614199248554, h_lemd});
    taxiwayEl.append({40.4891446439748, -3.56615872989592, h_lemd});
    taxiwayEl.append({40.489143830973, -3.56617549605238, h_lemd});
    taxiwayEl.append({40.4891556980662, -3.56497059474494, h_lemd});
    taxiwayEl.append({40.4951117342801, -3.56502278865956, h_lemd});
    taxiwayEl.append({40.4951108578191, -3.56517520671215, h_lemd});
    taxiwayEl.append({40.4955384683338, -3.56518013217484, h_lemd});

    taxiwayCol << taxiwayEl;


    // APRON.
    KmlReader::Collection apronCol;


    // STAND.
    KmlReader::Collection standCol;

    QVector<QGeoCoordinate> standEl;
    standEl.append({40.4987262748599, -3.568028714195321, h_lemd});
    standEl.append({40.4997535120862, -3.56803160138062, h_lemd});
    standEl.append({40.4997521991933, -3.56850675716531, h_lemd});
    standEl.append({40.4997497981431, -3.56919973913505, h_lemd});
    standEl.append({40.4997495597458, -3.56922505635471, h_lemd});
    standEl.append({40.4997471873851, -3.5692502639799, h_lemd});
    standEl.append({40.4997426974549, -3.56927518055092, h_lemd});
    standEl.append({40.4997361227429, -3.56929963089636, h_lemd});
    standEl.append({40.4997303547562, -3.56931521846316, h_lemd});
    standEl.append({40.4997233237408, -3.5693302787189, h_lemd});
    standEl.append({40.4997150775127, -3.5693447092556, h_lemd});
    standEl.append({40.4997056720843, -3.56935841215696, h_lemd});
    standEl.append({40.4996951709824, -3.56937129579477, h_lemd});
    standEl.append({40.4996836459308, -3.56938327123581, h_lemd});
    standEl.append({40.4996755916453, -3.569391083883831, h_lemd});
    standEl.append({40.4996668856932, -3.56939816440492, h_lemd});
    standEl.append({40.4996575950169, -3.56940445710349, h_lemd});
    standEl.append({40.4996477906571, -3.56940991616545, h_lemd});
    standEl.append({40.4996375470704, -3.56941449757341, h_lemd});
    standEl.append({40.4996269428116, -3.56941816719135, h_lemd});
    standEl.append({40.499616059168, -3.5694208980698, h_lemd});
    standEl.append({40.4996049781101, -3.56942266775091, h_lemd});
    standEl.append({40.499205157126, -3.56942063306679, h_lemd});
    standEl.append({40.4991975898281, -3.56942056928643, h_lemd});
    standEl.append({40.4989364861029, -3.56941906281169, h_lemd});
    standEl.append({40.4986773951339, -3.56941784469616, h_lemd});
    standEl.append({40.4984714900238, -3.56941687182071, h_lemd});
    standEl.append({40.4981213409997, -3.56941505183392, h_lemd});
    standEl.append({40.4979213047917, -3.56941364686883, h_lemd});
    standEl.append({40.4977655668111, -3.56941278987606, h_lemd});
    standEl.append({40.4977212051413, -3.56941259943321, h_lemd});
    standEl.append({40.4975674470295, -3.56941168674489, h_lemd});
    standEl.append({40.4972156278584, -3.56940936998978, h_lemd});
    standEl.append({40.4970686081924, -3.56940856959083, h_lemd});
    standEl.append({40.4970189232327, -3.56940816265401, h_lemd});
    standEl.append({40.4965120140389, -3.56940526828216, h_lemd});
    standEl.append({40.4963161311951, -3.56940424510107, h_lemd});
    standEl.append({40.4962949721768, -3.5694041238285, h_lemd});
    standEl.append({40.4960143498436, -3.56940299374788, h_lemd});
    standEl.append({40.4960107456766, -3.5694029775782, h_lemd});
    standEl.append({40.4957742398982, -3.56940186726052, h_lemd});
    standEl.append({40.4957278022527, -3.56940164088505, h_lemd});
    standEl.append({40.4954121062933, -3.56939980472862, h_lemd});
    standEl.append({40.4952109389369, -3.56939863691876, h_lemd});
    standEl.append({40.4950560977688, -3.56939753827914, h_lemd});
    standEl.append({40.4950126756676, -3.56939731370033, h_lemd});
    standEl.append({40.4948275731306, -3.56939632914678, h_lemd});
    standEl.append({40.4942957277175, -3.5693933512316, h_lemd});
    standEl.append({40.4942265082801, -3.56939264515582, h_lemd});
    standEl.append({40.4940989646616, -3.56939215377734, h_lemd});
    standEl.append({40.4935637892663, -3.56938890097772, h_lemd});
    standEl.append({40.4933922562723, -3.56938782120274, h_lemd});
    standEl.append({40.4932216020582, -3.56938702799033, h_lemd});
    standEl.append({40.4931742417547, -3.56938671178336, h_lemd});
    standEl.append({40.4930252518941, -3.56938608475928, h_lemd});
    standEl.append({40.4924347462366, -3.569382656788159, h_lemd});
    standEl.append({40.4924209163173, -3.56938258761788, h_lemd});
    standEl.append({40.492459357119, -3.56921927389923, h_lemd});
    standEl.append({40.4924634232936, -3.56879276727705, h_lemd});
    standEl.append({40.492464044287, -3.56848409087408, h_lemd});
    standEl.append({40.4924640914251, -3.568480354780819, h_lemd});
    standEl.append({40.4924642068793, -3.56847662048419, h_lemd});
    standEl.append({40.4924643913328, -3.56847288888248, h_lemd});
    standEl.append({40.4924682757862, -3.56799150328128, h_lemd});
    standEl.append({40.4987262748599, -3.568028714195321, h_lemd});

    standCol << standEl;


    // AIRBORNE1.
    KmlReader::Collection airborne1Col;

    QVector<QGeoCoordinate> airborne1El;
    airborne1El.append({40.4737353699986, -3.535915761470541, h_lemd});
    airborne1El.append({40.4734043700575, -3.53647499138694, h_lemd});
    airborne1El.append({40.4359454555003, -3.51065416077014, h_lemd});
    airborne1El.append({40.4452787156093, -3.494880933245279, h_lemd});
    airborne1El.append({40.4737353699986, -3.535915761470541, h_lemd});

    airborne1Col << airborne1El;


    // AIRBORNE2.
    KmlReader::Collection airborne2Col;

    QVector<QGeoCoordinate> airborne2El;
    airborne2El.append({40.4452787156093, -3.494880933245279, h_lemd});
    airborne2El.append({40.4359454555003, -3.51065416077014, h_lemd});
    airborne2El.append({40.3984805851604, -3.48486200830645, h_lemd});
    airborne2El.append({40.4168074078382, -3.45388076836991, h_lemd});
    airborne2El.append({40.4452787156093, -3.494880933245279, h_lemd});

    airborne2Col << airborne2El;


    QTest::newRow("LEMD") << "lemd_test.kml"
                          << arpCoordinates
                          << smrCoordinates
                          << runwayCol
                          << taxiwayCol
                          << apronCol
                          << standCol
                          << airborne1Col
                          << airborne2Col;
}

void KmlReaderTest::test()
{
    KmlReader reader;

    QFETCH(QString, fileName);
    QFile file(QFINDTESTDATA(fileName));
    QVERIFY(file.open(QIODevice::ReadOnly));

    QFETCH(QGeoCoordinate, arpCoordinates);
    QFETCH(SmrMap, smrCoordinates);
    QFETCH(KmlReader::Collection, runwayCoordinates);
    QFETCH(KmlReader::Collection, taxiwayCoordinates);
    QFETCH(KmlReader::Collection, apronCoordinates);
    QFETCH(KmlReader::Collection, standCoordinates);
    QFETCH(KmlReader::Collection, airborne1Coordinates);
    QFETCH(KmlReader::Collection, airborne2Coordinates);

    reader.read(&file);
    //Aerodrome aerodrome = reader.makeAerodrome();

    // Check coordinates of each collection.
    QCOMPARE(reader.arp_, arpCoordinates);
    QCOMPARE(reader.smr_, smrCoordinates);
    QCOMPARE(reader.runwayElements_.value(QLatin1String("14R/32L")), runwayCoordinates);
    QCOMPARE(reader.taxiwayElements_.value({}), taxiwayCoordinates);
    QCOMPARE(reader.apronLaneElements_.value({}), apronCoordinates);
    QCOMPARE(reader.standElements_.value({}), standCoordinates);
    QCOMPARE(reader.airborne1Elements_.value({}), airborne1Coordinates);
    QCOMPARE(reader.airborne2Elements_.value({}), airborne2Coordinates);
}

QTEST_APPLESS_MAIN(KmlReaderTest)
#include "kmlreadertest.moc"
