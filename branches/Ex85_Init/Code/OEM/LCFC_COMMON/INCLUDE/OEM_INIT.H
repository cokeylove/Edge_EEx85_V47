/*-----------------------------------------------------------------------------
 * TITLE: OEM_INI.H
 *
 * OEM defined constants for initializing variables to known values on reset.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef OEM_INI_H
#define OEM_INI_H

extern void Oem_StartUp(void);
extern void Oem_Initialization(void);
extern void Init_ClearRam(void);
extern void Clear_RAM_Range(WORD nAddr, WORD nSize);
extern void RAM_travel(WORD sour, WORD targ, BYTE count);
extern void Hook_ECExitFollowMode(void);
extern BYTE Hook_ECRetunrMainFuncKeepCondition(void);
extern void Init_SMBus_Regs(void);
extern void Init_Regs(void);
#if chPWSW1WDT
extern void OEM_RESETPWSWTIMER1(void);	
extern void OEM_PWRSWWDTEn(void);
#endif  //chPWSW1WDT
extern void CacheDma(BYTE sramnum,WORD addr);

extern BYTE Check_Mirror_Occurs(void);

typedef struct REG_INIT_DEF
{
    XBYTE   *address;
    BYTE    initdata;
} sREG_INIT_DEF;

typedef struct DMAScratchSRAM
{
  XBYTE  *scarh  ;
  XBYTE  *scarm  ;
  XBYTE  *scarl  ;
} sDMAScratchSRAM;

#if Auto_Detect_ITE_chip
extern void Auto_Setting_ITE_CHIP(void);
#endif 

#endif
