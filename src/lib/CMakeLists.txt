find_package(Qt5 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOMOC ON)

add_library(lib asterixxmlreader.cpp mopsprocessor.cpp)
target_include_directories(lib PUBLIC .)
target_link_libraries(lib Qt5::Core)
