/*-----------------------------------------------------------------------------
 * TITLE: OEM_EVENT.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//-----------------------------------------------------------------------------
// no change function
//-----------------------------------------------------------------------------
void NullEvent(void)
{

}

//-----------------------------------------------------------------------------
// Adapter in function
//-----------------------------------------------------------------------------
void AdapterIn(void)
{
#if (PLATFORM_CHIP == INTEL)
  PM_PWRBTN_HI();
  EC_WAKE_OUT_HI();
  EC_ON_HI();
  if(Read_RSMRST())
  {
  	AC_PRESENT_HI();
  }

  PECI_SETPL2Watts(2);
#endif // PLATFORM_CHIP
  if ( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) )
  { cBATTThrottling = 0; }
  
  CLEAR_MASK(BatteryAlarm,ADPOUT);
  CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);

  SET_MASK(SYS_STATUS,AC_ADP);  //set battery flag
  SET_MASK(u4SenseSTS3, SomeAcAdapterIn);
  Chk_Battery_Full();
#if 1 //BROOKEWORKAROUND
  ACEn;
  ACattachedEn;
  CLEAR_MASK( u4PeriSTS3, UNSUPPORTED_AC );
#endif
#if !Support_SMTADP
  ECSMI_SCIEvent(ACPI_ACIN_SCI);
#endif
 ECSMI_SCIEvent(ACPI_ACIN_SCI);
  if( SystemIsS0 )
  {
    //PWM_BEEP = 0x7F;      // Out beep
    //PWM_LEDBeep_CNT = 0x0A;    // 50ms*10
    u4BeepID = 0x06;
  }
//[-start-131213-T130124_9-add]//
  ADPID_ON_EN();
//[-end-131213-T130124_9-add]//
  SetACIN_Int();
  BatSMbusFailCount = 0;
  nBattErrorCnt = 0;
  inhibit2sec = 20;
  if((IS_MASK_CLEAR(nStopChgStat3H,NotLenovoBattery))&&
  ((IS_MASK_CLEAR(nStopChgStat3L,ENCMDSTOPCHG))&&(IS_MASK_CLEAR(nStopChgStat3H, PMTHRESHOLD))&&(IS_MASK_CLEAR(nStopChgStat3H,ENBOOSTSTOPCHG)))){
    pFakeCHGCnt = inhibit2sec+40;      // 6sec
  }
  SET_MASK(nStopChgStat3L,ENCHARGESUSP);    // use for detect battery charging suspend
  CurrentL = 0;
  CurrentH = 0;
  // Set 0x12h and 0x3Fh before set ChargeCurrent and ChargeVoltage.
  CHGIC_ptr = 1;
  WriteSmartChgIC();
  WriteSmartChgIC();
  WriteSmartChgIC();       
  //
  CHGIC_ptr = 4;           

  inhibit2sec = 0;
  EC_ON_HI();
  ReadSmartChgIC();
  Unlock_ShipMode();
  if (SystemNotS5)
  {
    ACIN_BLINK3 = 35;        // LED blink 3times in 1.5sec
  }
  else
  {
    ACIN_BLINK3 = 30;        // LED blink 3times in 1sec
  }
  
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L, SMBus_NoPEC);
  SET_MASK(CHGIC_ReadCmd0x34L,PROFILE_5);  
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  if((RelativeStateOfChgL < pEdgeBATStopCharge)&&(RelativeStateOfChgL > pEdgeBATStartCharge))
  	{
  	 SET_MASK(nStopChgStat3H, PMTHRESHOLD);
  	}
  //SetPowerBatteryparameter();
#if (CHARGER_IC == BQ25700)
  ChargerIC_Init = 0;
#endif

//#if Support_TYPE_C
//	SendPowerModeChangeToPdc();
//#endif
#ifdef OEM_Hook_AdapterIn_support
    OEM_Hook_AdapterIn();
#endif // OEM_Hook_AdapterIn_support 
TypeCAdpPdStableCheck = 20;
if(SystemIsS5)
{
 PD_Ini_Stable_Delay = 0XFFF0;
CLEAR_MASK(TypeCStatus,TypeCIniOK);
}
}

//-----------------------------------------------------------------------------
// Adapter out function
//-----------------------------------------------------------------------------
void AdapterOut(void)
{
  BYTE  AcDis_changed;

  AcDis_changed = 0;
if(SystemIsS0)
{
  H_PROCHOT_HI();
}
  AC_PRESENT_LOW();
  //VGA_AC_BATT_OFF();
  
  if( IS_MASK_SET(pProject0,bAPUTURBO) )
  {
   	CPUtuborONOFF(Enable);
	  CLEAR_MASK(pProject0,bAPUTURBO);
  }

  if( IS_MASK_SET(pProject0,bHYBAPUTURBO) )
  {
    CPUtuborONOFF(Enable);
	  CLEAR_MASK(pProject0,bHYBAPUTURBO);
   }
  if( IS_MASK_SET(pProject0,bRSOCAPUTURBO) )
  {
    CPUtuborONOFF(Enable);
    CLEAR_MASK(pProject0,bRSOCAPUTURBO);
   }

  cADPIDTemp = 0;
  AdapterID = 0;
  u4ACAdaptorCap = 0;
  AdpWatt = 0;
  CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
  CLEAR_MASK(SYS_STATUS,AC_ADP);  //clear battery flag
  CLEAR_MASK(nStopChgStat3L,ENOVERVOLTAGE);
  Chk_Battery_Full();
  ADPID_ON_DIS();

  // Set P-State highest to force that ChkBattery_OCP will decress P-State step by step.
  cBATTThrottling = CPUMAXPSTATE;
  cGPUThrottling = GPUMAXPSTATE;
  SET_MASK(BatteryAlarm,ADPOUT);
  SET_MASK(GPUAlarm,GPUAlarm_ADPOUT);  
  CLEAR_MASK(BatteryAlarm, HybridSTPP);
  ADPI2Sec = ADPI_Sec;
  GPU2Sec = GPU_Sec;
  // Send QEvent to Bios
  ThrottlingControl();

  inhibit2sec = 20;
  if( SystemIsS0 )
  {
    //PWM_BEEP = 0x7F;      // Out beep
    //PWM_LEDBeep_CNT = 0x0A;    // 50ms*10
    u4BeepID = 0x06;

  }

  if ( SystemIsS5 )
  {
    ECSleepCount = 3;  // Delay entry EC sleep.
    if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut)&&(IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
      && (LOWBATT_3TIMES==0)&&((SysStatus2&0x03)== 0))  // Not cut power in the battery mode for BIOS used.
    {
      if ( IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut) && IS_MASK_CLEAR(cCmd,cC_PCHPWR_Keep))  // Not cut power in the battery mode for command used.
      {
        RSMRST_LOW();
        Delay1MS(20);
#if (PLATFORM_CHIP == INTEL)
        PCH_PWR_EN_OFF();
#endif // PLATFORM_CHIP
      }
    }
  }

  if( PriBattDisChgGet && IS_MASK_SET(pBattLearn,uRefreshmode) && IS_MASK_SET(pBattLearn,uACoffstchg) )
  {
    BAT_LEARN_ON();
    BATLEARNProcess(nAcOffCmdEN);
    SET_MASK(nStopChgStat3L,ENCUSTOMER);
  } else {
    ACOFF_SOURCE=0;
    ACOFF_LOW();
 //   BAT_LEARN_OFF();
	
  	CLEAR_MASK(CHGIC_ReadCmd0x12L,BATLearn);  
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
    {
      CHGIC_SMbusFailCnt++;
    }
  	else
  	{
      CHGIC_SMbusFailCnt = 0;
  	}
  }

  if (IS_MASK_SET(u4SenseSTS0, ACSts)) {
    ACDis;
    ACattachedDis;
    AcDis_changed = 1;
  }
  if (AcDis_changed) {
    ECSMI_SCIEvent(ACPI_ACOUT_SCI);
  }
  CLEAR_MASK(u4SenseSTS3, SomeAcAdapterIn);
  CLEAR_MASK(u4PeriSTS3, UNSUPPORTED_AC );
  AttachedPowerNOTsupportDis;  
  AvgCurrentL = 0;
  AvgCurrentH = 0;
  Bat0x0BFakeCnt = 60;
  //CLEAR_MASK(LENOVOPMFW,BATTERY_CYCLE);

//  SetACIN_Int();



  pFakeCHGCnt = 0;
//  CLEAR_MASK(pEdgeProjectFlag2, pEdgeGPUOCTHROT);
  CLEAR_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
  CLEAR_MASK(Thro_Status, b2ProCH_EXTVGA);
	CLEAR_MASK(uProject0,NeedRecoveryNotSupportADP);
	CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);

#if (CHARGER_IC == BQ25700)
  ChargerIC_Init = 0;
#endif

	ADPI_MUL = ClassicAdaptor_ADPI_MUL;	//
#if Support_TYPE_C
	//SendPowerModeChangeToPdc();
#endif
#ifdef OEM_Hook_AdapterOut_support
    OEM_Hook_AdapterOut();
#endif // OEM_Hook_AdapterOut_support   
}

//-----------------------------------------------------------------------------
//power button was pressed
//-----------------------------------------------------------------------------
void PSWPressed(void)
{
  const BYTE code LShift_Pressed[] = {0x12,0xF0,0x12,0x00};
  if( PWRBTN_WDTFLAG != 0x87 )
  PWRBTN_WDTFLAG = 0x78;

  if (IS_MASK_SET(cCmd,cC_TestBtnEn))
  {
    Buffer_Key(0xE0);    //
    Buffer_Key(0x25);    //
    F_Service_SEND = 1;
  }

  EC_ON_HI();
  ECSleepCount = 3;
  AutoTimer = 0;    // Clear Auto timer.

  // Clear PswPressedCounter
  CountSecAfterPswPressed = 0;

  //if (LID_readytime !=0)
  //{
  //  SET_MASK(SysStatus2,PWRBTN_press_retry);
  //  return;
  //}
  CLEAR_MASK(SysStatus2,PWRBTN_press_retry);

  //if(!Read_LID_SW_IN())
  //{  return; }

  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      if ((Battdata_ready != 1)&&(!Read_AC_IN()||Read_ACOFF()))
      {
        FirstGetBatData();
        if (Battdata_ready != 1)
        {
          Battdata_ready = 0xFF;
          break;
        }
      }
/*
#if Support_TYPE_C	  
      if(!BattExistGet && IS_MASK_CLEAR(TypeCStatus,TypeCAdpReady) && IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeADPIDReady))
#else
      if(!BattExistGet) // && IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeADPIDReady))
#endif
      {
      	break;
      }
*/    if(!BattExistGet && AttachedPowerNOTsupportGET)
      {
      	break;
      }
      if ((RelativeStateOfChgL <= 0x05)&&(!Read_AC_IN()||Read_ACOFF()))
      {
        LOWBATT_3TIMES = 0;    // Clear delay time.
      }
      if ((!Read_AC_IN()||Read_ACOFF()))      // if DC mode
      {
        BatSMbusFailCount = 0;
        nBattErrorCnt = 0;
      }
      if( IS_MASK_SET(SYS_MISC1,Crisis_On) )
      {
        if(  (!EC_KBFNON()) || (!CheckCrisisMode()) )
        {
          CLEAR_MASK(SYS_MISC1,Crisis_On);
        }
      }
