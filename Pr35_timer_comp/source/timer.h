#ifndef _TIMER_INCLUDED
#define _TIMER_INCLUDED


//---------���� ������--------------
typedef union 
{
	struct 
    {
		//���������� �� ������
        unsigned short f_write_data      :1; //��������� �������� ������ � ����������
		unsigned short f_start			 :1; //������� � ������ �����
    	unsigned short f_test_start		 :1; //����� �������, ��� ������� ������, � �������� �����
	}; 
    unsigned short word;
}flags_control_struct;
    
typedef struct
{
	flags_control_struct   flags_control;   //������� �� PC (������ ������ ��� ������)
    unsigned char t_global[16][8];			//���������� ��������� � ����
    unsigned char  channel_onoff[2];        //������� ���� ������������� ���\���� ������ PORTB
} data_struct;

//---------����������--------------
extern data_struct   datain;
extern data_struct   dataout;

//----------�������----------------
void timer_init(void);

#endif





