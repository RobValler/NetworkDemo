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


CTestClient::CTestClient(STestClientParms parms)
    : mParms(parms)
    , mpUDPStack(std::make_unique<CUDP_Stack>())
    , mpTCPIPStack(std::make_unique<CTCPIP_Client>())
    , mpSerialise(std::make_unique<CSerial>())
{ /* do nothing */ }

CTestClient::~CTestClient()
{ /* do nothing */ }

void CTestClient::Start(){


    // Start the UDP
    // SUDPParms udp_parms;
    // udp_parms.broadCastSender = false;
    // udp_parms.portLocalID = 8002;
    // udp_parms.portRemoteID = 8001;
    // udp_parms.localIpAddress = "192.168.100.12";
    // //udp_parms.remoteIpAddress = "192.168.100.255";
    // auto result = mpUDPStack->Start(udp_parms);

    // // Start the TCPIP server
    // STCPIPClientParms mTCPIPParms;
    // mTCPIPParms.ipAddress = mParms.tcp_ipAddress;
    // mTCPIPParms.portID = mParms.tcp_portID;
    // mTCPIPParms.maxConnectRetryAttempts = mParms.tcp_maxConnectRetryAttempts;
    // mpTCPIPStack->Start(mTCPIPParms);

    // start the threads
    mtDiscoveryRec = std::thread(&CTestClient::DiscoveryRec_ThreadFunc, this);
}

void CTestClient::Stop() {

    mShutdown = true;
    mtDiscoveryRec.join();
}

void CTestClient::Send() {

    if(!mpTCPIPStack->Connections()) {
        return;
    }

    message::SMessage message;
    StatusMsg status_msg;
    status_msg.set_id(20);
    status_msg.set_status("Status message - prepare for Information");

    int size;
    if(!mpSerialise->Serialise(status_msg, message.mMsgPayload, size)) {

        std::cerr << "error: Serialise" << std::endl;
        return;
    }
    mpTCPIPStack->Send(message);
    //std::cout << "Client send" << std::endl;
}

void CTestClient::Receive() {

    if(!mpTCPIPStack->Connections()) {
        return;
    }

    message::SMessage message;
    CommandMsg command_message;

    if(0 >= mpTCPIPStack->Receive(message)) {
        std::cerr << "error: client receive" << std::endl;
        return;
    }

    std::cout << "Received" << std::endl;

    int size = message.mMsgPayload.size();
    if(!mpSerialise->Deserialise(message.mMsgPayload, command_message, size)) {
        std::cerr << "error: Deserialise" << std::endl;
        return;
    }

    std::cout << "Client receive: " << command_message.command() << std::endl;
}

void CTestClient::DiscoveryRec_ThreadFunc() {

    message::SMessage msg;
    CSerial serialiser;
    TestMsgPackage rec_message;

    // Start the UDP
    SUDPParms udp_parms;
    udp_parms.broadCastSender = false;
    udp_parms.portLocalID = 8002;
    udp_parms.portRemoteID = 8001;
    udp_parms.localIpAddress = "192.168.100.12";
    //udp_parms.remoteIpAddress = "192.168.100.255";
    mpUDPStack->Start(udp_parms);

    while(!mShutdown) {

        if(0 >= mpUDPStack->Receive(msg)) {
            std::cerr << "error: Receive" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            continue;
        }

        int size = msg.mMsgPayload.size();
        if(!serialiser.Deserialise(msg.mMsgPayload, rec_message, size)) {

            std::cerr << "error: Deserialise" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        std::cout << "Client : received data from ("
                  << msg.mIpAddress
                  << ":"
                  << std::to_string(msg.mPort)
                  << ") - "
                  << rec_message.msgid()
                  << ", "
                  << rec_message.msgname() << std::endl;

        std::string loclal_tcpip_server_IP = msg.mIpAddress;

        // connect to the tcpip server
        if(mTCPIPServerIP != loclal_tcpip_server_IP) {

            STCPIPClientParms parms;
            parms.portID = 2001;
            parms.ipAddress = loclal_tcpip_server_IP;
            parms.maxConnectRetryAttempts = 10;
            if(1 == mpTCPIPStack->Start(parms)) {
                std::cerr << "error: tcpip_client start failed" << std::endl;
            }

            mTCPIPServerIP = loclal_tcpip_server_IP;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    mpUDPStack->Stop();
}
