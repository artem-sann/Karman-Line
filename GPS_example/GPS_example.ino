// Минимальный пример работы gps модуля платы RaceBoaard КБ INNOPOL`
#include <AutoUpdate.h>
#include <GPS.h>
#include <TinyGPS++.h>

//----------------------Переменные для GPS------------------------------------------------->
bool GPS_Flag;  //Актуальность данных (true-данные новые и не считывались)
double Long;    //Долгота, в градусах
double Lati;    //Широта, в градусах
double Sp;      //Горизонтальная скорость, в метрах в секунду
double Alt;     //ВысотаБ в метрах в секунду
String Coord;   //Строка с координатами и высотой
//----------------------------------------------------------------------------------------->

AutoUpd_m AUpd; 
GPS_m GPS(9600);
TinyGPSPlus GPSPlus; 

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

double startLong;
double startLati;
void setup() {
  GPS_Init();
  Serial.begin(9600);
  
}

void loop() {
  while(millis()<1000) {
    startLong = GPS.Longitude();
    startLati = GPS.Latitude();
  }

  GPS_Flag = GPS_piling(&Long, &Lati, &Sp, &Alt, &Coord);
  print_GPS(&Long, &Lati, &Sp, &Alt, &Coord, &GPS_Flag);

  Serial.println(GPSPlus.distanceBetween(startLati, startLong, Lati, Long));
  
  delay(1000);
}
