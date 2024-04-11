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
#define THROTTLE_PIN A4
#define STEERING_PIN A5

uint8_t receiver_mac[] = {0x84,0xCC,0xA8,0x60,0x6F,0xDC};

//create data struct to send
struct data{
    int throttle;
    int steering;
} data;

void setup() {
    Serial.begin(9600);
    Serial.println("ESPNow sender Demo");

    WiFi.mode(WIFI_MODE_STA);

    WiFi.disconnect();
    ESPNow.init();
    ESPNow.add_peer(receiver_mac);

    pinMode(THROTTLE_PIN, INPUT);
    pinMode(STEERING_PIN, INPUT);

}

void loop() {
    //read analog values and set them to the data struct
    
    data.throttle = analogRead(THROTTLE_PIN);
    data.steering = analogRead(STEERING_PIN);
    Serial.println(analogRead(THROTTLE_PIN));
    // map and constrain the values to -127 to 127
    data.throttle = map(data.throttle, 0, 4095, 0, 255);
    data.steering = map(data.steering, 0, 4095, 0, 255);
    data.throttle = constrain(data.throttle, 0, 255);
    data.steering = constrain(data.steering, 0, 255);
    
    uint8_t *dataBytes = (uint8_t *)&data;
    int dataSize = sizeof(data);

    delay(100);
    ESPNow.send_message(receiver_mac, dataBytes, dataSize);
    

}