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

install(FILES astmops.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/
)

install(FILES icon.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/128x128/
)
