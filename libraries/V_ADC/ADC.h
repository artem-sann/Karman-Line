
#ifndef ADC_h
#define ADC_h

#include <Arduino.h>

class ADC_m
{
public:
  ADC_m(uint8_t Pin_, float Div_)
  { //Конструктор, задаем пин, и множитель измерения
    analogReference(INTERNAL2V56);
    Pin = Pin_;
    Div = Div_;
    pinMode(Pin, INPUT);

  }
  float Result = 0.0; //Итоговое значение на пине

  float Measure(); //Производим измерение и пересчет

private:
  uint8_t Pin;
  uint16_t Buff;
  float Div;
};

/* Example
#include "src/ADC/ADC.h"

//ADC MODULE
ADC_m Photoresistor_V(A0,1.0);
ADC_m Battery_V(A1,1.91);
ADC_m Solar_mA(A2,58.823);

//ADC MODULE




float a =Photoresistor_V.Measure();
float b =Battery_V.Measure();
float c =Solar_mA.Measure();

*/

#endif
