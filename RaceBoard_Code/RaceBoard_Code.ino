#include <GPS.h>
#include <ADC.h>
#include <IntPin.h>
#include <Macros.h>
#include <SDCustom.h>
#include <TelemetryLLCC68.h>

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
  GPS_m GPS(9600);
  GPS.StartTrack();

  GPS_Flag = GPS.IsUpdated();
  *Long = GPS.Longitude();
  *Lati = GPS.Latitude();
  *Sp = GPS.Speed();
  *Alt = GPS.Altitude();
  *Coord = GPS.LonLatAlt();

  GPS.StopTrack();
  return GPS_Flag;
}


void print_GPS(double *Long, double *Lati, double *Sp, double *Alt, String *Coord, bool *GPS_Flag) {

  Serial.println("\nSpeed \tLongitude \tLatitude \tAltitude \tCoord \t\t\t\tGPS_Flag");
  Serial.println("---------------------------------------------------------------------------------------------------");
  Serial.print(*Sp);
  Serial.print("\t"); 
  Serial.print(*Long);
  Serial.print("\t\t");
  Serial.print(*Lati);
  Serial.print("\t\t");
  Serial.print(*Alt);
  Serial.print("\t\t");
  Serial.print(*Coord);
  Serial.print("\t\t");
  Serial.println(*GPS_Flag);

}
//----------------------------------------------------------------------------------------->

void setup() {
  Serial.begin(9600);
}

void loop() {

  GPS_Flag = GPS_piling(&Long, &Lati, &Sp, &Alt, &Coord);
  print_GPS(&Long, &Lati, &Sp, &Alt, &Coord, &GPS_Flag);

  delay(1000);
}