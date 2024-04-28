#include <Arduino.h>
#include <FastLED.h>


#define LEDS 60
#define PIN 3

CRGB leds[LEDS];

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  FastLED.addLeds<WS2811, PIN, BRG>(leds, LEDS);
  leds[0]=CRGB::Red;
  leds[1]=CRGB::Red;
  FastLED.show();
  delay(10000);
}

int i = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("HELLO WORLD");
  if(i >= LEDS*3){
    i = 0;
  }
  byte val = Serial.read();
  if(val == 255){
    FastLED.show();
    i = 0;
  }else{
    leds[i/3][i%3]=val;
    i++;
  }
}