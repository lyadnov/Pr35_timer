#ifndef _TIMER_INCLUDED
#define _TIMER_INCLUDED

//---------константы---------------

//---------переменные--------------
extern unsigned long long t_global[16];
extern unsigned short  channel_onoff;


//----------функции----------------
void timer_init(void);
void timer_eeprom_write_default_values(void);

unsigned char timer_verify_if_new_data(unsigned long long * t_temp,unsigned short * channel_onoff_temp);
void timer_usart_control(void);

#endif





