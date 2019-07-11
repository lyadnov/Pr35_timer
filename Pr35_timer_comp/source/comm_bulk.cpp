#include "stdafx.h"
#include "comm.h"
#include "errors.h"


//----------константы-------------- 
#define USART_TIMEOUT 2

//---------переменные--------------
unsigned int packet_count=0;

//----------функции----------------



//--------------------------основная часть-------------------------------------------

//комп--->пик формат пакета:   [1байт:размер=n(данные без ацк и без размера)] [nбайт:данные] [1байт:ACK(высчитывается_от_длина_данных_+_сами_данные)]  
//пик --->комп подтверждение:  есть ошибки: [0x01] [1байт:код ошибки]
//                             нет ошибок:  [0x00] [1байт: ACK]

//пик --->комп формат пакета:  тоже самое
//комп--->пик подтверждение:   нет


///////////////////////////////////
char CommTxPacket(unsigned char * buf, unsigned char len)	
{
	//возвращает 0 - если все успешно, 1 - в случае ошибки
	unsigned char i;
	unsigned char buftx[256+2];
	unsigned char bufrx[2];
	unsigned int usart_count=0;

	buftx[0]=len;
	for(i=0;i<len;i++) buftx[1+i]=buf[i];
	//считаем ACK
	buftx[len+1]=len;
	for(i=0;i<len;i++) buftx[len+1]+=buftx[1+i];

	WriteComm(buftx,len+2);
	do
	{
		len=ReadComm(bufrx,2);
		usart_count++;
		if(usart_count>=USART_TIMEOUT)
		{
			error_num=err_PC_UsartRxConfirmationTimeout;
			return(1);
		}		
	} while(len==0);
	if(bufrx[0]!=0)
	{
		error_num=bufrx[1];
		return(1);		
	}
	return(0);
}

///////////////////////////////////
char CommRxPacket(unsigned char* buf,unsigned char* len,unsigned char maxlen)
{
	int i;
	unsigned char size,length;
	unsigned char bufrx[256+2];
	unsigned int usart_count=0;	
	unsigned char ackin,ack=0;

	//принимаем длину
	do
	{
		size=ReadComm(&length,1);
		usart_count++;
		if(usart_count>=USART_TIMEOUT)
		{
			error_num=err_PC_UsartRxTimeout;
			return(1);
		}	

	} while(size==0);
	
	//проверяем длину
	if(length>maxlen)
	{
		error_num=err_PC_UsartRxBufSizeSmall;
		return(1);
	}

	ack=length;

	usart_count=0;
	if(length!=0)
	{
		//принимаем данные
		do
		{
			size=ReadComm(bufrx,length);
			usart_count++;
			if(usart_count>=USART_TIMEOUT)
			{
				error_num=err_PC_UsartRxTimeout;
				return(1);
			}	
		} while(size==0);
	}

	
	for(i=0;i<length;i++)
	{
		ack+=bufrx[i];
	}
	
	//принимаем ACK
	usart_count=0;
	do
	{
		size=ReadComm(&ackin,1);
		usart_count++;
		if(usart_count>=USART_TIMEOUT)
		{
			error_num=err_PC_UsartRxTimeout;
			return(1);
		}	
	} while(size==0);
	
	//вычисляем ACK
	if(ackin!=ack)
	{
		error_num=err_PC_UsartRxAckError;
		*len=0;
        return(1);
	}
    else
    {
        for(i=0;i<length;i++)
	    {
		    buf[i]=bufrx[i];
	    }
        *len=length;
    }
	return(0);
}