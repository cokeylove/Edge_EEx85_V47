 /*-----------------------------------------------------------------------------
 * Filename:OEM_MEMORY.C     For Chipset: ITE.IT85XX
 *
 * Function: Memory Definition for extern all code base reference
 *
 * [Memory Map Description]
 *
 * Chip Internal Ram : 0x00-0xFF For Kernel and Chip Level use
 *
 ***************************************************
 * Chip 8500       : External ram 0x000-0x7FF
 * Chip Other 85XX : External ram 0x000-0xFFF
 ***************************************************
 *
 * [OEM Memory Rang]
 * [External Ram]
 * 0x100-0x1FF   OEM RAM
 * 0x200-0x2FF   OEM RAM SPI buffer array
 * 0x300-0x3FF   OEM RAM EC Space
 * 0x400-0x4FF   OEM RAM
 * 0x500-0x5FF   OEM RAM

 * 0x800-0x8FF   OEM RAM keyboard matrix
 * 0x900-0x9FF   OEM RAM
 * 0xA00-0xAFF   OEM RAM
 * 0xB00-0xBFF   OEM RAM
 * 0xC00-0xCFF   OEM RAM
 * 0xD00-0xDFF   OEM RAM
 * 0xE00-0xEFF   OEM RAM
 * 0xF00-0xFFF   OEM RAM For HSPI ram code function / RamDebug
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x100-0x1FF   OEM RAM 1
//-------------------------------------------------------------------------------
//*******************************************************************************
XBYTE  SysPowState              _at_(OEMRAM1+0x00); 
XBYTE  PWSeqStep                _at_(OEMRAM1+0x01); 
XBYTE  DeepSleepCunt            _at_(OEMRAM1+0x02); 
XWORD  DelayDoPowerSeq          _at_(OEMRAM1+0x03); 
XWORD  PowSeqDelay              _at_(OEMRAM1+0x05); 
XBYTE  KB_S3Dly                 _at_(OEMRAM1+0x07);
XBYTE  CombineKeyStatus         _at_(OEMRAM1+0x08);   
XBYTE  ComboKeyState            _at_(OEMRAM1+0x09); 
XBYTE  LastComboKey             _at_(OEMRAM1+0x0A); 
XBYTE  KB_OBF_count             _at_(OEMRAM1+0x0B);
XBYTE  CmdData2                 _at_(OEMRAM1+0x0C);
XBYTE  uVGATurboFun             _at_(OEMRAM1+0x0D); 
XBYTE  eFlashVarifyOK           _at_(OEMRAM1+0x0E);
XBYTE  eFlash_r_data            _at_(OEMRAM1+0x0F);
XBYTE  eFlashA1                 _at_(OEMRAM1+0x10);
XBYTE  eFlashA0                 _at_(OEMRAM1+0x11);
XBYTE  eEEPROMBank              _at_(OEMRAM1+0x12);
XBYTE  eEEPROMAddrsss           _at_(OEMRAM1+0x13);
XBYTE  eEEPROMData              _at_(OEMRAM1+0x14);
XBYTE  eEEPROMMarkData          _at_(OEMRAM1+0x15);
XBYTE  e256ByteCnt              _at_(OEMRAM1+0x16);
XBYTE  eUpdateEEPROMCnt         _at_(OEMRAM1+0x17);
XBYTE  eUpdatepDevStusCnt       _at_(OEMRAM1+0x18);
XBYTE  WSMbusTemp01             _at_(OEMRAM1+0x19);
XBYTE  WSMbusTemp02             _at_(OEMRAM1+0x1A);
XBYTE  SMbusFailCnt2            _at_(OEMRAM1+0x1B);
XBYTE  SMbusFailCnt3            _at_(OEMRAM1+0x1C);
XBYTE  GPIO_STATUS              _at_(OEMRAM1+0x1D);
XBYTE  BatSMbusFailCount        _at_(OEMRAM1+0x1E);
XBYTE  Batpollstep1             _at_(OEMRAM1+0x1F);
XBYTE  CHGIC_SMbusFailCnt       _at_(OEMRAM1+0x20);
XBYTE  CHGIC_ptr                _at_(OEMRAM1+0x21);
XBYTE  CHGIC_InputCurrentL      _at_(OEMRAM1+0x22);
XBYTE  CHGIC_InputCurrentH      _at_(OEMRAM1+0x23);
XBYTE  CHGIC_ReadCmd0x14L       _at_(OEMRAM1+0x24);
XBYTE  CHGIC_ReadCmd0x14H       _at_(OEMRAM1+0x25);
XBYTE  CHGIC_ReadCmd0x15L       _at_(OEMRAM1+0x26);
XBYTE  CHGIC_ReadCmd0x15H       _at_(OEMRAM1+0x27);
XBYTE  CHGIC_ReadCmd0x3FL       _at_(OEMRAM1+0x28);
XBYTE  CHGIC_ReadCmd0x3FH       _at_(OEMRAM1+0x29);
XWORD  BATTEMP                  _at_(OEMRAM1+0x2A);                            
XBYTE  BatLowCnt                _at_(OEMRAM1+0x2C);
XBYTE  Chk_Trickle_Current_status      _at_(OEMRAM1+0x2D);
XBYTE  Chk_Trickle_Current_count       _at_(OEMRAM1+0x2E);
XWORD  Chk_Trickle_Current_Now_Current _at_(OEMRAM1+0x2F); 
XWORD  Chk_Trickle_Current_Chg_Current _at_(OEMRAM1+0x31); 
XBYTE  Get_Batt_debounce_count         _at_(OEMRAM1+0x33);
XBYTE  Get_Batt_debounce_hash1         _at_(OEMRAM1+0x34);
XBYTE  Get_Batt_debounce_hash2         _at_(OEMRAM1+0x35); 
XBYTE  Bat0x0BFakeCnt                  _at_(OEMRAM1+0x36);
XBYTE  BatteryAlarm                    _at_(OEMRAM1+0x37);
XBYTE  CHGIC_ReadCmd0x12L              _at_(OEMRAM1+0x38);
XBYTE  CHGIC_ReadCmd0x12H              _at_(OEMRAM1+0x39);
XBYTE  BattTemp                        _at_(OEMRAM1+0x3A);
XBYTE  inhibit2sec                     _at_(OEMRAM1+0x3B);
XBYTE  Battdata_ready                  _at_(OEMRAM1+0x3C);
XWORD  TrickleChgTimeOutCnt            _at_(OEMRAM1+0x3D);
XWORD  FastChgTimeOutCnt               _at_(OEMRAM1+0x3F);
XBYTE  RSMshutdownCnt                  _at_(OEMRAM1+0x41);
XBYTE  ChkBattery_abnormal_status      _at_(OEMRAM1+0x42);
XBYTE  ChkBattery_abnormal_count       _at_(OEMRAM1+0x43);
XBYTE  LOWBATT_3TIMES                  _at_(OEMRAM1+0x44);
XBYTE  Chk_Wrong_ADP_Status            _at_(OEMRAM1+0x45);
XBYTE  AdapterID                       _at_(OEMRAM1+0x46);
XWORD  API_ID                          _at_(OEMRAM1+0x47);
XWORD  Chk_Wrong_10ms_Count            _at_(OEMRAM1+0x49);
XBYTE  Chk_ACOP_Status                 _at_(OEMRAM1+0x4B);
XWORD  Chk_ACOP_Bat_Chg_Current        _at_(OEMRAM1+0x4C);
XWORD  Chk_ACOP_10ms_Count             _at_(OEMRAM1+0x4E);
XBYTE  ACIN_FallINT_Count              _at_(OEMRAM1+0x50);
XBYTE  ACIN_BLINK3                     _at_(OEMRAM1+0x51);
XBYTE  MaxPwrCtrlFsm                   _at_(OEMRAM1+0x52);
XBYTE  Chk_Hybrid_STPP_min_BattGasgauge   _at_(OEMRAM1+0x53);
XWORD  Chk_Hybrid_STPP_Batt_Output_Power  _at_(OEMRAM1+0x54);
XWORD  Chk_Hybrid_STPP_Batt_Output_Power_Limit    _at_(OEMRAM1+0x56);
XBYTE  BackLight_En_Delay              _at_(OEMRAM1+0x58);
XBYTE  Batteryin_delay                 _at_(OEMRAM1+0x59);
XBYTE  uProject0                       _at_(OEMRAM1+0x5A);   // byte
XBYTE  CRCDelay                        _at_(OEMRAM1+0x5B);
XBYTE  FAN_PCH_Max                     _at_(OEMRAM1+0x5C);   // byte
XBYTE  RPM_PCH_Max                     _at_(OEMRAM1+0x5D);
XBYTE  CHGIC_Status                    _at_(OEMRAM1+0x5E);
XBYTE  pPWRLEDS3_off_Count             _at_(OEMRAM1+0x5F);
         
XWORD  ADPI_Data[4]                    _at_(OEMRAM1+0x60);   //0x60 ~ 0x67
XWORD  XWTemp1                         _at_(OEMRAM1+0x68);
XBYTE  MpcSystemTotalPower             _at_(OEMRAM1+0x6A);
XBYTE  ADPI2Sec                        _at_(OEMRAM1+0x6B);
XBYTE  MpcCP                           _at_(OEMRAM1+0x6C);
XBYTE  MpcCpuTurboDisablePoint         _at_(OEMRAM1+0x6D);
XBYTE  MpcCpuTurboRecoverPoint         _at_(OEMRAM1+0x6E);
XBYTE  MpcCpuThrottlingPoint           _at_(OEMRAM1+0x6F);
XBYTE  MpcCpuThrottlingRecoverPoint    _at_(OEMRAM1+0x70);
XBYTE  MpcGpuThrottlingPoint           _at_(OEMRAM1+0x71);
XBYTE  MpcGpuThrottlingRecoverPoint    _at_(OEMRAM1+0x72);
XBYTE  MpcAcCpuTurboDisablePoint       _at_(OEMRAM1+0x73);
XBYTE  MpcAcCpuTurboRecoverPoint       _at_(OEMRAM1+0x74);
XBYTE  MpcAcCpuThrottlingPoint         _at_(OEMRAM1+0x75);
XBYTE  MpcAcCpuThrottlingRecoverPoint  _at_(OEMRAM1+0x76);
XBYTE  MpcAcGpuThrottlingPoint         _at_(OEMRAM1+0x77);
XBYTE  MpcAcGpuThrottlingRecoverPoint  _at_(OEMRAM1+0x78);
XWORD  ADPI_AvgData                    _at_(OEMRAM1+0x79);
XBYTE  pProject4                       _at_(OEMRAM1+0x7B); 
XWORD  PSW_COUNTER                     _at_(OEMRAM1+0x7C); 
XBYTE  DSxPowState                     _at_(OEMRAM1+0x7E); 
XBYTE  AOU_status                      _at_(OEMRAM1+0x7F); 
#if (PLATFORM_CHIP == INTEL)           
XWORD  SUSACK_LOW_CNT                  _at_(OEMRAM1+0x80); 
XBYTE  SUSACK_HI_CNT                   _at_(OEMRAM1+0x82);
#endif // PLATFORM_CHIP                
XBYTE  SLP_A_CNT                       _at_(OEMRAM1+0x83);
XBYTE  S3S4DelayCnt                    _at_(OEMRAM1+0x84);
XBYTE  cADPIDTemp                      _at_(OEMRAM1+0x85);
XBYTE  TPID1                           _at_(OEMRAM1+0x86);
XBYTE  TPID2                           _at_(OEMRAM1+0x87);
XBYTE  TPID3                           _at_(OEMRAM1+0x88);
XBYTE  TPID4                           _at_(OEMRAM1+0x89);
XBYTE  p2BeepCnt1                      _at_(OEMRAM1+0x8A);
XBYTE  p2BeepCnt2                      _at_(OEMRAM1+0x8B);
XBYTE  p2BeepCnt3                      _at_(OEMRAM1+0x8C);
XBYTE  p2BeepCtrl                      _at_(OEMRAM1+0x8D);
XBYTE  FnStatus                        _at_(OEMRAM1+0x8E);
XBYTE  SysStatus                       _at_(OEMRAM1+0x8F);
XBYTE  LID_DEBOUNCE_CNT                _at_(OEMRAM1+0x90);
XBYTE  PWRBTN_pressCnt                 _at_(OEMRAM1+0x91);
XBYTE  LID_readytime                   _at_(OEMRAM1+0x92);
XBYTE  ExtTimeCnt                      _at_(OEMRAM1+0x93);
XBYTE  SysStatus2                      _at_(OEMRAM1+0x94);
XBYTE  LEDcontrol_bak                  _at_(OEMRAM1+0x95);
XBYTE  Led_status                      _at_(OEMRAM1+0x96);
XBYTE  Led_PWM_step                    _at_(OEMRAM1+0x97);
XBYTE  PowerLed_Count                  _at_(OEMRAM1+0x98);                                       
XBYTE  LENOVOPMFW_Temp                 _at_(OEMRAM1+0x99);                                      
XBYTE  pFKTemp                         _at_(OEMRAM1+0x9A);
XBYTE  Batt0x3CFailRetry5sTimer        _at_(OEMRAM1+0x9B);                                     
XBYTE  cCmd                            _at_(OEMRAM1+0x9C);
XBYTE  cKBStatus                       _at_(OEMRAM1+0x9D);
XBYTE  cBattFlag0                      _at_(OEMRAM1+0x9E);
XBYTE  cSPIEEPROM                      _at_(OEMRAM1+0x9F);
XBYTE  Bat1_FPChgFlag                  _at_(OEMRAM1+0xA0);
/*-----------------------------------------------------------
; {Bat1_FPChgFlag} Battery 1 Fast & Pre Charging Flag
; B7 - Reserved
; B6 - Reserved
; B5 - Fast-Charge Time-Out
; B4 - Reserved
; B3 - Reserved
; B2 - Reserved
; B1 - Pre-Charge Time-Out
; B0 - Charge SMBus Failed
------------------------------------------------------------*/
XBYTE  cTargetGauge                    _at_(OEMRAM1+0xA1);
XBYTE  cPanelId                        _at_(OEMRAM1+0xA2);
XBYTE  cBrightUpDelayCnt               _at_(OEMRAM1+0xA3);
XBYTE  cBrightDnDelayCnt               _at_(OEMRAM1+0xA4);
XBYTE  cRecoveryBrightValue            _at_(OEMRAM1+0xA5);
XBYTE  cSysActionFlag                  _at_(OEMRAM1+0xA6);
XBYTE  cADPThrottling                  _at_(OEMRAM1+0xA7);
XBYTE  cTHERMThrottling                _at_(OEMRAM1+0xA8);
XBYTE  cBATTThrottling                 _at_(OEMRAM1+0xA9);
XBYTE  ISCT_Again                      _at_(OEMRAM1+0xAA);
XBYTE  pBattLearn                      _at_(OEMRAM1+0xAB);
XBYTE  pFakeCHGCnt                     _at_(OEMRAM1+0xAC);
XBYTE  S0S5PWRcodition                 _at_(OEMRAM1+0xAD);
XBYTE  pProject3                       _at_(OEMRAM1+0xAE);
XBYTE  AOURSTCNT                       _at_(OEMRAM1+0xAF);
XBYTE  EC_AOU_DBG_FLAG                 _at_(OEMRAM1+0xB0);
XBYTE  EC_AOU_PWR_TYPE                 _at_(OEMRAM1+0xB1);
XBYTE  EVT_STATUS1                     _at_(OEMRAM1+0xB2);  
XBYTE  EVT_STATUS2                     _at_(OEMRAM1+0xB3);
XBYTE  uAdpWattCnt                     _at_(OEMRAM1+0xB4);
XWORD  uAdpWattTemp                    _at_(OEMRAM1+0xB5);                                       
XBYTE  initMark                        _at_(OEMRAM1+0xB7);
XBYTE  TEMP_Error                      _at_(OEMRAM1+0xB8);
XBYTE  RFID_eFlashA1				   _at_(OEMRAM1+0xB9);
XBYTE  RFID_eFlashA0				   _at_(OEMRAM1+0xBA);
XBYTE  RFID_eEEPROMBlock	           _at_(OEMRAM1+0xBB);
XBYTE  RFID_eEEPROMAddrsss		       _at_(OEMRAM1+0xBC);
XBYTE  RFID_eEEPROMData			       _at_(OEMRAM1+0xBD);                                    
XBYTE  cPWRONFail                      _at_(OEMRAM1+0xBE);
XBYTE  SYS_MISC1                       _at_(OEMRAM1+0xBF);
XBYTE  AutoTimer                       _at_(OEMRAM1+0xC0);
XBYTE  SCI_Event_In_Index              _at_(OEMRAM1+0xC1);
XBYTE  SCI_Event_Out_Index             _at_(OEMRAM1+0xC2);
XBYTE  SCI_Event_Buffer[EVENT_BUFFER_SIZE]    _at_(OEMRAM1+0xC3);   //0x1C3 ~ 0x1CA
XWORD  ADP_I                           _at_(OEMRAM1+0xCB);
XBYTE  PM1PendingTXCount               _at_(OEMRAM1+0xCD);
XBYTE  PM1PendingRXCount               _at_(OEMRAM1+0xCE);
XBYTE  PM1DataPending[6]               _at_(OEMRAM1+0xCF);    //0x1CF~0x1D4
XBYTE  PM2PendingTXCount               _at_(OEMRAM1+0xD5);
XBYTE  PM2PendingRXCount               _at_(OEMRAM1+0xD6);
XBYTE  PM2DataPending[6]               _at_(OEMRAM1+0xD7);    //0x1D7~0x1DC
XBYTE  uCritBattWakeThre               _at_(OEMRAM1+0xDD);
#if Support_IFFS
XBYTE  uIffsCnt                        _at_(OEMRAM1+0xDE);
#endif // Support_IFFS         
#if SW_ISCT                    
XBYTE  uISCT                           _at_(OEMRAM1+0xDF);
XBYTE  uISCT_2                         _at_(OEMRAM1+0xE0);
#endif // SW_ISCT              
#if NV_GC6
XBYTE  GC6EventCnt                     _at_(OEMRAM1+0xE1);
#endif // NV_GC6                                                   
XBYTE  pACDCCnt                        _at_(OEMRAM1+0xE2);
XBYTE  pTempBank                       _at_(OEMRAM1+0xE3);
XBYTE  pTempAddress                    _at_(OEMRAM1+0xE4);                                  
XBYTE  nBattTsVolt                     _at_(OEMRAM1+0xE5);
XBYTE  cThrottlingSet                  _at_(OEMRAM1+0xE6);   // 00: Nothing, 01: Thr 12.5%, 02: Thr 25%,..,08: Thr 100%  
XBYTE	pCPUBasePStage                 _at_(OEMRAM1+0xE7);
XBYTE  PWRCONTROL                      _at_(OEMRAM1+0xE8);
XBYTE  CHGIC_ReadTempL                 _at_(OEMRAM1+0xE9);
XBYTE  CHGIC_ReadTempH                 _at_(OEMRAM1+0xEA);
XBYTE  LED_PWM_Status                  _at_(OEMRAM1+0xEB);
XBYTE  Cgr_LED_Count                   _at_(OEMRAM1+0xEC);
XBYTE  IP_GHOSTBEEP					   _at_(OEMRAM1+0xED);	// 20160407a
XBYTE  IP_GBEEPCnt			 	 	   _at_(OEMRAM1+0xEE);
#if (CHARGER_IC == BQ25700)
XBYTE  ChargerIC_Init                  _at_(OEMRAM1+0xEF);
#endif

