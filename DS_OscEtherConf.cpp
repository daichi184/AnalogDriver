#include "DS_OscEtherConf.h"
#include "common.h"

static const int LEN = 200;
char debugBuf_[LEN];

const uint8_t MAX_VALUE = 255;
const uint8_t MIN_VALUE = 0;
//-------------------------------------------------------------------------------
void recvOscValue(OSCMessage& _msg, int addrOffset){
    int pwmCh = _msg.getInt(0);
    int recvValue = _msg.getInt(1);
    if(recvValue < MIN_VALUE) recvValue = MIN_VALUE;
    if(recvValue > MAX_VALUE) recvValue = MAX_VALUE;
    
    switch(pwmCh){
        case 1:
          g_analogValue_1_ = recvValue;
          break;
        case 2:
          g_analogValue_2_ = recvValue;
          break;
        case 3:
          g_analogValue_3_ = recvValue;
          break;
        case 4:
          g_analogValue_4_ = recvValue;
          break;
    }
    
    sprintf(debugBuf_, "Recv OSC ch: %d, value: %d\n", pwmCh, recvValue);
    Serial.print(debugBuf_);    
}

//-------------------------------------------------------------------------------
void powerOn(OSCMessage& _msg, int addrOffset){
    Serial.println("powerOn");
    int pwmCh = _msg.getInt(0);

    switch(pwmCh){
        case 1:
          g_analogValue_1_ = g_maxValue_1_;
          break;
        case 2:
          g_analogValue_2_ = g_maxValue_2_;
          break;
        case 3:
          g_analogValue_3_ = g_maxValue_3_;
          break;
        case 4:
          g_analogValue_4_ = g_maxValue_4_;
          break;
    }

    sprintf(debugBuf_, "Recv OSC ch: %d, value: %d\n", pwmCh, g_analogValue_1_);
    Serial.print(debugBuf_);    
}

//-------------------------------------------------------------------------------
void powerOff(OSCMessage& _msg, int addrOffset){
    Serial.println("powerOff");
    int pwmCh = _msg.getInt(0);

    switch(pwmCh){
        case 1:
          g_analogValue_1_ = g_minValue_1_;
          break;
        case 2:
          g_analogValue_2_ = g_minValue_2_;
          break;
        case 3:
          g_analogValue_3_ = g_minValue_3_;
          break;
        case 4:
          g_analogValue_4_ = g_minValue_4_;
          break;
    }
}

//-------------------------------------------------------------------------------
void setMaxValue(OSCMessage& _msg, int addrOffset){
    Serial.println("setMaxValue");
    int pwmCh = _msg.getInt(0);
    
    Serial.print("pwmCh: ");
    Serial.print(pwmCh);
    switch(pwmCh){
        case 1:
          g_maxValue_1_ = _msg.getInt(1);
          g_analogValue_1_ = g_maxValue_1_;
          Serial.print(" maxValue_1: ");
          Serial.println(g_maxValue_1_);
          break;
          
        case 2:
          g_maxValue_2_ = _msg.getInt(1);
          g_analogValue_2_ = g_maxValue_2_;
          Serial.print(" maxValue_2: ");
          Serial.println(g_maxValue_2_);
          break;
          
        case 3:
          g_maxValue_3_ = _msg.getInt(1);
          g_analogValue_3_ = g_maxValue_3_;
          Serial.print(" maxValue_3: ");
          Serial.println(g_maxValue_3_);
          break;
          
        case 4:
          g_maxValue_4_ = _msg.getInt(1);
          g_analogValue_4_ = g_maxValue_4_;
          Serial.print(" maxValue_4: ");
          Serial.println(g_maxValue_4_);
          break;
    }
}

