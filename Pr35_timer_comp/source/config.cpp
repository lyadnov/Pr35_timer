#include "stdafx.h"
#include "comm.h"
#include "errors.h"
#include "config.h"


//----------константы-------------- 

//---------переменные--------------
config_struct    config;


//----------функции----------------


//--------------------------основная часть-------------------------------------------


///////////////////////////////////
void config_load(void)
{
    FILE *config_file;
    const char buf[]="COM=";
	unsigned char x;

    config_file=fopen("timer.ini","rb");
    if(config_file==NULL)
    {
        config_set_default_values();
    
    } 
    else
    {
    
        //COM num
        fscanf(config_file, "%c", &x);  //C
        fscanf(config_file, "%c", &x);  //O
        fscanf(config_file, "%c", &x);  //M
        fscanf(config_file, "%c", &x);  //=
        fscanf(config_file, "%c", &x);
        config.com_num=x-0x30;       
        fclose(config_file);
        if((config.com_num<1)||(config.com_num>8)) config_set_default_values();
    }
    return;
}


///////////////////////////////////
void config_save(void)
{
    FILE *config_file;
    const char buf[]="COM=";
	int i;

    config_file=fopen("timer.ini","wb");
	for(i=0;i<(int)strlen(buf);i++) fprintf(config_file,"%c",buf[i]);
	fprintf(config_file,"%.1u",config.com_num);
    fclose(config_file);
    return;
}


///////////////////////////////////
void config_set_default_values(void)
{
    config.com_num=1;
    config_save();
    return;
}

