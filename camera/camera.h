#ifndef CAMERA_H
#define CAMERA_H

#include "esp_camera.h"
#include <WiFi.h>
#include <base64.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
// Select camera model
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"
// #include "request.h"
#define DEBUG

extern WiFiClient wifi;
extern HttpClient client;
extern hw_timer_t * timer;       // 拍照计时器
extern hw_timer_t * timer_msg;   // 指令发送计时器

const char ipAddress[] = "192.168.1.13";
const int port = 80;
const char ssid[] = "glerium";
const char password[] = "Wenzelin2004";

void init();                    // 初始化
camera_fb_t * capture();        // 拍照
bool get_api_result(camera_fb_t *);    // 请求识别结果
void process_error();           // 异常处理
void do_feed();                 // 向单片机发送放粮指令
void IRAM_ATTR onTimer();       // 拍照计时回调
void IRAM_ATTR onTimerMsg();    // 指令发送计时回调

#endif