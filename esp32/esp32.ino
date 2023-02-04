/*
    猫猫投喂机的单片机端代码
    本代码作者：温泽林@翻海小分队
    小组成员：刘泽瑄、温泽林、路广城、王童、吴浏宇
*/
#include <Ticker.h>
const int buzz=0, motor=1, steer=2;             // 蜂鸣器、电机、舵机绑定的ledc_channel
const int S_LEFT=7, S_FORWARD=21, S_RIGHT=33;   // 舵机方向参数
Ticker greens, reds;
const int red = 32, green = 33;

void init();
void do_feed();

void shut_green() {
  digitalWrite(green, 1);
}

void shut_red() {
  digitalWrite(red, 1);
}

void setup() {
  Serial.begin(115200);
  // init();
  Serial.write('1');
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  digitalWrite(red, 1);
  digitalWrite(green, 1);
}

bool val[32];
void loop() {
  if(Serial.available()) {
    char a = Serial.read();
    if(a == '0') {
      digitalWrite(red, 0);
      reds.attach_ms(5, shut_red);
    } else {
      digitalWrite(green, 0);
      greens.attach_ms(5, shut_green);
    }
  }
  // 读取UART数据
  // static int sum = 0, idx = 0;
  // if(sum > 25) {
  //   for(int i = 0; i < 30; i++)
  //     val[i] = 0;
  //   do_feed();
  //   sum = 0;
  // } else {
  //   sum -= val[idx];
  //   if(Serial.available()){
  //     val[idx] = Serial.read() == '1' ? 1 : 0;
  //     sum += val[idx];
  //     idx = (idx + 1) % 30;
  //   }
  // }
}

void init() {
  // 初始化UART模块
  Serial.begin(9600, SERIAL_8N1, 19, 21);
  // 将对应引脚设为输出模式
  pinMode(25, OUTPUT);    // 蜂鸣器
  pinMode(26, OUTPUT);    // 电机
  pinMode(15, OUTPUT);    // 舵机
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
  delay(1000);
  ledcWrite(motor, 0);
  // 舵机转动90度
  ledcWrite(steer, S_LEFT);
  delay(5000);
  ledcWrite(steer, S_FORWARD);
  // 蜂鸣器长报警，流程结束
  ledcWriteTone(buzz, 494);
  delay(2000);
  ledcWriteTone(buzz, 0);
  // 向摄像头返回数据
  for(int i = 1; i <= 30; i++) {
    Serial.write('1');
    delay(10);
  }
}