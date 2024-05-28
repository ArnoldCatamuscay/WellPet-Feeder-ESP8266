/**
 * Complete project details at https://RandomNerdTutorials.com/esp8266-load-cell-hx711/
 * 
 * HX711 library for Arduino - example file
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/

#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int DT = 12;
const int SCK = 13;

HX711 celda;

void setup() {
  Serial.begin(115200);
  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");

  celda.begin(DT, SCK);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(celda.read()); // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(celda.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(celda.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(celda.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight (not set) divided
                                        // by the SCALE parameter (not set yet)
            
  celda.set_scale(-478.507); //TODO insert your calibration factor
  //scale.set_scale(-471.497); // this value is obtained by calibrating the scale with known weights; see the README for details
  celda.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(celda.read()); // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(celda.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(celda.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(celda.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight, divided
                                        // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(celda.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(celda.get_units(10), 5);

  celda.power_down(); // put the ADC in sleep mode
  delay(5000);
  celda.power_up();
}