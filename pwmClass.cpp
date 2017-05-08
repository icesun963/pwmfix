#include "pwmClass.h"

unsigned  int encoder0PinA = 2;  //A输入
unsigned  int encoder0PinB = 8;  //B输入


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
long pwmStepUnit = pwmStep * pwmNs / encodeStep / 2 * linkValue;

long lastnum = 0;

unsigned long lastms = 0; //长时间没动则清0
unsigned long time = 0;
unsigned int pwmnum = 1;

 void pwmClass::doloop()
{
	bool debugOut = true;
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



//中断0调用函数
void encode()
{
	if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
	{
		encoder0Pos++;
	}
	else
	{
		encoder0Pos--;
	}
	//time = micros();
}

//中断1函数，pwm输入
void pwmInput() {

	lastms = micros();

	int dir = digitalRead(PinPWMDir);
	//直接输出
	if (digitalRead(PinPWMStep) == HIGH) {

		if (dir == 0) {
			targetPos++;
		}
		else {
			targetPos--;
		}

		//默认直接输出

		int nowpos = targetPos / pwmStepUnit;

		int p = abs(nowpos - encoder0Pos);

		//当获得下一个信号，并且，解码器误差和当前误差过大
		//超过5
		if (p > 5) {
			if (dir == 0) { //正走
				if (nowpos > encoder0Pos) {
					//当前过前
					//则不走
				}
				else
				{
					//不够 继续走
					digitalWrite(PinPwmOutStep, HIGH);
				}
			}
			else {
				if (encoder0Pos < nowpos) {
					//往回走，现在位置已经小于 目标位置
					//不走
				}
				else
				{
					//不够 继续走 反转
					digitalWrite(PinPwmOutStep, HIGH);
				}
			}
		}
		else {
			digitalWrite(PinPwmOutStep, HIGH);
		}


	}
	else
	{
		//低信号不管他
		digitalWrite(PinPwmOutStep, LOW);
	}


}

void pwmClass::dosetup(int Interrupt0, int Interrupt1)
{


	pinMode(encoder0PinA, INPUT); //D2脚为输入
	pinMode(encoder0PinB, INPUT); //D3脚为输入

	pinMode(PinPWMStep, INPUT); //D2脚为输入
	pinMode(PinPWMDir, INPUT); //D3脚为输入

							   //有高低，所以 一步=2 一圈总步数= 单圈步数*2
	attachInterrupt(Interrupt0, encode, CHANGE);  //注册中断0   //D2 解码器输入
	attachInterrupt(Interrupt1, pwmInput, CHANGE); //注册中断1 	//D3 PWM输入
	time = micros(); //时间初值
}

void pwmClass::printPwmPos() {
	Serial.print(pwmnum);
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

