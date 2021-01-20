# Read Temperature (Multiple sensors)

##### Connection diagram. Rpu should be 3.3K-5K.

![Schematic](https://i.imgur.com/eRvysCx.png)

```c++
/* NST1001 "MultiCast" temperature example!
 * The sensor data pin must be connected to pin "T1". Since it uses the 16-bit hardware counter. 
 * T1:UNO   = 5
 * T1:Nano  = 8
 * T1:Micro = 26
 * 
 * getTemp returns the temperature as a float value. Pass the index of what temperature sensor will be used. 
 * getTemp will return a temperature of 1000 if an error with the sensor occurs.
 */
#include <NST1001.h>


float Temperature_1{0};
float Temperature_2{0};
const int  En_Pin[]{8,3}; // Any digital pin can be used as enable pin! 
const char Unit{'C'};     // The available units are Celsius(C), Fahrenheit(F) and Kelvin(K).

NST1001 nst1001(En_Pin, Unit);


void setup(){

  Serial.begin(115200);
  nst1001.init();
  
}

void loop() {

  Temperature_1 = nst1001.getTemp(0);
  Temperature_2 = nst1001.getTemp(1);

  //This will print both temperatures as independent graphs in the serial plotter!
  Serial.print(Temperature_1);
  Serial.print(",");
  Serial.println(Temperature_2);
  
  delay(10);
}
```

