#include "p33FJ128GP706.h"

#include "system\eeprom\eeprom.h"
#include "system\menu\menu_proc.h"
#include "system\menu\menu00_main.h"
#include "system\timer\timer.h"

//---------переменные--------------

//----------функции----------------
void menu14_out(void);
void menu14_refresh(void);
void menu14_key_pressed(void);
void menu14_key_unpressed(void);
void timer_start(void);

//--------------------------основная часть-------------------------------------------

///////////////////////////////////
/*rom near*/ const unsigned char menu_14[]=
//	'--------------------'
//  '    xx ОЖИДАНИЕ     '
//  '    xx ЗАПУСКА...   '
//	'                    '
//  'РУЧНОЙ ПУСК    СБРОС'

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
//                      О  Ж  И  Д  А  Н  И  Е      
 {__,__,__,__,__,__,__,_O,ZH,RI,RD,_A,_H,RI,_E,__,__,__,__,__,

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
//                      З  А  П  У  С  К  А  .  .  .  
  __,__,__,__,__,__,__,RZ,_A,RP,RU,xC,_K,_A,zz,zz,zz,__,__,__,
     
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
//  
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,     
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
// Р  У  Ч  Н  О  Й     П  У  С  К              С  Б  Р  О  С 
  xP,RU,CH,_H,_O,RJ,__,RP,RU,xC,_K,__,__,__,__,xC,RB,xP,_O,xC}; 

const unsigned char menu_14_pic[]=
{
    0x00, 0x07, 0x3D, 0x65, 0xD5, 0xA5, 0xD5, 0x65, 0x3D, 0x07, 0x00, 0x00, 0xE0, 0xBC, 0xA2, 0xAB, 
    0xA5, 0xAB, 0xA2, 0xBC, 0xE0, 0x00, 
};

void menu14_out(void)
{
	//инициаллизация параметров меню
	menu_body_out(menu_14);
	menu_picture_out((unsigned char *)menu_14_pic,28,0,11,16);
	
	active_menu=m14;
	flags_menu.f_refresh=0;
    flags_menu.f_autochange=0;
	//инициаллизация переменных menu0
	//выводим переменные
	menu14_refresh();
	return;
}

///////////////////////////////////
void menu14_refresh(void)
{
	return;
}

///////////////////////////////////
void menu14_key_pressed(void)
{
	switch(button_status)
    {
		case (0b00000100):	//левая нижняя кнопка
			//menu0_out();
			break;	
		case (0b00000000):	//все кнопки отпущены
			menu14_key_unpressed();
			break;
		default:
			break;
	}

	return;
}


///////////////////////////////////
void menu14_key_unpressed(void)
{
	unsigned short count;
	
	//все кнопки отпущены
	SRbits.IPL=5;
	timer_start();
	
	//костыль №2 - по возвращении после ручного запуска, все кнопки должны быть отпущены
	while(count<10000)
	{
    	//проверяем что все кнопки отпущены
    	if( (~PORTG&0x03C0) !=0)  
    	{
            //есть нажатые
            count=0;
        }
    	else
    	{
            //все кнопки отпущены. ждем некоторое время
            count++;
        }
	}
	IFS1bits.CNIF=0;
		
	SRbits.IPL=4;
	menu0_out();
	return;
}
