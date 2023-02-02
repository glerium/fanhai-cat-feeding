#include "camera.h"

WiFiClient wifi;
HttpClient client = HttpClient(wifi, ipAddress, port);
hw_timer_t* timer = NULL;   // 拍照计时器

const char ipAddress[] = "192.168.1.13";
const int port = 80;
const char* ssid = "glerium";
const char* password = "Wenzelin2004";

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
  char image[fb->len * 2 + 10];
  memcpy(image, fb->buf, fb->len);
  const char path[] = "/?threshold=0.3";
  client.beginRequest();
  client.post(path, "image/jpeg", image);
  
  int status = client.responseStatusCode();
  // client.skipResponseHeaders();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.print(status);
  Serial.print("\nResponse:\n" + response);
}

void process_error() {
  while(1);
}

void do_feed() {
  // 向单片机发送数据
}