//-------------------------------------------------------------------------------
void setMinValue(OSCMessage& _msg, int addrOffset){
    Serial.println("setMinValue");
    int pwmCh = _msg.getInt(0);
    
    Serial.print("pwmCh: ");
    Serial.print(pwmCh);
    switch(pwmCh){
        case 1:
          g_minValue_1_ = _msg.getInt(1);
          g_analogValue_1_ = g_minValue_1_;
          Serial.print(" minValue_1: ");
          Serial.println(g_minValue_1_);
          break;
          
        case 2:
          g_minValue_2_ = _msg.getInt(1);
          g_analogValue_2_ = g_minValue_2_;
          Serial.print(" minValue_2: ");
          Serial.println(g_minValue_2_);
          break;
          
        case 3:
          g_minValue_3_ = _msg.getInt(1);
          g_analogValue_3_ = g_minValue_3_;
          Serial.print(" minValue_3: ");
          Serial.println(g_minValue_3_);
          break;
          
        case 4:
          g_minValue_4_ = _msg.getInt(1);
          g_analogValue_4_ = g_minValue_4_;
          Serial.print(" minValue_4: ");
          Serial.println(g_minValue_4_);
          break;
    }
}
//-------------------------------------------------------------------------------
void setBlinkSpeedValue(OSCMessage& _msg, int addrOffset){
    int pwmCh = _msg.getInt(0);
    g_speedf_ = _msg.getFloat(1);

    Serial.print("setBlinkSpeedValue: ");
    Serial.println(g_speedf_, 8);
}

//-------------------------------------------------------------------------------
void changeModeManual(OSCMessage& _msg, int addrOffset){
//    Serial.println("Manual"); //なぜかこれを有効にすると落ちる
    int bang = _msg.getInt(0);
    g_status_ = 0;
}

//-------------------------------------------------------------------------------
void changeModeAutoBlink(OSCMessage& _msg, int addrOffset){
//    Serial.println("AutoBlink");　//なぜかこれを有効にすると落ちる
    int bang = _msg.getInt(0);
    g_status_ = 1;
}

//-------------------------------------------------------------------------------
void DS_OscEther::recvPacketInfomation(int16_t _packetSize){
    Serial.print("Received packet of size ");
    Serial.println(_packetSize);
    Serial.print("From ");
    IPAddress remote = Osc_.remoteIP();
    for (int i = 0; i < 4; i++)
    {
        Serial.print(remote[i], DEC);
        if (i < 3){
            Serial.print(".");
        }
    }
    Serial.print(", port ");
    Serial.println(Osc_.remotePort());
}

//-------------------------------------------------------------------------------
void DS_OscEther::msgReceive(){      
    int packetSize = Osc_.parsePacket();
    if(packetSize){
        recvPacketInfomation(packetSize);
    
        OSCMessage msgIn;
        while(packetSize--){
            msgIn.fill(Osc_.read());
        }

        if(!msgIn.hasError()){
            msgIn.route("/deviceController/pwm/value", recvOscValue);
            msgIn.route("/deviceController/power/on", powerOn);
            msgIn.route("/deviceController/power/off", powerOff);
            msgIn.route("/deviceController/set/maxValue", setMaxValue);
            msgIn.route("/deviceController/set/minValue", setMinValue);
            msgIn.route("/deviceController/set/speed", setBlinkSpeedValue);
//            msgIn.route("/deviceController/change/status", changeStatus);

            msgIn.route("/deviceController/mode/manual", changeModeManual);
            msgIn.route("/deviceController/mode/autoBlink", changeModeAutoBlink);
        }
    }
}

//-------------------------------------------------------------------------------
void DS_OscEther::getConfInfomation(){
    Serial.println(g_version_text);
    char print_buf[200];
    sprintf(print_buf, "RECV IP Addr : %d.%d.%d.%d\nMAC Addr : %02X:%02X:%02X:%02X:%02X:%02X\nLocalPort : %d\nSEND IP Addr : %d.%d.%d.%d\nSendPort %d\n", 
                        recv_ip_[0], recv_ip_[1], recv_ip_[2], recv_ip_[3],
                        mac_[0], mac_[1], mac_[2], mac_[3], mac_[4], mac_[5],
                        LOCAL_PORT, 
                        send_ip_[0], send_ip_[1], send_ip_[2], send_ip_[3],
                        SEND_PORT);                 
    Serial.println(print_buf); 


    OSCMessage mess("/get/confInfomation");
    mess.add(print_buf);
    Osc_.beginPacket(send_ip_, SEND_PORT);
    mess.send(Osc_);
    Osc_.endPacket();
    mess.empty();
}

//-------------------------------------------------------------------------------
void DS_OscEther::msgSend(char* _address, int16_t _v1){
    OSCMessage mess(_address);
    mess.add(_v1);
    Osc_.beginPacket(send_ip_, SEND_PORT);
    mess.send(Osc_);
    Osc_.endPacket();
    mess.empty();
}
