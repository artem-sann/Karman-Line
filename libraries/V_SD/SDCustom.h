#ifndef  SDCustom_h
#define  SDCustom_h

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


class SDCard_m{
    public:
        SDCard_m(){} //Конструктор
    bool begin(String msg_); //Инициальзация карты, и запись первой строки
    void print(String msg_); //Запись строки в карту, без символа переноса
    void println(String msg_); //Запись строки в карту, с символом переноса
    void WriteIntDouble(int32_t *msg_,uint8_t len_i, double* msg__,uint8_t len_d); //Запись int (затем) double массивов (LittleEndian)
    void end(); //Завершение работы с картой памяти

    void upd();
    private:
};

/* Example
#include "src/SD/SDCustom.h"
SDCard_m Card;

Card.begin("");
Card.println("");
Card.WriteIntDouble(msg_i,2,msg_d,2);

*/

#endif
