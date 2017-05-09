#include "pwmClass.h"

unsigned  int encoder0PinA = 2;  //A输入
unsigned  int encoder0PinB = 8;  //B输入
unsigned  int encoder0Dir = 0; 
  
unsigned  int  PinPWMStep = 3;//PWM 输入
unsigned  int PinPWMDir = 7;//Dir 输入

unsigned  int PinPwmOutStep = 9; //pwm输出
unsigned  int PinPwmOutDir = 10; //dir输出

unsigned  int encodeStep = 400; //编码器一圈步数 0.9度
unsigned  int pwmStep = 400; //pwm 一圈步数 1.8度
unsigned  int pwmNs = 8; //pwm细分 数

double linkValue = 1.4375;

volatile int encoder0Pos = 0;

long targetPos = 0; //目标位置
long num = 0;
long num2 = 0;
//每8个脉冲=1个解码器信号
//当前pwm信号和解码器信号的比率
double pwmStepUnit = (((pwmStep * pwmNs) / encodeStep) / 2) * linkValue;

long lastnum = 0;

unsigned long lastms = 0; //长时间没动则清0
unsigned long time = 0;

unsigned int pwmnum = 1;//当前pwm通道

unsigned int nowdir = 0;//当前方向

bool infix = false;//修复进程

int InterruptA = 0;
int InterruptB = 0;


int InterruptDefines[] = { 0, 1, 2, 3 , 4, 5};
int InterruptMaps[] = { 2, 3, 21, 20 , 19, 18};



//中断0调用函数
void pwmClass::encode()
{
  //Serial.println("encode");
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
  {
    encoder0Pos--;
  }
  else
  {
    encoder0Pos++;
  }
  //time = micros();
}


//中断1函数，pwm输入
void pwmClass::pwmInput() {
  //Serial.print(pwmnum);
  //Serial.println("pwmInput");

  int dir = digitalRead(PinPWMDir);
  if (nowdir != dir) {
    digitalWrite(PinPwmOutDir, dir);
    nowdir = dir;
  }
  if (dir == 0) {
    targetPos++;
  }
  else {
    targetPos--;
  }

  //默认直接输出
  if (true) {
    //Serial.print(" PinPwmOutStep Out:");
    //Serial.print(PinPwmOutStep);
    digitalWrite(PinPwmOutStep, HIGH);
    digitalWrite(PinPwmOutStep, LOW);
    return;
  }
  int nowpos = targetPos / pwmStepUnit;

  int p = abs(nowpos - encoder0Pos);

  int limitp = 10;
  if (infix) {
    limitp = 0;
  }
  //当获得下一个信号，并且，解码器误差和当前误差过大
  //超过5
  if (p > limitp)
  {
    if (!infix) {
      infix = true;
      Serial.println("--------------tryfix----------");
      printPwmPos();
    }
    /*
      Serial.print("--tryfix---");
      Serial.print("dir:");
      Serial.print(dir);
      Serial.println("");
    */
    if (dir == 0) { //正走
      if (encoder0Pos > nowpos) {
        //走过头
        return;
      }

    }
    else {
      if (encoder0Pos > nowpos) {
        //走过头
      }
      else {
        //缺布
        return;
      }
    }
  }
  else {
    infix = false;
  }

  digitalWrite(PinPwmOutStep, HIGH);
  digitalWrite(PinPwmOutStep, LOW);

}


void pwmClass::dosetup()
{

  pinMode(encoder0PinA, INPUT); //D2脚为输入
  Serial.print(" Endcoder PinA:");
  Serial.print(encoder0PinA);
  pinMode(encoder0PinB, INPUT); //D3脚为输入
  Serial.print(" Endcoder PinB:");
  Serial.print(encoder0PinB);

  digitalWrite(encoder0PinA, HIGH); // turn on pullup resistor
  digitalWrite(encoder0PinB, HIGH); // turn on pullup resistor

  pinMode(PinPWMStep, INPUT); //D2脚为输入
  pinMode(PinPWMDir, INPUT); //D3脚为输入

  Serial.print(" PinPwm Step:");
  Serial.print(PinPWMStep);

  Serial.print(" PinPwm Dir:");
  Serial.print(PinPWMDir);
  Serial.println("");

  pinMode(PinPwmOutStep, OUTPUT);
  pinMode(PinPwmOutDir, OUTPUT);

  Serial.print(" PinPwmOutStep:");
  Serial.print(PinPwmOutStep);
  Serial.print(" PinPwmOut Dir:");
  Serial.print(PinPwmOutDir);
  Serial.println("");

  for (size_t i = 0; i < 6; i++)
  {
    if (InterruptMaps[i] == encoder0PinA) {
      //attachInterrupt(InterruptDefines[i], this->doloop, CHANGE);  //注册中断
      Serial.print(" InterruptDefines A:");
      Serial.print(InterruptMaps[i]);
      Serial.print("->");
      Serial.print(InterruptDefines[i]);
      InterruptA = InterruptDefines[i];
      break;
    }
  }
  for (size_t i = 0; i < 6; i++)
  {
    if (InterruptMaps[i] == PinPWMStep) {
      //attachInterrupt(InterruptDefines[i], pwmInput, CHANGE);  //注册中断
      Serial.print(" InterruptDefines B:");
      Serial.print(InterruptMaps[i]);
      Serial.print("->");
      Serial.print(InterruptDefines[i]);
      InterruptB = InterruptDefines[i];
      break;
    }
  }

  time = micros(); //时间初值

  Serial.println("");
}

void pwmClass::doloop()
{
  bool debugOut = false;
  //
  if (true) {
    //测试输出
    if (num != encoder0Pos) {
      while (num != encoder0Pos)
      {
        num = encoder0Pos;
      }
      if (debugOut) {
        Serial.print("pos");
        Serial.print(pwmnum);
        Serial.print(" ");
        Serial.println(num);
      }
    }
    //
    if (num2 != targetPos) {
      while (num2 != targetPos)
      {
        num2 = targetPos;

        lastms = micros();
      }
      if (debugOut) {
        Serial.print("pwm");
        Serial.print(pwmnum);
        Serial.print(" ");
        Serial.println(targetPos);
      }
    }

  }

  //长时间没动则清0
  int mspass = (micros() - lastms) / 1000000;

  if (mspass >= 60 && false) {
    lastms = micros();
    Serial.print(pwmnum);
    Serial.print(" ");
    Serial.println("clear");

    targetPos = 0;
    encoder0Pos = 0;
  }
}

void pwmClass::printPwmPos() {
  Serial.print(this-> pwmnum);
  Serial.print(" ");
  Serial.print(F("Position="));
  Serial.print(encoder0Pos);
  Serial.print(F(" target1="));
  Serial.print(targetPos);
  Serial.print(F(" targetPostion="));
  Serial.print(targetPos / pwmStepUnit);
  Serial.print(F(" pwmStepUnit="));
  Serial.print(pwmStepUnit);

  int nowpos = targetPos / pwmStepUnit;

  int p = abs(nowpos - encoder0Pos);
  Serial.print(F(" p="));
  Serial.print(p);
  Serial.println("");

}






