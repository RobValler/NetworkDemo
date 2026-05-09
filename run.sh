#!/bin/bash

cd build/Debug

echo -n "Press any key to continue..."

server/./server_demo &

#client/./client_demo "192.168.100.11" &
#client/./client_demo "192.168.100.12" &
client/./client_demo "192.168.100.13" &
#client/./client_demo "192.168.100.14" &

read -n 1 -s -r

killall -9 server_demo
killall -9 client_demo

ps -aux | grep client_demo
ps -aux | grep server_demo
