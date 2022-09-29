#include "IntPin.h"

#define Jumper_pin A8

#define ButtonOne_pin A10
#define ButtonTwo_pin A9

#define CameraOne_pin A12
#define CameraTwo_pin A14
#define CameraThree_pin A13
#define CameraFour_pin 13

#define DCMotorINT_pin 14

#define SDCardExINT_pin 15


//High level
Inter_pin Jumper;
Inter_pin ButtonOne;
Inter_pin ButtonTwo;
Inter_pin CameraOne;
Inter_pin CameraTwo;
Inter_pin CameraThree;
Inter_pin CameraFour;
Inter_pin DCMotor;
Inter_pin SDCardEx;

bool pinRead(uint8_t pin);
void Smachine(Inter_pin* struct_);


long SetTime =0;
uint8_t OldPin=13;

void Interrupt_m::JumperINT(void (ff_)(), uint8_t trig_)
{
  Jumper.Pin=Jumper_pin;
  Setup(&Jumper,ff_,trig_);

}

void Interrupt_m::ButtonOneINT(void (ff_)(), uint8_t trig_)
{
  ButtonOne.Pin=ButtonOne_pin;
  Setup(&ButtonOne,ff_,trig_);
}

void Interrupt_m::ButtonTwoINT(void (ff_)(), uint8_t trig_)
{
  ButtonTwo.Pin=ButtonTwo_pin;
  Setup(&ButtonTwo,ff_,trig_);
}

void Interrupt_m::CameraOneINT(void (ff_)(), uint8_t trig_)
{
  CameraOne.Pin=CameraOne_pin;
  Setup(&CameraOne,ff_,trig_);
}

void Interrupt_m::CameraTwoINT(void (ff_)(), uint8_t trig_)
{
  CameraTwo.Pin=CameraTwo_pin;
  Setup(&CameraTwo,ff_,trig_);
}

void Interrupt_m::CameraThreeINT(void (ff_)(), uint8_t trig_)
{
  CameraThree.Pin=CameraThree_pin;
  Setup(&CameraThree,ff_,trig_);
}

void Interrupt_m::CameraFourINT(void (ff_)(), uint8_t trig_)
{
  CameraFour.Pin=CameraFour_pin;
  Setup(&CameraFour,ff_,trig_);
}

void Interrupt_m::DCMotorINT(void (ff_)(), uint8_t trig_)
{
  DCMotor.Pin=DCMotorINT_pin;
  Setup(&DCMotor,ff_,trig_);
}

void Interrupt_m::SDCardExINT(void (ff_)(), uint8_t trig_)
{
  SDCardEx.Pin=SDCardExINT_pin;
  Setup(&SDCardEx,ff_,trig_);
}


void Interrupt_m::Setup(Inter_pin* struct_,void (ff_)(), uint8_t trig_)
{
  pinMode(struct_->Pin,INPUT_PULLUP);
  attachPCINT(struct_->Pin);
  struct_->Flag=true;
  struct_->Ttriger=trig_;
  struct_->ISR_=ff_;
  struct_->Old=pinRead(struct_->Pin);
}


//Low level
void Interrupt_m::attachPCINT(uint8_t pin) 
{
  if (pin >= A8 and pin <=A15) {            // (PCINT2)
      PCICR |= (1 << PCIE2);
      PCMSK2 |= (1 << pin-A8);} 
    else if(pin >= 10 and pin <=13) {        //  (PCINT0)
      PCICR |= (1 << PCIE0);
      PCMSK0 |= (1 << pin-6); }
    else if(pin == 14 || pin==15) {        //  (PCINT1)
      PCICR |= (1 << PCIE1);
      PCMSK1 |= (1 << 16-pin);
  }

}

bool pinRead(uint8_t pin) {
  if (pin >= A8 and pin <=A15) { 
    return bitRead(PINK, pin-A8);}
  else if(pin >= 10 and pin <=13) {
    return bitRead(PINB, pin - 6);}
  else if(pin == 14 || pin==15) {
    return bitRead(PINJ, 15-pin);
  }
}


//Interrupts
ISR(PCINT0_vect) {  // 10-13
    CameraFour.Flag?Smachine(&CameraFour):yield();
}
ISR(PCINT1_vect) {  // 14,15
    DCMotor.Flag?Smachine(&DCMotor):yield();
    SDCardEx.Flag?Smachine(&SDCardEx):yield();
}
ISR(PCINT2_vect){ //A8-A15
    Jumper.Flag?Smachine(&Jumper):yield();
    ButtonOne.Flag?Smachine(&ButtonOne):yield();
    ButtonTwo.Flag?Smachine(&ButtonTwo):yield();
    CameraOne.Flag?Smachine(&CameraOne):yield();
    CameraTwo.Flag?Smachine(&CameraTwo):yield();
    CameraThree.Flag?Smachine(&CameraThree):yield();
}


void Smachine(Inter_pin* struct_)
{
    uint8_t buff=pinRead( struct_->Pin);
    if(buff!=struct_->Old)
    {
      struct_->Old=buff;
      if( (struct_->Ttriger==CHANGE) || (struct_->Ttriger-2==buff) )
      {
        struct_->ISR_();
      }
    }
}
