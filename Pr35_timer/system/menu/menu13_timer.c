#include "p33FJ128GP706.h"

#include "system\eeprom\eeprom.h"
#include "system\timer\timer.h"
#include "system\menu\menu_proc.h"
#include "system\menu\menu00_main.h"

//----------���������--------------
#define	CURSOR_ON_TIME 	3
#define	CURSOR_OFF_TIME	6

//---------����������--------------
unsigned short channel_onoff_temp;     
unsigned long long t_menu_temp[16];     //��� ��
unsigned char k=0;                      //����� �������� ������
unsigned int min,sec,ms,mks,ns;

//��� �������
typedef union 
{
    struct                                  //���������� ��������
    {
        unsigned char f_cursor_on        :1; 
    }; 
    unsigned char byte;
} flags_cursor_struct;

typedef struct  
{
	flags_cursor_struct flags;
    unsigned char position;  //������� ������� �������
    unsigned char count;     //�������, ��������� ��������� �������    
} cursor_struct;

cursor_struct timer_cursor;
	

////----------�������----------------
void menu13_out(void);
void menu13_refresh(void);
void menu13_key_pressed(void);
	void menu13_select(void);
	void menu13_plus(void);
	void menu13_minus(void);
    void menu13_save(void);
void menu13_intervals_calculate(void);

void menu13_2_out(void);
void menu13_2_refresh(void);
void menu13_2_key_pressed(void);
    void menu13_2_yes(void);

//--------------------------�������� �����-------------------------------------------

///////////////////////////////////
/*rom near*/ const unsigned char menu_13[]=

//	'--------------------'
//  'M     ����� 01     >'
//  '    ����.   00���   '
//  '    00���   000��   '
//  '-   000���  000��  +'
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
// M                 �  �  �  �  �     0  1                 >
 {_M,__,__,__,__,__,_K,_A,_H,_A,RL,__,__,__,__,__,__,__,__,z0,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
//             �  �  �  �  .           0  0  �  �  �           
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,rm,ri,rn,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
// -           0  0  c  �  �           0  0  0  �  �                                    
  __,__,__,__,__,__,_c,_e,rk,__,__,__,__,__,__,rm,_c,__,__,__, 
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
// -           0  0  0  �  �  �        0  0  0  �  �        +                                   
  zm,__,__,__,__,__,__,rm,rk,_c,__,__,__,__,__,rn,_c,__,__,zp}; 


void menu13_out(void)
{
	unsigned int i;
	
	//�������������� ���������� ����
	menu_body_out(menu_13);
	active_menu=m13;
	flags_menu.f_refresh=1;
    flags_menu.f_autochange=1;
    autochange_rate=AUTOCHANGE_RATE_FAST;
    //������
    timer_cursor.position=0;
	timer_cursor.count=0;
	timer_cursor.flags.byte=0;
	//�������������� ���������� menu0
    k=0;
    channel_onoff_temp=channel_onoff;
    for(i=0;i<16;i++) t_menu_temp[i]=t_global[i];
   
    menu13_intervals_calculate();
      
    //������� ����������
	menu13_refresh();
	return;
}

///////////////////////////////////
void menu13_intervals_calculate(void)
{
    unsigned long long temp_array[3];   //���������� �� ����� ��������� ���������� ���� unsigned long long, � ������� �����.
    #define x   temp_array[0]
    #define n   temp_array[1]
    #define ost temp_array[2]    
    
    x=t_menu_temp[k];       
    //������
    n=x/(60000000);      //n=x/60000000000 ����� �� 2 ��������, ������ ��� ����� ����������� �������� �� ������ ��������� 60000000000
    n=n/1000;
    ost=x-(n*60000000*1000);        //x%100000000000;
    x=ost;
    min=n;
    //�������
    n=x/1000000000;
    ost=x%1000000000;
    x=ost;
    sec=n;
    //������������
    n=x/1000000;
    ost=x%1000000;
    x=ost;
    ms=n;
    //������������
    n=x/1000;
    ost=x%1000;
    x=ost;
    mks=n;
    //�����������
    ns=x;
    
    return;
}

///////////////////////////////////
void menu13_refresh(void)
{
	unsigned char cursor_string[5]={zb,zb,zb,zb,zb};
    unsigned char on[5] ={_B,_K,RL,zz,__};	
    unsigned char off[5]={_B,YY,_K,RL,zz};	
    	
	//������� ����������
    menu_variable_out(0,12,k+1,2);
    
    if(channel_onoff_temp&(1<<k))  
        menu_string_out(1,4,on,5);
    else
        menu_string_out(1,4,off,5);
      
    menu_variable_out(1,12,min,2);
    menu_variable_out(2,4,sec,2);
    menu_variable_out(2,12,ms,3);
    menu_variable_out(3,4,mks,3);
    menu_variable_out(3,12,ns,3);
    

    //������� ������
	timer_cursor.count++;
	if(timer_cursor.flags.f_cursor_on==1)
	{
		//������ ������ ���������
		if(timer_cursor.count>=CURSOR_ON_TIME)
		{
			timer_cursor.count=0;
			timer_cursor.flags.f_cursor_on=0;
		}
		switch(timer_cursor.position)
	    {
			case (0):
				menu_string_out(0,12,cursor_string,2);
				break;
			case (1):
				menu_string_out(1,4,cursor_string,5);
				break;
			case (2):
				menu_string_out(1,12,cursor_string,2);
				break;
            case (3):
				menu_string_out(2,4,cursor_string,2);
				break;
	        case (4):
				menu_string_out(2,12,cursor_string,3);
				break;
			case (5):
				menu_string_out(3,4,cursor_string,3);
				break;
			case (6):
				menu_string_out(3,12,cursor_string,3);
				break;
		}
	}
	else
	{
		//������� �� ������ ���������
		if(timer_cursor.count>=CURSOR_OFF_TIME)
		{
			timer_cursor.count=0;
			timer_cursor.flags.f_cursor_on=1;
		}

	}
	
	return;
}

