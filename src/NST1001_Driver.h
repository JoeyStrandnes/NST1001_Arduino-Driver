
#ifndef NST1001_DRIVER_H
#define NST1001_DRIVER_H

#include <Arduino.h> //Arduino already includes delay and io

class NST1001{

  public:
    NST1001(int const Enable_Pin, char const Temp_Unit = 'C');
    const void  init();
    const float getTemp();
    
  private:
    int   EN_Pin{};
    char  Unit{};
    //int   Unit{};
    //int MultiModePins[];
    //Multi mode...
};

#endif
