
// Includes libraries for tone generation, pitch definitions, distance sensors, and digital potentiometers
//https://github.com/bhall66/DacTone
#include <DacTone.h>
#include <pitches.h>
//https://github.com/Segilmez06/DistanceSensor
#include <DistanceSensor.h>
//https://github.com/minhaj6/DigiPotX9Cxxx
#include <DigiPotX9Cxxx.h>

// Pin definitions for the tone and volume ultrasonic sensors
#define toneTrigPin 22
#define toneEchoPin 23
#define volTrigPin 15
#define volEchoPin 16

// Pin definitions for the digital potentiometer (DigiPot)
#define incPin 17
#define udPin 18
#define csPin 19

// Define maximum distances for hand from sensor (volume control needs less dynamic range)
#define maxDF 50.
#define maxDV 15.

//define number of notes to tune to,  In this case, just an octave
#define Nnotes 8
// Frequencies of autotuned notes
float fnote[Nnotes];

// Set up ultrasonic distance sensors for tone and volume control
DistanceSensor tonehc(toneTrigPin, toneEchoPin);
DistanceSensor volhc(volTrigPin, volEchoPin);

// Set up digital potentiometer as volume control for amplifier
DigiPot pot(incPin,udPin,csPin);

// Audio outpout instances that uses ESP32's sine wave generator
DacTone audio;


//initialize a few things
void setup() {
//put digipot in defined state
pot.reset();
pot.set(50);
// don't deafen the neighbours
audio.setVolume(50);

// Set the autotuned notes
fnote[0] = NOTE_C5;
fnote[1] = NOTE_D5;
fnote[2] = NOTE_E5;
fnote[3] = NOTE_F5;
fnote[4] = NOTE_G5;
fnote[5] = NOTE_A5;
fnote[6] = NOTE_B5;
fnote[7] = NOTE_C6;
  
}

// And on to the main loop
void loop() {
  //current distance of hands from sensors
  float fd, vd;
  // instantaneous values of frequency
  float fnow, vnow;
  // smoothed values for playing
  static float f, v; 
  // variables for figuring out autotune
  float df, fbest;

  // get hand distances from volume and tone control ultrasonic sensors
  fd = tonehc.getCM();
  vd = volhc.getCM();
  
   // make sure we are in range 
  if ((fd > maxDF) || (fd == 0)) fd = maxDF;
  if ((vd > maxDV) || (vd == 0)) vd = maxDV;

  // mapping between hand spacing and frequency/volume, which accounts for their naturally logarithmic nature
  fnow = 1200. * pow(10, -0.35*fd/maxDF);
  vnow = 300. * pow(10., -3*vd/maxDV);

  //autotune frequency to closest note
  df = 100000.;
  for (int i=0; i<Nnotes; i++) {
    if (abs(fnote[i] - fnow) < df) {
      df = abs(fnote[i] - fnow);
      fbest = fnote[i];
    }
  }

  // and introduce a bit of smoothing to the values used, to eliminate glitches and to create that theremin glide
  f = 0.9*f + 0.1*fbest;
  v = 0.8*v + 0.2*vnow;
  // set the volume control on the amplifier
  pot.set(v);
  // and play the tone
  audio.tone(f);
  //pause briefly for applause
  delay(10); 
}