///////////////////////////////////
void menu13_key_pressed(void)
{
	switch(button_status)
    {
		case (0b00001000):	//����� ������� ������
			menu13_save();
			break;	
		case (0b00000100):	//����� ������ ������
			menu13_minus();
			break;	
		case (0b00000010):	//������ ������� ������
			menu13_select();
			break;	
		case (0b00000001):	//������ ������ ������
			menu13_plus();
			break;	
		default:
			break;
	}

	return;
}

///////////////////////////////////
void menu13_select(void)
{
    timer_cursor.position++;
	if(timer_cursor.position==7) timer_cursor.position=0;
	timer_cursor.count=0;
	timer_cursor.flags.f_cursor_on=1;
	return;
}

///////////////////////////////////
void menu13_plus(void)
{
    switch(timer_cursor.position)
	{
		case (0):   //����� ������
			k++;
			if(k==16) k=0;
			menu13_intervals_calculate();
			break;
		case (1):   //���\���� ������
			channel_onoff_temp^= (1<<k);
			break;
		case (2):   //������
			min++;
			if(min>59) min=0;
			break;
		case (3):   //�������
			sec++;
			if(sec>59) sec=0;
			break;
		case (4):
            ms++;   //��
			if(ms>999) ms=0;
			break;
		case (5):   //���
			mks++;
			if(mks>999) mks=0;
			break;
		case (6):   //��
            ns+=25;
			if(ns>999) ns=0;
			break;
    }
	t_menu_temp[k]=ns+(mks*1000)+(ms*1000000)+((unsigned long long)sec*1000000000)+((unsigned long long)min*60000000*1000); 
	
	timer_cursor.count=0;
	timer_cursor.flags.f_cursor_on=0;
	menu13_refresh();
	return;
}

///////////////////////////////////
void menu13_minus(void)
{
    switch(timer_cursor.position)
	{
		case (0):   //����� ������
			k--;
			if(k==255) k=15;
			menu13_intervals_calculate();
			break;
		case (1):   //���\���� ������
			channel_onoff_temp^= (1<<k);
			break;	
		case (2):   //������
			min--;
			if(min>59) min=59;
			break;
		case (3):   //�������
			sec--;
			if(sec>59) sec=59;
			break;
		case (4):
            ms--;   //��
			if(ms>999) ms=999;
			break;
		case (5):   //���
			mks--;
			if(mks>999) mks=999;
			break;
		case (6):   //��
            ns-=25;
			if(ns>999) ns=975;
			break;
    }
	t_menu_temp[k]=ns+(mks*1000)+(ms*1000000)+((unsigned long long)sec*1000000000)+((unsigned long long)min*60000000*1000);
	
	timer_cursor.count=0;
	timer_cursor.flags.f_cursor_on=0;
	menu13_refresh();
	return;
}

///////////////////////////////////
void menu13_save(void)
{
    //��������� ���������� �� ������
    if(timer_verify_if_new_data(t_menu_temp,&channel_onoff_temp)) menu13_2_out();
    else menu0_out();   
    
    return;
}


///////////////////////////////////////////////////////////////

///////////////////////////////////
/*rom near*/ const unsigned char menu_13_2[]=
//	'--------------------'
//  '   xx ���������     ' 
//  '   xx ���������?    ' 
//	'                    '   
//  '���               ��'

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
//                      �  �  �  �  �  �  �  �  � 
 {__,__,__,__,__,__,__,xC,_O,_X,xP,_A,_H,RI,_T,_b,__,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//                      �  �  �  �  �  �  �  �  �  ?                                                                                         
  __,__,__,__,__,__,__,RI,RZ,_M,_E,_H,_E,_H,RI,JA,zw,__,__,__,
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
//  
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
// �  �  �                                               �  �                                                                           
  _H,_E,_T,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,RD,_A}; 

const unsigned char menu_13_pic[]=
{
    0xFE, 0x02, 0xFE, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x0A, 0xFE, 0x3F, 0x40, 
    0x40, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x45, 0x45, 0x7C, 0x40, 0x7F, 
};

void menu13_2_out(void)
{
	//�������������� ���������� ����
	menu_body_out(menu_13_2);
	menu_picture_out((unsigned char*) menu_13_pic,24,0,14,16);

	
	active_menu=m13_2;
	flags_menu.f_refresh=0;
    flags_menu.f_autochange=0;
	//�������������� ���������� menu
 	//������� ����������
	menu13_2_refresh();
    return;
}

///////////////////////////////////
void menu13_2_refresh(void)
{
	return;
}

///////////////////////////////////
void menu13_2_key_pressed(void)
{
	switch(button_status)
    {
		case (0b00001000):	//����� ������� ������
			break;	
		case (0b00000100):	//����� ������ ������
			menu0_out();
			break;	
		case (0b00000010):	//������ ������� ������
			break;	
		case (0b00000001):	//������ ������ ������
			menu13_2_yes();
			break;	
		default:
			break;
	}
	return;
}

///////////////////////////////////
void menu13_2_yes(void)
{
	timer_analyse(t_menu_temp,&channel_onoff_temp);
	menu0_out();
	return;
}



