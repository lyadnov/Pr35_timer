#ifndef _COMM_H_INCLUDED
#define _COMM_H_INCLUDED

extern "C"
{

int InitComm(char *);
void SetBaudrate(int);
void SetReadTimeout(int,int);
void WriteComm(unsigned char *,int);
int ReadComm(unsigned char *,int);
void FlushRx();
void FlushTx();
void FlushComm();
void CloseComm();
};

#endif
