#include "ADC.h"


float ADC_m::Measure(){
   Buff = analogRead(Pin);
   Result = Buff*2.56/1024.0;
   Result *= Div;
   return Result;
}