//reserved 0x1ED ~ 0x1EF           

//===============================================================================
// OEMRAM1 Offset 0xF0 ~ 0xFF   Event debounce counter
XBYTE  DEBOUNCE_CONT1                  _at_(DebounceRAM+0x00); //(byte)
XBYTE  DEBOUNCE_CONT2                  _at_(DebounceRAM+0x01); //(byte)
XBYTE  DEBOUNCE_CONT3                  _at_(DebounceRAM+0x02); //(byte)
XBYTE  DEBOUNCE_CONT4                  _at_(DebounceRAM+0x03); //(byte)
XBYTE  DEBOUNCE_CONT5                  _at_(DebounceRAM+0x04); //(byte)
XBYTE  DEBOUNCE_CONT6                  _at_(DebounceRAM+0x05); //(byte)
XBYTE  DEBOUNCE_CONT7                  _at_(DebounceRAM+0x06); //(byte)
XBYTE  DEBOUNCE_CONT8                  _at_(DebounceRAM+0x07); //(byte)
XBYTE  DEBOUNCE_CONT9                  _at_(DebounceRAM+0x08); //(byte)
XBYTE  DEBOUNCE_CONT10                 _at_(DebounceRAM+0x09); //(byte)
XBYTE  DEBOUNCE_CONT11                 _at_(DebounceRAM+0x0A); //(byte)
XBYTE  DEBOUNCE_CONT12                 _at_(DebounceRAM+0x0B); //(byte)
XBYTE  DEBOUNCE_CONT13                 _at_(DebounceRAM+0x0C); //(byte)
XBYTE  DEBOUNCE_CONT14                 _at_(DebounceRAM+0x0D); //(byte)
XBYTE  DEBOUNCE_CONT15                 _at_(DebounceRAM+0x0E); //(byte)
XBYTE  DEBOUNCE_CONT16                 _at_(DebounceRAM+0x0F); //(byte)
                                       
//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x200-0x2FF   OEM RAM 2
//-------------------------------------------------------------------------------
//*******************************************************************************

XBYTE  pProject0                       _at_(OEMRAM2+0x00);
XBYTE  pPROCHOT                        _at_(OEMRAM2+0x01);                                      
//XBYTE  uMBID                           _at_(OEMRAM2+0x02);
XBYTE  BoardID                         _at_(OEMRAM2+0x03);
XBYTE  GPU_THROTTLE_SRC                _at_(OEMRAM2+0x04);
XWORD  ADP_ID_REC                      _at_(OEMRAM2+0x05); 
XBYTE  AC_DROP_COUNT                   _at_(OEMRAM2+0x07);                                                                              
XWORD  p2FUDayTemp                     _at_(OEMRAM2+0x08);
XWORD  p2FUDay                         _at_(OEMRAM2+0x0A);
XWORD  p2FUD_Year                      _at_(OEMRAM2+0x0C);
XBYTE  p2FUD_Month                     _at_(OEMRAM2+0x0E);
XBYTE  p2FUD_Day                       _at_(OEMRAM2+0x0F);
XBYTE  Port80_Data[15]                 _at_(OEMRAM2+0x10);  // 0x210~0x21E
XBYTE  Port80_Index                    _at_(OEMRAM2+0x1F);                                
XBYTE  pLastSID                        _at_(OEMRAM2+0x20);
XBYTE  pLastSID2                       _at_(OEMRAM2+0x21);
XBYTE  pLastSID3                       _at_(OEMRAM2+0x22);
XBYTE  pLastSID4                       _at_(OEMRAM2+0x23);
XBYTE  cPacketPowerLimit1              _at_(OEMRAM2+0x24);
XWORD  KeepBattRemineCap               _at_(OEMRAM2+0x25);
XBYTE  ChkBattery_FCCchg_count         _at_(OEMRAM2+0x27);
XBYTE  ChkBattery_FCCchg_lastFCCL      _at_(OEMRAM2+0x28);
XBYTE  ChkBattery_FCCchg_lastFCCH      _at_(OEMRAM2+0x29);
XBYTE  CountSecAfterPswPressed         _at_(OEMRAM2+0x2A);                                     
XBYTE  pVCompareFlag                   _at_(OEMRAM2+0x2B);
XWORD  TURBO_V                         _at_(OEMRAM2+0x2C);
//reserve 0x22E
 XBYTE  pEdgeProjectFlag3               _at_(OEMRAM2+0x2E);

XBYTE  pEdgeProjectFlag1               _at_(OEMRAM2+0x2F);
XBYTE  pWorkAround                     _at_(OEMRAM2+0x30);
XBYTE  pWakeCnt                        _at_(OEMRAM2+0x31);
XBYTE  pSMIEvent                       _at_(OEMRAM2+0x32);
XBYTE  pEdgeBATStopCharge              _at_(OEMRAM2+0x33);
XBYTE  pEdgeBATStartCharge             _at_(OEMRAM2+0x34);
XWORD  pInhitTimer                     _at_(OEMRAM2+0x35);
XBYTE  pEdgeProjectFlag2               _at_(OEMRAM2+0x37);
XBYTE  mPWRDelay                       _at_(OEMRAM2+0x38);                                
XWORD  ACACOFFEn                       _at_(OEMRAM2+0x39);  
#if (CHARGER_IC == BQ25700)                              
XBYTE  CHGIC_ReadCmd0x31L              _at_(OEMRAM2+0x3B);
XBYTE  CHGIC_ReadCmd0x31H              _at_(OEMRAM2+0x3C);
XBYTE  CHGIC_ReadCmd0x3EL              _at_(OEMRAM2+0x3D);
XBYTE  CHGIC_ReadCmd0x3EH              _at_(OEMRAM2+0x3E);
XBYTE  CHGIC_ReadCmd0x30L              _at_(OEMRAM2+0x3F);
XBYTE  CHGIC_ReadCmd0x30H              _at_(OEMRAM2+0x40);
XBYTE  CHGIC_ReadCmd0x33L              _at_(OEMRAM2+0x41);
XBYTE  CHGIC_ReadCmd0x33H              _at_(OEMRAM2+0x42);
XBYTE  CHGIC_ReadCmd0x34L              _at_(OEMRAM2+0x43);
XBYTE  CHGIC_ReadCmd0x34H              _at_(OEMRAM2+0x44);
#endif //BQ25700
#if (CHARGER_IC == BQ24780S)                              
XBYTE  CHGIC_ReadCmd0x37L              _at_(OEMRAM2+0x3B);
XBYTE  CHGIC_ReadCmd0x37H              _at_(OEMRAM2+0x3C);
XBYTE  CHGIC_ReadCmd0x39L              _at_(OEMRAM2+0x3D);
XBYTE  CHGIC_ReadCmd0x39H              _at_(OEMRAM2+0x3E);
XBYTE  CHGIC_ReadCmd0x3BL              _at_(OEMRAM2+0x3F);
XBYTE  CHGIC_ReadCmd0x3BH              _at_(OEMRAM2+0x40);
XBYTE  CHGIC_ReadCmd0x3CL              _at_(OEMRAM2+0x41);
XBYTE  CHGIC_ReadCmd0x3CH              _at_(OEMRAM2+0x42);
XBYTE  CHGIC_ReadCmd0x3DL              _at_(OEMRAM2+0x43);
XBYTE  CHGIC_ReadCmd0x3DH              _at_(OEMRAM2+0x44);
#endif //BQ24780S

XBYTE  pPWRRSTMark1                    _at_(OEMRAM2+0x45);
XBYTE  pPWRRSTMark2                    _at_(OEMRAM2+0x46);
XBYTE  pPWRRSTCnt                      _at_(OEMRAM2+0x47);                                   
#if (PLATFORM_CHIP == AMD)
XBYTE  pAMDPWRBTNCnt                   _at_(OEMRAM2+0x48);
#endif // PLATFORM_CHIP                                                       
XBYTE  AdpCUTCnt                       _at_(OEMRAM2+0x49);
XWORD  cCADP_ID                        _at_(OEMRAM2+0x4A);
XWORD  cCADP_I                         _at_(OEMRAM2+0x4C);
XBYTE  cCheatAD                        _at_(OEMRAM2+0x4E);
                                      
