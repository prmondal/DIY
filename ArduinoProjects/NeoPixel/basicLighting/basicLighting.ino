#include <Adafruit_NeoPixel.h>

#define PIN_IN 6
#define NO_LEDS 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NO_LEDS, PIN_IN, NEO_GRB + NEO_KHZ800);
unsigned int ringSingleColorDelay = 100;
unsigned int ringDoubleColorDelay = 100;
unsigned int ringTripleColorDelay = 100;
unsigned int ringSingleColorWithGapDelay = 200;
unsigned int ringToggleSequenceDelay = 100;

unsigned int ringBrightness = 50;

uint32_t red = pixels.Color(255, 0, 0);
uint32_t green = pixels.Color(0, 255, 0);
uint32_t blue = pixels.Color(0, 0, 255);
uint32_t cyan = pixels.Color(0, 255, 255);
uint32_t magenta = pixels.Color(255, 0, 255);
uint32_t yellow = pixels.Color(255, 255, 0);
uint32_t white = pixels.Color(255, 255, 255);
uint32_t pink = pixels.Color(255,192,203);

void makeRingSingleColor(int delayVal) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i++){
    delay(delayVal);
    pixels.setPixelColor(i, magenta);
    pixels.show();
  }
}

void makeRingSingleColorReverse(int delayVal) {
  pixels.clear();
  
  for(int i = NO_LEDS - 1; i >= 0; i--){
    delay(delayVal);
    pixels.setPixelColor(i, cyan);
    pixels.show();
  }
}

void makeRingDoubleColor(int delayVal) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i++){
    delay(delayVal);
    
    if(i % 2 == 0) {
      pixels.setPixelColor(i, blue);
    } else {
      pixels.setPixelColor(i, red);
    }
    
    pixels.show();
  }
}

void makeRingTripleColor(int delayVal) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i++){
    delay(delayVal);
    
    if(i % 3 == 0) {
      pixels.setPixelColor(i, red);
    } else if(i % 2 == 0) {
      pixels.setPixelColor(i, green);
    } else {
      pixels.setPixelColor(i, blue);
    }
    
    pixels.show();
  }
}

void makeRingTripleColorReverse(int delayVal) {
  pixels.clear();
  
  for(int i = NO_LEDS - 1; i >= 0; i--){
    delay(delayVal);
    
    if(i % 3 == 0) {
      pixels.setPixelColor(i, red);
    } else if(i % 2 == 0) {
      pixels.setPixelColor(i, green);
    } else {
      pixels.setPixelColor(i, blue);
    }
    
    pixels.show();
  }
}

void makeRingSingleColorWithGap(int delayVal) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i += 2){
    delay(delayVal);
    pixels.setPixelColor(i, cyan);
    pixels.show();
  }
}

void makeRingToggleSequence(int delayVal, int times) {
  int c = 0;
  
  while(c <= times) {
    c++;
    delay(delayVal);
    pixels.clear();
  
    for(int i = 0; i < NO_LEDS; i += 2){
      pixels.setPixelColor(i, red);
      pixels.show();
    }
    
    delay(delayVal);
    pixels.clear();
    
    for(int i = 1; i < NO_LEDS; i += 2){
      pixels.setPixelColor(i, blue);
      pixels.show();
    }
  }
}

void makeRingSequenceDim(int delayVal) {
  pixels.setPixelColor(0, pixels.Color(255 * sin(millis()),0,0));
  pixels.show();
}

void setup() {
  pixels.begin();
}

void loop() {
  pixels.setBrightness(ringBrightness);
  makeRingSingleColor(ringSingleColorDelay);
  makeRingSingleColorReverse(ringSingleColorDelay);
  makeRingDoubleColor(ringDoubleColorDelay);
  makeRingTripleColorReverse(ringTripleColorDelay);
  makeRingSingleColorWithGap(ringSingleColorWithGapDelay);
  makeRingToggleSequence(ringToggleSequenceDelay, 10);
}
