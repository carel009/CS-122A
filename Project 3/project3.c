// Christopher Arellano

#include "rims.h"

volatile int TimerFlag = 0;

void TimerISR() {
   TimerFlag = 1;
}

int desiredValue = 0;
int actualValue = 0;
int error = 0;
int integral = 0;
int derivative = 0;
int actualPrevious = 0;
int actuator = 0;
const int actualValueMin = 0;
const int actualValueMax = 105;
const int actuatorMin = 0;
const int actuatorMax = 15;
const int integralMin = 5049;
const int Kp = 50; // 0.05
const int Ki = 1; // 0.001
const int Kd = 2500; //2.5
const int disturbance = 5;

int a0 = 0;
int a1 = 0;
int a2 = 0;
int a3 = 0;
int a4 = 0;
int a5 = 0;

void PID() {
    
    if (!A0) {a0 = 0;}
    if (A0) {a0 = 5;}
    
    if (!A1) {a1 = 0;}
    if (A1) {a1 = 10;}
    
    if (!A2) {a2 = 0;}
    if (A2) {a2 = 15;}
    
    if (!A3) {a3 = 0;}
    if (A3) {a3 = 20;}
    
    if (!A4) {a4 = 0;}
    if (A4) {a4 = 25;}
    
    if (!A5) {a5 = 0;}
    if (A5) {a5 = 30;}
    
    desiredValue = (a0+a1+a2+a3+a4+a5);
    
    if (A7) {
        desiredValue = 0;
        actualValue = 0;
        integral = 0;
        actualPrevious = 0;
    }
    
    printf("Desired Speed = %d mph\n", desiredValue);
    
    error = desiredValue - actualValue;

    integral = integral + error;
    
    if (integral < integralMin) {integral = integralMin;}
    
    derivative = actualValue - actualPrevious;
   
    actuator = ((Kp * error) + (Ki * integral) - (Kd * derivative))/1000;
    
    if (actuator < actuatorMin) {actuator = actuatorMin;}
    if (actuator > actuatorMax) {actuator = actuatorMax;}
    
    actualValue = (actualPrevious) + (actuator) - (disturbance);
    
    if (actualValue < actualValueMin) {actualValue = actualValueMin;}
    if (actualValue > actualValueMax) {actualValue = actualValueMax;}
    
    if (!A6) {B = actualValue;}
    
    if (A6) {printf("Current Speed = %d mph\n", actualValue); B = 0;}
    
    actualPrevious = actualValue;
}

int main(){
   TimerSet(1000);
   TimerOn();
   while(1){
      PID(); 
      while(!TimerFlag);
      TimerFlag=0;
   }
   return 0;
}      