#if  Support_SMTADP
      Edge_SMTADPProcess();
#endif
      PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState=SYSTEM_S5_S0;
      break;

    case SYSTEM_S3 :
    case SYSTEM_S0 :
      if( PwrBtnWarnGET )
      {
        if( PowerOffWarnGET )
        {
          u4BeepID = 0x08;
        }
      }
      else
      {
#if SW_ISCT
        if ( IS_MASK_SET(AOAC_STATUS, ISCT_S0_ISCT))  // Check ISCT enable?
        {
          Buffer_String(LShift_Pressed);                // L-shift
        }
        else
#endif // SW_ISCT
        {
#if chPWSW1WDT	
          if (IS_MASK_CLEAR(pProject4,pPWSWdisable))  //power button for 7s reset-2015/7/17 
		  {
	        PM_PWRBTN_LOW();
	      }
#else
			PM_PWRBTN_LOW();
#endif //chPWSW1WDT			
        }
      }
      break;

    default :
      break;
  }
}

//-----------------------------------------------------------------------------
//power button was released
//-----------------------------------------------------------------------------
void PSWReleased(void)
{
  if( PWRBTN_WDTFLAG != 0x87 )
  PWRBTN_WDTFLAG = 0x00;

  if (IS_MASK_SET(cCmd,cC_TestBtnEn))
  {
    Buffer_Key(0xE0);  //
    Buffer_Key(0xF0);  //
    Buffer_Key(0x25);  //
    F_Service_SEND = 1;
  }

  PSW_COUNTER = T_PSWOFF;  // initialize counter
  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      break;

    case SYSTEM_S3 :
    case SYSTEM_S0 :
      PM_PWRBTN_HI();
      break;

    default :
      break;
  }
}
//-----------------------------------------------------------------------------
//power button was overrided
//-----------------------------------------------------------------------------
void PSWOverrided(void)
{
  if( Read_EC_PWRBTN() )
  {
    pPWRRSTMark1 = 0x78;
    pPWRRSTMark2 = 0x87;
  }
  else
  {
    pPWRRSTMark1 = 0x00;
    pPWRRSTMark2 = 0x00;
  }
  if(!Read_LID_SW_IN())
  { return; }
#if chPWSW1WDT
	if (IS_MASK_CLEAR(pProject4,pPWSWdisable))  //power button for 7s reset-2015/7/17 
 	{
#endif  //chPWSW1WDT	
	  if (SystemIsS0 || SystemIsS3)
	  {
	    if (Read_EC_PWRBTN())
	    {
	      PSW_COUNTER--;
	      if (PSW_COUNTER == 0)
	      {
	        PWSeqStep = 1;
	        PowSeqDelay = 0x00;
	        SysPowState=SYSTEM_S0_S5;
	      }
	    }
	    else
	    { PSW_COUNTER = T_PSWOFF; }   // initialize counter
#if chPWSW1WDT		
	  }
#endif  //chPWSW1WDT	  
	}
}



