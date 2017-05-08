

#include "pwmClass.h"

//通道数量
int defineCount = 1;

pwmClass pwmDefines[] = { pwmClass(),pwmClass() };

//输入控制
int InterruptDefines[] = { 0,1,2,3 };

int pinDefines[] = {
	2,8, //encoder0PinA //encoder0PinB
	3,7, //pwmIn  //pwmDir
	9,10, //pwmOut //pwmOutDir


};

//初始化
void setup()
{

	Serial.begin(115200);
	Serial.println("start"); // a personal quirk
	printPos();

	for (size_t i = 0; i < defineCount; i++)
	{
		pwmClass pwmdefine = pwmDefines[i];
		int x = i * 6;
		pwmdefine.encoder0PinA = pinDefines[x];
		pwmdefine.encoder0PinB = pinDefines[x + 1];
		pwmdefine.PinPWMStep = pinDefines[x + 2];
		pwmdefine.PinPWMDir = pinDefines[x + 3];
		pwmdefine.PinPwmOutStep = pinDefines[x + 4];
		pwmdefine.PinPwmOutDir = pinDefines[x + 5];
		pwmdefine.pwmnum = i + 1;
	}

	for (size_t i = 0; i < defineCount; i++)
	{
		pwmClass pwmdefine = pwmDefines[i];
		pwmdefine.dosetup(InterruptDefines[i * 2], InterruptDefines[i * 2 + 1]);
	}


	interrupts();

}


void loop()
{
	for (size_t i = 0; i < defineCount; i++)
	{
		pwmDefines[i].doloop();
	}
	if (micros() % 100000 == 0) {
		printPos();

	}

}




void process_line() {
	if (Serial.available()) {
		char cmd = Serial.read();

		switch (cmd) {
		case '?':
			printPos();
			break;

		}
		while (Serial.read() != 10);
	}
}


void printPos() {
	for (size_t i = 0; i < defineCount; i++)
	{
		pwmDefines[i].printPwmPos();
	}
}

