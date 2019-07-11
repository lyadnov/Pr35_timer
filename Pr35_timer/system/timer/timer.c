#include "p33FJ128GP706.h"
#include "main\errors.h" 

#include "system\timer\timer.h" 
#include "system\timer\timer_intervals.h" 
#include "system\flash\flash.h"
#include "system\eeprom\eeprom.h"
#include "system\menu\menu14_start.h"

//----------���������--------------
typedef union 
{
    struct 
    {
        //���������� �� ������
         unsigned short f_write_data      :1; //��������� �������� ������ � ����������
         unsigned short f_start	          :1; //������� � ������ �����
         unsigned short f_test_start      :1; //����� �������, ��� ������� ������, � �������� �����
    }; 
    unsigned short word;
}flags_control_struct;
    
typedef struct
{
    flags_control_struct   flags_control;   //������� �� PC (������ ������ ��� ������)
    unsigned long long t_global[16];        //���������� ��������� � ����
    unsigned short  channel_onoff;          //������� ���� ������������� ���\���� ������ PORTB
} data_struct;

data_struct data_in;
data_struct data_out;

//----------���������--------------
//������� ����������
#define CMD_NOP         0x00000000  //nop
#define CMD_LNK         0x00FA0000  //lnk      
#define CMD_ULNK        0x00FA8000  //ulnk  
#define CMD_MOV_CONST_W 0x00200000  //mov   #0000,w0
#define CMD_MOV_W_F     0x00880000  //mov   wx,f
#define CMD_MOV_W0PP_W1 0x007808B0  //mov	[w0++],[w1]
#define CMD_RETURN      0x00060000  //return
#define CMD_RETFIE      0x00064000  //retfie
#define CMD_RCALL       0x00070000  //rcall 0x000000              
#define CMD_CALL_1      0x00020000  //call 0x000000
#define CMD_CALL_2      0x00000000  //call 0x000000
#define CMD_GOTO_1      0x00040000  //goto 0x000000
#define CMD_GOTO_2      0x00000000  //goto 0x000000
#define CMD_BRA_Z       0x0032FFFE  //bra   z,$-2 

#define CMD_BCLR_F      0x00A90000  //bclr 0x0000,0 
#define CMD_BSET_F      0x00A80000  //bset 0x0000,0 
#define CMD_BTST_F      0x00AB0000  //btst 0x0000,0 
#define CMD_BTSC_F      0x00AF0000  //btsc 0x0000,0 
#define CMD_BTSS_F      0x00AE0000  //btss 0x0000,0

//---------����������--------------
unsigned short  channel_onoff;      //������� ���� ������������� ���\���� ������ PORTB
unsigned long long t_global[16];    //���������� ��������� � ����
unsigned long long dt[16];          //������������� ���������
unsigned short portb[16];           //�������������� �������� PORTB     

unsigned char timer_buf[1536];      //� ���� ������ ���������� ���, ������������ ����� PORTB � 1, � ������ ������, ���� ��� ����� ������������ � Program memory
unsigned int timer_buf_offset;
unsigned long timer_flash_offset;
unsigned char actual_num=0;         //�������� ���������� ����� PORTB �� ������� ����� ��������� �������. ���� ��������������� ��������� ��������=0. ����� - �� ������������

unsigned char flag_first_start=0;
unsigned char flag_test_mode=0;     //���� ����� � 1. �� ������ �������� � ����� RD10 � ������� �� ���������

//----------�������----------------
void timer_analyse(unsigned long long * t_temp,unsigned short * channel_onoff_temp);
    unsigned char timer_verify_if_new_data(unsigned long long * t_temp, unsigned short * channel_onoff_temp);
    void timer_create_new_function(void);
        void timer_delete_off_channels(void);
        void timer_sort_dt_portb(void);
        void timer_calculate_dt(void);
        void timer_delete_same_values(void);
        void timer_code_start(void);
        void timer_code_generate(unsigned char num);
            void timer_write_cmd_to_buf(unsigned long cmd);
        void timer_code_end(void);
void timer_start(void);        

//--------------------------�������� �����-------------------------------------------

