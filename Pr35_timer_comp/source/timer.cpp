#include "stdafx.h"
#include "comm.h"
#include "errors.h"
#include "timer.h"


//----------���������-------------- 

//---------����������--------------
data_struct   datain;
data_struct   dataout;

//----------�������----------------


//--------------------------�������� �����-------------------------------------------

///////////////////////////////////
void timer_init(void)
{
	unsigned char i;
    
	//����������
	
    for(i=0;i<sizeof(data_struct);i++)
    {
        ((unsigned char*) &datain)[i]=0;
		((unsigned char*) &dataout)[i]=0;
    }
  
    return;
}

