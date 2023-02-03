// 初始化WiFi模块
#include "camera.h"

void init();
void wifi_init();     // 初始化Wifi模块

void init(){
  // Serial2.begin(9200, SERIAL_8N1, 19, 21);  //跨单片机串口
  wifi_init();
}