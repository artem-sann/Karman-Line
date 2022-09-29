
#include <TenDOF.h>


//BMP Zone
Adafruit_BMP280 bmp;


void BMP_upd_timer();
TenDOF_d BMP_d;


bool Barometer_m::begin()
{
    bool on_;
    for (size_t i = 0; i < 3; i++)
    {   on_=bmp.begin();
         if (on_)break;
        delay(100);
    }

    BMP_d.varVolt = BarVolt ;  
    BMP_d.varProcess = BarProcess; 
    BMP_d.Ext_H =0;
    BMP_d.Pc = 0.0;
    BMP_d.G = 0.0;
    BMP_d.P = 1.0;
    BMP_d.Xp = 0.0;
    BMP_d.Zp = 0.0;
    BMP_d.Xe = 0.0;
    BMP_d.Height=0;
    BMP_d.Upd = false;

    for(int i=0;i<10;i++)BMP_d.HeigtHist[i]=0;
    BMP_d.VertSpeed=0;

    return on_;
}


double Barometer_m::GetTemp()
{
    return (double)bmp.readTemperature();
}

double Barometer_m::GetHeight()
{
    return (double)(bmp.readAltitude(1013.25)-ZeroHeight);
}
double Barometer_m::GetHeightK()
{
    return (double)(BMP_d.HeigtHist[9]);
}

void Barometer_m::SetZeroHeight()
{
    ZeroHeight=bmp.readAltitude(1013.25);
}

double Barometer_m::GetVertSpeed()
{
    return BMP_d.VertSpeed;
}


bool Barometer_m::Upd()
{
    bool ret = BMP_d.Upd;
    if(!BMP_d.Upd)
    {
        BMP_d.Upd=true;

        BMP_d.Ext_H = GetHeight();
        //Время фильтровать
        BMP_d.Pc = BMP_d.P + BMP_d.varProcess;
        BMP_d.G = BMP_d.Pc/(BMP_d.Pc + BMP_d.varVolt);
        BMP_d.P = (1-BMP_d.G)*BMP_d.Pc;
        BMP_d.Xp = BMP_d.Xe;
        BMP_d.Zp = BMP_d.Xp;
        BMP_d.Xe = BMP_d.G*(BMP_d.Ext_H-BMP_d.Zp)+BMP_d.Xp; // "фильтрованное" значение

        for(int i=0;i<9;i++){BMP_d.HeigtHist[i]=BMP_d.HeigtHist[i+1];}
        BMP_d.HeigtHist[9]=BMP_d.Xe;
        BMP_d.VertSpeed=(BMP_d.HeigtHist[9]-BMP_d.HeigtHist[0]);
    }
    return ret;
}


bool Barometer_m::Upd(double h_)
{
    bool ret = BMP_d.Upd;
    if(!BMP_d.Upd)
    {
        BMP_d.Upd=true;

        BMP_d.Ext_H = h_;
        //Время фильтровать
        BMP_d.Pc = BMP_d.P + BMP_d.varProcess;
        BMP_d.G = BMP_d.Pc/(BMP_d.Pc + BMP_d.varVolt);
        BMP_d.P = (1-BMP_d.G)*BMP_d.Pc;
        BMP_d.Xp = BMP_d.Xe;
        BMP_d.Zp = BMP_d.Xp;
        BMP_d.Xe = BMP_d.G*(BMP_d.Ext_H-BMP_d.Zp)+BMP_d.Xp; // "фильтрованное" значение

        for(int i=0;i<9;i++){BMP_d.HeigtHist[i]=BMP_d.HeigtHist[i+1];}
        BMP_d.HeigtHist[9]=BMP_d.Xe;
        BMP_d.VertSpeed=(BMP_d.HeigtHist[9]-BMP_d.HeigtHist[0]);
    }
    return ret;
}









void BMP_upd_timer()
{
    BMP_d.Upd=false; 
}


//MPU Zone

MPU9250 mpu_ob( MPU9250_ADDRESS_AD0_LOW );      //v0.2
//MPU9250 mpu_ob( MPU9250_ADDRESS_AD0_HIGH );

bool IMU_m::begin()
{

uint8_t sensorId=255;
for(int i=0;i<3;i++){
    mpu_ob.beginAccel(ACC_FULL_SCALE_16_G);
    mpu_ob.beginGyro(GYRO_FULL_SCALE_2000_DPS);
    mpu_ob.beginMag();
    delay(150);
}
    if (!mpu_ob.readId(&sensorId) == 0)return false;

    return true;
}


