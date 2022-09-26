#include <AutoUpdate.h>
#include <GPS.h>

AutoUpd_m AUpd; 
GPS_m GPS(9600);

void setup() {
  GPS.StartTrack();
  AUpd.enableTimer();
  AUpd.GPSEn(30);
  Serial.begin(9600);
}

void loop() {
  Serial.println(GPS.Longitude());
  delay(100);
}
// Минимальный пример работы gps модуля платы RaceBoaard КБ INNOPOL`