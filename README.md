# NetworkDemo - a NetStack test application
Use-case for NetStack  

## Setup
include the support libs:  
git submodule add git@github.com:RobValler/NetStack.git   

./create.sh  

## IP Addresses
For local testing, please add the following ip addresses:  
```
sudo ip addr add 192.168.100.11/24 dev enp1s0
sudo ip addr add 192.168.100.12/24 dev enp1s0
sudo ip addr add 192.168.100.13/24 dev enp1s0
sudo ip addr add 192.168.100.14/24 dev enp1s0
sudo ip addr add 192.168.100.15/24 dev enp1s0
```
Check the connections, for example:  
```
ip addr show dev enp1s0
```
## Firewall stuff
If testing between separate machines please ensure the  
firewall allows for the relevant ports. For example:  
```
sudo ufw allow from 192.168.1.0/24 to any port 12345 proto udp
```

