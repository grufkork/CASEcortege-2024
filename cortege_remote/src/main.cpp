 /* "THE BEER-WARE LICENSE" (Revision 42):
 * regenbogencode@gmail.com wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return
 */

#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include "ESPNowW.h"

//Potentiometer joystick code
#define MOTORA_PIN A5
#define MOTORB_PIN A4
#define DEAD_MAN_PIN A6


uint8_t receiver_mac[] = {0x84,0xCC,0xA8,0x60,0x6F,0xDC};

//create data struct to send
struct data{
    byte forward;
    byte motorA;
    byte motorB;
} data;

void setup() {
    Serial.begin(9600);
    Serial.println("ESPNow sender Demo");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_MODE_STA);

    WiFi.disconnect();
    ESPNow.init();
    ESPNow.add_peer(receiver_mac);

    pinMode(MOTORA_PIN, INPUT);
    pinMode(MOTORB_PIN, INPUT);
    pinMode(DEAD_MAN_PIN, INPUT);

}

void loop() {
    //read analog values and set them to the data struct
    
    int buttonPressed = 0;
    for(int i = 0; i < 5; i++){
        buttonPressed += analogRead(DEAD_MAN_PIN) / 5;
    }

    if(buttonPressed > 3200){
        

        
        int motorA_val = analogRead(MOTORA_PIN);
        int motorB_val = analogRead(MOTORB_PIN);


        if(motorA_val > 50){
            //map and constraon
        }
        //same for motor b


        //check if below 20 then map for reverse, and change forward = ??


        //then this
        
        if(motorA_val > 20 && motorA_val < 50){
            data.motorA = 0;
        }
        if(motorB_val > 20 && motorB_val < 50){
            data.motorA = 0;
        }


        data.motorA = map(motorA_val, 0, 4095, 0, 127);
        data.motorB = map(motorB_val, 0, 4095, 0, 127);
        
        // map and constrain the values to -127 to 127
        data.motorA = constrain(data.motorA, 0, 127);
        data.motorB = constrain(data.motorB, 0, 127);
        
        data.forward = 1;


        //Apply deadzone, and choose whether to go forwards or backwards

        


    


    }
    else{
        data.motorA = 0;
        data.motorB = 0;
    }
  
    Serial.println(data.motorA);
    Serial.println(data.motorB);

    uint8_t *dataBytes = (uint8_t *)&data;
    int dataSize = sizeof(data);

    delay(10);
    ESPNow.send_message(receiver_mac, dataBytes, dataSize);
    

}