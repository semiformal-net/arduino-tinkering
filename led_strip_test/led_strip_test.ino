// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).

#include <Adafruit_NeoPixel.h>

#define PIN      2
#define N_LEDS 8

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

void loop() {
  rnd_walk();
}

#define N_PARTICLE 3 
#define NV N_LEDS*3
#define PARTICLE_VOLUME 16

static void rnd_walk(){
  int pos[N_PARTICLE] = {7}; // all non-initialized values will be set to 0 (ie, 7,0,0}
  int rnd[N_PARTICLE]={0}; // all zeros
  unsigned int rgb_buffer[3]; // holds RGB
  
  while(true){
    // select random left or right movement for each particle
    for(int i=0;i<N_PARTICLE;i=i+1){
      rnd[i]=random(2); // random(max): max is _exclusive_ upper bound
    }
    clear(); // set all LEDs to zero
    // loop over each particle...
    for (int i=0; i<N_PARTICLE; i=i+1){ 
      if(rnd[i] == 0){ pos[i]=pos[i]+1; } // move right,
      else { pos[i]=pos[i]-1; } // or left
      pos[i]=min(max(pos[i],0),strip.numPixels()-1); // bound the new position to [0,7] -- sticky edges
      unpack_rgb( rgb_buffer, strip.getPixelColor(pos[i]) ); // get the i-th pixel color and convert to RGB. Write RGB values to array.
      // add to the LED intensity. each third particle is red, hence the mod(). if you have lots of particles they are: r, g, b, r, g, b,...
      rgb_buffer[i%3]=rgb_buffer[i%3] + PARTICLE_VOLUME; 
      strip.setPixelColor(pos[i],rgb_buffer[0],rgb_buffer[1],rgb_buffer[2]); // set the LED color
    }
    strip.show();
    delay(1000);
  }
}

static void alternate(){
//
// alternates all odd then all even lights
// uses colors from col 
//
//
  short vol = 128;
  short col[6] = {0,vol  ,0,
                    vol,0,0 };
  while(true){
    for(short i=0; i<2; i=i+1){
      for(short j=i; j<strip.numPixels(); j=j+2) {
       //strip.setPixelColor(j, 128, 0,128);
       strip.setPixelColor(j, col[i*3], col[i*3+1], col[i*3+2]);
      }
      strip.show();
      delay(500);
      clear();      
    }
  }
  
}
static void bounce() {
  short dir=1;
  short pos=0;
  while(true){
   clear();
   if(pos>=(strip.numPixels()-1)){ dir=-1; }
   if(pos<=0){ dir=1; }
   for(short i=pos; i<strip.numPixels(); i=i+dir) {
     short z=255-abs( pos-i )*96;
     strip.setPixelColor(i, min(max(z,0),255), 0,0);
   }
   pos=pos+dir;
   strip.show();
   delay(500);
  }
}
/**********************************
 * 
 * 
 *  LIBRARY OF FUNCTIONS FOR GENERAL USE
 * 
 */
static void clear() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, 0, 0,0);
   }
}

// This takes an packed 32-bit RGB from getPixelColor() and converts to RGB
// Call it like this:
//     unsigned int rgb_buffer[3]; // holds RGB
//     unpack_rgb( rgb_buffer, strip.getPixelColor(pos[i]) ); 
//
void unpack_rgb(unsigned int *buf, uint32_t rgb) { // buffer, and input -- y'all working in C now
  buf[0]=(rgb>>16) &0xff;
  buf[1]=(rgb>>8) &0xff;
  buf[2]= rgb & 0xff;
}