//---------------------PECI------------------------
XBYTE  PECI_TIMEOUT                    _at_(OEMRAM2+0x4F);
XBYTE  PECI_ERROR                      _at_(OEMRAM2+0x50);
XBYTE  PECIDelayCnt                    _at_(OEMRAM2+0x51);
XWORD  PECIBuffer0                     _at_(OEMRAM2+0x52);
XWORD  PECIBuffer1                     _at_(OEMRAM2+0x54);
XWORD  PECIBuffer2                     _at_(OEMRAM2+0x56);
XWORD  PECIBuffer3                     _at_(OEMRAM2+0x58);
XWORD  PECIBuffer4                     _at_(OEMRAM2+0x5A);
XWORD  PECIBuffer5                     _at_(OEMRAM2+0x5C);
XWORD  PECIBuffer6                     _at_(OEMRAM2+0x5E);
XWORD  PECIBuffer7                     _at_(OEMRAM2+0x60);                                    
XBYTE  TJMAX                           _at_(OEMRAM2+0x62);
XBYTE  PECI_Err_CNT                    _at_(OEMRAM2+0x63);
XBYTE  VGA_TempDly                     _at_(OEMRAM2+0x64);                                    
XBYTE  PECI_GET_TEMPL                  _at_(OEMRAM2+0x65);
XBYTE  PECI_GET_TEMPH                  _at_(OEMRAM2+0x66);                                    
XBYTE  ERR_THMSTS                      _at_(OEMRAM2+0x67);
XBYTE  TMErrCnt                        _at_(OEMRAM2+0x68);
XBYTE  VGAErrCnt                       _at_(OEMRAM2+0x69);
XBYTE  EXTVGAErrCnt                    _at_(OEMRAM2+0x6A);
XBYTE  TmlICStep                       _at_(OEMRAM2+0x6B);
XBYTE  PCHErrCnt                       _at_(OEMRAM2+0x6C);
XWORD  cWriteCurrentPL1                _at_(OEMRAM2+0x6D);
XWORD  cWriteCurrentPL2                _at_(OEMRAM2+0x6F);                                                                        
XBYTE  MaxCPU_MCHTemp0                 _at_(OEMRAM2+0x71);
XBYTE  MaxCPU_MCHTemp1                 _at_(OEMRAM2+0x72);
XBYTE  MaxCPU_MCHTemp2                 _at_(OEMRAM2+0x73);
XBYTE  MaxCPU_MCHTemp3                 _at_(OEMRAM2+0x74);                                    
XWORD  FAN_ID                          _at_(OEMRAM2+0x75);
XBYTE  FAN_ID_TYPE                     _at_(OEMRAM2+0x77);                                    
XBYTE  nAtmFanSpeed                    _at_(OEMRAM2+0x78);    // Fan speed. For Lenovo ATM

//---------------------FAN-------------------------
XBYTE  TEMP_Buff_1                     _at_(OEMRAM2+0x79);
XBYTE  TEMP_Buff_2                     _at_(OEMRAM2+0x7A);
XBYTE  TEMP_Buff_3                     _at_(OEMRAM2+0x7B);
XBYTE  EXT_Duty                        _at_(OEMRAM2+0x7C);
XBYTE  Target_Duty                     _at_(OEMRAM2+0x7D);
XBYTE  MBID_Reload                     _at_(OEMRAM2+0x7E);
XBYTE  Fan_ON_Count                    _at_(OEMRAM2+0x7F);
XBYTE  Fan_Debug_Temp                  _at_(OEMRAM2+0x80);
XBYTE  VGA_TBuff1                      _at_(OEMRAM2+0x81);
XBYTE  VGA_TBuff2                      _at_(OEMRAM2+0x82);
XBYTE  VGA_TBuff3                      _at_(OEMRAM2+0x83);
XBYTE  Average_Temp                    _at_(OEMRAM2+0x84);  // Average temperature stage
XBYTE  USB_Charger                     _at_(OEMRAM2+0x85);  // USB charger EN/Dis
XBYTE  MuteCount                       _at_(OEMRAM2+0x86);
XBYTE  Thro_Status                     _at_(OEMRAM2+0x87);
XBYTE  Thro_Status2                    _at_(OEMRAM2+0x88);
XBYTE  CPU_Shut_Cnt                    _at_(OEMRAM2+0x89);
XBYTE  VGA_Shut_Cnt                    _at_(OEMRAM2+0x8A);
XBYTE  FAN2PRM                         _at_(OEMRAM2+0x8B);
XBYTE  FAN_CpTarget                    _at_(OEMRAM2+0x8C);                                      
XBYTE  ManualFan2PRM                   _at_(OEMRAM2+0x8D);
XWORD  SHA1_SEED                       _at_(OEMRAM2+0x8E);                                       
XBYTE  SHA1failCnt                     _at_(OEMRAM2+0x90);
XBYTE  Service_Auth_Step               _at_(OEMRAM2+0x91);                                       
XBYTE  FANCPUSeg                       _at_(OEMRAM2+0x92);
XBYTE  FANVGASeg                       _at_(OEMRAM2+0x93);
XBYTE  FANPCHSeg                       _at_(OEMRAM2+0x94);
XBYTE  TargetRPM                       _at_(OEMRAM2+0x95);                                      
XBYTE  StatusKeeper                    _at_(OEMRAM2+0x96);
XBYTE  ECSleepCount                    _at_(OEMRAM2+0x97);                                       
XBYTE  TouchPadCount                   _at_(OEMRAM2+0x98);
XBYTE  EEPROM_PwrSts                   _at_(OEMRAM2+0x99);
XBYTE  FNKB_S3Dly                      _at_(OEMRAM2+0x9A);
XBYTE  Target_Duty_BCD                 _at_(OEMRAM2+0x9B);
XBYTE  FAN_PWM_BCD                     _at_(OEMRAM2+0x9C);
XBYTE  TargetRPM_BCD                   _at_(OEMRAM2+0x9D);
XBYTE  Real_RPM_BCD                    _at_(OEMRAM2+0x9E);
XBYTE  PWM_CHG_delay                   _at_(OEMRAM2+0x9F);
XBYTE  ManualFanPRM                    _at_(OEMRAM2+0xA0);
XBYTE  uReserve07                      _at_(OEMRAM2+0xA1);
XBYTE  FAN_Tab_OFF                     _at_(OEMRAM2+0xA2);
XBYTE  FAN_Tab_LOW                     _at_(OEMRAM2+0xA3);
XBYTE  FAN_Tab_HI                      _at_(OEMRAM2+0xA4);
XBYTE  FAN_Tab_DAC                     _at_(OEMRAM2+0xA5);
XBYTE  VFAN_Tab_HI                     _at_(OEMRAM2+0xA6);
XBYTE  VFAN_Tab_LOW                    _at_(OEMRAM2+0xA7);
XBYTE  VFAN_Tab_DAC                    _at_(OEMRAM2+0xA8);
XBYTE  THR_PRO_OFF                     _at_(OEMRAM2+0xA9);
XBYTE  THR_PRO_ON                      _at_(OEMRAM2+0xAA);
XBYTE  THR_Turo_Rem                    _at_(OEMRAM2+0xAB);
XBYTE  THR_Turo_OFF                    _at_(OEMRAM2+0xAC);
XBYTE  THR_Tab_Shut                    _at_(OEMRAM2+0xAD);
XBYTE  LENOVOPMFW                      _at_(OEMRAM2+0xAE);
XBYTE  SWITCHABLE                      _at_(OEMRAM2+0xAF);
XBYTE  SMB3_DATA[32]                   _at_(OEMRAM2+0xB0);  // 0xB0~0xCF
XBYTE  nRamTemp                        _at_(OEMRAM2+0xD0);  // Ram temperature
XBYTE  nVramTemp                       _at_(OEMRAM2+0xD1);  // V Ram temperature
XBYTE  BATTUPDATEFW                    _at_(OEMRAM2+0xD2);  // 0x11
/*                                  
0x37 (Get Battery Information )   Read Block
byte 0,1: The battery maker ID
byte 2,3: Hardware ID
byte 4,5: Firmware version (0 if the firmware is corrupted
byte 6,7: Data version (0 if this is not supported or the firmware is corrupted.
*/
XBYTE  ISCT_Timer                      _at_(OEMRAM2+0xD3);  
XBYTE  TP_Type                         _at_(OEMRAM2+0xD4);
XBYTE  AOAC_STATUS                     _at_(OEMRAM2+0xD5);    //
XBYTE  ACPI_HOTKEY                     _at_(OEMRAM2+0xD6);    //
XBYTE  SYS_STATUS                      _at_(OEMRAM2+0xD7);    //
XBYTE  ACOFF_SOURCE                    _at_(OEMRAM2+0xD8);    //
XBYTE  CURRENT_STATUS                  _at_(OEMRAM2+0xD9);    // 
XBYTE  nThrottlingAPSet                _at_(OEMRAM2+0xDA);
XBYTE  REAL_THROTTLING_INDEX           _at_(OEMRAM2+0xDB);
XBYTE  THROTTLING_HW                   _at_(OEMRAM2+0xDC);    // HW throttling define
XBYTE  nCpuTempLocal                   _at_(OEMRAM2+0xDD);    // CPU local temperature
XBYTE  PCH_Temp3                       _at_(OEMRAM2+0xDE);    // OS Shutdown Temp. For system read setting.(VGA)
XBYTE  VGA_TEMP                        _at_(OEMRAM2+0xDF);    // VGA temperature
XBYTE  EXTVGA_TEMP                     _at_(OEMRAM2+0xE0);
XBYTE  ACPI_STAT                       _at_(OEMRAM2+0xE1);
XBYTE  nBrightValue                    _at_(OEMRAM2+0xE2);    // LCD Brightness value(0-0x09)
XBYTE  nKbProjectID                    _at_(OEMRAM2+0xE3);
XBYTE  CPU_TYPE                        _at_(OEMRAM2+0xE4);
XBYTE  nBatteryStatH                   _at_(OEMRAM2+0xE5);
XBYTE  nBattAverTemp                   _at_(OEMRAM2+0xE6);    // (BYTE) Battery Average Temp.
XBYTE  nBattTempCnt                    _at_(OEMRAM2+0xE7);    // (BYTE) Battery current Temp sample counter
XBYTE  nBattOvrTempCnt                 _at_(OEMRAM2+0xE8);    // (BYTE) Count up if battery is arrived overtemp.
XBYTE  nBattErrorCnt                   _at_(OEMRAM2+0xE9);    // (BYTE) Communicatiion fail counter
XBYTE  nStopChgStat3L                  _at_(OEMRAM2+0xEA);
XBYTE  nStopChgStat3H                  _at_(OEMRAM2+0xEB);
XBYTE  rACCPAGE[8]				       _at_(OEMRAM2+0xEC);	  // 0x2EC ~ 0x2F3
XWORD  cCSTPPBATPWR                    _at_(OEMRAM2+0xF4);    // word  
XWORD  BIOS_SID                        _at_(OEMRAM2+0xF6);    // word  
XWORD  BIOS_SID_REC                    _at_(OEMRAM2+0xF8);    // word  
XBYTE  SMBusRBDataCount                _at_(OEMRAM2+0xFA);
XBYTE  I2CRBDataCount                  _at_(OEMRAM2+0xFB);
XBYTE  LimitedSupportAdpWatt           _at_(OEMRAM2+0xFC);
XBYTE  NotSupportAdpWatt               _at_(OEMRAM2+0xFD);
XBYTE  Ccb42_States                    _at_(OEMRAM2+0xFE);
XBYTE  ADPI_MUL		                   _at_(OEMRAM2+0xFF);


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x300-0x3FF   OEM RAM 3
//-------------------------------------------------------------------------------
//*******************************************************************************
XWORD  uAPSLastX                       _at_(OEMRAM3+0x00);  // word
XWORD  uAPSLastY                       _at_(OEMRAM3+0x02);  // word
XBYTE  uCountX                         _at_(OEMRAM3+0x04);  // byte
XWORD  uTotalX                         _at_(OEMRAM3+0x05);  // word
XBYTE  uCountY                         _at_(OEMRAM3+0x07);  // byte
XWORD  uTotalY                         _at_(OEMRAM3+0x08);  // word
XBYTE  uAPSThresholdX                  _at_(OEMRAM3+0x0A);  // byte
XBYTE  uAPSThresholdY                  _at_(OEMRAM3+0x0B);  // byte
XBYTE  uAPSTimeOut                     _at_(OEMRAM3+0x0C);  // byte
XBYTE  uAPStickCount                   _at_(OEMRAM3+0x0D);  // byte
XWORD  AD_Xdiff                        _at_(OEMRAM3+0x0E);  // word
XWORD  AD_Ydiff                        _at_(OEMRAM3+0x10);  // word
GSensor GS                             _at_(OEMRAM3+0x12);// 0x12 ~ 0x42

XBYTE  ADTOTALHDDCount                 _at_(OEMRAM3+0x44);  // byte
XBYTE  APSHDDFLAG                      _at_(OEMRAM3+0x45);  // byte
XBYTE  APSHDDCount                     _at_(OEMRAM3+0x46);  // byte
XWORD  X_Max180                        _at_(OEMRAM3+0x47);
XWORD  X_Min180                        _at_(OEMRAM3+0x49);
XWORD  Y_Max180                        _at_(OEMRAM3+0x4B);
XWORD  Y_Min180                        _at_(OEMRAM3+0x4D);
XWORD  X_Avg                           _at_(OEMRAM3+0x4F);
XWORD  Y_Avg                           _at_(OEMRAM3+0x51);
XWORD  GS_VOUTX                        _at_(OEMRAM3+0x53);
XWORD  GS_VOUTY                        _at_(OEMRAM3+0x55);
XBYTE  GSSmbusFailCnt                  _at_(OEMRAM3+0x57);  //G-sensor communication fail count
XBYTE  GSDeviceID                      _at_(OEMRAM3+0x58);  //G-sensor Device ID LIS3DH:0x33, LIS3DSH:0x3F, Kx023_1025:0x15
XBYTE  StableDelay                     _at_(OEMRAM3+0x59);  //Ex85 add for common code;
 
//reserved 0x35F     
XBYTE  cGPUThrottlingSet               _at_(OEMRAM3+0x5F); //VGA P-state
XWORD  Tamper_ram0                     _at_(OEMRAM3+0x60);
XBYTE  Tamper_ram1                     _at_(OEMRAM3+0x62);
XBYTE  Tamper_ram2                     _at_(OEMRAM3+0x63);

XBYTE AOU_IFG_Debounce_Cnt             _at_(OEMRAM3+0x64);

XBYTE InputCurrent_Watt                _at_(OEMRAM3+0x65);
XBYTE ACCutoff_Watt                    _at_(OEMRAM3+0x66);

XBYTE MpcCpuThrottlingPoint_L          _at_(OEMRAM3+0x67);
XBYTE MpcCpuThrottlingRecoverPoint_L   _at_(OEMRAM3+0x68);
XBYTE MpcGpuThrottlingPoint_L          _at_(OEMRAM3+0x69);
XBYTE MpcGpuThrottlingRecoverPoint_L   _at_(OEMRAM3+0x6A);

