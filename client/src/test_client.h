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
class CTCPIP_Client;
class CSerial;

class CTestClient {
public:
    CTestClient(int argc, char *argv[]);
    ~CTestClient();

    void Start();
    void Stop();
    void Send();
    void Receive();

private:
    void Discovery_ThreadFunc();
    std::thread mtDiscovery;
    void Operational_ThreadFunc();
    std::thread mtOperational;
    std::atomic<bool> mShutdown{false};

    std::unique_ptr<CUDP_Stack> mpUDPStack;
    std::unique_ptr<CTCPIP_Client> mpTCPIPStack;
    std::unique_ptr<CSerial> mpSerialise;

    //bool mConnected{false};
    //std::string mTCPIPServerIP{""};
    std::string mTCPIPLocalIP{""};

    int mArgc;
    char **mArgv;
};
