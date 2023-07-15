// Christopher Arellano

#include "RIMS.h"

volatile int TimerFlag = 0;
void TimerISR()
{
   TimerFlag = 1;
}

// SM function here
int stat = 0;

enum System_States { Start, Off_1, Check_On, On, On_1, Check_Off, Off} System_State;

void System() {
   switch ( System_State ) { //Transitions
      case Start:
         System_State = Off_1; //Initial state
         break;
      case Off_1:
         if(A0)
         {
            System_State = Check_On;
         }
         else
         {
            System_State = Off_1;
         }
         break;
      case Check_On:
         if(A0)
         {
            System_State = On;
         }
         else
         {
            System_State = Off_1;
         }
         break;
      case On:
         if(!A0)
         {
            System_State = On_1;
         }
         else
         {
            System_State = On;
         }
         break;
      case On_1:
         if(A0)
         {
            System_State = Check_Off;
         }
         else
         {
            System_State = On_1;
         }
         break;
      case Check_Off:
         if(A0)
         {
            System_State = Off;
         }
         else
         {
            System_State = On_1;
         }
         break;
      case Off:
         if(!A0)
         {
            System_State = Off_1;
         }
         else
         {
            System_State = Off;
         }
         break;
      default:
         System_State = Start;
         break;
   }

   switch ( System_State ) { //State actions
      case Off_1:
         stat = 0;
         break;
      case On:
         stat = 1;
         break;
      case On_1:
         stat = 1;
         break;
      case Off:
         stat = 0;
         break;
      default:
         break;
   }
}

// SM function here
enum Status_States { start, check, off, on} Status_State;

void Status() {
   switch ( Status_State ) { //Transitions
      case start:
         Status_State = check; //Initial state
         break;
      case check:
         if (stat == 0)
         {
            Status_State = off;
         }
         else if (stat == 1)
         {
            Status_State = on;
         }
         else
         {
            Status_State = check;
         }
         break;
      case off:
         Status_State = check;
         break;
      case on:
         Status_State = check;
         break;
      default:
         Status_State = start;
         break;
   }
   
   switch ( Status_State ) { //State actions
      case off:
         B7 = B6 = B5 = B4 = B3 = B2 = B1 = B0 = 0; // System Off
         break;
      case on:
         B7 = 1; // System On
         break;
      default:
         break;
   }
}

// SM function here
enum Game_States { StartGame, Led1, Led2, Led3, Led4, Led5, Led6, Led7, Led6_1, Led5_1, Led4_1, Led3_1, Led2_1, Check_Stop, Stop, Flash_On, Flash_Off, Flash_On1, Flash_Off1, Stay_On, Check_Resume, Resume} Game_State;

void Game() {
   switch ( Game_State ) { //Transitions
      case StartGame:
         Game_State = Led1; //Initial state
         break;
      case Led1:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led2;
         }
         break;
      case Led2:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led3;
         }
         break;
      case Led3:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led4;
         }
         break;
      case Led4:
         if(A1)
         {
            Game_State = Flash_On;
         }
         else
         {
            Game_State = Led5;
         }
         break;
      case Led5:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led6;
         }
         break;
      case Led6:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led7;
         }
         break;
      case Led7:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led6_1;
         }
         break;
      case Led6_1:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led5_1;
         }
         break;
      case Led5_1:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led4_1;
         }
         break;
      case Led4_1:
         if(A1)
         {
            Game_State = Flash_On;
         }
         else
         {
            Game_State = Led3_1;
         }
         break;
      case Led3_1:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led2_1;
         }
         break;
      case Led2_1:
         if(A1)
         {
            Game_State = Check_Stop;
         }
         else
         {
            Game_State = Led1;
         }
         break;
      case Check_Stop:
         if(!A1)
         {
            Game_State = Stop;
         }
         else
         {
            Game_State = Check_Stop;
         }
         break;
      case Stop:
         if(A1)
         {
            Game_State = Resume;
         }
         else
         {
            Game_State = Stop;
         }
         break;
      case Flash_On:
         Game_State = Flash_Off;
         break;
      case Flash_Off:
         Game_State = Flash_On1;
         break;
      case Flash_On1:
         Game_State = Flash_Off1;
         break;
      case Flash_Off1:
         Game_State = Stay_On;
         break;
      case Stay_On:
         if(!A1)
         {
            Game_State = Check_Resume;
         }
         else
         {
            Game_State = Stay_On;
         }
         break;
      case Check_Resume:
	      if(A1)
	      {
	         Game_State = Resume;
	      }
	      else
         {
            Game_State = Check_Resume;
         }
	      break;
      case Resume:
	      if(!A1)
	      {
	         Game_State = Led1;
	      }
	      else
         {
            Game_State = Resume;
         }
	      break;
      default:
         Game_State = StartGame;
         break;
   }
   
   switch ( Game_State ) { //State actions
      case Led1:
         B6 = 0; B5 = 0; B4 = 0; B3 = 0; B2 = 0; B1 = 0; B0 = 1;
         break;
      case Led2:
         B6 = 0; B5 = 0; B4 = 0; B3 = 0; B2 = 0; B1 = 1; B0 = 0;
         break;
      case Led3:
         B6 = 0; B5 = 0; B4 = 0; B3 = 0; B2 = 1; B1 = 0; B0 = 0;
         break;
      case Led4:
         B6 = 0; B5 = 0; B4 = 0; B3 = 1; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Led5:
         B6 = 0; B5 = 0; B4 = 1; B3 = 0; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Led6:
         B6 = 0; B5 = 1; B4 = 0; B3 = 0; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Led7:
         B6 = 1; B5 = 0; B4 = 0; B3 = 0; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Led6_1:
         B6 = 0; B5 = 1; B4 = 0; B3 = 0; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Led5_1:
         B6 = 0; B5 = 0; B4 = 1; B3 = 0; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Led4_1:
         B6 = 0; B5 = 0; B4 = 0; B3 = 1; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Led3_1:
         B6 = 0; B5 = 0; B4 = 0; B3 = 0; B2 = 1; B1 = 0; B0 = 0;
         break;
      case Led2_1:
         B6 = 0; B5 = 0; B4 = 0; B3 = 0; B2 = 0; B1 = 1; B0 = 0;
         break;
      case Flash_On:
         B6 = 1; B5 = 1; B4 = 1; B3 = 1; B2 = 1; B1 = 1; B0 = 1;
         break;
      case Flash_Off:
         B6 = 0; B5 = 0; B4 = 0; B3 = 0; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Flash_On1:
         B6 = 1; B5 = 1; B4 = 1; B3 = 1; B2 = 1; B1 = 1; B0 = 1;
         break;
      case Flash_Off1:
         B6 = 0; B5 = 0; B4 = 0; B3 = 0; B2 = 0; B1 = 0; B0 = 0;
         break;
      case Stay_On:
         B6 = 1; B5 = 1; B4 = 1; B3 = 1; B2 = 1; B1 = 1; B0 = 1;
         break;
      default:
         break;
   }
}

int main()
{
int ElapsedTime = 500;
int periodGCD = 500;
TimerSet(periodGCD);
TimerOn();

// SM start here
System_State = Start;
Status_State = start;
Game_State = StartGame;

   while(1)
   {
      if (ElapsedTime >= 500)
      {
         System();
         if (stat == 1)
         {
            // SM Call here
            Status();
            Game();  
         }
         else
         {
            // SM Call here
            Status();
            Game_State = StartGame; // Reset Game
            System();
         }
         
         ElapsedTime = 0;
      }
      ElapsedTime += 500;
      while(!TimerFlag);
      TimerFlag=0;
   }
return 0;
}