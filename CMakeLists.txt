# Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
# Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
# OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Project configuration
cmake_minimum_required (VERSION 2.8)

project(kindr)

if(NOT ANDROID_NREAL)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

    find_package(Eigen REQUIRED)
    include_directories(include)
    include_directories(${EIGEN_INCLUDE_DIR})

    # Ensure Eigen version is at least 3.2
    if(${EIGEN_WORLD_VERSION} LESS "3" OR (${EIGEN_WORLD_VERSION} EQUAL "3" AND
       ${EIGEN_MAJOR_VERSION} LESS "2"))
      message(FATAL_ERROR "Eigen must be of version 3.2 or higher. Detected version
              ${EIGEN_WORLD_VERSION}.${EIGEN_MAJOR_VERSION}.${EIGEN_MINOR_VERSION}")
    else()
     message(STATUS "Using Eigen of version ${EIGEN_WORLD_VERSION}.${EIGEN_MAJOR_VERSION}.${EIGEN_MINOR_VERSION} from ${EIGEN_INCLUDE_DIR}")
    endif()

    # Noisily default to Release build
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
      message(STATUS "Setting build type to 'Release' as none was specified.")
      set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()

    add_definitions(-std=c++11)

    # Don't build tests if not specified.
    if(NOT BUILD_TEST)
      message(STATUS "Setting build-tests to false as not specified.")
      set(BUILD_TEST false CACHE BOOL "Choose whether to build tests." FORCE)
      set_property(CACHE BUILD_TEST PROPERTY STRINGS
        "True" "False")
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gtest/" AND BUILD_TEST)
        message(STATUS "Building GTests!")
        option(BUILD_GTEST "build gtest" ON)
        add_subdirectory(gtest gtest)
        enable_testing()
    endif()

    # Add CMake module path
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


    install(DIRECTORY include/kindr DESTINATION include)

    # Add unit tests
    if(BUILD_GTEST)
        add_subdirectory(test)
    endif()

    # Add Doxygen documentation
    add_subdirectory(doc/doxygen)

    # Install catkin package.xml
    #install(FILES package.xml DESTINATION share/kindr)


    #=============================================
    # to allow find_package() on kindr
    #=============================================
    #
    # the following case be used in an external project requiring kindr:
    #  ...
    #  find_package(kindr)
    #  include_directories(${kindr_INCLUDE_DIRS})
    #  ...

    # NOTE: the following will support find_package for 1) local build (make) and 2) for installed files (make install)

    # 1- local build #

    # Register the local build in case one doesn't use "make install"
    export(PACKAGE kindr)

    # Create variable for the local build tree
    get_property(kindr_include_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

    # Configure config file for local build tree
    configure_file(kindrConfig.cmake.in
      "${PROJECT_BINARY_DIR}/kindrConfig.cmake" @ONLY)

    # 2- installation build #

    # Change the include location for the case of an install location
    set(kindr_include_dirs ${CMAKE_INSTALL_PREFIX}/include ${EIGEN_INCLUDE_DIR})


    # We put the generated file for installation in a different repository (i.e., ./CMakeFiles/)
    configure_file(kindrConfig.cmake.in
      "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/kindrConfig.cmake" @ONLY)

    install(FILES
      "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/kindrConfig.cmake"
      DESTINATION share/kindr/cmake COMPONENT dev)



    #=============================================
    # Add uninstall target
    #=============================================
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()