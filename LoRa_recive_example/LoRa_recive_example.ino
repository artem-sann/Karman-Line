//Минимальный пример отправки данных через передатчик LoRa установленный на плате RaceBoard
#include <TelemetryLLCC68.h>
#include <TinyGPS++.h>

TelemetryLLCC68_m Telemetry;
String msg;

double msg_d[10];
int32_t msg_i[10];

void setup() {
  Serial.begin(9600);
}

void loop() {
  Telemetry.on();
  Telemetry.SetMaxPower();

  msg = Telemetry.ReciveS();
  Serial.println(msg);

  delay(100);
}
