#include "starsfx.h"
#include "config.h"
#include <Arduino.h>

StarsFX::StarsFX() {
  lum = 255;
  duration = 2000;
  threshold = 97.0;
  bRefresh = false;
}

StarsFX &StarsFX::refreshRate(int freq) {
  rfreq = freq;
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
  bRefresh = true;
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
  return *this;
}

StarsFX& StarsFX::update() {

  if(bRefresh) {
    for(int i = 0; i < howmany; i++) {
      int chance = random(0, 99);
      if(chance > threshold) {
        live[i] =  lum;
      } else {
        live[i] =  0;
      }
    } // for

    if(millis() > (timein + duration) ) {
      bRefresh = false;
    }

  } /*else {
    if( (millis() % rfreq) == 0) {
      // mark to refresh in next update
      bRefresh = true;
    }
    
  }*/

  return *this;
} // update


StarsFX& StarsFX::show() {
  if(bRefresh) {
    for( int i = 0; i < LED_COUNT; i++ ) {
      int val = live[i];
      if ( val > 0) {
        digitalWrite(led_pin[i], HIGH);
      } else {
        digitalWrite(led_pin[i], LOW);
      }
    } // for
  }  // if
  return *this;
}

