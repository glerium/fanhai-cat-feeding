#ifndef CAMERA_H
#define CAMERA_H

#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <Ticker.h>
// #include <avr/wdt.h> // enabling atmega328 watchdog.
// Select camera model
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"
#define DEBUG

extern WiFiClient wifi;
extern HttpClient client;
extern Ticker ticker, ticker_msg;

const char ipAddress[] = "192.168.43.142";
const int port = 5000;
const char ssid[] = "glerium";
const char password[] = "Wenzelin2004";

void init();                    // 初始化
camera_fb_t * capture();        // 拍照
bool get_api_result(camera_fb_t *);    // 请求识别结果
void process_error();           // 异常处理
void do_feed();                 // 向单片机发送放粮指令
void IRAM_ATTR onTimer();       // 计时器，每10ms计数一次

void work_capture();            // 拍照上传识别
void work_msg();                // 和单片机通信

#endif