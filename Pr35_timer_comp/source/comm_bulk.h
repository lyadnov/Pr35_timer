#ifndef _COMM_BULK_INCLUDED
#define _COMM_BULK_INCLUDED


//---------���� ������--------------

//---------����������--------------
extern unsigned int packet_count;

//----------�������----------------
char CommTxPacket(unsigned char * buf, unsigned char len);
char CommRxPacket(unsigned char* buf,unsigned char* len,unsigned char maxlen);

#endif 