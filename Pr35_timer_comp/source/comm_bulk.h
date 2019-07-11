#ifndef _COMM_BULK_INCLUDED
#define _COMM_BULK_INCLUDED


//---------типы данных--------------

//---------переменные--------------
extern unsigned int packet_count;

//----------функции----------------
char CommTxPacket(unsigned char * buf, unsigned char len);
char CommRxPacket(unsigned char* buf,unsigned char* len,unsigned char maxlen);

#endif 