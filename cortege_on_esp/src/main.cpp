#include <Arduino.h>
#include <SabertoothSimplified.h>
#include <SPI.h>    

//CAR STUFF -----------------------------------------
int throttle, brake, steering;

//define potentiometer pins
const int THROTTLE_PIN = A4; 
const int STEERING_PIN = A5;

int throttle_reading, steering_reading;


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


void setup() {
  // intital setup
  //SabertoothTXPinSerial.begin(9600);
  Serial.begin(9600);
  // CALIBRATION
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);

}



void loop() {

  previous_motorA_speed = motorA_speed;
  previous_motorB_speed = motorB_speed;

  throttle_reading = map(analogRead(THROTTLE_PIN), 0, 1023, -127, 127);
  steering_reading = map(analogRead(STEERING_PIN), 0, 1023, -127, 127);
  //constrain the values
  throttle_reading = constrain(throttle_reading, -127, 127);
  steering_reading = constrain(steering_reading, -127, 127);

  motorA_speed_desired = throttle_reading - steering_reading;
  motorB_speed_desired = throttle_reading + steering_reading;
  
  Serial.println(throttle_reading);

  motorA_speed += (motorA_speed_desired-motorA_speed)*acceleration_constant;
  motorB_speed += (motorB_speed_desired-motorB_speed)*acceleration_constant;
  
 

  DRIVER.motor(1, motorA_speed);
  DRIVER.motor(2, motorB_speed);

  delay(10);
    
  
  

}


