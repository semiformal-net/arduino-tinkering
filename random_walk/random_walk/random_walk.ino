
/**** DONT TOUCH THESE **********/
#include <Adafruit_NeoPixel.h>
#define PIN      2
#define N_LEDS 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
/*******************************/
#define N_PARTICLE 3
#define NV N_LEDS*3
#define PARTICLE_VOLUME 16 // intensity of each particle. these add when overlapping so make sure that PARTICLE_VOLUME*N_PARTICLE <255

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

static void rnd_walk(){
  short pos[N_PARTICLE] = {7}; // all non-initialized values will be set to 0 (ie, 7,0,0}
  short rnd[N_PARTICLE] = {0}; // all zeros
  unsigned int rgb_buffer[3]; // holds RGB
  
  while(true){
    // select random left or right movement for each particle
    for(short i=0;i<N_PARTICLE;i=i+1){
      rnd[i]=random(3); // random(max): max is _exclusive_ upper bound. random(3) (or greater) will leave a possibility of the particle staying still
    }
    clear(); // set all LEDs to zero
    // loop over each particle...
    for (short i=0; i<N_PARTICLE; i=i+1){ 
      if(rnd[i] == 0){ pos[i]=pos[i]+1; } // move right,
      else if(rnd[i] == 1) { pos[i]=pos[i]-1; } // or left, otherwise the particle will stay still
      pos[i]=min(max(pos[i],0),strip.numPixels()-1); // bound the new position to [0,7] -- sticky edges
      unpack_rgb( rgb_buffer, strip.getPixelColor(pos[i]) ); // get the i-th pixel color and convert to RGB. Write RGB values to array.
      // add to the LED intensity. each third particle is red, hence the mod(). if you have lots of particles they are: r, g, b, r, g, b,...
      rgb_buffer[i%3]=rgb_buffer[i%3] + PARTICLE_VOLUME; 
      strip.setPixelColor(pos[i],rgb_buffer[0],rgb_buffer[1],rgb_buffer[2]); // set the LED color
    }
    strip.show();
    delay(750);
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
