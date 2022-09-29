#include "SDCustom.h"

#define MISO_pin 50
#define MOSI_pin 51
#define CLK_pin 52
#define CS_pin 37 

#define LogFrecuency 2// 1Hz-30Hz


File LogFile;
uint8_t LogTick=30;
uint8_t tick=0;
bool FileOk=false;

bool SDCard_m::begin(String msg_)
{
  pinMode(53,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(53,HIGH);

  SPI.setClockDivider(SPI_CLOCK_DIV16);
  bool Init=SD.begin(CS_pin);
  if(!Init)return 0;


  String Name = "Log";
  String EndName = ".txt";
  for (size_t i = 0; i < 96; i++)
  {
    if(!SD.exists(Name+i+EndName))
    {
      LogFile = SD.open(Name+i+EndName, O_READ | O_WRITE | O_CREAT);break;
    }
    if(i==95)
    {
      LogFile = SD.open(Name+95+EndName, FILE_WRITE);
      break;
    }
  }
    LogFile.println(msg_);
    FileOk=true;

    return 1;
}

void SDCard_m::print(String msg_)
{
  LogFile.print(msg_);
}
void SDCard_m::println(String msg_)
{
  LogFile.println(msg_);
}

void SDCard_m::end()
{
    LogFile.close();
    FileOk=false;
    SD.end();
}

void SDCard_m::WriteIntDouble(int32_t *msg_,uint8_t len_i, double* msg__,uint8_t len_d)
{
  uint8_t a=sizeof(int32_t);
  uint8_t b=sizeof(double);

  uint8_t buf[a];
  uint8_t buff_d[b];

  for (size_t k = 0; k < len_i; k++)
  {
    memcpy(buf,&msg_[k],a);
    for (uint8_t i = 0; i < a; i++)
    {
      LogFile.write(buf[i]);
     
    }
  }
  for (size_t k = 0; k < len_d; k++)
  {
    memcpy(buff_d,&msg__[k],b);
    for (uint8_t i = 0; i < b; i++)
    {
      LogFile.write(buff_d[i]);
    }
  }
      //LogFile.println("");
}


void SDCard_m::upd()
{
 if(FileOk){LogFile.flush();}
}


void SD_upd_timer()
{
  //tick++;
  //if(tick>=LogTick)
  {
    //tick=0;
    if(FileOk){LogFile.flush();}
  }
}



