#include <GPS.h>
#include <ADC.h>
#include <IntPin.h>
#include <Macros.h>
#include <SDCustom.h>
#include <TelemetryLLCC68.h>

bool GPS_Flag;
double Long;
double Lati;
double Sp;
double Alt;
String Coord;

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
