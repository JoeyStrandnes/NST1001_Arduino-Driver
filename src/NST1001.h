/* NST1001 driver implementation for the Arduino embedded platform.
 * Supported architectures: AVR
 * Date: 20/01-2021
 * Joey Strandnes
 */
#ifndef NST1001_H
#define NST1001_H

#include <Arduino.h> //Arduino already includes delay and io

class NST1001{

  public:
    NST1001(int const Enable_Pin, char const Temp_Unit = 'C');  //Normal Mode
    NST1001(int const Pins[], char const Temp_Unit = 'C');      //MultiCast Mode
    NST1001(char const Temp_Unit = 'C');                        //Freerunning Mode
    
    const void  init();
    const float getTemp(int const Index = 0);
    
  private:
    int   EN_Pin{};
    char  Unit{};
    bool  MultiCast{false};
    bool  Freerunning{false};
    
    int   MultiCastPins[];

};

#endif
