#include "config.h"

int alreadyTested = 0;

void setup(void) {
  Serial.begin(57600);

  // pin config
  for (int i = 0 ; i < LED_COUNT; i++) {
    pinMode(led_pin[i], OUTPUT);
  }
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

  if( !alreadyTested ) {
    for( int i = 0; i < LED_COUNT; i++ ) {
        digitalWrite(led_pin[i], HIGH);
        delay(200);
    }

    for( int i = 0; i < LED_COUNT; i++ ) {
        digitalWrite(led_pin[i], LOW);
        delay(200);
    }

    alreadyTested = 1;
  }

  double ldr = readLightSensor();
  Serial.print(" LDR=");
  Serial.print(ldr);
  Serial.println();
  delay(200);
} // loop
