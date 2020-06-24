/*
 * main.c
 *
 *  Created on: Apr 5, 2020
 *      Author: Alaa Abdah
 */

#include "Std_Types.h"
#include "Utils.h"
#include "DIO.h"
#include "TIMER0.h"
#include "UART.h"
#include "Scheduler.h"




uint8 LCD_MainFlag =0;
uint8 Traffic_MainFlag =0;



int main()
{

//	//////////
//	DIO_vidSetPortDir(0,0xff);
//	DIO_vidSetPortDir(3,0xff);
//
	SET_BIT(DDRA,0);
//	SET_BIT(DDRB,3);
//	SET_BIT(DDRB,2);
//	//SET_BIT(SREG,7);

	timer0_vidInit(presc8,ctc,125);
	timer0_vidIntMs();


//	LED_vidInit();
//	LED_vidToggle();

	///////GLOPAL TIMER /////
	

	//TIMER_vidSetISRCallback(Sch_UpdateTick );
	//timer0_vidInit(presc8,ctc,125);
	Timer0_vidSetCallBack(Sch_UpdateTick);


	UART_vidInit(TxRxOn,UnormalMode,stop1size8,DataRegEmpInter,risingTXfallingRX,parityDisapled);


	//lcd_vidWriteData('A');
	//DIO_vidSetPinDir(1,HIGH);

	while (1)
	{

		Sch_Start();
		
		
		


	}
return 0;
}




