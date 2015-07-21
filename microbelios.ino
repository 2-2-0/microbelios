/*********************
 * Microbelios
 *
 * a LAEN development by
 * @juan pablo villegas (art, system design, hw design and hw construction)
 * @220 (sw design and coding)
 *
 * Copyright (C) 2010, LAEN. All rights reserved
 * www.laen.cc
 *
 */

const int elements=6;

// Analog pins
const int sensors [] = {5, 4, 3, 2, 1, 0};

// Digital pins
const int dcs [] = {3, 5, 6, 9, 10, 11};
const int pin_testled = 13;

// Storage
int states [] = {0, 0, 0, 0, 0, 0};
int prev_states [] = {0, 0, 0, 0, 0, 0};

// Parameters
const int decay_factor = 0.2;
const int threshold = 15;

void setup () {
  pinMode (pin_testled, OUTPUT);
  
  for (int i=0; i<elements; i++) {
      pinMode (dcs [i], OUTPUT);
  }
}

void CalcPelucho (int pelucho, int blow) {
  // Set storage for values "mangling" :)
  int val = map (blow, 0, 1023, 0, 255);
    
  int state = states [pelucho];
  int delta = val - state;
      
  // Assign state
  prev_states [pelucho] = states [pelucho];

  
  if (pelucho>0) {
    int peluchoLeft = states [pelucho-1];
    if (peluchoLeft>threshold) {
      // This one's playing...
      if (pelucho>peluchoLeft) {
        states [pelucho-1] = peluchoLeft+(abs (pelucho-peluchoLeft)*0.5);
      }
    } else {
      // This one doesn't have any sound...
      states [pelucho-1] = (states [pelucho]*0.5);
    }
  }
  
  
  if (pelucho<elements-1) {
    int peluchoRight = states [pelucho+1];
    if (peluchoRight>threshold) {
      // This one's playing...
      if (pelucho>peluchoRight) {
        states [pelucho+1] = peluchoRight+(abs (pelucho-peluchoRight)*0.5);
      }
    } else {
      // This one doesn't have any sound...
      states [pelucho+1] = (states [pelucho]*0.5);
    }
  }
}

void process () {
  int blow;
  for (int i=0; i<elements; i++) {
    // Read value
    blow = analogRead (sensors [i]);
    CalcPelucho (i, blow);
  }
  for (int i=0; i<elements; i++) {
    // Play state
    analogWrite (dcs [i], states [i]);
    
    // Decay
    states [i] = int (states [i]*decay_factor);
  }
}

void loop () {
    digitalWrite (pin_testled, 13);
    process ();
}
