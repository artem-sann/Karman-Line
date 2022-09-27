// минимальный пример работы мотора на плате RaceBoard
// мотор использовался для активации системы выброса полезной нангрузки
// выкручивается ось из гайки и освобождается пружина
#define DCMotorPin1 6   //Пин 1-го входа мотора V0.1R && V0.2
#define DCMotorPin2 7   //Пин 2-го входа мотора
#define DCWakeUPPin 41  //Пин запуска драйвера двигателя

void DCMotorSetSpeed(int y) {
  int x = constrain(y, -100, 100);
  x >= 0 ? digitalWrite(DCMotorPin2, HIGH) : digitalWrite(DCMotorPin1, HIGH);
  x >= 0 ? analogWrite(DCMotorPin1, 255 - (uint8_t)(x * 2.55)) : analogWrite(DCMotorPin2, 255 - (uint8_t)(-x * 2.55));
  //Управление вращением мотора , скорость в % от -100 до 100
}

void DCMotorInit() {
  pinMode(DCMotorPin1, OUTPUT);
  pinMode(DCMotorPin2, OUTPUT);
  pinMode(DCWakeUPPin, OUTPUT);
  digitalWrite(DCWakeUPPin, HIGH);
  //Инициализациия мотора
}

void setup() {
  DCMotorInit();
}

void loop() {
  DCMotorSetSpeed(-100);
}