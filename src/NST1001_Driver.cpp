/*ToDo: 
 * Add Pulldown mode and "Multicast" mode
 */
#include "NST1001_Driver.h"



NST1001::NST1001(int const Enable_Pin, char const Temp_Unit): EN_Pin{Enable_Pin}, Unit{Temp_Unit}{
/* No idea why this doesnt work...
#if Temp_Unit != 'C'// || Temp_Unit != 'K' || Temp_Unit != 'F')
#error "Incorrect unit input. Only C, K and F are allowed"
#endif  
*/
}

const void NST1001::init(){
/*  Setup timer-counter 1/16-bit timer  
 *  Maximum number of pulses = 3201 => 150 degrees celsius
 */
  cli();
  
  pinMode(EN_Pin, OUTPUT);
  
//Timer 1 Normal mode with falling edge as clock source. Most code below is unnecessary, it is for clarification.
  TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
  TCCR1B = (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(1<<CS12)|(1<<CS11)|(0<<CS10); 
  TIMSK1 = (0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(0<<TOIE1);
  TCNT1 = 0;

  sei();
}

const float NST1001::getTemp(){
  
  float Temp{0};
  bool  Fault{false};
  
  TCCR1B |= (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(1<<CS12)|(1<<CS11)|(0<<CS10); // Enable counter.
  
  digitalWrite(EN_Pin, HIGH);
  
  TCNT1 = 0;                              // Reset counter value.
  
  //while(TCNT1 == 0);                    // First ~24ms of ADC conversion time. Will get stuck if pin never changes. Can use millis or WTD but dont want to >:(
  for(int i = 0; i < 50; i++){            // Same as the row above but with a timeout. ~50ms timeout, fault is set if reached.    
    if (TCNT1 > 0){
      break;
    }
    else if(TCNT1 == 0 && i == 49){
      Fault = true;
    }    
    _delay_ms(1);
  }
  
  while(1){                             // Rest of the unknown transmission time. A 11 micro-second delay is longest possible pulse time.
    Temp = TCNT1;
    _delay_us(11);  
    if(Temp == TCNT1 || TCNT1 == 3201 || Fault == true){ // Check if transmission is finished by checking if the pulse train has stopped, reached max or skip if fault is detected.   
      TCCR1B = 0;                       // Disable counter.
      digitalWrite(EN_Pin, LOW);
      break;
    }
  }

  if(TCNT1 < 1 || TCNT1 > 3201 || Fault == true){// Sanity check, returns "huge" value
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
    
    //Changing unit, defaults to Celsius (0): 1 = Fahrenheit, 2 = Kelvin
    if(Unit == 'F'){
      Temp = ((Temp*1.8)+32);
    }
    else if(Unit == 'K'){
      Temp += 273.15;
      }
    }
  return Temp;
}


/*
float Get_Temp(uint8_t const TEMP_UNIT){

  float Temp = 0;

  if(NST_Mode == 0){                    // Pullup mode
    NST_EN_PORT = (1 << NST_EN_PIN);    // Enable pin high
  }
  else if(NST_Mode == 1){               // Pulldown mode
    NST_EN_PORT = (0 << NST_EN_PIN);    // Enable pin low
  }
  
  TCNT1 = 0;                            // Reset counter value
  _delay_ms(50);                        // Conversion + transmission delay
  Temp = ((TCNT1*0.0625) - 50.0625);    // Calculate temperature from sampled pulses

  if(NST_Mode == 0){                    // Pullup mode
    NST_EN_PORT = (0 << NST_EN_PIN);    // Enable pin low, Turning the sensor off
  }
  else if(NST_Mode == 1){               // Pulldown mode
    NST_EN_PORT = (1 << NST_EN_PIN);    // Enable pin High, Turning the sensor off 
  }
  
//Temp compensation, no compensation is needed in the interval 30 ≤ T ≤ 100 
  if(Temp < 30){
    Temp += ((Temp-30)*0.005);          // Compensatio for T < 30
  }
  else if (100 < Temp && Temp < 150){
    Temp += ((100-Temp)*0.012);         // Compensatio for 100 < T < 150
  }
  else if(Temp < -50 || Temp > 150 || TEMP_UNIT > 2){    // Sanity check, returns "huge" value
    Temp = 32500;
  }

//Changing uint, defaults to Celsius: 1 = Fahrenheit, 2 = Kelvin
  if(TEMP_UNIT == 1){
    Temp = ((Temp*1.8)+32);
  }
  else if(TEMP_UNIT == 2){
    Temp += 273.15;
  }
  return Temp;
}*/
