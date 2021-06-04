# Copyright (C) 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
#
# ASTMOPS is a surveillance performance analyzer
#      for Air Traffic Control
#
# This file is part of ASTMOPS.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

find_package(Qt5 REQUIRED COMPONENTS Core Gui Positioning)
set(CMAKE_AUTOMOC ON)

add_library(lib
    aerodrome.cpp
    aixmreader.cpp
    asterix.cpp
    asterixxmlreader.cpp
    astmops.cpp
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
    )

target_include_directories(lib PUBLIC .)
target_link_libraries(lib Qt5::Core Qt5::Gui Qt5::Positioning)
