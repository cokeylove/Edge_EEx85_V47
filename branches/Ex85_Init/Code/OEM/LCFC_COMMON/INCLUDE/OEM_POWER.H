/*----------------------------------------------------------------------------
 * Filename: OEM_Power.H  For Chipset: ITE.IT85XX
 *
 * Function: Power sequnce control function example
 *
 * Author  : Dino
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_POWER_H
#define OEM_POWER_H

//-----------------------------------------------------------------------------
// Define
//-----------------------------------------------------------------------------
#define SYSTEM_S0         0x10
#define SYSTEM_S3         0x33
#define SYSTEM_S4         0x44
#define SYSTEM_S5         0x55
#define SYSTEM_S5_S0      0x50
#define SYSTEM_S4_S0      0x40
#define SYSTEM_S3_S0      0x30
#define SYSTEM_S0_S3      0x03
#define SYSTEM_S0_S4      0x04
#define SYSTEM_S0_S5      0x05
#define SYSTEM_EC_WDTRST  0xFF
#define SYSTEM_S3_DS3     0x60
#define SYSTEM_DS3        0x63
#define SYSTEM_DSxOK      0x64
#define SYSTEM_DS3_S3     0x65
#define SYSTEM_S3S4       0x34

//-----------------------------------------------------------------------------
// System status define
//-----------------------------------------------------------------------------
#define SystemIsS0          (SysPowState==SYSTEM_S0)
#define SystemIsS3          (SysPowState==SYSTEM_S3)
#define SystemIsS4          (SysPowState==SYSTEM_S4)
#define SystemIsS5          (SysPowState==SYSTEM_S5)

#define SystemNotS0         (SysPowState!=SYSTEM_S0)
#define SystemNotS3         (SysPowState!=SYSTEM_S3)
#define SystemNotS4         (SysPowState!=SYSTEM_S4)
#define SystemNotS5         (SysPowState!=SYSTEM_S5)

extern void SysPowerContrl(void);
extern void OEM_SYSTEM_S0 (void);
extern void OEM_SYSTEM_S3 (void);
extern void OEM_SYSTEM_S5 (void);
extern void OEM_SYSTEM_S3_S0 (void);
extern void OEM_SYSTEM_S0_S3 (void);
extern void OEM_SYSTEM_S5_S0 (void);
extern void OEM_SYSTEM_S0_S5 (void);
extern void Check_LPC_Reset(void);
extern void OEM_LPC_Reset(void);
#if Support_EC_Port80_Debug
extern void EC_OUT_PORT80(BYTE bDate);
#endif //Support_EC_Port80_Debug
extern void FnKeyWakeS3(void);


#endif
