#include <Arduino.h>
#include <SabertoothSimplified.h>
#include <SPI.h> 
#include "ESPNowW.h"   
#include <WiFi.h>
//CAR STUFF -----------------------------------------
int throttle, brake, steering;

//define potentiometer pins



int forward;
float motorA_speed = 0;
float motorB_speed = 0;
float motorA_speed_desired, motorB_speed_desired;
float previous_motorA_speed = 0;
float previous_motorB_speed = 0;

float acceleration_constant = 0.005;
float slow_down_constant = 0.008;

SabertoothSimplified DRIVER;
//--------------------------------------------------

//COMMUNICATION STUFF ------------------------------

//mac address of the receiver
uint8_t mac[] = {u tryna do sumn?};
//ESPNowW espNow;
//--------------------------------------------------
//msg structure
struct data{
    byte forward;
    byte motorA;
    byte motorB;
} data;


//callback function
void onDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  forward = data.forward;
  motorA_speed_desired = data.motorA;
  motorB_speed_desired = data.motorB;


}


void setup() {

  Serial.begin(9600);
  //COMMUNICATION SETUP


    WiFi.mode(WIFI_MODE_STA);

    ESPNow.set_mac(mac);
    WiFi.disconnect();
    ESPNow.init();
    ESPNow.reg_recv_cb(onDataRecv);
}

  // intital setup
  //SabertoothTXPinSerial.begin(9600);
  // CALIBRATION


void loop() {
  previous_motorA_speed = motorA_speed;
  previous_motorB_speed = motorB_speed;
  if(forward == 2) {
    motorA_speed_desired = motorA_speed_desired;
    motorB_speed_desired = motorB_speed_desired;
  }
  else if(forward == 1){
    motorA_speed_desired = -motorA_speed_desired;
    motorB_speed_desired = -motorB_speed_desired;

  }
  else{
    motorA_speed_desired = 0;
    motorB_speed_desired = 0;
  }

    motorA_speed += (motorA_speed_desired-motorA_speed)*acceleration_constant;
    motorB_speed += (motorB_speed_desired-motorB_speed)*acceleration_constant;
    
    // Serial.println(motorA_speed);
    // Serial.println(motorB_speed);
    // DRIVER.motor(1, motorA_speed);
    // DRIVER.motor(2, motorB_speed);
    Serial.println(motorA_speed);
    delay(10);
  

  
  

}

