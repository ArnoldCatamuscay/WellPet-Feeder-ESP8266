/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-load-cell-hx711/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Calibrating the load cell
#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int DT = 12;
const int SCK = 13;

HX711 celda;

void setup() {
  Serial.begin(115200);
  celda.begin(DT, SCK);
}

void loop() {

  if (celda.is_ready()) {
    celda.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    celda.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = celda.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
}

//calibration factor will be the (reading)/(known weight)