///////////////////////////////////
void timer_init(void)
{
	//�����
	TRISB=0;	
  	PORTB=0;
    TRISDbits.TRISD10=0;   //�� ����� - ���������� �������� �����������
	//TRISDbits.TRISD8=1;    //�� ���� - ����� �������

	//����������
	eeprom_read_buf(ADDR_EEPROM_TIMER_CHANNEL_ONOFF,(unsigned char*)&channel_onoff,2);
	eeprom_read_buf(ADDR_EEPROM_TIMER_INTERVALS,(unsigned char*)t_global,16*8);
	
    timer_create_new_function();    //������� ������� timer_start()
	
	//����������
	INTCON2bits.INT1EP=0;   //Interrupt on positive edge
    IPC5bits.INT1IP=5;      //��������� ���������� =5(���� ��� � ��������� ����������)
    IFS1bits.INT1IF=0;      //�� ������ ������ ���������� ���� ����������	
  	//IEC1bits.INT1IE=1;      //��������� ���������� �� int1(RD8)
	
	return;
}

///////////////////////////////////
void timer_eeprom_write_default_values(void)
{
    unsigned int i;
    unsigned char buf[2];
    
    //���������\���������� ������� PORTB
    buf[0]=0xFF;
    buf[1]=0xFF;    //��� ������ �� ��������� ��������
    eeprom_write_buf(ADDR_EEPROM_TIMER_CHANNEL_ONOFF,buf,2);                   
    
    //��������� ��������� �� ���������
    for(i=0;i<16;i++) t_global[i]=(unsigned long long)i*1000000000;
    //for(i=0;i<16;i++) t_global[i]=25;
    eeprom_write_buf(ADDR_EEPROM_TIMER_INTERVALS,(unsigned char*) t_global,16*8);
    return;
}

///////////////////////////////////
void timer_analyse(unsigned long long * t_temp,unsigned short * channel_onoff_temp)
{
	unsigned int i;
	//��������� ���������� �� �������� ��������� ����������
    if(!timer_verify_if_new_data(t_temp,channel_onoff_temp)) return;
    //��������� ����� �������� ���������� � RAM
    channel_onoff = *channel_onoff_temp;
    for(i=0;i<16;i++) t_global[i]=t_temp[i];
    //��������� ����� �������� ���������� � ������
    eeprom_write_buf(ADDR_EEPROM_TIMER_CHANNEL_ONOFF,(unsigned char*) &channel_onoff,2);
    eeprom_write_buf(ADDR_EEPROM_TIMER_INTERVALS,(unsigned char*) t_global,16*8);
    
    timer_create_new_function();  
    
    return;
}

///////////////////////////////////
unsigned char timer_verify_if_new_data(unsigned long long * t_temp,unsigned short * channel_onoff_temp)
{
	unsigned char i;
	
	//��������� ���������� �� ���\���� ��������� ������� PORTB
	if(*channel_onoff_temp!=channel_onoff) return(1);   //������ ����������
	
	//��������� ���������� �� �������� ��������� ����������
    for(i=0;i<16;i++)
    {
        if(t_temp[i]!=t_global[i]) return(1); //������ ����������
    }
    return(0);  //�� ��������
}

void timer_create_new_function(void)
{
    unsigned char i;
    //��������������� ������ portb
    for(i=0;i<16;i++) portb[i]= (1<<i);      
    //��������������� ������ dt
    for(i=0;i<16;i++) dt[i]=t_global[i];
    //������� ����������� ������
    actual_num=0;   //����� �������� �������� ����� ������������ ����� PORTB
    timer_delete_off_channels();
    //��������� dt � portb
    timer_sort_dt_portb();
    //��������� t_delta
    timer_calculate_dt();
    //������� ���������� ��������
    timer_delete_same_values();
    
    //�������� �� ������ �� ��� ����� ��� timer_start() �� ������ ADDR_TIMER_START - ����� ��� �� ����� ���� � ���� ������ ��� ��� ������
    if( (flash_read_word(ADDR_TIMER_START)==0x00200000)&&(flag_first_start==0) ) //0x00200000=������������ ����������: mov #0,w0
    { 
        flag_first_start=1;
        return; 
    }
    flag_first_start=1;
    
    //������� � ���������� � ������ �������� ����� ���
    timer_buf_offset=0;
    timer_flash_offset=0;
    timer_code_start();
    for(i=0;i<actual_num;i++) timer_code_generate(i);
    timer_code_end();
   
    return;
}  

