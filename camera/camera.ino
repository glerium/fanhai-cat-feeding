/*
    猫猫投喂机的摄像头端代码，部分代码来自Arduino 2.0.1的示例代码ESP32CameraWebServer
    作者：温泽林@翻海小分队
    小组成员：刘泽瑄、温泽林、路广城、王童、吴浏宇
*/

#include "camera.h"

void setup() {
  init();       // 调用初始化函数
  camera_fb_t * fb = capture();
  if(!fb) {
    while(true);  // 错误处理
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}