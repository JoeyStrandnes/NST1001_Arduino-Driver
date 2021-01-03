
#ifndef NST1001_DRIVER_H
#define NST1001_DRIVER_H

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>


class NST1001{

  public:
    NST1001();
    const void  init(uint8_t const Enable_Pin, uint8_t const Temp_Unit = 0);
    const float getTemp();

  private:
    int EN_Pin{};
    int Temp_Unit{};
    //Multi mode...
};

#endif
