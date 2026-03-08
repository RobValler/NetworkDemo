/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: <insert project name here>
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#include "test_client.h"

// protocol
#include "udp_stack.h"
#include "tcpip_client.h"

// messages - protobuff
#include "message_define.h"
#include "testMsgPackage.pb.h"
#include "serialise.h"


CTestClient::CTestClient()
    : mUDPStack(std::make_unique<CUDP_Stack>())
    , mTCPIPStack(std::make_unique<CTCPIP_Client>())
{ /* do nothing */ }

CTestClient::~CTestClient()
{ /* do nothing */ }

void CTestClient::Start(){

    // Start the UDP
    SUDPParms udp_parms;
    udp_parms.broadCastSender = false;
    udp_parms.portLocalID = 8002;
    udp_parms.portRemoteID = 8001;
    udp_parms.ipAddress = "127.0.0.1";
    mUDPStack->Start(udp_parms);

    // start the threads
    mtDiscoveryRec = std::thread(&CTestClient::DiscoveryRec_ThreadFunc, this);
}

void CTestClient::Stop() {

    mShutdown = true;
    mtDiscoveryRec.join();
}

void CTestClient::DiscoveryRec_ThreadFunc() {

    message::SMessage msg;
    CSerial serialiser;
    TestMsgPackage rec_message;

    while(!mShutdown) {

        if(0 >= mUDPStack->Receive(msg)) {
            std::cerr << "error: Receive" << std::endl;
            continue;
        }

        int size = msg.mMsgPayload.size();
        if(!serialiser.Deserialise(msg.mMsgPayload, rec_message, size)) {
            std::cerr << "error: Deserialise" << std::endl;
            continue;
        }

#if 0
        std::cout << "Client : received data from ("
                  << msg.mIpAddress
                  << ":"
                  << std::to_string(msg.mPort)
                  << ") - "
                  << rec_message.msgid()
                  << ", "
                  << rec_message.msgname() << std::endl;
#endif

        // call only once
        if(!mIsConnectionRequested) {
            STCPIPClientParms parms;
            parms.ipAddress = msg.mIpAddress;
            parms.portID = 1234;
            parms.maxConnectRetryAttempts = 10;
            mIsConnectionRequested = true;
            mTCPIPStack->Start(parms);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