//-----------------------------------------------------------------------------
//LAN_WAKE was low
//-----------------------------------------------------------------------------
void LanWakeLow(void)
{
#if Support_EC_LANWake
  #if Support_G3ToS5_LANWake
  #else 
//  if (uReserve07.fbit.nPmeWakeEN==0) return;
  if ((!WOLACWakeEnGet)
     #if Support_TYPE_C
  	 && IS_MASK_CLEAR(TypeCStatus2,DockingEvent)
     #endif
     )
  {return;}
  #endif
  if ((!WOLDCWakeEnGet) && IS_MASK_CLEAR(SYS_STATUS,AC_ADP)
     #if Support_TYPE_C  	
  	 && IS_MASK_CLEAR(TypeCStatus2,DockingEvent)
     #endif
     )
  {return;}                // DC mode

  #if Support_vPro
  if((!Read_LAN_PWR())||(!Read_EC_ON()))
  {
    return;
  }
  #endif
  if ((WOLDCWakeEnGet) ||(WOLACWakeEnGet))
  	{
  switch (SysPowState)
  {
    case SYSTEM_S4 :
    case SYSTEM_S5 :
        #if Support_TYPE_C		
    	if(IS_MASK_SET(TypeCStatus2,DockingEvent))
    	{
    		CLEAR_MASK(TypeCStatus2,DockingEvent);
    		EvDockingWOLOn;
    	}
    	else
        #endif			
    	{
      		PMEWakeEn;
	    }
      EC_ON_HI();
      EC_WAKE_OUT_LOW();
      #if Support_G3ToS5_LANWake

	  #else 
	  	#if (PLATFORM_CHIP == AMD)
      		if ((WOLDCWakeEnGet) ||(WOLACWakeEnGet))
	  	#endif	
	  #endif
	  {
        PWSeqStep = 1;
        PowSeqDelay = 0x00;
        SysPowState=SYSTEM_S5_S0;
      }
      break;
    case SYSTEM_S3 :
        #if Support_TYPE_C			
    	if(IS_MASK_SET(TypeCStatus2,DockingEvent))
    	{
    		CLEAR_MASK(TypeCStatus2,DockingEvent);
    		EvDockingWOLOn;
    	}
    	else
        #endif			
    	{
      		PMEWakeEn;
    	}
      #if SW_ISCT
      if ( IS_MASK_SET(AOAC_STATUS, ISCT_Enable) )  // Check ISCT enable?
      {
        PM_PWRBTN_LOW();
        AOAC_STATUS |= 0x80;  // Set PME(LAN) wake function.
      }
      else
      #endif // SW_ISCT
      {
        EC_WAKE_OUT_LOW();
      }
      break;

    case SYSTEM_S0 :
      break;

    default :
      break;
  }
  	}
#endif  // Support_EC_LANWake
}

