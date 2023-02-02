#include "camera.h"

void request_writeln(const String& str);   // 向服务器发送行（调试模式下同时向串口发送）
void request_writeln();                    // 重载函数，向服务器发送空行
int send_request(const char *host_name, const String& line, const String &header, const String &body, String &req);   // 发送POST请求
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
// int send_request(const char *host_name, const String& line, const String &header, const String &body, String &req){
//   int ret;
//   if((ret = client.connect(host_name, 80))){   //尝试连接服务器
//     Serial.println("Connected to server.");
//     request_writeln(header);
//     request_writeln();
//     request_writeln(body);
//     req = "";
//     while(client.connected()){   //当服务器请求未断开时
//       if(client.available()){
//         String str = client.readString();
//         req += str;
//       }
//     }
//     while(client.available()){   //防止请求已经断开但还有消息没有读取
//       String str = client.readString();
//       req += str;
//     }
//   }
//   return ret;
// }

//请求识别结果，参考文档https://ai.baidu.com/ai-doc/EASYDL/Wksjvmgbc
// void get_api_result(const String& token, const String& img, String& req){    
//   const char *host_name = "192.168.1.5";
//   const String line = "?threshold=0.5 HTTP/1.1";
//   String header = 
//     "Content-Type: application/x-www-form-urlencoded\n 
//      Host: aip.baidubce.com\n 
//      Connection: close";
//   String body = img;
//   if(send_request(host_name, line, header, body, req))
//     Serial.println("Successfully requested API!");
//   else{
//     Serial.println("Error occurred while requesting API!");
//     while(1);   //结束程序
//   }
// }
