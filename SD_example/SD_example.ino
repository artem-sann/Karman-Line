#include <AutoUpdate.h>
#include <Timers.h>
#include <GPS.h>
#include <ADC.h>
#include <IntPin.h>
#include <SDCustom.h>
#include <TelemetryLLCC68.h>
#include <SD.h>


//----------------------Переменные для GPS------------------------------------------------->
bool GPS_Flag;  //Актуальность данных (true-данные новые и не считывались)
double Long;    //Долгота, в градусах
double Lati;    //Широта, в градусах
double Sp;      //Горизонтальная скорость, в метрах в секунду
double Alt;     //ВысотаБ в метрах в секунду
String Coord;   //Строка с координатами и высотой
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

bool SD_data_Write() {
  long time_to_SD = millis();

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


  




void setup() {
  
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
  

  
  

  delay(1000);
}