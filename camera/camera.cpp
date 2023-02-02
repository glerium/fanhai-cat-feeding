#include "camera.h"
WiFiClient client;
hw_timer_t* timer = NULL;   // 拍照计时器

camera_fb_t * capture();    // 拍照
void IRAM_ATTR onTimer();   // 拍照计时回调
void process_error();       // 异常处理
bool get_recogn_result(camera_fb_t *);
void do_feed();

/* 拍照计时回调，每三秒调用一次 */
// 功能：拍照并获取结果，成功则向单片机发送执行指令
void IRAM_ATTR onTimer() {
  camera_fb_t * fb = capture();
  if(!fb) {
    process_error();
  }
  bool iscat = get_recogn_result(fb);
  if(iscat) {
    timerAlarmDisable(timer);   // 暂停计时器
    do_feed();
    timerAlarmEnable(timer);    // 启动计时器
  }
}

camera_fb_t * capture() {
  camera_fb_t * fb = esp_camera_fb_get();   // 调用摄像机
  if (!fb) {    // 拍摄不成功则fb的值为nullptr
      Serial.println("Image capture failed");
      return NULL;
  }
  if(fb->format == PIXFORMAT_JPEG) {
    Serial.println("Image captured.");
    return fb;
  }
  else {
    Serial.println("Error: Not JPEG!");
    return NULL;
  }
}

bool get_recogn_result(camera_fb_t * fb) {
  uint8_t image[fb->len + 10];
  memcpy(image, fb->buf, fb->len);
  return true;
}

void process_error() {
  while(1);
}

void do_feed() {
  // 向单片机发送数据
}