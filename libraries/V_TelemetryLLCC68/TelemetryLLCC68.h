#ifndef Telemetry_h
#define Telemetry_h

#include <Arduino.h>


typedef enum {Channel_1,Channel_2,Channel_3,Channel_4,Channel_5,Channel_6} Channels;



class TelemetryLLCC68_m
{
    public:
        TelemetryLLCC68_m()//Конструктор
        {} 
        uint8_t on(); //Включить радиомодуль
        void off(); //Выключить радиомодуль
        void SetLowPower(); //Режим низкой мощности 
        void SetMaxPower(); //Режим максимальной мощности
        void SetChannel(Channels ch_); //Установить канал 1-6
        //Send
        void SendS(String msg_); //Отправка строки в блокирующем режиме
        void SendTelemetryPack(bool start_, bool par_,bool par_speed, double height_, double speed_,
                                                         double Lon_,    double Lat_); //Отправляет пакет отслеживания на приемную станцию
        //Recive
        bool AvailablePacket(); //Проверка наличия целостных сообщений
        String ReciveS(); //Прием строки в блокирующем режиме
        String ParceTelemetryPack(); //Принимает пакет отслеживания
    private:
        void init(long freq);
        int8_t powerTx = 10;
        uint8_t initialT=0;
};


/* Example
#include "src/Telemetry/Telemetry.h"

Telemetry_m Telemetry;
String msg;
double msg_d[10];
int32_t msg_i[10];

//
Telemetry.on();
for (uint8_t i = 0; i < 10; i++){ msg_d[i]=i*2.25;}
for (uint8_t i = 0; i < 10; i++){ msg_i[i]=i*3;}

Telemetry.SetLowPower();
Telemetry.SetMaxPower();

Telemetry.SendS(msg);
Telemetry.SendIntDouble(msg_i,msg_d);

Telemetry.AsyncOn();

Telemetry.AsyncSendS(msg);
Telemetry.AsyncSendIntDouble(msg_i,msg_d);

msg= Telemetry.ReciveS();
Telemetry.ReciveIntDouble(msg_i,msg_d);
//

Telemetry.off();
*/

#endif
