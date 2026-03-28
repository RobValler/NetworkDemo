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

struct STestServerParms {
    // general
    std::string name;

    // udp
    bool udp_broadCastSender{false};
    int udp_portLocalID{0};
    int udp_portRemoteID{0};
    std::string  udp_ipAddress{""};

    // tcpip
    int tcp_portID{0};
    std::string tcp_ipAddress;
    //int tcp_maxConnectRetryAttempts;
};

class CUDP_Stack;
class CTCPIP_Server;
class CSerial;

class CTestServer {
public:
    CTestServer(STestServerParms parms);
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

    STestServerParms mParms;

};
