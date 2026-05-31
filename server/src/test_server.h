/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: <insert project name here>
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#ifndef TEST_SERVER__H
#define TEST_SERVER__H

#include <memory>
#include <thread>
#include <atomic>

class CUDP_Stack;
class CTCPIP_Server;
class CSerial;

class CTestServer {
public:
    CTestServer();
    ~CTestServer();

    void Start();
    void Stop();
    void Send();

private:
    void Receive();
    void Discovery_ThreadFunc();
    std::thread mtDiscovery;
    void Operational_ThreadFunc();
    std::thread mtOperational;
    std::atomic<bool> mShutdown{false};

    std::unique_ptr<CUDP_Stack> mpUDPStack;
    std::unique_ptr<CTCPIP_Server> mpTCPIPStack;
    std::unique_ptr<CSerial> mpSerialise;

};

#endif // TEST_SERVER__H
