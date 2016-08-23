#include "starsfx.h"
#include "config.h"
#include <Arduino.h>

#define DEF_THRESHOLD 97.0

StarsFX::StarsFX() {
  lum = 255;
  metro = new Metro(10);
  _duration = 2000;
  threshold = DEF_THRESHOLD;
  bRefresh = false;
}

StarsFX &StarsFX::refreshRate(int ms) {
  //rfreq = freq;
  metro->interval(ms);
}

StarsFX& StarsFX::bind(int *leds, int cnt) {
    
    live = leds;
    howmany = cnt;

//    pinClock = _pinClock;
//    pinData = _pinData;
//    
//    if(_pinClock != 0) {
//      FastLED.addLeds<APA102, pinData, pinClock>(leds, cnt);
//    } else {
//      FastLED.addLeds<NEOPIXEL, pinData>(leds, cnt);
//    }

    return *this;
}

StarsFX& StarsFX::enter() {
  timein = millis();
  threshold = DEF_THRESHOLD;
  bRefresh = true;
  metro->reset();
  return *this;
}

StarsFX& StarsFX::leave() {
  bRefresh = false;
  return *this;
}


StarsFX& StarsFX::blank() {
  for(int i = 0; i < howmany; i++) {
    live[i] =  0;
  }

  flip();
  
  return *this;
}

StarsFX& StarsFX::flip() {
  for( int i = 0; i < LED_COUNT; i++ ) {
    int val = live[i];
    if ( val > 0) {
      digitalWrite(led_pin[i], HIGH);
    } else {
      digitalWrite(led_pin[i], LOW);
    }
  } // for
}

StarsFX& StarsFX::update() {

  if(bRefresh) {
    long elapsed = millis() - timein;
    for(int i = 0; i < howmany; i++) {
      int chance = random(0, 99);
      if(chance > threshold) {
        live[i] =  lum;
      } else {
        live[i] =  0;
      }
    } // for

    if(elapsed > _duration ) {
      bRefresh = false;
    }
  }

  return *this;
} // update


StarsFX& StarsFX::show() {
  if( bRefresh && metro->check() ) {
    flip();
    /*
    for( int i = 0; i < LED_COUNT; i++ ) {
      int val = live[i];
      if ( val > 0) {
        digitalWrite(led_pin[i], HIGH);
      } else {
        digitalWrite(led_pin[i], LOW);
      }
    } // for
    */
  }  // if
  return *this;
}

