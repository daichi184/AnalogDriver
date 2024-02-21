#include <avr/wdt.h>
#include "common.h"
#include "DS_OscEtherConf.h"
DS_OscEther* osc_;

String bufferStr = "";

const int PWM_PIN_1 = 3;
const int PWM_PIN_2 = 5;
const int PWM_PIN_3 = 6;
const int PWM_PIN_4 = 9;

const int CS_PIN = 10;

enum{
  PWM_MANUAL,
  LED_AUTO_BLINK
};

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println();
    Serial.println(g_version_text);
    Serial.println(g_date_text);
    
//    Serial.println("Command List");
//    Serial.println("v1:OOO");
//    Serial.println("v2:OOO");
//    Serial.println("v3:OOO");
//    Serial.println("v4:OOO");
//    Serial.println("Header v[1-4]:");
//    Serial.println("data value[0-255]");

    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 4  };
    unsigned int myPort = 8880;
    IPAddress myIp = IPAddress(192,168,2,24);
    IPAddress sendIp = IPAddress(192,168,2,100);
//    IPAddress myIp = IPAddress(192,168,2,23);
//    IPAddress sendIp = IPAddress(192,168,2,100);
    unsigned int sendPort = 9002;
    osc_ = new DS_OscEther(mac, myIp, CS_PIN, myPort, sendIp, sendPort);

    osc_->getConfInfomation();

    delay(1000); //起動時にwaitを入れないと外部電源だけでESPがドライブしなかった
    
    pinMode(PWM_PIN_1, OUTPUT);
    pinMode(PWM_PIN_2, OUTPUT);
    pinMode(PWM_PIN_3, OUTPUT);
    pinMode(PWM_PIN_4, OUTPUT);

    wdt_enable(WDTO_4S); //WDTを４秒で設定し有効化
//    wdt_disable();      //WDTの無効化

  Serial.println("-------------START-------------");
    
}

void loop() {
    // put your main code here, to run repeatedly:
    wdt_reset();  //WDTのリセット

    osc_->msgReceive();

    switch(g_status_){
        case PWM_MANUAL:
          pwmManualCntrl();
          break;

        case LED_AUTO_BLINK:
          autoBlink(g_maxValue_1_, g_speedf_);
          break;
    }
}

void pwmManualCntrl(){
    Serial.println("pwmManualCntrl");
    analogWrite(PWM_PIN_1, g_analogValue_1_);
    analogWrite(PWM_PIN_2, g_analogValue_2_);
    analogWrite(PWM_PIN_3, g_analogValue_3_);
    analogWrite(PWM_PIN_4, g_analogValue_4_);
}


void autoBlink(int _maxValue, float _speed){
    static int dir = 1;
    static float value_f = 0.0f;
    
    if(value_f < 0.0 || value_f > 1.0){
        dir *= -1;
    }
    value_f += dir* _speed;
    g_analogValue_1_ = (int)(pow(value_f, 3) * _maxValue);
    analogWrite(PWM_PIN_1, g_analogValue_1_);
}


void serialEvent(){
    while(Serial.available()){
       char incomingByte = (char)Serial.read();
       Serial.print(incomingByte);
       if(incomingByte != '\n'){
            bufferStr += incomingByte;
       }
       else{
            //TODO
            //コンマ区切りでスプリットし、１つ目でCh指定、2つ目でPWMの値を指定
            //それぞれ範囲制限をつける
            char debugBuf[255];
            if(bufferStr.substring(0, 3) == "v1:"){
                uint16_t val = (bufferStr.substring(3, bufferStr.length())).toInt();
                g_analogValue_1_ = val;
                
                if(val < 0 || val >256){
                    sprintf(debugBuf, "Error! Must be a value between 0 and 255.");
                    Serial.println(debugBuf);
                }
                
                sprintf(debugBuf, "PWM 1 Value: %d", g_analogValue_1_);
                Serial.println(debugBuf);
            }
            else if(bufferStr.substring(0, 3) == "v2:"){
                uint16_t val = (bufferStr.substring(3, bufferStr.length())).toInt();
                if(val < 0 || val >256){
                    sprintf(debugBuf, "Error! Must be a value between 0 and 255.");
                    Serial.println(debugBuf);
                }
                g_analogValue_2_ = val;

                sprintf(debugBuf, "PWM 2 Value: %d", g_analogValue_2_);
                Serial.println(debugBuf);
            }
            else if(bufferStr.substring(0, 3) == "v3:"){
                uint16_t val = (bufferStr.substring(3, bufferStr.length())).toInt();
                g_analogValue_3_ = val;
                if(val < 0 || val >256){
                    sprintf(debugBuf, "Error! Must be a value between 0 and 255.");
                    Serial.println(debugBuf);
                }
                
                sprintf(debugBuf, "PWM 3 Value: %d", g_analogValue_3_);
                Serial.println(debugBuf);
            }
            else if(bufferStr.substring(0, 3) == "v4:"){
                uint16_t val = (bufferStr.substring(3, bufferStr.length())).toInt();
                g_analogValue_4_ = val;
                if(val < 0 || val >256){
                    sprintf(debugBuf, "Error! Must be a value between 0 and 255.");
                    Serial.println(debugBuf);
                }
                
                sprintf(debugBuf, "PWM 4 Value: %d", g_analogValue_4_);
                Serial.println(debugBuf);
            }
            else {
                sprintf(debugBuf, "Error! Header is different.");
                Serial.println(debugBuf);
            }
            bufferStr = "";
       }
    }
}
