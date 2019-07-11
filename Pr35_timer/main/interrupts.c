#include "p33FJ128GP706.h"

#include "system\menu\menu_proc.h"
#include "system\timer\timer.h"

//---------����������--------------

//----------�������----------------

//--------------------------�������� �����-------------------------------------------
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF=0;
    
	menu_button_processing_TMR1(); 	//����������� ���������� ������ (������ �� ��������)
	menu_refresh();					//�������������� �������
	menu_autochange();				//��������� ����������� ������� ������
    return;
}

void __attribute__((interrupt,auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF=0;
    menu_button_processing_CN();	//����������� ������� ������
    return;
}

void __attribute__((interrupt,auto_psv)) _INT1Interrupt(void)
{
    //timer_start();
    IFS1bits.INT1IF=0;              
    
    return;
}




