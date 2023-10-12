// Platforms&Dragons.c
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 1/12/2022 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer0A.h"
#include "Timer1.h"
#include "Timer2.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

void PortE_Init(void){
		SYSCTL_RCGCGPIO_R |= 0x10;			// 1) activate clock for Port E 
		while((SYSCTL_PRGPIO_R&0x10) == 0){}; 
		GPIO_PORTE_DIR_R &= 0xD;			// 2) make PF 1,2,3 OUTPUT
		GPIO_PORTE_AFSEL_R &= ~0x2;		 // 3) DISable alternate fun on PE 1,2,3
		GPIO_PORTE_DEN_R |= 0x2;			// 4) ENable digital I/O on PE 1,2,3 
		GPIO_PORTE_AMSEL_R &= ~0x2;		 // 5) DISable analog fun on PE 1,2,3
}

void PortF_Init(void){
		SYSCTL_RCGCGPIO_R |= 0x20;			// 1) activate clock for Port f 
		while((SYSCTL_PRGPIO_R&0x20) == 0){}; 
		GPIO_PORTF_DIR_R |= 0xE;			// 2) make PF 1,2,3 OUTPUT
		GPIO_PORTF_AFSEL_R &= ~0xE;		 // 3) DISable alternate fun on PF 1,2,3
		GPIO_PORTF_DEN_R |= 0xE;			// 4) ENable digital I/O on PF 1,2,3 
		GPIO_PORTF_AMSEL_R &= ~0xE;		 // 5) DISable analog fun on PF 1,2,3
		GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
		GPIO_PORTF_DIR_R &= ~0x10;	
		GPIO_PORTF_AFSEL_R &= ~0x10; 
		GPIO_PORTF_DEN_R |= 0x10;
		GPIO_PORTF_CR_R = 0x1F;
		GPIO_PORTF_PUR_R= 0x10;
}

#define stop 0
#define moved 1
#define left 0
#define right 1

//Structs
struct Sprite{
	uint8_t x;
	uint8_t y;
	uint8_t xold;
	uint8_t yold;
	uint8_t direction; // 0 is left, 1 is right
	double dx;
	double dy;
	uint8_t w,h;
	uint8_t health;
	const unsigned short *image;
	uint8_t status;
};

typedef struct Sprite sprite;

sprite DS = {72,64,72,64,0,1,1,11,16,3,Left1};
sprite Dragon = {0,0,0,0,0,0,0,0,0};

//Functions
uint32_t Score = 0;
void TimeSurvived()
{
	Score++;
}

void Move(){
	DS.xold = DS.x;
	DS.yold = DS.y;
	DS.x +=DS.dx;
	DS.y +=DS.dy;
	DS.status = moved;
}

//Arrays


int main1(void){
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black

  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);
  while(1){
  }

}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char *Phrases[3][2]={
  {Hello_English,Hello_Spanish,},
  {Goodbye_English,Goodbye_Spanish,},
  {Language_English,Language_Spanish}
};

int main2(void){ char l;
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Delay100ms(30);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Delay100ms(20);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }  
}

int main(void){
	DisableInterrupts();
	//PLL_Init(Bus80MHz);
	TExaS_Init(NONE);
	PortE_Init();
	Output_Init();
	ADC_Init();
	ST7735_SetRotation(-1);
	ST7735_FillScreen(0xFFFF);
	EnableInterrupts();
	Timer0A_Init(&TimeSurvived,80000000);
	//Timer2_Init(&Move,8000000);
	//ST7735_InvertDisplay(1);
	
	// Draw the score, the platforms, and the Dragon Slayer on the middle platform
	ST7735_DrawFastHLine(58,65,41,ST7735_BLACK); ST7735_DrawFastHLine(21,38,41,ST7735_BLACK); ST7735_DrawFastHLine(94,38,41,ST7735_BLACK); ST7735_DrawFastHLine(21,91,41,ST7735_BLACK); ST7735_DrawFastHLine(94,91,41,ST7735_BLACK);
	ST7735_SetCursor(20,0);
	ST7735_OutUDec(0);
	ST7735_SetCursor(19,0);
	ST7735_OutUDec(0);
	ST7735_SetCursor(18,0);
	ST7735_OutUDec(0);
	ST7735_SetCursor(17,0);
	ST7735_OutUDec(0);
	ST7735_DrawBitmap(DS.x,DS.y,DS.image,DS.w,DS.h);
	DS.status = stop;
	
	//Main Loop
	while(1){
		
		if(Score>=10)
		{
			ST7735_SetCursor(19,0);
			ST7735_OutUDec(Score);
		}
		else if(Score>=100)
		{
			ST7735_SetCursor(18,0);
			ST7735_OutUDec(Score);
		}
		
		if((ADC_In() > 1365) && (ADC_In() < 2731))
		{
			if(DS.direction == left)
			{
				DS.image = Left1;
				ST7735_DrawBitmap(DS.x,DS.y,DS.image,DS.w,DS.h);
			}
			else
			{
				DS.image = Right1;
				ST7735_DrawBitmap(DS.x,DS.y,DS.image,DS.w,DS.h);
			}
			DS.status = stop;
		}
		
		if(ADC_In() <= 1365)
		{
			DS.image = RunLeft1;
			DS.direction = left;
			DS.dx = -2;
			DS.dy = 0;
			while(ADC_In() <= 1365)
			{
				Timer2_Init(&Move,8000000);
				ST7735_FillRect(DS.xold,DS.yold-DS.h,DS.w,DS.h,0xFFFF);
				ST7735_DrawBitmap(DS.x,DS.y,DS.image,DS.w,DS.h);
			}
		}
		
		if(ADC_In() >= 2731)
		{
			DS.image = RunRight1;
			DS.direction = right;
			DS.dx = 2;
			DS.dy = 0;
			while(ADC_In() <= 1365)
			{
				Timer2_Init(&Move,8000000);
				ST7735_FillRect(DS.xold,DS.yold-DS.h,DS.w,DS.h,0xFFFF);
				ST7735_DrawBitmap(DS.x,DS.y,DS.image,DS.w,DS.h);
			}
		}
	
 		/*while(DS.status==stop){}
		ST7735_FillRect(DS.xold,DS.yold-DS.h,DS.w,DS.h,0xFFFF);
		ST7735_DrawBitmap(DS.x,DS.y,DS.image,DS.w,DS.h);
		DS.status = stop;*/
	}
}