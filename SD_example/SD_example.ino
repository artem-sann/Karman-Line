#include <AutoUpdate.h>
#include <Timers.h>
#include <GPS.h>
#include <ADC.h>
#include <IntPin.h>
#include <SDCustom.h>
#include <TelemetryLLCC68.h>
#include <SD.h>


AutoUpd_m AUpd; 

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

bool SD_data_Write() {
  File dataFile = SD.open("logs.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(time_to_SD);
    dataFile.print(", ");
    dataFile.println(Coord);
    dataFile.close();
  }
  else {
    Serial.println("FAILLLLLL");
  }
  

}


//----------------------Функции для SS----------------------------------------------------->

void save_event(int delay) {


}
  

//----------------------Переменные для GPS------------------------------------------------->
bool GPS_Flag;  //Актуальность данных (true-данные новые и не считывались)
double Long;    //Долгота, в градусах
double Lati;    //Широта, в градусах
double Sp;      //Горизонтальная скорость, в метрах в секунду
double Alt;     //ВысотаБ в метрах в секунду
String Coord;   //Строка с координатами и высотой
//----------------------------------------------------------------------------------------->

//-----------------------------Функции по работе с GPS------------------------------------->
bool GPS_piling(double *Long, double *Lati, double *Sp, double *Alt, String *Coord) {
  
  
  

  GPS_Flag = GPS.IsUpdated();
  *Long = GPS.Longitude();
  *Lati = GPS.Latitude();
  *Sp = GPS.Speed();
  *Alt = GPS.Altitude();
  *Coord = GPS.LonLatAlt();

  
  return GPS_Flag;
}


void print_GPS(double *Long, double *Lati, double *Sp, double *Alt, String *Coord, bool *GPS_Flag) {

  Serial.println(F("\nSpeed \tLongitude \tLatitude \tAltitude \tCoord \t\t\t\tGPS_Flag"));
  Serial.println(F("---------------------------------------------------------------------------------------------------"));
  Serial.print(*Sp);
  Serial.print(F("\t")); 
  Serial.print(*Long);
  Serial.print(F("\t\t"));
  Serial.print(*Lati);
  Serial.print(F("\t\t"));
  Serial.print(*Alt);
  Serial.print(F("\t\t"));
  Serial.print(*Coord);
  Serial.print(F("\t\t"));
  Serial.println(*GPS_Flag);

}
//----------------------------------------------------------------------------------------->

void setup() {
  GPS.StartTrack();
  AUpd.enableTimer();
  AUpd.GPSEn(30);
  Serial.begin(9600);
  SD_begin();
  if (!SD.begin(CS_pin)) {
    Serial.println("Card Failure");
  }
  else {
    Serial.println("Card Ready");
  }
}

void loop() {
  

  long time_to_SD = millis();
  GPS_Flag = GPS_piling(&Long, &Lati, &Sp, &Alt, &Coord);
  print_GPS(&Long, &Lati, &Sp, &Alt, &Coord, &GPS_Flag);

  delay(1000);
}