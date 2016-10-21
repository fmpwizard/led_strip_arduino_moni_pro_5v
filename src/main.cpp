/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

/*
#include <Arduino.h>

int outPin = 9; // LED connected to digital pin 13
int inPin = 10;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

void setup()
{
  pinMode(outPin, OUTPUT);      // sets the digital pin 13 as output
  pinMode(inPin, INPUT);      // sets the digital pin 7 as input
}

void loop()
{
  val = digitalRead(inPin);   // read the input pin
  digitalWrite(outPin, val);    // sets the LED to the button's value
}

*/

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

//On off with ultrasonic distance sensor
//RGB LED on pin 9 to indicate if lights are on or off


//======================================
//int indicatorLED = 9;

// Read pin from moteino
int inPin = 10;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value
//======================================

boolean on = false;

//======================================

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel indicator = Adafruit_NeoPixel(1, indicatorLED, NEO_RGB + NEO_KHZ800);


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(9600);
  pinMode(inPin, INPUT);      // sets the digital pin 10 as input

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //indicator.begin();
  //indicator.setPixelColor(0, indicator.Color(0, 255, 0));
  //indicator.show();

}

//=============================

void lightsOff(){
  uint16_t x;
  for(x=0; x < strip.numPixels(); x++){
    strip.setPixelColor(x, strip.Color(0, 0, 0));
  }
  strip.show();
  //indicator.setPixelColor(0, indicator.Color(255, 0, 0));
  //indicator.show();
  on = false;
  delay(2000);
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  if (val == 0) {
    on = false;
    return;
  }
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    if (val == 0 && on == true){
      lightsOff();
      return;
    }
    delay(wait);
  }
}


void loop() {
  //colorWipe(strip.Color(255, 10, 100), 50);
  //colorWipe(strip.Color(0, 150, 255), 50);
  //colorWipe(strip.Color(100, 0, 255), 50);



  //theaterChase(strip.Color(127, 127, 127), 100); // White
  //calculate effect time as
  // 3 * delay * cycles
  // 3 * 1000 * 2 = 6000ms => 6 seconds
  // cycles = sec / 3 / delay
  // 9000 / 3 / 100 => 30
  //theaterChase(strip.Color(255, 0, 0), 100, 29); // Red
  //Serial.println("2");
  //theaterChase(strip.Color(0, 0, 255), 100, 6); // Blue
  //theaterChase(strip.Color(0, 255, 0), 100, 10);

  //rainbow(20);



  //rainbowCycle(20);
  //colorWipe(strip.Color(255, 10, 100), 50);

  val = digitalRead(inPin);
  if (val == 1){
    on = true;
    colorWipe(strip.Color(255, 10, 100), 50);
    //theaterChase(strip.Color(255, 0, 0), 100, 29); // Red
    //Serial.println("2");
    //theaterChase(strip.Color(0, 0, 255), 100, 6); // Blue
    //theaterChase(strip.Color(0, 255, 0), 100, 10);
    //rainbow(20);

  }
  if (val == 0 && on == true) {
    lightsOff();
  }
  Serial.println("1");
  delay(2000);




  //theaterChaseRainbow(50);
  //theaterChaseRainbowMiddle(50);

}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  if (on == false) {
    return;
  }
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    if (val == 0 && on == true){
      lightsOff();
      return;
    }
    delay(wait);
  }
}



//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint16_t wait, uint8_t cycles) {
  for (int j=0; j<cycles; j++) {  //do N cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      Serial.println(q);

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


//Theatre-style crawling lights with rainbow effect from corners to middle
void theaterChaseRainbowMiddle(uint8_t wait) {
  uint16_t i, j, q, x;
  for (j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (q=0; q < 3; q++) {
      for (i=0; i < strip.numPixels() /2; i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      for (x=strip.numPixels(); x > strip.numPixels() /2; x=x-3) {
        strip.setPixelColor(x-q, Wheel( (x-j) % 255));    //turn every third pixel on
      }
      strip.show();
      //if (nextPattern() == true){
      if (val == 0){
        return;
      }
      delay(wait);

      for (i=0; i < strip.numPixels() / 2; i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (i=strip.numPixels(); i > strip.numPixels() / 2; i=i-3) {
        strip.setPixelColor(i-q, 0);        //turn every third pixel off
      }
    }
  }
}




void rainbow(uint8_t wait) {

  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    if (val == 0){
      return;
    }
    delay(wait);
  }
}
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  uint16_t i, j, q;
  for (j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (q=0; q < 3; q++) {
      for (i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();
      if (val == 0){
        return;
      }
      delay(wait);
      for (i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
