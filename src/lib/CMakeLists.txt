find_package(Qt5 REQUIRED COMPONENTS Core Gui)
set(CMAKE_AUTOMOC ON)

add_library(lib asterix.cpp aerodrome.cpp aixmreader.cpp asterixxmlreader.cpp recordcollator.cpp mopsprocessor.cpp)
target_include_directories(lib PUBLIC .)
target_link_libraries(lib Qt5::Core Qt5::Gui)
