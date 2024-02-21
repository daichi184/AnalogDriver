#pragma once

#include "arduino.h"

#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>


class DS_OscEther{
private:
    byte mac_[6];
    IPAddress recv_ip_;
    
    const int SPI_SS;
    const unsigned int LOCAL_PORT;

    char packet_buf_[UDP_TX_PACKET_MAX_SIZE];

    EthernetUDP Osc_;

    IPAddress send_ip_;
    const unsigned int SEND_PORT;

public:
    DS_OscEther(byte* _mac, IPAddress _recv_ip, const int _ss = 10, const unsigned int _port = 8880, IPAddress _send_ip = IPAddress(192,168,11,2),
    const unsigned int _send_port = 8000)
    : recv_ip_(_recv_ip)
    , SPI_SS(_ss)
    , LOCAL_PORT(_port)
    , send_ip_(_send_ip)
    , SEND_PORT(_send_port)
    {
        memcpy(mac_, _mac, 6);
        
        Ethernet.init(SPI_SS);
        Ethernet.begin(mac_, recv_ip_);
        Osc_.begin(LOCAL_PORT);
    };
    
    ~DS_OscEther()
    {};

    
    void recvPacketInfomation(int16_t _packetSize);
    void msgReceive();
    void getConfInfomation();
    void msgSend(char* _address, int16_t _v1);
};