//-----------------------------------------------------------------------------
//LAN_WAKE was hi
//-----------------------------------------------------------------------------
void LanWakeHi(void)
{
#if Support_EC_LANWake
//  if (uReserve07.fbit.nPmeWakeEN==0) return;

  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      break;

    case SYSTEM_S3 :
      EC_WAKE_OUT_HI();
      break;

    case SYSTEM_S0 :
      EC_WAKE_OUT_HI();
      break;

    default :
      EC_WAKE_OUT_HI();
      break;
  }
#endif  // Support_EC_LANWake
}

//-----------------------------------------------------------------------------
//WLAN_WAKE was low
//-----------------------------------------------------------------------------
void WLanWakeLow(void)
{
#if Support_WLAN_WAKE
//  if (uReserve07.fbit.nPmeWakeEN==0) return;
  if (!WOLACWakeEnGet)
    return;
  if ((!WOLDCWakeEnGet)&& IS_MASK_CLEAR(SYS_STATUS,AC_ADP) )
    return;                // DC mode

  switch (SysPowState)
  {
    case SYSTEM_S4 :
    case SYSTEM_S5 :
//      PMEWakeEn;
      EC_ON_HI();

      PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState=SYSTEM_S5_S0;
      break;

    case SYSTEM_S3 :
//      PMEWakeEn;
#if SW_ISCT
      if ( IS_MASK_SET(AOAC_STATUS, ISCT_Enable) )  // Check ISCT enable?
      {
//        PM_PWRBTN_LOW();
//        AOAC_STATUS |= 0x80;  // Set PME(LAN) wake function.
        if(IS_MASK_CLEAR(APSHDDFLAG,b7RTCWAKE))
        {
          ISCT_Timer = 0;
          ISCT_OPEN_APS();
          APSHDDFLAG = 0;
          SET_MASK( APSHDDFLAG , b6PMEWAKE);
          CLEAR_MASK( APSHDDFLAG , b7RTCWAKE);
          ISCT_Again = 2;  // Last 2 Sec count for ISCT used.
          SET_MASK( APSHDDFLAG , APSHDDPRT_MIN);
        }
      }
      else
#endif // SW_ISCT
      {
        //if ( IS_MASK_SET(SYS_STATUS,AC_ADP) )
        { PM_PWRBTN_LOW(); } // Set PME(LAN) wake function.
      }
      break;

    case SYSTEM_S0 :
      break;

    default :
      break;
  }
#endif //Support_WLAN_WAKE  
}

