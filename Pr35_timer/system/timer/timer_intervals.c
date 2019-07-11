#include "p33FJ128GP706.h"
#include "main\errors.h" 

#include "system\timer\timer.h" 
#include "system\timer\timer_intervals.h" 
#include "system\flash\flash.h"
#include "system\eeprom\eeprom.h"

//----------константы--------------
	
//---------переменные--------------

//в этом месте (ADDR_TIMER_START) на лету генерируетс€ функци€, осуществл€юща€ задержку по 16ти каналам
unsigned char timer_start[ADDR_TIMER_PAUSE_250ns - ADDR_TIMER_START] __attribute__ ((space(prog),address(ADDR_TIMER_START)));


//----------функции----------------

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_250ns))) timer_pause_250ns(void) // функции 1мс,10мс и тд
{
            //rcall 2
            //ink   1
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
            //unk   1
    return; //      3 хот€ отладчик говорит что 4
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_1mcs))) timer_pause_1mcs(void) 
{
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_10mcs))) timer_pause_10mcs(void) 
{
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; //
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_100mcs))) timer_pause_100mcs(void) 
{
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
        
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_1ms))) timer_pause_1ms(void) 
{
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
        
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
                                
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_10ms))) timer_pause_10ms(void) 
{
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
                            
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
        
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
        
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_100ms))) timer_pause_100ms(void) 
{
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
                            
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
        
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
        
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_1s))) timer_pause_1s(void) 
{
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
                            
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
        
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
        
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_10s))) timer_pause_10s(void) 
{
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
                            
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
        
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
        
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}

///////////////////////////////////
void __attribute__((address(ADDR_TIMER_PAUSE_100s))) timer_pause_100s(void) 
{
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    timer_pause_10s();      //400000000
    
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    timer_pause_1s();       //40000000
    
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    timer_pause_100ms();    //4000000
    
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    timer_pause_10ms();     //400000
    
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
    timer_pause_1ms();      //40000
                            
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
    timer_pause_100mcs();   //4000
        
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
    timer_pause_10mcs();    //400
        
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    timer_pause_1mcs();     //40
    
    timer_pause_250ns();    //10
    timer_pause_250ns();    //10    
    timer_pause_250ns();    //10
    
    Nop();  //      1
    Nop();  //      1
    Nop();  //      1
    return; 
}
