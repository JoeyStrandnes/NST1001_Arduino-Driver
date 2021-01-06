# NST1001 Arduino Driver

##### This is a simple Arduino driver for the NST1001 temperature sensor. (Example below)

##### This driver utilizes Timer-Counter 1 (TCCR1) to measure the temperature from the sensor.

### Supported units

- Celsius (default)
- Kelvin
- Fahrenheit 

### Connections

##### Only two pins are required to communicate with the sensor, T1 and any GPIO!

- Rpu should be between 3.3k-10k 

##### The sensor data pin must be connected to pin "T1"/ PD5. Since it uses the 16-bit hardware counter.

- T1:UNO   = 5

- T1:Nano  = 8

- T1:Micro = 26

![Pullup](https://i.imgur.com/5tKvrPX.png)



### Temperature example

```c++
/* NST1001 read temperature example!
 * The sensor data pin must be connected to pin "T1". Since it uses the 16-bit hardware counter. 
 * T1:UNO   = 5
 * T1:Nano  = 8
 * T1:Micro = 26
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

```



### Known issues

##### Below is and extreme example with "fault detection" removed, this is to illustrate what can happen. The problems rarely (dont ever) occur when fault detection is enabled. But worth keeping in mind.

##### Taking rapid measurements or drifting clock. The spikes are caused by looping the measurement with zero delay, a minimum delay of 1ms between measurements is required to ensure stability in the system. Instability can also be caused if the system clock significantly drifts. 

![Timing Error](https://i.imgur.com/08EtMkI.png)