//-----------------------------------------------------------------------------
//WLAN_WAKE was hi
//-----------------------------------------------------------------------------
void WLanWakeHi(void)
{
#if Support_WLAN_WAKE
//  if (uReserve07.fbit.nPmeWakeEN==0) return;

  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      break;

    case SYSTEM_S3 :
      PM_PWRBTN_HI();
      break;

    case SYSTEM_S0 :
      PM_PWRBTN_HI();
      break;

    default :
      PM_PWRBTN_HI();
      break;
  }
#endif //Support_WLAN_WAKE  
}

//-----------------------------------------------------------------------------
// battery 1 in function
//-----------------------------------------------------------------------------
void Battery1In(void)
{
  BattExistEn;    //set battery flag                          
  FirstGetBatData();          //first get battery data
  SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);     
  if( (BatSMbusFailCount != 0) && ((ChargeCurrH==0x00)&&(ChargeCurrL==0x00)) )
  {
#if (BatLowVot_Rule == LowVot_3S4S)     //3S and 4S battery
    ChargeCurrL = 0x00;      // 256mA
    ChargeCurrH = 0x01;
	if(IS15Inch){
      ChargeVoltL = ChargeVoltL_L_15;        
      ChargeVoltH = ChargeVoltL_H_15;
	}else{		
      ChargeVoltL = ChargeVoltL_L;        
      ChargeVoltH = ChargeVoltL_H;
	}
#endif

#if (BatLowVot_Rule == LowVot_LGNotLG)   // 3S battery
    ChargeCurrL = 0x80;      // 128mA
    ChargeCurrH = 0x00;
    ChargeVoltL = ChargeVoltL_L;        
    ChargeVoltH = ChargeVoltL_H;
#endif
    SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATWake);
  }
  SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATChange);
  Service_Auth_Step = 1;        // start SHA1
  CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
  CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
  CLEAR_MASK(pEdgeProjectFlag2,pEdgeBATTUnder20);
  CLEAR_MASK(pEdgeProjectFlag2,pEdgeBATTOver20);
  //SetPowerBatteryparameter();
  //Mos: Clean Get_Batt_debounce_count to make it check battery data stable or not?
  Get_Batt_debounce_count = 0;
  if(((Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStartCharge)!= 0)&&(Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStopCharge) != 0))
    &&((Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStartCharge)<= 0x64)&&(Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStopCharge)<= 0x64)))
  {
    pEdgeBATStartCharge = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStartCharge);
    pEdgeBATStopCharge = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStopCharge);
  }
  else{
    pEdgeBATStartCharge = 95;
    pEdgeBATStopCharge = 100;
  }

  cBATTThrottling = 0;
  CLEAR_MASK(BatteryAlarm,ADPOUT);
  CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);
  
  SET_MASK(uAuthenFlag,uDF_SWstart);
  #if (PLATFORM_CHIP == AMD)
  SmartCharge = 0x01;
  if(bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, 0x26, &SmartCharge,SMBus_NeedPEC))
  {
    BatSMbusFailCount++;
  }
  #endif
  CLEAR_MASK(TWR00,AuthenticationFailed);
  NonGenuineBattDis;  // BIOS check this flag for SW auth result. Clear this flag will let BIOS check pass.
