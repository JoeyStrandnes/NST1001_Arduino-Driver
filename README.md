# NST1001 Arduino Driver

##### This is a simple Arduino driver for the NST1001 temperature sensor. (Example below)

##### This driver utilizes Timer-Counter 1 (TCCR1) to measure the temperature from the sensor.

### Overview of functions.

#### Sensor mode = Normal (single sensor)

| Function              | Parameters | Comment                                        | Return type |
| --------------------- | ---------- | ---------------------------------------------- | ----------- |
| NST1001 (Constructor) | int, char  | int = enable pin<br />char = C, F or K. (unit) | void        |
| NST1001 (Constructor) | int        | Will default to Celsius.                       | void        |
| init                  | -          | Starts timer.                                  | void        |
| getTemp               | -          | Returns temperature.                           | float       |



#### Sensor mode = MultiCast (multiple sensors)

| Function              | Parameters      | Comment                                                    | Return type |
| --------------------- | --------------- | ---------------------------------------------------------- | ----------- |
| NST1001 (Constructor) | int array, char | int array = all enable pins.<br />char = C, F or K. (unit) | void        |
| NST1001 (Constructor) | int array       | Will default to Celsius.                                   | void        |
| init                  | -               |                                                            | void        |
| getTemp               | int             | int = index from the enable pin array                      | float       |



### Supported units

- Celsius (default)
- Kelvin
- Fahrenheit 



### Examples with connection diagram

##### [Single sensor example](https://github.com/JoeyStrandnes/NST1001_Arduino-Driver/tree/Dev/examples/Read_Temperature)

##### [Multicast example](https://github.com/JoeyStrandnes/NST1001_Arduino-Driver/tree/Dev/examples/Read_Temperature_MultiCast)



### Known issues

##### Below is and extreme example with "fault detection" removed, this is to illustrate what can happen. The problems rarely occur when fault detection is enabled. But worth keeping in mind.

##### Taking rapid measurements or drifting clock. The spikes are caused by looping the measurement with zero delay, a minimum delay of 1ms between measurements is required to ensure stability in the system. Instability can also be caused if the system clock significantly drifts. 

![Timing Error](https://i.imgur.com/08EtMkI.png)