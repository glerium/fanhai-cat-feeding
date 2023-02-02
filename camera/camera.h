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
#include "request.h"
#define DEBUG

extern WiFiClient wifi;
extern HttpClient client;
extern hw_timer_t* timer;   // 拍照计时器

extern const char ipAddress[];
extern const int port;

extern const char* ssid;
extern const char* password;
// extern const int buzz, motor, steer;          // 蜂鸣器、电机、舵机绑定的ledc_channel
// extern const int S_LEFT, S_FORWARD, S_RIGHT;  // 舵机方向参数

void init();
camera_fb_t * capture();   // 拍照

void IRAM_ATTR onTimer();   // 拍照计时回调
#endif