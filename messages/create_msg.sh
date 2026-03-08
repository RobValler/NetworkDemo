#!/bin/sh
#protoc --cpp_out=. *.proto

rm *.pb.cc
rm *.pb.h

# update build folder appropriately
~/.conan2/p/b/protofc2018eefb4dd/p/bin/protoc --cpp_out=. *.proto
