#!/bin/sh
#protoc --cpp_out=. *.proto

echo "#### Protobuf generate ####"
echo ""

#rm *.pb.cc
rm ./src/* || true

# update build folder appropriately
~/.conan2/p/b/protofc2018eefb4dd/p/bin/protoc --cpp_out=./src *.proto
