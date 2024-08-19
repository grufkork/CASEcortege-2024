#include <Arduino.h>
#include <FastLED.h>

#define MATLEDS 60
#define CARLEDS 294
#define MATPIN 0
#define CARPIN 2

#define LEDSTOTAL 354

CRGB carleds[CARLEDS];
CRGB matleds[MATLEDS];

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  FastLED.addLeds<WS2811, CARPIN, BRG>(carleds, CARLEDS);
  FastLED.addLeds<WS2811, MATPIN, BRG>(matleds, MATLEDS);
  carleds[0]=CRGB::Red;
  matleds[0]=CRGB::Red;
  FastLED.show();
  delay(1000);
}

int i = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("HELLO WORLD");
  if(i >= LEDSTOTAL*3){
    i = 0;
  }
  byte val = Serial.read();
  if(val == 255){
    FastLED.show();
    i = 0;
  }else{
    if(i < CARLEDS*3){
      carleds[i/3][i%3]=val;
    }else{
      matleds[i/3 - CARLEDS][i%3]=val;
    }
    i++;
  }
}