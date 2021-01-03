
#include "NST1001_Driver.h"

float Temperature;

NST1001 nst1001;

void setup() {

  Serial.begin(115200);
  
  nst1001.init(8);

}

void loop() {

  Temperature = nst1001.getTemp();
  Serial.println(Temperature);
  delay(10);
}
