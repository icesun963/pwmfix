// pwmClass.h

#ifndef _PWMCLASS_h
#define _PWMCLASS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class pwmClass {

  public:



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
    

    void dosetup();

    void doloop();

    void printPwmPos();

    void encode();

    void pwmInput();

  private:



};

#endif

