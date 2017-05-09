

#include "pwmClass.h"

//通道数量
int defineCount = 2;

pwmClass* pwmDefines[2] ;


int pinDefines[] = {
  20, 14, //encoder0PinA //encoder0PinB
  3, 11, //pwmIn  //pwmDir
  13, 12, //pwmOut //pwmOutDir
  
  21, 15,
  2, 8,
  10, 9,
};

//初始化
void setup()
{

  Serial.begin(115200);
  Serial.println("start"); // a personal quirk
  printPos();

  for (size_t i = 0; i < defineCount; i++)
  {
    pwmClass* pwmdefine = new pwmClass();
    int x = i * 6;
    pwmdefine->encoder0PinA = pinDefines[x];
    pwmdefine->encoder0PinB = pinDefines[x + 1];
    pwmdefine->PinPWMStep = pinDefines[x + 2];
    pwmdefine->PinPWMDir = pinDefines[x + 3];
    pwmdefine->PinPwmOutStep = pinDefines[x + 4];
    pwmdefine->PinPwmOutDir = pinDefines[x + 5];
    pwmdefine->pwmnum = i + 1;
    pwmdefine->dosetup();
   
    pwmDefines[i] = pwmdefine;

    if (i == 0) {
      
      attachInterrupt(pwmdefine->InterruptA, InterruptA1, CHANGE);
      attachInterrupt(pwmdefine->InterruptB, InterruptB1, RISING);
      
    }
    else {
       pwmdefine->encoder0Dir = 1;
      attachInterrupt(pwmdefine->InterruptA, InterruptA2, CHANGE);
      attachInterrupt(pwmdefine->InterruptB, InterruptB2, RISING);
    }
  }

  //attachInterrupt(1,InterruptB1,RISING);
  interrupts();

}
void InterruptTest(){
   Serial.println("InterruptTest");
}

void InterruptA1() {
  pwmDefines[0]->encode();
}

void InterruptB1() {
  //Serial.println("InterruptB1");
  pwmDefines[0]->pwmInput();
}

void InterruptA2() {
   pwmDefines[1]->encode();
}

void InterruptB2() {
  pwmDefines[1]->pwmInput();
}


void loop()
{
  for (size_t i = 0; i < defineCount; i++)
  {
    pwmDefines[i]->doloop();
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
    pwmDefines[i]->printPwmPos();
  }
}

