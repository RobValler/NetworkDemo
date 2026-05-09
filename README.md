# NetStack test application
Use-case for NetStack  

## Setup
include the support libs:  
git submodule add git@github.com:RobValler/NetStack.git   

./create.sh  

## IP Addresses
```
sudo ip addr add 192.168.100.11/24 dev enp1s0
sudo ip addr add 192.168.100.12/24 dev enp1s0
sudo ip addr add 192.168.100.13/24 dev enp1s0
sudo ip addr add 192.168.100.14/24 dev enp1s0
sudo ip addr add 192.168.100.15/24 dev enp1s0
```

check the connections:  
ip addr show dev enp1s0  
