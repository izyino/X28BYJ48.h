#include "X28BYJ48.h"


//----------------------------------------------------------------------
 X28BYJ48:: X28BYJ48(uint8_t t)
{
  xtipostep=t;
}


//----------------------------------------------------------------------
void  X28BYJ48::begin() {
  pinMode(0, OUTPUT); pinMode(1, OUTPUT); pinMode(2, OUTPUT); pinMode(3, OUTPUT); //motores CN1
  pinMode(10, OUTPUT); //beep
  pinMode(8, OUTPUT); //led

  digitalWrite(0,0); digitalWrite(1,0); digitalWrite(2,0); digitalWrite(3,0);
  digitalWrite(10,0); digitalWrite(8,1);

  ledcAttachPin(10, 1);                              //define pino 10 channel 1 (beep)
  ledcSetup(1, 1000, 8);                             //PWM sempre a 1KHz
  ledcWrite(1, 0);                                   //grava 0 nele (silencia)

  const uint8_t timerNumber = 0;
  hw_timer_t *timer1ms = NULL;
  timer1ms = timerBegin(timerNumber, 80, true);
  isrTable[timerNumber] = this;
  auto isr = getIsr(timerNumber);
  timerAttachInterrupt(timer1ms, isr, false);
  timerAlarmWrite(timer1ms, 1000, true);
  timerAlarmEnable(timer1ms);
}


//----------------------------------------------------------------------
void  X28BYJ48::runStep(uint32_t steps, uint8_t velstep, boolean cwstep)
{

  xsteps=steps;
  xvelstep=60000L/passos[xtipostep-1]/velstep;
  xvelnow=xvelstep;
  xcwstep=cwstep;
  if (xcwstep){xfase=-1;}
  if (!xcwstep){xfase=4; if (xtipostep==3){xfase=8;}}
  digitalWrite(0,0); digitalWrite(1,0); digitalWrite(2,0); digitalWrite(3,0);
}


//----------------------------------------------------------------------
uint32_t  X28BYJ48::where()
{
  return xsteps;
}


//----------------------------------------------------------------------
void  X28BYJ48::beep(int xbnum, int xbdur, int xbfreq, int xbinter)
{
  bnum=xbnum; bdur=xbdur; bfreq=xbfreq; binter=xbinter;
}


//----------------------------------------------------------------------
void  X28BYJ48::led(int xlnum, int xldur, int xlinter)
{
  lnum=xlnum; ldur=xldur; linter=xlinter;
}


//----------------------------------------------------------------------
void  X28BYJ48::setms(uint32_t yms)
{
  xms=yms;
}


//----------------------------------------------------------------------
uint32_t  X28BYJ48::getms()
{
  return xms;
}


//----------------------------------------------------------------------
void  X28BYJ48::stopStep()
{
  xsteps=0;
}


//----------------------------------------------------------------------
void  X28BYJ48::stopBeep()
{
  bnum=0;
}


//----------------------------------------------------------------------
void  X28BYJ48::stopLed()
{
  lnum=0;
}


