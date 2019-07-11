#ifndef _USART_INCLUDED
#define _USART_INCLUDED

//---------переменные--------------
extern unsigned int rs_overrun_error;
extern unsigned int rs_timeout_error;
extern unsigned int rs_maxlen_error;
extern unsigned int rs_ack_error;
extern unsigned int rs_good_count;

//----------функции----------------
void UsartInit(void);

void UsartTxByte(char data);			//записывает байт на передачу
char UsartRxByte(unsigned char* data);	//считывает 1 байт

char UsartTxReadyVerify(void);	//возращает 0 когда передача завершена
char UsartRxReadyVerify(void);	//возращает 0 когда есть данные

char UsartRxByte2(unsigned char * data); //ждет прежде чем принять байт, высылает код ошибки, если ошибка происходит

char UsartTxPacket(unsigned char*  buf, unsigned char len);
char UsartRxPacket(unsigned char* buf, unsigned char* len,unsigned char maxlen);	

#endif





