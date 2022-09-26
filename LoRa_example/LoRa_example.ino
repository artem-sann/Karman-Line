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
  for (uint8_t i = 0; i < 10; i++) { msg_d[i] = i * 2.25; }
  for (uint8_t i = 0; i < 10; i++) { msg_i[i] = i * 3; }
  //Telemetry.SetLowPower();
  Telemetry.SetMaxPower();

  Telemetry.SendS(msg);

  delay(100);
}
