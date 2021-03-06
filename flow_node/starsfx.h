#ifndef __STARFX_H__
#define __STARFX_H__

#include <math.h>
#include "metro.h"  // simple metronome-like library

class StarsFX {
  protected:
    int     *live;
    int     *cache;

    float   threshold;
    
    unsigned long _duration;  // duration
    unsigned long timein;    // time in

    bool bRefresh;
    
    int lum;
    //int rfreq;

    Metro *metro;
    
    int howmany;
    int pinData;
    int pinClock;

  public:
    StarsFX();
    StarsFX& bind(int *leds, int cnt);

    StarsFX &refreshRate(int ms);

    StarsFX& enter();  // stars the animation for the special FX
    StarsFX& leave();  // stops the animation for the special FX

    StarsFX& duration(unsigned long d) { _duration = d; return *this; } // set duration
    
    StarsFX& blank();  // blank-out the LED backbuffer

    StarsFX& flip(); //  copy backbuffer to LEDs
    
    StarsFX& update(); // update animation
    StarsFX& show();   // copy backbuffer to real LEDs
};

#endif // __STARFX_H__
