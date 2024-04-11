#include <Arduino.h>
#include <FastLED.h>


#define LEDS 20
#define PIN 16

CRGB leds[LEDS];

void setup() {
  // put your setup code here, to run once:

  //Serial.begin(115200);
  FastLED.addLeds<WS2811, 0, RGB>(leds, 2);
  leds[0]=CRGB::Red;
  FastLED.show();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("HELLO WORLD");
  byte val;
  /*for(int i = 0; i < LEDS && Serial.available(); i++){
    val = Serial.read();
    if(val == 255){
      break;
    }
    leds[i/3][i%3]=val;
  }
  FastLED.show();*/
}