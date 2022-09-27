#include <AutoUpdate.h>
#include <Timers.h>
#include <GPS.h>
#include <ADC.h>
#include <IntPin.h>
#include <SDCustom.h>
#include <TelemetryLLCC68.h>
#include <SD.h>

bool SS_flag;
//----------------------Переменные для GPS------------------------------------------------->
bool GPS_Flag;  //Актуальность данных (true-данные новые и не считывались)
double Long;    //Долгота, в градусах
double Lati;    //Широта, в градусах
double Sp;      //Горизонтальная скорость, в метрах в секунду
double Alt;     //ВысотаБ в метрах в секунду
String Coord;   //Строка с координатами и высотой
//----------------------------------------------------------------------------------------->
//----------------------Переменные для IMU------------------------------------------------->
double accel[3];
double gyro[3];
double mag[3];
double Temp;
double Height;
double Height_K;
double VertSpeed;
double Accel_1;
double Accel_2;
double Accel_3;
double Gyro_1;
double Gyro_2;
double Gyro_3;
double Mag_1;
double Mag_2;
double Mag_3;
//----------------------------------------------------------------------------------------->
//---------------------Пины SD карты------------------------------------------------------->
#define MISO 50
#define MOSI 51
#define SCLK 52
#define CS_pin 37 
//----------------------------------------------------------------------------------------->

GPS_m GPS(9600);

//----------------------Функции для SD----------------------------------------------------->
void SD_begin() {
  pinMode(53,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(CS_pin, OUTPUT);
  
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(53,HIGH);
}

bool SD_Header_write() {
  File dataFile = SD.open("logs.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Time");
    dataFile.print(",");
    dataFile.print("Temp");
    dataFile.print(",");
    dataFile.print("Height");
    dataFile.print(",");
    dataFile.print("Height_K");
    dataFile.print(",");
    dataFile.print("VertSpeed");
    dataFile.print(",");
    dataFile.print("accel_X");
    dataFile.print(",");
    dataFile.print("accel_Y");
    dataFile.print(",");
    dataFile.print("accel_Z");
    dataFile.print(",");
    dataFile.print("gyro_X");
    dataFile.print(",");
    dataFile.print("gyro_Y");
    dataFile.print(",");
    dataFile.print("gyro_Z");
    dataFile.print(",");
    dataFile.print("Long");
    dataFile.print(",");
    dataFile.print("Lati");
    dataFile.print(",");
    dataFile.print("Sp");
    dataFile.print(",");
    dataFile.print("Alt");
    dataFile.print(",");
    dataFile.println("SS_flag");

    dataFile.close();
    return true;
  }
  else {
    Serial.println("FAILLLLLL");
    return false;
  }


}
bool SD_data_Write(double *Temp, double *Height, double *Height_K, double *VertSpeed, double *accel,  //судя по всему функция выполняется 0.2 секунды
                    double *gyro, double *Long, double *Lati, double *Sp, double *Alt, bool SS_flag) {
  
  long time_to_SD = millis(); // милисекунды
  File dataFile = SD.open("logs.csv", FILE_WRITE);

  if (dataFile) { 
    dataFile.print(time_to_SD);
    dataFile.print(",");
    dataFile.print(*Temp);
    dataFile.print(",");
    dataFile.print(*Height);
    dataFile.print(",");
    dataFile.print(*Height_K);
    dataFile.print(",");
    dataFile.print(*VertSpeed);
    dataFile.print(",");
    dataFile.print(accel[0]);
    dataFile.print(",");
    dataFile.print(accel[1]);
    dataFile.print(",");
    dataFile.print(accel[2]);
    dataFile.print(",");
    dataFile.print(gyro[0]);
    dataFile.print(",");
    dataFile.print(gyro[1]);
    dataFile.print(",");
    dataFile.print(gyro[2]);
    dataFile.print(",");
    dataFile.print(*Long);
    dataFile.print(",");
    dataFile.print(*Lati);
    dataFile.print(",");
    dataFile.print(*Sp);
    dataFile.print(",");
    dataFile.print(*Alt);
    dataFile.print(",");
    dataFile.println(SS_flag);

    dataFile.close();
  }
  else {
    Serial.println("FAILLLLLL");
  }
}



void setup() {
  
  Serial.begin(9600);
  SD_begin();
  if (!SD.begin(CS_pin)) {
    Serial.println("Card Failure");
  }
  else {
    Serial.println("Card Ready");
  }

  SD_Header_write();

}

void loop() {
  SD_data_Write(&Temp, &Height, &Height_K, &VertSpeed, accel, gyro, &Long, &Lati, &Sp, &Alt, SS_flag);
  delay(10);

  
}