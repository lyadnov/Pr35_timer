#include "p33FJ128GP706.h"
#include "main\errors.h" 

#include "system\timer\timer.h" 
#include "system\timer\timer_intervals.h" 
#include "system\flash\flash.h"
#include "system\eeprom\eeprom.h"
#include "system\menu\menu14_start.h"

//----------структуры--------------
typedef union 
{
    struct 
    {
        //управление от кнопок
         unsigned short f_write_data      :1; //требутеся записать данные с компьютера
         unsigned short f_start	          :1; //переход в ждущий режим
         unsigned short f_test_start      :1; //старт таймера, без ждущего режима, в тестовых целях
    }; 
    unsigned short word;
}flags_control_struct;
    
typedef struct
{
    flags_control_struct   flags_control;   //команда от PC (чтение данных или запись)
    unsigned long long t_global[16];        //абсолютные интервалы в нсек
    unsigned short  channel_onoff;          //каждому биту соответствует вкл\выкл канала PORTB
} data_struct;

data_struct data_in;
data_struct data_out;

//----------константы--------------
//команды процессору
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

//---------переменные--------------
unsigned short  channel_onoff;      //каждому биту соответствует вкл\выкл канала PORTB
unsigned long long t_global[16];    //абсолютные интервалы в нсек
unsigned long long dt[16];          //относительные интервалы
unsigned short portb[16];           //соответсвующие значения PORTB     

unsigned char timer_buf[1536];      //в этом буфере генериться код, выставляющий ножки PORTB в 1, в нужный момент, этот код затем записывается в Program memory
unsigned int timer_buf_offset;
unsigned long timer_flash_offset;
unsigned char actual_num=0;         //реальное количество ножек PORTB на которых нужно сгенерить импульс. если соответствующий временной интервал=0. ножка - не используется

unsigned char flag_first_start=0;
unsigned char flag_test_mode=0;     //если стоит в 1. то запуск тестовый и ножку RD10 в единицу не выставляю

//----------функции----------------
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

//--------------------------основная часть-------------------------------------------

///////////////////////////////////
void timer_init(void)
{
	//ножки
	TRISB=0;	
  	PORTB=0;
    TRISDbits.TRISD10=0;   //на выход - управление тактовым генератором
	//TRISDbits.TRISD8=1;    //на вход - старт таймера

	//переменные
	eeprom_read_buf(ADDR_EEPROM_TIMER_CHANNEL_ONOFF,(unsigned char*)&channel_onoff,2);
	eeprom_read_buf(ADDR_EEPROM_TIMER_INTERVALS,(unsigned char*)t_global,16*8);
	
    timer_create_new_function();    //создаем функцию timer_start()
	
	//прерывания
	INTCON2bits.INT1EP=0;   //Interrupt on positive edge
    IPC5bits.INT1IP=5;      //приоритет прерывания =5(выше чем у остальных прерываний)
    IFS1bits.INT1IF=0;      //на всякий случай сбрасываем флаг прерывания	
  	//IEC1bits.INT1IE=1;      //разрешаем прерывания от int1(RD8)
	
	return;
}

///////////////////////////////////
void timer_eeprom_write_default_values(void)
{
    unsigned int i;
    unsigned char buf[2];
    
    //включение\выключение каналов PORTB
    buf[0]=0xFF;
    buf[1]=0xFF;    //все каналы по умолчанию включены
    eeprom_write_buf(ADDR_EEPROM_TIMER_CHANNEL_ONOFF,buf,2);                   
    
    //временные интервалы по умолчанию
    for(i=0;i<16;i++) t_global[i]=(unsigned long long)i*1000000000;
    //for(i=0;i<16;i++) t_global[i]=25;
    eeprom_write_buf(ADDR_EEPROM_TIMER_INTERVALS,(unsigned char*) t_global,16*8);
    return;
}

