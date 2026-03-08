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

#EXTERNAL INCLUDES
include_directories(${ProtobufIncludePath})

#INCLUDES
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/main)
include_directories(${CMAKE_SOURCE_DIR}/src/network)

#SOURCE FILES
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/main/main.cpp
)

file(GLOB_RECURSE HEADERS
#    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/messages/*.pb.h
)

#PROTOBUF FILES
file(GLOB ProtoFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/messages/*.proto)

#GENERATE
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${ProtoFiles}
)
target_link_libraries(  ${PROJECT_NAME}
                        ${CMAKE_THREAD_LIBS_INIT}
                        protobuf::protobuf
                        #net_stack
)