#if (CHARGER_IC == BQ25700)
  ChargerIC_Init = 0;
#endif
#ifdef OEM_Hook_Battery1In_support
    OEM_Hook_Battery1In();
#endif // OEM_Hook_Battery1In_support 
}

//-----------------------------------------------------------------------------
// battery 1 out function
//-----------------------------------------------------------------------------
void Battery1Out(void)
{
  PORT_BYTE_PNTR BAT1_pntr;

  BattExistDis; 

  CLEAR_MASK(CHGIC_ReadCmd0x12L, IDPM_EN);

  H_PROCHOT_HI();
  //VGA_AC_BATT_OFF();

  nBatteryStatH =0;
  nBattAverTemp =0;
  nBattTempCnt =0;
  nBattOvrTempCnt =0;
  nBattErrorCnt =0;
  nStopChgStat3L =0;
  nStopChgStat3H =0;
  
	pFKTemp = 0;
	Batt0x3CFailRetry5sTimer = 0;

  // Set P-State highest to force that ChkBattery_OCP will decress P-State step by step.
  cBATTThrottling = CPUMAXPSTATE;
  cGPUThrottling = GPUMAXPSTATE;
  SET_MASK(BatteryAlarm,ADPOUT);
  SET_MASK(GPUAlarm,GPUAlarm_ADPOUT);
  ADPI2Sec = 200;
  GPU2Sec = 200;
  // Send QEvent to Bios
  ThrottlingControl();

  BAT1_pntr = &ManufacturerName;  // Base on address 0x048F.
  for( ITempW01=0; ITempW01<=16; ITempW01++ )  // Clear 048F~049F.
  {
    *BAT1_pntr=0;
    BAT1_pntr++;
  }
  RelativeStateOfChgL = 0x00;
  RelativeStateOfChgH = 0x00;
  TWR_BATClear();
  IBMbatteryStore();
  CLEAR_MASK(pEdgeProjectFlag1,pEdgeProjectBATWake);
  RST_ChgTimeOutCnt();
  StopChgIC();

  Battdata_ready = 0;
  BatSMbusFailCount = 0;
  nBattErrorCnt = 0;
  BatteryAlarm = 0;
  SHA1failCnt = 0;
  Bat1_FPChgFlag = 0;
  SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);
  SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATChange);
  CLEAR_MASK(TWR06,HWauthSupport);

