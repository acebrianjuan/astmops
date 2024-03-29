# Copyright (C) 2020-2021 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
#
# ASTMOPS is a command line tool for evaluating
# the performance of A-SMGCS sensors at airports
#
# This file is part of ASTMOPS.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

find_package(Qt5 REQUIRED COMPONENTS Core Gui Positioning)

# Workaround for bug: https://bugs.launchpad.net/ubuntu/+source/geographiclib/+bug/1805173
if(UNIX AND NOT APPLE)
    set(CMAKE_MODULE_PATH "/usr/share/cmake/geographiclib/")
endif()
find_package(GeographicLib REQUIRED)

set(CMAKE_AUTOMOC ON)

add_library(lib
    aerodrome.cpp
    aixmreader.cpp
    asterix.cpp
    asterixxmlreader.cpp
    astmops.cpp
    config.cpp
    counters.cpp
    dgpscsvreader.cpp
    functions.cpp
    geofunctions.cpp
    kmlreader.cpp
    perfevaluator.cpp
    targetreport.cpp
    targetreportextractor.cpp
    track.cpp
    trackassociator.cpp
    trackextractor.cpp
    trafficperiod.cpp
)

target_include_directories(lib PUBLIC .)
target_link_libraries(lib Qt5::Core Qt5::Gui Qt5::Positioning ${GeographicLib_LIBRARIES} coverage_config)
