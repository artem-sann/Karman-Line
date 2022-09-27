#include <GPS.h>
#include <BMP280.h>
#include <MPU9250.h>
#include <TenDOF.h>
//#include <Telemetry.h>

Barometer_m Bar;

IMU_m IMU;

double accel[4];
double gyro[3];
double mag[3];
double Temp;
double Height;
double Height_K;
double VertSpeed;
double Accel_1;
double Accel_2;
double Accel_3;
double Gyro_1;
double Gyro_2;
double Gyro_3;
double Mag_1;
double Mag_2;
double Mag_3;


double msg_d[10];
int32_t msg_i[10];



double CalibAccel[4][3]={
          {0,0,0},  //ofsett
          {1,0,0},   //first line
          {0,1,0},   //second line
          {0,0,1}};  //third line
double CalibGyro[3]={0,0,0};  //ofsett
double CalibMag[4][3]={
          {0,0,0},  //ofsett
          {1,0,0},   //first line
          {0,1,0},   //second line
          {0,0,1}};  //third line
 


void setup() {

  Serial.begin(9600);
  Bar.begin();
  Bar.SetZeroHeight();
  IMU.begin();
  
  pinMode(53, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(9, HIGH);


}


void print_Telemetry(double *Temp, double *Height, double *Height_K, double *VertSpeed, double *Accel_1, double *Accel_2, double *Accel_3, double *Gyro_1, double *Gyro_2, double *Gyro_3, double *Mag_1, double *Mag_2, double *Mag_3) {

  Serial.println("\nTemp \tHeight \tHeight_K \tVertSpeed \tAccel_1 \tAccel_2 \tAccel_3 \tGyro_1 \tGyro_2 \tGyro_3 \tMag_1 \tMag_2 \tMag_3");
  Serial.println("----------------------------------------------------------------------------------------------------------------------------------------------");
  Serial.print(*Temp);
  Serial.print("\t"); 
  Serial.print(*Height);
  Serial.print("\t");
  Serial.print(*Height_K);
  Serial.print("\t\t");
  Serial.print(*VertSpeed);
  Serial.print("\t\t");
  Serial.print(*Accel_1);
  Serial.print("\t\t");
  Serial.print(*Accel_2);
  Serial.print("\t\t");
  Serial.print(*Accel_3);
  Serial.print("\t\t");
  Serial.print(*Gyro_1);
  Serial.print("\t");
  Serial.print(*Gyro_2);
  Serial.print("\t");
  Serial.print(*Gyro_3);
  Serial.print("\t");
   Serial.print(*Mag_1);
  Serial.print("\t");
  Serial.print(*Mag_2);
  Serial.print("\t");
  Serial.print(*Mag_3);
  Serial.print("\t\t");
  

} 
 
void loop() {

  
  
  
  delay(100);

  IMU.SetCalibAccel(CalibAccel[0],CalibAccel[1],CalibAccel[2],CalibAccel[3]);
  IMU.SetCalibGyro(CalibGyro);
  IMU.SetCalibMag(CalibMag[0],CalibMag[1],CalibMag[2],CalibMag[3]);

  IMU.GetAccel(accel);
  IMU.GetGyro(gyro);
  IMU.GetMag(mag);
  Bar.Upd();

  Temp = Bar.GetTemp();
  Height = Bar.GetHeight();
  Height_K = Bar.GetHeightK();

  // = Serial.print(Bar.GetTemp()); Serial.print(" : ");
  //Height = Serial.print(Bar.GetHeight()); Serial.print(" : ");
  //Height_K = Serial.print(Bar.GetHeightK());Serial.print(" : ");
  //VertSpeed = Serial.println(Bar.GetVertSpeed()); 
  VertSpeed = Bar.GetVertSpeed();
  //Accel_1 = Serial.print(accel[0]); Serial.print(":");
  Accel_1 = accel[0];
  //Accel_2 = Serial.print(accel[1]); Serial.print(":");
  Accel_2 = accel[1];
  //Accel_3 = Serial.print(accel[2]); Serial.print(":");
  Accel_3 = accel[2];

  //Gyro_1 = Serial.print(gyro[0]); Serial.print(":");
  Gyro_1 = gyro[0];
  //Gyro_2 = Serial.print(gyro[1]); Serial.print(":");
  Gyro_2 = gyro[1];
  //Gyro_3 = Serial.print(gyro[2]); Serial.print(":");
  Gyro_3 = gyro[2];

  //Mag_1 = Serial.print(mag[0]); Serial.print(":");
  Mag_1 = mag[0];
  //Mag_2 =Serial.print(mag[1]); Serial.print(":");
  Mag_2 = mag[1];
  //Mag_3 =Serial.println(mag[2]);
  Mag_3 = mag[2];

  print_Telemetry(&Temp, &Height, &Height_K, &VertSpeed, &Accel_1, &Accel_2, &Accel_3, &Gyro_1, &Gyro_2, &Gyro_3, &Mag_1, &Mag_2, &Mag_3);

  delay(1000);

  /*Serial.print(Temp);

  Serial.print("\t"); 

  Serial.print(Height);

  Serial.print("\t\t"); 

  Serial.print(Height_K);

  Serial.print("\t\t"); 

  Serial.print(VertSpeed);

  Serial.print("\t\t"); 

  Serial.print(Accel_1);

  Serial.print("\t\t");

  Serial.print(Accel_2);

  Serial.print("\t\t");

  Serial.print(Accel_3);

  Serial.print("\t\t");

  Serial.print(Gyro_1);

  Serial.print("\t\t");

  Serial.print(Gyro_2);

  Serial.print("\t\t");

  Serial.print(Gyro_3);

  Serial.print("\t\t");

  Serial.print(Mag_1);

  Serial.print("\t\t");

  Serial.print(Mag_2);

  Serial.print("\t\t");

  Serial.print(Mag_3);

  Serial.print("\t\t");

  Serial.println();*/

  delay(1000);

    
}
