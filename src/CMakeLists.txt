 #*************************************************************************
 #   CMakeLists.txt  --  This file is part of edp.                        *
 #                                                                        *
 #   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
 #                                                                        *
 #   edp is free software: you can redistribute it and/or modify          *
 #   it under the terms of the GNU General Public License as published    *
 #   by the Free Software Foundation, either version 3 of the License,    *
 #   or (at your option) any later version.                               *
 #                                                                        *
 #   edp is distributed in the hope that it will be useful,               *
 #   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 #   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 #   See the GNU General Public License for more details.                 *
 #                                                                        *
 #   You should have received a copy of the GNU General Public License    *
 #   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 #                                                                        *
 #*************************************************************************/

# set minimum cmake requirements
cmake_minimum_required(VERSION 2.8.12)
project (edp)

# add custom directory to look for .cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

# prepare configuration file
SET(VERSION_MAJOR "2")
SET(VERSION_MINOR "0")
SET(VERSION_MICRO "1")
configure_file(config.h.in config.h @ONLY)
if(EXISTS "${CMAKE_BINARY_DIR}/config.h")
    MESSAGE("[USER] Creating ${CMAKE_SOURCE_DIR}/config.h from ${CMAKE_BINARY_DIR}/config.h.in")
else()
    MESSAGE("[USER] Could not find configuration file in ${CMAKE_BINARY_DIR}!")
endif()

# Enable release build
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# add OS specific
if(APPLE)
    add_definitions(-D_APPLE)
    SET(BOOST_INCLUDEDIR "/usr/local/include")
    SET(BOOST_LIBRARYDIR "/usr/local/lib")
else()
    SET(BOOST_INCLUDEDIR "/usr/include")
    SET(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu")
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    option(HAS_OPENMP "OpenMP enabled" ON)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# set Boost
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

# Include libraries
find_package(PkgConfig REQUIRED)
find_package(Boost COMPONENTS regex iostreams filesystem system REQUIRED)
find_package(CPPUNIT REQUIRED) # for unit tests
pkg_check_modules(TCLAP tclap REQUIRED)
pkg_check_modules(CAIRO cairo REQUIRED)
pkg_check_modules(EIGEN eigen3 REQUIRED)

# Set include folders
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${EIGEN_INCLUDE_DIRS}
                    ${TCLAP_INCLUDE_DIR}
                    ${CAIRO_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIRS}
                    ${CPPUNIT_INCLUDE_DIR})

# Add sources
file(GLOB SOURCES "*.cpp")
add_executable(edp ${SOURCES})
add_library(edpsources STATIC ${SOURCES})

# Set C++14
add_definitions(-std=c++14)

# Link libraries
if(UNIX AND NOT APPLE)
    SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath=\$ORIGIN/lib")
endif()
if(APPLE)
    SET(CMAKE_MACOSX_RPATH TRUE)
    SET_TARGET_PROPERTIES(edp PROPERTIES INSTALL_RPATH "@executable_path/lib")
    target_link_libraries(edp ${Boost_LIBRARIES} ${CAIRO_LIBRARIES})
else()
    target_link_libraries(edp ${Boost_LIBRARIES} ${CAIRO_LIBRARIES})
endif()

# add Wno-literal-suffix to suppress warning messages
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")


if(NOT DISABLE_TEST)
    message("[USER] Performing unit tests")
    enable_testing ()
    add_subdirectory("test")
else()
    message("[USER] Testing routine disabled")
endif()


###
# Installing
##
install (TARGETS edp DESTINATION bin)