XBYTE  Recovery2Sec                    _at_(OEMRAM3+0x6B);
XBYTE  GPUAlarm                        _at_(OEMRAM3+0x6C);
XBYTE  ACTypeCAlarm                    _at_(OEMRAM3+0x6D);
XBYTE  cGPUThrottling                  _at_(OEMRAM3+0x6E);
XBYTE  GPU2Sec                         _at_(OEMRAM3+0x6F);
//reserved 0x363 ~ 0x36F

XWORD  BatLowVoltage                   _at_(OEMRAM3+0x70);
XBYTE  BIOS_WDT_Cnt                    _at_(OEMRAM3+0x72);
//reserved 0x373 ~ 0x37F

XBYTE  ENHANCED_PC_BEEP_RAM0[8]        _at_(OEMRAM3+0x80);	//0x80~0x87
XBYTE  ENHANCED_PC_BEEP_RAM1	       _at_(OEMRAM3+0x88);
XBYTE  ENHANCED_PC_BEEP_RAM2	       _at_(OEMRAM3+0x89);
XBYTE  ENHANCED_PC_BEEP_RAM3	       _at_(OEMRAM3+0x8A);
XBYTE  ENHANCED_PC_BEEP_RAM4	       _at_(OEMRAM3+0x8B);
XBYTE  ENHANCED_PC_BEEP_RAM5	       _at_(OEMRAM3+0x8C);
XBYTE  ENHANCED_PC_BEEP_RAM6	       _at_(OEMRAM3+0x8D);
XBYTE  ENHANCED_PC_BEEP_RAM7	       _at_(OEMRAM3+0x8E);  
XBYTE  ENHANCED_PC_BEEP_RAM8	       _at_(OEMRAM3+0x8F);  

XBYTE	I2C_DATA     				        _at_(OEMRAM3+0xC2);
XBYTE	I2C_DATA1[5]       				    _at_(OEMRAM3+0xC3); //0x3C3 ~ 0x3D3

XBYTE	GSensorSelfTestState                _at_(OEMRAM3+0xD4);
XBYTE	GSensorSelfTestCountX               _at_(OEMRAM3+0xD5);
XBYTE	GSensorTmpX_1                       _at_(OEMRAM3+0xD6);
XBYTE	GSensorTmpX_2                       _at_(OEMRAM3+0xD7);
XBYTE	GSensorTmpY_1                       _at_(OEMRAM3+0xD8);
XBYTE	GSensorTmpY_2                       _at_(OEMRAM3+0xD9);

XBYTE	uMBID                               _at_(OEMRAM3+0xDF);
//*** 0x3E0~0x3F7 reserved for BIOS usage
XBYTE BiosReserved3E2                  _at_(OEMRAM3+0xE2);
XBYTE FANTSSeg							_at_(OEMRAM3+0xE3);							
//XBYTE TEMP_TS_1							_at_(OEMRAM3+0xE4);								
//XBYTE TEMP_TS_2							_at_(OEMRAM3+0xE5);

XBYTE Workaround_LPCcount                   _at_(OEMRAM3+0xF8);
XBYTE Debug_Test_3F9                   _at_(OEMRAM3+0xF9);
XBYTE Debug_Test_3FA                   _at_(OEMRAM3+0xFA);
XBYTE Debug_Test_3FB                   _at_(OEMRAM3+0xFB);
XBYTE Debug_Test_3FC                   _at_(OEMRAM3+0xFC);
XBYTE Debug_Test_3FD                   _at_(OEMRAM3+0xFD);
XBYTE Debug_Test_3FE                   _at_(OEMRAM3+0xFE);
XBYTE Debug_Test_3FF                   _at_(OEMRAM3+0xFF);

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x400-0x4FF   OEM RAM 4
//-------------------------------------------------------------------------------
//*******************************************************************************
XBYTE  TYPE_C_RAM0                         _at_(OEMRAM4+0x00);    //for type-c used
XBYTE  TYPE_C_RAM1                         _at_(OEMRAM4+0x01);    //for type-c used
XBYTE  TYPE_C_RAM2                         _at_(OEMRAM4+0x02);    //for type-c used
XBYTE  TYPE_C_RAM3                         _at_(OEMRAM4+0x03);    //for type-c used
XBYTE  TYPE_C_RAM4                         _at_(OEMRAM4+0x04);    //for type-c used
XBYTE  TYPE_C_RAM5                         _at_(OEMRAM4+0x05);    //for type-c used
XBYTE  TYPE_C_RAM6                         _at_(OEMRAM4+0x06);    //for type-c used
XBYTE  TYPE_C_RAM7                         _at_(OEMRAM4+0x07);    //for type-c used
XBYTE  TYPE_C_RAM8                         _at_(OEMRAM4+0x08);    //for type-c used
XBYTE  TYPE_C_RAM9                         _at_(OEMRAM4+0x09);    //for type-c used
XWORD  TYPE_C_RAM10                        _at_(OEMRAM4+0x0A);    //for type-c used
XWORD  TYPE_C_RAM11                        _at_(OEMRAM4+0x0C);    //for type-c used
XBYTE  TYPE_C_RAM12                        _at_(OEMRAM4+0x0E);    //for type-c used
XBYTE  TYPE_C_RAM13                        _at_(OEMRAM4+0x0F);    //for type-c used
XBYTE  TYPE_C_RAM14                        _at_(OEMRAM4+0x10);    //for type-c used

XBYTE  TYPE_C_DOCKING_RAM0                 _at_(OEMRAM4+0x11);
XBYTE  TYPE_C_DOCKING_RAM1                 _at_(OEMRAM4+0x12);
XBYTE  TYPE_C_DOCKING_RAM2[4]              _at_(OEMRAM4+0x13);    //0x0413~0x0416
XBYTE  TYPE_C_RAM15[2]                     _at_(OEMRAM4+0x17);    //for type-c used
XBYTE  TYPE_C_RAM16[2]                     _at_(OEMRAM4+0x19);    //for type-c used

//UCSI
#if Support_UCSI
XWORD  UCSI_RAM0							_at_(OEMRAM4+0x20);    //for UCSI used
XWORD  UCSI_RAM1							_at_(OEMRAM4+0x22);    //for UCSI used
XBYTE  UCSI_RAM2[4]							_at_(OEMRAM4+0x24);    //for UCSI used
XBYTE  UCSI_RAM3[8]							_at_(OEMRAM4+0x28);    //for UCSI used
XBYTE  UCSI_RAM4[16]						_at_(OEMRAM4+0x30);    //for UCSI used
XBYTE  UCSI_RAM5[16]						_at_(OEMRAM4+0x40);    //for UCSI used
USB_POWER_STATUS  Usb_Pdc_Power_Status[1]	_at_(OEMRAM4+0x50);    //for UCSI used
USB_STATUS  Usb_Pdc_Status[1]				_at_(OEMRAM4+0x51);    //for UCSI used

XBYTE  UCSI_RDO1                            _at_(OEMRAM4+0x52);
XBYTE  UCSI_RDO2                            _at_(OEMRAM4+0x53);
XBYTE  UCSI_RDO3                            _at_(OEMRAM4+0x54);
XBYTE  UCSI_RDO4                            _at_(OEMRAM4+0x55);
XBYTE  UCSI_RTS5455_CMD                     _at_(OEMRAM4+0x56);
XBYTE  UCSI_RTS5455_WR_DATA_LEN             _at_(OEMRAM4+0x57);
XBYTE  UCSI_RTS5455_CMD_WAIT                _at_(OEMRAM4+0x58);
XBYTE  UCSI_RTS5455_CMD_WAIT_CNT              _at_(OEMRAM4+0x59);
XBYTE  UCSI_RTS5455_CMD_ERROR               _at_(OEMRAM4+0x5A);
XBYTE  UCSI_POWER_SOURCE              _at_(OEMRAM4+0x5B);
XBYTE  UCSI_POWER_DIRECTION_CHANGE   _at_(OEMRAM4+0x5C);
XBYTE  UCSI_DATA_ROLE              _at_(OEMRAM4+0x5D);
XBYTE  UCSI_CONNECTOR_PARTNER_CHANGE  _at_(OEMRAM4+0x5E);
XBYTE  UCSI_NEGOTIATED_POWER_LEVEL_CHANGE  _at_(OEMRAM4+0x5F);
XBYTE  UCSI_BATTERY_CHARGE_STATUS_CHANGE  _at_(OEMRAM4+0x60);
XBYTE  UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE  _at_(OEMRAM4+0x61);
XBYTE  UCSI_GET_CONNECTOR_STATUS_RTS5455[9]       _at_(OEMRAM4+0x62);  // 0x62~0x6A
XBYTE UCSI_NOTIFICATION_ENABLE[2]    _at_(OEMRAM4+0x6B);  // 0x6B~0x6C
XBYTE UCSI_OPTIONAL_FEATURE    _at_(OEMRAM4+0x6D);
XWORD  Ucsi_Connector_Change_Status[NUMBER_OF_USB_PDC]    _at_(OEMRAM4+0x6E);// 0x6E~0x6F
XBYTE UCSI_PD_RESET_COMPLETE_CHANGE     _at_(OEMRAM4+0x70);
XBYTE Ucsi_Connector_Number_Changed     _at_(OEMRAM4+0x71);
XBYTE UCSI_SUPPORTED_CAM_CHANGE    _at_(OEMRAM4+0x72);
XBYTE Ucsi_Device_Connected    _at_(OEMRAM4+0x73);
XBYTE ReTry_P2P    _at_(OEMRAM4+0x74);
XBYTE  UCSI_RTS5455_CMD_ERROR_CNT               _at_(OEMRAM4+0x75);
XBYTE  Detect_Adaptor_Watt_SKIP               _at_(OEMRAM4+0x76);
XBYTE  P2P_Charge_Disable               _at_(OEMRAM4+0x77);
XBYTE  WlanWakeStatusEEPROM       	  _at_(OEMRAM4+0x79);
 XBYTE  LENOVOP2PSTATUS      	  _at_(OEMRAM4+0x7A);
#endif
XBYTE  TYPE_C_RAM17                    _at_(OEMRAM4+0x80);    //for type-c used
XBYTE  TYPE_C_RAM18                    _at_(OEMRAM4+0x81);    //for type-c used
XBYTE  TYPE_C_RAM19                    _at_(OEMRAM4+0x82);    //for type-c used
XBYTE  TYPE_C_RAM20                     _at_(OEMRAM4+0xE5);    //for type-c used
XBYTE  TYPE_C_RAM21                     _at_(OEMRAM4+0xE6);    //for type-c used
 XBYTE  TYPE_C_RAM22                    _at_(OEMRAM4+0xE7);    //for type-c used
//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x500-0x5FF   OEM RAM 5
//-------------------------------------------------------------------------------
//*******************************************************************************
XWORD  OEM_MpcCP                            _at_(OEMRAM5+0x00);
XWORD  OEM_MpcCpuTurboDisablePoint          _at_(OEMRAM5+0x02);
XWORD  OEM_MpcCpuTurboRecoverPoint          _at_(OEMRAM5+0x04);
XWORD  OEM_MpcCpuThrottlingPoint            _at_(OEMRAM5+0x06);
XWORD  OEM_MpcCpuThrottlingRecoverPoint     _at_(OEMRAM5+0x08);
XWORD  OEM_MpcGpuThrottlingPoint            _at_(OEMRAM5+0x0A);
XWORD  OEM_MpcGpuThrottlingRecoverPoint     _at_(OEMRAM5+0x0C);
XWORD  OEM_MpcCpuThrottlingPoint_L          _at_(OEMRAM5+0x0E);
XWORD  OEM_MpcCpuThrottlingRecoverPoint_L   _at_(OEMRAM5+0x10);
XWORD  OEM_MpcGpuThrottlingPoint_L          _at_(OEMRAM5+0x12);
XWORD  OEM_MpcCP_107                        _at_(OEMRAM5+0x14);
XWORD  OEM_MpcGpuThrottlingRecoverPoint_L   _at_(OEMRAM5+0x16);
XWORD  OEM_MpcAcCpuThrottlingPoint          _at_(OEMRAM5+0x18);
XWORD  OEM_MpcAcCpuThrottlingRecoverPoint   _at_(OEMRAM5+0x1A);
XWORD  OEM_MpcAcGpuThrottlingPoint          _at_(OEMRAM5+0x1C);
XWORD  OEM_MpcAcGpuThrottlingRecoverPoint   _at_(OEMRAM5+0x1E);
XWORD  OEM_ACACOFFEn                        _at_(OEMRAM5+0x20);
LWORD  OEM_Batt_Output_Power                _at_(OEMRAM5+0x24);
#if Support_COMPAL_MFG_TOOL
XBYTE  Step0_L                       _at_(OEMRAM5+0x03);
XBYTE  Step0_H                       _at_(OEMRAM5+0x04);
XBYTE  Step0_DAC                     _at_(OEMRAM5+0x05);
XBYTE  Step1_L                       _at_(OEMRAM5+0x06);
XBYTE  Step1_H                       _at_(OEMRAM5+0x07);
XBYTE  Step1_DAC                     _at_(OEMRAM5+0x08);
XBYTE  Step2_L                       _at_(OEMRAM5+0x09);
XBYTE  Step2_H                       _at_(OEMRAM5+0x0A);
XBYTE  Step2_DAC                     _at_(OEMRAM5+0x0B);
XBYTE  Step3_L                       _at_(OEMRAM5+0x0C);
XBYTE  Step3_H                       _at_(OEMRAM5+0x0D);
XBYTE  Step3_DAC                     _at_(OEMRAM5+0x0E);
XBYTE  Step4_L                       _at_(OEMRAM5+0x0F);
XBYTE  Step4_H                       _at_(OEMRAM5+0x10);
XBYTE  Step4_DAC                     _at_(OEMRAM5+0x11);
#endif //Support_COMPAL_MFG_TOOL     
                                     
// External name space 0x5D ~ 0x5F   
#if Support_COMPAL_MFG_TOOL          
XBYTE  EXT_NAMESPACE_INDEX           _at_(OEMRAM5+0x12); 
XBYTE  EXT_NAMESPACE_BANK            _at_(OEMRAM5+0x13); 
XBYTE  EXT_NAMESPACE_DATA            _at_(OEMRAM5+0x14); 
#endif                               
                                     
