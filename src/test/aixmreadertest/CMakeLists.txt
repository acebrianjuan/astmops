# Copyright (C) 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
#
# ASTMOPS is a surveillance performance analyzer
#      for Air Traffic Control
#
# This file is part of ASTMOPS.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

find_package(Qt5 REQUIRED COMPONENTS Core Test)
if(NOT Qt5_FOUND)
     message(FATAL_ERROR "Fatal error: Qt5 required.")
endif(NOT Qt5_FOUND)

set(CMAKE_AUTOMOC ON)

set(QT5_LIBRARIES
    Qt5::Core
    Qt5::Test
)

add_executable(aixmreadertestapp aixmreadertest.cpp)
target_link_libraries(aixmreadertestapp PUBLIC ${QT5_LIBRARIES} lib)
add_test(NAME aixmreadertest COMMAND aixmreadertestapp)
