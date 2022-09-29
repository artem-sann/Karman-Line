#ifndef Macros_h
#define Macros_h


#include <Arduino.h>
#include <Servo.h>

//Camera
#define CameraRecord A12
#define CameraPower A14
#define CameraLed A13
//#define CameraBtn4 13

bool CamEn()
{
  pinMode(CameraRecord,OUTPUT);           pinMode(CameraPower,OUTPUT);
  pinMode(CameraLed,INPUT);             //pinMode(CameraBtn4,OUTPUT);

Serial.println("start reset");
  digitalWrite(CameraRecord,LOW); digitalWrite(CameraPower,HIGH); //OFF
  Serial.println("reset");
  delay(11000);
  digitalWrite(CameraPower,LOW);
  delay(1000);

  return 1;//digitalRead(CameraLed);
}

void CamOn()
{

Serial.println("on");
  digitalWrite(CameraPower,HIGH); //ON
  delay(3000);
  digitalWrite(CameraPower,LOW);
  delay(1000);

  digitalWrite(CameraPower,HIGH); //REC
  delay(700);
  digitalWrite(CameraPower,LOW);
  delay(300);
}

void CamOff()
{
Serial.println("start OFf");
  digitalWrite(CameraPower,HIGH); //OFF
  Serial.println("off");
  delay(11000);
  digitalWrite(CameraPower,LOW);
  delay(1000);

}




//Buzzer
#define Buzzer_Pin 4        //Пин к которому подключен бузер
#define Buzzer_Frequency 1000 //Частота бузера в Гц

#define BuzzerOn tone(Buzzer_Pin, Buzzer_Frequency)     //Активировать бузер
#define BuzzerOnTime(x)  tone(Buzzer_Pin, Buzzer_Frequency, (x) )  //Активировать бузер на некоторое время в мс
#define BuzzerOff noTone(Buzzer_Pin) //Деактивировать бузер

//Servos
Servo ServoOne; Servo ServoTwo;Servo ServoThree;Servo ServoFour; 
#define ServoOneEn ServoOne.attach(11) // Пин 1-го сервопривода на плате питания (11!)
#define ServoTwoEn ServoTwo.attach(12) // Пин 2-го сервопривода на плате питания
#define ServoThreeEn ServoThree.attach(46) // Пин 1-го сервопривода на плате инерциального датчика
#define ServoFourEn ServoFour.attach(45) // Пин 2-го сервопривода на плате инерциального датчика

#define ServoOneSet(x)  ServoOne.write(x)  //Установить углы на сервоприводах, от 0 до 180
#define ServoTwoSet(x)  ServoTwo.write(x) 
#define ServoThreeSet(x)  ServoThree.write(x) 
#define ServoFourSet(x)  ServoFour.write(x) 


//DC Motor
//#define DCMotorPin1 3 //Пин 1-го входа мотора   V0.1L
#define DCMotorPin1 6 //Пин 1-го входа мотора V0.1R && V0.2
#define DCMotorPin2 7 //Пин 2-го входа мотора

#define DCMotor pinMode(14,INPUT_PULLUP) //Необязательно, настраивает пин слежения за перегрузкой по току
void  DCMotorZState()
{
  digitalWrite(DCMotorPin2,LOW);
  digitalWrite(DCMotorPin1,LOW); //Отпускаем мотор в свободное вращение (Z-state)
}
void  DCMotorStop() 
{
  digitalWrite(DCMotorPin2,HIGH);
  digitalWrite(DCMotorPin1,HIGH); //Торможение мотором (блокировка ротора)
}
void  DCMotorSetSpeed(int y)
{ 
  
  int x=constrain(y,-100,100);
  x>=0?digitalWrite(DCMotorPin2,HIGH):digitalWrite(DCMotorPin1,HIGH);
  x>=0?analogWrite(DCMotorPin1,255-(uint8_t)(x*2.55)):
       analogWrite(DCMotorPin2,255-(uint8_t)(-x*2.55));   //Управление вращением мотора , скорость в % от -100 до 100


}


#endif
