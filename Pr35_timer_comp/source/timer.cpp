#include "stdafx.h"
#include "comm.h"
#include "errors.h"
#include "timer.h"


//----------константы-------------- 

//---------переменные--------------
data_struct   datain;
data_struct   dataout;

//----------функции----------------


//--------------------------основная часть-------------------------------------------

///////////////////////////////////
void timer_init(void)
{
	unsigned char i;
    
	//переменные
	
    for(i=0;i<sizeof(data_struct);i++)
    {
        ((unsigned char*) &datain)[i]=0;
		((unsigned char*) &dataout)[i]=0;
    }
  
    return;
}