bool IMU_m::GetAccel(double * bf_)
{
    if(mpu_ob.accelUpdate()==0)
    {
        bf_[0]=mpu_ob.accelX();
        bf_[1]=mpu_ob.accelY();
        bf_[2]=mpu_ob.accelZ();

		double buf[3];
		buf[0]=-CalibAccel[0][0]+bf_[0];
		buf[1]=-CalibAccel[0][1]+bf_[1];
		buf[2]=-CalibAccel[0][2]+bf_[2];

		bf_[0]=buf[0]*CalibAccel[1][0] + buf[1]*CalibAccel[2][0] + buf[2]*CalibAccel[3][0]; // ГАВНО// ГАВНО// ГАВНО// ГАВНО// ГАВНО// ГАВНО
		bf_[1]=buf[0]*CalibAccel[1][1] + buf[1]*CalibAccel[2][1] + buf[2]*CalibAccel[3][1];// ГАВНО// ГАВНО// ГАВНО
		bf_[2]=buf[0]*CalibAccel[1][2] + buf[1]*CalibAccel[2][2] + buf[2]*CalibAccel[3][2];// ГАВНО// ГАВНО// ГАВНО// ГАВНО// ГАВНО// ГАВНО

        bf_[3]=sqrt(pow(bf_[0], 2) +
              pow(bf_[1], 2) +
              pow(bf_[2], 2));
    }
    else return false;
    return true;
}

bool IMU_m::GetGyro(double * bf_)
{
    if(mpu_ob.gyroUpdate()==0)
    {
        bf_[0]=mpu_ob.gyroX()-CalibGyro[0];
        bf_[1]=mpu_ob.gyroY()-CalibGyro[1];
        bf_[2]=mpu_ob.gyroZ()-CalibGyro[2];
    }
    else return false;
    return true;
}

bool IMU_m::GetMag(double * bf_)
{
    if(mpu_ob.magUpdate()==0)
    {
        bf_[0]=mpu_ob.magX();
        bf_[1]=mpu_ob.magY();
        bf_[2]=mpu_ob.magZ();

        double buf[3];
		buf[0]=-CalibMag[0][0]+bf_[0];
		buf[1]=-CalibMag[0][1]+bf_[1];
		buf[2]=-CalibMag[0][2]+bf_[2];

		bf_[0]=buf[0]*CalibMag[1][0] + buf[1]*CalibMag[2][0] + buf[2]*CalibMag[3][0];
		bf_[1]=buf[0]*CalibMag[1][1] + buf[1]*CalibMag[2][1] + buf[2]*CalibMag[3][1];
		bf_[2]=buf[0]*CalibMag[1][2] + buf[1]*CalibMag[2][2] + buf[2]*CalibMag[3][2];
    }
    else return false;
    return true;
}

void IMU_m::WhileCalib()
{
    while(1)
    {
        if(mpu_ob.accelUpdate()==0 &&  mpu_ob.gyroUpdate()==0 && mpu_ob.magUpdate()==0)
        {
        Serial.print(mpu_ob.accelX()); Serial.print(":");
        Serial.print(mpu_ob.accelY()); Serial.print(":");
        Serial.print(mpu_ob.accelZ()); Serial.print(":");

        Serial.print(mpu_ob.gyroX()); Serial.print(":");
        Serial.print(mpu_ob.gyroY()); Serial.print(":");
        Serial.print(mpu_ob.gyroZ()); Serial.print(":");

        Serial.print(mpu_ob.magX()); Serial.print(":");
        Serial.print(mpu_ob.magY()); Serial.print(":");
        Serial.println(mpu_ob.magZ());
        delay(50);
        }
    }
}

void IMU_m::SetCalibAccel(double * ac1,double * ac2,double * ac3,double * ac4)
{
    for (size_t i = 0; i < 3; i++) { CalibAccel[0][i]=ac1[i];}
    for (size_t i = 0; i < 3; i++) { CalibAccel[1][i]=ac2[i];}
    for (size_t i = 0; i < 3; i++) { CalibAccel[2][i]=ac3[i];}
    for (size_t i = 0; i < 3; i++) { CalibAccel[3][i]=ac4[i];}
}
void IMU_m::SetCalibGyro(double * clb_)
{
    for (size_t i = 0; i < 3; i++)
    {
        CalibGyro[i]=clb_[i];
    }
}
void IMU_m::SetCalibMag(double * ma1,double * ma2,double * ma3,double * ma4)
{
    for (size_t i = 0; i < 3; i++) { CalibMag[0][i]=ma1[i];}
    for (size_t i = 0; i < 3; i++) { CalibMag[1][i]=ma2[i];}
    for (size_t i = 0; i < 3; i++) { CalibMag[2][i]=ma3[i];}
    for (size_t i = 0; i < 3; i++) { CalibMag[3][i]=ma4[i];}
}