// add for control in Mailbox        
#if Support_COMPAL_MFG_TOOL          
XBYTE  MMIO_rDATA_index              _at_(OEMRAM5+0x15); 
XBYTE  MMIO_CMD_Ctrl                 _at_(OEMRAM5+0x16); 
XBYTE  MMIO_rDATA[8]                 _at_(OEMRAM5+0x17);  // 0x17 ~ 0x1E
XBYTE  MMIO_iCMD                     _at_(OEMRAM5+0x1F);  
XBYTE  MMIO_iDATA8C                  _at_(OEMRAM5+0x20);  
XBYTE  MMIO_iDATA8D                  _at_(OEMRAM5+0x21);  
XBYTE  MMIO_iDATA8E                  _at_(OEMRAM5+0x22);  
XBYTE  MMIO_iDATA8F                  _at_(OEMRAM5+0x23);  
XBYTE  MMIO_iDATA90                  _at_(OEMRAM5+0x24);  
#endif                               

//0x580 ~ 0x5FF reserved for project usage


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x600-0x6FF   OEM RAM 6
//-------------------------------------------------------------------------------
//*******************************************************************************
// ITE tool usage
 
//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x700-0x7FF   OEM RAM 7
//-------------------------------------------------------------------------------
//*******************************************************************************
// Keil C

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x800-0x8FF   OEM RAM 8
//-------------------------------------------------------------------------------
//*******************************************************************************
// Keyboard matrix
XBYTE  KB_RAM_Table[128]             _at_(OEMRAM8+0x00);  // 0x00
XBYTE  ExKB_RAM_Table[24]            _at_(OEMRAM8+0x80);  // 0x80

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x900-0x9FF   OEM RAM 9
//-------------------------------------------------------------------------------
//*******************************************************************************
XBYTE  edgePassword[PWDSIZE*PWDNUM]  _at_(OEMRAM9+0x00);// 0x0900 ~ 0x097F
XBYTE  edgePasswordID                _at_(OEMRAM9+0x80);

//reserved 0x981 ~ 0x987

#if Lenovo_Authentication
XBYTE  uAuthenRand1                  _at_(OEMRAM9+0x88);
XBYTE  uAuthenRand2                  _at_(OEMRAM9+0x89);
XBYTE  uAuthenRand3                  _at_(OEMRAM9+0x8A);
XBYTE  uAuthenRand4                  _at_(OEMRAM9+0x8B);
XBYTE  uAuthenRPCL                   _at_(OEMRAM9+0x8C);
XBYTE  uAuthenRPCH                   _at_(OEMRAM9+0x8D);
XBYTE  uAuthenRPackL                 _at_(OEMRAM9+0x8E);
XBYTE  uAuthenRPackH                 _at_(OEMRAM9+0x8F);
XWORD  uAuthenNPack                  _at_(OEMRAM9+0x90);
LWORD  uAuthenNPCtemp                _at_(OEMRAM9+0x92);
XWORD  uAuthenNPC                    _at_(OEMRAM9+0x96);
XBYTE  uAuthenFlag                   _at_(OEMRAM9+0x98);
XBYTE  uAuthenReadAfter1Sec          _at_(OEMRAM9+0x99);  // EC write Challenge to battery and read after 1 sec
XBYTE  uAuthenFailCnt                _at_(OEMRAM9+0x9A);
XWORD  uAuthenIdx3rd                 _at_(OEMRAM9+0x9B);
XWORD  uAuthenNPack3rd               _at_(OEMRAM9+0x9D);
XWORD  uAuthenIdx3rdtemp             _at_(OEMRAM9+0x9F);
LWORD  uAuthenIdx3rdtemp2            _at_(OEMRAM9+0xA1);
XWORD  uAuthenNPC3rd                 _at_(OEMRAM9+0xA5);
XBYTE  uAuthenFlag3rd                _at_(OEMRAM9+0xA7);
XBYTE  uAuthenFailCnt3rd             _at_(OEMRAM9+0xA8);
XBYTE  LV_SW_SIGNERTURE[16]          _at_(OEMRAM9+0xA9);   //0x9A9 ~0x9B8 
XBYTE  edgeProject4                  _at_(OEMRAM9+0xB9);
XBYTE  p2AuthFailCnt                 _at_(OEMRAM9+0xBA);
XBYTE  pAuthCommFailCnt              _at_(OEMRAM9+0xBB);
#endif

