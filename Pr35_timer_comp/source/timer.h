#ifndef _TIMER_INCLUDED
#define _TIMER_INCLUDED


//---------типы данных--------------
typedef union 
{
	struct 
    {
		//управление от кнопок
        unsigned short f_write_data      :1; //требутеся записать данные с компьютера
		unsigned short f_start			 :1; //переход в ждущий режим
    	unsigned short f_test_start		 :1; //старт таймера, без ждущего режима, в тестовых целях
	}; 
    unsigned short word;
}flags_control_struct;
    
typedef struct
{
	flags_control_struct   flags_control;   //команда от PC (чтение данных или запись)
    unsigned char t_global[16][8];			//абсолютные интервалы в нсек
    unsigned char  channel_onoff[2];        //каждому биту соответствует вкл\выкл канала PORTB
} data_struct;

//---------переменные--------------
extern data_struct   datain;
extern data_struct   dataout;

//----------функции----------------
void timer_init(void);

#endif





