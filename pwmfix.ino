#define encoder0PinA 2  //A输入
#define encoder0PinB 8  //B输入


#define PinPWMStep 3//PWM 输入
#define PinPWMDir  7//Dir 输入

#define PinPwmOutStep 9 //pwm输出
#define PinPwmOutDir 10 //dir输出

#define encodeStep 400 //编码器一圈步数 0.9度 
#define pwmStep 200 //pwm 一圈步数 1.8度 
#define pwmNs 16 //pwm细分 数


//输入的步数 = 360/16/20
//联轴器连接
//如果采用同步带连接，需要计算当前同步齿轮大小 齿轮大小如果一致可不管
//不一致需要计算各自的齿轮比 
#define linkValue 1.0




//最大输出速度，过大的输出，会导致失步，所以保持一个最高速度
//默认响应速度肯定比原始速度快
//当失步时，累计步数将被叠加
#define maxOutSpeed = 10000

volatile int encoder0Pos = 0; 

unsigned long time = 0;

long targetPos = 0; //目标位置
long num = 0;

//每8个脉冲=1个解码器信号
//当前pwm信号和解码器信号的比率
long pwmStepUnit = pwmStep * pwmNs / encodeStep / 2 * linkValue;

long lastnum = 0;
unsigned long lastms = 0; //长时间没动则清0
//初始化
void setup()
{
	Serial.begin(115200); //窗口初始化

	pinMode(encoder0PinA, INPUT); //D2脚为输入
	pinMode(encoder0PinB, INPUT); //D3脚为输入

	pinMode(PinPWMStep, INPUT); //D2脚为输入
	pinMode(PinPWMDir, INPUT); //D3脚为输入

	//有高低，所以 一步=2 一圈总步数= 单圈步数*2
	attachInterrupt(0, encode, CHANGE);  //注册中断0   //D2 解码器输入
	attachInterrupt(1, pwmInput, CHANGE); //注册中断1 	//D3 PWM输入
	time = micros(); //时间初值
}


void loop()
{
	//测试输出
	while (num != encoder0Pos)
	{
		num = encoder0Pos;
		Serial.println(num);
	}
	//长时间没动则清0
	
		if (micros() - lastms / 1000000 > 60) {
			targetPos = 0;
			encoder0Pos = 0;
			lastms = micros();
		}
		else
		{
			int lastChangeMs = micros() - lastms / 1000;
			//5毫秒，并没有变化
			if (lastChangeMs > 1) {
				int nowpos = targetPos / pwmStepUnit;
				int p = abs(nowpos - encoder0Pos);
				//如果电机处于低速或者停止状态
				//回抽，或者换层
				if (p > 5) {
					//尝试修复
					if (nowpos < targetPos) {
						digitalWrite(PinPwmOutDir, LOW);

						digitalWrite(PinPwmOutStep, HIGH);
						digitalWrite(PinPwmOutStep, LOW);
					}
					else
					{
						digitalWrite(PinPwmOutDir, HIGH);

						digitalWrite(PinPwmOutStep, HIGH);
						digitalWrite(PinPwmOutStep, LOW);
					}
					delayMicroseconds(10);
				}
			}
			
		}
	

	
	//
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
		else{
			targetPos--;
		}

		//默认直接输出
		
		int nowpos = targetPos / pwmStepUnit;
		
		int p = abs(nowpos - encoder0Pos);

		//当获得下一个信号，并且，解码器误差和当前误差过大
		//超过5
		if (p >5 ) {
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


