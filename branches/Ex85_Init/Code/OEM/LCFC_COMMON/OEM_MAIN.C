/*-----------------------------------------------------------------------------
 * TITLE: OEM_MAIN.C - OEM specific code
 *
 * Some "HOOKS" from CHIPHOOK.C are copied into this file in order to replace
 * (or augment) core functions.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//------------------------------------------------------------
// Hook 1ms events
// EventId is 0 ~ 9 cycle
//------------------------------------------------------------
void Hook_Timer1msEvent(IBYTE EventId)
{
#if Support_EC_MainPwr
  if ( !EC_MainPwr() && SystemIsS0 )
  {
    PWSeqStep = 1;
    PowSeqDelay = 0x00;
    SysPowState = SYSTEM_S0_S5;
	  SET_MASK(PWRCONTROL,MAINPWRONTrig);
  //  DPWROK_EC_LOW();
  }
#endif  //Support_EC_MainPwr

#if NV_GC6
  GC6Process();
#endif // NV_GC6
  PollingBIOS80Port();
  EventManager(EventId);      // Polling system event
  Oem_SysPowerContrl();       // System Power Control
  SetVGA_AC_DET();
  if ( !Read_ENBKL_IN() )
  { BKOFF_OFF(); }
  
  #if NV_GC6
  if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeGC6EventHI) )
  {
    if( !Read_GC6Event_ON() )  // GC6_event Hi
    {
      if( GC6EventCnt < 3 )
      {
        GC6EventCnt++;
      }
      else
      {
        CLEAR_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
        GC6EventCnt = 0;
      }
    }
    else
    {
      GC6EventCnt = 0;
    }
  }
#endif // NV_GC6

#ifdef OEM_Hook_Timer1msEvent_support
    OEM_Hook_Timer1msEvent();
#endif // OEM_Hook_Timer1msEvent_support
  if (UCSI_50CMD_WA != 0)
	  {
		UCSI_50CMD_WA = 0;
		  Hook_ACPICommand();
		  //RamDebug(0x88);
	  }

  Command_Process_To_Host();
}

//------------------------------------------------------------
// Hook 5ms events
//------------------------------------------------------------
void Hook_Timer5msEvent(void)
{
#if !EN_PwrSeqTest

#if Support_LenovoAPS
#if Support_GSNS_I2C
  ScanGsensorI2C();
	
#if Support_GSNS_VS_PWR
	if(StableDelay && Read_SUSP())
	{	//G-sensor power source is Vs, need delay after Vs turn on.
		StableDelay--;
	}
#endif   //Support_GSNS_VS_PWR

#else
  ScanGsensorADC();
#endif

  if(CRCDelay!=0){
    CRCDelay--;
    if(CRCDelay==0){
      unlockRFIDProtect();
    }
  }

  LnvApsProtect();
#endif  
#endif

#if Support_KBD_BL_LED
	KbdBacklightLED();
#endif //Support keyboard backlight

#ifdef OEM_Hook_Timer5msEvent_support
    OEM_Hook_Timer5msEvent();
#endif // OEM_Hook_Timer5msEvent_support
  
}

//------------------------------------------------------------
// Hook 10ms events
//------------------------------------------------------------
void Hook_Timer10msEventA(void)
{
#if Support_ADC_BoardID
  ScanADC_BoardID();
#endif

  Backlight_Control();
 	if( tmpSSKeyCnt > 0 )
  	{
  	  tmpSSKeyCnt--;
	  if(tmpSSKeyCnt == 0)
	  {
  		simple_code(tmpSSKey2, tmpEvent);  
	  }
  	}
#if !EN_PwrSeqTest
  if (PWRBTN_pressCnt !=0)
  {
    PWRBTN_pressCnt--;
    if (PWRBTN_pressCnt==0)
    {
      PM_PWRBTN_HI();
      EC_WAKE_OUT_HI();
    }
  }
  if ((LID_readytime != 0) && (Read_EC_ON()))
  { LID_readytime--; }
#endif

  PowerMonitor();
#if Support_COMPAL_MFG_TOOL
  ThermalMailBox();
#endif

  if( IS_MASK_SET(pEdgeProjectFlag2,pEdgeACCUTOFF) )
  {
    CLEAR_MASK(BatteryAlarm, HybridSTPP);
    CLEAR_MASK(BatteryAlarm, DISTURBO);
    CLEAR_MASK(nStopChgStat3H, HybridModeStopChg);
    CLEAR_MASK(nStopChgStat3H, ENADPTHROTTLING);
   // CLEAR_MASK(pEdgeProjectFlag2, pEdgeACCUTOFF);
  }

#if Lenovo_Authentication
  if( (IS_MASK_SET(uAuthenFlag, uDF_SWstart))
    && !PriBattInhibGET && (pFKTemp<30) )
  LAuthProcess();
#endif
  if( pPWRRSTCnt > 0 )
  {
    pPWRRSTCnt--;
    if( pPWRRSTCnt == 0x00 )
    {
      OTP_RESET_LOW();
    }
  }

  #if (PLATFORM_CHIP == AMD)
  if (pAMDPWRBTNCnt > 0)
  {
    if(pAMDPWRBTNCnt >= (AMDPWRBTNWK - AMDPWRBTNPULSE)) {
      PM_PWRBTN_LOW();
    } else {
      PM_PWRBTN_HI();
    }

    pAMDPWRBTNCnt--;
    if( SystemNotS0 ) {
      pAMDPWRBTNCnt = 0x00;
    } else if ( pAMDPWRBTNCnt == 0x00 ) {
      RSMRST_shutdown();
    }
  }
  #endif // PLATFORM_CHIP
  
  if(GSensorSelfTestCountX)
  {
	GSensorSelfTestCountX--;
  }

#ifdef OEM_Hook_Timer10msEventA_support
    OEM_Hook_Timer10msEventA();
#endif // OEM_Hook_Timer10msEventA_support

#if Support_TYPE_C
	//if(Read_EC_ON2_3V())
	if(Read_EC_ON_5V())
	{// Need check Type-C IC is working, HW design the power source is Valw(EC_ON control).
    OEM_TYPE_C_HOOK();
	SENT_PD_RETRY_ReceiveByte();
	SENT_UNIT_RETRY_ReceiveByte();
	SendPdoSet_ReceiveByte();
	Sinkpower_ReceiveByte();
  }
	else
		{
		TypeCStatus=0;
		TypeCProcessStep=0;
		RTS5455WaitingPingID = 0;
		TypeCAdpID = 0;
	  CurrentTypeCV = 0;
	  CurrentTypeCI = 0;
	  CurrentTypeCWatt = 0;
		}
#if Support_UCSI 
if(SystemIsS0)
  UcsiHousekeeper(0x01);
#endif
	
#endif //Support_TYPE_C	
}

//------------------------------------------------------------
// Hook 10ms events
//------------------------------------------------------------
void Hook_Timer10msEventB(void)
{
#if !EN_PwrSeqTest
  ScanADCChannel();
  Chk_Wrong_ADP();
#endif

#if EC_Brightness_Ctrl
  Brightness_Control(); // EC control brightness.
#endif  // EC_Brightness_Ctrl
#if 0 //BROOKEWORKAROUD
  if( Read_AC_IN()) {
    if (IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady) && (AdapterID != AdapterID_NON_SUPPORT))
    {
      ACEn;
      ACattachedEn;
      CLEAR_MASK( u4PeriSTS3, UNSUPPORTED_AC );
    }
    else
    {
      ACDis;
      ACattachedDis;
      if (Read_AC_IN()) {
        SET_MASK( u4PeriSTS3, UNSUPPORTED_AC );
      }
    }
  }
#endif
  TWR_Initial();

  if( IS_MASK_SET(pEdgeProjectFlag2,pEdgeACINOUTTrigger) && IS_MASK_SET(SYS_STATUS,AC_ADP) )
  {
    if (IS_MASK_SET(u4SenseSTS0, ACSts)) {
      ECSMI_SCIEvent(ACPI_ACIN_SCI);
    } else {
//      ECSMI_SCIEvent(ACPI_ACOUT_SCI);
    }
    CLEAR_MASK(pEdgeProjectFlag2,pEdgeACINOUTTrigger);
  }

  if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady) )
  {
    if(IS_MASK_SET(SYS_STATUS,AC_ADP) )//&& (RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge)) //Check Battery over 20%
    {
#if Support_TYPE_C
	if(!Read_DCIN_ATTACHED2() || (Read_DCIN_ATTACHED2() && (InputCurrent_Watt == ACCutoff_Watt)))
#endif
	{
      if( ADPI_AvgData > WATT_TO_ADC_BASE(ACACOFFEn, &OEM_ACACOFFEn) )
      {
        if( (AdpCUTCnt > ACcutoff_Ts) && (AdpCUTCnt < (ACcutoff_Ts+2)) ) 
        {
          SET_MASK(SysStatus,ERR_ShuntDownFlag);
          ProcessSID (AC_CUTOFF_ID);
         // H_PROCHOT_HI();
           //VGA_AC_BATT_OFF();
          AdpCUTCnt = ACcutoff_Ts+8;

		  	
		  #if (PLATFORM_CHIP == AMD)		  
          if( !Read_BATLEARN() )
          {
            //BAT_LEARN_ON();
          }
		  #endif

          //if( nAdpOvpGet )
          {
           // BATLEARNProcess(nAdpOvpEN);
           // ACOFF_HI();
          }
         
          SET_MASK(pEdgeProjectFlag2,pEdgeACCUTOFF);
        }
        else
        {
          AdpCUTCnt++;
        }
      }
      else if(ADPI_AvgData<137)
      {
      CLEAR_MASK(pEdgeProjectFlag2, pEdgeACCUTOFF);
        AdpCUTCnt = 0;
      }
      }
    }
  }
  else
  {
    AdpCUTCnt = 0;
  }

  if(Read_EC_ON()&& (PCHRSTCNT >= 2))
  {
//	RSMRST_HI();
  }

  if(!BattExistGet && AttachedPowerNOTsupportGET)
  {
    SET_MASK(SysStatus,ERR_ShuntDownFlag);
    ProcessSID(NOT_SUPPORT_AD_ID);  // 0x2A shutdown ID.
  	RSMRST_shutdown();
  }
#if Support_vPro
  if((Read_EC_ON()) && (PCHRSTCNT >= 13))
  {
	  if((!Read_PM_SLP_LAN())
	  	&&((!WOLACWakeEnGet)||((!WOLDCWakeEnGet)&& IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))
	  &&(IS_MASK_CLEAR(SYS_MISC1,G3Mode))
		&&(SystemNotS0))
	  {
		LAN_PWROFF();  // Turn off LAN power.
	  }
	  else{
		LAN_PWRON();  // Turn on LAN power. 	 
	  }
  }
#endif

  if(Read_EC_ON())
  {
	if(PCHRSTCNT < 13)
	{
		PCHRSTCNT++;
	}
  }
  else
  {
    PCHRSTCNT = 0;
  }
  
#ifdef OEM_Hook_Timer10msEventB_support
    OEM_Hook_Timer10msEventB();
#endif // OEM_Hook_Timer10msEventB_support

}

//------------------------------------------------------------
// Hook 50ms events A
//------------------------------------------------------------
void Hook_Timer50msEventA(void)
{
 
    TPG_LED();
  if( pACDCCnt != 0 )
    pACDCCnt--;

#ifdef OEM_Hook_Timer50msEventA_support
    OEM_Hook_Timer50msEventA();
#endif // OEM_Hook_Timer50msEventA_support
  
}

//------------------------------------------------------------
// Hook 50ms events B
//------------------------------------------------------------
void Hook_Timer50msEventB(void)
{
  BeepControl();
  Keyboard_beep();
#if Support_Tamper_function
  Tamper_DET();  // Check HDD/SSD/ODD detect process.
#endif //Support_Tamper_function

#ifdef OEM_Hook_Timer50msEventB_support
    OEM_Hook_Timer50msEventB();
#endif // OEM_Hook_Timer50msEventB_support

}

//------------------------------------------------------------
// Hook 50ms events C
//------------------------------------------------------------
void Hook_Timer50msEventC(void)
{
  MuteProcess();
#if !EN_PwrSeqTest

  Chk_FAN_RPM_Control();  // Check FAN is AP control.
#endif

#ifdef OEM_Hook_Timer50msEventC_support
    OEM_Hook_Timer50msEventC();
#endif // OEM_Hook_Timer50msEventC_support


#if Support_TYPE_C
  if(!Read_DCIN_ATTACHED2()){
    if(IS_MASK_CLEAR(ACTypeCAlarm,BIT1)){
      SET_MASK(ACTypeCAlarm,BIT1);
      CLEAR_MASK(ACTypeCAlarm,BIT0);
      H_PROCHOT_HI();
      //VGA_AC_BATT_OFF();
      cBATTThrottling = CPUMAXPSTATE;
      cGPUThrottling = GPUMAXPSTATE;
      SET_MASK(BatteryAlarm,ADPOUT);
      SET_MASK(GPUAlarm,GPUAlarm_ADPOUT);
      ADPI2Sec = ADPI_Sec;
      GPU2Sec = GPU_Sec;
      inhibit2sec = inhibit_sec;
      ThrottlingControl();
    }
  }else{
    if(IS_MASK_CLEAR(ACTypeCAlarm,BIT0)){
      CLEAR_MASK(ACTypeCAlarm,BIT1);
      SET_MASK(ACTypeCAlarm,BIT0);
      cBATTThrottling = 0;
      CLEAR_MASK(BatteryAlarm,ADPOUT);
      CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);
      ADPI2Sec = 0;
      inhibit2sec = 0;
    }
  }
  #else
    if(IS_MASK_CLEAR(ACTypeCAlarm,BIT1)){
      SET_MASK(ACTypeCAlarm,BIT1);
      CLEAR_MASK(ACTypeCAlarm,BIT0);
      H_PROCHOT_HI();
      //VGA_AC_BATT_OFF();
      cBATTThrottling = CPUMAXPSTATE;
      cGPUThrottling = GPUMAXPSTATE;
      SET_MASK(BatteryAlarm,ADPOUT);
      SET_MASK(GPUAlarm,GPUAlarm_ADPOUT);
      ADPI2Sec = ADPI_Sec;
      GPU2Sec = GPU_Sec;
      inhibit2sec = inhibit_sec;
      ThrottlingControl();
    	}
  #endif

}

//------------------------------------------------------------
// Hook 100ms events A
//------------------------------------------------------------
void Hook_Timer100msEventA(void)
{
#if !EN_PwrSeqTest
#if WDT_Support
  ResetInternalWDT();
#endif

  if (BattExistGet && !PriBattInhibGET )
  {
    if( IS_MASK_SET(uProject0,uFUdayClr) )
    {
      uSMBusData0 = 0;
      uSMBusData1 = 0;
      if(bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, FirstUsedDate, &uSMBusData0,SMBus_NeedPEC))
      {
        CLEAR_MASK(uProject0,uFUdayClr);
        OptMfgFunction1H = 0;
        OptMfgFunction1L = 0;
      }
    }
    OEM_PollingBatData_TASK();
  }
  else
  { CLEAR_MASK(StatusKeeper, BatteryProtectCHG); }

  //AOU_Main();  // Charger IC process center.

  UpdateEdgeNameSpace();

#if SW_ISCT
  ISCT_SECtoChk();
#endif  // SW_ISCT
#endif  // EN_PwrSeqTest
  AOU_Main();
#ifdef OEM_Hook_Timer100msEventA_support
    OEM_Hook_Timer100msEventA();
#endif // OEM_Hook_Timer100msEventA_support
}

//------------------------------------------------------------
// Hook 100ms events B
//------------------------------------------------------------
void Hook_Timer100msEventB(void)
{
#if !EN_PwrSeqTest
#if Support_WebCam
  CameraProcess();
#endif  // Support_WebCam
  WirelessProcess();    // Control WLAN and BT.
#if Support_IFFS
  IFFSProcess();
#endif // Support_IFFS
#endif // !EN_PwrSeqTest

#ifdef OEM_Hook_Timer100msEventB_support
    OEM_Hook_Timer100msEventB();
#endif // OEM_Hook_Timer100msEventB_support
}

//------------------------------------------------------------
// Hook 100ms events C
//------------------------------------------------------------
void Hook_Timer100msEventC(void)
{
#if !EN_PwrSeqTest
  Oem_Fan_Speed();
  ThrottlingControl();
  Battery100ms();
  if (TouchPadCount !=0)
  { TouchPadCount--; }
#endif
  if(AOURSTCNT)
  {
    AOURSTCNT--;
  }
#if Support_EnhancedPcBeep   
  OEM_ENHANCED_PC_BEEP_HOOK();
#endif   //Support_EnhancedPcBeep
  
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL, SMBus_NoPEC);
  CHGIC_ReadTempH = 0x02;
  CHGIC_ReadTempL = 0xC0;
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  if(IS_MASK_CLEAR(BATTUPDATEFW,PriBattInhib))
  {
  	bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L, SMBus_NoPEC);
	CLEAR_MASK(CHGIC_ReadCmd0x12L,IADP_RATIO);
  	SET_MASK(CHGIC_ReadCmd0x12H,SwitchingFreq);   //Set frequence to 800KHz
  	CLEAR_MASK(CHGIC_ReadCmd0x12H,AUDIO_Freq);
  	CLEAR_MASK(CHGIC_ReadCmd0x12H,OTG_ON_ChrgOK);
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  	{
    	CHGIC_SMbusFailCnt++;
  	}

	bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L, SMBus_NoPEC);
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_0);  
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_1);
  	CLEAR_MASK(CHGIC_ReadCmd0x33H,ICRIT_2);
  	CLEAR_MASK(CHGIC_ReadCmd0x33H,ICRIT_3);
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_4);
  	CLEAR_MASK(CHGIC_ReadCmd0x33L,PROCHOT_HOST_EN);
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L,SMBus_NoPEC))
  	{
   		CHGIC_SMbusFailCnt++;
  	}
	
  	bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L, SMBus_NoPEC);
  	CLEAR_MASK(CHGIC_ReadCmd0x30L,FET_LATCHOFF);
	CHGIC_ReadCmd0x30H &= 0x13;      //bit 10~15 : 0
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L,SMBus_NoPEC))
  	{
    	CHGIC_SMbusFailCnt++;
  	}
  }
#ifdef OEM_Hook_Timer100msEventC_support
    OEM_Hook_Timer100msEventC();
#endif // OEM_Hook_Timer100msEventC_support
}

//------------------------------------------------------------
// Hook 500ms events A
//------------------------------------------------------------
void Hook_Timer500msEventA(void)
{
#if !EN_PwrSeqTest
#if  Support_SMTADP
  Edge_SMTADPProcess();
#endif
    if (BattExistGet)
    {
    Chk_BatSMbusFailCount();

    if (Read_AC_IN() && IS_MASK_CLEAR(BATTUPDATEFW,PriBattInhib))
    {
        WriteSmartChgIC();
      ReadSmartChgIC();
    }
    }
      if (Read_AC_IN() && IS_MASK_CLEAR(BATTUPDATEFW,PriBattInhib))
      {
          Update_ChargeIC();
      }
  ThmIC_Temperature();
  if(IS_MASK_SET(BATTUPDATEFW, PriBattInhib))
    SET_MASK(StatusKeeper, BatteryFwUpdate);
  if(IS_MASK_SET(StatusKeeper, BatteryFwUpdate) && IS_MASK_CLEAR(BATTUPDATEFW, PriBattInhib))
  {
    CLEAR_MASK(StatusKeeper, BatteryFwUpdate);
    FirstGetBatData();
    uAuthenFlag = 0;
    Service_Auth_Step = 1;        // start SHA1
  }

#endif  // EN_PwrSeqTest
#if Thinkpad_Battery_HW_Auth
  if(DesignCapL !=0 && VoltageH != 0)
  {
    if (BattExistGet)
    { LV_BAT_Authentication(); }
  }
#endif  // Thinkpad_Battery_HW_Auth

#ifdef OEM_Hook_Timer500msEventA_support
    OEM_Hook_Timer500msEventA();
#endif // OEM_Hook_Timer500msEventA_support
}

//------------------------------------------------------------
// Hook 500ms events B
//------------------------------------------------------------
void Hook_Timer500msEventB(void)
{
#if !EN_PwrSeqTest

#if SW_ISCT
  ISCT_Process();
#endif  // SW_ISCT

  if( (pWorkAround > 1) && (SystemIsS5) )
  { pWorkAround--; }

#if Support_HUAshutdown
  if( (SysPowState == SYSTEM_S5_S0) )
  {
    if( cPWRONFail <= PWRFAILCnt )
    {
      cPWRONFail++;
    }
    else
    {
      SET_MASK(SysStatus,ERR_ShuntDownFlag);
      ProcessSID(POWERSEQFAIL_ID);
      cPWRONFail = 0x00;
      PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState = SYSTEM_S0_S5;
      ACIN_BLINK3 = 45; // LED blink 3times in 2.5sec
    }
  }
#endif

#endif

#ifdef OEM_Hook_Timer500msEventB_support
    OEM_Hook_Timer500msEventB();
#endif // OEM_Hook_Timer500msEventB_support

}

//------------------------------------------------------------
// Hook 500ms events C
//------------------------------------------------------------
void Hook_Timer500msEventC(void)
{
#if !EN_PwrSeqTest
  if(IS_MASK_CLEAR(SysStatus,LidKBIgnore))
  { Lid_Process(); }

  // Check KBC Output Buffer Full
  if (IS_MASK_SET(KBHISR,OBF)&&(SystemIsS0))
  {
    KB_OBF_count++;
    if ((KB_OBF_count & 0x0F) > 10)        // OBF=1 over 5sec?
    {
      KB_OBF_count = KB_OBF_count + 0x10;    // high nibble for count times of clear
      KB_OBF_count &= 0xF0;
      GENIRQ = 0x0C;
    }
  }
  else
  { KB_OBF_count &= 0xF0; }

  IBMbatteryStore();
  CHK_Capacity_trip_point();
#endif  // EN_PwrSeqTest
#if (CHARGER_IC == BQ25700)
  if(ChargerIC_Init <= 4){
    ChargeIC_InitACDC();
    ChargerIC_Init++;
  }
#endif
#ifdef OEM_Hook_Timer500msEventC_support
    OEM_Hook_Timer500msEventC();
#endif // OEM_Hook_Timer500msEventC_support
}

//------------------------------------------------------------
// Hook 1sec events A
//------------------------------------------------------------
void Hook_Timer1SecEventA(void)
{
#if !EN_PwrSeqTest
  Battery1Sec();
  IBMChargeCondition();   //(JSN)

  if (BattExistGet)
  {BatteryFirstUseDay();}

  if ( SystemIsS0 )
  {
    if ( (VGA_TempDly != 0) && (IS_MASK_SET(SYS_MISC1, ACPI_OS)) )
    { VGA_TempDly--; }
  }

  if ( SystemIsS5 )
  {
    if (ECSleepCount !=0)
    { ECSleepCount--; }
  }

#if SW_ISCT
  ISCT_MinutetoSec();
#endif  // SW_ISCT

  if (CountSecAfterPswPressed < 255)
    CountSecAfterPswPressed++;

  if( pInhitTimer )
  {
    if( pInhitTimer != 0xFFFF) {
      pInhitTimer--;
    }

    if ( pInhitTimer == 0 ) {
      PriBattinhibitDis;
      Inhibit_chargingCLR;  
      CLEAR_MASK(nStopChgStat3L,ENCMDSTOPCHG);
    }
  }
  if ((Batt0x3CFailRetry5sTimer != 0xFF) && (Batt0x3CFailRetry5sTimer > 0)) {
    Batt0x3CFailRetry5sTimer--;
    if (Batt0x3CFailRetry5sTimer == 0) {
      pFKTemp = 0; // Clear pFKTemp to make SW auth re-run again.
      Service_Auth_Step = 1;
      Batt0x3CFailRetry5sTimer = 0xFF; // Disable retry timer
    }
  }
#endif

#ifdef OEM_Hook_Timer1SecEventA_support
    OEM_Hook_Timer1SecEventA();
#endif // OEM_Hook_Timer1SecEventA_support
}

//------------------------------------------------------------
// Hook 1sec events B
//------------------------------------------------------------
void Hook_Timer1SecEventB(void)
{
#if !EN_PwrSeqTest
  SystemAutoOn();
  ChkBattery_abnormal();

  if(IS_MASK_CLEAR(BATTUPDATEFW,PriBattInhib) && IS_MASK_SET(EVT_STATUS1,bat_in))
  {
    bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr,C_FW_Status,&FirmwareUpdateStatusL,SMBus_NoPEC);
    bRSMBusBlock(SMbusChB,SMbusRBK,SmBat_Addr,C_BAT_Info,&BatterymakerIDL);
  }
  OEM_VGA_Hook();

#endif

#if Support_UCSI 
	UcsiFuelStateChanged(0);
#endif

#ifdef OEM_Hook_Timer1SecEventB_support
    OEM_Hook_Timer1SecEventB();
#endif  // OEM_Hook_Timer1SecEventB_support

//BIOS WDT for workaround
	if((BIOS_WDT_Cnt > 1)&&(BIOS_WDT_Cnt < 4))
	{
		if(SysPowState == SYSTEM_S5)
		{
			BIOS_WDT_Cnt--;
			if(BIOS_WDT_Cnt == 1)
			{
				BIOS_WDT_Cnt = 0;
	      PWSeqStep = 1;
	      PowSeqDelay = 0x00;
	      SysPowState=SYSTEM_S5_S0;
	    }
	  }		
	}
	else if(BIOS_WDT_Cnt == 4)
	{
		BIOS_WDT_Cnt = 3;
    SET_MASK(SysStatus,ERR_ShuntDownFlag);
    ProcessSID(BIOS_WDT_ID);  // 0x28 shutdown ID.
    RSMRST_shutdown();		
	}
	else if(BIOS_WDT_Cnt > 4)
	{
		BIOS_WDT_Cnt--;
	}
//BIOS WDT for workaround end
}

//------------------------------------------------------------
// Hook 1sec events C
//------------------------------------------------------------
void Hook_Timer1SecEventC(void)
{
  DetectFanId();  // Detect FAN vendor first.
                  // Warning!! This step must be front of other fan related functions.
  Oem_Thermal_Control();  // Read CPU temperature by PECI.
#if !EN_PwrSeqTest
  Fan_control_update();
  if ( IS_MASK_CLEAR(uReserve07,nFanManual))
  {
#if FAN_TABLE_Already
      Oem_Fan_Control();
#else	//Need recovery PROCHOT to low after power protect feature "PROCHOT set high for adaptor plug-out".
     //H_PROCHOT_LOW();
     if(!Read_SUSP())
     	{
 	H_PROCHOT_HI(); //David- For BT Lose
     	}
    nRealThermalPinDIS;
      CLEAR_MASK(pPROCHOT, b0Thermal_PRCOHOTon);  // for AP display.	
#endif  // FAN_TABLE_Already
  }
 OEM_Throttling_Ctrl();
#if FAN_TABLE_Already
  OEM_Throttling_Ctrl();
#endif  // FAN_TABLE_Already
#endif  // !EN_PwrSeqTest
if(WorkaroundforBIOSSMI!=0)
	{
	WorkaroundforBIOSSMIcunt++;
	if(WorkaroundforBIOSSMIcunt==30)
	{
ECSMI_SCIEvent(0xff);
WorkaroundforBIOSSMI=0;
	
	}
	}
else
{WorkaroundforBIOSSMIcunt=0;
}
#ifdef OEM_Hook_Timer1SecEventC_support
    OEM_Hook_Timer1SecEventC();
#endif  // OEM_Hook_Timer1SecEventC_support
}

//------------------------------------------------------------
// Hook1min events
//------------------------------------------------------------
void Hook_Timer1MinEvent(void)
{
#if !EN_PwrSeqTest
  Chk_CHG_TimeOut();

#if SW_ISCT
  ISCT_TimerCnt();
#endif  // SW_ISCT
#endif  // !EN_PwrSeqTest

#ifdef OEM_Hook_Timer1MinEvent_support
    OEM_Hook_Timer1MinEvent();
#endif  // OEM_Hook_Timer1MinEvent_support
}

//------------------------------------------------------------
// service_OEM_1
//------------------------------------------------------------
void service_OEM_1(void)
{

}

//------------------------------------------------------------
// service_OEM_2
//------------------------------------------------------------
void service_OEM_2(void)
{

}

//------------------------------------------------------------
// service_OEM_3
//------------------------------------------------------------
void service_OEM_3(void)
{

}

//------------------------------------------------------------
// service_OEM_4
//------------------------------------------------------------
void service_OEM_4(void)
{

}

//------------------------------------------------------------
//
//------------------------------------------------------------
void Hook_main_service_H(void)
{

}

//------------------------------------------------------------
//
//------------------------------------------------------------
void Hook_main_service_L(void)
{

}

//------------------------------------------------------------
// Timer event control function
// Return :
//  Only_Timer1msEvent : Only Timer1msEvent and Hook_Timer1msEvent function
//  All_TimerEvent : All timer events are OK.
//------------------------------------------------------------
BYTE Hook_Only_Timer1msEvent(void)
{

// Return :
//  Only_Timer1msEvent : Only Timer1msEvent and Hook_Timer1msEvent function
//  All_TimerEvent : All timer events are OK.
    return(All_TimerEvent);
}

//----------------------------------------------------------------------------
// OEM_SkipMainServiceFunc
//  Note:
//      1. Always return(0xFF|Normal_MainService) to run normal main_service function.
//      2. If you don't understand the use of OEM_SkipMainServiceFunc function, don't change anything.
//----------------------------------------------------------------------------
BYTE OEM_SkipMainServiceFunc(void)
{

    // Return :
    return(Normal_MainService);
}


//-----------------------------------------------------------------------------
const BYTE code SEG7_Table[]=
{
  /*            a
               ---------
               |       |
           f   |   g   |  b
              ---------
               |       |
           e   |       |  c
               ---------
                   d      . h
     */
  // COMMON ANODE SEVEN SEGMENT DISPLAY TABLE
    //  0    1     2     3     4     5     6     7
       0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    //  8     9     A     b     C     d     E     F
       0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E,
};

