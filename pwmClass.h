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
	unsigned  int encoder0PinA;  //A输入
	unsigned  int encoder0PinB;  //B输入


	unsigned  int  PinPWMStep;//PWM 输入
	unsigned  int PinPWMDir;//Dir 输入

	unsigned  int PinPwmOutStep; //pwm输出
	unsigned  int PinPwmOutDir; //dir输出

	unsigned  int encodeStep; //编码器一圈步数 0.9度 
	unsigned  int pwmStep; //pwm 一圈步数 1.8度 
	unsigned  int pwmNs; //pwm细分 数
	unsigned int pwmnum;//通道编号
	double linkValue;

	void dosetup(int Interrupt0, int Interrupt1);

	void doloop();

	void printPwmPos();

private:
	volatile int encoder0Pos = 0;

	unsigned long time = 0;

	long targetPos = 0; //目标位置
	long num = 0;

	//每8个脉冲=1个解码器信号
	//当前pwm信号和解码器信号的比率
	long pwmStepUnit = pwmStep * pwmNs / encodeStep / 2 * linkValue;

	long lastnum = 0;
	unsigned long lastms = 0; //长时间没动则清0

};

#endif

