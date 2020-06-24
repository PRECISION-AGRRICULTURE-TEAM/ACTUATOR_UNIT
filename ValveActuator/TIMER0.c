/*
 * TIMER0.c
 *
 *  Created on: Dec 21, 2019
 *      Author: Alaa Abdah
 */



#include "TIMER0.h"

uint32 counter =0;
uint32 SCounter=0;
void (* Timer_vidCallBack_Ptr )(void);

void timer0_vidInit(uint8 clkValue ,uint8 mode,uint8 compValue )
{


	CLEAR_PORT(TCCR0);
	CLEAR_PORT(TIMSK);
	ASSIGN_PORT(TCCR0,TCCR0|mode);
	ASSIGN_PORT(TCCR0,TCCR0|clkValue);


	switch (mode)
	{
		case normal	:ASSIGN_PORT(OCR0, 0xff);

					break;
		case ctc	:ASSIGN_PORT(OCR0, compValue);
					break;
	}

}

void timer0_vidDelayMs( uint16 msTime)
{
	uint8 mode = GET_PORT(TCCR0)&MODE;
	switch (mode)
	{
	case normal	:ASSIGN_PORT(TIMSK,TIMSK|overflowIntEn);
				break;
	case ctc	:ASSIGN_PORT(TIMSK,TIMSK|compareInterEn);
			break;
	}
	uint16 fcpuInMS = F_CPU/1000;
	uint16 msTFreq ;
	uint8 prescaller = GET_PORT(TCCR0)& PRESCALLER;
	switch (prescaller)
	{
		case noPrescaller: msTFreq = fcpuInMS;
					break;
		case presc8: msTFreq = fcpuInMS/8;
					break;
		case presc64: msTFreq =fcpuInMS/64;
						break;
		case presc256:msTFreq = fcpuInMS/256;
						break;
		case presc1024: msTFreq =fcpuInMS/1024;
						break;
	}

	uint8 compValue = GET_PORT(OCR0);
	uint16 msFComp  = msTFreq/compValue;
	uint32 noInt    = msFComp*msTime;

	while(1)
	{
		if ( counter == noInt)
			{
				counter =0;
				break;
			}
	}
	ASSIGN_PORT(TIMSK, TIMSK|noOvFlComInt);
}


void timer0_vidIntMs()
{

	uint16 fcpuInMS = F_CPU/1000;

	uint8 mode = GET_PORT(TCCR0)&MODE;
	switch (mode)
	{
	case normal	:
		ASSIGN_PORT(TIMSK,TIMSK|overflowIntEn);
		break;
	case ctc	:

		ASSIGN_PORT(TIMSK,TIMSK|compareInterEn);
		break;
	}

	uint32 msTFreq ;
	uint16 temp = fcpuInMS;;
	uint8 prescaller ;
	uint8 fraction8 = fcpuInMS%8;
	uint8 ocr0 ;
	uint8 fraction64 = fcpuInMS%64;
	uint8 fraction256 = fcpuInMS%256;
	uint8 fraction1024 = fcpuInMS%1024;

	if ( fcpuInMS <256 )
		{
		temp = fcpuInMS%1;
		prescaller = noPrescaller;
		ocr0  = fcpuInMS -1;
		}
	if ( (fcpuInMS/8)<256 )
	{
		if (fraction8< temp)
			{
			temp = fcpuInMS%8;
			prescaller = presc8;
			ocr0  = (fcpuInMS/8)-1;
			}
	}
	if ( (fcpuInMS/64)<256 )
	{
		if (fraction64< temp)
		{


			temp = fcpuInMS%64;
			prescaller = presc64;
			ocr0  = (fcpuInMS/64) -1;
		}
	}
	if ( (fcpuInMS/256)<256 )
	{
		if (fraction256< temp)
		{
			temp = fcpuInMS%256;
			prescaller = presc256;
			ocr0  = (fcpuInMS/256 ) -1;
		}
	}
	if ( (fcpuInMS/1024)<256 )
	{
		if (fraction1024< temp)
		{
			temp = fcpuInMS%1024;
			prescaller = presc1024;
			ocr0  = (fcpuInMS/1024) -1;
		}
	}

	////////////////

	DIO_vidSetPinDir(11,1);
	///////////////////
	CLEAR_BIT(TCCR0,0);
	CLEAR_BIT(TCCR0,1);
	CLEAR_BIT(TCCR0,2);
	ASSIGN_PORT(TCCR0,TCCR0|prescaller|0x10);
	ASSIGN_PORT(OCR0 ,ocr0);







}



void Timer0_vidSetCallBack( void (*CallBack)(void) )
{
Timer_vidCallBack_Ptr = CallBack;
SET_BIT(SREG,GLOPAL_INT);
//SET_BIT(PORTB,0);
//SET_BIT(PORTB,1);
//
//		DIO_vidWritePin(10,1);

}

uint16 i =0;


void __vector_10(void) __attribute__((signal,__INTR_ATTRS));
void __vector_10(void)
{

	if ( counter++ >1000 )
	{
		SCounter++;
		counter=0;
	}

(Timer_vidCallBack_Ptr)();


}

//
//void __vector_11(void) __attribute__((signal,__INTR_ATTRS));
//void __vector_11(void)
//{
//
//}