//-----------------------------------------------------------------------------
// Out7SegLED
//-----------------------------------------------------------------------------
void Out7SegLED(BYTE bData)
{
  BYTE bIndex = 7;

//  while(bIndex < -1)
  while(bIndex != -1)
  {
    if (bData & BIT(bIndex))
    { DEBUG_TxDAT_HI(); }
    else
    { DEBUG_TxDAT_LO(); }

    DEBUG_RxCLK_LO();
    DEBUG_RxCLK_HI();
    bIndex--;
  }
}

//-----------------------------------------------------------------------------
// Dark7SegLed
//-----------------------------------------------------------------------------
void Dark7SegLed(void)
{
  Out7SegLED(0xFF);  // dark LED
  Out7SegLED(0xFF);  // dark LED
}


//-----------------------------------------------------------------------------
// P80LedOut
//-----------------------------------------------------------------------------
void P80LedOut(BYTE bData)
{
  BYTE code * data_pntr;

//  if(FgDebugCarIn)              // if card dose not exist, return
  {
    // out upper byte
    data_pntr = SEG7_Table;          // get 7-seg table
    data_pntr +=((bData & 0xF0) >> 4);    // mask upper byte and shit 4 bytes  then get data of table

    BRAM3A = *data_pntr;
    Out7SegLED(*data_pntr & ~(GET_ECPORT80 << 7));          // display upper value.

    // out lower byte
    data_pntr = SEG7_Table;          // get 7-seg table
    data_pntr += (bData & 0x0F);       // mask lower byte then get data of table.

    BRAM3B = *data_pntr;

    Out7SegLED(*data_pntr);          // dispaly lower value.
  }
}