#if Thinkpad_Battery_HW_Auth
UWORD  LV_HW_BatteryReserved6        _at_(OEMRAM9+0xBC); // 0x09BC ~ 0x09BD
UWORD  LV_HW_BatteryResponseX[4]     _at_(OEMRAM9+0xBE); // 0x09BE ~ 0x09C5
UWORD  LV_HW_BatteryResponseZ[9]     _at_(OEMRAM9+0xC6); // 0x09C6 ~ 0x09D7
XBYTE  LV_HW_BatteryControl          _at_(OEMRAM9+0xD8);
XBYTE  LV_HW_BatteryChallengeKey[18] _at_(OEMRAM9+0xD9); // 0x09D9 ~ 0x09EA
UWORD  LV_HW_BatteryCheckValue[9]    _at_(OEMRAM9+0xEB); // 0x09EB ~ 0x09FC
#endif
//reserved 0x9FD ~ 0x9FF

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xA00-0xAFF   OEM RAM A
//-------------------------------------------------------------------------------
//*******************************************************************************
XBYTE  u4Cfgspace0                   _at_(OEMRAMA+0x00);    //   0x00
XBYTE  u4Cfgspace1                   _at_(OEMRAMA+0x01);    //   0x01
XBYTE  u4Cfgspace2                   _at_(OEMRAMA+0x02);    //   0x02
XBYTE  u4Cfgspace3                   _at_(OEMRAMA+0x03);    //   0x03
XBYTE  u4SoundMask1                  _at_(OEMRAMA+0x04);    //   0x04
XBYTE  u4SoundMask2                  _at_(OEMRAMA+0x05);    //   0x05
XBYTE  u4BeepID                      _at_(OEMRAMA+0x06);    //   0x06
XBYTE  u4BeepRepeat                  _at_(OEMRAMA+0x07);    //   0x07
XBYTE  u4BeepMask3                   _at_(OEMRAMA+0x08);    //   0x08
XBYTE  u4KBDID                       _at_(OEMRAMA+0x09);    //   0x09
XBYTE  u4Reserve0A                   _at_(OEMRAMA+0x0A);    //   0x0A
XBYTE  KEYBOARD_TYPE                   _at_(OEMRAMA+0x0B);    //   0x0B
XBYTE  u4LEDcontrol                  _at_(OEMRAMA+0x0C);    //   0x0C
XBYTE  u4PeriCtrl3                   _at_(OEMRAMA+0x0D);    //   0x0D
XBYTE  u4PeriSTS4                    _at_(OEMRAMA+0x0E);    //   0x0E
XBYTE  u4PeriSTS5                    _at_(OEMRAMA+0x0F);    //   0x0F
XBYTE  AttentionOffset               _at_(OEMRAMA+0x10);    //   0x10
XBYTE  u4Reserve11                   _at_(OEMRAMA+0x11);    //   0x11
XBYTE  u4Reserve12                   _at_(OEMRAMA+0x12);    //   0x12
XBYTE  u4Reserve13                   _at_(OEMRAMA+0x13);    //   0x13
XBYTE  u4Reserve14                   _at_(OEMRAMA+0x14);    //   0x14
XBYTE  u4Reserve15                   _at_(OEMRAMA+0x15);    //   0x15
XBYTE  u4Reserve16                   _at_(OEMRAMA+0x16);    //   0x16
XBYTE  u4Reserve17                   _at_(OEMRAMA+0x17);    //   0x17
XBYTE  u4Reserve18                   _at_(OEMRAMA+0x18);    //   0x18
XBYTE  u4Reserve19                   _at_(OEMRAMA+0x19);    //   0x19
XBYTE  u4Reserve1A                   _at_(OEMRAMA+0x1A);    //   0x1A
XBYTE  u4Reserve1B                   _at_(OEMRAMA+0x1B);    //   0x1B
XBYTE  u4Reserve1C                   _at_(OEMRAMA+0x1C);    //   0x1C
XBYTE  u4Reserve1D                   _at_(OEMRAMA+0x1D);    //   0x1D
XBYTE  u4Reserve1E                   _at_(OEMRAMA+0x1E);    //   0x1E
XBYTE  u4Reserve1F                   _at_(OEMRAMA+0x1F);    //   0x1F
XBYTE  u4Reserve20                   _at_(OEMRAMA+0x20);    //   0x20
XBYTE  u4InhChgTimerL                _at_(OEMRAMA+0x21);    //   0x21
XBYTE  u4InhChgTimerH                _at_(OEMRAMA+0x22);    //   0x22
XBYTE  u4ExtCommand                  _at_(OEMRAMA+0x23);    //   0x23
XBYTE  uThresholdSetting             _at_(OEMRAMA+0x24);    //   0x24 // Battery charge start/ stop threshold setting for the main battery
XBYTE  u4Reserve25                   _at_(OEMRAMA+0x25);    //   0x25 // Battery charge start/ stop threshold setting for the secondary battery
XBYTE  TamperSTS                     _at_(OEMRAMA+0x26);    //   0x26
XBYTE  u4Reserve27                   _at_(OEMRAMA+0x27);    //   0x27
XBYTE  p2IBMChargeflag               _at_(OEMRAMA+0X28);    //   0X28
XBYTE  p2InhChgTimer                 _at_(OEMRAMA+0x29);    //   0x29
XBYTE  u4AttentionReq                _at_(OEMRAMA+0x2A);    //   0x2A  /
XBYTE  u4TripPointCapacity0H         _at_(OEMRAMA+0x2B);    //   0x2B
XBYTE  u4TripPointCapacity0L         _at_(OEMRAMA+0x2C);    //   0x2C
XBYTE  u4TripPointCapacity1H         _at_(OEMRAMA+0x2D);    //   0x2D
XBYTE  u4TripPointCapacity1L         _at_(OEMRAMA+0x2E);    //   0x2E
XBYTE  u4Fanspeedcontrol             _at_(OEMRAMA+0x2F);    //   0x2F
XBYTE  u4Audio                       _at_(OEMRAMA+0x30);    //   0x30
XBYTE  u4Peripheralcontrol2          _at_(OEMRAMA+0x31);    //   0x31
XBYTE  EventMask0                    _at_(OEMRAMA+0x32);    //   0x32
XBYTE  EventMask1                    _at_(OEMRAMA+0x33);    //   0x33
XBYTE  u4PeriSTS2                    _at_(OEMRAMA+0x34);    //   0x34
XBYTE  u4PeriSTS3                    _at_(OEMRAMA+0x35);    //   0x35
XBYTE  u4Reserve36                   _at_(OEMRAMA+0x36);    //   0x36
XBYTE  u4Reserve37                   _at_(OEMRAMA+0x37);    //   0x37
XBYTE  PriBattSTS                    _at_(OEMRAMA+0x38);    //   0x38
XBYTE  SecBattSTS                    _at_(OEMRAMA+0x39);    //   0x39
XBYTE  u4PeriCtrl0                   _at_(OEMRAMA+0x3A);    //   0x3A
XBYTE  u4PeriCtrl1                   _at_(OEMRAMA+0x3B);    //   0x3B
XBYTE  u4Reserve3C                   _at_(OEMRAMA+0x3C);    //   0x3C
XBYTE  u4PassCtrl                    _at_(OEMRAMA+0x3D);    //   0x3D
XBYTE  u4PassData1                   _at_(OEMRAMA+0x3E);    //   0x3E
XBYTE  u4PassData2                   _at_(OEMRAMA+0x3F);    //   0x3F
XBYTE  u4PassData3                   _at_(OEMRAMA+0x40);    //   0x40
XBYTE  u4PassData4                   _at_(OEMRAMA+0x41);    //   0x41
XBYTE  u4PassData5                   _at_(OEMRAMA+0x42);    //   0x42
XBYTE  u4PassData6                   _at_(OEMRAMA+0x43);    //   0x43
XBYTE  u4PassData7                   _at_(OEMRAMA+0x44);    //   0x44
XBYTE  u4PassData8                   _at_(OEMRAMA+0x45);    //   0x45
XBYTE  u4SenseSTS0                   _at_(OEMRAMA+0x46);    //   0x46
XBYTE  u4SenseSTS1                   _at_(OEMRAMA+0x47);    //   0x47
XBYTE  u4SenseSTS2                   _at_(OEMRAMA+0x48);    //   0x48
XBYTE  u4SenseSTS3                   _at_(OEMRAMA+0x49);    //   0x49
XBYTE  u4Reserve4A                   _at_(OEMRAMA+0x4A);    //   0x4A
XBYTE  u4Reserve4B                   _at_(OEMRAMA+0x4B);    //   0x4B
XBYTE  u4Reserve4C                   _at_(OEMRAMA+0x4C);    //   0x4C
XBYTE  u4Reserve4D                   _at_(OEMRAMA+0x4D);    //   0x4D
XBYTE  EventSTS                      _at_(OEMRAMA+0x4E);    //   0x4E
XBYTE  u4Reserve4F                   _at_(OEMRAMA+0x4F);    //   0x4F
XBYTE  uSMBusPrtcl                   _at_(OEMRAMA+0x50);    //   0x50
XBYTE  uSMBusSts                     _at_(OEMRAMA+0x51);    //   0x51
XBYTE  uSMBusAddr                    _at_(OEMRAMA+0x52);    //   0x52
XBYTE  uSMBusCmd                     _at_(OEMRAMA+0x53);    //   0x53
XBYTE  uSMBusData0                   _at_(OEMRAMA+0x54);    //   0x54
XBYTE  uSMBusData1                   _at_(OEMRAMA+0x55);    //   0x55
XBYTE  uSMBusData2                   _at_(OEMRAMA+0x56);    //   0x56
XBYTE  uSMBusData3                   _at_(OEMRAMA+0x57);    //   0x57
XBYTE  uSMBusData4                   _at_(OEMRAMA+0x58);    //   0x58
XBYTE  uSMBusData5                   _at_(OEMRAMA+0x59);    //   0x59
XBYTE  uSMBusData6                   _at_(OEMRAMA+0x5A);    //   0x5A
XBYTE  uSMBusData7                   _at_(OEMRAMA+0x5B);    //   0x5B
XBYTE  uSMBusData8                   _at_(OEMRAMA+0x5C);    //   0x5C
XBYTE  uSMBusData9                   _at_(OEMRAMA+0x5D);    //   0x5D
XBYTE  uSMBusData10                  _at_(OEMRAMA+0x5E);    //   0x5E
XBYTE  uSMBusData11                  _at_(OEMRAMA+0x5F);    //   0x5F
XBYTE  uSMBusData12                  _at_(OEMRAMA+0x60);    //   0x60
XBYTE  uSMBusData13                  _at_(OEMRAMA+0x61);    //   0x61
XBYTE  uSMBusData14                  _at_(OEMRAMA+0x62);    //   0x62
XBYTE  uSMBusData15                  _at_(OEMRAMA+0x63);    //   0x63
XBYTE  uSMBusData16                  _at_(OEMRAMA+0x64);    //   0x64
XBYTE  uSMBusData17                  _at_(OEMRAMA+0x65);    //   0x65
XBYTE  uSMBusData18                  _at_(OEMRAMA+0x66);    //   0x66
XBYTE  uSMBusData19                  _at_(OEMRAMA+0x67);    //   0x67
XBYTE  uSMBusData20                  _at_(OEMRAMA+0x68);    //   0x68
XBYTE  uSMBusData21                  _at_(OEMRAMA+0x69);    //   0x69
XBYTE  uSMBusData22                  _at_(OEMRAMA+0x6A);    //   0x6A
XBYTE  uSMBusData23                  _at_(OEMRAMA+0x6B);    //   0x6B
XBYTE  uSMBusData24                  _at_(OEMRAMA+0x6C);    //   0x6C
XBYTE  uSMBusData25                  _at_(OEMRAMA+0x6D);    //   0x6D
XBYTE  uSMBusData26                  _at_(OEMRAMA+0x6E);    //   0x6E
XBYTE  uSMBusData27                  _at_(OEMRAMA+0x6F);    //   0x6F
XBYTE  uSMBusData28                  _at_(OEMRAMA+0x70);    //   0x70
XBYTE  uSMBusData29                  _at_(OEMRAMA+0x71);    //   0x71
XBYTE  uSMBusData30                  _at_(OEMRAMA+0x72);    //   0x72
XBYTE  uSMBusData31                  _at_(OEMRAMA+0x73);    //   0x73
XBYTE  uSMBusBcnt                    _at_(OEMRAMA+0x74);    //   0x74
XBYTE  uSMBusAlarAddr                _at_(OEMRAMA+0x75);    //   0x75
XBYTE  uSMBusAlarData                _at_(OEMRAMA+0x76);    //   0x76
XBYTE  u4Reserve77                   _at_(OEMRAMA+0x77);    //   0x77
XBYTE  u4CpuThermal                  _at_(OEMRAMA+0x78);    //   0x78
XBYTE  u4Reserve79                   _at_(OEMRAMA+0x79);    //   0x79
XBYTE  u4Reserve7A                   _at_(OEMRAMA+0x7A);    //   0x7A
XBYTE  u4Reserve7B                   _at_(OEMRAMA+0x7B);    //   0x7B
XBYTE  u4Reserve7C                   _at_(OEMRAMA+0x7C);    //   0x7C
XBYTE  u4Reserve7D                   _at_(OEMRAMA+0x7D);    //   0x7D
XBYTE  u4Reserve7E                   _at_(OEMRAMA+0x7E);    //   0x7E
XBYTE  u4Reserve7F                   _at_(OEMRAMA+0x7F);    //   0x7F
XBYTE  u4AttentionCtrl               _at_(OEMRAMA+0x80);    //   0x80  
XBYTE  u4BattInfoID                  _at_(OEMRAMA+0x81);    //   0x81
XBYTE  u4Reserve82                   _at_(OEMRAMA+0x82);    //   0x82
XBYTE  u4Reserve83                   _at_(OEMRAMA+0x83);    //   0x83
XBYTE  u4FanSpdL                     _at_(OEMRAMA+0x84);    //   0x84
XBYTE  u4FanSpdH                     _at_(OEMRAMA+0x85);    //   0x85
XBYTE  u4PasswordSts1                _at_(OEMRAMA+0x86);    //   0x86
XBYTE  u4PasswordSts2                _at_(OEMRAMA+0x87);    //   0x87
XBYTE  u4Reserve88                   _at_(OEMRAMA+0x88);    //   0x88
XBYTE  u4Reserve89                   _at_(OEMRAMA+0x89);    //   0x89
XBYTE  u4Reserve8A                   _at_(OEMRAMA+0x8A);    //   0x8A
XBYTE  u4Reserve8B                   _at_(OEMRAMA+0x8B);    //   0x8B
XBYTE  u4Reserve8C                   _at_(OEMRAMA+0x8C);    //   0x8C
XBYTE  u4Reserve8D                   _at_(OEMRAMA+0x8D);    //   0x8D
XBYTE  u4Reserve8E                   _at_(OEMRAMA+0x8E);    //   0x8E
XBYTE  u4ManualFanPRM                _at_(OEMRAMA+0x8F);    //   0x8F
XBYTE  u4Reserve90                   _at_(OEMRAMA+0x90);    //   0x90
XBYTE  u4Reserve91                   _at_(OEMRAMA+0x91);    //   0x91
XBYTE  u4Reserve92                   _at_(OEMRAMA+0x92);    //   0x92
XBYTE  u4Reserve93                   _at_(OEMRAMA+0x93);    //   0x93
XBYTE  u4Reserve94                   _at_(OEMRAMA+0x94);    //   0x94
XBYTE  u4Reserve95                   _at_(OEMRAMA+0x95);    //   0x95
XBYTE  u4Reserve96                   _at_(OEMRAMA+0x96);    //   0x96
XBYTE  u4Reserve97                   _at_(OEMRAMA+0x97);    //   0x97
XBYTE  u4Reserve98                   _at_(OEMRAMA+0x98);    //   0x98
XBYTE  u4Reserve99                   _at_(OEMRAMA+0x99);    //   0x99
XBYTE  u4Reserve9A                   _at_(OEMRAMA+0x9A);    //   0x9A
XBYTE  u4Reserve9B                   _at_(OEMRAMA+0x9B);    //   0x9B
XBYTE  u4Reserve9C                   _at_(OEMRAMA+0x9C);    //   0x9C
XBYTE  u4Reserve9D                   _at_(OEMRAMA+0x9D);    //   0x9D
XBYTE  u4Reserve9E                   _at_(OEMRAMA+0x9E);    //   0x9E
XBYTE  u4Reserve9F                   _at_(OEMRAMA+0x9F);    //   0x9F
XBYTE  u4BattInfo1                   _at_(OEMRAMA+0xA0);    //   0xA0
XBYTE  u4BattInfo2                   _at_(OEMRAMA+0xA1);    //   0xA1
XBYTE  u4BattInfo3                   _at_(OEMRAMA+0xA2);    //   0xA2
XBYTE  u4BattInfo4                   _at_(OEMRAMA+0xA3);    //   0xA3
XBYTE  u4BattInfo5                   _at_(OEMRAMA+0xA4);    //   0xA4
XBYTE  u4BattInfo6                   _at_(OEMRAMA+0xA5);    //   0xA5
XBYTE  u4BattInfo7                   _at_(OEMRAMA+0xA6);    //   0xA6
XBYTE  u4BattInfo8                   _at_(OEMRAMA+0xA7);    //   0xA7
XBYTE  u4BattInfo9                   _at_(OEMRAMA+0xA8);    //   0xA8
XBYTE  u4BattInfo10                  _at_(OEMRAMA+0xA9);    //   0xA9
XBYTE  u4BattInfo11                  _at_(OEMRAMA+0xAA);    //   0xAA
XBYTE  u4BattInfo12                  _at_(OEMRAMA+0xAB);    //   0xAB
XBYTE  u4BattInfo13                  _at_(OEMRAMA+0xAC);    //   0xAC
XBYTE  u4BattInfo14                  _at_(OEMRAMA+0xAD);    //   0xAD
XBYTE  u4BattInfo15                  _at_(OEMRAMA+0xAE);    //   0xAE
XBYTE  u4BattInfo16                  _at_(OEMRAMA+0xAF);    //   0xAF
XBYTE  u4MainBattChgOnCap            _at_(OEMRAMA+0xB0);    //   0xB0
XBYTE  u4MainBattChgOffCap           _at_(OEMRAMA+0xB1);    //   0xB1
XBYTE  u4ReserveB2                   _at_(OEMRAMA+0xB2);    //   0xB2
XBYTE  u4ReserveB3                   _at_(OEMRAMA+0xB3);    //   0xB3
XBYTE  u4MainBattCtrl                _at_(OEMRAMA+0xB4);    //   0xB4
XBYTE  u4ReserveB5                   _at_(OEMRAMA+0xB5);    //   0xB5
XBYTE  u4ReserveB6                   _at_(OEMRAMA+0xB6);    //   0xB6
XBYTE  u4ReserveB7                   _at_(OEMRAMA+0xB7);    //   0xB7
XBYTE  u4ReserveB8                   _at_(OEMRAMA+0xB8);    //   0xB8
XBYTE  u4ReserveB9                   _at_(OEMRAMA+0xB9);    //   0xB9
XBYTE  u4ReserveBA                   _at_(OEMRAMA+0xBA);    //   0xBA
XBYTE  u4ReserveBB                   _at_(OEMRAMA+0xBB);    //   0xBB
XBYTE  u4ReserveBC                   _at_(OEMRAMA+0xBC);    //   0xBC
XBYTE  u4ReserveBD                   _at_(OEMRAMA+0xBD);    //   0xBD
XBYTE  u4ReserveBE                   _at_(OEMRAMA+0xBE);    //   0xBE
XBYTE  u4ReserveBF                   _at_(OEMRAMA+0xBF);    //   0xBF
XBYTE  u4ReserveC0                   _at_(OEMRAMA+0xC0);    //   0xC0
XBYTE  u4ReserveC1                   _at_(OEMRAMA+0xC1);    //   0xC1
XBYTE  u4ReserveC2                   _at_(OEMRAMA+0xC2);    //   0xC2
XBYTE  u4ReserveC3                   _at_(OEMRAMA+0xC3);    //   0xC3
XBYTE  u4ReserveC4                   _at_(OEMRAMA+0xC4);    //   0xC4
XBYTE  u4ReserveC5                   _at_(OEMRAMA+0xC5);    //   0xC5
XBYTE  u4ReserveC6                   _at_(OEMRAMA+0xC6);    //   0xC6
XBYTE  u4ReserveC7                   _at_(OEMRAMA+0xC7);    //   0xC7
XBYTE  u4ReserveC8                   _at_(OEMRAMA+0xC8);    //   0xC8
XBYTE  u4ACAdaptorCap                _at_(OEMRAMA+0xC9);    //   0xC9
XBYTE  u4ReserveCA                   _at_(OEMRAMA+0xCA);    //   0xCA
XBYTE  u4ReserveCB                   _at_(OEMRAMA+0xCB);    //   0xCB
XBYTE  u4AC_PwrH                     _at_(OEMRAMA+0xCC);    //   0xCC
XBYTE  u4AC_PwrL                     _at_(OEMRAMA+0xCD);    //   0xCD
XBYTE  u4Cfgspace4                   _at_(OEMRAMA+0xCE);    //   0xCE
XBYTE  u4ReserveCF                   _at_(OEMRAMA+0xCF);    //   0xCF
XBYTE  u4ReserveD0                   _at_(OEMRAMA+0xD0);    //   0xD0
XBYTE  u4PriBattType                 _at_(OEMRAMA+0xD1);    //   0xD1
XBYTE  u4ReserveD2                   _at_(OEMRAMA+0xD2);    //   0xD2
XBYTE  u4ReserveD3                   _at_(OEMRAMA+0xD3);    //   0xD3
XBYTE  u4ReserveD4                   _at_(OEMRAMA+0xD4);    //   0xD4
XBYTE  u4ReserveD5                   _at_(OEMRAMA+0xD5);    //   0xD5
XBYTE  u4ReserveD6                   _at_(OEMRAMA+0xD6);    //   0xD6
XBYTE  u4ReserveD7                   _at_(OEMRAMA+0xD7);    //   0xD7
XBYTE  u4ReserveD8                   _at_(OEMRAMA+0xD8);    //   0xD8
XBYTE  u4ReserveD9                   _at_(OEMRAMA+0xD9);    //   0xD9
XBYTE  u4ReserveDA                   _at_(OEMRAMA+0xDA);    //   0xDA
XBYTE  u4ReserveDB                   _at_(OEMRAMA+0xDB);    //   0xDB
XBYTE  u4ReserveDC                   _at_(OEMRAMA+0xDC);    //   0xDC
XBYTE  u4ReserveDD                   _at_(OEMRAMA+0xDD);    //   0xDD
XBYTE  u4ReserveDE                   _at_(OEMRAMA+0xDE);    //   0xDE
XBYTE  u4ReserveDF                   _at_(OEMRAMA+0xDF);    //   0xDF
XBYTE  u4ReserveE0                   _at_(OEMRAMA+0xE0);    //   0xE0
XBYTE  u4ReserveE1                   _at_(OEMRAMA+0xE1);    //   0xE1
XBYTE  u4ReserveE2                   _at_(OEMRAMA+0xE2);    //   0xE2
XBYTE  u4ReserveE3                   _at_(OEMRAMA+0xE3);    //   0xE3
XBYTE  u4ReserveE4                   _at_(OEMRAMA+0xE4);    //   0xE4
XBYTE  u4ReserveE5                   _at_(OEMRAMA+0xE5);    //   0xE5
XBYTE  u4ReserveE6                   _at_(OEMRAMA+0xE6);    //   0xE6
XBYTE  u4ReserveE7                   _at_(OEMRAMA+0xE7);    //   0xE7
XBYTE  u4VersionH                    _at_(OEMRAMA+0xE8);    //   0xE8
XBYTE  u4VersionL                    _at_(OEMRAMA+0xE9);    //   0xE9
XBYTE  u4VersionT                    _at_(OEMRAMA+0xEA);    //   0xEA
XBYTE  u4SpecversionL                _at_(OEMRAMA+0xEB);    //   0xEB
XBYTE  u4EcCapability0               _at_(OEMRAMA+0xEC);    //   0xEC
XBYTE  u4EcCapability1               _at_(OEMRAMA+0xED);    //   0xED
XBYTE  u4HighBattLV                  _at_(OEMRAMA+0xEE);    //   0xEE
XBYTE  u4SpecversionH                _at_(OEMRAMA+0xEF);    //   0xEF
XBYTE  u4BuildID1                    _at_(OEMRAMA+0xF0);    //   0xF0
XBYTE  u4BuildID2                    _at_(OEMRAMA+0xF1);    //   0xF1
XBYTE  u4BuildID3                    _at_(OEMRAMA+0xF2);    //   0xF2
XBYTE  u4BuildID4                    _at_(OEMRAMA+0xF3);    //   0xF3
XBYTE  u4BuildID5                    _at_(OEMRAMA+0xF4);    //   0xF4
XBYTE  u4BuildID6                    _at_(OEMRAMA+0xF5);    //   0xF5
XBYTE  u4BuildID7                    _at_(OEMRAMA+0xF6);    //   0xF6
XBYTE  u4BuildID8                    _at_(OEMRAMA+0xF7);    //   0xF7
XBYTE  u4BuildDate1                  _at_(OEMRAMA+0xF8);    //   0xF8
XBYTE  u4BuildDate2                  _at_(OEMRAMA+0xF9);    //   0xF9
XBYTE  u4BuildTime1                  _at_(OEMRAMA+0xFA);    //   0xFA
XBYTE  u4BuildTime2                  _at_(OEMRAMA+0xFB);    //   0xFB
XBYTE  SMB_PRTC0                   _at_(OEMRAMA+0xFC);    //   0xFC
XBYTE  u4ReserveFD                   _at_(OEMRAMA+0xFD);    //   0xFD
XBYTE  u4ReserveFE                   _at_(OEMRAMA+0xFE);    //   0xFE
XBYTE  u4ReserveFF                   _at_(OEMRAMA+0xFF);    //   0xFF
                                     
