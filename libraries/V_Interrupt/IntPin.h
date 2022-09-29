#ifndef IntPin_h
#define IntPin_h

#include <Arduino.h>

struct Inter_pin
{
    uint8_t Pin;
    bool Flag=false;
    uint8_t Ttriger;
    uint8_t Old;
    void (*ISR_)()=NULL;
};



class Interrupt_m
{   
    public:
        Interrupt_m(){//Конструктор

        } 

        void JumperINT(void (ff_)(), uint8_t trig_); //Активация прерываний на стартовом джампере
                                                // (функция+тригер)(см пример ниже)
        void ButtonOneINT(void (ff_)(), uint8_t trig_); //Активация прерываний на кнопке (1) 
        void ButtonTwoINT(void (ff_)(), uint8_t trig_); //Активация прерываний на кнопке (2) 
        void CameraOneINT(void (ff_)(), uint8_t trig_); //Активация прерываний на пине камеры(1)
        void CameraTwoINT(void (ff_)(), uint8_t trig_); //Активация прерываний на пине камеры(2)
        void CameraThreeINT(void (ff_)(), uint8_t trig_); //Активация прерываний на пине камеры(3)
        void CameraFourINT(void (ff_)(), uint8_t trig_); //Активация прерываний на пине камеры(4)
        void DCMotorINT(void (ff_)(), uint8_t trig_); //Активация прерываний по превышению потребления тока мотором
        void SDCardExINT(void (ff_)(), uint8_t trig_); //Активация прерываний по обнаружению карты памяти
    private:
        void attachPCINT(uint8_t pin);
        void Setup(Inter_pin* struct_,void (ff_)(), uint8_t trig_);


};


/* Example
#include "src/Interrupt/IntPin.h"

Interrupt_m Inter;

Inter.JumperINT(led1,CHANGE);
Inter.ButtonOneINT(led2,RISING);
Inter.ButtonTwoINT(led3,FALLING);
Inter.CameraOneINT(led4,RISING);
Inter.CameraTwoINT(led5,CHANGE);
Inter.CameraThreeINT(led6,FALLING);
Inter.CameraFourINT(led7,CHANGE);

*/



#endif
