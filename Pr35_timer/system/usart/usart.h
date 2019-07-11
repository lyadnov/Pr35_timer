#ifndef _USART_INCLUDED
#define _USART_INCLUDED

//---------����������--------------
extern unsigned int rs_overrun_error;
extern unsigned int rs_timeout_error;
extern unsigned int rs_maxlen_error;
extern unsigned int rs_ack_error;
extern unsigned int rs_good_count;

//----------�������----------------
void UsartInit(void);

void UsartTxByte(char data);			//���������� ���� �� ��������
char UsartRxByte(unsigned char* data);	//��������� 1 ����

char UsartTxReadyVerify(void);	//��������� 0 ����� �������� ���������
char UsartRxReadyVerify(void);	//��������� 0 ����� ���� ������

char UsartRxByte2(unsigned char * data); //���� ������ ��� ������� ����, �������� ��� ������, ���� ������ ����������

char UsartTxPacket(unsigned char*  buf, unsigned char len);
char UsartRxPacket(unsigned char* buf, unsigned char* len,unsigned char maxlen);	

#endif





