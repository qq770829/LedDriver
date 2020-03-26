#include "DFRobot_LedDriver.h"

MY9221SS::MY9221SS(uint32_t pinClock, uint32_t pinData)
  :_pinData(pinData),
   _pinClock(pinClock)
{
  for (uint32_t i = 0; i < LED_BIN_COUNT; i++)
  {
    led[i] = LED_TURN_OFF;
  }
  pinMode(_pinClock, OUTPUT);
  pinMode(_pinData, OUTPUT);
}
void MY9221SS::sendcmd(uint16_t bits)//发送16位CMD命令
{
  for (uint8_t i = 0, clk = 0; i < 16; i++)
  {
    digitalWrite(_pinData, bits & 0x8000 ? HIGH : LOW);//数据发送
    digitalWrite(_pinClock, clk);//数据传输于时钟上升沿或下降沿
    clk = ~clk;
    bits <<= 1;
  }
}
void MY9221SS::senddata(uint16_t bits)//每次调用发送16位数据
{
  for (uint8_t i = 0, clk = 0; i < 16; i++)
  {
    digitalWrite(_pinData, bits & 0x8000 ? HIGH : LOW);//数据发送
    digitalWrite(_pinClock, clk);//数据传输于时钟上升沿或下降沿
    clk = ~clk;
    bits <<= 1;
  }
}
void MY9221SS::send()//共发送208位数据
{
  sendcmd(0x00); //发送命令，选择模式
  for (uint32_t i = 0; i < 12; i++)//发送灰阶数据，从A3引脚开始
  {
    senddata(led[i]);
  }
  latch();//锁存
  return;
}
void MY9221SS::latch()//内部栓锁的控制
{
  digitalWrite(_pinData, LOW);
  digitalWrite(_pinClock, HIGH);
  digitalWrite(_pinClock, LOW);
  digitalWrite(_pinClock, HIGH);
  digitalWrite(_pinClock, LOW);
  delayMicroseconds(240);
  digitalWrite(_pinData, HIGH);
  digitalWrite(_pinData, LOW);
  digitalWrite(_pinData, HIGH);
  digitalWrite(_pinData, LOW);
  digitalWrite(_pinData, HIGH);
  digitalWrite(_pinData, LOW);
  digitalWrite(_pinData, HIGH);
  digitalWrite(_pinData, LOW);
  delayMicroseconds(1);
  digitalWrite(_pinClock, HIGH);
  digitalWrite(_pinClock, LOW);
}









DFRobot_LedDriver::DFRobot_LedDriver(uint32_t pinClock, uint32_t pinData)
  :MY9221SS(pinClock, pinData)
{
}


void DFRobot_LedDriver::setLed(uint32_t ledNo, float R, float G, float B)
{
    ledNo = 5 - ledNo;
  if(ledNo <= 4 || ledNo >= 1)
  {
    for(uint8_t i = 0; i < LED_BIN_COUNT; i++)
    {
      if(i == ledNo*3-3)
      {
        led[i] = G ;
      }
      else if(i == ledNo*3-2)
      {
        led[i] = R ;
      }
      else if(i == ledNo*3-1)
      {
        led[i] = B ;
      }
      else
      {
        led[i] = LED_TURN_OFF;
      }
    }
    send();
  }
  else
  {
    DBG("ledNo error");
  }
}

void DFRobot_LedDriver::setAllLed(float R, float G, float B)
{
    for(uint8_t i = 0; i < LED_BIN_COUNT; i++)
    {
      if(i%3 == 0)
      {
        led[i] = G ;
      }
      else if(i%3 == 1)
      {
        led[i] = R ;
      }
      else if(i%3 == 2)
      {
        led[i] = B ;
      }
    }
    send();
}