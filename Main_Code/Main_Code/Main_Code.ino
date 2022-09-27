#include <AutoUpdate.h>
#include <GPS.h>
#include <BMP280.h>
#include <MPU9250.h>
#include <TenDOF.h>
#include <TelemetryLLCC68.h>
#include <TinyGPS++.h>
#include <Timers.h>
#include <IntPin.h>
#include <SDCustom.h>
#include <SD.h>

//---------------------Пины SD карты------------------------------------------------------->
#define MISO 50
#define MOSI 51
#define SCLK 52
#define CS_pin 37 
//----------------------------------------------------------------------------------------->

#define Buzzer_Pin 4           //Пин к которому подключен бузер
#define Buzzer_Frequency 2000  //Частота бузера в Гц

#define BuzzerOn tone(Buzzer_Pin, Buzzer_Frequency)              //Активировать бузер
#define BuzzerOnTime(x) tone(Buzzer_Pin, Buzzer_Frequency, (x))  //Активировать бузер на некоторое время в мс
#define BuzzerOff noTone(Buzzer_Pin)   

#define DCMotorPin1 6   //Пин 1-го входа мотора V0.1R && V0.2
#define DCMotorPin2 7   //Пин 2-го входа мотора
#define DCWakeUPPin 41  //Пин запуска драйвера двигателя

TinyGPSPlus GPSPlus; 
Barometer_m Bar;
IMU_m IMU;

TelemetryLLCC68_m Telemetry;
String msg;
double msg_d[10];
int32_t msg_i[10];

AutoUpd_m AUpd; 
GPS_m GPS(9600);

bool SS_flag = false;
//----------------------Переменные для GPS------------------------------------------------->
bool GPS_Flag;  //Актуальность данных (true-данные новые и не считывались)
double Long;    //Долгота, в градусах
double Lati;    //Широта, в градусах
double Sp;      //Горизонтальная скорость, в метрах в секунду
double Alt;     //ВысотаБ в метрах в секунду
String Coord;   //Строка с координатами и высотой
double Delta_S;
double StartLati;
double StartLong;
//----------------------------------------------------------------------------------------->

double accel[4];
double gyro[3];
double mag[3];
double Temp;
double Height;
double Height_K;
double VertSpeed;


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
    dataFile.print("Alt");
    dataFile.print(",");
    dataFile.print("Sp");
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
bool SD_data_Write(double *Temp, double *Height_K, double *VertSpeed, double *accel,  //судя по всему функция выполняется 0.2 секунды
                    double *gyro, String Coord, double *Sp, bool SS_flag) {
  
  long time_to_SD = millis(); // милисекунды
  File dataFile = SD.open("logs.csv", FILE_WRITE);

  if (dataFile) { 
    dataFile.print(time_to_SD);
    dataFile.print(",");
    dataFile.print(*Temp);
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
    dataFile.print(Coord);
    dataFile.print(",");
    dataFile.print(*Sp);
    dataFile.print(",");
    dataFile.println(SS_flag);

    dataFile.close();
  }
  else {
    Serial.println("FAILLLLLL");
  }
}



void GPS_Init(){
  GPS.StartTrack();
  AUpd.enableTimer();
  AUpd.GPSEn(30);
} 

bool GPS_piling(double *Long, double *Lati, double *Sp, double *Alt, String *Coord) {
  
  GPS_Flag = GPS.IsUpdated();
  *Long = GPS.Longitude();
  *Lati = GPS.Latitude();
  *Sp = GPS.Speed();
  *Alt = GPS.Altitude();
  *Coord = GPS.LonLatAlt();
   
  return GPS_Flag;
}

void DCMotorSetSpeed(int y) {
  int x = constrain(y, -100, 100);
  x >= 0 ? digitalWrite(DCMotorPin2, HIGH) : digitalWrite(DCMotorPin1, HIGH);
  x >= 0 ? analogWrite(DCMotorPin1, 255 - (uint8_t)(x * 2.55)) : analogWrite(DCMotorPin2, 255 - (uint8_t)(-x * 2.55));
  //Управление вращением мотора , скорость в % от -100 до 100
}



void DCMotorInit() {
  pinMode(DCMotorPin1, OUTPUT);
  pinMode(DCMotorPin2, OUTPUT);
  pinMode(DCWakeUPPin, OUTPUT);
  digitalWrite(DCWakeUPPin, HIGH);
  //Инициализациия мотора
}