//-----------------------------------------------------------------------------
// PollingBIOS80Port
//-----------------------------------------------------------------------------
void PollingBIOS80Port(void)
{
  BRAM3D++;
  if(BRAM3F != BRAM3E)        // if no data in , return.
  {  // clear it.
    EC_TX_ALT;
    printf("\rP80h : 0x%02bX   \n\r",BRAM3F);
    EC_TX_INPUT;
    BRAM3E=  BRAM3F;          // clear it.
    DelayXms(1);
/*
    DelayXms(1);

    EC_RX_OUTPUT;
    EC_TX_OUTPUT;
        P80LedOut(BRAM3F);        // to show it
        EC_RX_ALT;
    EC_TX_ALT;
*/

    Port80_Data[Port80_Index] = BRAM3F;
    Port80_Index++;

    if ( Port80_Index >= (sizeof(Port80_Data)/sizeof(Port80_Data[0])) ) // Range from 0 to 14
    { Port80_Index = 0; }

  }
  else
  {
    if( IS_MASK_CLEAR(CombineKeyStatus, b2FnRNL) )  // 20130607 Log
    {
      if(BRAM3F != BRAM3C)      // if no data in , return.
      {
        BRAM3C= BRAM3F;        // clear it.
            //GCR15 &= 0x3F;
        if ( (SystemNotS0&&IS_MASK_CLEAR(CombineKeyStatus, b2FnRNL)) || IS_MASK_CLEAR(pEdgeProjectFlag1, pEdgeProjectDebugIn) )  // 20130607 Log
        {
          EC_RX_INPUT;
          EC_TX_INPUT;
          return;
        } else {
          EC_RX_OUTPUT;
          EC_TX_OUTPUT;
          P80LedOut(BRAM3F);      // to show it
          //GCR15 |= 0xC0;
          EC_RX_INPUT;
          EC_TX_INPUT;
        }
      }
    }
  }
}

void ProcessSID(BYTE SID)  // Shutdown ID.
{
  pLastSID4 = pLastSID3;
  pLastSID3 = pLastSID2;
  pLastSID2 = pLastSID;
  if ((SID == S5_ID) && (BIOS_SID != 0)){      //normal shutdown && BIOS shutdown != 0 => BIOS shutdown
    BIOS_SID_REC = BIOS_SID;
    BIOS_SID = 0;
    pLastSID = BIOS_ID;    
  } else {                                     //normal S5 shutdown
    pLastSID  = SID;
  }
#if WDT_Support
  DisableInternalWDT();
#endif
  Update_16ByteEEPROM();
#if WDT_Support
  EnableInternalWDT();
#endif
}

