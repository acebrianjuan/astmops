# Copyright (C) 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
#
# ASTMOPS is a surveillance performance analyzer
#      for Air Traffic Control
#
# This file is part of ASTMOPS.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

find_package(Qt5 REQUIRED COMPONENTS Core)
if(NOT Qt5_FOUND)
     message(FATAL_ERROR "Fatal error: Qt5 required.")
endif(NOT Qt5_FOUND)

set(CMAKE_AUTOMOC ON)

set(QT5_LIBRARIES
    Qt5::Core
)

set(SOURCES
    main.cpp
)

add_executable(app ${SOURCES})
target_link_libraries(app PUBLIC ${QT5_LIBRARIES} lib)
#install(TARGETS app RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
