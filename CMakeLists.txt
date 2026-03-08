#*****************************************************************
# Copyright (C) 2017 - Robert Valler - All rights reserved.
#
# This file is part of the project: StarterApp
#
# This project can not be copied and/or distributed
# without the express permission of the copyright holder
#****************************************************************/

cmake_minimum_required(VERSION 3.5.0)
project (network_test)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set (CMAKE_CXX_STANDARD 20)
set (Wall)
set (W1)
set (Wfloat-conversion)

#PACKAGES
find_package(Threads REQUIRED)
find_package(protobuf 6.32.1 REQUIRED)

#CONFIG
set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL messages)

# PROCESS SUBMODULES
#add_subdirectory(external/NetStack)

#EXTERNAL INCLUDES
include_directories(${ProtobufIncludePath})
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/network)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/common)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/serialise)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/network/udp)
include_directories(${CMAKE_SOURCE_DIR}/external/NetStack/src/network/tcpip)

#INCLUDES
include_directories(${CMAKE_SOURCE_DIR}/messages)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/main)
include_directories(${CMAKE_SOURCE_DIR}/src/network)


#SOURCE FILES
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/messages/*.pb.cc
)

file(GLOB_RECURSE HEADERS
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/messages/*.pb.h
)

#PROTOBUF FILES
file(GLOB ProtoFiles
    ${CMAKE_SOURCE_DIR}/messages/*.proto
)

#CONFIG FILES
file(GLOB APP_CONFIG
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/conanfile.py
    ${CMAKE_SOURCE_DIR}/create.sh
)

#GENERATE
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${ProtoFiles}
    ${APP_CONFIG}
)

find_library(MY_NET_LIB NAMES net_stack PATHS ${CMAKE_SOURCE_DIR}/external/NetStack/build)

target_link_libraries(  ${PROJECT_NAME}
                        ${CMAKE_THREAD_LIBS_INIT}
                        protobuf::protobuf
                        ${MY_NET_LIB}
)

