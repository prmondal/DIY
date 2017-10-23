#include "Adafruit_NeoPixel.h"

#define PIN_IN 6
#define NO_LEDS 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NO_LEDS, PIN_IN, NEO_GRB + NEO_KHZ800);

unsigned int ringBrightness = 200;
unsigned int delayVal = 100;

uint32_t red = pixels.Color(255, 0, 0);
uint32_t green = pixels.Color(0, 255, 0);
uint32_t blue = pixels.Color(0, 0, 255);
uint32_t cyan = pixels.Color(0, 255, 255);
uint32_t magenta = pixels.Color(255, 0, 255);
uint32_t yellow = pixels.Color(255, 255, 0);
uint32_t white = pixels.Color(255, 255, 255);
uint32_t black = pixels.Color(0, 0, 0);
uint32_t pink = pixels.Color(255,192,203);
uint32_t lightBlue = pixels.Color(0, 0, 50);

void makeRingSingleColor(uint32_t color) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i++){
    delay(delayVal);
    pixels.setPixelColor(i, color);
    pixels.show();
  }
}

void makeRingSingleColorReverse(uint32_t color) {
  pixels.clear();
  
  for(int i = NO_LEDS - 1; i >= 0; i--){
    delay(delayVal);
    pixels.setPixelColor(i, color);
    pixels.show();
  }
}

void makeRingDoubleColor(uint32_t color1, uint32_t color2) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i++){
    delay(delayVal);
    
    if(i % 2 == 0) {
      pixels.setPixelColor(i, color1);
    } else {
      pixels.setPixelColor(i, color2);
    }
    
    pixels.show();
  }
}

void makeRingTripleColor(uint32_t color1, uint32_t color2, uint32_t color3) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i++){
    delay(delayVal);
    
    if(i % 3 == 0) {
      pixels.setPixelColor(i, color1);
    } else if(i % 2 == 0) {
      pixels.setPixelColor(i, color2);
    } else {
      pixels.setPixelColor(i, color3);
    }
    
    pixels.show();
  }
}

void makeRingTripleColorReverse(uint32_t color1, uint32_t color2, uint32_t color3) {
  pixels.clear();
  
  for(int i = NO_LEDS - 1; i >= 0; i--){
    delay(delayVal);
    
    if(i % 3 == 0) {
      pixels.setPixelColor(i, color1);
    } else if(i % 2 == 0) {
      pixels.setPixelColor(i, color2);
    } else {
      pixels.setPixelColor(i, color3);
    }
    
    pixels.show();
  }
}

void makeRingSingleColorWithGap(uint32_t color) {
  pixels.clear();
  
  for(int i = 0; i < NO_LEDS; i += 2){
    delay(delayVal);
    pixels.setPixelColor(i, color);
    pixels.show();
  }
}

void makeRingToggleSequence(uint32_t color1, uint32_t color2, int times) {
  int c = 0;
  
  while(c < times) {
    c++;
    delay(delayVal);
    pixels.clear();
  
    for(int i = 0; i < NO_LEDS; i += 2){
      pixels.setPixelColor(i, color1);
      pixels.show();
    }
    
    delay(delayVal);
    pixels.clear();
    
    for(int i = 1; i < NO_LEDS; i += 2){
      pixels.setPixelColor(i, color2);
      pixels.show();
    }
  }
}

void makeRingChaserWithFixedBackground(uint32_t color1, uint32_t color2, uint32_t color3, int length, int times) {
  int c = 0;

  while(c < times) {
    c++;

    pixels.clear();

    for(int i = 0; i < length; i++) {
      pixels.setPixelColor(i, color1);
    }
    
    for(int i = length; i < NO_LEDS; i++) {
      pixels.setPixelColor(i, color2);
    }
  
    for(int i = 0; i < NO_LEDS; i++) {
      delay(delayVal);
  
      for(int j = 0; j < length; j++) {
        pixels.setPixelColor((i + j) % NO_LEDS, color1);
      }
      
      if(i == 0) {
        pixels.setPixelColor(NO_LEDS - 1, color3);
      } else {
        pixels.setPixelColor((i - 1) % NO_LEDS, color2);
      }
      
      pixels.show();
    }
  }
}

void makeRingChaser(int times) {
  int c = 0;
  uint32_t colList[] = {red, green, blue};
  
  while(c < times) {
    c++;

    uint32_t color = colList[c % 3];
    
    for(int i = 0; i < NO_LEDS; i++) {
      pixels.clear();
      delay(delayVal);
      pixels.setPixelColor(i, color);
      pixels.setPixelColor((i + 1) % NO_LEDS, color);
      pixels.setPixelColor((i + 2) % NO_LEDS, color);
      pixels.show();
    }
  }
}

void makeRingBounceTwoColor(uint32_t color1, uint32_t color2, int times) {
  int c = 0;

  while(c < times) {
    c++;
    
    for(int i = 0; i < NO_LEDS / 2; i++) {
      pixels.clear();
      delay(delayVal);
      pixels.setPixelColor(i, color1);
      pixels.setPixelColor(NO_LEDS - i - 1, color2);
      pixels.show();
    }
  
    for(int i = NO_LEDS / 2 - 1; i >= 0; i--) {
      pixels.clear();
      delay(delayVal);
      pixels.setPixelColor(i, color1);
      pixels.setPixelColor(NO_LEDS - i - 1, color2);
      pixels.show();
    }
  }
}

//TODO: make smooth dim
void makeRingDoubleColorDimIndependent(int times) {
  int c = 0;

  while(c < times) {
    c++;

    delay(delayVal);
    
    for(int i = 0; i < NO_LEDS; i++) {
      if(i % 2 == 0) {
        pixels.setPixelColor(i, pixels.Color(255 * sin(millis()), 0, 0)); 
      } else {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255 * sin(millis())));
      }

      pixels.show();
    }
  }
}

//TODO
void makeRingSingleColorDimSimultaneous(int times) {
  int c = 0;

  while(c < times) {
    c++;

    delay(200);
    uint8_t color = 255 * sin(millis());
    
    for(int i = 0; i < NO_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(color, 0, 0));
    }

    pixels.show();
  }
}

//taken from example
void rainbow() {
  uint16_t i, j;

  for(j = 0; j < 256; j++) {
    for(i = 0; i < NO_LEDS; i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    
    pixels.show();
    delay(20);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup() {
  pixels.begin();
  pixels.show(); //set all to off
}

void loop() {
  pixels.setBrightness(ringBrightness);
  
  makeRingChaser(5);
  makeRingChaserWithFixedBackground(red, blue, lightBlue, 3, 2);
  makeRingSingleColor(magenta);
  makeRingSingleColorReverse(cyan);
  makeRingDoubleColor(blue, red);
  makeRingTripleColorReverse(red, green, blue);
  makeRingSingleColorWithGap(cyan);
  makeRingToggleSequence(red, blue, 10);
  makeRingDoubleColorDimIndependent(50);
  makeRingBounceTwoColor(red, blue, 2);
  rainbow();
}