#if Lenovo_Authentication
  CLEAR_MASK(edgeProject4,uHWAuthTruePass);
  CLEAR_MASK(edgeProject4,uECAuthTruePass);
  CLEAR_MASK(edgeProject4,uAuthComplete);
  CLEAR_MASK(edgeProject4,uLENOVO_Error);
  CLEAR_MASK(edgeProject4,uLenovo_JapanError);
  CLEAR_MASK(edgeProject4,uSWDone);
  uAuthenFlag = 0;
  uAuthenNPack = 0;
  CLEAR_MASK(nStopChgStat3H,NotLenovoBattery);
  CLEAR_MASK(TWR00,AuthenticationFailed);
  //CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
  CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
  CLEAR_MASK(BattAuthStat,HWauthFail);
  CLEAR_MASK(u4PriBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.

  BattAuthStat = 0;
  uAuthenFailCnt = 0;
//  p2AuthFailCnt = 0;

  CLEAR_MASK(uAuthenFlag,uDF_SWstart);
  Batteryin_delay = 20 ;
  uThresholdSetting = 0; //Clear Battery Threhold setting
  #endif
  Update_ChargeIC(); 
  #if (PLATFORM_CHIP == AMD)
  SmartCharge = 0x00;
  if(bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, 0x26, &SmartCharge,SMBus_NeedPEC))
  {
    BatSMbusFailCount++;
  }
  #endif
  #if (CHARGER_IC == BQ25700)
  ChargerIC_Init = 0;
  #endif
#ifdef OEM_Hook_Battery1Out_support
    OEM_Hook_Battery1Out();
#endif // OEM_Hook_Battery1Out_support 
}

//  +
const BYTE code LCtrl_Pressed[] = {0x14,0x00};

void KBFNPressed(void)
{
  union KEY key;

  //
  // Send scancode by KSI:04 & KSO:0E which coresponding 0x8E in CS13X standard keyboard matrix
  //
  key.field.input = 0x04;
  key.field.output = 0x0E;
  key.field.debounce_status = 1;
  Send_Scan2(key, MAKE_EVENT);

//   DebugCombineKey=0x01;
  SET_MASK(ComboKeyState, ComboKeyValid);   // ComboKeyValid = TRUE;
  ComboKeyState++;      // Start count, bit0:4 for counter use.
  LastComboKey = 0x8E;  // Fn
}

const BYTE code LCtrl_Released[] = {0xF0,0x14,0x00};

void KBFNReleased(void)
{
  union KEY key;

  //
  // Send scancode by KSI:04 & KSO:0E which coresponding 0x8E in CS13X standard keyboard matrix
  //
  key.field.input = 0x04;
  key.field.output = 0x0E;
  key.field.debounce_status = 1;
  Send_Scan2(key, BREAK_EVENT);

//   CombineKeyRN=0x00;
  ComboKeyState &= 0x80;  // Clear ComboKeyState except ECDebugMode. ComboKeyValid = FALSE;
  LastComboKey = 0xFF; // Clear
}
//  -

// ----------------------------------------------------------------------------
// Device insert/remove debounce routine.
// input:   device_id
// ----------------------------------------------------------------------------

const struct sDebounce code Debounce_TBL[] =
{
   // REG         // Flag          // Cunter          // Timer            // press        // release     // nochange
  {&EVT_STATUS1  ,wait_PSW_off    ,&DEBOUNCE_CONT1   ,T_PSW_DEBOUNCE     ,PSWPressed     ,PSWReleased   ,PSWOverrided   },  //0
  {&EVT_STATUS1  ,adapter_in      ,&DEBOUNCE_CONT2   ,T_AC_DEBOUNCE      ,AdapterIn      ,AdapterOut    ,NullEvent      },  //1
  {&EVT_STATUS1  ,bat_in          ,&DEBOUNCE_CONT3   ,T_BAT_DEBOUNCE     ,Battery1In     ,Battery1Out   ,NullEvent      },  //2
  {&EVT_STATUS2  ,DummyFlag       ,&DEBOUNCE_CONT4   ,T_EXTEVT_DEBOUNCE  ,NullEvent      ,NullEvent     ,NullEvent      },  //3
  {&EVT_STATUS1  ,LAN_WAKE_Status ,&DEBOUNCE_CONT5   ,T_LAN_WAKE_DEBOUNCE,LanWakeLow     ,LanWakeHi     ,NullEvent      },  //4
  {&EVT_STATUS1  ,KB_FN_Status    ,&DEBOUNCE_CONT6   ,T_KBFN_DEBOUNCE    ,KBFNPressed    ,KBFNReleased  ,NullEvent      },  //5
  {&EVT_STATUS2  ,DummyFlag       ,&DEBOUNCE_CONT7   ,T_EXTEVT_DEBOUNCE  ,NullEvent      ,NullEvent     ,NullEvent      },  //6
  {&EVT_STATUS1  ,WLAN_WAKE_Status,&DEBOUNCE_CONT8   ,T_LAN_WAKE_DEBOUNCE,WLanWakeLow    ,WLanWakeHi    ,NullEvent      },  //7
  {&EVT_STATUS2  ,DummyFlag       ,&DEBOUNCE_CONT9   ,T_EXTEVT_DEBOUNCE  ,NullEvent      ,NullEvent     ,NullEvent      },  //8
  {&EVT_STATUS2  ,DummyFlag       ,&DEBOUNCE_CONT10  ,T_EXTEVT_DEBOUNCE  ,NullEvent      ,NullEvent     ,NullEvent      },  //9
};

