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

add_library(lib asterix.cpp geofunctions.cpp aerodrome.cpp aixmreader.cpp kmlreader.cpp asterixxmlreader.cpp recordcollator.cpp mopsprocessor.cpp counters.cpp)
target_include_directories(lib PUBLIC .)
target_link_libraries(lib Qt5::Core Qt5::Gui Qt5::Positioning)
