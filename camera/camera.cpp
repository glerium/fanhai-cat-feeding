#include "camera.h"

WiFiClient wifi;
HttpClient client = HttpClient(wifi, ipAddress, port);
hw_timer_t * timer = NULL;       // 拍照计时器
hw_timer_t * timer_msg = NULL;   // 消息发送计时器

void blink(int time = 100) {
  digitalWrite(33, 0);
  delay(time);
  digitalWrite(33, 1);
}

/* 拍照计时回调，每三秒调用一次 */
// 功能：拍照并获取结果，成功则向单片机发送执行指令
void IRAM_ATTR onTimer() {
  Serial.println("timer activated");
  camera_fb_t * fb = capture();
  blink();        // 流程开始时闪烁提示
  if(!fb) {
    process_error();
    return;
  }
  Serial.println("img captured");
  bool iscat = get_api_result(fb);
  Serial.println("requested API");
  esp_camera_fb_return(fb);      // 释放帧内存
  Serial.println("img memory released");
  blink();        // 流程结束时闪烁提示
}

/* 拍摄照片，返回一个camera_fb_t指针 */
camera_fb_t * capture() {
  camera_fb_t * fb = esp_camera_fb_get();   // 调用摄像头
  if (!fb) {      // 拍摄不成功则fb的值为nullptr
      Serial.println("Image capture failed");
      return NULL;
  }
  if(fb->format == PIXFORMAT_JPEG) {
    Serial.println("JPEG img captured.");
    return fb;
  }
  else {
    Serial.println("Error: Not JPEG!");
    return NULL;
  }
}

/* 向服务器发送API识别请求、处理服务器的回复 */
// 参数：来自capture()函数的camera_fb_t指针
// 返回值：一个bool，表示是否识别成功
bool get_api_result(camera_fb_t * fb) {
  const char path[] = "/";
  client.beginRequest();
  Serial.println("begin request");
  byte* img = (byte*)(fb->buf);
  int len = fb->len * sizeof(uint8_t) / sizeof(byte);
  client.post(path, "image/jpeg", len, img);
  Serial.print("posted ");
  int status = client.responseStatusCode();
  Serial.println(status);
  if(status != 200) {
    Serial.print("HTTP Error: ");
    Serial.println(status);
    return false;
  }
  String response = client.responseBody();
  Serial.println("Response: " + response);
  return true;
}

/* 错误处理 */
void process_error() {
  Serial.println("An error has occurred.");
  blink(3000);    // 出现故障时摄像头LED长亮指示
}