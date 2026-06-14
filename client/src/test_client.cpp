/*****************************************************************
 * Copyright (C) 2017 - Robert Valler - All rights reserved.
 *
 * This file is part of the project: <insert project name here>
 *
 * This project can not be copied and/or distributed
 * without the express permission of the copyright holder
 *****************************************************************/

#include "test_client.h"

#include "logger.h"

// protocol
#include "udp_stack.h"
#include "tcpip_client.h"

// messages - protobuff
#include "message_define.h"
#include "projectMsg.pb.h"
#include "serialise.h"


CTestClient::CTestClient(int argc, char *argv[])
    : mArgc(argc)
    , mArgv(argv)
    , mpUDPStack(std::make_unique<CUDP_Stack>())
    , mpTCPIPStack(std::make_unique<CTCPIP_Client>())
    , mpSerialise(std::make_unique<CSerial>())
{
    if(mArgc > 1) {
        mTCPIPLocalIP = argv[1];
        CLogger::Print("Client : ipAddress = ", mTCPIPLocalIP);
    }
}

CTestClient::~CTestClient()
{ /* do nothing */ }

void CTestClient::Start() {

    // start the threads
    mtDiscovery = std::thread(&CTestClient::Discovery_ThreadFunc, this);
    mtOperational = std::thread(&CTestClient::Operational_ThreadFunc, this);
}

void CTestClient::Stop() {

    mShutdown = true;
    mtDiscovery.join();
    mtOperational.join();
}

void CTestClient::Send() {

    if(!mpTCPIPStack->Connection()) {
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

    if(!mpTCPIPStack->Connection()) {
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

void CTestClient::Discovery_ThreadFunc() {

    message::SMessage msg;
    CSerial serialiser;
    DiscoveryMsg rec_message;

    // Start the UDP
    SUDPParms udp_parms;
    udp_parms.portLocalID = 12346;
    udp_parms.portRemoteID = 12345;

    udp_parms.broadcastIpAddress = "0.0.0.0";
    mpUDPStack->Start(udp_parms);

    while(!mShutdown) {

        auto rec_size = mpUDPStack->Receive(msg);
        //CLogger::Log("mpUDPStack->Receive : " + std::to_string(rec_size));

        if(0 >= rec_size) {
            CLogger::Err("Client " + mTCPIPLocalIP + " receive error");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            continue;
        }

        int size = msg.mMsgPayload.size();
        if(!serialiser.Deserialise(msg.mMsgPayload, rec_message, size)) {

            CLogger::Err("Client " + mTCPIPLocalIP + " Deserialise error");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
#if 0
        std::cout << "Client : received data from ("
                  << rec_message.ipaddress()
                  << ":"
                  << std::to_string(rec_message.port())
                  << ") - "
                  << rec_message.id()
                  << ", "
                  << rec_message.type() << std::endl;
#endif
        // 10 is the discovery ID
        if(10 == rec_message.id()) {

            // connect to the tcpip server
            if(!mpTCPIPStack->Connection()) {

                STCPIPClientParms tcpip_parms;
                tcpip_parms.portID = 12347;
                tcpip_parms.remoteIpAddress = rec_message.ipaddress();
                tcpip_parms.localIpAddress = mTCPIPLocalIP;
                tcpip_parms.maxConnectRetryAttempts = 10;
                tcpip_parms.cert = "../cert/cert.pem";
                tcpip_parms.pkey = "../cert/key.pem";
                if(1 == mpTCPIPStack->Start(tcpip_parms)) {
                    std::cerr << "error: tcpip_client start failed" << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    mpUDPStack->Stop();
}

void CTestClient::Operational_ThreadFunc() {

    message::SMessage message;
    StatusMsg status_msg;
    int send_index = 0;

    while(!mShutdown) {

        if(!mpTCPIPStack->Connection()) {

            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        status_msg.set_id(15);
        status_msg.set_status("This is a STATUS message from Client " + mTCPIPLocalIP + " : Index = " + std::to_string(send_index++));
        int size = message.mMsgPayload.size();
        if(mpSerialise->Serialise(status_msg, message.mMsgPayload, size)) {

            if(0 >= mpTCPIPStack->Send(message)) {

                std::cerr << "[Client] Send failed" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    mpTCPIPStack->Stop();
}