///////////////////////////////////
void timer_analyse(unsigned long long * t_temp,unsigned short * channel_onoff_temp)
{
	unsigned int i;
	//проверяем поменялись ли значения временных интервалов
    if(!timer_verify_if_new_data(t_temp,channel_onoff_temp)) return;
    //сохраняем новые значения интервалов в RAM
    channel_onoff = *channel_onoff_temp;
    for(i=0;i<16;i++) t_global[i]=t_temp[i];
    //сохраняем новые значения интервалов в еепром
    eeprom_write_buf(ADDR_EEPROM_TIMER_CHANNEL_ONOFF,(unsigned char*) &channel_onoff,2);
    eeprom_write_buf(ADDR_EEPROM_TIMER_INTERVALS,(unsigned char*) t_global,16*8);
    
    timer_create_new_function();  
    
    return;
}

///////////////////////////////////
unsigned char timer_verify_if_new_data(unsigned long long * t_temp,unsigned short * channel_onoff_temp)
{
	unsigned char i;
	
	//проверяем поменялись ли вкл\выкл состояния каналов PORTB
	if(*channel_onoff_temp!=channel_onoff) return(1);   //данные поменялись
	
	//проверяем поменялись ли значения временных интервалов
    for(i=0;i<16;i++)
    {
        if(t_temp[i]!=t_global[i]) return(1); //данные поменялись
    }
    return(0);  //не менялись
}

