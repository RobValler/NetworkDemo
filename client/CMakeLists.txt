#*****************************************************************
# Copyright (C) 2017 - Robert Valler - All rights reserved.
#
# This file is part of the project: StarterApp
#
# This project can not be copied and/or distributed
# without the express permission of the copyright holder
#****************************************************************/
cmake_minimum_required(VERSION 3.5.0)
project (client_demo)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set (CMAKE_CXX_STANDARD 20)
set (Wfloat-conversion)

#PACKAGES
find_package(Threads REQUIRED)
find_package(OpenSSL 3.6.1 REQUIRED)
find_package(protobuf 6.32.1 REQUIRED)
find_package(logger 1.0.1 REQUIRED)
find_package(netstack 1.0.0 REQUIRED)

#EXTERNAL INCLUDES
include_directories(${ProtobufIncludePath})

#INCLUDES
include_directories(${CMAKE_SOURCE_DIR}/messages/src)
include_directories(${CMAKE_SOURCE_DIR}/src)

#SOURCE FILES
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

#CONFIG FILES
file(GLOB APP_CONFIG
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/conanfile.py
    ${CMAKE_SOURCE_DIR}/create.sh
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${APP_CONFIG}
)

target_link_libraries(  ${PROJECT_NAME}
                        ${CMAKE_THREAD_LIBS_INIT}
                        protobuf::protobuf
                        messages
                        OpenSSL::SSL
                        devfw::logger
                        devfw::netstack
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
