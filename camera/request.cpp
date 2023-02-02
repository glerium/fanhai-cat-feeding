#include "camera.h"

void request_writeln(const String& str);   // 向服务器发送行（调试模式下同时向串口发送）
void request_writeln();                    // 重载函数，向服务器发送空行
int send_request(const char *host_name, const String& line, const String &header, const String &body, String &req);   // 发送POST请求
void get_access_token(String& token);       // 向百度API请求access_token
void get_api_result(const String& token, const String& img_base64, String& req);    // 向API请求识别结果

void request_writeln(const String& str){
  client.println(str);
  #ifdef DEBUG
  Serial.println(str);
  #endif
}
void request_writeln(){
  client.println();
  #ifdef DEBUG
  Serial.println();
  #endif
}

/* 向服务器发送请求 */
// 参数：服务器名、请求消息行、请求头、消息正文，req用来储存响应报文
int send_request(const char *host_name, const String& line, const String &header, const String &body, String &req){
  int ret;
  if((ret = client.connect(host_name, 80))){   //尝试连接服务器
    Serial.println("Connected to server.");
    request_writeln(header);
    request_writeln();
    request_writeln(body);
    req = "";
    while(client.connected()){   //当服务器请求未断开时
      if(client.available()){
        String str = client.readString();
        req += str;
      }
    }
    while(client.available()){   //防止请求已经断开但还有消息没有读取
      String str = client.readString();
      req += str;
    }
  }
  return ret;
}

//请求识别结果，参考文档https://ai.baidu.com/ai-doc/EASYDL/Wksjvmgbc
void get_api_result(const String& token, const String& img_base64, String& req){    
  const char *host_name = "aip.baidubce.com";
  const String line = "POST /rest/2.0/image-classify/v1/animal&access_token=" + token + " HTTP/1.1";
  String header = 
    "Content-Type: application/x-www-form-urlencoded\n \
     Host: aip.baidubce.com\n \
     Connection: close";
  String body = "image=" + img_base64;
  if(send_request(host_name, line, header, body, req))
    Serial.println("Successfully requested result!");
  else{
    Serial.println("Error occurred while requesting recognition result!");
    while(1);   //结束程序
  }
}

/* 向百度请求token */
// token存储返回的token值
// 返回值int为成功与否，非零值表示成功
void get_access_token(String& token){    
  const char host_name[] = "aip.baidubce.com";
  const String client_id = "otMs5VYi08Zwi1uCzM9WXNMG";              //API Key
  const String client_secret = "Q59X67CqjKLLSfVxMFuw5ggSEMmUungQ";  //Secret Key
  const String path = "POST /oauth/2.0/token?grant_type=client_credentials&client_id=" + client_id + "&client_secret=" + client_secret;
  
  const String line = path + " HTTP/1.1";                           //请求消息行
  String header = 
    "Host: aip.baidubce.com\n  \
     Connection: close";

  Serial.println("Requesting access token...");
  if(send_request(host_name, line, header, "", token))
    Serial.println("Token: " + token);
  else{
    Serial.println("Error occurred while requesting token!");
    while(1);     //结束程序
  }
}