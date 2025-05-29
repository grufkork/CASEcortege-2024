// written by Gustav, C.A.S.E ASSOCIATION for the 2025 chalmers cortege.
#include <Arduino.h>
#include "BluetoothSerial.h"

// READ ME PLEASE I BEG YOU.
// GENERAL STRUCTURE OF THE CODE:----------------------------------------------------------------------------------------------------------

// first we recieve the data from the phone. The data is sent as a byte array. The first byte is always 255, this is the start byte.
// the next byte is the rawMove value, which is the value for motion. The next byte is the rawTurn value, which is the value for how much we want to turn.
// we also have a checksum. The default values of the "raw" variables are dependent on the phone app. You dont need to set them when declaring but it makes things easier to read.

// we take these values map them. These values are now called just "move" and "turn".

// we then apply all the scenarios in which we want to stop the robot.
// in some scenarios we want to stop the robot instantly, in some scenarios we want to stop the robot slowly.
// then set speed of the motors

// we lowpass filter these values.
 
//then send them to the motors.

// GOOD LUCK AND GOD SPEED GRIFFIN!
//----------------------------------------------------------------------------------------------------------------------------------------

#define forward_pwm_motor1 22 
#define reverse_pwm_motor1 23 

#define forward_pwm_motor2 18  //NOTE THESE PINS ARE REVERSED ON THE BOARD. THIS IS FOR DIRECTION TO BE CORRECT WHEN HOOKING UP THE MOTORS.
#define reverse_pwm_motor2 21 

// the "raw" values are the values that are sent from the phone to the robot before they are mapped.
int rawMove = 0; 
int rawTurn = 100; //100 is the middle value for the joystick. 0 is left, 200 is right.

int recievedCheksum = 0; 
int forwardBackward = 10; // 10 is the value for forward motion, 1 is the value for backward motion.
int startStop = 1; // 1 is stop, 10 is go.

//these are the values that we map and filter.
float turn = 0; 
float move = 0;

float previous_move = 0;
float previous_turn = 0;

int motor1_speed = 0; //  speed of motor 1
int motor2_speed = 0; //  speed of motor 2

BluetoothSerial SerialBT;


// time tracking variables
int minimumFilterUpdateTime = 10; // update time for the filter in ms
int timeAtLastFilterUpdate = 0; // time at the last update of the filter

// TIMEOUT VARIABLES
int maxTimeSinceLastDataReceived = 200; // maximum time since the last data was received in ms
int timeWhenLastDataRecieved = 0; // time when the last data was received



void setup() {

  Serial.begin(115200); // start the serial monitor at 115200 baud rate
  
  SerialBT.begin("Gustavs Huawei P20"); //Bluetooth device name

  pinMode(forward_pwm_motor1, OUTPUT); 
  pinMode(reverse_pwm_motor1, OUTPUT); 

  pinMode(forward_pwm_motor2, OUTPUT); 
  pinMode(reverse_pwm_motor2, OUTPUT); 

  Serial.println("Waiting for BT client…");
}


void loop() {

  // retrieve the data from the phone.
  if(SerialBT.available() >= 6){
   
    if(SerialBT.read() == 255){ // if we have a 255, we have a new packet.
  
      rawMove = SerialBT.read(); 
      rawTurn = SerialBT.read(); 
      recievedCheksum = SerialBT.read(); 
      forwardBackward = SerialBT.read(); 
      startStop = SerialBT.read();

      timeWhenLastDataRecieved = millis(); // reset the timer
    }
  }

  move = map(rawMove, 0, 200, 0, 255);
  turn = map(rawTurn, 0, 200, -100, 100); 


//ABOUT THE CHECKSUM: The reason for doing the checksum BEFORE the filter is to prevent our wrong values from polluting the filter.
// we also dont stop immediatly when this happens, we stop by letting the filter go to 0. this is because checksum errors can happen even when
// everything is working properly, we just had some bad data.
  if(checksum(rawMove, rawTurn, forwardBackward, startStop) != recievedCheksum){
  
    move = 0; 
    turn = 0; 
  }


  // We only filter every 20ms so that the filter is not too fast

  if(millis() - timeAtLastFilterUpdate > minimumFilterUpdateTime){

    timeAtLastFilterUpdate = millis(); // reset the timer

    if(forwardBackward == 1){
      move = -move; // if forwardBackward is 1, we want to move backwards, so we flip the value.
    }               // this makes the transition smooth and prevents us from going from forward to backward instantly.

    // Below is the code that handles the different scenarios in which we want to stop the robot (except for the checksum error).
    // In some scenarios we want to stop the robot instantly, in some scenarios we want to stop the robot slowly.
  if(millis() - timeWhenLastDataRecieved > maxTimeSinceLastDataReceived){ // if the bluetooth is not connected, we stop the robot
    move = 0.0; 
    turn = 0.0; 
  }

 
  if(SerialBT.connected() == 0 || startStop != 10){ // if the bluetooth is not connected, we stop the robot INSTANTLY. This means we have disconnected from the phone completely.
    move = 0.0;                                      // we also stop instantly if we have a startStop value that is not 10 (10 meaning go).
    turn = 0.0; 
    previous_move = 0.0;
    previous_turn = 0.0;
  }
    

 
    // if move is very low and turn is zero, the user obviously wants to stop the robot
    // so we do it slightly faster than the normal lowpass filter.
    if(abs(move) <= 10 && turn == 0){
      move = 0.95 * previous_move + 0.05 * move; // lowpass filter 
      turn = 0.95 * previous_turn + 0.05 * turn;
    }else{
    move = 0.98 * previous_move + 0.02 * move; // lowpass filter 
    
    turn = 0.98 * previous_turn + 0.02 * turn; 
    }

    previous_move = move; 
    previous_turn = turn; 



    motor1_speed = move - turn; // add the turn value to the move value for one motor
    motor2_speed = move + turn; 

    motor1_speed = constrain(motor1_speed, -255, 255); // limit the value to 0-255
    motor2_speed = constrain(motor2_speed, -255, 255); 
    
    
    if (motor1_speed > 0) {
      analogWrite(forward_pwm_motor1, motor1_speed);
      analogWrite(reverse_pwm_motor1, 0);
    } else {
      analogWrite(forward_pwm_motor1, 0);
      analogWrite(reverse_pwm_motor1, -motor1_speed);
    }


    if (motor2_speed > 0) {
      analogWrite(forward_pwm_motor2, motor2_speed);
      analogWrite(reverse_pwm_motor2, 0);
    } else {
      analogWrite(forward_pwm_motor2, 0);
      analogWrite(reverse_pwm_motor2, -motor2_speed);
    }

    Serial.print("Motor 1 speed: "); // print the speed of motor 1
    Serial.print(motor1_speed); // print the speed of motor 1
    Serial.print(" Motor 2 speed: "); // print the speed of motor 2
    Serial.println(motor2_speed); // print the speed of motor 2

      
  }


}

