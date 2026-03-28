#*****************************************************************
# Copyright (C) 2017 - Robert Valler - All rights reserved.
#
# This file is part of the project: StarterApp
#
# This project can not be copied and/or distributed
# without the express permission of the copyright holder
#****************************************************************/
cmake_minimum_required(VERSION 3.5.0)
project (network_demo_client)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set (CMAKE_CXX_STANDARD 20)
set (Wall)
set (W1)
set (Wfloat-conversion)

#PACKAGES
find_package(Threads REQUIRED)
find_package(protobuf 6.32.1 REQUIRED)

#EXTERNAL INCLUDES
include_directories(${ProtobufIncludePath})
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/network)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/common)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/serialise)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/network/udp)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/network/tcpip)

#INCLUDES
include_directories(${CMAKE_SOURCE_DIR}/messages/src)

#SOURCE FILES
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

find_library(MY_NET_LIB NAMES net_stack PATHS ${CMAKE_SOURCE_DIR}/external/NetStack/build)

target_link_libraries(  ${PROJECT_NAME}
                        ${CMAKE_THREAD_LIBS_INIT}
                        protobuf::protobuf
                        messages
                        ${MY_NET_LIB}
)
