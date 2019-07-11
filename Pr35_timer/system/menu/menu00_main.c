#include "p33FJ128GP706.h"

#include "system\menu\menu_proc.h"
#include "system\menu\menu03_contrast.h"
#include "system\menu\menu13_timer.h"
#include "system\menu\menu14_start.h"

//---------����������--------------
unsigned char flag_security_menu;

//----------�������----------------
void menu0_out(void);
void menu0_refresh(void);
void menu0_key_pressed(void);

//--------------------------�������� �����-------------------------------------------

///////////////////////////////////
/*rom near*/ const unsigned char menu_0[]=
//	'--------------------'
//  '     ����������     '
//  '      ��������      '
//  '                    '
//  '��������� ����������'

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//                �  �  �  �  �  �  �  �  �  �  
 {__,__,__,__,__,RU,RP,xP,_A,_B,RL,_E,_H,RI,_E,__,__,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//                   �  �  �  �  �  �  �  � 
  __,__,__,__,__,__,_T,_A,RJ,_M,_E,xP,_O,_M,__,__,__,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//  
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
// �  �  �  �  �  �  �  �  �     �  �  �  �  �  �  �  �  �  �                                            
  _H,_A,xC,_T,xP,_O,RJ,_K,_A,__,RP,_O,RD,RG,_O,_T,_O,_B,_K,_A}; 

/*
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//                         �  �  �  �  �  �  �  �  �  �  
 {__,__,__,__,__,__,__,__,RU,RP,xP,_A,_B,RL,_E,_H,RI,_E,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//                         �  �  �  �  �  �  �  � 
  __,__,__,__,__,__,__,__,_T,_A,RJ,_M,_E,xP,_O,_M,__,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//  
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
// �  �  �  �  �  �  �  �  �     �  �  �  �  �  �  �  �  �  �                                            
  _H,_A,xC,_T,xP,_O,RJ,_K,_A,__,RP,_O,RD,RG,_O,_T,_O,_B,_K,_A}; 

*/
/*
const unsigned char menu_0_pic[]=
{
    0x80, 0x40, 0xC0, 0x40, 0xC0, 0x20, 0xE0, 0xF0, 0x70, 0x18, 0x0C, 0x0C, 0x06, 0x07, 0xC6, 0x07, 
    0x06, 0x0C, 0x0C, 0x18, 0x30, 0x70, 0xE0, 0x20, 0xC0, 0x40, 0xC0, 0xC0, 0x80, 0x3F, 0x60, 0x7F, 
    0x60, 0x7F, 0x80, 0xFF, 0xFF, 0xC0, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07, 0x08, 0x10, 0x20, 0x00, 
    0x00, 0x00, 0xC0, 0xFF, 0x80, 0x7F, 0x71, 0x7F, 0x71, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x01, 0x03, 0x06, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x02, 0x01, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/

void menu0_out(void)
{
	//�������������� ���������� ����
	menu_body_out(menu_0);
	//menu_picture_out(menu_0_pic,16,0,29,24);

	active_menu=m0;
	flags_menu.f_refresh=0;
    flags_menu.f_autochange=0;
	flag_security_menu=0;
	//�������������� ���������� ����
 	//������� ����������
 	menu0_refresh();
    return;
}


///////////////////////////////////
void menu0_refresh(void)
{
	
	return;
}

///////////////////////////////////
void menu0_key_pressed(void)
{
	switch(button_status)
    {
		
		case (0b00001000):	//����� ������� ������
			break;	
		case (0b00000100):	//����� ������ ������
			if(flag_security_menu==0) flag_security_menu=1;
			break;	
		case (0b00000010):	//������ ������� ������
			break;	
		case (0b00000001):	//������ ������ ������
			menu14_out();
			break;
		
		//��� ������� �������
		case (0b00001100):	
			if(flag_security_menu==1) flag_security_menu=2;
			else flag_security_menu=5;		 
			break;
		case (0b00000110):	
			if(flag_security_menu==2) flag_security_menu=3; 
			else flag_security_menu=5;
			break;
		case (0b00000101):	
			if(flag_security_menu==3) flag_security_menu=4; 
			else flag_security_menu=5;
			break;
		case (0b00000000):	
			if(flag_security_menu==1) menu13_out();
			if(flag_security_menu==4) menu3_out();
			flag_security_menu=0;
			break;		
		default:
			break;
	}
	return;
}

