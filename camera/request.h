#ifndef REQUEST_H
#define REQUEST_H
#include "camera.h"
extern int send_request(const char *host_name, const String& line, const String &header, const String &body, String &req);   // 发送POST请求
extern bool get_api_result(camera_fb_t * fb);    // 请求识别结果
#endif