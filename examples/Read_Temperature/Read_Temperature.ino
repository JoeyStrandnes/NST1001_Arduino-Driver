/* NST1001 read temperature example!
 * The sensor data pin must be connected to pin "T1". Since it uses the 16-bit hardware counter. 
 * T1:UNO   = 5
 * T1:Nano  = 5
 * T1:Mini  = 9
 * 
 * getTemp returns the temperature as a float value.
 * getTemp will return a temperature of 1000 if an error with the sensor occurs.
 */
#include <NST1001.h>


float Temperature{0};
const int  En_Pin{8}; // Any digital pin can be used as enable pin! 
const char Unit{'C'}; // The available units are Celsius(C), Fahrenheit(F) and Kelvin(K).

NST1001 nst1001(En_Pin, Unit);


void setup(){

  Serial.begin(115200);
  nst1001.init();
  
}

void loop() {

  Temperature = nst1001.getTemp();
  
  Serial.println("The temperature is: " + String(Temperature) + Unit);
  delay(1000);
}
