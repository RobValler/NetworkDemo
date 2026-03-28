#!/bin/bash

cd build
server/./network_demo_server &

client/./network_demo_client &

read -n 1 -s -r -p "Press any key to continue..."


killall -9 network_demo_server
killall -9 network_demo_client

cd ..

