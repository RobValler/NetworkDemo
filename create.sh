#!/bin/sh
#BUILD_TYPE=Release
BUILD_TYPE=Debug

echo "Build type $BUILD_TYPE"
set -e
rm -rf build || true
mkdir build
cd build
conan install .. --build=missing --settings=build_type=$BUILD_TYPE
cmake   -DCMAKE_TOOLCHAIN_FILE=$BUILD_TYPE/generators/conan_toolchain.cmake \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DTEST_ENABLE=ON \
        -DProtobuf_PROTOC_EXECUTABLE=~/.conan2/p/b/protofc2018eefb4dd/p/bin/protoc \
        ..
cmake --build . -j16


