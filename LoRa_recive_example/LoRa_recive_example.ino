//Минимальный пример приема данных через передатчик LoRa установленный на плате RaceBoard
#include <TelemetryLLCC68.h>
#include <TinyGPS++.h>

TelemetryLLCC68_m Telemetry;
String msg = "None";

double msg_d[10]; // это не нукжно
int32_t msg_i[10];

void setup() {
  Serial.begin(9600);
  Telemetry.on();
  Telemetry.SetMaxPower();
  Telemetry.SetChannel(Channel_6); // выбираем канал
}

void loop() {

  if (Telemetry.AvailablePacket()) {
    msg = Telemetry.ReciveS();
  }
  else {
    msg = "none";
  }
  Serial.println(msg); // вывод результата в порт

  delay(50);
}
