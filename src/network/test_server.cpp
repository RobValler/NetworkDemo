/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: <insert project name here>
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#include "test_server.h"


// protocol
#include "udp_stack.h"
#include "tcpip_server.h"

// messages - protobuff
#include "message_define.h"
#include "testMsgPackage.pb.h"
#include "serialise.h"

#include <chrono>

CTestServer::CTestServer()
    : mUDPStack(std::make_unique<CUDP_Stack>())
    , mTCPIPStack(std::make_unique<CTCPIP_Server>())
{ /* do nothing */ }

CTestServer::~CTestServer()
{ /* do nothing */ }

void CTestServer::Start() {

    // Start the UDP
    SUDPParms udp_parms;
    udp_parms.broadCastSender = false;
    udp_parms.portLocalID = 8001;
    udp_parms.portRemoteID = 8002;
    udp_parms.ipAddress = "127.0.0.1";
    mUDPStack->Start(udp_parms);

    // Start the TCPIP server
    STCPIPServerParms tcpip_parms;
    tcpip_parms.portID = 1234;
    mTCPIPStack->Start(tcpip_parms);

    // start the threads
    mtDiscoverySend = std::thread(&CTestServer::DiscoverySend_ThreadFunc, this);
}

void CTestServer::Stop() {

    mShutdown = true;
    mtDiscoverySend.join();
}

void CTestServer::Send() {

}

void CTestServer::Receive() {

    message::SMessage msg;
    CSerial serialise;
    mUDPStack->Receive(msg);
}

void CTestServer::DiscoverySend_ThreadFunc() {

    CSerial serialise;
    TestMsgPackage send_message;
    send_message.set_msgid(10);
    send_message.set_msgname("Discovery request");
    message::SMessage msg;

    while(!mShutdown) {

        int size;
        if(!serialise.Serialise(send_message, msg.mMsgPayload, size)) {

            std::cerr << "error: Serialise" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }

        if(0 >= mUDPStack->Send(msg)) {

            std::cerr << "error: Send" << std::endl;            
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
