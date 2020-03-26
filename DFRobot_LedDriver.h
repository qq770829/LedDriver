
#ifndef __DFROBOT_LEDDRIVER_H
#define __DFRobot_LEDDRIVER_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
//打开这个宏，可以看到程序的详细运行过程
#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif


#define LED_BIN_COUNT   12            //芯片的RGB引脚总数

// 00000000 darkest
// 00000011 brighter
// ........
// 11111111 brightest
#define LED_TURN_OFF           0     //关闭LED灯
#define LED_FULL_BRIGHTNESS    0xff  //打开LED灯（最亮）




class MY9221SS
{
public:
  /*
  *设置芯片模式的CMD数据格式
  * ------------------------------------------------------------------------------------------
  * |    b15   |   b14    |   b13    |   b12    |   b11    |    b10   |    b9    |    b8     |
  * ------------------------------------------------------------------------------------------
  * |                      保留位元                        | 电流快慢 |   灰度分辨率的选择   |
  * ------------------------------------------------------------------------------------------
  * ------------------------------------------------------------------------------------------
  * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
  * ------------------------------------------------------------------------------------------
  * |    内部振荡器频率选择          | 输出波形 |灰度时钟源| 输出极性 |计数器复位 | 单次模式 |
  * ------------------------------------------------------------------------------------------
  */
  typedef struct
  {
    uint8_t   temp: 5; /*!< 没有用，全部填0 */
    uint8_t   hspd: 1; /*!< 1：输出电流快速模式 0：输出电流慢速模式 */
    uint8_t   bs: 2; /*!< 0：8位灰度级应用，1：12位灰度级应用，2：14位灰度级应用，3：16位灰度级应用 */
    uint8_t   gck: 3; /*!< 0：原始频率(8.6MHz)(如果osc="1"则此处应设置为0)，1：原始/2，2：原始/4，3：原始/8，4：原始/16，5：原始/64，6：原始/128，7：原始/256 */
    uint8_t   sep:1; /*!< 1：APDM输出波形 0：MY-PWM输出波形(与传统波形相似) */
    uint8_t   osc:1; /*!< 1：外部时钟 0：内部振荡器(8.6MHz) */
    uint8_t   pol:1; /*!< 1：作为MY-PWM/APDM生成器工作 0：作为LED驱动工作 */
    uint8_t   cntset:1; /*!< 1：计数器复位模式(仅当osc="1"时可用) 0：自由运行模式 */
    uint8_t   onest:1; /*!< 1：单次模式(仅当cntset="1"时可用) 0：帧周期重复模式 */
  } __attribute__ ((packed)) sMode_t;

public:
  MY9221SS(uint32_t pinClock, uint32_t pinData);
  uint32_t countOfLed();
  void sendcmd(uint16_t bits);  //发送16位CMD命令
  void senddata(uint16_t bits); //每次调用发送16位数据
  void send();                  //调用发送全部208位数据
  void latch();                 //内部栓锁的控制

protected:
  uint32_t _pinClock;
  uint32_t _pinData;
  uint8_t  led[LED_BIN_COUNT];
};

class DFRobot_LedDriver :public MY9221SS
{
public:
  DFRobot_LedDriver(uint32_t pinClock, uint32_t pinData);
  void setLed(uint32_t ledNo, float R, float G, float B);
  void setAllLed(float R, float G, float B);
};
#endif
