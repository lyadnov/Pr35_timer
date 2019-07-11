#include "main\p33FJ128GP706.h"
#include "main\errors.h" 


//----------���������-------------- 
#define	USART_TIMEOUT 10000

//---------����������--------------
unsigned int rs_overrun_error=0;
unsigned int rs_timeout_error=0;
unsigned int rs_maxlen_error=0;
unsigned int rs_ack_error=0;
unsigned int rs_good_count=0;

//----------�������----------------

//--------------------------�������� �����-------------------------------------------

//����--->��� ������ ������:   [1����:������=n(������ ��� ��� � ��� �������)] [n����:������] [1����:ACK(�������������_��_�����_������_+_����_������)]  
//��� --->���� �������������:  ���� ������: [0x01] [1����:��� ������]
//                             ��� ������:  [0x00] [1����: ACK]

//��� --->���� ������ ������:  ���� �����
//����--->��� �������������:   ���


///////////////////////////////////
void UsartInit(void)
{
	//�����
    TRISFbits.TRISF2=1;     //RX
	TRISFbits.TRISF3=0;     //TX
		
	//������ usart
    U1MODEbits.BRGH=1;      //High Baud Rate Enable bit
    U1BRG=86;                //115200��/���= (40*1000*1000��)/(4*(86+1))  
    U1MODEbits.UEN=0;       //TX RX - ����������, � ������ CTS RTS - ���    
    U1MODEbits.PDSEL=0;     //8bit �������� ����.
    U1MODEbits.STSEL=0;     //1 ���� ���
    U1MODEbits.URXINV=0;    //UxRX Idle state is �1�
     
    //����������
    U1STAbits.URXISEL=0;    //���������� �� ������� ���������� �����
    U1STAbits.UTXISEL0=0;    //?
    U1STAbits.UTXISEL1=0;
    IPC2bits.U1RXIP=4;  //��������� ���������� =4
    IPC3bits.U1TXIP=4;  //��������� ���������� =4
    IPC16bits.U1EIP=4;  //��������� ���������� =4
    IEC0bits.U1TXIE=0;  //���������� �� �������� ���������
  	IEC0bits.U1RXIE=0;  //���������� �� ����� ���������
  	IEC4bits.U1EIE=0;   //���������� �� ������ ���������
  	
    U1MODEbits.UARTEN=1;    //UART1 is enabled
    U1STAbits.UTXEN=1;      //Transmit Enable

	IFS0bits.U1TXIF=0;  //�� ������ ������ ���������� ���� ����������      
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
//���������� 1, � ������ ������, 0 ���� ��� �������
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
//���� Tx ����� ���������� 1, ����� 0
{
	if(U1STAbits.UTXBF==0) return(0); 
	return(1);   
}

///////////////////////////////////
char UsartRxReadyVerify(void)
//���� ����� ���� ���������� 1, ����� 0
{
  if(U1STAbits.URXDA==1) return(0); // Data is available
  else return(1);  				    // Data not available
}

///////////////////////////////////
char UsartRxByte2(unsigned char * data)
//���������� 1, � ������ ������, 0 ���� ��� �������
//���� ������ ��� ������� ����
//���� ���� ������ � �������, ���������� ��������������� �����������
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
			//�������� � ����� ��� ������
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
	//���������� 0 - ���� ��� �������, 1 - � ������ ������
	unsigned char i,ack;
	
	//��������� ack
	ack=len;
	for(i=0;i<len;i++) ack+=buf[i];
	//���������� �����
	while(UsartTxReadyVerify());
	UsartTxByte(len);
	//���������� ������
	for(i=0;i<len;i++)
	{
		while(UsartTxReadyVerify());
		UsartTxByte(buf[i]);
	}
	//��������� ACK
	while(UsartTxReadyVerify());
	UsartTxByte(ack);
	return(0);
}

///////////////////////////////////
char UsartRxPacket(unsigned char * buf, unsigned char * len,unsigned char maxlen)	
{
	//���������� 0 - ���� ��� �������, 1 - � ������ ������
	unsigned char size,i,ackin,ack;
	unsigned char buf_rx[255];

	if(UsartRxByte2(&size))	 return(1);
    if(size!=maxlen) //(size>maxlen)
	{
		rs_maxlen_error++;
		error_num=err_UsartRxBufSizeSmall;
		//�������� � ����� ��� ������
		while(UsartTxReadyVerify());
		UsartTxByte(0x01);
		while(UsartTxReadyVerify());
		UsartTxByte(error_num);
		UsartRxByte(&size);	//��������� �� ������ ���� � ������ ����� ����
		UsartRxByte(&size);
		return(1);
	}
	
	ack=size;
	//��������� ���� ������
	for(i=0;i<size;i++)
	{
		if(UsartRxByte2(&buf_rx[i])) return(1);
		ack+=buf_rx[i];
	}
	//��������� ack
	if(UsartRxByte2(&ackin)) return(1);
	if(ack==ackin)
	{
		//�������� ������������� 0x00 0xACK
		while(UsartTxReadyVerify());
		UsartTxByte(0x00);
		while(UsartTxReadyVerify());
		UsartTxByte(ack);
		*len=size;
		//�������� ������ �� buf_rx � buf
		for(i=0;i<size;i++) buf[i]=buf_rx[i];
		rs_good_count++;
		return(0);
	}
	else
	{
		rs_ack_error++;
		//�������� ��� ������ 0x01 0xerror_num
		error_num=err_UsartRxAckError;
		while(UsartTxReadyVerify());
		UsartTxByte(0x01);
		while(UsartTxReadyVerify());
		UsartTxByte(error_num);
		*len=0;
		return(1);
	}	
}