///////////////////////////////////
void timer_delete_off_channels(void)
{
    unsigned int i;
    unsigned long long dt_temp[16];
    unsigned short portb_temp[16];
       
    for(i=0;i<16;i++)
    {
        if(channel_onoff&(1<<i))
        { 
            dt_temp[actual_num]=dt[i];
            portb_temp[actual_num]=portb[i];
            actual_num++;
        }    
    }
    for(i=0;i<actual_num;i++)
    { 
        dt[i]=dt_temp[i];
        portb[i]=portb_temp[i];
    }    
    return;
}

///////////////////////////////////
void timer_sort_dt_portb(void)
//��������� dt � portb. ������� ����������� ����������
{
    int i,j;
    unsigned long long temp_array[1];   //���������� �� ����� ��������� ���������� ���� unsigned long long, � ������� �����.
    #define temp    temp_array[0]
    
    for(j=1;j<actual_num;j++)
	{
        for(i=0;i<(actual_num-j);i++)
       	{
       	    if(dt[i]>dt[i+1])
       	    {
       	        //������ ������� dt
       	        temp=dt[i+1];
       	        dt[i+1]=dt[i];
       	        dt[i]=temp;      
       	        //������ ������� portb
       	        temp=portb[i+1];
       	        portb[i+1]=portb[i];
       	        portb[i]=temp;
       	    }   
       	}
	}

    return;
}

///////////////////////////////////
void timer_calculate_dt(void)
//��������� ������������� ��������� � ������� dt    
{
	unsigned int i;
	
    if(actual_num==0) return;
    for(i=(actual_num-1);i>0;i--)
    {
        dt[i]=dt[i]-dt[i-1];    
    }
        
    return;        
}


///////////////////////////////////
void timer_delete_same_values(void)
//������� ���������� �������� �� ������� dt
{
    unsigned int i,actual_num_temp;
    unsigned long long dt_temp[16];
    unsigned short portb_temp[16];
    unsigned short x;
       
    if(actual_num==0) return; 
    
    dt_temp[0]=dt[0];
    portb_temp[0]=portb[0];
    actual_num_temp=1;
        
    for(i=1;i<actual_num;i++)
    {
        if(dt[i]!=0)
        { 
            dt_temp[actual_num_temp]=dt[i];
            portb_temp[actual_num_temp]=portb[i];
            actual_num_temp++;
        }
        else
        {
            portb_temp[actual_num_temp-1]|=portb[i];  //������������ ��������� �� ���� �������
        }    
    }
    actual_num=actual_num_temp;
    for(i=0;i<actual_num;i++)
    { 
        dt[i]=dt_temp[i];
        x|=portb_temp[i];
        portb[i]=x;
    }    
    
    return;
}

  
///////////////////////////////////
void timer_code_start(void)
{
    unsigned long cmd;
    unsigned long addr;
    unsigned char reg;
    unsigned long bit_num;
      
    //�������� PORTB
    //mov	#0,w0
    cmd=CMD_MOV_CONST_W;
    addr=0;
    reg=0;
    cmd=cmd | (addr<<4) | reg;
    timer_write_cmd_to_buf(cmd);
    //mov   w0,PORTB
    cmd=CMD_MOV_W_F;
    addr=(unsigned long)&PORTB;
    reg=0;
    cmd=cmd | (addr<<3) | reg;
    timer_write_cmd_to_buf(cmd);
            
    //�������������� �������� w0 � w1
    //mov	#_portb,w0
    cmd=CMD_MOV_CONST_W;
    addr=(unsigned long)portb;
    reg=0;
    cmd=cmd | (addr<<4) | reg;
    timer_write_cmd_to_buf(cmd);
    //mov	#PORTB,w1
    cmd=CMD_MOV_CONST_W;
    addr=(unsigned long)&PORTB;
    reg=1;
    cmd=cmd | (addr<<4) | reg;
    timer_write_cmd_to_buf(cmd);
    
    //���� �������� ����� �������. �� ��������� ������� �� �����������
    //btss test_mode,0
    cmd=CMD_BTSS_F;
    addr=(unsigned long)&flag_test_mode;
    bit_num=0;
    cmd=cmd | addr | ((bit_num&0x08)>>3) |(bit_num&0x07)<<13;
    timer_write_cmd_to_buf(cmd);
    
    //���������� ����� RD10 � 1
    //bset  PORTD,10
    cmd=CMD_BSET_F;
    addr=(unsigned long)&PORTD;
    bit_num=10;
    cmd=cmd | addr | ((bit_num&0x08)>>3) |(bit_num&0x07)<<13;
    timer_write_cmd_to_buf(cmd);
    
//    //nop
//    cmd=CMD_NOP;
//    timer_write_cmd_to_buf(cmd);
//    
//    //nop
//    cmd=CMD_NOP;
//    timer_write_cmd_to_buf(cmd);
    
    //��� ����� ��� ����� ������� ...
       
    return;
}

