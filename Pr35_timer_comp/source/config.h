#ifndef _CONFIG_INCLUDED
#define _CONFIG_INCLUDED


//---------���� ������--------------

typedef struct 
{
    unsigned char com_num;
} config_struct; 


//---------����������--------------
extern config_struct    config;

//----------�������----------------
void config_load(void);
void config_save(void);
void config_set_default_values(void);

#endif





