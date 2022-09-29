#include "TelemetryLLCC68.h"
#include "Ra01S.h"
#include <SPI.h>


//https://github.com/chandrawi/LoRaRF-Arduino
#define wait_time 500


uint32_t packet = 0;

#define imp_p 37
SX126x  lora(9,               //Port-Pin Output: SPI select
      10,               //Port-Pin Output: Reset 
      2                //Port-Pin Input:  Busy
      );


uint8_t TelemetryLLCC68_m::on()
{
  packet = 0;

  pinMode(imp_p,OUTPUT); //37
  digitalWrite(imp_p,HIGH); 
  
  pinMode(53,OUTPUT);
  digitalWrite(53,HIGH); 

  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);

  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);

  init(433000000);

  if(initialT!=0)return false;
  else return true;
}

void TelemetryLLCC68_m::init(long freq)
{
  initialT = lora.begin(freq, powerTx);
  lora.LoRaConfig(8, 5, 4, 6,0,true,false);     
}


void TelemetryLLCC68_m::off()
{
  SetLowPower();
  initialT=0;
}

void TelemetryLLCC68_m::SetLowPower()
{
  lora.SetTxPower(3);
  powerTx=3;
}

void TelemetryLLCC68_m::SetMaxPower()
{
  lora.SetTxPower(20);
  powerTx=20;
}

void TelemetryLLCC68_m::SetChannel(Channels ch_)
{
  long freq_=433E6;
  int sw_=0x12;

  switch (ch_)
  {
  case Channel_1:{freq_=435E6;sw_=0x13;}
    break;
  case Channel_2:{freq_=436E6;sw_=0x14;}
    break;
  case Channel_3:{freq_=437E6;sw_=0x15;}
    break;
  case Channel_4:{freq_=438E6;sw_=0x16;}
    break;
  case Channel_5:{freq_=439E6;sw_=0x17;}
    break;
  case Channel_6:{freq_=440E6;sw_=0x18;}
    break;
  default:
    break;
  }

  init(freq_);
}




/////////////////////////////////////////////////////////////Send
void TelemetryLLCC68_m::SendS(String msg_)
{
  static uint8_t charBuf[128];
  msg_.toCharArray((char*)charBuf, msg_.length()+1);
  lora.Send(charBuf, msg_.length(), SX126x_TXMODE_SYNC);
}



void TelemetryLLCC68_m::SendTelemetryPack(bool start_, bool par_,bool par_speed, double height_, double speed_,
                                                         double Lon_,    double Lat_)
{
  packet++;
  uint8_t Len_=23;
  uint8_t msg_[Len_];

  uint8_t flag_=0;
  if(start_)flag_=1;
  if(par_)flag_+=2;
  if(par_speed)flag_+=4;

  int32_t Lon= (int32_t)(Lon_*1000000.0);
  int32_t Lat= (int32_t)(Lat_*1000000.0);

  msg_[0]='*';
  memcpy(msg_+1,&packet,4);
  memcpy(msg_+5,&height_,4);
  memcpy(msg_+9,&speed_,4);
  memcpy(msg_+13,&Lon,4);
  memcpy(msg_+17,&Lat,4);
  memcpy(msg_+21,&flag_,1);
  msg_[22]='#';

  lora.Send(msg_, Len_, SX126x_TXMODE_SYNC);
}

//////////////////////////////////////////////////////////////////Recive
bool TelemetryLLCC68_m::AvailablePacket()
{
  return lora.Available();
}

String TelemetryLLCC68_m::ReciveS()
{
  uint8_t rxData[255];
  uint8_t rxLen = lora.Receive(rxData, 255);
  String msg="";

  for (uint8_t i = 0; i < rxLen; i++)
    msg+=(char)rxData[i];
  return msg;
}

String TelemetryLLCC68_m::ParceTelemetryPack()
{
  uint8_t len_=23;
  String msg="";
  uint8_t msg_[len_];

  uint32_t Buff=0;
  double Buff_d=0.0;

  uint8_t rxData[255];
  uint8_t rxLen = lora.Receive(rxData, 255);

  if( rxLen==len_)
  {
    for (size_t i = 0; i < len_; i++)
    {
      msg_[i]= rxData[i];
    }

    packet++;

    if(msg_[0]=='*' && msg_[22]=='#')
    {
      memcpy(&Buff,&msg_[1],4); //packet
      msg+="R:"+String(Buff);

      msg+="/"+String(packet); //My packet 

      memcpy(&Buff_d,&msg_[5],4); //height
      msg+=":"+String(Buff_d);

      memcpy(&Buff_d,&msg_[9],4); //speed
      msg+=":"+String(Buff_d);

      memcpy(&Buff,&msg_[13],4); //Lon
      Buff_d=Buff/1000000.0;
      msg+=":"+String(Buff_d,6);
      
      memcpy(&Buff,&msg_[17],4); //Lat
      Buff_d=Buff/1000000.0;
      msg+=":"+String(Buff_d,6);

      Buff=msg_[21]; //flag
      msg+=":"+String(Buff&0b00000001);
      msg+=":"+String((Buff&0b00000010)>>1);
      msg+=":"+String((Buff&0b00000100)>>2);

      msg+="\n";

      return msg;
    }
    return "";
  }
  else 
  {
      String msg__="";
      for (uint8_t i = 0; i < rxLen; i++)
        msg__+=(char)rxData[i];
      return msg__;
  }
  
}