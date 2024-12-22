
// Includes libraries for tone generation, pitch definitions, distance sensors, and digital potentiometers
//https://github.com/bhall66/DacTone
#include <DacTone.h>
#include <pitches.h>
//https://github.com/Segilmez06/DistanceSensor
#include <DistanceSensor.h>
//https://github.com/minhaj6/DigiPotX9Cxxx
#include <DigiPotX9Cxxx.h>

// Pin definitions for the tone and volume ultrasonic sensors
#define toneTrigPin 21
#define toneEchoPin 22
#define volTrigPin 16
#define volEchoPin 17        

// Pin definitions for the digital potentiometer (DigiPot)
#define incPin 18
#define udPin 19
#define csPin 23

// Define maximum distances for hand from sensor (volume control needs less dynamic range)
#define maxDF 50.
#define maxDV 25.

//define number of notes to tune to,  In this case, just an octave
//#define Nnotes 8
// Two octaves:
#define Nnotes 15
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
//audio.setVolume(100);



// Set the autotuned notes

// C Major Scale
fnote[0] = NOTE_C5;
fnote[1] = NOTE_D5;
fnote[2] = NOTE_E5;
fnote[3] = NOTE_F5;
fnote[4] = NOTE_G5;
fnote[5] = NOTE_A5;
fnote[6] = NOTE_B5;
fnote[7] = NOTE_C6;
// second octave
fnote[8] = NOTE_D6;
fnote[9] = NOTE_E6;
fnote[10] = NOTE_F6;
fnote[11] = NOTE_G6;
fnote[12] = NOTE_A6;
fnote[13] = NOTE_B6;
fnote[14] = NOTE_C7;

/*
// B Flat Major Scale (for Star Trek Theme!)
fnote[0] = NOTE_AS4;
fnote[1] = NOTE_C5;
fnote[2] = NOTE_D5;
fnote[3] = NOTE_DS5;
fnote[4] = NOTE_F5;
fnote[5] = NOTE_G5;
fnote[6] = NOTE_A5;
fnote[7] = NOTE_AS5;
*/


}

// And on to the main loop
void loop() {
  //current distance of hands from sensors
  float fd, vd;
  // instantaneous values of frequency
  float fnow, vnow;
  int inow;
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

  // mapping between hand spacing and frequency/volume
  // now using autotuned notes to define linear hand spacing in frequency
  inow = Nnotes - ((Nnotes*fd/maxDF > Nnotes) ? Nnotes : Nnotes*fd/maxDF);
  fnow = fnote[inow];
  // and logarithmic nature of perceived volume
  vnow = 300. * pow(10., -3*vd/maxDV);

 

  // and introduce a bit of smoothing to the values used, to eliminate glitches and to create that theremin glide
  f = 0.9*f + 0.1*fnow;
  v = 0.8*v + 0.2*vnow;
  // set the volume control on the amplifier
  pot.set(v);
  // and play the tone
  audio.tone(f);
  //pause briefly for applause
  delay(10); 
}