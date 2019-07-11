#ifndef _ERRORS_INCLUDED
#define _ERRORS_INCLUDED

//----------���������--------------
#define	err_UnknownActiveMenu	0x01
#define	err_VariableTooLong		0x02

#define	err_UsartRxBufSizeSmall	0x03	//������ ������������ ������ ������������ �������				
#define	err_UsartRxAckError		0x04	//ACK ������������ ������ ������ ��������				
#define err_UsartRxFrameError	0x05
#define err_UsartRxParityError	0x06
#define err_UsartRxOverrunError	0x07
#define err_UsartRxTimeout		0x08	//������ �� USART ��� � �� ������

#define err_FlashOddAddress	    0x09    //������� ������ ��������� ������ ����
#define err_EepromBigAddress	0x0A    //����� ������������ �� ������ ������ ��� EEPROM_SIZE

#define err_PC_UsartRxConfirmationTimeout	0x80//PC: ������������� ����� TX �� ������			
#define err_PC_UsartRxTimeout				0x81//PC: ������ �� ��������
#define	err_PC_UsartRxBufSizeSmall			0x82//������ ������������ ������ ������������ �������				
#define	err_PC_UsartRxAckError				0x83//ACK ������������ ������ ������ ��������				

//---------����������--------------
extern unsigned char error_num;

//----------�������----------------

#endif






