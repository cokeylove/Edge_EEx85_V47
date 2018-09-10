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

#ifndef OEM_POWERSEQ_H
#define OEM_POWERSEQ_H

extern void CheckResetSource(void);
extern void Oem_SysPowerContrl(void);
extern void Oem_S5S0Sequence(void);
extern void Oem_S0S5Sequence(void);
extern void Oem_S3S0Sequence(void);
extern void Oem_S0S3Sequence(void);
extern void CheckAutoPowerOn(void);
extern void CHK_BATT_5PERCL(void);
extern void S34toDSx(void);
extern void DSxtoS34(void);
extern void S0S3_VR_ON(void);

extern void Check_EC_ON();
extern void Oem_PowerSequence(BYTE SxSeq);
extern void Oem_hook_FnEquIOFunc(void);


extern void EnableS3Int(void);
extern void EnableS4Int(void);

extern  void InitPMICReg(void);
extern	BYTE CheckPMIC(void);
extern	BYTE InitPMIC(void);
extern	void PollingPMICReg(void);
extern  void InitPMICParameter(void);

typedef struct PowerSEQ
{
  FUNCT_PTR_B_V  func;  // Do step function
  WORD  delay;          // delay time (ms)
  BYTE  checkstatus;    // step function is check pin status (EX: slp_s3 HWPG ...)
} sPowerSEQ;


typedef struct _PowerSEQTableList {
  sPowerSEQ       *PowerSEQTable;
  BYTE            PowerSEQCount;
} PowerSEQTableList;

enum {
  POWERSEQS5toS0 = 0,  // 0 -- S5toS0
  POWERSEQS0toS5,      // 1 -- S0toS5
  POWERSEQS3toS0,      // 2 -- S3toS0
  POWERSEQS0toS3       // 3 -- S0toS3
};

typedef struct PMICStruct
{
	BYTE  	cmd;
    XBYTE 	*smbdataA;
}sPMICStruct;

//address
#define PMICADDRESS 	0x68    
//command
#define VENDODID 		0x00   //default: 0x67
#define REVID 			0x01  //default: ---
#define SDWN 			0x49  //default: 0x00
#define EN_RW			0xD1  //default: 0xBF
#define EN_MASK			0xD2  //default: 0x00
#define EN_DEGL_SEL1		0xD3  //default: 0x00
#define EN_DEGL_SEL2        0xD4  //default: 0x00
#define PG_STATE 			0xD5  //default: 0x00
#define OVP_REG			0xD6  //default: 0x00
#define UVP_REG			0xD7  //default: 0x00
#define OCP_REG			0xD8  //default: 0x00
#define OTP_REG			0xD9  //default: 0x00
#define BUCK1CTRL		0xDA  //default: 0x24
#define BUCK2CTRL		0xDB  //default: 0x04
#define BUCK3CTRL		0xDC  //default: 0x32
#define LDO1CTRL			0xDD  //default: 0x1A
#define LDO2CTRL			0xDE  //default: 0x03
#define DISCHCTRL1		0xDF  //default: 0x14
#define DISCHCTRL2		0xE0  //default: 0x00
#define OC_CTRL			0xE1  //default: 0x26
#define BUCK_FSW		0xE2  //default: 0x01

extern void Lowpower_ENfunction(void);
extern void Lowpower_DISfunction(void);	

#endif
