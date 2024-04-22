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


uint8_t receiver_mac[] = {say sike mf};

//create data struct to send
struct data{
    byte forward;
    byte motorA;
    byte motorB;
} data;

void setup() {
    Serial.begin(9600);
    Serial.println("ESPNow sender Demo");

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
    int topdeadzone = 1500;
    int bottomdeadzone = 600;

    int buttonPressed = 0;


    for(int i = 0; i < 5; i++){
        buttonPressed += analogRead(DEAD_MAN_PIN) / 5;
    }

    if(buttonPressed > 3200){
        

        data.forward = 0;
        int motorA_val = analogRead(MOTORA_PIN);
        int motorB_val = analogRead(MOTORB_PIN);
        bool AisDead = false;
        bool BisDead = false;

        //we have a deadzone between 20 and 50. If the value is between these, we set it to 0.
        
        //then if we are above 50, we map the value to 0-127 and set forward = 2.
        //if we are below 20, we map the value to 0-60, and change the forward value to 1
        // we can only go backward if BOTH motors are set to go still (forward = 0) or backwards (forward = 1)
        // if forward is set to 2, we can only go forward.
        

        //PROBLEMET ÄR HÄR. vi sätter värdet till 0 och mappar det vilket alltid ger 60


        if(motorB_val > bottomdeadzone && motorB_val < topdeadzone && motorA_val > bottomdeadzone && motorA_val < topdeadzone){
            data.forward = 0;
            AisDead = true;
            BisDead = true;
        }
        else if(motorA_val > bottomdeadzone && motorA_val < topdeadzone){
            AisDead = true;
        }
        else if(motorB_val > bottomdeadzone && motorB_val < topdeadzone){
            BisDead = true;
        }


        // if motor a or b is above 50, we set the forward value to 2
        if (motorA_val > topdeadzone || motorB_val > topdeadzone){
            data.forward = 2;
        }

        //we only allow backwards of both motors are either in deadzone or set to go backwards
        if((motorA_val < bottomdeadzone && BisDead) || (motorB_val < bottomdeadzone && AisDead) || (motorA_val < bottomdeadzone && motorB_val < bottomdeadzone)){
            data.forward = 1;

        //if one of the motors are set to go forward and the other is set to go backwards, we stop both motors
        }
        if(motorA_val < bottomdeadzone && BisDead || AisDead && motorB_val < bottomdeadzone){
            data.forward = 0;
        }




        if(data.forward == 2){
            data.motorA = map(motorA_val, topdeadzone+1, 4095, 0, 127);
            data.motorB = map(motorB_val, topdeadzone+1, 4095, 0, 127);
        }else if(data.forward == 1){
            data.motorA = map(motorA_val, 0, bottomdeadzone-1, 60, 0);
            data.motorB = map(motorB_val, 0, bottomdeadzone-1, 60, 0);
        }else{  
            data.motorA = 0;
            data.motorB = 0;
        }

        if(AisDead)
        {
            data.motorA = 0;
        }
        if(BisDead)
        {
            data.motorB = 0;
        }       

        //if one of the motors are set to zero or backwards and the other is set to go forward, we stop both motors


        Serial.println(data.motorA);
        //Apply deadzone, and choose whether to go forwards or backwards

        

    }else{
        data.motorA = 0;
        data.motorB = 0;
        data.forward = 0;
    }

    data.motorA = constrain(data.motorA, 0, 127);
    data.motorB = constrain(data.motorB, 0, 127);

   


    uint8_t *dataBytes = (uint8_t *)&data;
    int dataSize = sizeof(data);

    delay(10);

    ESPNow.send_message(receiver_mac, dataBytes, dataSize);
    

}