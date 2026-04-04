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
#include <iostream>

CTestServer::CTestServer()
    : mpUDPStack(std::make_unique<CUDP_Stack>())
    , mpTCPIPStack(std::make_unique<CTCPIP_Server>())
    , mpSerialise(std::make_unique<CSerial>())
{ /* do nothing */ }

CTestServer::~CTestServer()
{ /* do nothing */ }

void CTestServer::Start() {

    // start the threads
    mtDiscoverySend = std::thread(&CTestServer::DiscoverySend_ThreadFunc, this);
}

void CTestServer::Stop() {

    mShutdown = true;
    mtDiscoverySend.join();
}

void CTestServer::Send() {

    if(0 >= mpTCPIPStack->Connections()) {
        return;
    }

    message::SMessage message;
    CommandMsg command_msg;
    command_msg.set_id(20);
    command_msg.set_command("Command message - prepare for Obedience");

    int size;
    if(!mpSerialise->Serialise(command_msg, message.mMsgPayload, size)) {

        std::cerr << "error: Serialise" << std::endl;
        return;
    }

    mpTCPIPStack->Send(message);
    //std::cout << "Server send" << std::endl;
}

void CTestServer::Receive() {

    if(0 >= mpTCPIPStack->Connections()) {
        return;
    }

    StatusMsg status_msg;
    message::SMessage message;
    if(0 >= mpTCPIPStack->Receive(message)) {
        std::cerr << "error: server receive" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return;
    }

    int size = message.mMsgPayload.size();
    if(!mpSerialise->Deserialise(message.mMsgPayload, status_msg, size)) {
        std::cerr << "error: Deserialise" << std::endl;
    }

    std::cout << "Server receive: " << status_msg.status() << std::endl;
}

void CTestServer::DiscoverySend_ThreadFunc() {

    // ### SERVER ###
    CSerial serialise;
    DiscoveryMsg send_message;
    send_message.set_id(10);
    send_message.set_type("Discovery request");
    message::SMessage msg;

    // Start the UDP
    SUDPParms udp_parms;
    udp_parms.portLocalID = 8001;
    udp_parms.portRemoteID = 8002;
    udp_parms.broadCastSender = false;
    udp_parms.localIpAddress = "192.168.100.11";
    udp_parms.remoteIpAddress = "192.168.100.12";
    mpUDPStack->Start(udp_parms);

    // // Start the TCPIP server
    STCPIPServParms tcpip_parms;
    tcpip_parms.portID = 2001;
    mpTCPIPStack->Start(tcpip_parms);


    while(!mShutdown) {

        int size;
        if(!serialise.Serialise(send_message, msg.mMsgPayload, size)) {

            std::cerr << "error: Serialise" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }

        if(0 < mpUDPStack->Send(msg)) {

            //std::cout << "[UDP] Sent OK" << std::endl;
        } else {

            std::cerr << "error: Send" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    mpUDPStack->Stop();

}
