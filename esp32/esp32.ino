/*
    猫猫投喂机的单片机端代码
    本代码作者：温泽林@翻海小分队
    小组成员：刘泽瑄、温泽林、路广城、王童、吴浏宇
*/
#include <Ticker.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
const int buzz=0, motor=1, steer=2;             // 蜂鸣器、电机、舵机绑定的ledc_channel
const int S_LEFT=7, S_FORWARD=21, S_RIGHT=33;   // 舵机方向参数
Ticker greens, reds;
const int red = 32, green = 33;
const char ipAddress[] = "192.168.43.142";
const int port = 5000;
const char ssid[] = "glerium";
const char password[] = "Wenzelin2004";
WiFiClient wifi;
HttpClient client = HttpClient(wifi, ipAddress, port);

void init();
void do_feed();

void do_green(int time = 500) {
  digitalWrite(green, 0);
  delay(time);
  digitalWrite(green, 1);
}

void do_red(int time = 500) {
  digitalWrite(red, 0);
  delay(time);
  digitalWrite(red, 1);
}

void blink(int time = 500) {
  digitalWrite(green, 0);
  digitalWrite(red, 0);
  delay(time);
  digitalWrite(green, 1);
  digitalWrite(red, 1);
}

void setup() {
  Serial.begin(115200);
  init();
  Serial.println(WiFi.macAddress());
  wifi_init();
  blink(700);
}

void loop() {
  client.get("/recognized");
  int status = client.responseStatusCode();
  Serial.println(status);
  String response = client.responseBody();
  if(response[0] == '1')
    do_feed();
}

void init() {
  // 将对应引脚设为输出模式
  pinMode(25, OUTPUT);    // 蜂鸣器
  pinMode(26, OUTPUT);    // 电机
  pinMode(15, OUTPUT);    // 舵机
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  // 关闭LED灯
  digitalWrite(red, 1);
  digitalWrite(green, 1);
  // 将引脚与对应ledc频道绑定
  ledcAttachPin(25, buzz);
  ledcAttachPin(26, motor);
  ledcAttachPin(15, steer);
  // 对电机和舵机的ledc进行初始化
  ledcSetup(motor, 50, 10);
  ledcSetup(steer, 50, 8);
  // 设定舵机状态
  ledcWrite(steer, S_FORWARD);   // 默认居中
}

void wifi_init(){
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

void do_feed() {
  // 流程开始，发出3次警报声
  for(int i = 3; i > 0; i--){
    ledcWriteTone(buzz, 494);
    delay(200);
    ledcWriteTone(buzz, 0);
    if(i) delay(30);
  }
  // 驱动电机旋转3圈
  ledcWrite(motor, 200);
  delay(1200);
  ledcWrite(motor, 0);
  // 舵机转动90度
  ledcWrite(steer, S_LEFT);
  delay(5000);
  ledcWrite(steer, S_FORWARD);
  // 蜂鸣器长报警，流程结束
  ledcWriteTone(buzz, 494);
  delay(2000);
  ledcWriteTone(buzz, 0);
}