/*----------------------------------------------------------------------------
 * Filename: OEM_Power.C  For Chipset: ITE.IT85XX
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

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

// ----------------------------------------------------------------------------
// To read variable [RSTStatus] to get EC reset source
// ----------------------------------------------------------------------------
void CheckResetSource(void)
{

}

void Charge_inhibit2sec(void)
{
  inhibit2sec = 20;    // 100ms*20 = 2000ms
  SET_MASK(nStopChgStat3L,ENCHARGESUSP);    // use for detect battery charging suspend
  CHGIC_ptr = 2;
  WriteSmartChgIC();
  if(SysPowState == SYSTEM_S3_S0){
    SET_MASK(pProject3,uReSendBat1ChgQevent);
  }
}

void CHK_PLT_RST(void)
{
  if(Read_PLT_RST())
  {
    if(Read_TPM_PLT_RST())
      TPM_PLT_RST_HI();
  }
  else
  {
   if(!Read_TPM_PLT_RST())
      TPM_PLT_RST_LOW();
  }    
}
void CHK_DRAMRST_CNTRL(void)
{

}

#if PMIC_SUPPORT
const sPMICStruct code powerInit[] =
{//     command               REG                      		no.
	
	{ SDWN,			&PowerSDWN			},
	{ EN_MASK,		&PowerEN_MASK		},
	{ EN_DEGL_SEL1,		&PowerEN_DEGL_SEL1},
	{ EN_DEGL_SEL2,		&PowerEN_DEGL_SEL2},
	//{ EN_RW,			&PowerEN_RW		},	
	{ BUCK1CTRL,		&PowerBUCK1CTRL	},
	{ BUCK2CTRL,		&PowerBUCK2CTRL	},
	{ BUCK3CTRL,		&PowerBUCK3CTRL	},
	{ LDO1CTRL,		&PowerLDO1CTRL		},
	{ LDO2CTRL,		&PowerLDO2CTRL		},
	{ DISCHCTRL1,		&PowerDISCHCTRL1	},
	{ DISCHCTRL2,		&PowerDISCHCTRL2	},
	{ OC_CTRL,		&PowerOC_CTRL		},
	{ BUCK_FSW,		&PowerBUCK_FSW	},
//	{ EN_RW,			&PowerEN_RW		},
};
void InitPMICReg(void)
{
	PowerInitIndex=0;
	PowerSDWN = 0x00;
	PowerEN_MASK = 0x80; //
	PowerEN_RW=0xBF;  //
	
	PowerEN_DEGL_SEL1 = 0x09;
  PowerEN_DEGL_SEL2 = 0x08;

	PowerBUCK1CTRL = 0x26; 
	PowerBUCK2CTRL = 0x06;
	PowerBUCK3CTRL = 0x20; 
	PowerLDO1CTRL = 0x1E;
	PowerLDO2CTRL = 0x02; //
	PowerDISCHCTRL1 = 0x15; 
	PowerDISCHCTRL2 = 0x05;
	PowerOC_CTRL = 0x25; 
	PowerBUCK_FSW = 0x00; //
}
void InitPMICParameter(void)
{
	PowerInitOK = 0;
}

BYTE CheckPMIC(void) 
{
  XBYTE temp=0, return_flag=0;
  XBYTE write_smbus_fail_retry = 0,read_smbus_fail_retry=0,read_notequal_write_retry=0; //JIMAPLO028
  PowerInitError=0;
  PowerReturnData = 0xFE;
  if(PowerInitOK&PMIC_Check_Result_Flag) 
  {
    return_flag = 1;
  }
  else if(IS_MASK_SET(PowerInitOK,PMIC_SMBUS_Retry_Fail_Flag)) 
  	   {
  	      return_flag = 0;
  	   }
       else{
              while(PowerInitError < 4){ //JIMAPLO028 update
              if(!bRWSMBus(SMbusCh3, SMbusWB, PMICADDRESS, BUCK_FSW, &PowerBUCK_FSW, SMBus_NoPEC))
		      {
			     //JIMAPLO028 PowerInitError++;
			     write_smbus_fail_retry++; //JIMAPLO028
			     ResetSMBus(SMbusCh3); 
			     //JIMAPLO08 add start
			     if(write_smbus_fail_retry >=3 ){   //JIMAPLO028 
			        PowerInitError++;
			 	    write_smbus_fail_retry = 0;    //JIMAPLO028
			 	    SET_MASK(PowerInitOK,PMIC_SMBUS_Retry_Fail_Flag); 
			 	 }
			     //JIMAPLO028 add end 
		}
       else{
	   	    write_smbus_fail_retry = 0;
	   		if(!bRWSMBus(SMbusCh3, SMbusRB, PMICADDRESS, BUCK_FSW, &PowerReturnData,SMBus_NoPEC))
				{
				read_smbus_fail_retry++;
				ResetSMBus(SMbusCh3);
				//JIMAPLO08 add start
				 if(read_smbus_fail_retry >=3 ){
				 	PowerInitError++;
			 		read_smbus_fail_retry = 0;
			 		 SET_MASK(PowerInitOK,PMIC_SMBUS_Retry_Fail_Flag); 
			 		}
			//JIMAPLO028 add end 
			
		   	 	} 
	     	 else{
			 	   read_smbus_fail_retry =0;
			 		if(PowerReturnData!=PowerBUCK_FSW)
		        	 {
  						read_notequal_write_retry++;
		      
							if(read_notequal_write_retry>=3)
							{
						 		read_notequal_write_retry = 0;
								PowerInitError++;
		 						CLEAR_MASK(PowerInitOK,PMIC_Check_Result_Flag);
								SET_MASK(PowerInitOK,PMIC_SMBUS_Retry_Fail_Flag); 
		 						
							}					 		          	 	
		         	 }
		  			else
		  				{
		  				PowerInitError = 0;
				  		 PowerInitIndex = 0;
				  		 SET_MASK(PowerInitOK,PMIC_Check_Result_Flag);
				   		 RamDebug(0x0E);
			       		return_flag = 1;
						 break;
		  				}
	     	 	}
       		}
  	}
  }
  return return_flag;//JIMPALO01E: need return value 
}

BYTE InitPMIC(void)
{
  XBYTE return_flag =0;
  if(PowerInitOK&PMIC_Intial_Result_Flag) return_flag = 1;
   else if(IS_MASK_SET(PowerInitOK,PMIC_SMBUS_Retry_Fail_Flag)) return_flag = 0;
  else{
  if(!bRWSMBus(SMbusCh3, SMbusWB, PMICADDRESS, powerInit[PowerInitIndex].cmd, powerInit[PowerInitIndex].smbdataA,SMBus_NoPEC))
  	{
  	  PowerInitError++;
	  //JIMAPLO007  RamDebug(0xFC);
	   if(PowerInitError>=3){
	   	SET_MASK(PowerInitOK,BIT7);
        CLEAR_MASK(PowerInitOK,PMIC_Intial_Result_Flag);
		SET_MASK(PowerInitOK,PMIC_SMBUS_Retry_Fail_Flag);
	   	}
	   ResetSMBus(SMbusCh3);
  	}else{
  	      PowerInitIndex++;
  	     if(PowerInitIndex >=(sizeof(powerInit)/sizeof(sPMICStruct))){
	 	     SET_MASK(PowerInitOK,PMIC_Intial_Result_Flag);
			//JIMAPLO007  RamDebug(0xFB);
			 PowerInitIndex = 0;
			RamDebug(0x0F);
			return_flag = 1;
  	 	   }
		//JIMAPLO007    RamDebug(0xFD);  	      
  		}
  	}
  return return_flag;
} 
const sPMICStruct code pollingPMICTable[] =
{
 { OVP_REG,&PowerOVP_REG},
 { UVP_REG,&PowerUVP_REG},
 { OCP_REG,&PowerOCP_REG},
 { OTP_REG,&PowerOTP_REG}
  
};
void PollingPMICReg(void)
{
		if(!bRWSMBus(SMbusCh3, SMbusRB, PMICADDRESS, pollingPMICTable[PowerInitIndex].cmd, pollingPMICTable[PowerInitIndex].smbdataA,SMBus_NoPEC))
		{
			PowerInitError++;
			if(PowerInitError>=3)
				{
				 SET_MASK(PowerInitOK,PMIC_Polling_Retry_Fail_Flag);
				 CLEAR_MASK(PowerInitOK,PMIC_Polling_Result_Flag);
				 SET_MASK(PowerInitOK,PMIC_Polling_End_Flag);
				 PowerInitError = 0;
				 PowerInitIndex = 0;
				}
			ResetSMBus(SMbusCh3); 
		}
		else {
		      PowerInitIndex++;
			  if(PowerInitIndex>=(sizeof(pollingPMICTable)/sizeof(sPMICStruct))){
			  	 SET_MASK(PowerInitOK,PMIC_Polling_Result_Flag);
				 CLEAR_MASK(PowerInitOK,PMIC_Polling_Start_Flag);
				 SET_MASK(PowerInitOK,PMIC_Polling_End_Flag);
				 PowerInitIndex = 0;//JIMAPLO023
			  	}			  	
			 }
	if(IS_MASK_SET(PowerInitOK,PMIC_Polling_End_Flag)&&(PowerOVP_REG||PowerUVP_REG||PowerOCP_REG||PowerOTP_REG))
		{
			ProcessSID(PMIC_ID);
			if(PowerOTP_REG)
				SET_MASK(OTP_TEMP,b0OTP_Occurr_Flag);
		}
}  
#endif //PMIC_SUPPORT
void DSxtoS34(void)
{
#if (SUPPORT_DSx)
  if ((DSxPowState == SYSTEM_DSxOK)&&IS_MASK_SET(SYS_STATUS,AC_ADP))
  {
    //if (Read_PM_SLP_SUS())
    {
      SxS5_Record = 0x00;
      EC_ON_HI();
      EC_ON2_3V_HI();
      Delay1MS(10);  //for DS3 test
      if(CheckPMIC())
      {
        SxS5_Record = 0x02;
        if(!InitPMIC())
        {
          SxS5_Record = 0x03;
          return;
        }
          
      }
      else
      {
        SxS5_Record = 0x01;
        return;
      }
      Delay1MS(10);
      EC_ON3_VDDP_HI();
      if(Read_0P9VALW_PWRGD())
      {
        SxS5_Record = 0x05;
      }
      else
      {
         SxS5_Record = 0x06;
         return;
      }
      //PCH_PWR_EN_ON();
      PM_PWRBTN_HI();          // CAC. 2013.05.22
      if(ACPOWER_ON)
      {
  	    AC_PRESENT_HI();
      }
      EC_WAKE_OUT_HI();
      Delay1MS(20);
      GPOTF = 0x00;          // CAC. 2013.05.30
      RSMRST_HI();
//      USB_ON_OUTPUT;
//      USB_ON_LOW();
      SxS5_Record = 0xFF;
      DSxPowState = SYSTEM_S3S4;
    }
  }
#endif  
}

void S34toDSx(void)
{
#if (SUPPORT_DSx)
  if((DSxPowState == SYSTEM_S3S4)&&IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
  {
    //if (!Read_PM_SLP_SUS())
    {
      InitPMICParameter();
      Delay1MS(2);
      //PCH_PWR_EN_OFF();
      GPOTF = 0x02;          // CAC. 2013.05.30 enable open drain

      RSMRST_LOW();
      Delay1MS(5);
      //PCH_PWR_EN_OFF();
      EC_ON3_VDDP_LOW();
      Delay1MS(5);
      EC_WAKE_OUT_LOW();
      CLEAR_MASK(SysStatus,LidStatus);
      //RSMRST_LOW();
      PM_PWRBTN_LOW();
      if ( ((!USB_PWRGET) || IS_MASK_SET(USB_Charger, b2USB_TmlDis))
          && IS_MASK_CLEAR(SYS_STATUS,AC_ADP)
          ){
        EC_ON_LOW();
      }
      EC_ON2_3V_LOW();
      AC_PRESENT_LOW();
//      GPOTF = 0x00;          // CAC. 2013.07.15
//      USB_ON_INPUT;  // Disable USB wake.
      DSxPowState = SYSTEM_DSxOK;
    }
  }
#endif  
}

//-----------------------------------------------------------------------------
// Handle System Power Control
// Called by service_1mS -> Timer1msEvent -> Hook_Timer1msEvent
//-----------------------------------------------------------------------------
void Oem_SysPowerContrl(void)
{
  //CHK_SUSACK();
  //CHK_DRAMRST_CNTRL();
  //CHK_SLP_A_CNTRL();
  CHK_PLT_RST();

  // Check current System PowerState to see what should we do.
  switch (SysPowState)
  {
    case SYSTEM_S0:
      if(!Read_SLPS3())
      {
      EC_MUTE_ACT();
        if (S3S4DelayCnt > 10)
        {
          PWSeqStep = 1;
          PowSeqDelay = 00;
          SysPowState=SYSTEM_S0_S3;
          //S0S3_VR_ON(); //Ex85: AMD need not turn off VR_ON fast;
          if (!Read_SLPS4())
          {
            PWSeqStep = 1;
            PowSeqDelay = 0x00;
            SysPowState=SYSTEM_S0_S5;
          }
          if (IS_MASK_SET(SysStatus,CloseLid))
          {
            CLEAR_MASK(SysStatus,CloseLid);
          }
        }
        else
        {  S3S4DelayCnt++;  }
      }
      else
      {  S3S4DelayCnt = 0;    }
      Check_LPC_Reset();
      OEM_SYSTEM_S0();
    break;

    case SYSTEM_S3:
      OEM_SYSTEM_S3();
      //S34toDSx();
      //DSxtoS34();

      if(Read_SLPS3())  //S3 to S0
      {
        PWSeqStep = 3;
        PowSeqDelay = 20;
        SysPowState = SYSTEM_S3_S0;
      }
    break;

    case SYSTEM_S4:
    case SYSTEM_S5:
#if !EN_PwrSeqTest       
    	if( IS_MASK_SET(PWRCONTROL,MAINPWRONTrig) )
	    {
	       RSMRST_LOW();
	       CLEAR_MASK(PWRCONTROL,MAINPWRONTrig);
      }
#endif

      if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut) )
      { S34toDSx(); }
      
      DSxtoS34(); 

      if(Read_SLPS3() && Read_SLPS4()) 
      {
        PWSeqStep = 1;
        PowSeqDelay = 0x00;
        SysPowState=SYSTEM_S5_S0;
        RamDebug(0x56);
      }
      CheckAutoPowerOn();
      if ( (pWorkAround == 0x01)
        && (IS_MASK_CLEAR(cCmd,cC_PCHPWR_Keep))
        && (!WOLDCWakeEnGet)
        )
      {
        pWorkAround = 0x00;
        Check_EC_ON();
      }
      OEM_SYSTEM_S5();      
    break;

    case SYSTEM_S5_S0:
#if (PLATFORM_CHIP == INTEL)
      if ( EC_MainPwr() )
      {
        Oem_PowerSequence(POWERSEQS5toS0);      
        mPWRDelay=0;
      }
      else
      {
        mPWRDelay++;
        if( mPWRDelay > 50 )
        {
          PWSeqStep = 1;
          PowSeqDelay = 0x00;
          SysPowState=SYSTEM_S0_S5;
        }
      }
#else
    Oem_PowerSequence(POWERSEQS5toS0);      
    mPWRDelay=0;
#endif

    break;

    case SYSTEM_S3_S0:
      Oem_PowerSequence(POWERSEQS3toS0);
    break;

    case SYSTEM_S0_S3:
      Oem_PowerSequence(POWERSEQS0toS3);
    break;

    case SYSTEM_S0_S4:
    break;

    case SYSTEM_S0_S5:
      Oem_PowerSequence(POWERSEQS0toS5);
    break;

    case SYSTEM_EC_WDTRST:
    break;

    default:
      #if (SUPPORT_DSx)
      RamDebug(0x64);
      DSxPowState = SYSTEM_DSxOK; 
      #endif
      SysPowState = SYSTEM_S5;
    break;
  }
}

void SetS0GPIO(void)
{

//  VM_PWRON_ON();

  CLEAR_MASK(S0S5PWRcodition,S0S5PWROFF);
  SCI_HI();          //RUNSCI Hi
#if Support_GATEA20
  GATEA20_HI();      //GA20 Hi
#endif 
  KBRST_OFF();

#if SW_ISCT
  if(IS_MASK_CLEAR(AOAC_STATUS, ISCT_S0_ISCT))
  {
    AOAC_STATUS = 0xC0;    // Clear ISCT status.
    ISCT_Timer = 0;
  }
#endif // SW_ISCT  
  AOAC_PWRON();      // Turn on WLAN power.

#if Support_EC_GS_ON
    EC_GS_ON();
#endif // Support_EC_GS_ON

  TP_CLK_ALT;          // TP_CLK to alternate pin
  TP_DATA_ALT;         // TP_DATA to alternate pin
  SMBus3_CLK_ALT;      // SMBus3 CLK to alternate pin
  SMBus3_DAT_ALT;      // SMBus3 DATA to alternate pin

#if FAN_TABLE_Already
  FAN_PWM_ALT;      // Set FAN_PWM Alt.
#else
  FAN_PWM_OUT;      // Set FAN_PWM OUTPUT.
  EC_FAN_PWM_HI();
#endif  // FAN_TABLE_Already

  FAN_SPEED_ALT;      // Set FAN Speed Alt.
  InitSio();

  KBHISR = 0x00;
  KBHIKDOR = 0x00;
  CLEAR_MASK(KBHISR,SYSF);

  SPCTRL1 = 0x83;      //msmart test
  BRAM3E =0;
  BRAM3F =0;

  PECIDelayCnt = 0x04;

  Charge_inhibit2sec();
  CLEAR_MASK(ERR_THMSTS,b0ThmICError);  // Clear bit0.
  TMErrCnt = 0;              // Clear error count.  
  CLEAR_MASK(ERR_THMSTS,b4PCHThmError);  // Clear bit4.
  PCHErrCnt = 0;              // Clear error count.
  VGAErrCnt = 00; // Clear error count.
  CLEAR_MASK(SysStatus2, b4VGATemp_Init); // Restart initial GPU temperature.
  CLEAR_MASK(ERR_THMSTS,b1VGATempEr); 	// Clear Error states.
  //Clear PORT80 dump log
  for (Port80_Index =0;Port80_Index <=14;Port80_Index++)
  {
	Port80_Data[Port80_Index] = 0;
  }
  Port80_Index = 0;
}

void SetS5GPIO(void)
{
  SCI_LOW();      //RUNSCI Hi
#if Support_GATEA20
  GATEA20_LOW();    //GA20 Hi
#endif  
  KBRST_ON();

  TP_CLK_INDW;    // TP_CLK to Input+pull_DW pin
  TP_DATA_INDW;    // TP_DATA to Input+pull_DW pin
  SMBus3_CLK_INDW;  // SMBus3 CLK to Input+pull_DW pin
  SMBus3_DAT_INDW;  // SMBus3 DATA to Input+pull_DW pin
  FAN_PWM_INDW;    // Set FAN_PWM input+pull_dw.
  FAN_SPEED_INDW;    // Set FAN Speed input+pull_dw.


#if SW_ISCT
  if ( (IS_MASK_CLEAR(AOAC_STATUS, ISCT_Enable))||(SysPowState != SYSTEM_S0_S3) )  // Check ISCT enable?
  {
    AOAC_PWROFF();    // Turn off WLAN power.
#if Support_WLAN_ON_OFF_control    
    BTRF_OFF_OFF();
#endif //Support_WLAN_ON_OFF_control
  }
#else
  if (((!WOLACWakeEnGet)||((!WOLDCWakeEnGet)&& IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))||(SysPowState != SYSTEM_S0_S3) )
  {
    AOAC_PWROFF();	// Turn off WLAN power.
#if Support_WLAN_ON_OFF_control    
    BTRF_OFF_OFF();
#endif //Support_WLAN_ON_OFF_control
  }
#endif // SW_ISCT
  APSHDDFLAG = 0;
#if Support_EC_GS_ON
  EC_GS_OFF();
#endif

#if !Support_AOU5_V1_2
  if ( SysPowState != SYSTEM_S0_S3 )
  {
    //[Project][CC20130508][Start] Modify power behave on Non-AOU USB port.
    if((Read_AC_IN())&&(USB_PWRGET))
    {
      USB_ON_OUTPUT;
      USB_ON_LOW();
    } // Turn on USB power.
    else
    {
      USB_ON_INPUT;
    } // turn off USB power.
  }
  else
  {
    if( ((TurnOnUSBGET) || (USB_PWRGET))&&((Read_AC_IN())||(RelativeStateOfChgL > NO_USBDevice_BAT_LOW)))
    {
      USB_ON_OUTPUT;
      USB_ON_LOW();  // Turn on USB power.
    }
    else
    {
      USB_ON_INPUT;
    } // turn off USB power.
  }
#endif

  AC_IN_INPUT;
  if( nSwitchableBTNGET )
  {
    SET_MASK(pProject0,b7SWITCHBAK);
  }
  else
  {
    CLEAR_MASK(pProject0,b7SWITCHBAK);
  }
  nSwitchableBTNDIS;    //UMA
}

//============S5 -> S0==========================================================
//
// Intel Power Sequence subroutines
//
void S5S0_EC_ON_HI(void)
{
//	SET_MASK(FanSpeedLevel,FanAuto);	// Default Fan auto mode
//	PalBKLidSyncEn;										// Initial default Lid Sync backlight
    //Init_IT8302_ControlPin();
  GPOTF = 0x00;
OEM_TYPE_C_S5S0_HOOK();
  EC_ON_HI();
  EC_ON2_3V_HI();
  PM_PWRBTN_HI();   
//    PM_PWRBTN_HI();        // CAC. 2013.05.22
//  GPOTF = 0x00;
#if (SUPPORT_DSx)
  //SUSACK_HI();
#endif
  LedPowerOn();     // turn on Power LED
//[CC20131118][Project][Start] Add shutdown function for hang on power sequence.
  cPWRONFail = 0x00;
//[CC20131118][Project][End] Add shutdown function for hang on power sequence.
}

void S5S0_EC_ON2_HI(void)
{
#if (PLATFORM_CHIP == INTEL)
  EC_ON2_HI();
#endif
}

BYTE S5S0_VDDP(void)
{
  EC_ON3_VDDP_HI();
  if(Read_0P9VALW_PWRGD())
  {
    return(1);
  }
  return(0);
}

void S5S0_RSMRST_HI(void)
{
  //DRAMRST_CNTRL_EC_HI();
  RSMRST_HI();
  //DPWROK_EC_HI(); 
}

void S5S0_AC_PRESENT_HI(void)
{
  PM_PWRBTN_HI();
  if(ACPOWER_ON)
  {
  	AC_PRESENT_HI();
  }
}

void S5S0_PM_PWER_BTN2(void)
{
  PM_PWRBTN_LOW();
}

BYTE S5S0_CHK_S4(void)
{
  if(Read_SLPS4())
  {
    PM_PWRBTN_HI();
    SYSON_HI();
    //EnableS4Int(); //Ex85: should be Intel platform SLP_S4# low then SYSON_OFF fastest;
    //LAN_PWR_ON_LO();
    return(1);
  }
  PM_PWRBTN_LOW();
  return(0);
}

BYTE S5S0_CHK_S3(void)
{
  if(Read_SLPS3())
  {
    //EnableS3Int(); //Ex85: should be for Intel platform;
    return(1);
  }
  return(0);
}

void S5S0_CP_RESET_OFF(void)
{
#if Support_CP_RESET_control
	CP_RESET_OFF();
#endif //Support_CP_RESET_control
}
void S5S0_CP_RESET_ON(void)
{
#if Support_CP_RESET_control
	CP_RESET_ON();
#endif //Support_CP_RESET_control
}


void S5S0_SUSP(void)
{
  SUSP_ON();
  StableDelay = 200;
  H_PROCHOT_LOW();
  CURRENT_STATUS &= 0xC0; //Clear flags of entering and resuming from S3 & S4 &S5
  SetS0GPIO();
}

void S5S0_USBON(void)
{
  USB_ON_OUTPUT;
  USB_ON_LOW();
  nKbProjectID = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank0,EEPROM_KB_Type);  // read KB Type.
  switch( nKbProjectID )
  {
    case 'K':  // UK keyboard  ascii=4b
      cKBStatus &= 0xFC;
      cKBStatus |= 0x01;  // Set UK keyboard.
      break;
    case 'J':  // JP keyboard   ascii=4a
      cKBStatus &= 0xFC;
      cKBStatus |= 0x02;  // Set JP keyboard.
      break;
    case 'E':  // EX keyboard   ascii=45
      cKBStatus |= 0x03;  // Set BR keyboard.
      break;
    case 'S':  // US keyboard   ascii=53
    default:   // Other character then set US matrix for default
      cKBStatus &= 0xFC;  // Set US keyboard.
      break;
  }
  LoadKBMatrixToRAM();
  RSTS =0x44;
}

void S5S0_APUPWR_EN(void)
{
  APUPWR_EN_HI();
  //SinkPowerPathControl();
  if(IS_MASK_SET(LENOVOP2PSTATUS,LENOVOStatusFlag)){
 //if(IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Charge_Target))
 if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type))
					{	//Disable changing
					 RamDebug(0xA9);
							SinkPowerPathControl(1);
					}
				//	else
					{	//Enable changing
					 RamDebug(0xAA);
							SinkPowerPathControl(1);
						}
  	}
}
#if (PLATFORM_CHIP == INTEL)
void S5S0_CHK_ALL_PWRGD(void)
{
  BYTE BPwrDate=0x03;
  //if(Read_ALL_PWRGD())
  {
		{
		//	VCCSTPG_INPUT;
		    VCCSTPG_HI(); // Set to OD, Hi-Z or Output High
			VR_ON_ON();
		}
    //return(1);
  }
  //return(0);
  
  CLEAR_MASK(GS.Status,GS_Init); // G sensor : 3Valw->3Vs
}

void S5S0_VCCSTPG_HI(void)
{
		VCCSTPG_INPUT;
		VR_ON_ON();  
}
#endif 
BYTE S5S0_CHK_APU_PWRGD(void)
{
  if(Read_APU_PWRGD())
  {
    return(1);
  }
  return(0);
}

//void S5S0_VR_ON(void)
void S5S0_Initial_FollowIntel(void)
{
 // VR_ON_ON();
#if NV_GC6
  FB_CLAMP_OFF();
  nGC6ClampOff;
#endif // NV_GC6

#if !Support_SMTADP
  SetPowerBatteryparameter();
#endif
  MuteCount = 1;
  EC_TX_INPUT;
  if( Chk_DEBUG_IN() )
  {
    SET_MASK(pEdgeProjectFlag1, pEdgeProjectDebugIn);
    EC_TX_OUTPUT;
    EC_RX_OUTPUT;
  }
  else
  {
    CLEAR_MASK(pEdgeProjectFlag1, pEdgeProjectDebugIn);
    EC_RX_INPUT;
    EC_TX_INPUT;
  }
  if( PriBattDisChgGet && IS_MASK_SET(pBattLearn,uRefreshmode) && IS_MASK_SET(pBattLearn,uACoffstchg) )
  {
	  BAT_LEARN_ON();		
	  BATLEARNProcess(nAcOffCmdEN);
      SET_MASK(nStopChgStat3L,ENCUSTOMER);
  }
}



void S5S0_PCH_PwrOK(void)
{
	//PCH_PWROK_HI();
	 if(IS_MASK_SET(PriBattSTS,BattExist))  //
  	{
  //	if(IS_MASK_SET(DETECTDOCKORDAPTER,b0_ACORDOCK))
	if((IS_MASK_SET(DETECTDOCKORDAPTER,b0_ACORDOCK))&&IS_MASK_CLEAR(LENOVOP2PSTATUS,LENOVOStatusFlag))
  		{
  SET_TPC_RECONNECT_RTS5455(0); //
  		}
  	}

}

//void S5S0_PCH_SYSPwrOK(void)
void S5S0_PCH_PWR_GOOD(void)
{

	  PCH_PWR_GOOD_HI();
	VGA_TempDly = 14; // Set VGA delay inital temperature 14 sec

}

void S5S0_Init_Status(void)
{
  //for project design
 Lowpower_DISfunction();
  //for LCFC common code
  OEM_SYSTEM_S5_S0();
}

void S5S0_TP_RESET_ON(void)
{
#if Support_TP_RESET_control
  if (!RST_TRACK_POINT_GET)
    TP_RESET_ON();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_ON();
#endif //Support_BYPASS_PAD_control

}

void S5S0_TP_RESET_OFF(void)
{
#if Support_TP_RESET_control
  if (!RST_TRACK_POINT_GET)
    TP_RESET_OFF();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_OFF();
#endif //Support_BYPASS_PAD_control
}

//-------------------------------------------------------------------------------------
//  Do S5 -> S0 power sequence
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS5S0[]=
{
//
// Intel Power Sequence
//
    // Func               // Delay  // Check flag
  { 0x0000,                 0,       0,  },  // Dummy step always in the top of tabel
  { S5S0_EC_ON_HI,         10,       0,  },  // 1
  //{ S5S0_EC_ON2_HI,        20,       0,  }, // 2
  { CheckPMIC,              1,       1,  },  // 2
  { InitPMIC,              10,       1,  },  // 3 
  { S5S0_VDDP,              20,       1,  },  // 4
  { S5S0_RSMRST_HI,        25,       0,  },  // 5
  { S5S0_AC_PRESENT_HI,    25,       0,  },  // 6
  { S5S0_PM_PWER_BTN2,     20,       0,  },  // 7
  { S5S0_CHK_S4,            1,       1,  },  // 8
  { S5S0_CHK_S3,           23,       1,  },  // 9
//  { S5S0_CP_RESET_OFF,     0,       0,  }, 
  { S5S0_SUSP,             10,       0,  },  // 10
  { S5S0_USBON,            10,       0,  },  // 11
  { S5S0_APUPWR_EN,        10,       0,  },  // 12
//  { S5S0_VCCSTPG_HI,       5,       0,  },
  { S5S0_CHK_APU_PWRGD,    5,       1,  },	 // 13
{ S5S0_PCH_PwrOK,        50,       0,  },  // 
//  { S5S0_TP_RESET_ON,     45,       0,  },      // TP_RESET_ON after SUSP 51ms
  { S5S0_Initial_FollowIntel,5,       0,  },  // 14
  { S5S0_PCH_PWR_GOOD,     75,       0,  },  // 15
  { S5S0_CP_RESET_OFF,     20,       0,  },  // 16
  { S5S0_CP_RESET_ON,      20,       0,  },  // 17
  { S5S0_CP_RESET_OFF,    200,       0,  },  // 18
  { S5S0_TP_RESET_ON,      20,       0,  },  // 19
  { S5S0_TP_RESET_OFF,     50,       0,  },  // 20
  { S5S0_Init_Status,       0,       0,  },  // 21
};


//============S0 -> S5==========================================================
//
// Intel Power Sequence subroutines
//
void S0S5_PCH_PWR_GOOD()
{
  PCH_PWR_GOOD_LOW();
   if(IS_MASK_SET(LENOVOP2PSTATUS,LENOVOStatusFlag)){
 //if(IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Charge_Target))
 if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type))
					{	//Disable changing
					 RamDebug(0xA9);
							SinkPowerPathControl(1);
					}
				//	else
					{	//Enable changing
					 RamDebug(0xAA);
							SinkPowerPathControl(1);
						}
  	}
}
void S0S5_APUPWR_EN(void) //Ex85:move S0S5_VR_ON related code to here;
{
#if NV_GC6
  CLEAR_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
#endif // NV_GC6
  APUPWR_EN_LOW();
  //AC_PRESENT_LOW();
  if( !Read_SLPS5() )
  {
    if( IS_MASK_SET(pBattLearn,uRefreshmode) )
    {
	    BAT_LEARN_OFF();
	    BATLEARNProcess(nAcOffCmdDIS);
      PriBattDisChgDis;
      pBattLearn = 0;
        CLEAR_MASK(nStopChgStat3L,ENCUSTOMER);
    }
  }
}

BYTE S0S5_CHK_APU_PWRGD(void)
{
  if(!Read_APU_PWRGD())
  {
    return(1);
  }
  return(0);
}

void S0S5_VR_ON(void)
{
#if NV_GC6
  CLEAR_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
#endif // NV_GC6
//  VR_ON_OFF();
#if (PLATFORM_CHIP == INTEL) 
  PCH_PWROK_LOW();
  PCH_SYSPWROK_LOW();
  //PCH_APWROK_OFF();
#endif  
  AC_PRESENT_LOW();
  if( !Read_SLPS5() )
  {
    if( IS_MASK_SET(pBattLearn,uRefreshmode) )
    {
	  BAT_LEARN_OFF();
	  BATLEARNProcess(nAcOffCmdDIS);
      PriBattDisChgDis;
      pBattLearn = 0;
        CLEAR_MASK(nStopChgStat3L,ENCUSTOMER);
    }
  }
}

void S0S5_NVDD_OFF(void)
{
}

void S0S5_SUSP(void)
{
#if Support_CP_RESET_control
  CP_RESET_ON();
#endif //Support_CP_RESET_control  
  TP_RESET_OFF();
BYPASS_PAD_OFF();
  SUSP_OFF();
  SetS5GPIO();
#if (PLATFORM_CHIP == INTEL)   
  VCCSTPG_OUTPUT;
  VCCSTPG_LOW(); 
  VR_ON_OFF();
#endif
}

void S0S5_SYS_ON(void)
{
  SYSON_LOW();
}

void S0S5_DPWROK_OFF(void)
{
#if (PLATFORM_CHIP == INTEL) 
    DPWROK_EC_LOW();
#endif
}
void S0S5_PCH_PWR_EN_OFF(void)
{
}

void S0S5_Init_Status(void)
{
 //for project design
 
 if(!Read_AC_IN())
 	Lowpower_ENfunction();
 //for LCFC common code
 OEM_SYSTEM_S0_S5(); 
 WlanWakeStatusEEPROM= ((EventMask0&0X01)|(EventMask1&02));
 WRITE_DATA2EEPROM(&WlanWakeStatusEEPROM , 1 , EEPROM_Bank7, EEPROM_ACWOL_STATUS);
 
}

//-------------------------------------------------------------------------------------
//  Do S0 -> S5 power sequence
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS0S5[]=
{
//
// Intel Power Sequence
//
  { 0x0000,               0,    0,  },  // Dummy step always in the top of tabel
  { S0S5_PCH_PWR_GOOD,    2,    0,  },
  { S0S5_APUPWR_EN,       1,    0,  },
  { S0S5_CHK_APU_PWRGD,  16,    1,  },
  //{ S0S5_VR_ON,         119,    0,  },
  //{ S0S5_NVDD_OFF,        1,    0,  },
  { S0S5_SUSP,           10,    0,  },
  { S0S5_SYS_ON,         10,    0,  },
  //{ S0S5_DPWROK_OFF,     20,    0,  },
  //{ S0S5_PCH_PWR_EN_OFF,  0,    0,  },
  { S0S5_Init_Status,     0,    0,  },
};


//============S0 -> S3==========================================================
//
// Intel Power Sequence subroutines
//
void S0S3_PCH_PWR_GOOD(void)
{
  PCH_PWR_GOOD_LOW();
}

void S0S3_APUPWR_EN(void) //Ex85:move S0S5_VR_ON related code to here;
{
#if NV_GC6
  CLEAR_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
#endif // NV_GC6
  APUPWR_EN_LOW();
}

BYTE S0S3_CHK_APU_PWRGD(void)
{
  if(!Read_APU_PWRGD())
  {
    return(1);
  }
  return(0);
}

void S0S3_VR_ON(void)
{
#if NV_GC6
  CLEAR_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
#endif // NV_GC6
//    VR_ON_OFF();
#if (PLATFORM_CHIP == INTEL) 
  PCH_PWROK_LOW();
  PCH_SYSPWROK_LOW();
  //PCH_APWROK_ON();
#endif  
}

void S0S3_SUSP(void)
{
  SUSP_OFF();
  #if Support_CP_RESET_control
  CP_RESET_ON();
#endif //Support_CP_RESET_control  
  TP_RESET_OFF();
BYPASS_PAD_OFF();
  SetS5GPIO();
#if (PLATFORM_CHIP == INTEL)   
  VCCSTPG_OUTPUT;
  VCCSTPG_LOW();
  VR_ON_OFF();
#endif  
  ISCT_Again = 59;  // Last 60 Sec count for ISCT used.
}

void S0S3_Init_Status(void)
{
  //for project design
 if(!Read_AC_IN())
 	Lowpower_ENfunction();
  //for LCFC common code
  OEM_SYSTEM_S0_S3();  
}

//-------------------------------------------------------------------------------------
//  Do S0 -> S3 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS0S3[]=
{
//
// Intel Power Sequence
//
    { 0x0000,               0,    0,  },  // Dummy step always in the top of tabel
    { S0S5_PCH_PWR_GOOD,    2,    0,  },
    { S0S5_APUPWR_EN,       1,    0,  },
    { S0S5_CHK_APU_PWRGD,  20,    1,  },
    { S0S3_SUSP,           10,    0,  },
    { S0S3_Init_Status,     0,    0,  },
};

//============S3 -> S0==========================================================
//
// Intel Power Sequence subroutines
//
void S3S0_PM_PWER_BTN1(void)
{
  FNKB_S3Dly = 0;
  PM_PWRBTN_LOW();
}

void S3S0_PM_PWER_BTN2(void)
{
  PM_PWRBTN_HI();
  SysPowState=SYSTEM_S3;
}

void S3S0_CP_RESET_OFF(void)
{
#if Support_CP_RESET_control
  CP_RESET_OFF();
#endif //Support_CP_RESET_control  
}

void S3S0_APUPWR_EN(void)
{
  APUPWR_EN_HI();
  CLEAR_MASK(GS.Status,GS_Init);  // G sensor : 3Valw->3Vs
  MuteCount = 1;  //tony 1208
   if(IS_MASK_SET(LENOVOP2PSTATUS,LENOVOStatusFlag)){
 //if(IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Charge_Target))
 if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type))
					{	//Disable changing
					 RamDebug(0xA9);
							SinkPowerPathControl(1);
					}
				//	else
					{	//Enable changing
					 RamDebug(0xAA);
							SinkPowerPathControl(1);
						}
  	}
}

BYTE S3S0_CHK_APU_PWRGD(void)
{
  if(Read_APU_PWRGD())
  {
    return(1);
  }
  return(0);
}

void S3S0_SUSP(void)
{
  PM_PWRBTN_HI();
  SUSP_ON();
  StableDelay = 200;
  H_PROCHOT_LOW();
  SetS0GPIO();
  USB_ON_OUTPUT;
  USB_ON_LOW();
  //EnableS3Int();
  //EnableS4Int();
}



#if(PLATFORM_CHIP == INTEL)
void S3S0_CHK_ALL_PWRGD(void)
{
//	VCCSTPG_INPUT;
	VCCSTPG_HI();
	VR_ON_ON(); 
    
}

BYTE S3S0_CHK_VGATE(void)
{
  if(Read_VGATE())
  {
    return(1);
  }
  return(0);
}
#endif
void PCH_PWROK(void)
{
  MuteCount = 1;  //tony 1208
}

void S3S0_TP_RESET_ON(void)
{
#if Support_TP_RESET_control
  if (!RST_TRACK_POINT_GET)
    TP_RESET_ON();
#endif //Support_TP_RESET_control  
#if Support_BYPASS_PAD_control
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_ON();
#endif //Support_BYPASS_PAD_control
}
void S3S0_TP_RESET_OFF(void)
{
#if Support_TP_RESET_control
  if (!RST_TRACK_POINT_GET)
    TP_RESET_OFF();
#endif //Support_TP_RESET_control  
#if Support_BYPASS_PAD_control
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_OFF();
#endif //Support_BYPASS_PAD_control
}
#if (PLATFORM_CHIP == INTEL)
void S3S0_VR_ON(void)
{
//  PCH_APWROK_ON();
  PCH_PWROK_HI();
  //PCH_PWR_GOOD_HI();

  if( PriBattDisChgGet && IS_MASK_SET(pBattLearn,uRefreshmode) && IS_MASK_SET(pBattLearn,uACoffstchg) )
  {
	  BAT_LEARN_ON();		
	  BATLEARNProcess(nAcOffCmdEN);
      SET_MASK(nStopChgStat3L,ENCUSTOMER);
  }
}
#endif
//void S3S0_PCH_SYSPwrOK(void)
void S3S0_PCH_PWR_GOOD(void)
{
   //PCH_SYSPWROK_HI();
   PCH_PWR_GOOD_HI();
   //Ex85:add start
   if( PriBattDisChgGet && IS_MASK_SET(pBattLearn,uRefreshmode) && IS_MASK_SET(pBattLearn,uACoffstchg) )
   {
	  BAT_LEARN_ON();		
	  BATLEARNProcess(nAcOffCmdEN);
      SET_MASK(nStopChgStat3L,ENCUSTOMER);
   }
   //Ex85:add end
   VGA_TempDly = 10; // Set VGA delay inital temperature 10 sec.
}

void S3S0_Init_Status(void)
{
  //for project design
 Lowpower_DISfunction();
  AOU_IFG_Debounce_Cnt = 0;
  //for LCFC common code
  OEM_SYSTEM_S3_S0(); 
}
//-------------------------------------------------------------------------------------
//  Do S3 -> S0 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS3S0[]=
{
//
// Intel Power Sequence
//
    { 0x0000,              0,      0,  },  // Dummy step always in the top of tabel
    { S3S0_PM_PWER_BTN1,  50,      0,  },
    { S3S0_PM_PWER_BTN2,   0,      0,  },
    { S3S0_CP_RESET_OFF,   0,      0,  },
    { S3S0_SUSP,          20,      0,  },
    { S3S0_APUPWR_EN,     10,      0,  },
    { S3S0_CHK_APU_PWRGD, 10,      1,  },
    { S3S0_TP_RESET_ON,   74,      0,  },
    { S5S0_PCH_PWR_GOOD,  36,      0,  },
    { S3S0_Init_Status,    0,      0,  },
};

//=============================================================================
void CheckAutoPowerOn(void)
{
  if ((WinFlashMark == autoOnMark1) && (WinFlashMark2 == autoOnMark2) && SystemIsS5)	
  {
//    AutoTimer = 0x01;
    AutoTimer = 0x83;
    WinFlashMark = 0;
    WinFlashMark2 = 0;
  }
}

void Check_EC_ON()
{
  if ( (IS_MASK_SET(Thro_Status2,b5BAT_TooLow))&&(SystemIsS5) )
  {
	  AOAC_PWROFF(); // Turn off WLAN power.
	  RSMRST_LOW();
	  //Delay1MS(5);
#if ((PLATFORM_CHIP == INTEL)&&(SUPPORT_DSx))
    DPWROK_EC_LOW();
#endif // PLATFORM_CHIP
    _nop_();
    _nop_();
    _nop_();
    EC_ON3_VDDP_LOW();
    //Delay1MS(10);
#if (PLATFORM_CHIP == INTEL)    
	  EC_ON2_LOW();
#endif
    CLEAR_MASK(GS.Status, GS_Init);  
    PM_PWRBTN_LOW();   
	EC_ON_LOW();
  EC_ON2_3V_LOW();
	AC_PRESENT_LOW(); //For power consumption leakage current issue
    EC_WAKE_OUT_LOW();
    CLEAR_MASK(SysStatus,LidStatus);
    SET_MASK(S0S5PWRcodition,S0S5PWROFF);
    LID_readytime = 11 ;  // LID switch wakeup time = 100ms
    CLEAR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
    SET_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);
    CLEAR_MASK(Thro_Status2,b5BAT_TooLow);
    return;
  }
  else
  {
    if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut)
      && (IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
      && (LOWBATT_3TIMES==0)&&((SysStatus2&0x03)== 0)
      )  // Not cut power in the battery mode for BIOS used.
    {
	    if ( IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut) && IS_MASK_CLEAR(cCmd,cC_PCHPWR_Keep))  // Not cut power in the battery mode for command used.
        {
        //if ( (IS_MASK_CLEAR(USB_Charger, b0USB_EN)) || (IS_MASK_SET(USB_Charger, b2USB_TmlDis)) )
          if ( ((!USB_PWRGET) || IS_MASK_SET(USB_Charger, b2USB_TmlDis))
          && IS_MASK_CLEAR(SYS_STATUS,AC_ADP)
          )
             {
			  AOAC_PWROFF(); // Turn off WLAN power.
			  RSMRST_LOW();
             // Delay1MS(5);
#if ((PLATFORM_CHIP == INTEL)&&(SUPPORT_DSx))
              DPWROK_EC_LOW();
#endif // PLATFORM_CHIP
              _nop_();
              _nop_();
              _nop_();
        EC_ON3_VDDP_LOW();
        //Delay1MS(10);     
#if (PLATFORM_CHIP == INTEL)              
		         EC_ON2_LOW();
#endif
              CLEAR_MASK(GS.Status, GS_Init);          
              PM_PWRBTN_LOW();  
			  EC_ON_LOW();
        EC_ON2_3V_LOW();
			  AC_PRESENT_LOW(); //For power consumption leakage current issue
			  EC_WAKE_OUT_LOW();
		      CLEAR_MASK(SysStatus,LidStatus);
		      SET_MASK(S0S5PWRcodition,S0S5PWROFF);
          }
          else
          {
            if(  Battdata_ready != 1)
            { return; }
            if(!Read_AOU_EN())
            {
			         AOAC_PWROFF(); // Turn off WLAN power.
			         RSMRST_LOW();
			         //Delay1MS(5);
#if ((PLATFORM_CHIP == INTEL)&&(SUPPORT_DSx))
              DPWROK_EC_LOW();
#endif 
              _nop_();
              _nop_();
              _nop_();
              EC_ON3_VDDP_LOW();
              //Delay1MS(10);  
#if (PLATFORM_CHIP == INTEL)              
		         EC_ON2_LOW();
#endif
             CLEAR_MASK(GS.Status, GS_Init);      
          PM_PWRBTN_LOW();  
		      EC_ON_LOW();
          EC_ON2_3V_LOW();
		      AC_PRESENT_LOW(); //For power consumption leakage current issue
			  EC_WAKE_OUT_LOW();
			  CLEAR_MASK(SysStatus,LidStatus);
			  SET_MASK(S0S5PWRcodition,S0S5PWROFF);
            }
          }
        }
        LID_readytime = 11 ;  // LID switch wakeup time = 100ms
        CLEAR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
        SET_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);
        return;
      }
    }
  SET_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
  CLEAR_MASK(LENOVOPMFW_Temp,b5EC_ON_EN); 
}

void CHK_BATT_5PERCL(void)
{
  Init_SMBus_Regs();  // initial SMBUS
  if ( !Read_AC_IN() && Read_EC_PWRBTN() )
  {
    EC_ON_HI();

    if ((Battdata_ready != 1)&&(!Read_AC_IN()||Read_ACOFF()))
    {
      FirstGetBatData();
      if (Battdata_ready != 1)
      {
        Battdata_ready = 0xFF;
        return;
      }
    }

    if ((RelativeStateOfChgL <= 0x05)&&(!Read_AC_IN()||Read_ACOFF()))
    {
      LOWBATT_3TIMES = 0;    // Clear delay time.
      return;
    }

    if(!Read_LID_SW_IN())
    { return; }

    PWSeqStep = 1;
    PowSeqDelay = 0x00;
    SysPowState=SYSTEM_S5_S0;
    RamDebug(0x57);
  }
}

const PowerSEQTableList code PowerSEQSTBL[4] = {
  {
    asPowerSEQS5S0,
    sizeof (asPowerSEQS5S0) / sizeof (sPowerSEQ),
  },
  {
    asPowerSEQS0S5,
    sizeof (asPowerSEQS0S5) / sizeof (sPowerSEQ),
  },
  {
    asPowerSEQS3S0,
    sizeof (asPowerSEQS3S0) / sizeof (sPowerSEQ),
  },
  {
    asPowerSEQS0S3,
    sizeof (asPowerSEQS0S3) / sizeof (sPowerSEQ),
  },
};


void Oem_PowerSequence(BYTE SxSeq)
{
  if ( PowSeqDelay != 0x00 )
  {
    PowSeqDelay--;                   //1m Sec count base
    if ( PowSeqDelay != 0x00 )  return;
  }
  while(PWSeqStep<(PowerSEQSTBL[SxSeq].PowerSEQCount))
  {
    if(PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].checkstatus==0x00)  // Do function
    {
      (PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].func)();
      PowSeqDelay = PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].delay;
#if Support_EC_Port80_Debug      
      EC_OUT_PORT80(PWSeqStep);
#endif //Support_EC_Port80_Debug
      PWSeqStep++;
      if(PowSeqDelay!=0x00)
      {
        break;
      }
    }
    else  // Check input status pin
    {
      if((PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].func)())
      {
        PowSeqDelay = PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].delay;
        PWSeqStep++;
      }
      break;
    }
  }
}

void Oem_hook_FnEquIOFunc(void)
{
	GPIO_STATUS = 0;
	if ( Read_RSMRST() )				// bit0:GPIO_RSMRST
	{ SET_MASK(GPIO_STATUS,GPIO_RSMRST); }
	if ( Read_PM_PWRBTN() )				// bit1:GPIO_PBTN_OUT
	{ SET_MASK(GPIO_STATUS,GPIO_PBTN_OUT); }
#if (PLATFORM_CHIP == INTEL)
//	if ( Read_VDDQ_PGOOD() )			// bit2:GPIO_VDDQ_PGOOD
//	{ SET_MASK(GPIO_STATUS,GPIO_VDDQ_PGOOD); }
#else // PLATFORM_CHIP
// AMD
	if ( Read_VDDAPWROK() )			// bit2:GPIO_VDDAPWROK
	{ SET_MASK(GPIO_STATUS,GPIO_VDDQ_PGOOD); }
#endif // PLATFORM_CHIP
	if ( Read_SLPS3() )					// bit3:GPIO_PM_SLP_S3
	{ SET_MASK(GPIO_STATUS,GPIO_PM_SLP_S3); }
#if (PLATFORM_CHIP == INTEL)
	if ( Read_SLPS4() )					// bit4:GPIO_PM_SLP_S4
	{ SET_MASK(GPIO_STATUS,GPIO_PM_SLP_S4); }
#endif // PLATFORM_CHIP
	if ( Read_SLPS5() )					// bit5:GPIO_PM_SLP_S5
	{ SET_MASK(GPIO_STATUS,GPIO_PM_SLP_S5); }
#if (PLATFORM_CHIP == INTEL)
	if ( Read_VR_ON() )					// bit6:GPIO_VR_ON
	{ SET_MASK(GPIO_STATUS,GPIO_VR_ON); }
	if ( Read_PCH_PWROK() )				// bit7:GPIO_PCH_PWROK
	{ SET_MASK(GPIO_STATUS,GPIO_PCH_PWROK); }
#else // PLATFORM_CHIP
// AMD
	if ( Read_APUPWR_EN() )					// bit6:GPIO_APUPWR_EN
	{ SET_MASK(GPIO_STATUS,GPIO_VR_ON); }
	if ( Read_PCH_PWR_GOOD() )				// bit7:GPIO_FCH_PWR_GOOD
	{ SET_MASK(GPIO_STATUS,GPIO_PCH_PWROK); }
#endif // PLATFORM_CHIP
}

//=============================================
// S3/S4 Int Enable
//=============================================
void EnableS3Int(void)
{
	ISR0 = Int_WKO20;
    SET_MASK(IELMR0,Int_WKO20);
    SET_MASK(WUEMR2,WUC_WUI0);
	SET_MASK(WUESR2,WUC_WUI0);
    
    CLEAR_MASK(IPOLR0,Int_WKO20);    // rising
    
    ISR0 = Int_WKO20;

    SET_MASK(IER0, Int_WKO20);
}

void EnableS4Int(void)
{
	ISR3 = Int_WKO21;
    SET_MASK(IELMR3,Int_WKO21);
    SET_MASK(WUEMR2,WUC_WUI1);
	SET_MASK(WUESR2,WUC_WUI1);
    
    CLEAR_MASK(IPOLR3,Int_WKO21);    // rising
    
    ISR3 = Int_WKO21;

    SET_MASK(IER3, Int_WKO21);
}

void Lowpower_ENfunction(void)
{
	SET_MASK(CHGIC_ReadCmd0x12H,LOWPWR_EN);
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))  
    {
		CHGIC_SMbusFailCnt++;
			   
    }
}

void Lowpower_DISfunction(void)
{
	CLEAR_MASK(CHGIC_ReadCmd0x12H,LOWPWR_EN);
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))  
    {
		 CHGIC_SMbusFailCnt++;
	}
}

