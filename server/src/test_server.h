/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: <insert project name here>
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#pragma (once)

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
    void Receive();

private:
    void DiscoverySend_ThreadFunc();
    std::thread mtDiscoverySend;
    std::atomic<bool> mShutdown{false};

    std::unique_ptr<CUDP_Stack> mpUDPStack;
    std::unique_ptr<CTCPIP_Server> mpTCPIPStack;
    std::unique_ptr<CSerial> mpSerialise;

};
