#*****************************************************************
# Copyright (C) 2017 - Robert Valler - All rights reserved.
#
# This file is part of the project: StarterApp
#
# This project can not be copied and/or distributed
# without the express permission of the copyright holder
#****************************************************************/
cmake_minimum_required(VERSION 3.5.0)
project (messages)

#PACKAGES
find_package(Threads REQUIRED)
find_package(protobuf 6.32.1 REQUIRED)

#CONFIG
set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL messages)

#EXTERNAL INCLUDES
include_directories(${ProtobufIncludePath})

#PROTOBUF FILES
file(GLOB ProtoFiles
    ${CMAKE_SOURCE_DIR}/messages/*.proto
)

#GENERATE
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

add_library(${PROJECT_NAME}
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${ProtoFiles}
)

target_link_libraries(  ${PROJECT_NAME}
                        ${CMAKE_THREAD_LIBS_INIT}
                        protobuf::protobuf
)

