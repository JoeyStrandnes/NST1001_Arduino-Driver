/* NST1001 driver implementation for the Arduino embedded platform.
 * Supported architectures: AVR
 * Date: 10/02-2021
 * Joey Strandnes
 */
#ifndef NST1001_H
#define NST1001_H

#include <Arduino.h> //Arduino already includes delay and io

class NST1001{

  public:
    NST1001(int const Enable_Pin, char const Temp_Unit = 'C');
    NST1001(int const Pins[], char const Temp_Unit = 'C');
    
    const void  init();
    const float getTemp(int const Index = 0);
    //const float getMultiTemp(int const Index);
    
  private:
    int   EN_Pin{};
    char  Unit{};
    bool  MultiCast{false};
        
    int MultiCastPins[];

};

#endif
