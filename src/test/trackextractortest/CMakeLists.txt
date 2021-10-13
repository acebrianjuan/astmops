# Copyright (C) 2020-2021 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
#
# ASTMOPS is a command line tool for evaluating
# the performance of A-SMGCS sensors at airports
#
# This file is part of ASTMOPS.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

find_package(Qt5 REQUIRED COMPONENTS Core Test)
if(NOT Qt5_FOUND)
    message(FATAL_ERROR "Fatal error: Qt5 required.")
endif()

set(CMAKE_AUTOMOC ON)

set(QT5_LIBRARIES
    Qt5::Core
    Qt5::Test
)

add_executable(trackextractortestapp trackextractortest.cpp)
target_link_libraries(trackextractortestapp PUBLIC ${QT5_LIBRARIES} lib)
add_test(NAME trackextractortest COMMAND trackextractortestapp)
