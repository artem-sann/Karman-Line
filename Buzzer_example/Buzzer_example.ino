//миинимальный пример работы с пищалкой на плате RaaceBoard
#define Buzzer_Pin 4           //Пин к которому подключен бузер
#define Buzzer_Frequency 2000  //Частота бузера в Гц

#define BuzzerOn tone(Buzzer_Pin, Buzzer_Frequency)              //Активировать бузер
#define BuzzerOnTime(x) tone(Buzzer_Pin, Buzzer_Frequency, (x))  //Активировать бузер на некоторое время в мс
#define BuzzerOff noTone(Buzzer_Pin)                             //Деактивировать бузер
void setup() {
  
}

void loop() {
  if (analogRead(A0) >) {
    BuzzerOnTime(200);
    delay(300);
  }
  
}