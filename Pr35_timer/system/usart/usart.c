#include "main\p33FJ128GP706.h"
#include "main\errors.h" 


//----------константы-------------- 
#define	USART_TIMEOUT 10000

//---------переменные--------------
unsigned int rs_overrun_error=0;
unsigned int rs_timeout_error=0;
unsigned int rs_maxlen_error=0;
unsigned int rs_ack_error=0;
unsigned int rs_good_count=0;

//----------функции----------------

//--------------------------основна€ часть-------------------------------------------

//комп--->пик формат пакета:   [1байт:размер=n(данные без ацк и без размера)] [nбайт:данные] [1байт:ACK(высчитываетс€_от_длина_данных_+_сами_данные)]  
//пик --->комп подтверждение:  есть ошибки: [0x01] [1байт:код ошибки]
//                             нет ошибок:  [0x00] [1байт: ACK]

//пик --->комп формат пакета:  тоже самое
//комп--->пик подтверждение:   нет


///////////////////////////////////
void UsartInit(void)
{
	//ножки
    TRISFbits.TRISF2=1;     //RX
	TRISFbits.TRISF3=0;     //TX
		
	//модуль usart
    U1MODEbits.BRGH=1;      //High Baud Rate Enable bit
    U1BRG=86;                //115200кб/сек= (40*1000*1000√ц)/(4*(86+1))  
    U1MODEbits.UEN=0;       //TX RX - используем, а вс€кие CTS RTS - нет    
    U1MODEbits.PDSEL=0;     //8bit четность выкл.
    U1MODEbits.STSEL=0;     //1 стоп бит
    U1MODEbits.URXINV=0;    //UxRX Idle state is С1Т
     
    //прерывани€
    U1STAbits.URXISEL=0;    //прерывание по каждому пришедшему байту
    U1STAbits.UTXISEL0=0;    //?
    U1STAbits.UTXISEL1=0;
    IPC2bits.U1RXIP=4;  //приоритет прерывани€ =4
    IPC3bits.U1TXIP=4;  //приоритет прерывани€ =4
    IPC16bits.U1EIP=4;  //приоритет прерывани€ =4
    IEC0bits.U1TXIE=0;  //прерывани€ на передачу запрещены
  	IEC0bits.U1RXIE=0;  //прерывани€ на прием запрещены
  	IEC4bits.U1EIE=0;   //прерывани€ по ошибке запрещены
  	
    U1MODEbits.UARTEN=1;    //UART1 is enabled
    U1STAbits.UTXEN=1;      //Transmit Enable

	IFS0bits.U1TXIF=0;  //на вс€кий случай сбрасываем флаг прерывани€      
    IFS0bits.U1RXIF=0;
    IFS4bits.U1EIF=0;
    
      	    
	return;
}

/////////////////////////////////////
void UsartTxByte(char data)
{
 	U1TXREG = data;     
}

///////////////////////////////////
char UsartRxByte(unsigned char * data)
//возвращает 1, в случае ошибки, 0 если все успешно
{
    if(U1STAbits.FERR)                 // If a framing error occured
    {
		*data = U1RXREG; 
		error_num=err_UsartRxFrameError;
		return(1);
	}
    if(U1STAbits.PERR)                 // If a framing error occured
    {
		*data = U1RXREG; 
		error_num=err_UsartRxParityError;
		return(1);
	}
    
    *data = U1RXREG; 
  	
	if(U1STAbits.OERR)
	{	
		U1STAbits.OERR=0;
		error_num=err_UsartRxOverrunError;
		return(1);   
	}

    return (0);                     // Return the received data
}


///////////////////////////////////
char UsartTxReadyVerify(void)
//если Tx зан€т возвращает 1, иначе 0
{
	if(U1STAbits.UTXBF==0) return(0); 
	return(1);   
}

///////////////////////////////////
char UsartRxReadyVerify(void)
//если буфер пуст возвращает 1, иначе 0
{
  if(U1STAbits.URXDA==1) return(0); // Data is available
  else return(1);  				    // Data not available
}

///////////////////////////////////
char UsartRxByte2(unsigned char * data)
//возвращает 1, в случае ошибки, 0 если все успешно
//ждет прежде чем прин€ть байт
//если байт прин€т с ошибкой, отправл€ет соответствующее уведомление
{
	unsigned int usart_count=0;
	
	while(UsartRxReadyVerify())
	{
		usart_count++;
		if(usart_count>=USART_TIMEOUT)
		{
			rs_timeout_error++;
			error_num=err_UsartRxTimeout;
			return(1);
		}	
	}
	
      
	if(UsartRxByte(data))
	{
		if((error_num==err_UsartRxOverrunError)||(error_num==err_UsartRxFrameError)||(error_num==err_UsartRxParityError))		
		{
			rs_overrun_error++;
			//высылаем в ответ код ошибки
			while(UsartTxReadyVerify());
			UsartTxByte(0x01);
			while(UsartTxReadyVerify());
			UsartTxByte(error_num);
			return(1);	
		}
	}
	
    return(0);  
}

///////////////////////////////////
char UsartTxPacket(unsigned char * buf, unsigned char len)	
{
	//возвращает 0 - если все успешно, 1 - в случае ошибки
	unsigned char i,ack;
	
	//вычисл€ем ack
	ack=len;
	for(i=0;i<len;i++) ack+=buf[i];
	//отправл€ем длину
	while(UsartTxReadyVerify());
	UsartTxByte(len);
	//отправл€ем данные
	for(i=0;i<len;i++)
	{
		while(UsartTxReadyVerify());
		UsartTxByte(buf[i]);
	}
	//отпрвл€ем ACK
	while(UsartTxReadyVerify());
	UsartTxByte(ack);
	return(0);
}

///////////////////////////////////
char UsartRxPacket(unsigned char * buf, unsigned char * len,unsigned char maxlen)	
{
	//возвращает 0 - если все успешно, 1 - в случае ошибки
	unsigned char size,i,ackin,ack;
	unsigned char buf_rx[255];

	if(UsartRxByte2(&size))	 return(1);
    if(size!=maxlen) //(size>maxlen)
	{
		rs_maxlen_error++;
		error_num=err_UsartRxBufSizeSmall;
		//высылаем в ответ код ошибки
		while(UsartTxReadyVerify());
		UsartTxByte(0x01);
		while(UsartTxReadyVerify());
		UsartTxByte(error_num);
		UsartRxByte(&size);	//считываем на случай если в буфере чтото есть
		UsartRxByte(&size);
		return(1);
	}
	
	ack=size;
	//принимаем сами данные
	for(i=0;i<size;i++)
	{
		if(UsartRxByte2(&buf_rx[i])) return(1);
		ack+=buf_rx[i];
	}
	//принимаем ack
	if(UsartRxByte2(&ackin)) return(1);
	if(ack==ackin)
	{
		//высылаем подтверждение 0x00 0xACK
		while(UsartTxReadyVerify());
		UsartTxByte(0x00);
		while(UsartTxReadyVerify());
		UsartTxByte(ack);
		*len=size;
		//копируем данные из buf_rx в buf
		for(i=0;i<size;i++) buf[i]=buf_rx[i];
		rs_good_count++;
		return(0);
	}
	else
	{
		rs_ack_error++;
		//высылаем код ошибки 0x01 0xerror_num
		error_num=err_UsartRxAckError;
		while(UsartTxReadyVerify());
		UsartTxByte(0x01);
		while(UsartTxReadyVerify());
		UsartTxByte(error_num);
		*len=0;
		return(1);
	}	
}


