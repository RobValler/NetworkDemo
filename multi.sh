#!/bin/bash
set -e

#BUILD_TYPE=Release
BUILD_TYPE=Debug

cd ./build/$BUILD_TYPE/client

./client_demo "192.172.100.12" &
./client_demo "192.172.100.13" &
./client_demo "192.172.100.14" &

read 'Press enter to continue...\n'

killall -9 client_demo
