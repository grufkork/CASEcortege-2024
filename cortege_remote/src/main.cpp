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
#define MOTORA_PIN A0
#define MOTORB_PIN A3

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

}

void loop() {
    //read analog values and set them to the data struct
    
    data.motorA = map(analogRead(MOTORA_PIN), 0, 4095, 0, 127);
    data.motorB = map(analogRead(MOTORB_PIN), 0, 4095, 0, 127);
    data.forward = 1;
    // map and constrain the values to -127 to 127
    data.motorA = constrain(data.motorA, 0, 127);
    data.motorB = constrain(data.motorB, 0, 127);

    uint8_t *dataBytes = (uint8_t *)&data;
    int dataSize = sizeof(data);

    delay(100);
    ESPNow.send_message(receiver_mac, dataBytes, dataSize);
    

}