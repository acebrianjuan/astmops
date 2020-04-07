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
