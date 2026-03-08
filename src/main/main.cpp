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

#ifdef _WIN32
#include <conio.h>
#endif
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {

    std::cout << "Network Test"  << std::endl;;
    std::cout << "=======================" << std::endl;;

    std::atomic<bool> ExitCalled = false;

    // ### SERVER ###
    auto threadServer = [&]() {
        CTestServer server;

        server.Start();
        while(!ExitCalled) {

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        server.Stop();
    };

    // ### CLIENT ###
    auto threadClient = [&]() {
        CTestClient client;

        client.Start();
        while(!ExitCalled) {

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        client.Stop();
    };

    std::thread tServer(threadServer);
    std::thread tClient(threadClient); // client is receiver, receiver creates the channel for MQ

    std::cout << "Press Enter to continue..."  << std::endl;;

#ifdef __linux__
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
#elif _WIN32
    _getch();
#endif

//    ExitCalled = true;
    tClient.join();
    tServer.join();


    std::cout << "Network Test closed."  << std::endl;;
}
