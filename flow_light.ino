#include "config.h"
#include "easing.h"
#include "starsfx.h"
#include "runningaverage.h"
#include <CmdMessenger.h>  // CmdMessenger


Metro *metroldr;  // timer that determines how often we read from the LDR

RunningAverage ravg( (1000/LDR_READ_EVERY_MS)*10 );

/**
 *  PROTOCOL: The node gets a string-based serial protocol defined by numbers, the
 *  string has a space in between parameters and it is terminated by a semicolon ';'
 *     
 *  #### TURNING ON AND OFF AN LED
 *  line: command node led intensity;  
 *      -> example: 10 1 3 200;  
 *      -> meaning: Command = kSet, node = 1, led = 3, intensity of LED = 200 
 *      -> result: LED 3 on node 1 goes on
 *  
 *  The LED number can be the actual number of a real LED, or bigger. If it is bigger it will
 *  indicate the number of a special effect.
 */

//#define LED_COUNT 6       // total number of LEDs in the node
//#define FX_STARS  40      // this we compare to the LED number in the protocol
//
//const int led_pin[] = {3, 4, 5, 6, 7, 8}; // PIN configuration
//

int ledframe[LED_COUNT];   // backbuffer for LEDs, so that we can change values without touching pins
StarsFX stars;

enum
{
  kReset = 0,
  kSet = 10,
  kFire = 20
};

char field_separator   = ' ';
char command_separator = ';';

// attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

void debug(int node, int led, int val) {
  Serial.println("[RX] ");
  Serial.print("   NODE=");
  Serial.print(node);
  Serial.print(" LED/FX=");
  Serial.print(led);
  Serial.print(" VAL=");
  Serial.print(val);
  Serial.println();
}

void on_reset()
{
  Serial.println("on reset");
  stars.leave();
  stars.blank();
  stars.show();
}

/** turn one LED on and OFF per command */
void on_set()
{
  Serial.println("on set");

  // Read led state argument, interpret string as boolean
  int node = cmdMessenger.readInt16Arg();
  int led_num = cmdMessenger.readInt16Arg();
  int led_val = cmdMessenger.readInt16Arg();

  debug(node, led_num, led_val);
  
  if(led_num < LED_COUNT) {
    if ( led_val > 0) {
      digitalWrite(led_pin[led_num], HIGH);
    } else {
      digitalWrite(led_pin[led_num], LOW);
    }
  } // if
  
} // on_set


/** trigger special FX stars */
void on_fire()
{
  Serial.println("on fire STARFX");

  // Read led state argument, interpret string as boolean
  int node = cmdMessenger.readInt16Arg();
  int freq = cmdMessenger.readInt16Arg();
  int secs = cmdMessenger.readInt16Arg();

  debug(node, freq, secs);

  // use the meta parameter to change the frequency of blinking
  stars.refreshRate(freq);
  stars.duration(secs * 1000);
  stars.enter(); // start special FX
} // on_fire

void on_unknown() {
  Serial.println("command unknown");
}

void attachCommandCallbacks() {
  cmdMessenger.attach(kReset, on_reset);
  cmdMessenger.attach(kSet, on_set);
  cmdMessenger.attach(kFire, on_fire);
  cmdMessenger.attach(on_unknown);
}

// /////////////////////////////////////
// Arduino
void setup(void) {
  Serial.begin(57600);

  metroldr = new Metro(LDR_READ_EVERY_MS);

  // pin config
  for (int i = 0 ; i < LED_COUNT; i++) {
    pinMode(led_pin[i], OUTPUT);
  }

  //printf_begin();

  cmdMessenger.printLfCr();
  attachCommandCallbacks();

  stars.bind(ledframe, LED_COUNT);
  stars.blank();
}

void serialPump() {
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
}

double readLightSensor() {
  double reading = .0;
  // read 4 times to get a stable reading
  for(int i = 0; i < 4; i++) {
    reading += analogRead(ldr_pin);
    delay(5);
  }

  return (reading / 4.0);
}

void loop() {
  serialPump();

  if( metroldr->check() ) {
    double ldr = readLightSensor();
    ravg.addValue(ldr);
    double avg = ravg.getAverage();
    double dev = ((100 * ldr) / avg );
    Serial.print(" LDR=");
    Serial.print(ldr);
    Serial.print(" AVG=");
    Serial.print(avg);
    Serial.print(" DEV=");
    Serial.print(dev);
    Serial.println();
    
    // check trigger condition
    if( dev < 80.0 ) {
      // use the meta parameter to change the frequency of blinking
      stars.refreshRate(8);
      stars.duration(5 * 1000);
      stars.enter();
    }
  }  
  
  // update special fx
  stars.update();
  stars.show();
}


