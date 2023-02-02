#ifndef REQUEST_H
#define REQUEST_H
#include "camera.h"
extern void request_writeln(const String& str);   // 向服务器和串口同时发送行（串口用来调试）
extern void request_writeln();                    // 重载函数，向服务器和串口同时发送空行
extern int send_request(const char *host_name, const String& line, const String &header, const String &body, String &req);   // 发送POST请求
extern void get_access_token(String& token);       // 向百度API请求access_token
extern void get_api_result(const String& token, const String& img_base64, String& req);    // 向API请求识别结果
extern bool get_recogn_result();    // 请求识别结果
#endif