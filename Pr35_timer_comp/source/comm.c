#include <windows.h>
#include <stdio.h>

#define deb(x)

HANDLE hComm;
char debstr[0x200];

static void outhex(unsigned char *data,int len)
{
  int i;

  for(i=0;i<len;i++)
     printf("%02X ",data[i]);
  puts("");
}

HANDLE InitComm(char *port)
{
  COMMTIMEOUTS ct;
  DCB dcb;

  sprintf(debstr,"InitComm(%s)\n",port);
  OutputDebugString(debstr);
  hComm = CreateFile(port,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_ALWAYS,0,0);
  if (hComm==INVALID_HANDLE_VALUE)
  {
     OutputDebugString("InitComm() failed\n");
     return 0;
  }
//  SetupComm(hComm,0x10000,0x10000);

  ct.ReadIntervalTimeout = 1600; //1600msec
  ct.ReadTotalTimeoutMultiplier = 0; //0
  ct.ReadTotalTimeoutConstant = 100; //100
  ct.WriteTotalTimeoutMultiplier = 0;
  ct.WriteTotalTimeoutConstant = 8000;
  SetCommTimeouts(hComm,&ct);

  dcb.DCBlength = sizeof(DCB);
  dcb.BaudRate = 115200;            // current baud rate
  dcb.fBinary = TRUE;          // binary mode, no EOF check
  dcb.fParity = FALSE;          // enable parity checking
  dcb.fOutxCtsFlow = FALSE;      // CTS output flow control
  dcb.fOutxDsrFlow = FALSE;      // DSR output flow control
  dcb.fDtrControl = DTR_CONTROL_DISABLE;       // DTR flow control type
  dcb.fDsrSensitivity = FALSE;   // DSR sensitivity
  dcb.fTXContinueOnXoff = TRUE; // XOFF continues Tx
  dcb.fOutX = FALSE;            // XON/XOFF out flow control
  dcb.fInX = FALSE;             // XON/XOFF in flow control
  dcb.fErrorChar = FALSE;       // enable error replacement
  dcb.fNull = FALSE;            // enable null stripping
  dcb.fRtsControl = RTS_CONTROL_DISABLE;       // RTS flow control
  dcb.fAbortOnError = FALSE;     // abort reads/writes on error
  dcb.XonLim = 0;               // transmit XON threshold
  dcb.XoffLim = 0;              // transmit XOFF threshold
  dcb.ByteSize = 8;             // number of bits/byte, 4-8
  dcb.Parity = NOPARITY;               // 0-4=no,odd,even,mark,space
  dcb.StopBits = ONESTOPBIT;             // 0,1,2 = 1, 1.5, 2
  dcb.XonChar = 0;              // Tx and Rx XON character
  dcb.XoffChar = 0;             // Tx and Rx XOFF character
  dcb.ErrorChar = 0;            // error replacement character
  dcb.EofChar = 0;              // end of input character
  dcb.EvtChar = 0;              // received event character
  SetCommState(hComm,&dcb);
//  EscapeCommFunction(hComm,SETDTR);
//  EscapeCommFunction(hComm,DROPRTS);
  return hComm;
}

void SetBaudrate(int baud)
{
  DCB dcb;

//  sprintf(debstr,"SetBaudrate(%d)\n",baud);
//  OutputDebugString(debstr);
  GetCommState(hComm,&dcb);
  dcb.BaudRate = baud;
  SetCommState(hComm,&dcb);
//  EscapeCommFunction(hComm,SETDTR);
//  EscapeCommFunction(hComm,DROPRTS);
}

void SetReadTimeout(int mult,int tot)
{
  COMMTIMEOUTS ct;

  GetCommTimeouts(hComm,&ct);
  ct.ReadTotalTimeoutMultiplier = mult;
  ct.ReadTotalTimeoutConstant = tot;
  SetCommTimeouts(hComm,&ct);
}

void FlushRx()
{
  deb("FlushRx()\n");
  PurgeComm(hComm,PURGE_RXABORT | PURGE_RXCLEAR);
}

void FlushTx()
{
  deb("FlushTx()\n");
  PurgeComm(hComm,PURGE_TXCLEAR | PURGE_TXABORT);
}

void FlushComm()
{
  FlushRx();
  FlushTx();
}

void CloseComm()
{
  deb("CloseComm()\n");
  PurgeComm(hComm,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
  CloseHandle(hComm);
}

void WriteComm(unsigned char *buf,int len)
{
  DWORD actlen;

  WriteFile(hComm,buf,len,&actlen,NULL);
}

int ReadComm(unsigned char *buf,int len)
{
  DWORD actlen;

  ReadFile(hComm,buf,len,&actlen,NULL);
  return actlen;
}