void timer_create_new_function(void)
{
    unsigned char i;
    //инициаллизируем массив portb
    for(i=0;i<16;i++) portb[i]= (1<<i);      
    //инициаллизируем массив dt
    for(i=0;i<16;i++) dt[i]=t_global[i];
    //удаляем отключенные каналы
    actual_num=0;   //здесь хранится реальное число используемых ножек PORTB
    timer_delete_off_channels();
    //сортируем dt и portb
    timer_sort_dt_portb();
    //вычисляем t_delta
    timer_calculate_dt();
    //удаляем одинаковые значения
    timer_delete_same_values();
    
    //проверяю не создан ли уже ранее код timer_start() по адресу ADDR_TIMER_START - чтобы пик не писал одно и тоже каждый раз при старте
    if( (flash_read_word(ADDR_TIMER_START)==0x00200000)&&(flag_first_start==0) ) //0x00200000=ассемблерная инструкция: mov #0,w0
    { 
        flag_first_start=1;
        return; 
    }
    flag_first_start=1;
    
    //создаем и записываем в память программ новый код
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
//сортируем dt и portb. методом пузырьковой сортировки
{
    int i,j;
    unsigned long long temp_array[1];   //компилятор не умеет создавать переменные типа unsigned long long, а массивы умеет.
    #define temp    temp_array[0]
    
    for(j=1;j<actual_num;j++)
	{
        for(i=0;i<(actual_num-j);i++)
       	{
       	    if(dt[i]>dt[i+1])
       	    {
       	        //меняем местами dt
       	        temp=dt[i+1];
       	        dt[i+1]=dt[i];
       	        dt[i]=temp;      
       	        //меняем местами portb
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
//вычисляем относительные интервалы в массиве dt    
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
//удаляем одинаковые значения из массива dt
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
            portb_temp[actual_num_temp-1]|=portb[i];  //одновременно выстрелим на этих каналах
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
      
    //затираем PORTB
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
            
    //подготавливаем регистры w0 и w1
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
    
    //если тестовый режим вклечен. то следующая команда не выполняется
    //btss test_mode,0
    cmd=CMD_BTSS_F;
    addr=(unsigned long)&flag_test_mode;
    bit_num=0;
    cmd=cmd | addr | ((bit_num&0x08)>>3) |(bit_num&0x07)<<13;
    timer_write_cmd_to_buf(cmd);
    
    //выставляем ножку RD10 в 1
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
    
    //вот гдето тут время замерло ...
       
    return;
}

///////////////////////////////////
void timer_code_end(void)
{
    unsigned long cmd;
    unsigned long addr;     //PORTB,....    
    unsigned long bit_num;
  
    //сбрасываем RD10 в 0
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
    
    //записываем последний раз timer_buf во флеш  
    flash_erase_block(ADDR_TIMER_START+timer_flash_offset);
    flash_write_block(ADDR_TIMER_START+timer_flash_offset,timer_buf);
    return;
}

///////////////////////////////////
void timer_code_generate(unsigned char num)
//num - номер элемента в массивах dt и portb
{   
    unsigned long cmd;
    unsigned long addr;
    unsigned int i;
    
    unsigned long long temp_array[3];   //компилятор не умеет создавать переменные типа unsigned long long, а массивы умеет.
    #define x   temp_array[0]
    #define n   temp_array[1]
    #define ost temp_array[2]
    
    x=dt[num];      
    if(num!=0) x-=25; //25нс-время необходимое на выполение последней команды "mov [w0++],[w1]", единица на ножке возникает в конце выполнения этой команды, т.е в конце 25нс интервала  
    //if(x!=0) x-=25;
           
    //100с
    n=x/(100000000);      //n=x/100000000000 делим за 2 операции, потому что иначе комплилятор ругается на размер константы 100000000000
    n=n/1000;
    ost=x-(n*100000000*1000);        //x%100000000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_100s
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_100s; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //10с
    n=x/10000000;       //n=x/10000000000;
    n=n/1000;
    ost=x-(n*10000000*1000);           //ost=x%10000000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_10s
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_10s; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //1с
    n=x/1000000000;
    ost=x%1000000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_1s
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_1s; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //100мс
    n=x/100000000;
    ost=x%100000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_100ms
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_100ms; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //10мс
    n=x/10000000;
    ost=x%10000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_10ms
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_10ms; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //1мс
    n=x/1000000;
    ost=x%1000000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_1ms
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_1ms; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //100мкс
    n=x/100000;
    ost=x%100000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_100mcs
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_100mcs; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //10мкс
    n=x/10000;
    ost=x%10000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_10mcs
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_10mcs; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //1мкс
    n=x/1000;
    ost=x%1000;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_1mcs
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_1mcs; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //250нс
    n=x/250;
    ost=x%250;
    for(i=0;i<n;i++)
    {
        //call _timer_pause_250ns
        cmd=CMD_CALL_1;
        addr=(unsigned int)timer_pause_250ns; //(unsigned long) глючит
        cmd=cmd | (addr&0xFFFF);
        timer_write_cmd_to_buf(cmd);
        cmd=CMD_CALL_2;
        cmd=cmd | ((addr>>16)&0x007F);
        timer_write_cmd_to_buf(cmd);    
    }
    x=ost;
    //25нс
    n=x/25;
    for(i=0;i<n;i++)
    {
        //nop
        timer_write_cmd_to_buf(CMD_NOP); 
    }
    //выводим требуемое значение в PORTB
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
        if(timer_flash_offset>= (ADDR_TIMER_PAUSE_250ns-ADDR_TIMER_START))  //проверка на макс. размер кода
		{
			error_num=err_TimerCodeTooBig;
			error_out();
		}
		//занулим timer_buf на всякий случай
        for(i=0;i<1536;i++) timer_buf[i]=0;
    }
    return;
}




///////////////////////////////////
void timer_usart_control(void)
{
    unsigned char i,len;
    unsigned short count;
      
    //принимаем данные от компьютера
    if(UsartRxPacket((unsigned char*)&data_in,&len,sizeof(data_struct))) return;
	
	//подготавливаем данные для отправки
	data_out.flags_control.word=0;
    for(i=0;i<16;i++) data_out.t_global[i]=t_global[i];
    data_out.channel_onoff=channel_onoff;
    //отправляем данные
    if(UsartTxPacket((unsigned char*)&data_out,sizeof(data_struct))) return;
	
	//обрабатываем принятые данные
	if(data_in.flags_control.f_write_data==1) 
    {
        //сохраняем принятые данные
        timer_analyse((unsigned long long *)data_in.t_global,(unsigned short *)&data_in.channel_onoff);
    }    
    if(data_in.flags_control.f_start==1) 
    {
        //запуск
        flag_test_mode=0;
        SRbits.IPL=5;
	    menu14_out();
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
	    
	    menu0_out();
	    SRbits.IPL=0;
	    
	}    
    if(data_in.flags_control.f_test_start==1) 
    {
        //тестовый запуск
        SRbits.IPL=5;
	    flag_test_mode=1;
	    menu14_out();
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
        
	    menu0_out();
	    flag_test_mode=0;
	    SRbits.IPL=0;
	}    
    return;
}
