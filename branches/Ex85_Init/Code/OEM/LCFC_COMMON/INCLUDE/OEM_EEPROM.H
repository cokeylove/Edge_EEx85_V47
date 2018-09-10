/*-----------------------------------------------------------------------------
 * TITLE: OEM_EEPROM.H
 *
 * Author : Ivan.Chen
 *
 * Note : EEPROM define.
 *---------------------------------------------------------------------------*/
 
#ifndef OEM_EEPROM_H
#define OEM_EEPROM_H

extern void Chk_pDevStus(void);
extern void Update_EEPROMMark(void);
extern void WRITE_DATA2EEPROM(PORT_BYTE_PNTR ram_pntr, XWORD size, BYTE EEPROMBANK, XWORD offset);
extern void UPDATA_EEPROM_DATA(PORT_BYTE_PNTR ram_pntr, XWORD size, BYTE EEPROMBANK, XWORD offset);
extern void Update_16ByteEEPROM(void);
extern void Erase_EEPROMAll(void);
extern void Update_ECEEPROM(void);


#define EEPROMA2              EFLASHSIZE	// 8996

#define EEPROMA1_16Byte       0xC8
#define EEPROMM1_16Byte_end   0xCB

#define EEPROM_Temp_Swith     0xCC

//BANK 0
#define EEPROM_Bank0          0xC0
#define EEPROM_modify_flag    0xEB
#define EEPROM_KB_Type        0xF0

//BANK 1
#define EEPROM_Bank1          0xC1

//BANK 2
#define EEPROM_Bank2          0xC2

//BANK 3
#define EEPROM_Bank3          0xC3

//BANK 4
#define EEPROM_Bank4          0xC4

//BANK 5
#define EEPROM_Bank5          0xC5

//BANK 6
#define EEPROM_Bank6          0xC6

//BANK 7
#define EEPROM_Bank7          0xC7
#define EEPROM_Bank7_L16B     0xB7F0
#define EEPROM_TYPEC_STATUS   0x02
#define EEPROM_PORT80_HISTORY 0xC0 //0xC0 ~ 0xCE
#define EEPROM_BATStopCharge  0xD0
#define EEPROM_BATStartCharge 0xD1
#define EEPROM_MBID           0xE0
#define EEPROM_DevStus        0xE5
 #define EEPROM_DevStus_USB   BIT0
#define EEPROM_GPIO_STATUS    0xEF
#define EEPROM_SAVE_PORT80    0xF1
#define EEPROM_ACWOL_STATUS    0xF2
#define EEPROM_DCWOL_STATUS    0xF3

#endif