///////////////////////////////////
void timer_code_end(void)
{
    unsigned long cmd;
    unsigned long addr;     //PORTB,....    
    unsigned long bit_num;
  
    //���������� RD10 � 0
    //bclr PORTD,10
    cmd=CMD_BCLR_F;
    addr=(unsigned long)&PORTD;
    bit_num=10;
    cmd=cmd | addr | ((bit_num&0x08)>>3) |(bit_num&0x07)<<13;
    timer_write_cmd_to_buf(cmd);
    
//    //btsc  PORTD,#8
//    cmd=CMD_BTSC_F;
//    addr=(unsigned long)&PORTD;
//    bit_num=8;
//    cmd=cmd | addr | ((bit_num&0x08)>>3) |(bit_num&0x07)<<13;
//    timer_write_cmd_to_buf(cmd);
//    
//    //goto  TIMER_START
//    cmd=CMD_GOTO_1;
//    addr=(ADDR_TIMER_START&0xFFFF);
//    cmd=cmd | addr;
//    timer_write_cmd_to_buf(cmd);
//    cmd=CMD_GOTO_2;
//    addr=((unsigned long)ADDR_TIMER_START>>16);
//    cmd=cmd | addr;
//    timer_write_cmd_to_buf(cmd);
       
    //return 
    timer_write_cmd_to_buf(CMD_RETURN);
    
    //���������� ��������� ��� timer_buf �� ����  
    flash_erase_block(ADDR_TIMER_START+timer_flash_offset);
    flash_write_block(ADDR_TIMER_START+timer_flash_offset,timer_buf);
    return;
}

///////////////////////////////////
void timer_code_generate(unsigned char num)
//num - ����� �������� � �������� dt � portb
{   
    unsigned long cmd;
    unsigned long addr;
    unsigned int i;
    
    unsigned long long temp_array[3];   //���������� �� ����� ��������� ���������� ���� unsigned long long, � ������� �����.
    #define x   temp_array[0]
    #define n   temp_array[1]
    #define ost temp_array[2]
    
    x=dt[num];      
    if(num!=0) x-=25; //25��-����� ����������� �� ��������� ��������� ������� "mov [w0++],[w1]", ������� �� ����� ��������� � ����� ���������� ���� �������, �.� � ����� 25�� ���������  
    //if(x!=0) x-=25;
           
    //100�
    n=x/(100000000);      //n=x/100000000000 ����� �� 2 ��������, ������ ��� ����� ����������� �������� �� ������ ��������� 100000000000
    n=n/1000;
    ost=x-(n*100000000*1000);        //x%100000000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_100s
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_100s; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //10�
    n=x/10000000;       //n=x/10000000000;
    n=n/1000;
    ost=x-(n*10000000*1000);           //ost=x%10000000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_10s
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_10s; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //1�
    n=x/1000000000;
    ost=x%1000000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_1s
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_1s; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //100��
    n=x/100000000;
    ost=x%100000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_100ms
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_100ms; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //10��
    n=x/10000000;
    ost=x%10000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_10ms
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_10ms; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //1��
    n=x/1000000;
    ost=x%1000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_1ms
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_1ms; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //100���
    n=x/100000;
    ost=x%100000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_100mcs
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_100mcs; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //10���
    n=x/10000;
    ost=x%10000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_10mcs
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_10mcs; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //1���
    n=x/1000;
    ost=x%1000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_1mcs
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_1mcs; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //250��
    n=x/250;
    ost=x%250;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_250ns
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_250ns; //(unsigned long) ������
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //25��
    n=x/25;
    for(i=0;i<n;i++)
    {
        //nop
        timer_write_cmd_to_buf(CMD_NOP); 
    }
    //������� ��������� �������� � PORTB
    //mov	[w0++],[w1];
    timer_write_cmd_to_buf(CMD_MOV_W0PP_W1);
     
    return;
}



