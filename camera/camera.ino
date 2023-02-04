/*
    猫猫投喂机的摄像头端代码，部分代码来自Arduino 2.0.1的示例代码ESP32CameraWebServer
    本代码作者：温泽林@翻海小分队
    小组成员：刘泽瑄、温泽林、路广城、王童、吴浏宇
*/

#include "camera.h"
void init_cam();      // 摄像头初始化
void init_timer();    // 计时器初始化
Ticker ticker, ticker_msg;

/* 指令发送计时回调 */
// 功能：每10ms触发一次，利用UART向单片机发送未识别消息
void IRAM_ATTR onTimerMsg() {
  #ifdef DEBUG_TIMER
  Serial.println("timer 1");
  #endif
  Serial1.write('0');
}

void setup() {
  Serial.begin(115200);             // 电脑串口
  Serial.setDebugOutput(true);      // 显示调试代码，测试结束后记得删掉
  Serial.println();
  // Serial1.begin(9600, SERIAL_8N1, 3, 1);  //跨单片机串口
  pinMode(33, OUTPUT);
  digitalWrite(33, 0);
  init_cam();     // 初始化摄像头
  wifi_init();
  init_timer();
  digitalWrite(33, 1);
}

void loop() {
  
}

/* 初始化计时器 */
void init_timer() {
  ticker.attach(10, onTimer);
  ticker_msg.attach_ms(10, onTimerMsg);
}

void wifi_init(){
  // IPAddress local_IP(192, 168, 1, 101);
  // IPAddress gateway(192, 168, 1, 1);
  // IPAddress subnet(255, 255, 255, 255);
  // IPAddress dns(8, 8, 8, 8);
  // Serial.printf("%s %s\n", ssid, password);
  // if (WiFi.config(local_IP, gateway, subnet, dns, dns) == false) {
  //   Serial.println("Configuration failed.");
  // }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/* 初始化摄像头 */
// 这段代码似乎只能放在主ino文件，放到外部文件会报错Camera init failed with error 0x105，原因不明
void init_cam() {
  /* 摄像头初始化开始 */
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);
#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
  // s->set_brightness(s, 2);
  // s->set_framesize(s, FRAMESIZE_XGA);
  // s->set_quality(s, 8);
  /* 摄像头初始化结束 */
}