void print_Telemetry(double *Temp, double *Height, double *Height_K, double *VertSpeed, double *Accel_1, double *Accel_2, double *Accel_3, double *Gyro_1, double *Gyro_2, double *Gyro_3, double *Mag_1, double *Mag_2, double *Mag_3) {

  Serial.println("\nTemp \tHeight \tHeight_K \tVertSpeed \tAccel_1 \tAccel_2 \tAccel_3 \tGyro_1 \tGyro_2 \tGyro_3 \tMag_1 \tMag_2 \tMag_3");
  Serial.println("----------------------------------------------------------------------------------------------------------------------------------------------");
  Serial.print(*Temp);
  Serial.print("\t"); 
  Serial.print(*Height);
  Serial.print("\t");
  Serial.print(*Height_K);
  Serial.print("\t\t");
  Serial.print(*VertSpeed);
  Serial.print("\t\t");
  Serial.print(*Accel_1);
  Serial.print("\t\t");
  Serial.print(*Accel_2);
  Serial.print("\t\t");
  Serial.print(*Accel_3);
  Serial.print("\t\t");
  Serial.print(*Gyro_1);
  Serial.print("\t");
  Serial.print(*Gyro_2);
  Serial.print("\t");
  Serial.print(*Gyro_3);
  Serial.print("\t");
  Serial.print(*Mag_1);
  Serial.print("\t");
  Serial.print(*Mag_2);
  Serial.print("\t");
  Serial.print(*Mag_3);
  Serial.print("\t\t");
  

} 

String MessageParser(double Height, double Long, double Lati, double Delta_S, double VertSpeed) {
  String answer = ("H: " + String(Height,3) + " Crd: " + String(Long,5)+" "+String(Lati,5) + " dS: " + String(Delta_S,2) + "        v: " + String(VertSpeed, 2));
  return answer;

}


void setup() {

  DCMotorInit();

  GPS_Init();

  Serial.begin(9600);
  Bar.begin();
  Bar.SetZeroHeight();
  IMU.begin();


  Serial.begin(9600);
  SD_begin();
  if (!SD.begin(CS_pin)) {
    Serial.println("Card Failure");
  }
  else {
    Serial.println("Card Ready");
  }
  SD_Header_write();

  //------------------Telemetry init--------->
  Serial.begin(9600);
  Telemetry.on();
  Telemetry.SetMaxPower();
  Telemetry.SetChannel(Channel_2);
  //---------------------------------------->
  BuzzerOnTime(200);
  delay(300);

}
//-------------------------------переменные для системы спасения---------------------->
bool flag_1st_point = true;
bool flag_Axel_null = false;
double 1st_Height = -100;
double maxHight_calculated = 270; // предполагаемая максимальная высота полета минус 5 метров

//------------------------------------------------------------------------------------>

void loop() {
  //калибровка начального положения gps------------------------------------------------>
  while (millis()<2000) {
    StartLong = GPS.Longitude();
    StartLati = GPS.Latitude();
  }

  // снятие данных со всех датчиков---------------------------------------------------->
  IMU.SetCalibAccel(CalibAccel[0],CalibAccel[1],CalibAccel[2],CalibAccel[3]);
  IMU.SetCalibGyro(CalibGyro);
  IMU.SetCalibMag(CalibMag[0],CalibMag[1],CalibMag[2],CalibMag[3]);

  IMU.GetAccel(accel);
  IMU.GetGyro(gyro);
  IMU.GetMag(mag);
  Bar.Upd();
  AUpd.BarEn(10);
  
  Temp = Bar.GetTemp();
  Height = Bar.GetHeight();
  Height_K = Bar.GetHeightK();
  VertSpeed = Bar.GetVertSpeed();

  GPS_piling(&Long, &Lati, &Sp, &Alt, &Coord);
  Delta_S = GPSPlus.distanceBetween(StartLati, StartLong, Lati, Long);

  // запись снятых данных в файл------------------------------------------------------->
  SD_data_Write(&Temp, &Height_K, &VertSpeed, accel, gyro, Coord, &Sp, SS_flag);

  // отправка полученных данных через радио ------------------------------------------->
  msg = MessageParser(Height_K,  Long,  Lati,  Delta_S, VertSpeed);
  Telemetry.SendS(msg);

  BuzzerOnTime(20);

  //-------------------------система спасения------------------------------------------>
  if ((Height_K => maxHight_calculated) and (flag_1st_point)) {
    1st_Height = Height_K;
    flag_1st_point = false;
  }

  if ((Height_K < 1st_Height) and (flag_Axel_null)) {
    //активация сс
  }

  if ((abs(axel[3]) < 0.1) {
    flag_Axel_null = true;
  }

}
