#include <DFRobot_LedDriver.h>

//自定义通信引脚
/*FireBeetle-M0*/
#if defined ARDUINO_SAM_ZERO
#define D  7
#define C  5
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define D  D3
#define C  D4
/*AVR系列主板*/
#else
#define D  2
#define C  3
#endif

DFRobot_LedDriver rgbdriver(C, D); 

void setup() {
  Serial.begin(115200);
}

void loop() {
  /**
   * @brief 设置所有灯的RGB颜色
   * @param R 设置RGB红色分量，硬件应连接引脚B
   * @param G 设置RGB绿色分量，硬件应连接引脚C
   * @param B 设置RGB蓝色分量，硬件应连接引脚A
   */
  rgbdriver.setAllLed(0x01,0x01,0x01);
  delay(100);
  while(1);
}
