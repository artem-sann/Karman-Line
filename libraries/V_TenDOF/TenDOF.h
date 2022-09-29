#ifndef  TenDOF_h
#define  TenDOF_h

#include <Arduino.h>
#include "BMP280.h"
#include "MPU9250.h"


#define BarVolt 0.35f;  // среднее отклонение (ищем в excel)
#define BarProcess 0.005f; // скорость реакции на изменение (подбирается вручную)


//Блок BMP280
struct TenDOF_d
{
    float varVolt = 0;  
    float varProcess = 0; 
    float Ext_H =0;
    float Pc = 0.0;
    float G = 0.0;
    float P = 1.0;
    float Xp = 0.0;
    float Zp = 0.0;
    float Xe = 0.0;

    double HeigtHist[10];
    double VertSpeed=0;
    float Height=0;
    bool Upd = false;
};



class Barometer_m{
    public:
        Barometer_m(){} //Конструктор

        bool begin();   //Инициализровать барометр
        double GetTemp(); //Получить температуру, в градусах цельсия
        double GetHeight(); //Получить высоту, в метрах
        double GetHeightK(); //Получить высоту, сглаженную фильром Калмана
        void SetZeroHeight(); //Установить нулевую высоту
        double GetVertSpeed(); //Получить вертикальную скорось (требуется включить автообновление барометра)

        bool Upd(); //Установить в loop() без delay(), по флагу с прерывания zанимается обработкой данных 
        bool Upd(double h_); //Для эмуляции полета
    private:
        float ZeroHeight=0;
};



//Блок MPU9250


class IMU_m
{
    public:
        IMU_m(){} //Конструктор

        bool begin(); //Инициальзация датчика
        bool GetAccel(double * bf_); //Получить вектор данных с акселерометра [x,y,z,Sqrt] 
        bool GetGyro(double * bf_); //Получить вектор данных с гироскопа [x,y,z]
        bool GetMag(double * bf_); //Получить вектор данных с магнитометра [x,y,z]

        void WhileCalib();  //Запуск калибровки, выполняется бесконечно
        void SetCalibAccel(double * ac1,double * ac2,double * ac3,double * ac4); //Установить калибровочные коэффициенты (см документацию)
        void SetCalibGyro(double * clb_); 
        void SetCalibMag(double * ma1,double * ma2,double * ma3,double * ma4);
    private:
        double CalibAccel[4][3]={
                {0,0,0},  //ofsett
                {1,0,0},   //first line
                {0,1,0},   //second line
                {0,0,1}};  //third line
        double CalibGyro[3]={0,0,0};  //ofsett
        double CalibMag[4][3]={
                {0,0,0},  //ofsett
                {1,0,0},   //first line
                {0,1,0},   //second line
                {0,0,1}};  //third line

};






/*Example Barom

#include "src/TenDOF/TenDOF.h"
Barometer_m Bar;

//Barometer
Bar.begin();
Bar.SetZeroHeight();

Bar.Upd();
Serial.print(Bar.GetTemp()); Serial.print(" : ");
Serial.print(Bar.GetHeight()); Serial.print(" : ");
Serial.print(Bar.GetHeightK());Serial.print(" : ");
Serial.println(Bar.GetVertSpeed()); 
*/

/*Example IMU

IMU_m IMU;
double accel[4];
double gyro[3];
double mag[3];

double CalibAccel[4][3]={
          {0,0,0},  //ofsett
          {1,0,0},   //first line
          {0,1,0},   //second line
          {0,0,1}};  //third line
double CalibGyro[3]={0,0,0};  //ofsett
double CalibMag[4][3]={
          {0,0,0},  //ofsett
          {1,0,0},   //first line
          {0,1,0},   //second line
          {0,0,1}};  //third line

IMU.begin();
IMU.SetCalibAccel(CalibAccel[0],CalibAccel[1],CalibAccel[2],CalibAccel[3]);
IMU.SetCalibGyro(CalibGyro);
IMU.SetCalibMag(CalibMag[0],CalibMag[1],CalibMag[2],CalibMag[3]);

IMU.GetAccel(accel);
IMU.GetGyro(gyro);
IMU.GetMag(mag);

//IMU.WhileCalib();

Serial.print(accel[0]); Serial.print(":");
Serial.print(accel[1]); Serial.print(":");
Serial.print(accel[2]); Serial.print(":");

Serial.print(gyro[0]); Serial.print(":");
Serial.print(gyro[1]); Serial.print(":");
Serial.print(gyro[2]); Serial.print(":");

Serial.print(mag[0]); Serial.print(":");
Serial.print(mag[1]); Serial.print(":");
Serial.println(mag[2]);
*/

#endif