cmake_minimum_required(VERSION 3.5)
project(cppBoostPlay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Note - BOOST_ROOT of "/Users/slieb/dev/3rd" set in CMAKE options

# list boost components needed here
set (BOOST_COMPONENTS_NEEDED timer chrono system)

# when using dynamic libraries, run config needs to include DYLD_LIBRARY_PATH
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREAD ON)

find_package(Boost 1.64.0 REQUIRED COMPONENTS ${BOOST_COMPONENTS_NEEDED})

include_directories(${Boost_INCLUDE_DIR})

if (NOT Boost_FOUND)
    message (WARNING "Could not find boost!")
endif()

if (Boost_FOUND)
    message (WARNING "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message (WARNING "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message (WARNING "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})

endif()

set(SOURCE_HOLIDAY_FILES playHoliday.cpp)
add_executable(playHoliday ${SOURCE_HOLIDAY_FILES})
set(SOURCE_JSON_FILES playJson.cpp)
add_executable(playJson ${SOURCE_JSON_FILES})
set(SOURCE_XML_FILES playXml.cpp)
add_executable(playXml ${SOURCE_XML_FILES})
