/*-----------------------------------------------------------------------------
 * TITLE: OEM_INIT.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// Oem_StartUp
//----------------------------------------------------------------------------
void Oem_StartUp(void)
{

}

//----------------------------------------------------------------------------
// The function of init. registers
//----------------------------------------------------------------------------
//const struct REG_INIT_DEF code reg_init_table[] =
const sREG_INIT_DEF code reg_init_table[] =
{
// ADC
      { &ADCSTS      ,AINITB             },   // Enable Analog accuracy initialization
        //{ &ADCSTS      ,0                   },   // Stop adc accuracy initialization
        { &ADCSTS      ,0x80              },   // Stop adc accuracy initialization
    { &ADCCFG      ,0x21               },   //
        { &KDCTL       ,AHCE                },   // Enable Hardware Callibration
//    { &ADCCFG      ,DFILEN          },  // Enable Digital Filter enable,
    { &ADCCTL    ,0x15        },

// DAC
        { &DACPWRDN    ,0x00                },   //

//PWM
    { &ZTIER    ,0x00        },  // Disable
    { &C0CPRS    ,0x23        },  // Channel 0 Clock Prescaler Register
    { &CTR      ,0xFF        },  // Cycle Time 0
    { &DCR0      ,0x00        },  // Channel 0 PWM Duty Cycle
    { &DCR1      ,0x00        },  // Channel 1 PWM Duty Cycle
    { &DCR2      ,0x00        },  // Channel 2 PWM Duty Cycle
    { &DCR3      ,0x00        },  // Channel 3 PWM Duty Cycle
    { &DCR4      ,0x00        },  // Channel 4 PWM Duty Cycle
    { &DCR5      ,0x00        },  // Channel 5 PWM Duty Cycle
    //{ &DCR6    ,0x00        },  // Channel 6 PWM Duty Cycle
    { &DCR7      ,0x00        },  // Channel 7 PWM Duty Cycle
//[-start-131205-T130124_5-modify]//
    { &PWMPOL    ,PWM_Polarity},  // PWM Polarity
//[-end-131205-T130124_5-modify]//
    { &PCFSR    ,0x05        },  // bit7=1: Disable CR256 channel 1 output. bit0-bit3: 0 => select 32.768 kHz, 1=> select EC clock frequency
    { &PCSSGL    ,0x14        },  // Select channel 0-3 Group
    { &PCSSGH    ,0xCA        },  // Select channel 4-7 Group
    { &CR256PCSSG  ,0x80        },  // CR256 Prescaler Clock Source Select Group
    { &PCSGR    ,0x00        },  // Prescaler Clock Source Gating Register
    { &C4CPRS    ,0xE3        },  // Channel 4 Clock Prescaler Register
    { &C4MCPRS    ,0x00        },  // Channel 4 Clock Prescaler MSB Register
    { &C6CPRS    ,0x01        },  // 23KHz Channel 6 Clock Prescaler Register for fan
    { &C6MCPRS    ,0x00        },  // 23KHz Channel 6 Clock Prescaler MSB Register
    { &C7CPRS    ,0x30        },  // Channel 7 Clock Prescaler Register
    { &C7MCPRS    ,0x00        },  // Channel 7 Clock Prescaler MSB Register
    { &CLK6MSEL    ,0x10        },  // PWM Clock 6MHz Select Register
    { &CTR1      ,0xFF        },  // Cycle Time 1
    { &CTR2      ,0xC8        },  // 23KHz Cycle Time 2(Fan used)
    { &CTR3      ,0xFF        },  // Cycle Time 3(Panel used)
    { &PWMODENR    ,0x00        },  // PWM output open-drain
    { &TSWCTRL    ,0x0A        },  // Enable TACH0A,TACH1A speed.
    { &ZTIER    ,0x02        },  // Enable
    { &GCR15     ,0xC0        },  // Fix the issue of black screen when plug out 80port.
};

void Init_Regs(void)
{
  BYTE index = 0x00;
    while(index < (sizeof(reg_init_table)/sizeof(sREG_INIT_DEF)))
    {
        Tmp_XPntr = reg_init_table[index].address;
    *Tmp_XPntr = reg_init_table[index].initdata;
        index ++;
    }
}


//for initial PWM setting
const sREG_INIT_DEF code Init_SMBus_table[] =
{
// SMBus
  { &SMB4P7USL    ,SMB4P7USL_REG       },  // 100K  (AC 20130313)
  { &SMB4P0USH    ,SMB4P0USH_REG       },
  { &SMB300NS     ,SMB300NS_REG        },
  { &SMB250NS     ,SMB250NS_REG        },
  { &SMB25MS      ,SMB25MS_REG         },
  { &SMB45P3USL   ,SMB45P3USL_REG      },
  { &SMBS4P3USH   ,SMBS4P3USH_REG      },
  { &SMB4P7A4P0H  ,SMB4P7A4P0H_REG     },

  { &HOCTL2_A     ,HOCTL2_A_REG        },
  { &HOCTL_A      ,HOCTL_A_REG | BIT1  },
  { &HOCTL_A      ,HOCTL_A_REG         },
  { &HOSTA_A      ,HOSTA_A_REG         },

  { &HOCTL2_B     ,HOCTL2_B_REG        },
  { &HOCTL_B      ,HOCTL_B_REG | BIT1  },
  { &HOCTL_B      ,HOCTL_B_REG         },
  { &HOSTA_B      ,HOSTA_B_REG         },

  { &HOCTL2_C     ,HOCTL2_C_REG        },
  { &HOCTL_C      ,HOCTL_C_REG | BIT1  },
  { &HOCTL_C      ,HOCTL_C_REG         },
  { &HOSTA_C      ,HOSTA_C_REG         },
  
  { &SCLKTS_A     ,SCLKTS_A_REG        },
  { &SCLKTS_B     ,SCLKTS_B_REG        },
  { &SCLKTS_C     ,SCLKTS_C_REG        },
  { &SCLKTS_D     ,SCLKTS_D_REG        },
};
//ITE Smart update for initial SMBUS
void Init_SMBus_Regs(void)
{
  BYTE index = 0x00;
    while(index < (sizeof(Init_SMBus_table)/sizeof(sREG_INIT_DEF)))
    {
        Tmp_XPntr = Init_SMBus_table[index].address;
    *Tmp_XPntr = Init_SMBus_table[index].initdata;
        index ++;
    }
}


//----------------------------------------------------------------------------
// Oem_Initialization - Initialize the registers, data variables, and oem
// functions after kernel init. function.
//----------------------------------------------------------------------------
void Oem_Initialization(void)
{
  BYTE Temp8;

#if Auto_Detect_ITE_chip
  Auto_Setting_ITE_CHIP();
#endif //Auto_Init_ITE_CHIP  

  if( (pPWRRSTMark1==0x78) && (pPWRRSTMark2==0x87) )
  {
    pPWRRSTCnt = 1;
    OTP_RESET_HI();
  }
#ifdef Oem_Initialization_hook_Support
  Oem_Initialization_hook();
#endif
  CheckKSO1617Support();
  Init_Regs();
  Init_SMBus_Regs();  // initial SMBUS
//8586 BX test  ChangePLLFrequency(0x07);
//  ChangePLLFrequency(0x03);   // Mask for only DC mode entry to S4 cannot bring up 
  InitSysMemory2ECRam();    // H2RAM function
  Init_ADC();
#if (PLATFORM_CHIP == INTEL)
  Init_PECI();
#endif // PLATFORM_CHIP
#if !Support_SMTADP
  SetPowerBatteryparameter();
#endif // !Support_SMTADP
  Init_VC(3);  // Init All VC Channel.  // CMW 20121022
  LID_readytime = 11;
#if Support_RFID
	CacheDma(1,FSCEHighLevel);
  DisableAllInterrupt();
  eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,0xFB,0xFF);  //
  if(eEEPROMData != 0xFF)
  {
  	if(eEEPROMData == 0x03)
  	{
			Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N5,0x00);
			Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N1,0x00);
  		Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N2,0x00);
			Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_index,0x00);
  	}
  	if(eEEPROMData == 0x01)
  	{
  		Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N2,0x00);
		Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_index,0x00);
  	}
  }
  eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,0xDF,0xFF);  //
  if(eEEPROMData != 0xFF)
  {
  	if(eEEPROMData == 0x01)
  	{
  		Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N7,0x00);
	  Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_index2,0x00);
  	}
  }
  SCRA2H = 3;
  EnableAllInterrupt();
	initRFIDProtect();	
#endif //Support_RFID	
//  uMBID = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7 ,EEPROM_MBID);
#ifdef OEM_Hook_Power_90WSystem_support
  OEM_Hook_Power_90WSystem();
#endif
  pWorkAround = 0x01;
  Edge_Initial();
  Gsensor_Initial();
  Battery_Initial();
#if (CHARGER_IC == BQ25700)
  ChargerIC_Initial();
#endif
   if( WinFlashMark == exitFollowMode )	
  {
    WinFlashMark = 0x00;
  }
  AC_DROP_COUNT = 0x00;

  Temp8 = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_DevStus);
  if(IS_MASK_SET(Temp8,EEPROM_DevStus_USB))
  { USB_PWREn; }
  else
  { USB_PWRDis;}

#if chPWSW1WDT
  OEM_PWRSWWDTEn();	
#endif

#if KB_Primary_Key
   SET_MASK(u4EcCapability1, Fn_primary);
#endif 
   pLastLPCstatusSET;

#if Support_TYPE_C
		if(Read_DCIN_ATTACHED2())
		{
			SET_MASK(TypeCStatus,OldDcinAttached2);
		}
		else
		{
			CLEAR_MASK(TypeCStatus,OldDcinAttached2);
		}
    OEM_TYPE_C_init_hook();
#endif //Support_TYPE_C	

	ADPI_MUL = ClassicAdaptor_ADPI_MUL;	//Default setting the ADPI_MUL is 20V.
		  	
#if (PLATFORM_CHIP == INTEL)	
	Ccb42_States = 0x47;
#endif

#if Support_ADC_BoardID
  ScanADC_BoardID();
#endif

#if Support_EnhancedPcBeep
  OEM_ENHANCED_PC_BEEP_Init_HOOK();
#endif   //Support_EnhancedPcBeep

#if Support_UCSI 
	UcsiInit();
#endif
 WlanWakeStatusEEPROM = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_ACWOL_STATUS);
EventMask1=(WlanWakeStatusEEPROM&0X02);
EventMask0=(WlanWakeStatusEEPROM&0X01);


  SET_MASK(SYS_MISC1,G3Mode);
}
//----------------------------------------------------------------------------
// The function of clearing external ram for OEM code
// Clear external ram (0x100~0xFFF)
//----------------------------------------------------------------------------
void Init_ClearRam(void)
{
  PORT_BYTE_PNTR byte_register_pntr;
  BYTE temp0,temp1;

  temp0 = pPWRRSTMark1;
  temp1 = pPWRRSTMark2;
  byte_register_pntr=0x100;
  while(byte_register_pntr<0x1000)// Clear external ram (0x100~0xFFF)
  {
      *byte_register_pntr=0;
      byte_register_pntr ++;
  }

  byte_register_pntr=0x2200;
  while(byte_register_pntr<0x2280)// Clear Batt ram BANK0 (0x2200~0x227F)
  {
      *byte_register_pntr=0;
      byte_register_pntr ++;
  }

  pPWRRSTMark1 = temp0;
  pPWRRSTMark2 = temp1;
}

void Clear_RAM_Range(WORD nAddr, WORD nSize)
{
    Tmp_XPntr = (unsigned int*)nAddr;
    for( ITempW01=0; ITempW01<=nSize; ITempW01++ )
    {
        *Tmp_XPntr = 0;
        Tmp_XPntr++;
    }
}

void RAM_travel(WORD sour, WORD targ, BYTE count)
{
    Tmp_XPntr = (unsigned int*)sour;
    Tmp_XPntr1 = (unsigned int*)targ;
    while(count) {
      *Tmp_XPntr1++ = *Tmp_XPntr++;
      count--;
    };
}


//----------------------------------------------------------------------------
// The function of DMA for scratch sram
//----------------------------------------------------------------------------
const sDMAScratchSRAM code asDMAScratchSRAM[]=
{
  {  &SCRA1H,  &SCRA1M,  &SCRA1L  },    // 1024 bytes (externl ram 0x800 ~ 0xBFF)
  {  &SCRA2H,  &SCRA2M,  &SCRA2L  },    // 512 bytes  (externl ram 0xC00 ~ 0xDFF)
  {  &SCRA3H,  &SCRA3M,  &SCRA3L  },    // 256 bytes  (externl ram 0xE00 ~ 0xEFF)
  {  &SCRA4H,  &SCRA4M,  &SCRA4L  }    // 256 bytes  (externl ram 0xF00 ~ 0xFFF)

};
void  CacheDma(BYTE sramnum,WORD addr)
{
  *asDMAScratchSRAM[sramnum].scarh = 0x80;
  *asDMAScratchSRAM[sramnum].scarm = (WORD) addr >> 8;     // high byte of function address
  *asDMAScratchSRAM[sramnum].scarl = (WORD) addr & 0xff;  // low byte of function address
  *asDMAScratchSRAM[sramnum].scarh = 0x00;  // start cache dma
}



//----------------------------------------------------------------------------
// Hook_ECExitFollowMode
//----------------------------------------------------------------------------
void Hook_ECExitFollowMode(void)
{
  CLEAR_MASK(nStopChgStat3H, ECFLASH);
}

//----------------------------------------------------------------------------
// EC leaves follow mode or EC scatch ROM and jump to main function
//  Note:
//      1. Before jump to main function if want to keep EC setting [return(0x33);] is necessary.
//      2. If you don't understand the use of Hook_ECRetunrMainFuncKeepCondition function,
//          don't change anything.
//      3. Always at bank0 (0x9000)
//----------------------------------------------------------------------------
BYTE Hook_ECRetunrMainFuncKeepCondition(void)
{
    if((WinFlashMark==exitFlashMark1) && (WinFlashMark2==exitFlashMark2))
    {
        return(0x33);
    }
    else
    {
        return(0x00);
    }
}

BYTE Check_Mirror_Occurs(void)
{
#if Support_Mirror_Always_EN
    if(IS_MASK_SET(FLHCTRL3R,BIT7))
	{
		SET_MASK(FLHCTRL3R,BIT7);
		//SET_MASK(FLHCTRL3R,BIT0); //BROOKEW033:Enable iTE8886 Mirror function
		SET_MASK(HINSTC1,BIT6);
	}
    return(1);
#else
  if(IS_MASK_SET(FLHCTRL3R,BIT7))
  {
    return(1);
  }
  return(0);
#endif 
}
#if chPWSW1WDT
void OEM_PWRSWWDTEn()
{
	// Disable PWRSW WDT Timer2
	CLEAR_MASK(GCR8,PWSW2EN);

	// Enable PWRSW WDT Timer1
	GCR11 |= PWSW7s;				// set PWSW WDT Timer to 7 sec.
	GCR9  |= (PWSW1EN1|PWSW1EN2);
}

void OEM_RESETPWSWTIMER1()
{
	SET_MASK(GCR9,PWSW1EN2);
}
#endif


#if Auto_Detect_ITE_chip
void Auto_Setting_ITE_CHIP(void)
{
   XWORD temp16;
   temp16 = (ECHIPID1 << 8) + ECHIPID2;
   if ((temp16 == IT8586) && (RSTS != IT8586_Init_RSTS)){
    RSTS = IT8586_Init_RSTS;
   } else if ((temp16 == IT8986) && (RSTS != IT8986_Init_RSTS)){ 
    RSTS = IT8986_Init_RSTS;
   }
}
#endif //Auto_detect_ITEEC_chip
