// Christopher Arellano

#include "RIMS.h"

volatile int TimerFlag = 0;

void TimerISR() {
   TimerFlag = 1;
}

int pid = 0;
int manual = 0;

void ControlSelect() {
   if (A0) { manual = 1; pid = 0;}
   else { pid = 1; manual = 0;}
}

int value = 0;
int height = 0;

void ModeSelect() {
   if (A1) { height = 1; value = 0;}
   else { value = 1; height = 0;}
}

int speed = 0;
const speedChange = 700; 

/*void FanSpeed() {
   if (!A7 && A6) { speed = speed + speedChange;}
   if (A7 && !A6) { speed = speed - speedChange;}
}*/

const int desiredValue = 100;
int actualValue = 0;
int error = 0;
int integral = 0;
int derivative = 0;
int actualPrevious = 0;
int actuator = 0;
const int actuatorMax = 10;
const int actuatorMin = 0;
const int integralMax = 1000;
const int integralMin = -1000;
const int baseSpeed = 700; // 0.7
const int Kp = 50; // 0.05
const int Ki = 1; // 0.001
const int Kd = 2500; //2.5

int fanAcceleration = 0;
int fanForce = 0;
int ballAcceleration = 0;
int ballVelocity = 0;
int ballPosition = 0;
const int fanMass = 500; // 0.5
const int ballMass = 27; // 0.027
const int gravity = 9810; // 9.81
const int time = 50; // 0.05
const int ballMin = 0;
const int ballMax = 200;


void PID() {
   if (pid == 1) {
      error = desiredValue - actualValue;

      integral = integral + error;
   
      if (integral < integralMin) {integral = integralMin;}
      if (integral > integralMax) {integral = integralMax;}

      derivative = actualValue - actualPrevious;
   
      actuator = ((Kp * error) + (Ki * integral) - (Kd * derivative))/1000;
   
      if (actuator < actuatorMin) {actuator = actuatorMin;}
      if (actuator > actuatorMax) {actuator = actuatorMax;}
   
      actuator = ((actuator * 1000) + baseSpeed)/1000;

      fanAcceleration = actuator * 1000;
      fanForce = fanMass * fanAcceleration;
   
      ballAcceleration = (fanForce/ballMass) - gravity;
      ballVelocity = ballVelocity + ((ballAcceleration * time)/1000);
      ballPosition = (ballPosition + ((ballVelocity * time)/1000))/1000;
      if (ballPosition < ballMin) { ballVelocity = 0; ballPosition = ballMin;}
      if (ballPosition > ballMax) { ballVelocity = 0; ballPosition = ballMax;}
   
      actualPrevious = actualValue;
      actualValue = ballPosition;
      ballPosition = ballPosition * 1000;
   
      if (height == 1) {    
         if ((actualValue >= 0) & (actualValue <= 24)) { B = 0x80; }
         if ((actualValue >= 25) & (actualValue <= 49)) { B = 0x40; }
         if ((actualValue >= 50) & (actualValue <= 74)) { B = 0x20; }
         if ((actualValue >= 75) & (actualValue <= 99)) { B = 0x10; }
         if ((actualValue == 100)) { B = 0x08; }
         if ((actualValue >= 101) & (actualValue <= 134)) { B = 0x04; }
         if ((actualValue >= 135) & (actualValue <= 167)) { B = 0x02; }
         if ((actualValue >= 168) & (actualValue <= 200)) { B = 0x01; }  
      }
   
      if (value == 1) { B = actualValue; }
   
   }
   
   if (manual == 1) {
      if (!A7 && A6) { speed = speed + speedChange;}
      if (A7 && !A6) { speed = speed - speedChange;}
      
      printf("speed = %d\n" , speed); // useful to see speed
   
      fanAcceleration = speed;
      fanForce = fanMass * fanAcceleration;
   
      ballAcceleration = (fanForce/ballMass) - gravity;
      ballVelocity = ballVelocity + ((ballAcceleration * time)/1000);
      ballPosition = (ballPosition + ((ballVelocity * time)/1000))/1000;
      if (ballPosition < ballMin) { ballVelocity = 0; ballPosition = ballMin;}
      if (ballPosition > ballMax) { ballVelocity = 0; ballPosition = ballMax;}
   
      actualValue = ballPosition;
      
      ballPosition = ballPosition * 1000;
   
      if (height == 1) {    
         if ((actualValue >= 0) & (actualValue <= 24)) { B = 0x80; }
         if ((actualValue >= 25) & (actualValue <= 49)) { B = 0x40; }
         if ((actualValue >= 50) & (actualValue <= 74)) { B = 0x20; }
         if ((actualValue >= 75) & (actualValue <= 99)) { B = 0x10; }
         if ((actualValue == 100)) { B = 0x08; }
         if ((actualValue >= 101) & (actualValue <= 134)) { B = 0x04; }
         if ((actualValue >= 135) & (actualValue <= 167)) { B = 0x02; }
         if ((actualValue >= 168) & (actualValue <= 200)) { B = 0x01; }  
      }
   
      if (value == 1) { B = actualValue; }
   
   }
}

int main(){
   TimerSet(1000);
   TimerOn();
   while(1){
      
      ControlSelect(); // check A0 (0 - pid) (1 - manual)
      ModeSelect(); // check A1 (0 - value) (1 - height)
      //FanSpeed(); // adjust fan speed
      PID(); // calculations
      
      while(!TimerFlag);
      TimerFlag=0;
   }
   return 0;
}