//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xB00-0xBFF   OEM RAM B
//-------------------------------------------------------------------------------
//*******************************************************************************


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xC00-0xCFF   OEM RAM C
//  0xD00-0xDFF   OEM RAM D
//-------------------------------------------------------------------------------
//*******************************************************************************
#if Thinkpad_Battery_HW_Auth
XBYTE  LV_HW_BatteryReserved1         _at_ ECRAMC+0x01;
gf2n_t gf2n_t_a                       _at_ ECRAMC+0x02; // 0x0C02 ~ 0x0C13
UWORD  LV_HW_BatteryReserved2         _at_ ECRAMC+0x14; // 0x0C14 ~ 0x0C15
UWORD  gf2n_temp[17]                  _at_ ECRAMC+0x16; // 0x0C16 ~ 0x0C37
UWORD  LV_HW_BatteryReserved3         _at_ ECRAMC+0x38; // 0x0C38 ~ 0x0C39
UWORD  LV_HW_BatteryReserved4         _at_ ECRAMC+0x4C; // 0x0C4C ~ 0x0C4D
UWORD  LV_HW_BatteryReserved5         _at_ ECRAMC+0x4E; // 0x0C4E ~ 0x0C4F
#endif


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xE00-0xEFF   OEM RAM E
//-------------------------------------------------------------------------------
//*******************************************************************************
/***********   EDGE Customer RAM for Battery  ****************/
XBYTE  TWR00                         _at_(OEMRAME+0x00);    // EDGE_BATTERY_INFO+0x00
XBYTE  TWR01                         _at_(OEMRAME+0x01);    // EDGE_BATTERY_INFO+0x01
XBYTE  MajorVer                      _at_(OEMRAME+0x02);    // EDGE_BATTERY_INFO+0x02
XBYTE  MinorVer                      _at_(OEMRAME+0x03);    // EDGE_BATTERY_INFO+0x03
XBYTE  TWRInfo                       _at_(OEMRAME+0x04);    // EDGE_BATTERY_INFO+0x04
XBYTE  AdpWatt                       _at_(OEMRAME+0x05);    // EDGE_BATTERY_INFO+0x05
XBYTE  TWR06                         _at_(OEMRAME+0x06);    // EDGE_BATTERY_INFO+0x06
XBYTE  u3Reserve07                   _at_(OEMRAME+0x07);    // EDGE_BATTERY_INFO+0x07
XBYTE  u3Reserve08                   _at_(OEMRAME+0x08);    // EDGE_BATTERY_INFO+0x08
XBYTE  u3Reserve09                   _at_(OEMRAME+0x09);    // EDGE_BATTERY_INFO+0x09
XBYTE  u3Reserve0A                   _at_(OEMRAME+0x0A);    // EDGE_BATTERY_INFO+0x0A
XBYTE  u3Reserve0B                   _at_(OEMRAME+0x0B);    // EDGE_BATTERY_INFO+0x0B
XBYTE  u3Reserve0C                   _at_(OEMRAME+0x0C);    // EDGE_BATTERY_INFO+0x0C
XBYTE  u3Reserve0D                   _at_(OEMRAME+0x0D);    // EDGE_BATTERY_INFO+0x0D
XBYTE  u3Reserve0E                   _at_(OEMRAME+0x0E);    // EDGE_BATTERY_INFO+0x0E
XBYTE  u3Reserve0F                   _at_(OEMRAME+0x0F);    // EDGE_BATTERY_INFO+0x0F
XBYTE  TWR10                         _at_(OEMRAME+0x10);    // EDGE_BATTERY_INFO+0x10
XBYTE  TWR11                         _at_(OEMRAME+0x11);    // EDGE_BATTERY_INFO+0x11
XBYTE  BattModeL                     _at_(OEMRAME+0x12);    // EDGE_BATTERY_INFO+0x12
XBYTE  BattModeH                     _at_(OEMRAME+0x13);    // EDGE_BATTERY_INFO+0x13
XBYTE  TemperatureL                  _at_(OEMRAME+0x14);    // EDGE_BATTERY_INFO+0x14
XBYTE  TemperatureH                  _at_(OEMRAME+0x15);    // EDGE_BATTERY_INFO+0x15
XBYTE  VoltageL                      _at_(OEMRAME+0x16);    // EDGE_BATTERY_INFO+0x16
XBYTE  VoltageH                      _at_(OEMRAME+0x17);    // EDGE_BATTERY_INFO+0x17
XBYTE  CurrentL                      _at_(OEMRAME+0x18);    // EDGE_BATTERY_INFO+0x18
XBYTE  CurrentH                      _at_(OEMRAME+0x19);    // EDGE_BATTERY_INFO+0x19
XBYTE  AvgCurrentL                   _at_(OEMRAME+0x1A);    // EDGE_BATTERY_INFO+0x1A
XBYTE  AvgCurrentH                   _at_(OEMRAME+0x1B);    // EDGE_BATTERY_INFO+0x1B
XBYTE  RelativeStateOfChgL           _at_(OEMRAME+0x1C);    // EDGE_BATTERY_INFO+0x1C
XBYTE  RelativeStateOfChgH           _at_(OEMRAME+0x1D);    // EDGE_BATTERY_INFO+0x1D
XBYTE  RemainCapL                    _at_(OEMRAME+0x1E);    // EDGE_BATTERY_INFO+0x1E
XBYTE  RemainCapH                    _at_(OEMRAME+0x1F);    // EDGE_BATTERY_INFO+0x1F
XBYTE  TWR20                         _at_(OEMRAME+0x20);    // EDGE_BATTERY_INFO+0x20
XBYTE  TWR21                         _at_(OEMRAME+0x21);    // EDGE_BATTERY_INFO+0x21
XBYTE  FullChgCapL                   _at_(OEMRAME+0x22);    // EDGE_BATTERY_INFO+0x22
XBYTE  FullChgCapH                   _at_(OEMRAME+0x23);    // EDGE_BATTERY_INFO+0x23
XBYTE  RunTimeToEmptyL               _at_(OEMRAME+0x24);    // EDGE_BATTERY_INFO+0x24 // Not fetch.
XBYTE  RunTimeToEmptyH               _at_(OEMRAME+0x25);    // EDGE_BATTERY_INFO+0x25 // Not fetch.
XBYTE  AvgTimeToEmptyL               _at_(OEMRAME+0x26);    // EDGE_BATTERY_INFO+0x26 // Not fetch.
XBYTE  AvgTimeToEmptyH               _at_(OEMRAME+0x27);    // EDGE_BATTERY_INFO+0x27 // Not fetch.
XBYTE  AvgTimeToFullL                _at_(OEMRAME+0x28);    // EDGE_BATTERY_INFO+0x28 // Not fetch.
XBYTE  AvgTimeToFullH                _at_(OEMRAME+0x29);    // EDGE_BATTERY_INFO+0x29 // Not fetch.
XBYTE  BattStatusL                   _at_(OEMRAME+0x2A);    // EDGE_BATTERY_INFO+0x2A
XBYTE  BattStatusH                   _at_(OEMRAME+0x2B);    // EDGE_BATTERY_INFO+0x2B
XBYTE  CycleCountL                   _at_(OEMRAME+0x2C);    // EDGE_BATTERY_INFO+0x2C
XBYTE  CycleCountH                   _at_(OEMRAME+0x2D);    // EDGE_BATTERY_INFO+0x2D
XBYTE  FETTemperatureL               _at_(OEMRAME+0x2E);    // EDGE_BATTERY_INFO+0x2E // Not fetch.
XBYTE  FETTemperatureH               _at_(OEMRAME+0x2F);    // EDGE_BATTERY_INFO+0x2F // Not fetch.
XBYTE  TWR30                         _at_(OEMRAME+0x30);    // EDGE_BATTERY_INFO+0x30
XBYTE  TWR31                         _at_(OEMRAME+0x31);    // EDGE_BATTERY_INFO+0x31
XBYTE  DesignCapL                    _at_(OEMRAME+0x32);    // EDGE_BATTERY_INFO+0x32
XBYTE  DesignCapH                    _at_(OEMRAME+0x33);    // EDGE_BATTERY_INFO+0x33
XBYTE  DesignVoltL                   _at_(OEMRAME+0x34);    // EDGE_BATTERY_INFO+0x34
XBYTE  DesignVoltH                   _at_(OEMRAME+0x35);    // EDGE_BATTERY_INFO+0x35
XBYTE  SpecInfoL                     _at_(OEMRAME+0x36);    // EDGE_BATTERY_INFO+0x36 // Not fetch.
XBYTE  SpecInfoH                     _at_(OEMRAME+0x37);    // EDGE_BATTERY_INFO+0x37 // Not fetch.
XBYTE  ManufactureDateL              _at_(OEMRAME+0x38);    // EDGE_BATTERY_INFO+0x38
XBYTE  ManufactureDateH              _at_(OEMRAME+0x39);    // EDGE_BATTERY_INFO+0x39
XBYTE  SerialNumberL                 _at_(OEMRAME+0x3A);    // EDGE_BATTERY_INFO+0x3A
XBYTE  SerialNumberH                 _at_(OEMRAME+0x3B);    // EDGE_BATTERY_INFO+0x3B
XBYTE  WarrantyPeriodL               _at_(OEMRAME+0x3C);    // EDGE_BATTERY_INFO+0x3C // Not fetch.
XBYTE  WarrantyPeriodH               _at_(OEMRAME+0x3D);    // EDGE_BATTERY_INFO+0x3D // Not fetch.
XBYTE  ACsourceInforL                _at_(OEMRAME+0x3E);    // EDGE_BATTERY_INFO+0x3E
XBYTE  ACsourceInforH                _at_(OEMRAME+0x3F);    // EDGE_BATTERY_INFO+0x3F
XBYTE  TWR40                         _at_(OEMRAME+0x40);    // EDGE_BATTERY_INFO+0x40
XBYTE  TWR41                         _at_(OEMRAME+0x41);    // EDGE_BATTERY_INFO+0x41
XBYTE  ManufacturerName[14]          _at_(OEMRAME+0x42);    // EDGE_BATTERY_INFO+0x42   //0x42~ 0x4F
XBYTE  TWR50                         _at_(OEMRAME+0x50);    // EDGE_BATTERY_INFO+0x50
XBYTE  TWR51                         _at_(OEMRAME+0x51);    // EDGE_BATTERY_INFO+0x51
XBYTE  DeviceName[14]                _at_(OEMRAME+0x52);    // EDGE_BATTERY_INFO+0x52    //0x52~0x5F
XBYTE  TWR60                         _at_(OEMRAME+0x60);    // EDGE_BATTERY_INFO+0x60
XBYTE  TWR61                         _at_(OEMRAME+0x61);    // EDGE_BATTERY_INFO+0x61
XBYTE  DeviceChemistry[14]           _at_(OEMRAME+0x62);    // EDGE_BATTERY_INFO+0x42
XBYTE  TWR70                         _at_(OEMRAME+0x70);    // EDGE_BATTERY_INFO+0x70
XBYTE  TWR71                         _at_(OEMRAME+0x71);    // EDGE_BATTERY_INFO+0x71
XBYTE  OptMfgFunction5[14]           _at_(OEMRAME+0x72);    // EDGE_BATTERY_INFO+0x72
XBYTE  TWR80                         _at_(OEMRAME+0x80);    // EDGE_BATTERY_INFO+0x80
XBYTE  TWR81                         _at_(OEMRAME+0x81);    // EDGE_BATTERY_INFO+0x81
XBYTE  OptMfgFunction1L              _at_(OEMRAME+0x82);    // EDGE_BATTERY_INFO+0x82 // Not fetch.
XBYTE  OptMfgFunction1H              _at_(OEMRAME+0x83);    // EDGE_BATTERY_INFO+0x83 // Not fetch.
XBYTE  OptMfgFunction2L              _at_(OEMRAME+0x84);    // EDGE_BATTERY_INFO+0x84 // Not fetch.
// OptMfgFunction2L
// BIT0: CAPACITY_CORRECTION  1: enable 0: disable
// BIT1-2: Preservation (Open for system use)
// BIT3: EXPRESS_CHARGE
// BIT4: CLEAN_SHEET2       1: enable 0: disable
// BIT5: Dual Mode Capability
//
// BIT6: Dual Mode Select(0: Long runtime mode(Default), 1:Long Lifespan mode, 
// This bit must be kept until next write operation and must not be reset at OFF 
// State)
//
// Bit7: Non-replaceable battery
// 0 : Replaceable battery
// 1 : Non-replaceable battery
XBYTE  OptMfgFunction2H              _at_(OEMRAME+0x85);    // EDGE_BATTERY_INFO+0x85 // Not fetch.
XBYTE  FirmwareUpdateStatusL         _at_(OEMRAME+0x86);    // EDGE_BATTERY_INFO+0x86
XBYTE  FirmwareUpdateStatusH         _at_(OEMRAME+0x87);    // EDGE_BATTERY_INFO+0x87
XBYTE  BatterymakerIDL               _at_(OEMRAME+0x88);    // EDGE_BATTERY_INFO+0x88
XBYTE  BatterymakerIDH               _at_(OEMRAME+0x89);    // EDGE_BATTERY_INFO+0x89
XBYTE  HardwareIDL                   _at_(OEMRAME+0x8A);    // EDGE_BATTERY_INFO+0x8A
XBYTE  HardwareIDH                   _at_(OEMRAME+0x8B);    // EDGE_BATTERY_INFO+0x8B
XBYTE  FirmwareversionL              _at_(OEMRAME+0x8C);    // EDGE_BATTERY_INFO+0x8C
XBYTE  FirmwareversionH              _at_(OEMRAME+0x8D);    // EDGE_BATTERY_INFO+0x8D
XBYTE  DataVersionL                  _at_(OEMRAME+0x8E);    // EDGE_BATTERY_INFO+0x8E
XBYTE  DataVersionH                  _at_(OEMRAME+0x8F);    // EDGE_BATTERY_INFO+0x8F
XBYTE  TWR90                         _at_(OEMRAME+0x90);    // EDGE_BATTERY_INFO+0x90
XBYTE  TWR91                         _at_(OEMRAME+0x91);    // EDGE_BATTERY_INFO+0x91
XBYTE  ManufactureAccessL            _at_(OEMRAME+0x92);    // EDGE_BATTERY_INFO+0x92
XBYTE  ManufactureAccessH            _at_(OEMRAME+0x93);    // EDGE_BATTERY_INFO+0x93
XBYTE  MaxErroL                      _at_(OEMRAME+0x94);    // EDGE_BATTERY_INFO+0x94 // Not fetch.
XBYTE  MaxErroH                      _at_(OEMRAME+0x95);    // EDGE_BATTERY_INFO+0x95 // Not fetch.
XBYTE  ChargeCurrL                   _at_(OEMRAME+0x96);    // EDGE_BATTERY_INFO+0x96
XBYTE  ChargeCurrH                   _at_(OEMRAME+0x97);    // EDGE_BATTERY_INFO+0x97
XBYTE  ChargeVoltL                   _at_(OEMRAME+0x98);    // EDGE_BATTERY_INFO+0x98
XBYTE  ChargeVoltH                   _at_(OEMRAME+0x99);    // EDGE_BATTERY_INFO+0x99
XBYTE  u3Reserve9A                   _at_(OEMRAME+0x9A);    // EDGE_BATTERY_INFO+0x9A
XBYTE  u3Reserve9B                   _at_(OEMRAME+0x9B);    // EDGE_BATTERY_INFO+0x9B
XBYTE  u3Reserve9C                   _at_(OEMRAME+0x9C);    // EDGE_BATTERY_INFO+0x9C
XBYTE  u3Reserve9D                   _at_(OEMRAME+0x9D);    // EDGE_BATTERY_INFO+0x9D
XBYTE  u3Reserve9E                   _at_(OEMRAME+0x9E);    // EDGE_BATTERY_INFO+0x9E
XBYTE  u3Reserve9F                   _at_(OEMRAME+0x9F);    // EDGE_BATTERY_INFO+0x9F
XBYTE  TWRA0                         _at_(OEMRAME+0xA0);    // EDGE_BATTERY_INFO+0xA0
XBYTE  TWRA1                         _at_(OEMRAME+0xA1);    // EDGE_BATTERY_INFO+0xA1
XBYTE  ManufactureData[12]           _at_(OEMRAME+0xA2);    // EDGE_BATTERY_INFO+0xA2
XBYTE  Major                         _at_(OEMRAME+0xAE);    // EDGE_BATTERY_INFO+0xAE
XBYTE  Minor                         _at_(OEMRAME+0xAF);    // EDGE_BATTERY_INFO+0xAF
XBYTE  TWRB0                         _at_(OEMRAME+0xB0);    // EDGE_BATTERY_INFO+0xB0
XBYTE  TWRB1                         _at_(OEMRAME+0xB1);    // EDGE_BATTERY_INFO+0xB1
XBYTE  BattUsageCondition[10]        _at_(OEMRAME+0xB2);    // EDGE_BATTERY_INFO+0xB2
XBYTE  u3ReserveBC                   _at_(OEMRAME+0xBC);    // EDGE_BATTERY_INFO+0xBC
XBYTE  u3ReserveBD                   _at_(OEMRAME+0xBD);    // EDGE_BATTERY_INFO+0xBD
XBYTE  u3ReserveBE                   _at_(OEMRAME+0xBE);    // EDGE_BATTERY_INFO+0xBE
XBYTE  u3ReserveBF                   _at_(OEMRAME+0xBF);    // EDGE_BATTERY_INFO+0xBF
XBYTE  TWRC0                         _at_(OEMRAME+0xC0);    // EDGE_BATTERY_INFO+0xC0
XBYTE  TWRC1                         _at_(OEMRAME+0xC1);    // EDGE_BATTERY_INFO+0xC1
XBYTE  BattAuthStat                  _at_(OEMRAME+0xC2);    // EDGE_BATTERY_INFO+0xC2
XBYTE  u3ReserveC3                   _at_(OEMRAME+0xC3);    // EDGE_BATTERY_INFO+0xC3
XBYTE  u3ReserveC4                   _at_(OEMRAME+0xC4);    // EDGE_BATTERY_INFO+0xC4
XBYTE  u3ReserveC5                   _at_(OEMRAME+0xC5);    // EDGE_BATTERY_INFO+0xC5
XBYTE  u3ReserveC6                   _at_(OEMRAME+0xC6);    // EDGE_BATTERY_INFO+0xC6
XBYTE  u3ReserveC7                   _at_(OEMRAME+0xC7);    // EDGE_BATTERY_INFO+0xC7
XBYTE  u3ReserveC8                   _at_(OEMRAME+0xC8);    // EDGE_BATTERY_INFO+0xC8
XBYTE  u3ReserveC9                   _at_(OEMRAME+0xC9);    // EDGE_BATTERY_INFO+0xC9
XBYTE  u3ReserveCA                   _at_(OEMRAME+0xCA);    // EDGE_BATTERY_INFO+0xCA
XBYTE  u3ReserveCB                   _at_(OEMRAME+0xCB);    // EDGE_BATTERY_INFO+0xCB
XBYTE  u3ReserveCC                   _at_(OEMRAME+0xCC);    // EDGE_BATTERY_INFO+0xCC
XBYTE  u3ReserveCD                   _at_(OEMRAME+0xCD);    // EDGE_BATTERY_INFO+0xCD
XBYTE  u3ReserveCE                   _at_(OEMRAME+0xCE);    // EDGE_BATTERY_INFO+0xCE
XBYTE  u3ReserveCF                   _at_(OEMRAME+0xCF);    // EDGE_BATTERY_INFO+0xCF
XBYTE  u3ReserveD0                   _at_(OEMRAME+0xD0);    // EDGE_BATTERY_INFO+0xD0
XBYTE  u3ReserveD1                   _at_(OEMRAME+0xD1);    // EDGE_BATTERY_INFO+0xD1
XBYTE  u3ReserveD2                   _at_(OEMRAME+0xD2);    // EDGE_BATTERY_INFO+0xD2
XBYTE  u3ReserveD3                   _at_(OEMRAME+0xD3);    // EDGE_BATTERY_INFO+0xD3
XBYTE  u3ReserveD4                   _at_(OEMRAME+0xD4);    // EDGE_BATTERY_INFO+0xD4
XBYTE  u3ReserveD5                   _at_(OEMRAME+0xD5);    // EDGE_BATTERY_INFO+0xD5
XBYTE  u3ReserveD6                   _at_(OEMRAME+0xD6);    // EDGE_BATTERY_INFO+0xD6
XBYTE  u3ReserveD7                   _at_(OEMRAME+0xD7);    // EDGE_BATTERY_INFO+0xD7
XBYTE  u3ReserveD8                   _at_(OEMRAME+0xD8);    // EDGE_BATTERY_INFO+0xD8
XBYTE  u3ReserveD9                   _at_(OEMRAME+0xD9);    // EDGE_BATTERY_INFO+0xD9
XBYTE  u3ReserveDA                   _at_(OEMRAME+0xDA);    // EDGE_BATTERY_INFO+0xDA
XBYTE  u3ReserveDB                   _at_(OEMRAME+0xDB);    // EDGE_BATTERY_INFO+0xDB
XBYTE  u3ReserveDC                   _at_(OEMRAME+0xDC);    // EDGE_BATTERY_INFO+0xDC
XBYTE  u3ReserveDD                   _at_(OEMRAME+0xDD);    // EDGE_BATTERY_INFO+0xDD
XBYTE  u3ReserveDE                   _at_(OEMRAME+0xDE);    // EDGE_BATTERY_INFO+0xDE
XBYTE  u3ReserveDF                   _at_(OEMRAME+0xDF);    // EDGE_BATTERY_INFO+0xDF
XBYTE  u3ReserveE0                   _at_(OEMRAME+0xE0);    // EDGE_BATTERY_INFO+0xE0
XBYTE  u3ReserveE1                   _at_(OEMRAME+0xE1);    // EDGE_BATTERY_INFO+0xE1
XBYTE  u3ReserveE2                   _at_(OEMRAME+0xE2);    // EDGE_BATTERY_INFO+0xE2
XBYTE  u3ReserveE3                   _at_(OEMRAME+0xE3);    // EDGE_BATTERY_INFO+0xE3
XBYTE  u3ReserveE4                   _at_(OEMRAME+0xE4);    // EDGE_BATTERY_INFO+0xE4
XBYTE  u3ReserveE5                   _at_(OEMRAME+0xE5);    // EDGE_BATTERY_INFO+0xE5
XBYTE  u3ReserveE6                   _at_(OEMRAME+0xE6);    // EDGE_BATTERY_INFO+0xE6
XBYTE  u3ReserveE7                   _at_(OEMRAME+0xE7);    // EDGE_BATTERY_INFO+0xE7
XBYTE  u3ReserveE8                   _at_(OEMRAME+0xE8);    // EDGE_BATTERY_INFO+0xE8
XBYTE  u3ReserveE9                   _at_(OEMRAME+0xE9);    // EDGE_BATTERY_INFO+0xE9
XBYTE  u3ReserveEA                   _at_(OEMRAME+0xEA);    // EDGE_BATTERY_INFO+0xEA
XBYTE  u3ReserveEB                   _at_(OEMRAME+0xEB);    // EDGE_BATTERY_INFO+0xEB
XBYTE  u3ReserveEC                   _at_(OEMRAME+0xEC);    // EDGE_BATTERY_INFO+0xEC
XBYTE  u3ReserveED                   _at_(OEMRAME+0xED);    // EDGE_BATTERY_INFO+0xED
XBYTE  u3ReserveEE                   _at_(OEMRAME+0xEE);    // EDGE_BATTERY_INFO+0xEE
XBYTE  u3ReserveEF                   _at_(OEMRAME+0xEF);    // EDGE_BATTERY_INFO+0xEF
XBYTE  u3ReserveF0                   _at_(OEMRAME+0xF0);    // EDGE_BATTERY_INFO+0xF0
XBYTE  u3ReserveF1                   _at_(OEMRAME+0xF1);    // EDGE_BATTERY_INFO+0xF1
XBYTE  u3ReserveF2                   _at_(OEMRAME+0xF2);    // EDGE_BATTERY_INFO+0xF2
XBYTE  u3ReserveF3                   _at_(OEMRAME+0xF3);    // EDGE_BATTERY_INFO+0xF3
XBYTE  u3ReserveF4                   _at_(OEMRAME+0xF4);    // EDGE_BATTERY_INFO+0xF4
XBYTE  u3ReserveF5                   _at_(OEMRAME+0xF5);    // EDGE_BATTERY_INFO+0xF5
XBYTE  u3ReserveF6                   _at_(OEMRAME+0xF6);    // EDGE_BATTERY_INFO+0xF6
XBYTE  u3ReserveF7                   _at_(OEMRAME+0xF7);    // EDGE_BATTERY_INFO+0xF7
XBYTE  u3ReserveF8                   _at_(OEMRAME+0xF8);    // EDGE_BATTERY_INFO+0xF8
XBYTE  u3ReserveF9                   _at_(OEMRAME+0xF9);    // EDGE_BATTERY_INFO+0xF9
XBYTE  u3ReserveFA                   _at_(OEMRAME+0xFA);    // EDGE_BATTERY_INFO+0xFA
XBYTE  u3ReserveFB                   _at_(OEMRAME+0xFB);    // EDGE_BATTERY_INFO+0xFB
XBYTE  u3ReserveFC                   _at_(OEMRAME+0xFC);    // EDGE_BATTERY_INFO+0xFC
XBYTE  u3ReserveFD                   _at_(OEMRAME+0xFD);    // EDGE_BATTERY_INFO+0xFD
XBYTE  u3ReserveFE                   _at_(OEMRAME+0xFE);    // EDGE_BATTERY_INFO+0xFE
XBYTE  u3ReserveFF                   _at_(OEMRAME+0xFF);    // EDGE_BATTERY_INFO+0xFF

/***********   EDGE Customer RAM for Interface space   ****************/

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xF00-0xFFF   OEM RAM F
//-------------------------------------------------------------------------------
//*******************************************************************************
XBYTE    KB_LOC_Table[128]            _at_ OEMRAMF+0x80;  // 0x80   //ivan

