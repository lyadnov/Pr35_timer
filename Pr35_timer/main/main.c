#include "p33FJ128GP706.h"

#include "main\errors.h" 
#include "system\flash\flash.h"
#include "system\eeprom\eeprom.h"
#include "system\lcd_graph\lcd_graph_low.h"
#include "system\menu\menu00_main.h"
#include "system\menu\menu_proc.h"
#include "system\timer\timer.h"
#include "system\usart\usart.h"

//#define   CLOCK_2MHZ

/* // 7,37МГц FRC(встроенных) без PLL
_FOSC(POSCMD_NONE)
_FOSCSEL(FNOSC_FRC & IESO_OFF)
*/

/* //20МГц HS без PLL
_FOSC(OSCIOFNC_OFF & POSCMD_HS & FCKSM_CSDCMD)  //OSC2 is clock O/P, HS oscillator,Clock switching and clock monitor:disabled
_FOSCSEL(FNOSC_PRI & IESO_OFF)  //,Two-speed Oscillator Startup:disabled
*/

//// 80Мгц из FRC + PLL
//_FOSC(OSCIOFNC_OFF & POSCMD_NONE & FCKSM_CSECMD)    //OSC2 is clock O/P, Primary oscillator disabled,Clock switching: enabled clock monitor:disabled
//_FOSCSEL(FNOSC_FRCPLL & IESO_OFF)                   //Fast RC oscillator w/ divide and PLL,Two-speed Oscillator Startup:disabled

#ifdef CLOCK_2MHZ
     //2МГЦ c PLL
    _FOSC(OSCIOFNC_OFF & POSCMD_HS & FCKSM_CSECMD)  //OSC2 is clock O/P, HS oscillator,Clock switching and clock monitor:disabled
	_FOSCSEL(FNOSC_PRIPLL & IESO_OFF)               //Two-speed Oscillator Startup:disabled
#else
    //внешняя частота без PLL
    _FOSC(OSCIOFNC_OFF & POSCMD_EC & FCKSM_CSECMD)  //OSC2 is clock O/P, External clock,Clock switching and clock monitor:disabled
    _FOSCSEL(FNOSC_PRI & IESO_OFF)                  //Two-speed Oscillator Startup:disabled
#endif



_FWDT(FWDTEN_OFF)               //Watchdog Timer: disabled       
_FPOR(FPWRT_PWR64)              // Power-on Reset Value: 64ms

//---------константы---------------
#define DEBUG 0
#define EEPROM_VERSION  0x09  //для записи новых значений параметров в EEPROM

//---------переменные--------------

//----------функции----------------
void Init(void);
void eeprom_write_default_values(void);


//--------------------------основная часть-------------------------------------------

///////////////////////////////////
int main(void)
{
    Init();
  
	while(1)
	{
        timer_usart_control();
    }   	
}

///////////////////////////////////
void Init(void)
{
    unsigned char eeprom_ver;
	unsigned char i;
	
	//-------------сброс периферии-----------------------	
//    //конфигурируем Fosc=((7,37мгц/PLLPRE)*PLLDIV)/PLLPOST=80МГц
//    OSCCONbits.CLKLOCK=0;   //Clock and PLL selections are not locked, configurations may be modified
//    OSCCONbits.NOSC=1;      //Fast RC oscillator (FRC) with PLL
//    CLKDIVbits.PLLPRE=0;    //PLLPRE=2;
//    CLKDIVbits.PLLPOST=0;   //PLLPOST=2;   
//    PLLFBDbits.PLLDIV=42; //PLLDIV=44 => 80,4МГЦ; PLLDIV=41 => 79,5 МГЦ; вообще работало при PLLDIV=60 => 115МГц, дальше на разгонял   
//    OSCCONbits.OSWEN=1;     //initiate the oscillator switch
//    while(OSCCONbits.OSWEN==1);

#ifdef CLOCK_2MHZ
    //конфигурируем Fosc=((2мгц/PLLPRE)*PLLDIV)/PLLPOST=80МГц
    OSCCONbits.CLKLOCK=0;   //Clock and PLL selections are not locked, configurations may be modified
    OSCCONbits.NOSC=3;      //Primary oscillator (XT, HS, EC) with PLL 
    CLKDIVbits.PLLPRE=0;    //PLLPRE=2;
    CLKDIVbits.PLLPOST=0;   //PLLPOST=2;   
    PLLFBDbits.PLLDIV= 158; //правильное значение 158=>80Мгц; 198=>100МГц; 210 работало, 220 уже нет.
    OSCCONbits.OSWEN=1;     //initiate the oscillator switch
    while(OSCCONbits.OSWEN==1);
#endif   
          
    //АЦП1
  	AD1CON1=0;
  	AD1CON2=0;
  	AD1CON3=0;
  	AD1CON4=0;
  	AD1CSSL=0;
  	AD1CSSH=0;
  	AD1PCFGL=0xFFFF;
  	AD1PCFGH=0xFFFF;    //все ножки цифровые входы\выходы  	
  	//АЦП2
  	AD2CON1=0;
  	AD2CON2=0;
  	AD2CON3=0;
  	AD2CON4=0;
  	AD2CSSL=0;
  	AD2PCFGL=0xFFFF;     //все ножки цифровые входы\выходы  	    
  	//CAN
  	C1CTRL1bits.REQOP=1; //disable mode
  	C2CTRL1bits.REQOP=1; //disable mode
  	  	  	    
    //инициаллизация портов
  	TRISB=0;	
  	PORTB=0;
  	TRISC=0;
  	PORTC=0;  
  	TRISD=0;	
  	PORTD=0;
  	TRISG=0;	
  	PORTG=0;
  	TRISF=0;	
  	PORTF=0;
  	
   	//инициаллизируем прерывания
  	INTCON1=0;
  	INTCON1bits.NSTDIS=0;   //вызов одного прерывания из под другого (interrupt nesting) разрешен
  	INTCON2=0;
  	INTCON2bits.ALTIVT=0;   //Use standard (default) vector table
  	
  	///-------------инициаллизация переменных в EEPROM-------------- 
	eeprom_ver=eeprom_read(ADDR_EEPROM_VERSION); 	//проверка EEPROM на наличие правильных записанных в него данных	
	if(eeprom_ver!=EEPROM_VERSION) eeprom_write_default_values();

	
	//-------------инициаллизация периферии-----------------------  
  	
  	//-----таймер TMR1 (защита от дребезга ЖК, и др функции) 
  	T1CON=0;
  	PR1=0xFFFF;
  	T1CONbits.TCS=0;    //Timer1 Clock Source Select bit: Internal clock (Fcy=40MHz)
  	T1CONbits.TCKPS=1;  //1:8 13мс период 
  	T1CONbits.TON=1;    //включаем таймер
  	IPC0bits.T1IP=4;    //приоритет прерывания =4
    IFS0bits.T1IF=0;    //на всякий случай сбрасываем флаг прерывания	
  	IEC0bits.T1IE=1;    //разрешаем прерывания от таймера1
 	
	
	//-----ЖКИ-графический-------
  	graph_init_pic();
	graph_init_external_controller();
	menu_init();
	menu0_out();
    
    //----USART-------	
	UsartInit();
  	
	//----таймер----
	timer_init();

  	//-------------инициаллизация прерываний----------------
	return;
}

///////////////////////////////////
void eeprom_write_default_values()
{
	eeprom_write(ADDR_EEPROM_VERSION,EEPROM_VERSION);   //версия еепром
	graph_eeprom_write_default_values();                  //жки
	timer_eeprom_write_default_values();
	return;
}


