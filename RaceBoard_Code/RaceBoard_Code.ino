#include <GPS.h>
#include <ADC.h>
#include <IntPin.h>
#include <Macros.h>
#include <SDCustom.h>
#include <TelemetryLLCC68.h>

//----------------------Переменные для GPS------------------------------------->
bool GPS_Flag;  //Актуальность данных (true-данные новые и не считывались)
double Long;  //Долгота, в градусах 
double Lati;  //Широта, в градусах
double Sp;  //Горизонтальная скорость, в метрах в секунду 
double Alt;  //ВысотаБ в метрах в секунду
String Coord; //Строка с координатами и высотой
//----------------------------------------------------------------------------->

bool GPS_piling(double *Long, double  *Lati, double *Sp, double *Alt, String *Coord) {
  GPS_m GPS(9600);
  GPS.StartTrack();

  GPS_Flag=GPS.IsUpdated();
  *Long=GPS.Longitude();
  *Lati=GPS.Latitude();
  *Sp=GPS.Speed();
  *Alt=GPS.Altitude();
  *Coord=GPS.LonLatAlt();

  GPS.StopTrack();
  return GPS_Flag;
}


void setup() {
  

}

void loop() {
  
  GPS_Flag = GPS_piling(&Long, &Lati, &Sp, &Alt, &Coord);
  
                     

  
  

}
