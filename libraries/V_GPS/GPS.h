
#ifndef GPS_h
#define GPS_h

#include <Arduino.h>


class GPS_m
{
    public:
        GPS_m(uint32_t Speed_)
        {
            Speed_s = Speed_; //Конструктор, задаем скорость обмена данными
        }
        
        void StartTrack(); //Начать обработку данных
        void StopTrack(); //Остановить обработку данных
        bool IsUpdated(); //Актуальность данных (true-данные новые и не считывались)
        double Longitude(); //Долгота, в градусах
        double Latitude(); //Широта, в градусах
        double Speed(); //Горизонтальная скорость, в метрах в секунду
        double Altitude(); //ВысотаБ в метрах в секунду
        String LonLatAlt(); //Строка с координатами и высотой

        bool existRec =false;

    private:
        uint32_t Speed_s;

};

/* Example
#include "src/GPS/GPS.h"

GPS_m GPS(9600);

GPS.StartTrack();

bool a=GPS.IsUpdated();
double b=GPS.Longitude();
double c=GPS.Latitude();
double d=GPS.Speed();
double e=GPS.Altitude();
String f=GPS.LatLonAlt();

GPS.StopTrack();
*/


#endif
