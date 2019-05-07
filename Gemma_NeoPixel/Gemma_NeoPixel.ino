#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN    1

#define PIXEL_COUNT 15
#define BRIGHTNESS 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'  
}

void loop() {
  for(int i=0;i<8;i++){
   startShow(i); 
  }
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 100);    // Black/off
            break;
    case 1: trianguloRGB(20);
            break;
    case 2: colorWipe(strip.Color(255, 0, 0), 100);  // Red
            break;
    case 3: colorWipe(strip.Color(0, 255, 0), 100);  // Green
            break;
    case 4: colorWipe(strip.Color(0, 0, 255), 100);  // Blue
            break;
    case 5: rainbow(1,5);
            break;
    case 6: rainbowCycle(1,5);
            break;
    case 7: pulseColor(strip.Color(0, 0, 255),10); 
            break;
  }
}

void fullColor(uint32_t c){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint16_t loops, uint8_t wait) {
  uint16_t i, j;
  for (int k=0;k<loops;k++){
    for(j=0; j<256; j++) {
      for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i+j) & 255));
      }
      strip.show();
      delay(wait);
    }    
  }
}


void rainbowCycle(uint16_t loops, uint8_t wait) {
  uint16_t i, j;

  for (int k=0;k<loops;k++){
   for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    } 
  }
  
}


void pulseColor(uint32_t color,uint8_t wait) {
  
  
  for(int j = 1; j < 256 ; j++){
    strip.setBrightness(j);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    delay(wait);
  }

  for(int j = 255; j > 5; j--){
    strip.setBrightness(j);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    delay(wait);
  }
  fullColor(strip.Color(0,0,0));
  strip.setBrightness(BRIGHTNESS);
}

void trianguloRGB(uint8_t wait){
  for (uint16_t j=0; j<256; j++){
   for(uint16_t i=0; i<(strip.numPixels()/3); i++) {
        strip.setPixelColor(i, strip.Color(j,0,0,j ) );
    }
    for(uint16_t i=(strip.numPixels()/3); i<((strip.numPixels()/3)*2); i++) {
        strip.setPixelColor(i, strip.Color(0,j,0, j ) );
    }
    for(uint16_t i=(strip.numPixels()/3)*2; i<(strip.numPixels()); i++) {
        strip.setPixelColor(i, strip.Color(0,0,j, j ) );
    } 
    strip.show();
    delay(wait);
  }
  delay(10);
  for (uint16_t j=255; j>=1; j--){
   for(uint16_t i=0; i<(strip.numPixels()/3); i++) {
        strip.setPixelColor(i, strip.Color(j,0,0, j ) );
    }
    for(uint16_t i=(strip.numPixels()/3); i<((strip.numPixels()/3)*2); i++) {
        strip.setPixelColor(i, strip.Color(0,j,0, j ) );
    }
    for(uint16_t i=(strip.numPixels()/3)*2; i<(strip.numPixels()); i++) {
        strip.setPixelColor(i, strip.Color(0,0,j, j ) );
    } 
    strip.show();
    delay(wait);
  }
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint8_t getRed(uint32_t c) {
  return (c >> 8);
}
uint8_t getGreen(uint32_t c) {
  return (c >> 16);
}
uint8_t getBlue(uint32_t c) {
  return (c);
}

