#ifndef I2C_APP_H
#define I2C_APP_H
#include "bsp_app.h"
void eeprom_write(uint8_t *EEPROM_String, uint8_t addr, uint8_t num);
void eeprom_read(uint8_t *EEPROM_String, uint8_t addr, uint8_t num);
void mcp4017_write(uint8_t data);
void mcp4017_read(uint8_t *data);




#endif



