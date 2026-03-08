/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: StarterApp
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#include "test_server.h"
#include "test_client.h"

// #ifdef _WIN32
// #include <conio.h>
// #endif
#include <iostream>
#include <thread>
#include <chrono>

#include <csignal>

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
    std::signal(SIGINT, signal_handler);   // CTRL+C
    std::signal(SIGTERM, signal_handler);   // CTRL+Z

    std::cout << "Network Test"  << std::endl;;
    std::cout << "=======================" << std::endl;;

    // ### SERVER ###
    auto threadServer = [&]() {
        CTestServer server;

        server.Start();
        while(!gExitRequest) {

            server.Send();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        server.Stop();
    };

    // ### CLIENT ###
    auto threadClient = [&]() {
        CTestClient client;

        client.Start();
        while(!gExitRequest) {

            client.Receive();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        client.Stop();
    };

    std::thread tServer(threadServer);
    std::thread tClient(threadClient);

    std::cout << "Press Enter to continue..."  << std::endl;;

// #ifdef __linux__
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//     std::cin.get();
// #elif _WIN32
//     _getch();
// #endif

//    ExitCalled = true;
    tClient.join();
    tServer.join();


    std::cout << "Network Test closed."  << std::endl;;
}
