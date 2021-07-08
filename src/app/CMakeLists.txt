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
if(NOT Qt5_FOUND)
    message(FATAL_ERROR "Fatal error: Qt5 required.")
endif()

set(CMAKE_AUTOMOC ON)

set(QT5_LIBRARIES
    Qt5::Core
    Qt5::Gui
    Qt5::Positioning
)

set(SOURCES
    main.cpp
)

add_executable(astmops ${SOURCES})
target_link_libraries(astmops PUBLIC ${QT5_LIBRARIES} lib)
install(TARGETS astmops RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
