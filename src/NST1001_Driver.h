
#ifndef NST1001_DRIVER_H
#define NST1001_DRIVER_H

#include <Arduino.h> //Arduino already includes delay and io

class NST1001{

  public:
    NST1001();
    const void  init(int const Enable_Pin, int const Temp_Unit = 0);
    const float getTemp();
    
  private:
    int   EN_Pin{};
    int   Unit{};
    //int MultiModePins[];
    //Multi mode...
};

#endif
