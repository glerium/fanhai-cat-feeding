#ifndef CAMERA_H
#define CAMERA_H
#include "esp_camera.h"
#include <WiFi.h>
#include <base64.h>
#include <ArduinoJson.h>
#include <esp_http_client.h>
// Select camera model
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"
#include "request.h"
#define DEBUG

extern WiFiClient client;
extern hw_timer_t* timer;   // 拍照计时器

extern const int buzz, motor, steer;          // 蜂鸣器、电机、舵机绑定的ledc_channel
extern const int S_LEFT, S_FORWARD, S_RIGHT;  // 舵机方向参数

extern void init();
extern camera_fb_t * capture();   // 拍照

extern void IRAM_ATTR onTimer();   // 拍照计时回调
#endif