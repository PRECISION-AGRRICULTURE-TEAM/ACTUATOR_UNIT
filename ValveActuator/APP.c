/*
 * APP.c
 *
 *  Created on: Jun 23, 2020
 *      Author: Alaa Abdah
 */

#include "APP.h"

//uint8 count =0;
//static uint8 index=0;
//uint8 * searchForFetchSize (uint8 *array, uint8* string,uint8 size )
//{
//	 static uint16 i=0;
//	 static uint8 j=0;
//
//	 static uint8 * arr;
//	 static uint8 searchState =0;
//	 switch ( searchState )
//	 {
//	 case 0:
//		 if (array[i]!='!')
//		 {
//			 /////////////// DEBUG /////////////
//			DIO_vidSetPortDir(0,0xff);
//			DIO_vidWritePort(0,string[j]);
//			count++;
//
//			 if ( string[j]!='\0')
//			 {
//
//			 	if ( string[j]== array[i])
//			 		{
//
//			 		j++;
//			 		}
//			 	//else j=0;
//			 }else
//			 {
//
//
//				 /
//
//				///////////////////////
//			 	if (index <= size)
//			 	{
//			 		/////////////// DEBUG /////////////
//			 					 					DIO_vidSetPinDir(8,1);
//			 					 					DIO_vidWritePin(8,1);
//				arr[index]=array[i];
//				index++;
//			 	}else
//			 	{
//
//			 		index=0;
//			 		i=0;
//			 		j=0;
//			 	arr[index]='\0';
//			 	return arr;
//			 	}
//			 }
//		 }
//		 searchState++;
//		 break;
//	 case 1:
//		 searchState=0;
//		 i++;
//		 break;
//	 }
//	 return 0;
//}



uint8 *  searchFor (uint8 *array, uint8* string,uint8 size )
{
	 uint16 i=0,j=0;
	 static uint8 arr[10];

for ( i =0;array[i]!='!';i++)
{

	if ( string[j]== array[i])
	{
		if ( string[j+1]=='\0')
		{
			for (j=0;j<size;j++)
			{
				arr[j]=array[i+1+j];
			}

			arr[j]='\0';
			j=0;

			return arr;
		}
		j++;

	}

}
return 0;


}

void copyString ( uint8 * string , uint8 * stringToCopy )
{
	uint8_t i,j;

	for ( j =0; string[j] != '\0' ; j++ )
	{}
	for (  i =0; stringToCopy[i] != '\0' ;i++)
	{
		string[i+j] = stringToCopy[i];
	}
	string[i+j] = '\0';
}

uint16 strToInt ( uint8 * str )
{
	uint8 i=0,j=0;
	uint16 num=0;
	for (i=0;str[i]!='\"';i++);

	uint16 weight =1;
	for (j=1;j<i;j++)
	{
		weight *=10;
	}

	for ( j=0;j<(i-1);j++)
	{
	num += (str[j]-48)*weight;
	weight /=10;
	}
	num+=(str[j]-48);

	return num;
}



 uint8 string[] ="id\":\"";
 uint16 lastId ;
 uint16 id =0;
 uint8  ServerData[12];
 uint16 ValveCounter =0;
 uint8 IrrigationFlag =0;

 enum APPState
 {
	 UpdateFromServer=0,
	 GetDataId,
	 CheckIfIDIsNew,
	 GetValveValue,
	 CheckValveValue,
	 GetIrrigationPeriod,
	 CheckIrrigationPeriod

 };
void APP_TASK()
{
	DIO_vidSetPinDir(8,1);
uint8 * buffer;
	static uint8 AppState =0;
	switch ( AppState )
	{

	case UpdateFromServer:
		if (  ESP8266_Example() )
			AppState=GetDataId;
	break;
	case GetDataId:
		ServerData[0]=0;
		buffer = searchFor( URxArray ,(uint8 *)"id\":\"",10);
		copyString(ServerData,buffer);
		AppState=CheckIfIDIsNew;
	break;
	case CheckIfIDIsNew:

		id = strToInt(ServerData);

		if ( id > lastId )
		{

			lastId = id;

			AppState=GetValveValue;
		}else
		{
//
			DIO_vidWritePin(10 ,1);
			AppState=UpdateFromServer;
		}
		break;
	case GetValveValue:
		DIO_vidSetPortDir(0,0xff);
		DIO_vidWritePort(0,lastId);
		ServerData[0]=0;
		buffer = searchFor(URxArray ,(uint8 *)"Valve\":\"",10);
		copyString( ServerData,buffer);
		AppState=CheckValveValue;
		break;
	case CheckValveValue:

		if( ServerData[1]=='N')
		{

			DIO_vidWritePin(8,1);
			AppState=GetIrrigationPeriod;
		}
		else
		{

			ValveCounter =0;
			DIO_vidWritePin(8,0);
			AppState=UpdateFromServer;
		}
		break;
	case GetIrrigationPeriod:
		ServerData[0]=0;
		buffer = searchFor(URxArray ,(uint8 *)"Period\":\"",10);
		copyString( ServerData, buffer);
		ValveCounter = strToInt(ServerData);
		counter=0;
		SCounter=0;
		AppState=UpdateFromServer;
		break;

	case 10:

		break;
	default :break;

		break;
	}

}

void IrrigationPeriodTask ()
{
	static uint8 periodTask =CheckIrrigationPeriod;
switch ( periodTask )
{
case CheckIrrigationPeriod:

	DIO_vidSetPortDir(2,0xff);
			DIO_vidWritePort(2,ValveCounter);
	if ( ValveCounter > 0)
	{

		DIO_vidWritePin(8,1);
		IrrigationFlag=1;
		if ( SCounter >ValveCounter)
		{
			IrrigationFlag=0;
			DIO_vidWritePin(8,0);
		}

	}else
	{
		IrrigationFlag=0;
		DIO_vidWritePin(8,0);
		periodTask= Wait;
	}
		break;
case Wait:
	if ( ValveCounter > 0)
		periodTask= CheckIrrigationPeriod;
	break;

}
}
