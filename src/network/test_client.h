/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: <insert project name here>
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#include <memory>
#include <thread>
#include <atomic>

class CUDP_Stack;
class CTCPIP_Client;

class CTestClient {
public:
    CTestClient();
    ~CTestClient();

    void Start();
    void Stop();

private:
    void DiscoveryRec_ThreadFunc();
    std::thread mtDiscoveryRec;
    std::atomic<bool> mShutdown{false};

    std::unique_ptr<CUDP_Stack> mUDPStack;
    std::unique_ptr<CTCPIP_Client> mTCPIPStack;
};
