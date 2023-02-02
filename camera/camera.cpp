#include "camera.h"

WiFiClient wifi;
HttpClient client = HttpClient(wifi, ipAddress, port);
hw_timer_t * timer = NULL;       // 拍照计时器
hw_timer_t * timer_msg = NULL;   // 消息发送计时器
char recognized;

/* 拍照计时回调，每三秒调用一次 */
// 功能：拍照并获取结果，成功则向单片机发送执行指令
void IRAM_ATTR onTimer() {
  camera_fb_t * fb = capture();
  if(!fb) {
    process_error();
  }
  bool iscat = get_api_result(fb);
  if(iscat) {
    timerAlarmDisable(timer);   // 暂停计时器
    do_feed();
    timerAlarmEnable(timer);    // 启动计时器
  }
}

/* 指令发送计时回调 */
// 功能：每10ms触发一次，利用UART向单片机发送是否识别的消息
void IRAM_ATTR onTimerMsg() {
  Serial2.write(recognized ? '1' : '0');
}  

/* 拍摄照片，返回一个camera_fb_t指针 */
camera_fb_t * capture() {
  camera_fb_t * fb = esp_camera_fb_get();   // 调用摄像头
  if (!fb) {      // 拍摄不成功则fb的值为nullptr
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

/* 向服务器发送API识别请求、处理服务器的回复 */
// 参数：来自capture()函数的camera_fb_t指针
// 返回值：一个bool，表示是否识别成功
bool get_api_result(camera_fb_t * fb) {
  // 图片预处理
  char image[fb->len * 2 + 10];
  memcpy(image, fb->buf, fb->len);    // 从内存中截取图片帧

  const char path[] = "/?threshold=0.3";
  client.beginRequest();
  client.post(path, "image/jpeg", image);
  int status = client.responseStatusCode();
  // client.skipResponseHeaders();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.print(status);
  Serial.print("\nResponse:\n" + response);

  return true;
}

/* 错误处理 */
void process_error() {
  while(1);
}

/* 向单片机发送放粮指令 */
void do_feed() {
  recognized = true;
  delay(300);
  recognized = false;
  int sum = 0, i = 1;
  while(i <= 100) {
    if(Serial2.available()) {
      char tm = Serial2.read();
      if(tm == '1') sum++;
      if(sum >= 20) break;
      i++;
    }
  }
}