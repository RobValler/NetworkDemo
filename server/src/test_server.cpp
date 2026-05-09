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

#include "logger.h"

// messages - protobuff
#include "message_define.h"
#include "projectMsg.pb.h"
#include "serialise.h"
#include "encrypt_tls.h"

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
    mtDiscovery = std::thread(&CTestServer::Discovery_ThreadFunc, this);
    mtOperational = std::thread(&CTestServer::Operational_ThreadFunc, this);
}

void CTestServer::Stop() {

    mShutdown = true;
    mtDiscovery.join();
    mtOperational.join();
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

void CTestServer::Discovery_ThreadFunc() {

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
    udp_parms.broadcaster = true;
    udp_parms.broadcastIpAddress = "192.168.100.255";
    mpUDPStack->Start(udp_parms);

    // // Start the TCPIP server
    STCPIPServParms tcpip_parms;
    tcpip_parms.portID = 2001;
    tcpip_parms.ipaddress = "192.168.100.11";
    tcpip_parms.cert = "../external/NetStack/cert/cert.pem";
    tcpip_parms.pkey = "../external/NetStack/cert/key.pem";
    mpTCPIPStack->Start(tcpip_parms);


    send_message.set_port(tcpip_parms.portID);
    send_message.set_ipaddress(tcpip_parms.ipaddress);
    while(!mShutdown) {

        int size;
        if(!serialise.Serialise(send_message, msg.mMsgPayload, size)) {

            std::cerr << "error: server Serialise" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }

        auto send_size = mpUDPStack->Send(msg);
        //CLogger::Log("mpUDPStack->Send : " + std::to_string(send_size));
        if(0 < send_size) {

            //std::cout << "[UDP] Sent OK" << std::endl;
        } else {

            std::cerr << "error: server Send" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    mpUDPStack->Stop();

}

void CTestServer::Operational_ThreadFunc() {

    message::SMessage message;
    StatusMsg status_msg;

    while(!mShutdown) {

        if(!mpTCPIPStack->Connections()) {

            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        auto bytes_rec = mpTCPIPStack->Receive(message);
        if(bytes_rec > 0) {

            int size = message.mMsgPayload.size();
            if(mpSerialise->Deserialise(message.mMsgPayload, status_msg, size)) {

                switch(status_msg.id()) {
                    case 15: {
                        std::cout << "[Server] " << status_msg.status() << std::endl;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
