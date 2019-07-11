#ifndef _EEPROM_INCLUDED
#define _EEPROM_INCLUDED

//---------константы---------------
#define FLASH_BLOCK_SIZE 0x400  //в словах: 2слова=3байта   
#define ADDR_FLASH_END 0x015800

#define EEPROM_SIZE 192         //в байтах 
#define ADDR_EEPROM_START (ADDR_FLASH_END-(EEPROM_SIZE/3)*2)
#define ADDR_BLOCK_WITH_EEPROM_START (ADDR_FLASH_END-FLASH_BLOCK_SIZE)

//адреса в EEPROM
#define ADDR_EEPROM_VERSION             0x01
#define ADDR_EEPROM_LIGHT               0x03
#define ADDR_EEPROM_CONTRAST            0x04
#define ADDR_EEPROM_TIMER_CHANNEL_ONOFF 0x05 //размер 2 байта
#define ADDR_EEPROM_TIMER_INTERVALS     0x07 //размер 16*8=128байт

//---------переменные--------------

//----------функции----------------
unsigned char eeprom_read(unsigned char addr);
void eeprom_read_buf(unsigned char addr,unsigned char *buf,unsigned char size);
void eeprom_write(unsigned char addr,unsigned char data);
void eeprom_write_buf(unsigned char addr,unsigned char *buf,unsigned char size);

#endif