// ----------------------------------------------------------------------------
// EventManager function
//
// Note :
//  If want to call the function of status not change, please set nochange_func = 1
// ----------------------------------------------------------------------------
#pragma OT(8, SPEED)
void EventManager(BYTE device_id)
{
  BYTE new_state,old_state;
  BYTE device_flag;
  //BYTE do_nochange_func = 0x00;
  if(device_id > EventMaxNumber)
    {
        return;
    }
  switch(device_id)
  {
    case 0 :
      if (((Battdata_ready == 0xFF)&&(!Read_AC_IN()||Read_ACOFF()))||
        (IS_MASK_SET(SysStatus2,PWRBTN_press_retry)))
      {
        new_state = !Read_EC_PWRBTN();
        old_state = Read_EC_PWRBTN();
      }
      else
      { 
      	if( PWRBTN_WDTFLAG == 0x87 )
      	{
      		if( Read_EC_PWRBTN() )
      		{
      			new_state = 0;
      		}
    			else
    			{
    				PWRBTN_WDTFLAG = 0x00;
    			}
      	}
    		else
    		{
            new_state = Read_EC_PWRBTN(); 
        }
      }
      break;

    case 1 :
      new_state = Read_AC_IN();
      break;

    case 2 :
      if((BATTEMP<0x390))
      { new_state = 1; }
      else
      { new_state = 0; }
      break;

    case 3 :
      new_state = 0x00;
      break;

    case 4 :
#if  Support_EC_LANWake
      new_state = Read_LAN_WAKE_IN();
#else
      new_state = 0x00;
#endif  // Support_EC_LANWake
      break;

    case 5 :
      //  +
#if Support_EC_KBFN
      if( EC_KBFNON() )
      {
        new_state = 1;
      }
      else
      {
        new_state = 0;
      }
#else
      new_state = 0x00;
#endif  // Support_KBFN
      break;

    case 6 :
      new_state = 0x00;
      break;

    case 7:
#if Support_WLAN_WAKE      
      new_state = Read_WLAN_WAKE_IN();
#else
      new_state = 0x00;
#endif
      break;

    case 8:
      new_state = 0x00;
      break;

    case 9:
      new_state = 0x00;
      break;

    default :
      new_state = 0x00;
      break;
  }

  Tmp_XPntr = Debounce_TBL[device_id].REG;
  Tmp_XPntr1 = Debounce_TBL[device_id].Cunter;
  device_flag = Debounce_TBL[device_id].Flag;

  old_state = ((*Tmp_XPntr & device_flag) != 0x00);
  if(new_state != old_state)
  {
    if((*Tmp_XPntr1) == 0x00)
    {
      (*Tmp_XPntr1) = Debounce_TBL[device_id].Time;
    }
    else
    {
      (*Tmp_XPntr1) --;
      if((*Tmp_XPntr1) == 0)
      {
        if(new_state)
        {
          (Debounce_TBL[device_id].press)();
          *Tmp_XPntr |= device_flag;
        }
        else
        {
          (Debounce_TBL[device_id].release)();
          *Tmp_XPntr  &= (~device_flag);
        }
      }
    }
  }
  else
  {
    *Tmp_XPntr1 = 0x00;
  }

  (Debounce_TBL[device_id].nochange)();
}
