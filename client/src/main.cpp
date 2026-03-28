/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: StarterApp
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#include "test_client.h"

#include <thread>
#include <chrono>
#include <csignal>
#include <iostream>


namespace
{
std::atomic<bool> gExitRequest{false};
}

void signal_handler(int signal)
{
    std::cout << "Terminate signal (" << std::to_string(signal) << ") recieved!" << std::endl;
    gExitRequest = true;
}

int main(int argc, char *argv[]) {

    // POSIIX signal handling
    std::signal(SIGINT, signal_handler);    // CTRL+C
    std::signal(SIGTERM, signal_handler);   // CTRL+Z

    STestClientParms parms;
    parms.name = "server";
    parms.tcp_portID = 2001;
    parms.tcp_ipAddress = "192.168.10.12";
    parms.udp_portLocalID = 3002;
    parms.udp_portRemoteID = 3001;
    parms.udp_ipAddress = "192.168.10.12";
    parms.tcp_maxConnectRetryAttempts = 10;

    CTestClient client(parms);
    client.Start();
    while(!gExitRequest) {

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    client.Stop();
}