//----------------------------------------------------------------------
void IRAM_ATTR  X28BYJ48::onTimer1ms()
{
  if (xms>0){xms--;}

  //processa os steps---------------------------------------------------------------------------------
  if (xsteps>0){
    xvelnow--;
    if (xvelnow==0){
      xvelnow=xvelstep;
      int nf=3;if (xtipostep==3){nf=7;}
      if (xcwstep){xfase++;if (xfase>nf){xfase=0;}}else{xfase--;if (xfase<0){xfase=nf;}}
      X28BYJ48::go();
      xsteps--;
      if (xsteps==0){
        digitalWrite(0, 0);digitalWrite(1, 0);digitalWrite(2, 0);digitalWrite(3, 0);
      }
    }
  }
    

  if (bnum>0){
    if (bxpri){                           //if is the beginning of cycle to beep,
      bxinter=binter+1; bxdur=bdur;       //init the time variables
      bxpausa=false; bxpri=false;         //with default values or user values
      ledcSetup(1, bfreq, 8);             //
    }                                     // 
    if (!bxpausa && (bxdur>0)) {          //if it is beeping 
      ledcWrite(1, 127);bxdur--;          //keep the beep beeping for bxdur ms
      if(bxdur==0){                       //at end,
        ledcWrite(1, 0);                  //stop the beep and advise 
        bxpausa=true;                     //that pause fase is to be initiated
      }
    }
    if (bxpausa && (bxinter>0)){          //if it is in pause
      ledcWrite(1, 0);bxinter--;          //keep the beep stoped for bxinter ms
      if(bxinter==0){                     //at end, exit from pause, subtract 1 from quantity of desired 
        bxpausa=false;bnum--;bxpri=true;  //beeps and advise to reload the variables for a new cycle
      }
    }
  }


  if (lnum>0){
    if (lxpri){                           //if is the beginning of cycle to blink led,
      lxinter=linter+1; lxdur=ldur;       //init the time variables
      lxpausa=false; lxpri=false;         //with default values or user valuess
    }                                     // 
    if (!lxpausa && (lxdur>0)) {          //if the led is on (out of pause fase)
      digitalWrite(8, 0);lxdur--;        //keep the led on for lxdur ms
      if(lxdur==0){                       //at end,
        digitalWrite(8, 1);              //turn off the led and advise
        lxpausa=true;                     //that pause fase is to be initiated
      }
    }
    if (lxpausa && (lxinter>0)){          //if the led is off (pause fase)
      digitalWrite(8, 1);lxinter--;      //keep the led off for lxinter ms
      if(lxinter==0){                     //at end, exit from pause, subtract 1 from quantity of desired
        lxpausa=false;lnum--;lxpri=true;  //blinks and advise to reload the variables for a new cycle
      }
    }
  }

}

 X28BYJ48 * X28BYJ48::isrTable[SOC_TIMER_GROUP_TOTAL_TIMERS];


//----------------------------------------------------------------------
void  X28BYJ48::go()
{
  switch (xtipostep) {
    case 1:  X28BYJ48::move1(); break;   //28byj-48, 2048 steps, full step, low torque, low consumption
    case 2:  X28BYJ48::move2(); break;   //28byj-48, 2048 steps, full step, high torque, high consumption
    case 3:  X28BYJ48::move3(); break;   //28byj-48, 4096 steps, half step, high torque, high consumption
  }
}


//----------------------------------------------------------------------
void  X28BYJ48::move1(){   //28byj-48, 2048 steps, full step, low torque, low consumption
  switch (xfase) {
    case 0:  X28BYJ48::writ(0,0,0,1); break;   //0x01
    case 1:  X28BYJ48::writ(0,0,1,0); break;   //0x02
    case 2:  X28BYJ48::writ(0,1,0,0); break;   //0x04
    case 3:  X28BYJ48::writ(1,0,0,0); break;   //0x08
  }
}

void  X28BYJ48::move2(){   //28byj-48, 2048 steps, full step, high torque, high consumption
  switch (xfase) {
    case 0:  X28BYJ48::writ(1,0,0,1); break;   //0x09
    case 1:  X28BYJ48::writ(0,0,1,1); break;   //0x03
    case 2:  X28BYJ48::writ(0,1,1,0); break;   //0x06
    case 3:  X28BYJ48::writ(1,1,0,0); break;   //0x0C    
  }
}

void  X28BYJ48::move3(){   //28byj-48, 4096 steps, half step, high torque, high consumption
  switch (xfase) {
    case 0:  X28BYJ48::writ(1,0,0,1); break;   //0x09
    case 1:  X28BYJ48::writ(0,0,0,1); break;   //0x01
    case 2:  X28BYJ48::writ(0,0,1,1); break;   //0x03
    case 3:  X28BYJ48::writ(0,0,1,0); break;   //0x02
    case 4:  X28BYJ48::writ(0,1,1,0); break;   //0x06
    case 5:  X28BYJ48::writ(0,1,0,0); break;   //0x04
    case 6:  X28BYJ48::writ(1,1,0,0); break;   //0x0C
    case 7:  X28BYJ48::writ(1,0,0,0); break;   //0x08
  } 
}


//----------------------------------------------------------------------
void  X28BYJ48::writ(uint8_t px1, uint8_t px2, uint8_t px3, uint8_t px4)
{
  digitalWrite(0, px1);digitalWrite(1, px2);digitalWrite(2, px3);digitalWrite(3, px4);
}
//----------------------------------------------------------------------