///////////////////////////////////
void timer_write_cmd_to_buf(unsigned long cmd)
{
    unsigned int i;
    timer_buf[timer_buf_offset++]=cmd;
    timer_buf[timer_buf_offset++]=cmd>>8;
    timer_buf[timer_buf_offset++]=cmd>>16;
    if(timer_buf_offset==1536)
    {
        flash_erase_block(ADDR_TIMER_START+timer_flash_offset);
        flash_write_block(ADDR_TIMER_START+timer_flash_offset,timer_buf);
        timer_buf_offset=0;
        timer_flash_offset+=1024;
        if(timer_flash_offset>= (ADDR_TIMER_PAUSE_250ns-ADDR_TIMER_START))  //�������� �� ����. ������ ����
		{
			error_num=err_TimerCodeTooBig;
			error_out();
		}
		//������� timer_buf �� ������ ������
        for(i=0;i<1536;i++) timer_buf[i]=0;
    }
    return;
}




///////////////////////////////////
void timer_usart_control(void)
{
    unsigned char i,len;
    unsigned short count;
      
    //��������� ������ �� ����������
    if(UsartRxPacket((unsigned char*)&data_in,&len,sizeof(data_struct))) return;
	
	//�������������� ������ ��� ��������
	data_out.flags_control.word=0;
    for(i=0;i<16;i++) data_out.t_global[i]=t_global[i];
    data_out.channel_onoff=channel_onoff;
    //���������� ������
    if(UsartTxPacket((unsigned char*)&data_out,sizeof(data_struct))) return;
	
	//������������ �������� ������
	if(data_in.flags_control.f_write_data==1) 
    {
        //��������� �������� ������
        timer_analyse((unsigned long long *)data_in.t_global,(unsigned short *)&data_in.channel_onoff);
    }    
    if(data_in.flags_control.f_start==1) 
    {
        //������
        flag_test_mode=0;
        SRbits.IPL=5;
	    menu14_out();
	    timer_start();
	    
	    //������� �2 - �� ����������� ����� ������� �������, ��� ������ ������ ���� ��������
    	while(count<10000)
    	{
        	//��������� ��� ��� ������ ��������
        	if( (~PORTG&0x03C0) !=0)  
        	{
                //���� �������
                count=0;
            }
        	else
        	{
                //��� ������ ��������. ���� ��������� �����
                count++;
            }
    	}
	    IFS1bits.CNIF=0;
	    
	    menu0_out();
	    SRbits.IPL=0;
	    
	}    
    if(data_in.flags_control.f_test_start==1) 
    {
        //�������� ������
        SRbits.IPL=5;
	    flag_test_mode=1;
	    menu14_out();
	    timer_start();
	    
	    //������� �2 - �� ����������� ����� ������� �������, ��� ������ ������ ���� ��������
    	while(count<10000)
    	{
        	//��������� ��� ��� ������ ��������
        	if( (~PORTG&0x03C0) !=0)  
        	{
                //���� �������
                count=0;
            }
        	else
        	{
                //��� ������ ��������. ���� ��������� �����
                count++;
            }
    	}
        IFS1bits.CNIF=0;
        
	    menu0_out();
	    flag_test_mode=0;
	    SRbits.IPL=0;
	}    
    return;
}
