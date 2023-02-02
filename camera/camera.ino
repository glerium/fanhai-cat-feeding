/*
    猫猫投喂机的摄像头端代码，部分代码来自Arduino 2.0.1的示例代码ESP32CameraWebServer
    作者：温泽林@翻海小分队
    小组成员：刘泽瑄、温泽林、路广城、王童、吴浏宇
*/

#include "camera.h"

void setup() {
  init();       // 调用初始化函数
  camera_fb_t * fb = capture();
  if(!fb) {
    // todo：错误处理
  }
  /* 向服务器请求token */
  String token;
  // get_access_token(token);
  /* 将帧转换为base64并向服务器请求识别数据 */
  String img_base64 = base64::encode((const uint8_t *)fb->buf, fb->len);
  // Serial.println("Content:");
  // Serial.println(img_base64);
  String ret;
  get_api_result(img_base64, token, ret);

  /* 解析响应报文 */
    //获取http状态码
  size_t ret_len = ret.length();
  String http_status = "";
  for(size_t i=0; i<ret_len; i++){
    if(ret[i]=='\n')
      break;
    http_status += ret[i];
  }
    //截取json部分
  String ret_json = "";
  {
    size_t i=0;
    while(ret[i] != '{') i++;
    while(ret[i] != '}'){
      ret_json += ret[i];
      i++;
    }
    ret_json += ret[i];
  }
    //调用函数解析json
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, ret_json);
  Serial.println("log_id: " + uint64_t(doc["log_id"]));
  // JsonArray
}

// void parse_result(const String& ret, String& http_status, String& )

void loop() {
  // put your main code here, to run repeatedly:
  
}
