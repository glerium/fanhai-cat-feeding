#include "camera.h"

WiFiClient wifi;
HttpClient client = HttpClient(wifi, ipAddress, port);
hw_timer_t * timer = NULL;       // 拍照计时器
hw_timer_t * timer_msg = NULL;   // 消息发送计时器

void blink(int time=50) {
  digitalWrite(33, 0);
  delay(time);
  digitalWrite(33, 1);
}

/* 拍照计时回调，每三秒调用一次 */
// 功能：拍照并获取结果，成功则向单片机发送执行指令
void IRAM_ATTR onTimer() {
  #ifdef DEBUG
  Serial.println("timer 0");
  // return;
  // Serial.flush();
  #endif
  Serial.println("ready to capture");
  camera_fb_t * fb = capture();
  Serial.println("capture done");
  blink();
  if(!fb) {
    process_error();
    blink(3000);
    return;
  }
  Serial.println("no error");
  bool iscat = get_api_result(fb);
  Serial.println("api got");
  blink();
  if(iscat) {
    do_feed();
  }
  blink();
  Serial.println("feed done");
  esp_camera_fb_return(fb);
  Serial.println("img memory released");
}

/* 拍摄照片，返回一个camera_fb_t指针 */
camera_fb_t * capture() {
  Serial.println("in capture()");
  camera_fb_t * fb = esp_camera_fb_get();   // 调用摄像头
  Serial.println("captured in capture()");
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
  // char image[fb->len * 2 + 10];
  // Serial.println("memcpy ready");
  // memcpy(image, fb->buf, fb->len);    // 从内存中截取图片帧
  // Serial.println("memcpy done");
  const char path[] = "/?threshold=0.3";
  client.beginRequest();
#define debug(x) Serial.println(x)
  debug("begin request");
  byte* img = (byte*)(fb->buf);
  int len = fb->len * sizeof(uint8_t) / sizeof(byte);
  int resp = client.post(path, "image/jpeg", len, img);
  debug("posted ");
  int status = client.responseStatusCode();
  Serial.println(status);
  if(status != 200) {
    Serial.print("HTTP Error: ");
    Serial.print(status);
    Serial.print("\n");
    return false;
  }
  // client.skipResponseHeaders();
  String response = client.responseBody();
  Serial.println("Response:\n" + response);
  return true;
}

/* 错误处理 */
void process_error() {
  Serial.println("An error occurred.");
}

/* 向单片机发送放粮指令 */
void do_feed() {
  digitalWrite(33, 0);
  for(int i = 0; i < 30; i++) {
    Serial1.write('1');
    delay(10);
  }
  digitalWrite(33, 1);
  // int sum = 0, i = 1;
  // while(i <= 100) {
  //   if(Serial2.available()) {
  //     char tm = Serial2.read();
  //     if(tm == '1') sum++;
  //     if(sum >= 20) break;
  //     i++;
  //   }
  // }
}