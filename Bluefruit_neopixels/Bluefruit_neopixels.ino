

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#include <Adafruit_NeoPixel.h>

#define FACTORYRESET_ENABLE     0
#define PIN                     5
#define NUMPIXELS               5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN);
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];

//Colores Globales
uint8_t gRed = 0;
uint8_t gGreen = 0;
uint8_t gBlue = 0;
uint8_t brightness = 255;

//Patron seleccionado
uint8_t patron = 3;

//
uint16_t contadorAux = 0;



void setup(void)
{
  delay(500);

  // turn off neopixel
  strip.begin(); // This initializes the NeoPixel library.
  for(uint8_t i=0; i<NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0,0,0)); // off
  }
  strip.show();

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Neopixel Color Picker Example"));
  Serial.println(F("------------------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  /*while (! ble.isConnected()) {
      delay(500);
  }*/

  Serial.println(F("***********************"));

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("***********************"));

}


void loop(void)
{
  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len != 0){   
    /* Got a packet! */
    // printHex(packetbuffer, len);
  
    // Color
    if (packetbuffer[1] == 'C') {
      gRed = packetbuffer[2];
      gGreen = packetbuffer[3];
      gBlue = packetbuffer[4];
      
      /*Serial.print ("RGB #");
      if (red < 0x10) Serial.print("0");
      Serial.print(red, HEX);
      if (green < 0x10) Serial.print("0");
      Serial.print(green, HEX);
      if (blue < 0x10) Serial.print("0");
      Serial.println(blue, HEX);*/
  
      for(uint8_t i=0; i<NUMPIXELS; i++) {
        strip.setPixelColor(i, strip.Color(gRed,gGreen,gBlue));
      }
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  
    // Buttons
    if (packetbuffer[1] == 'B') {
      uint8_t buttnum = packetbuffer[2] - '0';
      boolean pressed = packetbuffer[3] - '0';
      Serial.print ("Button "); Serial.print(buttnum);
      if (pressed) {
        switch(buttnum){
          case 1:
            patron = 1;
            contadorAux=0;
            break;
            
          case 2:
            patron = 2;
            contadorAux=255-brightness;
            break;
            
          case 3:
            patron = 3;
            contadorAux=0;
            break;
            
          case 4:
            patron = 4;
            contadorAux=0;
            break;
            
          case 5:
            //Sube Brillo
            if (patron!=2) subirBrillo(32);
            if(patron!=2 && patron!=4) actualizarColor();
            break;
            
          case 6:
            //Baja Brillo
            if (patron!=2) bajarBrillo(32); 
            if(patron!=2 && patron!=4) actualizarColor();          
            break;
            
          case 7:
            //Izquierda
            break;
            
          case 8:
            //Derecha
            //rainbowCycle(1,25);
            break;
        }
      } else {
        Serial.println(" released");
      }
    }  
  }

  switch(patron){

    case 1:
      fullColor(strip.Color(gRed,gGreen,gBlue));
      break;
      
    case 2:
      pulseColor(strip.Color(gRed,gGreen,gBlue),0);
      break;
      
    case 3:
      rainbow(0);
      break;
      
    case 4:
      colorWipe(strip.Color(gRed,gGreen,gBlue),50);
      break;
    
  }    

}

void actualizarColor(){
  for(uint8_t i=0; i<NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(gRed,gGreen,gBlue));
  }
  strip.setBrightness(brightness);
  strip.show(); // This sends the updated pixel color to the hardware.
}

void bajarBrillo(int cantidad){
  if(brightness>cantidad) brightness=brightness-cantidad;
  else brightness=0;
  
}

void subirBrillo(int cantidad){
  if(brightness<=255-cantidad) brightness=brightness+cantidad;
  else brightness=255;
}


//-----------------
//Patrones de Color
//----------------

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    gRed = 255 - WheelPos * 3;
    gGreen = 0;
    gBlue = WheelPos * 3;
    //return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }  
  else if(WheelPos < 170) {
    WheelPos -= 85;
    gRed = 0;
    gGreen = WheelPos * 3;
    gBlue = 255 - WheelPos * 3;
    //return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else{
    WheelPos -= 170;
    gRed = WheelPos * 3;
    gGreen = 255 - WheelPos * 3;
    gBlue = 0;
    //return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);    
  }
  return strip.Color(gRed, gGreen, gBlue);
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

void pulseColor(uint32_t color,uint8_t wait) {  

  if (contadorAux<251){
    brightness = 255-contadorAux;
    strip.setBrightness(brightness);
    for(uint8_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    delay(wait);
    contadorAux = contadorAux+5;
  }
  else if (contadorAux<490){
    brightness = contadorAux-245;
    strip.setBrightness(brightness);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    delay(wait);    
    contadorAux = contadorAux+5;
  }
  else{
    contadorAux=0;
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

void rainbow(uint8_t wait) {
  uint8_t i;
  if(contadorAux<255){
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+contadorAux) & 255));
    }
    strip.show();
    delay(wait);
    contadorAux++;
  }
  else{
    contadorAux=0;
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  if(contadorAux < (strip.numPixels()*2)){
    if(contadorAux < strip.numPixels()){
      strip.setPixelColor(contadorAux, 0);
      strip.setBrightness(brightness);
      strip.show();
      delay(wait);      
      
    }
    else{
      Serial.print("Entro a encender pin");
      strip.setPixelColor(contadorAux-strip.numPixels(), c);
      strip.setBrightness(brightness);
      strip.show();
      delay(wait);
    }
    contadorAux++;
  }
  else{
   contadorAux = 0; 
  }
}

void fullColor(uint32_t c){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

