/* NST1001 driver implementation for the Arduino embedded platform.
 * Supported architectures: AVR
 * Date: 20/01-2021
 * Joey Strandnes
 */
#include "NST1001.h"

//Normal Mode
NST1001::NST1001(int const Enable_Pin, char const Temp_Unit): EN_Pin{Enable_Pin}, Unit{Temp_Unit}{
  pinMode(EN_Pin, OUTPUT);
}

//MultiCast Mode
NST1001::NST1001(int const Pins[], char const Temp_Unit):Unit{Temp_Unit}{ //Might be an overlooked bug with "char const Temp_Unit = 'C'"
  
  MultiCast = true;
  
  for(int i = 0; i <= sizeof(Pins); i++){
    MultiCastPins[i] = Pins[i];
    pinMode(MultiCastPins[i], INPUT);   // Set all pins to high impedence. Should default as inputs.
  }
}

//Freerunning Mode
NST1001::NST1001(char const Temp_Unit): Unit{Temp_Unit}{
  Time_Out = 5;
  Freerunning = true;
}



const void NST1001::init(){
/*  Setup timer-counter 1/16-bit timer  
 *  Maximum number of pulses = 3201 => 150 degrees celsius
 */
  cli();
  
//Timer 1 Normal mode with falling edge as clock source. Most code below is unnecessary, it is for clarification.
  TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
  TCCR1B = (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(1<<CS12)|(1<<CS11)|(0<<CS10); 
  TIMSK1 = (0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(0<<TOIE1);
  TCNT1 = 0;

  sei();
}

const float NST1001::getTemp(int const Index){ // Index is only used in multicast mode.
  
  float Temp{0};
  bool  Fault{false};
  
  TCCR1B |= (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(1<<CS12)|(1<<CS11)|(0<<CS10); // Enable counter.
  
  //Start sensor, sensor is always on in freerunning mode. 
  if(MultiCast == true && Freerunning == false){            // Multicast mode                                 
    _delay_ms(5);                                           // Requires some initial delay to settle. Need to look into this.
    pinMode(MultiCastPins[Index], OUTPUT);
  }
  else if(MultiCast == false && Freerunning == false){      // Normal mode
    digitalWrite(EN_Pin, HIGH);
  }


// Freerunning mode requires some sort of sync to the sensor.
// If checking during transmission, discard transmission and check next. 
// If no transmission is detected, continue like normally.
// Time_Out is set to 50us in freerunning mode ~6 pulses. 

  TCNT1 = 0;                                                // Reset counter value.
  if(Freerunning == false){
    for(int i = 0; i < Time_Out; i++){                      // First ~24ms of ADC conversion time. ~50ms timeout (Normal/Multicast mode), fault is set if reached. Async function.    
      if (TCNT1 > 0){
        break;
      }
      else if(TCNT1 == 0 && i == (Time_Out-1)){
        Fault = true;
      }
      _delay_us(10);
    }
  }

  if(Freerunning == true && Fault == false){                //Condition is met if a transmission was detected.
    
    while(1){
      Temp = TCNT1;
      _delay_us(11);
      if(Temp == TCNT1 || TCNT1 >= 3201){                   // If transmission has stopped. 
        TCNT1 = 0;                                          // Set counter to zero.      
        break;
      }
    }

    //Now we need to wait for 24ms.
    for(int i = 0; i < 50; i++){
      if (TCNT1 > 0){
        break;
      }
      else if(TCNT1 == 0 && i == 49){
        Fault = true;
      }
      _delay_ms(1);
    }
  }
  

  while(1){                                                 // Rest of the unknown transmission time. A 11 micro-second delay is longest possible pulse time.
    Temp = TCNT1;
    _delay_us(11);  
    if(Temp == TCNT1 || TCNT1 >= 3201 || Fault == true){    // Check if transmission is finished by checking if the pulse train has stopped, reached max or skip if fault is detected.   
      TCCR1B = 0;                                           // Disable counter.
      if(MultiCast == true && Freerunning == false){        // Multicast mode  
        pinMode(MultiCastPins[Index], INPUT);
      }
      else if(MultiCast == false && Freerunning == false){  // Normal mode
        digitalWrite(EN_Pin, LOW);
      }        
      break;
    }
  }

  if(TCNT1 < 1 || TCNT1 > 3201 || Fault == true){           // Sanity check, returns "huge" value
    Temp = 1000;
  }
  else{
    
    Temp = ((TCNT1*0.0625) - 50.0625);    // Calculate temperature from sampled pulses
    
    //Temp compensation, no compensation is needed in the interval 30 ≤ T ≤ 100 
    if(Temp < 30){
      Temp += ((Temp-30)*0.005);          // Compensation for T < 30
    }
    else if (100 < Temp && Temp < 150){
      Temp += ((100-Temp)*0.012);         // Compensation for 100 < T < 150
    }
    
    //Changing unit, defaults to Celsius (C): F = Fahrenheit, K = Kelvin
    if(Unit == 'F'){
      Temp = ((Temp*1.8)+32);
    }
    else if(Unit == 'K'){
      Temp += 273.15;
      }
    }
  return Temp;
}
