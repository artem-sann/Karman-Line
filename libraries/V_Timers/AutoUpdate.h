#ifndef AutoUpdate_h
#define AutoUpdate_h


#include <Arduino.h>
#include "Timers.h"
#include <GPS.h> //Закоментрироать если не используется
#include <SDCustom.h> //Закоментрироать если не используется
#include <TenDOF.h> //Закоментрироать если не используется

#ifdef SDCustom_h
      extern void SD_upd_timer();  
#endif
#ifdef GPS_h
      extern void GPS_upd_timer();  
#endif
#ifdef TenDOF_h
      extern void BMP_upd_timer();  
#endif

struct AutoUpdate_d
{
    bool Enable=false;
    uint16_t TIck=0;
    uint16_t TIckNom=1000;
};
uint16_t GlobalTick=0;



AutoUpdate_d GPS_d;
AutoUpdate_d SD_d;
AutoUpdate_d Bar_d;


class AutoUpd_m
{
    public:
        AutoUpd_m(){ //Консруктор, включаем таймер5
    }
        void GPSEn(uint8_t freq_);  //Активировать автопарсинг данных с GPS
        void GPSDis();

        void SDEn(uint8_t freq_); //Активироать автосохранение данных на SDcard
        void SDDis();

        void BarEn(uint8_t freq_); //Активировать автосбор данных и расчет скорости
        void BarDis();

        void enableTimer(); //Включить таймер
        void disableTimer();
};



void AutoUpd_m::enableTimer()
{
    Timer5.setFrequency(1000);
    Timer5.enableISR(CHANNEL_A); 
}
void AutoUpd_m::disableTimer()
{
    Timer5.stop();
}

void AutoUpd_m::GPSEn(uint8_t freq_)
{
    GPS_d.Enable=true;
    GPS_d.TIck=0;
    GPS_d.TIckNom=1000/freq_;
}
void AutoUpd_m::GPSDis()
{
    GPS_d.Enable=false;
}

void AutoUpd_m::SDEn(uint8_t freq_)
{
    SD_d.Enable=true;
    SD_d.TIck=0;
    SD_d.TIckNom=1000/freq_;
}
void AutoUpd_m::SDDis()
{
    SD_d.Enable=false;
}

void AutoUpd_m::BarEn(uint8_t freq_)
{
    Bar_d.Enable=true;
    Bar_d.TIck=0;
    Bar_d.TIckNom=1000/freq_;
}
void AutoUpd_m::BarDis()
{
    Bar_d.Enable=false;
}


ISR(TIMER5_A) { 
    GlobalTick<1000?GlobalTick++:GlobalTick=0;

    if(GPS_d.Enable)
    {
        GPS_d.TIck++;
        if(GPS_d.TIck>=GPS_d.TIckNom)
        { 
            #ifdef GPS_h
            GPS_upd_timer();
            #endif
            GPS_d.TIck=0;
        }
    }

    if(SD_d.Enable)
    {
        SD_d.TIck++;
        if(SD_d.TIck>=SD_d.TIckNom)
        { 
            #ifdef SDCustom_h
            SD_upd_timer();  
            #endif
            SD_d.TIck=0;
        }
    }

    if(Bar_d.Enable)
    {
        Bar_d.TIck++;
        if(Bar_d.TIck>=Bar_d.TIckNom)
        { 
            #ifdef TenDOF_h
            BMP_upd_timer();  
            #endif
            Bar_d.TIck=0;
        }
    }




}



/*Example
#include "src/Timers/AutoUpdate.h"

AutoUpd_m AUpd; 

AUpd.enableTimer();
AUpd.GPSEn(30);
AUpd.SDEn(5);
AUpd.BarEn(10);



*/




#endif