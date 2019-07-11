#ifndef _TIMER_INTERVALS_INCLUDED
#define _TIMER_INTERVALS_INCLUDED

//---------константы---------------
#define ADDR_TIMER_START        0x7000 //0x4000 = 24576 байт на динамически-генерируемый код
#define ADDR_TIMER_PAUSE_250ns  0xB000
#define ADDR_TIMER_PAUSE_1mcs   0xB100
#define ADDR_TIMER_PAUSE_10mcs  0xB200
#define ADDR_TIMER_PAUSE_100mcs 0xB300
#define ADDR_TIMER_PAUSE_1ms    0xB400
#define ADDR_TIMER_PAUSE_10ms   0xB500
#define ADDR_TIMER_PAUSE_100ms  0xB600
#define ADDR_TIMER_PAUSE_1s     0xB700
#define ADDR_TIMER_PAUSE_10s    0xB800
#define ADDR_TIMER_PAUSE_100s   0xB900

//----------функции----------------
void timer_pause_250ns(void);    
void timer_pause_1mcs(void); 
void timer_pause_10mcs(void);
void timer_pause_100mcs(void);
void timer_pause_1ms(void);
void timer_pause_10ms(void);
void timer_pause_100ms(void);
void timer_pause_1s(void);
void timer_pause_10s(void);
void timer_pause_100s(void);

#endif





