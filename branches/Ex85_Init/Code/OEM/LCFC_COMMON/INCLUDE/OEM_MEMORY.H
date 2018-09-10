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
 * 0x100-0x1FF   OEM RAM for LCFC common code usage
 * 0x200-0x2FF   OEM RAM for LCFC common code usage
 * 0x300-0x3FF   OEM RAM for LCFC common code usage
 * 0x400-0x4FF   OEM RAM Reserve for project usage
 * 0x500-0x5FF   OEM RAM for LCFC common code usage
 * 0x600-0x6FF   OEM RAM Reserve for ITE tool

 * 0x800-0x8FF   OEM RAM keyboard matrix
 * 0x900-0x9FF   OEM RAM for LCFC common code usage
 * 0xA00-0xAFF   OEM RAM EC name space
 * 0xB00-0xBFF   OEM RAM for EC ram debug
 * 0xC00-0xCFF   OEM RAM X
 * 0xD00-0xDFF   OEM RAM X
 * 0xE00-0xEFF   OEM RAM For Battery1 TWR
 * 0xF00-0xFFF   OEM RAM Reserve for Battery2 TWR
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/

#ifndef OEM_MEMORY_H
#define OEM_MEMORY_H

#define EC_RAMBase  0x0000

#define OEMRAM1  (EC_RAMBase+0x0100)  // EC_RAMBase+0x0100
#define OEMRAM2  (EC_RAMBase+0x0200)  // EC_RAMBase+0x0200
#define OEMRAM3  (EC_RAMBase+0x0300)  // EC_RAMBase+0x0300
#define OEMRAM4  (EC_RAMBase+0x0400)  // EC_RAMBase+0x0400
#define OEMRAM5  (EC_RAMBase+0x0500)  // EC_RAMBase+0x0500
#define OEMRAM6  (EC_RAMBase+0x0600)  // EC_RAMBase+0x0600

#define OEMRAM8  (EC_RAMBase+0x0800)  // EC_RAMBase+0x0800
#define OEMRAM9  (EC_RAMBase+0x0900)  // EC_RAMBase+0x0900
#define OEMRAMA  (EC_RAMBase+0x0A00)  // EC_RAMBase+0x0A00
#define OEMRAMB  (EC_RAMBase+0x0B00)  // EC_RAMBase+0x0B00
#define OEMRAMC  (EC_RAMBase+0x0C00)  // EC_RAMBase+0x0C00
#define OEMRAMD  (EC_RAMBase+0x0D00)  // EC_RAMBase+0x0D00
#define OEMRAME  (EC_RAMBase+0x0E00)  // EC_RAMBase+0x0E00
#define OEMRAMF  (EC_RAMBase+0x0F00)  // EC_RAMBase+0x0F00

#define EVENT_BUFFER_SIZE 8


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x100-0x1FF   OEM RAM 1
//-------------------------------------------------------------------------------
//*******************************************************************************
extern XBYTE  SysPowState;            //_at_(OEMRAM1+0x00);                      
extern XBYTE  PWSeqStep;              //_at_(OEMRAM1+0x01);                     
extern XBYTE  DeepSleepCunt;          //_at_(OEMRAM1+0x02);                    
extern XWORD  DelayDoPowerSeq;        //_at_(OEMRAM1+0x03);                    
extern XWORD  PowSeqDelay;            //_at_(OEMRAM1+0x05);                    
extern XBYTE  KB_S3Dly;               //_at_(oECRAM1+0x07);                    
extern XBYTE  CombineKeyStatus;       //_at_(OEMRAM1+0x08);               
 #define  HotkeyStoreROM   BIT0
 #define  b2FnRNL          BIT2 
  #define b6Fn4Key4				BIT6 
 #define b7Fn4				BIT7 

extern XBYTE  ComboKeyState;          //_at_(OEMRAM1+0x09);               
 #define ComboKeyValid     BIT5
 #define CallingFunc       BIT6
 #define ECDebugMode       BIT7

extern XBYTE  LastComboKey;           //_at_(OEMRAM1+0x0A);               


extern XBYTE  KB_OBF_count;           //_at_(OEMRAM1+0x0B);                     


extern XBYTE  CmdData2;               //_at_(OECRAM1+0x0C);                     

extern XBYTE  uVGATurboFun;           //_at_(OEMRAM1+0x0D);                     
 #define  uEnableGPS           BIT0
 #define  uDisPowerSteeringOK  BIT1
 #define  uDisVGATurboOK       BIT2
 #define  uSTPturboOK          BIT3

extern XBYTE  eFlashVarifyOK;         //_at_(OEMRAM1+0x0E);                     
extern XBYTE  eFlash_r_data;          //_at_(OEMRAM1+0x0F);                     
extern XBYTE  eFlashA1;               //_at_(OEMRAM1+0x10);                     
extern XBYTE  eFlashA0;               //_at_(OEMRAM1+0x11);                     
extern XBYTE  eEEPROMBank;            //_at_(OEMRAM1+0x12);                     
extern XBYTE  eEEPROMAddrsss;         //_at_(OEMRAM1+0x13);                     
extern XBYTE  eEEPROMData;            //_at_(OEMRAM1+0x14);                     
extern XBYTE  eEEPROMMarkData;        //_at_(OEMRAM1+0x15);                     
extern XBYTE  e256ByteCnt;            //_at_(OEMRAM1+0x16);                     
extern XBYTE  eUpdateEEPROMCnt;       //_at_(OEMRAM1+0x17);                
extern XBYTE  eUpdatepDevStusCnt;     //_at_(OEMRAM1+0x18);               
extern XBYTE  WSMbusTemp01;           //_at_(OEMRAM1+0x19);               
extern XBYTE  WSMbusTemp02;           //_at_(OEMRAM1+0x1A);               
extern XBYTE  SMbusFailCnt2;          //_at_(OEMRAM1+0x1B);               
extern XBYTE  SMbusFailCnt3;          //_at_(OEMRAM1+0x1C);               
extern XBYTE  GPIO_STATUS;            // _at_(OECRAM1+0x1D);              
 #define  GPIO_RSMRST        BIT0      // PB.7 RSMRST.
 #define  GPIO_PBTN_OUT      BIT1      // PF.1 PBTN_OUT.
 #define  GPIO_VDDQ_PGOOD    BIT2      // PI.1 VDDQ_PGOOD
 #define  GPIO_PM_SLP_S3     BIT3      // PH.1 PM_SLP_S3
 #define  GPIO_PM_SLP_S4     BIT4      // PH.2 PM_SLP_S4
 #define  GPIO_PM_SLP_S5     BIT5      // Px.x PM_SLP_S5
 #define  GPIO_VR_ON         BIT6      // PE.3 VR_ON
 #define  GPIO_PCH_PWROK     BIT7      // PH.6 PCH_PWROK

extern XBYTE  BatSMbusFailCount;      //_at_(OECRAM1+0x1E);               
extern XBYTE  Batpollstep1;           //_at_(OECRAM1+0x1F);               

extern XBYTE  CHGIC_SMbusFailCnt;     //_at_(OEMRAM1+0x20);               
extern XBYTE  CHGIC_ptr;              //_at_(OEMRAM1+0x21);               
extern XBYTE  CHGIC_InputCurrentL;    //_at_(OEMRAM1+0x22);           
extern XBYTE  CHGIC_InputCurrentH;    //_at_(OEMRAM1+0x23);           
extern XBYTE  CHGIC_ReadCmd0x14L;     //_at_(OEMRAM1+0x24);          
extern XBYTE  CHGIC_ReadCmd0x14H;     //_at_(OEMRAM1+0x25);          
extern XBYTE  CHGIC_ReadCmd0x15L;     //_at_(OEMRAM1+0x26);          
extern XBYTE  CHGIC_ReadCmd0x15H;     //_at_(OEMRAM1+0x27);          
extern XBYTE  CHGIC_ReadCmd0x3FL;     //_at_(OEMRAM1+0x28);          
extern XBYTE  CHGIC_ReadCmd0x3FH;     //_at_(OEMRAM1+0x29);          
extern XWORD  BATTEMP;                //_at_(OEMRAM1+0x2A);                 

extern XBYTE  BatLowCnt;              //_at_(OEMRAM1+0x2C);               

typedef enum
{
  Chk_Trickle_Current_status_normal,
  Chk_Trickle_Current_status_error
}Chk_Trickle_Current_status_type;
extern XBYTE  Chk_Trickle_Current_status;    // _at_(ECRAM1+0x2D);        
extern XBYTE  Chk_Trickle_Current_count;    // _at_(ECRAM1+0x2E);         
extern XWORD  Chk_Trickle_Current_Now_Current;// _at_(ECRAM1+0x2F); //0x47~0x48       
extern XWORD  Chk_Trickle_Current_Chg_Current;// _at_(ECRAM1+0x31); //0x49~0x4A       
extern XBYTE    Get_Batt_debounce_count;    // _at_(ECRAM1+0x33);                     
extern XBYTE    Get_Batt_debounce_hash1;    // _at_(ECRAM1+0x34);                     
extern XBYTE    Get_Batt_debounce_hash2;    // _at_(ECRAM1+0x35);                     




extern XBYTE  Bat0x0BFakeCnt;          //_at_(OEMRAM1+0x36);                       

extern XBYTE  BatteryAlarm;          //_at_(OEMRAM1+0x37);                            
 #define  BATOCP          BIT0
 #define  BATOTP          BIT1
 #define  ACSTPP          BIT2
 #define  BATPercl_5      BIT3
 #define  HybridSTPP      BIT4
 #define  DISTURBO        BIT5
 #define  ADPOUT          BIT6

extern XBYTE  CHGIC_ReadCmd0x12L;        //_at_(ECRAM1+0x38);                      
 #define ChargeInhibit    BIT0
 #define IDPM_EN          BIT1
// #define TurboBoost       BIT2      //
 #define EN_LDO           BIT2      //
 #define IADP_RATIO       BIT4      //
 #define BATLearn         BIT5      //

extern XBYTE  CHGIC_ReadCmd0x12H;        //_at_(ECRAM1+0x39);
 #define SwitchingFreq    BIT1      //0:800KHz,1:1200MHz
 #define AUDIO_Freq       BIT2
 #define OTG_ON_ChrgOK    BIT3
 #define IDPM_AUTO_DIS    BIT4
  #define LOWPWR_EN       BIT7

extern XBYTE  BattTemp;                  //_at_(OEMRAM1+0x3A);                           



extern XBYTE  inhibit2sec;              //_at_(OEMRAM1+0x3B);                           
extern XBYTE  Battdata_ready;           //_at_(OEMRAM1+0x3C);                          
extern WORD  TrickleChgTimeOutCnt;      //_at_(OEMRAM1+0x3D);                      
extern WORD  FastChgTimeOutCnt;         //_at_(OEMRAM1+0x3F);                       
extern XBYTE  RSMshutdownCnt;           //_at_(OEMRAM1+0x41);                         
typedef enum
{
  ChkBattery_abnormal_status_normal,
  ChkBattery_abnormal_status_wait5sec,
  ChkBattery_abnormal_status_error
}ChkBattery_abnormal_status_type;
extern XBYTE  ChkBattery_abnormal_status;    //_at_(ECRAM1+0x42);                    
extern XBYTE  ChkBattery_abnormal_count;    //_at_(ECRAM1+0x43);                     
extern XBYTE  LOWBATT_3TIMES;         //_at_(ECRAM1+0x44);                           

typedef enum
{
  Chk_Wrong_ADP_Status_Null,
  Chk_Wrong_ADP_Status_wait_2sec,
  Chk_Wrong_ADP_Status_wait_10sec,
  Chk_Wrong_ADP_Status_ACOFF_Lock
}Chk_Wrong_ADP_Status_Type;
extern XBYTE  Chk_Wrong_ADP_Status;      //_at_(OEMRAM1+0x45);                 
extern XBYTE  AdapterID;                 //_at_(OEMRAM1+0x46);                      
extern XWORD  API_ID;                    //_at_(OEMRAM1+0x47);                     
extern XWORD  Chk_Wrong_10ms_Count;      //_at_(OEMRAM1+0x49);                 
typedef enum
{
  Chk_ACOP_Status_Null,
  Chk_ACOP_Status_wait_10sec,
  Chk_ACOP_Status_ACOFF_Lock
}Chk_ACOP_Status_Type;
extern XBYTE  Chk_ACOP_Status;             //_at_(OEMRAM1+0x4B);                          
extern XWORD  Chk_ACOP_Bat_Chg_Current;    //_at_(OEMRAM1+0x4C);  // using integer   
extern XWORD  Chk_ACOP_10ms_Count;         //_at_(OEMRAM1+0x4E);                        
extern XBYTE  ACIN_FallINT_Count;          //_at_(OEMRAM1+0x50);                       
extern XBYTE  ACIN_BLINK3;                 //_at_(ECRAM1+0x51);                            
typedef enum
{
  Chk_Hybrid_STPP_Status_SystemOverCP,
  Chk_Hybrid_STPP_Status_SystemOverChargerTurboPoint_NonStartTurbo,
  Chk_Hybrid_STPP_Status_SystemOverChargerTurboPoint_TurboStarted,
  Chk_Hybrid_STPP_Status_Disable_CPUTurbo,
  Chk_Hybrid_STPP_Status_CPU_throttling,
  Chk_Hybrid_STPP_Status_Recover_CPU_throttling,
  Chk_Hybrid_STPP_Status_Recover_Charger_Turbo
}Chk_Hybrid_STPP_Status_Type;

extern XBYTE  MaxPwrCtrlFsm;                           //_at_(ECRAM1+0x52);                                     

extern XBYTE  Chk_Hybrid_STPP_min_BattGasgauge;        //_at_(ECRAM1+0x53);                
extern XWORD  Chk_Hybrid_STPP_Batt_Output_Power;       //_at_(ECRAM1+0x54);              
extern XWORD  Chk_Hybrid_STPP_Batt_Output_Power_Limit;    //_at_(ECRAM1+0x56);        
extern XBYTE  BackLight_En_Delay;                      //_at_(ECRAM1+0x58);                        

extern XBYTE  Batteryin_delay;                         //_at_(ECRAM1+0x59);                           

extern XBYTE  uProject0;                               //_at_(OEMRAM1+0x5A);  // byte
 #define  ECPORT80          BIT0
 #define  NeedRecoveryNotSupportADP          BIT1
 #define  NeedRecoveryLimitedSupportADP      BIT2
 #define  uSysTimeOK        BIT4
 #define  uFUDWriteEN       BIT5
 #define  uFUdayFail        BIT6
 #define  uFUdayClr         BIT7
 #define  GET_ECPORT80      GET_MASK(uProject0,ECPORT80) 
 #define  EN_ECPORT80       SET_MASK(uProject0,ECPORT80)
 #define  DIS_ECPORT80      CLEAR_MASK(uProject0,ECPORT80) 


extern XBYTE  CRCDelay;                                 //_at_(OEMRAM1+0x5B)
extern XBYTE  FAN_PCH_Max;                             //_at_(OEMRAM1+0x5C);   // byte
extern XBYTE  RPM_PCH_Max;                             //_at_(OEMRAM1+0x5D);
extern XBYTE  CHGIC_Status;                             //_at_(OEMRAM1+0x5E)
 #define  ChgIcInitial          BIT0
extern XBYTE  pPWRLEDS3_off_Count;                     //_at_(OEMRAM1+0x5F);
extern XWORD  ADPI_Data[4];                            //_at_(ECRAM1+0x60);//0x60~0x67                 

extern XWORD  XWTemp1;                                 //_at_(OEMRAM1+0x68);                              

extern XBYTE  MpcSystemTotalPower;                     //_at_(OEMRAM1+0x6A);                      
extern XBYTE  ADPI2Sec;                                //_at_(OEMRAM1+0x6B);                   
extern XBYTE  MpcCP;                                   //_at_(OEMRAM1+0x6C);                   

extern XBYTE  MpcCpuTurboDisablePoint;                 //_at_(OEMRAM1+0x6D);                   
extern XBYTE  MpcCpuTurboRecoverPoint;                 //_at_(OEMRAM1+0x6E);                   
extern XBYTE  MpcCpuThrottlingPoint;                   //_at_(OEMRAM1+0x6F);                   
extern XBYTE  MpcCpuThrottlingRecoverPoint;            //_at_(OEMRAM1+0x70);                   
extern XBYTE  MpcGpuThrottlingPoint;                   //_at_(OEMRAM1+0x71);                   
extern XBYTE  MpcGpuThrottlingRecoverPoint;            //_at_(OEMRAM1+0x72);                   
extern XBYTE  MpcAcCpuTurboDisablePoint;               //_at_(OEMRAM1+0x73);                 
extern XBYTE  MpcAcCpuTurboRecoverPoint;               //_at_(OEMRAM1+0x74);                 
extern XBYTE  MpcAcCpuThrottlingPoint;                 //_at_(OEMRAM1+0x75);                 
extern XBYTE  MpcAcCpuThrottlingRecoverPoint;          //_at_(OEMRAM1+0x76);                 
extern XBYTE  MpcAcGpuThrottlingPoint;                 //_at_(OEMRAM1+0x77);                 
extern XBYTE  MpcAcGpuThrottlingRecoverPoint;          //_at_(OEMRAM1+0x78);                 

extern XWORD  ADPI_AvgData;                            //_at_(OEMRAM1+0x79);                            

extern XBYTE  pProject4;             //_at_ OEMRAM1+0xB2;
 #define  pPWSWdisable      BIT0	 //Disable Power switch function(PBTN_OUT and 7s power switch WDT).  
 #define  pLastLPCstatus    BIT1  //record LPC last status
 #define  pComboKeyWin      BIT5  //Work-around for combo "key Win + Left-Shift + Right-Arrow"(Ghost key).
 #define  pComboKeyLShift   BIT6  //Work-around for combo "key Win + Left-Shift + Right-Arrow"(Ghost key).
 #define  pComboKeyRArrow   BIT7  //Work-around for combo "key Win + Left-Shift + Right-Arrow"(Ghost key).
 #define pLastLPCstatusGET    GET_MASK(pProject4,pLastLPCstatus)
 #define pLastLPCstatusSET    SET_MASK(pProject4,pLastLPCstatus)
 #define pLastLPCstatusCLR    CLEAR_MASK(pProject4,pLastLPCstatus)
 
 
extern XWORD  PSW_COUNTER;                            //_at_ OEMRAM1+0x7C;  //(word)                  
extern XBYTE  DSxPowState;                            //_at_ OEMRAM1+0x7E;   //(byte)                    
extern XBYTE  AOU_status;                             //_at_ OEMRAM1+0x7F;
 #define AOUPort_Dev    BIT0    // 1 = Exist USB Device in AOU port.
 #define AOUPort_DevSET    SET_MASK(AOU_status,AOUPort_Dev)
 #define AOUPort_DevCLR    CLEAR_MASK(AOU_status,AOUPort_Dev)
 #define AOUPort_DevGET    GET_MASK(AOU_status,AOUPort_Dev)
#if (PLATFORM_CHIP == INTEL)
extern XWORD  SUSACK_LOW_CNT;                       //_at_ OEMRAM1+0x80;                               
extern XBYTE  SUSACK_HI_CNT;                        //_at_ OEMRAM1+0x82;                             
#endif // PLATFORM_CHIP
extern XBYTE  SLP_A_CNT;                            //_at_ OEMRAM1+0x83;                                  
extern XBYTE  S3S4DelayCnt;                         //_at_ OEMRAM1+0x84;                              
extern XBYTE  cADPIDTemp;                           //_at_ OEMRAM1+0x85;  // Bit7~4: ID , BIT3~0:Count   
extern XBYTE  TPID1;                     //_at_ OEMRAM1+0x86;
extern XBYTE  TPID2;                     //_at_ OEMRAM1+0x87;
extern XBYTE  TPID3;                     //_at_ OEMRAM1+0x88;
extern XBYTE  TPID4;                     //_at_ OEMRAM1+0x89;
extern XBYTE  p2BeepCnt1;          //  _at_ OEMRAM1+0x8A;
extern XBYTE  p2BeepCnt2;          //  _at_ OEMRAM1+0x8B;
extern XBYTE  p2BeepCnt3;          //  _at_ OEMRAM1+0x8C;
extern XBYTE  p2BeepCtrl;          //  _at_ OEMRAM1+0x8D;
 #define  p2RptCnt          BIT0
extern XBYTE  FnStatus;                 //_at_ OEMRAM1+0x8E;
extern XBYTE  SysStatus;             //_at_ OEMRAM1+0x8F;
 #define  LidStatus          BIT0
 #define  ERR_ShuntDownFlag  BIT1
 #define  FnF3WinKeyDn        BIT2
 #define  FnF3PKeyUp          BIT3
 #define  CloseLid            BIT5
 #define  LidKBIgnore         BIT7

extern XBYTE  LID_DEBOUNCE_CNT;         //_at_ OEMRAM1+0x90;
extern XBYTE  PWRBTN_pressCnt;          //_at_ OEMRAM1+0x91;
extern XBYTE  LID_readytime;            //_at_ OEMRAM1+0x92;
extern XBYTE  ExtTimeCnt;               //_at_ OEMRAM1+0x93;
extern XBYTE  SysStatus2;               //_at_ OEMRAM1+0x94;
 #define  PWRBTN_press_retry BIT0
 #define  b2NOVOKey_Press    BIT2
 #define  b3LCtrl_Pressed    BIT3
 #define  b4VGATemp_Init     BIT4      // For AMD(ATI) used. 1: GPU VGA initial finish.
 #define  LidCloseKBMSDis    BIT5
extern XBYTE  LEDcontrol_bak;           //_at_ OEMRAM1+0x95;
extern XBYTE  Led_status;          //_at_ OEMRAM1+0x96;
 #define  PWRLED_ON           BIT7      // 1: PWRLED ON.   0: PWRLED OFF
 #define  PWRLED_Blinking     BIT6      // 1: PWRLED Blinking. 
 #define  PWRLED_Gradually    BIT5      // 1: Gradually.
 #define  LOGOLED_ON          BIT3      // 1: LOGOLED ON.   0: LOGOLED OFF
 #define  LOGOLED_Blinking    BIT2      // 1: LOGOLED Blinking. 
 #define  LOGOLED_Gradually   BIT1      // 1: LOGOLED Gradually.
 #define  BatLED_test         BIT0      // 1: battery LED test mode.
extern XBYTE  Led_PWM_step;             //_at_ OEMRAM1+0x97;
 #define  LED_countupdown  BIT7      // 0: count up   1:count down.
extern XBYTE  PowerLed_Count;            //_at_(OEMRAM1+0x98);                   
extern XBYTE  LENOVOPMFW_Temp;           //_at_ OEMRAM1+0x99;
 #define  BATTERY_CYCLE_RUNNING   BIT0   //battery Cycle mode running  // Protection Mode
 #define  BATTERY_CALIBRATION_OK  BIT1   // Calibration Finish
 #define  BATTERY_FULLED_100      BIT2   // 100% Fulled ,from nonFull.
 #define  b3EC_ONStatus           BIT3   // EC not entry Sleep status.
 #define  EEPROM_Token            BIT4
 #define  b5EC_ON_EN              BIT5   // 1: EC turn on always electricity.
extern XBYTE  pFKTemp;                   //_at_ OEMRAM1 + 0x9A;
extern XBYTE  Batt0x3CFailRetry5sTimer;  //_at_ OEMRAM1 + 0x9B;
// Batt0x3CFailRetry5sTimer: Timer counter 
// Unit: second.
// Range: 0x01 ~ 0xFE (1 ~ 254) sec. 0xFF = disable.
//
extern XBYTE  cCmd;                //_at_ OEMRAM1+0x9C;
 #define  cC_PCHPWR_Keep    BIT0
 #define  b3BkOff           BIT3
 #define  cC_BkOff          BIT3
 #define  cC_TestBtnEn      BIT6// Send scancode for test button
 #define  cC_LidIgnore      BIT7// Ignore lid function

extern XBYTE  cKBStatus;           //_at_ OEMRAM1+0x9D;
 #define  cKBS_KBType0     BIT0  // Keyboard selection Bit0 control by EC
 #define  cKBS_KBType1     BIT1  // Keyboard selection Bit1 control by EC
 #define  cKBS_KBInit      BIT2  // Keyboard Type initial
 #define  cKBS_TPInS3      BIT3
 #define  cKBS_TPData      BIT4
 #define  cKBS_KBPress     BIT5
 #define  cKBS_KBWakeUp    BIT6  // KB Wake up
 #define  cKBS_TPWakeUp    BIT7  // TP Wake up

extern XBYTE  cBattFlag0;         //_at_ OEMRAM1+0x9E;
 #define  cBF0_Full          BIT0
 #define  cBF0_FullToTarget  BIT1
 #define  cBF0_GoTarget      BIT2
 #define  cBF0_BLMode        BIT3
 #define  cCmdAcOff          BIT7

extern XBYTE  cSPIEEPROM;           //_at_ OEMRAM1+0x9F;     
 #define  cSPI_ROMCopyStart  BIT2           //it's for debug

extern XBYTE  Bat1_FPChgFlag;            //_at_ OEMRAM1+0xA0;

extern XBYTE  cTargetGauge;        //_at_ OEMRAM1+0xA1;


extern XBYTE  cPanelId;              //_at_ OEMRAM1+0xA2;

extern XBYTE  cBrightUpDelayCnt;     //_at_ OEMRAM1+0xA3;
extern XBYTE  cBrightDnDelayCnt;     //_at_ OEMRAM1+0xA4;
extern XBYTE  cRecoveryBrightValue;   //_at_ OEMRAM1+0xA5;

extern XBYTE  cSysActionFlag;        //_at_ OEMRAM1+0xA6;

extern XBYTE  cADPThrottling;        //_at_ OEMRAM1+0xA7;
extern XBYTE  cTHERMThrottling;      //_at_ OEMRAM1+0xA8;
extern XBYTE  cBATTThrottling;       //_at_ OEMRAM1+0xA9;

extern XBYTE  ISCT_Again;            //_at_ OEMRAM1+0xAA;
extern XBYTE  pBattLearn;            //_at_ OEMRAM1+0xAB;
 #define  uACoffstchg      BIT0
 #define  uRefreshmode     BIT1
 #define  us4ResumeACOFF   BIT2
 #define  uACoffdschg      BIT3

extern XBYTE  pFakeCHGCnt;           //_at_ OEMRAM1+0xAC;
extern XBYTE  S0S5PWRcodition;       //_at_ OEMRAM1+0xAD;
 #define S0S5PWROFF    BIT0

extern XBYTE  pProject3;             //_at_ OEMRAM1+0xAE;
 #define  pEntryTB                 BIT0
 #define  pExitTB                  BIT1
 #define  pUpdateCpuTempFromBios   BIT4	//Work-around for Intel issue "Some time PECI will hangup".Update from BIOS.
 #define  pIsMeFlashing            BIT5	//Work-around for avoid the PCH communication fail during ME flashing.
 #define  uReSendBat1ChgQevent     BIT7
 
extern XBYTE  AOURSTCNT;             //_at_ OEMRAM1+0xAF;
  
extern XBYTE  EC_AOU_DBG_FLAG;       //_at_ OEMRAM1+0xB0;    
  // BIT6:0 used for setting battery level for testing.
 #define AOU_DC_LEVEL_DBG    BIT7
extern XBYTE  EC_AOU_PWR_TYPE;       //_at_ OEMRAM1+0xB1;    
  // BIT7:4 AOU port power mode
  // BIT3:0 Non-AOU port power mode

extern XBYTE  EVT_STATUS1;                 //_at_ OEMRAM1+0xB2;  //Event status flag 1    
 #define  bat_in             BIT0            // 1: Battery 1 exist
 #define  adapter_in         BIT1            // 1: AC adapter exist
 #define  wait_PSW_off       BIT2            // 1: waiting for PSW released
 #define  LAN_WAKE_Status    BIT3            //
 #define  KB_FN_Status       BIT4            // 1: Fn Key Pressed
 #define  WLAN_WAKE_Status   BIT5            //


extern XBYTE  EVT_STATUS2;                  //_at_ OEMRAM1+0xB3;  //Event status flag 1    
 #define  DummyFlag          BIT7            // 1:CRT Sense

 
extern XBYTE  uAdpWattCnt;                 //_at_ OEMRAM1 + 0xB4;
extern XWORD  uAdpWattTemp;                //_at_ OEMRAM1 + 0xB5;
extern XBYTE  initMark;                    //_at_ OEMRAM1+0xB7;  // CMW 20121203 for PS8625 translator
extern XBYTE  TEMP_Error;                  //_at_ OEMRAM1+0xB8; // CMW 20121203 for PS8625 translator
extern XBYTE  RFID_eFlashA1;					     //_at_ OEMRAM1+0xB9;
extern XBYTE  RFID_eFlashA0;					     //_at_ OEMRAM1+0xBA;
extern XBYTE  RFID_eEEPROMBlock;				   //_at_ OEMRAM1+0xBB;
extern XBYTE  RFID_eEEPROMAddrsss;			   //_at_ OEMRAM1+0xBC;
extern XBYTE  RFID_eEEPROMData;				     //_at_ OEMRAM1+0xBD;
extern XBYTE  cPWRONFail;                  //_at_ OEMRAM1+0xBE;
extern XBYTE  SYS_MISC1;                   //_at_ OEMRAM1+0xBF;
  #define  G3Mode		BIT3
  #define  Crisis_On    BIT6
  #define  ACPI_OS      BIT7

extern XBYTE  AutoTimer;                   //_at_ OEMRAM1+0xC0;
#define   b7AutoBootTm  BIT7

extern XBYTE  SCI_Event_In_Index;          //_at_ OEMRAM1+0xC1;
extern XBYTE  SCI_Event_Out_Index;         //_at_ OEMRAM1+0xC2;
extern XBYTE  SCI_Event_Buffer[EVENT_BUFFER_SIZE];  //_at_ OEMRAM1+0xC3;  //0x1C3 ~ 0xCA
extern XWORD  ADP_I;                       //_at_ OEMRAM1+0xCB;

extern XBYTE  PM1PendingTXCount;            //_at_ OEMRAM1+0xCD;
extern XBYTE  PM1PendingRXCount;            //_at_ OEMRAM1+0xCE;
extern XBYTE  PM1DataPending[6];            //_at_ OEMRAM1+0xCF;         //0x1CF ~ 0x1D4
extern XBYTE  PM2PendingTXCount;            //_at_ OEMRAM1+0xD5;
extern XBYTE  PM2PendingRXCount;            //_at_ OEMRAM1+0xD6;
extern XBYTE  PM2DataPending[6];            //_at_ OEMRAM1+0xD7;         //0x1D7 ~ 0x1DC 

extern XBYTE  uCritBattWakeThre;          //_at_ OEMRAM1+0xDD

#if Support_IFFS
 #define IFFS_Enable     BIT0
extern XBYTE  uIffsCnt;                    //_at_ OEMRAM1+0xDE
#endif // Support_IFFS
#if SW_ISCT
extern XBYTE  uISCT;                       //_at_ OEMRAM1+0xDF
 #define  b0IRST_WAKE_Enable    BIT0
 #define  b1IRST_WAKE_Finish    BIT1
 #define  b2IRST_TimeRecord     BIT2
 #define  b3ISCT_Permit         BIT3
 #define  b4ISCT_FanDis         BIT4        // ISCT 1:Disable Fan cotrol.
 #define  b5ISCT_BKOFF          BIT5        // ISCT 1:Enable Backlight OFF.
 #define  b6ISCT_Wake           BIT6
 #define  b7ISCT_ShiftKey       BIT7

extern XBYTE  uISCT_2;                      //_at_ OEMRAM1+0xE0
 #define  b0ISCT_pwrBTNpres     BIT0        // Check power button wake up status.
 #define  b1ISCT_LidWake        BIT1        // Check Lid wake up status.
 #define  b2ISCT_WlanLED        BIT2
 #define  b3ISCT_MUTE           BIT3        // ISCT 1:Enable MUTE.
 #define  b4ISCT_Camera         BIT4        // ISCT 1:Disable Camera.
 #define  b5ISCT_PwrLED         BIT5        // ISCT 1:Disable Power LED.
#endif // SW_ISCT



#if NV_GC6
extern XBYTE  GC6EventCnt;               // _at_ OEMRAM1+0xE1
#endif // NV_GC6

extern XBYTE  pACDCCnt;                  // _at_ OEMRAM1+0xE2

extern XBYTE  pTempBank;                 //_at_ OEMRAM1+0xE3;
extern XBYTE  pTempAddress;              //_at_ OEMRAM1+0xE4;

extern XBYTE  nBattTsVolt;               //_at_ OEMRAM1+0xE5;

extern XBYTE  cThrottlingSet;            //_at_ OEMRAM1+0xE6; // 00: Nothing, 01: Thr 12.5%, 02: Thr 25%,..,08: Thr 100%
  #define cTHRM_SW           BIT6
extern XBYTE	 pCPUBasePStage;	     //_at_ OEMRAM1+0xE7;

extern XBYTE  PWRCONTROL;		         //_at_ OEMRAM1 + 0xE8;
  #define MAINPWRONTrig      BIT0
  
extern XBYTE  CHGIC_ReadTempL;		     //_at_ OEMRAM1 + 0xE9;
extern XBYTE  CHGIC_ReadTempH;		     //_at_ OEMRAM1 + 0xEA;
extern XBYTE  LED_PWM_Status;            //_at_ OEMRAM1 + 0xEB;
extern XBYTE  Cgr_LED_Count;             //_at_ OEMRAM1 + 0xEC;
extern XBYTE  IP_GHOSTBEEP;				 //_at_ OEMRAM1 + 0xED;
extern XBYTE  IP_GBEEPCnt;			     //_at_ OEMRAM1 + 0xEE;
#if (CHARGER_IC == BQ25700)
extern XBYTE  ChargerIC_Init;                 //_at_ OEMRAM1 + 0xEF;
#endif

//RESERVE 0x1EC ~ 0x1EF
 #define DebounceRAM     OEMRAM1+0xF0
extern XBYTE  DEBOUNCE_CONT1;            //(DebounceRAM+0x00)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT2;            //(DebounceRAM+0x01)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT3;            //(DebounceRAM+0x02)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT4;            //(DebounceRAM+0x03)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT5;            //(DebounceRAM+0x04)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT6;            //(DebounceRAM+0x05)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT7;            //(DebounceRAM+0x06)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT8;            //(DebounceRAM+0x07)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT9;            //(DebounceRAM+0x08)   //(byte)                      
extern XBYTE  DEBOUNCE_CONT10;           //(DebounceRAM+0x09)   //(byte)                         
extern XBYTE  DEBOUNCE_CONT11;           //(DebounceRAM+0x0A)   //(byte)                         
extern XBYTE  DEBOUNCE_CONT12;           //(DebounceRAM+0x0B)   //(byte)                         
extern XBYTE  DEBOUNCE_CONT13;           //(DebounceRAM+0x0C)   //(byte)                         
extern XBYTE  DEBOUNCE_CONT14;           //(DebounceRAM+0x0D)   //(byte)                         
extern XBYTE  DEBOUNCE_CONT15;           //(DebounceRAM+0x0E)   //(byte)                         
extern XBYTE  DEBOUNCE_CONT16;           //(DebounceRAM+0x0F)   //(byte)                         

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x200-0x2FF   OEM RAM 2
//-------------------------------------------------------------------------------
//*******************************************************************************
extern XBYTE  pProject0;          //_at_ OEMRAM2+0x00
 #define  b0DispToggleEn      BIT0
 #define  bHYBAPUTURBO        BIT1  // 1:Disable CPU Turbo, 0:Enable CPU Turbo
 #define  bAPUTURBO           BIT2  // 1:Disable CPU Turbo, 0:Enable CPU Turbo
 #define  bRSOCAPUTURBO       BIT3  // 1:Disable CPU Turbo, 0:Enable CPU Turbo
 #define  b6PostHotKey        BIT6
 #define  b7SWITCHBAK         BIT7
extern XBYTE  pPROCHOT;            //_at_ OEMRAM2+0x01;
 #define  b0Thermal_PRCOHOTon    BIT0
 #define  b1ISCT_PROCHOTon    BIT1
//extern XBYTE  uMBID;               //_at_ OEMRAM2+0x2;
//BIT6 1: 15" 0:14"
// #define  IS15Inch IS_MASK_SET(uMBID, BIT6)       
// #define  IS14Inch IS_MASK_CLEAR(uMBID, BIT6)   
extern XBYTE  BoardID;             //_at_ OEMRAM2+0x03;
 #define  BoardID_ready    BIT7
 #define  BoardID_readyGET  GET_MASK(BoardID,BoardID_ready)
 #define  BoardID_readyEN   SET_MASK(BoardID,BoardID_ready)
extern XBYTE  GPU_THROTTLE_SRC;             // _at_ OEMRAM2+0x04;
 #define  BAT_HYBRID_OTP    BIT0
 #define  BAT_OCP           BIT1
 #define  ADP_LIMIT_SUPPORT BIT2
 #define  MAX_POWER_CTRL    BIT3
 #define  THERMAL_POLICY    BIT4
 #define  External_Sensor   BIT5
extern XWORD  ADP_ID_REC;   // _at_ OEMRAM2+0x05
extern XBYTE  AC_DROP_COUNT;   //    _at_ OEMRAM2+0x07;
extern XWORD  p2FUDayTemp;    //  _at_ OEMRAM2+0x08;
extern XWORD  p2FUDay;      //  _at_ OEMRAM2+0x0A;
extern XWORD  p2FUD_Year;      //  _at_ OEMRAM2+0x0C;
extern XBYTE  p2FUD_Month;    //  _at_ OEMRAM2+0x0E;
extern XBYTE  p2FUD_Day;      //  _at_ OEMRAM2+0x0F;
extern XBYTE  Port80_Data[15];      //_at_ OEMRAM2+0x10;  // 0x210~0x21E
extern XBYTE  Port80_Index;        //_at_ OEMRAM2+0x1F;
extern XBYTE  pLastSID;          //_at_ OEMRAM2+0x20;
extern XBYTE  pLastSID2;          //_at_ OEMRAM2+0x21;
extern XBYTE  pLastSID3;          //_at_ OEMRAM2+0x22;
extern XBYTE  pLastSID4;          //_at_ OEMRAM2+0x23;
extern XBYTE  cPacketPowerLimit1;      //_at_ OEMRAM2+0x24;
extern XWORD  KeepBattRemineCap;      //_at_ OEMRAM2+0x25;
extern XBYTE  ChkBattery_FCCchg_count;  //_at_ OEMRAM2+0x27;
extern XBYTE  ChkBattery_FCCchg_lastFCCL;  //_at_ OEMRAM2+0x28;
extern XBYTE  ChkBattery_FCCchg_lastFCCH;  //_at_ OEMRAM2+0x29;
extern XBYTE  CountSecAfterPswPressed;  //_at_ OEMRAM2+0x2A;
extern XBYTE  pVCompareFlag;        //_at_ OEMRAM2+0x2B;
 #define  VC0TriggerOn        BIT0
 #define  VC1TriggerOn        BIT1
 #define  VC2TriggerOn        BIT2
extern XWORD  TURBO_V;          //_at_OEMRAM2+0x2C;
extern XBYTE  pEdgeProjectFlag3;//_at_OEMRAM2+0x2E;
#define  pEdgeProjectRSOClow    BIT0

extern XBYTE  pEdgeProjectFlag1;      //_at_OEMRAM2+0x2F;
 #define  pEdgeProjectGhostKey    BIT0
 #define  pEdgeProjectBATIn       BIT1
 #define  pEdgeProjectBATChange   BIT2
 #define  pEdgeProjectDebugIn     BIT3
 #define  pEdgeProjectBATWake     BIT4
 #define  pEdgeTPReset            BIT5
 #define  pEdgeADPIDReady         BIT6
#if NV_GC6
 #define  pEdgeGC6EventHI         BIT7
#endif // NV_GC6

extern XBYTE  pWorkAround;        //_at_OEMRAM2+0x30;
extern XBYTE  pWakeCnt;          //_at_OEMRAM2+0x31;

extern XBYTE  pSMIEvent;          //_at_OEMRAM2+0x32;
extern XBYTE  pEdgeBATStopCharge;      //_at_OEMRAM2+0x33;
extern XBYTE  pEdgeBATStartCharge;    //_at_OEMRAM2+0x34;
extern XWORD  pInhitTimer;        //_at_OEMRAM2+0x35;
// Battery charge inhibit timer has a word-width value in units of minute, and is used to cancel inhibition
// of battery charge. The timer will start when high byte value is written in interface space, therefore low
// byte of timer value should be written first. 
// Writing 0000h cancels battery charge inhibition immediately. 
// If FFFFh is written, the timer will not expire. Reading the timer value is not defined.

extern XBYTE  pEdgeProjectFlag2;      //_at_OEMRAM2+0x37;
 #define  pEdgeProjectBTFWUP       BIT0
 #define  pEdgeACINOUTTrigger      BIT1
 #define  pEdgeNonSUPPORTADP       BIT2
 #define  pEdgeACCUTOFF            BIT3
 #define  pEdgeBATTOver20          BIT4
 #define  pEdgeGPUOTTHROT          BIT5
 #define  pEdgeGPUOCTHROT          BIT6
 #define  pEdgeBATTUnder20         BIT7
extern XBYTE  mPWRDelay;             //_at_ OEMRAM2+0x38;
extern XWORD  ACACOFFEn;          //_at_ OEMRAM2+0x39;
#if (CHARGER_IC == BQ25700)
extern XBYTE  CHGIC_ReadCmd0x31L;       //_at_ OEMRAM2+0x3B;
//  #define TurboBoost        BIT2      //
  #define IFAULT_LOW        BIT4      //
  #define IFAULT_HI        BIT5
  #define EN_IDCHG_REG            BIT6
  #define EN_EXTILIM            BIT7

extern XBYTE  CHGIC_ReadCmd0x31H;       //_at_ OEMRAM2+0x3C;
//  #define ACOKDeglitchTime        BIT4
extern XBYTE  CHGIC_ReadCmd0x3EL;   // _at_(ECRAM2+0x3D);
extern XBYTE  CHGIC_ReadCmd0x3EH;  // _at_(ECRAM2+0x3E);
extern XBYTE  CHGIC_ReadCmd0x30L;            //_at_(ECRAM2+0x3F);
  #define AUTO_WAKEUP_EN        BIT0      //Auto wakeup enable
  #define FET_LATCHOFF          BIT3
extern XBYTE  CHGIC_ReadCmd0x30H;            //_at_(ECRAM2+0x40);

extern XBYTE CHGIC_ReadCmd0x33L;    //_at_(ECRAM2+0x41);
  #define PROCHOT_HOST_EN   BIT5
  #define VSYS_T0           BIT6 
  #define VSYS_T1           BIT7
extern XBYTE CHGIC_ReadCmd0x33H;    //_at_(ECRAM2+0x42);
  #define ICRIT_0        BIT3 
  #define ICRIT_1        BIT4 
  #define ICRIT_2        BIT5
  #define ICRIT_3        BIT6 
  #define ICRIT_4        BIT7 
extern XBYTE CHGIC_ReadCmd0x34L;    //_at_(ECRAM2+0x43);
  #define BATPRES        BIT1 
  #define VSYS           BIT2
   #define IDCHG           BIT3
  #define PROFILE_5      BIT5
extern XBYTE CHGIC_ReadCmd0x34H;    //_at_(ECRAM2+0x44);
#endif  //BQ25700
#if (CHARGER_IC == BQ24780S)
extern XBYTE  CHGIC_ReadCmd0x37L;       //_at_ OEMRAM2+0x3B;
  #define TurboBoost        BIT2      //
  #define IFAULT_LOW        BIT6      //
  #define IFAULT_HI        BIT7

extern XBYTE  CHGIC_ReadCmd0x37H;       //_at_ OEMRAM2+0x3C;
  #define ACOKDeglitchTime        BIT4
  #define EN_IDCHG_REG            BIT7
extern XBYTE  CHGIC_ReadCmd0x39L;   // _at_(ECRAM2+0x3D);
extern XBYTE  CHGIC_ReadCmd0x39H;  // _at_(ECRAM2+0x3E);
extern XBYTE  CHGIC_ReadCmd0x3BL;            //_at_(ECRAM2+0x3F);
extern XBYTE  CHGIC_ReadCmd0x3BH;            //_at_(ECRAM2+0x40);
extern XBYTE  CHGIC_ReadCmd0x3CL;            //_at_(ECRAM2+0x41);
extern XBYTE  CHGIC_ReadCmd0x3CH;            //_at_(ECRAM2+0x42);
extern XBYTE  CHGIC_ReadCmd0x3DL;            //_at_(ECRAM2+0x43);
extern XBYTE  CHGIC_ReadCmd0x3DH;            //_at_(ECRAM2+0x44);
#endif  //BQ24780S

extern XBYTE  pPWRRSTMark1;                  //_at_(ECRAM2+0x45);
extern XBYTE  pPWRRSTMark2;                  //_at_(ECRAM2+0x46);
extern XBYTE  pPWRRSTCnt;                    //_at_(ECRAM2+0x47);
#if (PLATFORM_CHIP == AMD)
extern XBYTE    pAMDPWRBTNCnt;                 //_at_(ECRAM2+0x48);
#endif // PLATFORM_CHIP
extern XBYTE  AdpCUTCnt;          //_at_ OEMRAM2+0x49;
extern XWORD  cCADP_ID;          //_at_OEMRAM2+0x4A;
extern XWORD  cCADP_I;          //_at_OEMRAM2+0x4C;
extern XBYTE  cCheatAD;          //_at_OEMRAM2+0x4E;
 #define  cCAD_ADPID         BIT0
 #define  cCAD_ADPI          BIT1
 #define  cCAD_BATPWR        BIT2
 #define  cCAD_TypeCADPID    BIT3
 #define  cCAD_EN            BIT7

extern XBYTE  PECI_TIMEOUT;        //_at_ OEMRAM2 + 0x4F;
extern XBYTE  PECI_ERROR;        //_at_ OEMRAM2 + 0x50;
extern XBYTE  PECIDelayCnt;        //_at_ OEMRAM2 + 0x51;
 #define PECIBuffer      (* (XBYTE * )( 0x0200 + 0x0E))
extern XWORD  PECIBuffer0;        //_at_ OEMRAM2 + 0x52;
extern XWORD  PECIBuffer1;        //_at_ OEMRAM2 + 0x54;
extern XWORD  PECIBuffer3;        //_at_ OEMRAM2 + 0x56;
extern XWORD  PECIBuffer2;        //_at_ OEMRAM2 + 0x58;
extern XWORD  PECIBuffer4;        //_at_ OEMRAM2 + 0x5A;
extern XWORD  PECIBuffer5;        //_at_ OEMRAM2 + 0x5C;
extern XWORD  PECIBuffer6;        //_at_ OEMRAM2 + 0x5E;
extern XWORD  PECIBuffer7;        //_at_ OEMRAM2 + 0x60;

extern XBYTE  TJMAX;            //_at_ OEMRAM2 + 0x62;
extern XBYTE  PECI_Err_CNT;        //_at_ OEMRAM2 + 0x63;
extern XBYTE  VGA_TempDly;       //_at_ OEMRAM2 + 0x64;

extern XBYTE  PECI_GET_TEMPL;      //_at_ OEMRAM2 + 0x65;
extern XBYTE  PECI_GET_TEMPH;      //_at_ OEMRAM2 + 0x66;

extern XBYTE  ERR_THMSTS;        // _at_ OEMRAM2 + 0x67;
#define  b0ThmICError    BIT0    // Thermal IC read temperature error.
#define  b1VGATempEr      BIT1    // VGA read temperature error.

#define  b3PECITempEr    BIT3    // CPU read temperature error.
#define  b4PCHThmError    BIT4    // PCH Thermal read temperature error.

extern XBYTE  TMErrCnt;          //_at_ OEMRAM2 + 0x68;
extern XBYTE  VGAErrCnt;          //_at_ OEMRAM2 + 0x69;
extern XBYTE  EXTVGAErrCnt;        //_at_ OEMRAM2 + 0x6A;
extern XBYTE  TmlICStep;          //_at_ OEMRAM2 + 0x6B;
extern XBYTE  PCHErrCnt;          //_at_ OEMRAM2 + 0x6C;
extern XWORD  cWriteCurrentPL1;      //_at_ OEMRAM2 + 0x6D;
extern XWORD  cWriteCurrentPL2;      //_at_ OEMRAM2 + 0x6F;
extern XBYTE  MaxCPU_MCHTemp0;   //_at_ OEMRAM2+0x71
extern XBYTE  MaxCPU_MCHTemp1;   //_at_ OEMRAM2 + 0x72;
extern XBYTE  MaxCPU_MCHTemp2;   //_at_ OEMRAM2 + 0x73;
extern XBYTE  MaxCPU_MCHTemp3;   //_at_ OEMRAM2 + 0x74;
extern XWORD  FAN_ID;                //_at_ OEMRAM2+0x75; 
extern XBYTE  FAN_ID_TYPE;           //_at_ OEMRAM2+0x77;
extern XBYTE  nAtmFanSpeed;           //_at_(OEMRAM2+0x78);    // Fan speed. For Lenovo ATM
extern XBYTE  TEMP_Buff_1;             //_at_ OEMRAM2 + 0x79;
extern XBYTE  TEMP_Buff_2;             //_at_ OEMRAM2 + 0x7A;
extern XBYTE  TEMP_Buff_3;             //_at_ OEMRAM2 + 0x7B;
extern XBYTE  EXT_Duty;                //_at_ OEMRAM2 + 0x7C;
extern XBYTE  Target_Duty;             //_at_ OEMRAM2 + 0x7D;
extern XBYTE  MBID_Reload;             //_at_ OEMRAM2 + 0x7E;
 #define  b0MBID_High     BIT0         // MBID reload bit.
 #define  b1MBID_LOW      BIT1         // MBID reload bit.
extern XBYTE  Fan_ON_Count;            //_at_ OEMRAM2 + 0x7F;
extern XBYTE  Fan_Debug_Temp;          //_at_ OEMRAM2 + 0x80;
 #define  b0CPU_Temp       BIT0        // CPU Temperature.
 #define  b1VGA_Temp       BIT1        // VGA Temperature.
 #define  b2EXTVGA_Temp    BIT2        // SLI VGA Temperature.
extern XBYTE  VGA_TBuff1;              //_at_ OEMRAM2 + 0x81;
extern XBYTE  VGA_TBuff2;              //_at_ OEMRAM2 + 0x82;
extern XBYTE  VGA_TBuff3;              //_at_ OEMRAM2 + 0x83;
extern XBYTE  Average_Temp;            //_at_ OEMRAM2 + 0x84;  // Average temperature stage
extern XBYTE  USB_Charger;             //_at_ OEMRAM2 + 085;
 #define  b0USB_EN        BIT0         // USB charger bit, 1:Enable.
 #define  b1USB_initOK    BIT1         // USB charger bit, 1:initial finish.
 #define  b2USB_TmlDis    BIT2         // USB charger bit, 1:Disable USB charger.
 #define  b3USB_IDdeounce BIT3         // USB charger bit, 1:SMSC charger IC Fail.
extern XBYTE  MuteCount;               //_at_ OEMRAM2 + 0x86;
extern XBYTE  Thro_Status;             //_at_ OEMRAM2 + 0x87;
 #define  b0ProCH_CPU      BIT0
 #define  b1ProCH_VGA      BIT1
 #define  b2ProCH_EXTVGA   BIT2
 #define  b3Turbo_CPU      BIT3
 #define  b4Turbo_VGA      BIT4
 #define  b5Turbo_EXTVGA   BIT5
 #define  b6ProCH_TURBO    BIT6
 #define  b7PMAXD2_VGA     BIT7
extern XBYTE  Thro_Status2;             //_at_ OEMRAM2 + 0x88;
 #define  b0Steer_CPU      BIT0
 #define  b1Steer_VGA      BIT1
 #define  b2Steer_EXTVGA   BIT2
 #define  b4FAN1_FullOn    BIT4
 #define  b5BAT_TooLow     BIT5        // Battery under 8.5V must turn off ALW's power.
 #define  b7AC_Again       BIT7
extern XBYTE  CPU_Shut_Cnt;             //_at_ OEMRAM2 + 0x89;
extern XBYTE  VGA_Shut_Cnt;             //_at_ OEMRAM2 + 0x8A;
extern XBYTE  FAN2PRM;                  //_at_ OEMRAM2 + 0x8B;
extern XBYTE  FAN_CpTarget;             //_at_ OEMRAM2 + 0x8C;
extern XBYTE  ManualFan2PRM;            //_at_ OEMRAM2 + 0x8D;
extern XWORD  SHA1_SEED;                //_at_ OEMRAM2 + 0x8E;
extern XBYTE  SHA1failCnt;              //_at_ OEMRAM2 + 0x90;
extern XBYTE  Service_Auth_Step;        //_at_ OEMRAM2 + 0x91
extern XBYTE  FANCPUSeg;                //_at_ OEMRAM2 + 0x92;
extern XBYTE  FANVGASeg;                //_at_ OEMRAM2 + 0x93;
extern XBYTE  FANPCHSeg;                //_at_ OEMRAM2 + 0x94;
extern XBYTE  TargetRPM;                //_at_ OEMRAM2 + 0x95;
extern XBYTE  StatusKeeper;             //_at_ OEMRAM2 + 0x96;
 #define  BatteryFwUpdate    BIT0
 #define  BatteryProtectCHG  BIT1
extern XBYTE  ECSleepCount;             //_at_ OEMRAM2 + 0x97;
extern XBYTE  TouchPadCount;            //_at_ OEMRAM2 + 0x98;
extern XBYTE  EEPROM_PwrSts;            //_at_ OEMRAM2 + 0x99;
extern XBYTE  FNKB_S3Dly;               //_at_ OEMRAM2 + 0x9A;
extern XBYTE  Target_Duty_BCD;          //_at_ OEMRAM2 + 0x9B;  // Target_duty  BCD value
extern XBYTE  FAN_PWM_BCD;              //_at_ OEMRAM2 + 0x9C;  // FAN_PWM duty BCD value
extern XBYTE  TargetRPM_BCD;            //_at_ OEMRAM2 + 0x9D;  // TargetRPM    BCD value
extern XBYTE  Real_RPM_BCD;             //_at_ OEMRAM2 + 0x9E;  // Real_RPM     BCD value
extern XBYTE  PWM_CHG_delay;            //_at_ OEMRAM2 + 0x9F;
extern XBYTE  ManualFanPRM;          //_at_(OEMRAM2+0xA0);                     
extern XBYTE  uReserve07;            //_at_(OEMRAM2+0xA1);                    
 #define nFanManual        BIT6
 #define b7Fan2Manual      BIT7
extern XBYTE  FAN_Tab_OFF;          //_at_(OEMRAM2+0xA2);                     
extern XBYTE  FAN_Tab_LOW;          //_at_(OEMRAM2+0xA3);
extern XBYTE  FAN_Tab_HI;            //_at_(OEMRAM2+0xA4);
extern XBYTE  FAN_Tab_DAC;          //_at_(OEMRAM2+0xA5);
extern XBYTE  VFAN_Tab_HI;          //_at_(OEMRAM2+0xA6);
extern XBYTE  VFAN_Tab_LOW;          //_at_(OEMRAM2+0xA7);
extern XBYTE  VFAN_Tab_DAC;          //_at_(OEMRAM2+0xA8);
extern XBYTE  THR_PRO_OFF;          //_at_(OEMRAM2+0xA9);
extern XBYTE  THR_PRO_ON;            //_at_(OEMRAM2+0xAA);
extern XBYTE  THR_Turo_Rem;          //_at_(OEMRAM2+0xAB);
extern XBYTE  THR_Turo_OFF;          //_at_(OEMRAM2+0xAC);
extern XBYTE  THR_Tab_Shut;          //_at_(OEMRAM2+0xAD);
extern XBYTE  LENOVOPMFW;           //_at_(OEMRAM2+0xAE);    //
 #define  BATTERY_CAL_END    BIT0        //End the calibration cycle  //09111001 Isaac Start
 #define  AUTO_BRIGHTNESS    BIT1         // 1 = Auto Brightness on
 #define  BATTERY_MAIN_CAL   BIT2         //calibration cycle for the main battery
 #define  BATTERY_SEC_CAL    BIT3         //calibration cycle for the second battery
 #define  BATTERY_STORAGE    BIT4         //battery storage mode
 #define  BATTERY_CYCLE      BIT5         //battery Cycle mode
 #define  BATTERY_LONGEST    BIT6         //Battery longest mode
 #define  BATTERY_GBMD       BIT7         //Clear bit of advise user to select battery mode

extern XBYTE  SWITCHABLE;            //_at_(OEMRAM2+0xAF);    //                                         
 #define SWITCHABLEBTN       BIT0          // 1: discrete VGA  0: UMA VGA
 #define HDMI                BIT1          // 1 = HDMI exist
 #define HybridGraphics      BIT2        // 1: Hybrid Graphics 0: External Graphic Only
 #define Switchstate         BIT3        // 1: Switch started  0: Switch complete
 #define EVENT_NOTIFY        BIT4         //send input event notify 1:Send 0:Don't send  //09110502 Isaac
 #define DCRFunction         BIT5        // 1: Enable  0: Disable
 #define ColorEngine         BIT6        // 1: Enable  0: Disable
#if NV_GC6
 #define  GC6_CLAMP          BIT7        // 1: CLAMP ON 0: CLAMP OFF
#endif // NV_GC6
 #define nSwitchableBTNGET  GET_MASK(SWITCHABLE,SWITCHABLEBTN)
 #define nSwitchableBTNEN   SET_MASK(SWITCHABLE,SWITCHABLEBTN)
 #define nSwitchableBTNDIS  CLEAR_MASK(SWITCHABLE,SWITCHABLEBTN)
 #define nHybridGraphicsGET GET_MASK(SWITCHABLE,HybridGraphics)
 #define nHybridGraphicsEN  SET_MASK(SWITCHABLE,HybridGraphics)
 #define nHybridGraphicsDIS CLEAR_MASK(SWITCHABLE,HybridGraphics)
 #define nSwitchstateGET    GET_MASK(SWITCHABLE,Switchstate)
#if NV_GC6
 #define  nGC6ClampOn       SET_MASK(SWITCHABLE,GC6_CLAMP)
 #define  nGC6ClampOff      CLEAR_MASK(SWITCHABLE,GC6_CLAMP)
#endif // NV_GC6
extern XBYTE  SMB3_DATA[32];           //_at_(OEMRAM2+0xB0);  // 0xB0~0xCF
extern XBYTE  nRamTemp;            //_at_(OEMRAM2+0xD0);    // Ram temperature
extern XBYTE  nVramTemp;             //_at_(OEMRAM2+0xD1);    // V Ram temperature

extern XBYTE  BATTUPDATEFW;          //_at_(OEMRAM2+0xD2); 
 #define  PriBattInhib       BIT0        // bit0 1-Disable reading information from primary battery for update battery firmware
 #define  SecBattInhib       BIT1        // bit1 1-Disable reading information from second battery for update battery firmware.

/*
0x37 (Get Battery Information )   Read Block
byte 0,1: The battery maker ID
byte 2,3: Hardware ID
byte 4,5: Firmware version (0 if the firmware is corrupted
byte 6,7: Data version (0 if this is not supported or the firmware is corrupted.
*/
extern XBYTE  ISCT_Timer;          //_at_(OEMRAM2+0xD3); 
extern XBYTE  TP_Type;                           //_at_(OEMRAM2+0xD4);     
#if SW_ISCT
extern XBYTE  AOAC_STATUS;          //_at_(OEMRAM2+0xD5);
 #define  ISCT_BAT_Temp      BIT0        // ISCT BAT temperature limitaion.
 #define  ISCT_BAT_Capy      BIT1        // ISCT BAT capacity limitaion.
 #define  ISCT_S0_ISCT       BIT2        // ISCT S0_ISCT status.
 #define  RESERVEA1_BIT3     BIT3        // Reserved
 #define  ISCT_Enable        BIT4        // ISCT support enable.
 #define  RESERVEA1_BIT5     BIT5        // Reserved
// bit6-7 ISCT wake up status. 01:EC time out, 02:PME(LAN) wake.
#endif // SW_ISCT
extern XBYTE  ACPI_HOTKEY;          //_at_(OEMRAM2+0xD6);    //
#if Support_IFFS
 #define  b5BIOS_IFFS        BIT5        // BIOS enter IFFS status.
#endif // Support_IFFS
 #define  b6Cmd_NoShut       BIT6        // For command use to cut power bit.
 #define  b7BIOS_NoShut      BIT7        // For BIOS used.
extern XBYTE  SYS_STATUS;            //_at_(OEMRAM2+0xD7);
 #define OSTYPE0          BIT0        // OSTYPE Bit2~Bit0 0x00:XP, 0x01:VISTA, 0x02:Linux, 0x03:WIN7, 0x04:WIN8, 0x05:WIN10
 #define OSTYPE1          BIT1        //
 #define OSTYPE2          BIT2        //
 #define RESERVEA3_BIT3   BIT3        // Reserved
 #define RESERVEA3_BIT4   BIT4        // Reserved
 #define RESERVEA3_BIT5   BIT5        // Reserved
 #define AC_ADP           BIT7        // (AC Adapter:0=OffLine, 1=OnLine)

extern XBYTE  ACOFF_SOURCE;          //_at_(OEMRAM2+0xD8);    //
 #define CHARGECURRENT    BIT0        // 1 = AC_OFF for when stop charge but have charge current
 #define ADPOVP           BIT1        // 1 = AC_OFF for adaptor improper
 #define BATTLEARN        BIT2        // 1 = AC_OFF for battery at learning mode
 #define ACMD             BIT3        // 1 = AC_OFF for Command
 #define BATTOVP          BIT4        // 1 = AC_OFF for battery OVP
 #define LEAKCURRENT      BIT5        // 1 = AC_OFF for Leakage current
 #define AIRADP           BIT6        // 1 = AC_OFF for air adaptor
 #define ACOP             BIT7        // 1 = AC_OFF for ACOP
 #define nChargeCurrentEN     SET_MASK(ACOFF_SOURCE,CHARGECURRENT)
 #define nChargeCurrentDIS    CLEAR_MASK(ACOFF_SOURCE,CHARGECURRENT)
 #define nChargeCurrentGET    GET_MASK(ACOFF_SOURCE,CHARGECURRENT)
 #define nAdpOvpEN            SET_MASK(ACOFF_SOURCE,ADPOVP)
 #define nAdpOVPDIS           CLEAR_MASK(ACOFF_SOURCE,ADPOVP)
 #define nBattLearnEN         SET_MASK(ACOFF_SOURCE,BATTLEARN)
 #define nBattLearnDIS        CLEAR_MASK(ACOFF_SOURCE,BATTLEARN)
 #define nAcOffCmdEN          SET_MASK(ACOFF_SOURCE,ACMD)
 #define nAcOffCmdDIS         CLEAR_MASK(ACOFF_SOURCE,ACMD)
 #define nAcOffCmdGET         GET_MASK(ACOFF_SOURCE,ACMD)
 #define nAcOffBattOvpEN      SET_MASK(ACOFF_SOURCE,BATTOVP)
 #define nAcOffBattOvpDIS     CLEAR_MASK(ACOFF_SOURCE,BATTOVP)
 #define nLeakCurrentEN       SET_MASK(ACOFF_SOURCE,LEAKCURRENT)
 #define nLeakCurrentDIS      CLEAR_MASK(ACOFF_SOURCE,LEAKCURRENT)
 #define nLeakCurrentGET      GET_MASK(ACOFF_SOURCE,LEAKCURRENT)
 #define nDis65WOVPEN         SET_MASK(ACOFF_SOURCE,AIRADP)
 #define nDis65WOVPDIS        CLEAR_MASK(ACOFF_SOURCE,AIRADP)
 #define nDis65WOVPGET        GET_MASK(ACOFF_SOURCE,AIRADP)
 #define nACOPEN              SET_MASK(ACOFF_SOURCE,ACOP)
 #define nACOPDIS             CLEAR_MASK(ACOFF_SOURCE,ACOP)

extern XBYTE  CURRENT_STATUS;         //_at_(OEMRAM2+0xD9);    //                     
 #define ENTER_S3      BIT0        // Entry S3 State flag
 #define RESUME_S3     BIT1        // Resume S3 State flag
 #define ENTER_S4      BIT2        // Entry S4 State flag
 #define RESUME_S4     BIT3        // Resume S4 State flag
 #define ENTER_S5      BIT4        // Entry S5 State flag
 #define RESUME_S5     BIT5        // Resume S5 State flag
extern XBYTE  nThrottlingAPSet;        //_at_(OEMRAM2+0xDA);
extern XBYTE  REAL_THROTTLING_INDEX;       //_at_(OEMRAM2+0xDB);
 #define REAL_SW_THRM       BIT6        // (1=SW Throttling active)
 #define REAL_HW_THRM_PIN  BIT7        // (1=HW Throttling Pin active)
 #define nSWThermalPinEN    SET_MASK(REAL_THROTTLING_INDEX,REAL_SW_THRM)
 #define nSWThermalPinDIS   CLEAR_MASK(REAL_THROTTLING_INDEX,REAL_SW_THRM)
 #define nSWThermalPinGET   GET_MASK(REAL_THROTTLING_INDEX,REAL_SW_THRM)
 #define nRealThermalPinEN  SET_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)
 #define nRealThermalPinDIS CLEAR_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)
 #define nRealThermalPinGET GET_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)

extern XBYTE  THROTTLING_HW;          //_at_(OEMRAM2+0xDC);    //HW throttling define     P-MAX          

extern XBYTE  nCpuTempLocal;        //_at_(OEMRAM2+0xDD);    // CPU local temperature 
extern XBYTE  PCH_Temp3;          //_at_(OEMRAM2+0xDE);    // PCH Temperature.
extern XBYTE  VGA_TEMP;          //_at_(OEMRAM2+0xDF);    // VGA temperature
extern XBYTE  EXTVGA_TEMP;          //_at_(OEMRAM2+0xE0);    // External VGA temperature.
extern XBYTE  ACPI_STAT;          //_at_(OEMRAM2+0xE1);
#if NV_GC6
#define GC6ENTRY         BIT4        // Reserved [L10110801] [L11051201]
#else
#define RESERVEB7_BIT4   BIT4        // Reserved
#endif // NV_GC6
extern XBYTE  nBrightValue;          //_at_(OEMRAM2+0xE2);    // LCD Brightness value.
extern XBYTE  nKbProjectID;           //_at_(OEMRAM2+0xE3);
extern XBYTE  CPU_TYPE;             //_at_(OEMRAM2+0xE4);                            
                        // CPU_TYPE : bit0~bit2 0x00:Tj85,    0x01:Tj90,   0x02:Tj100,   0x03:Tj105
                        // CPU TDP : bit3~bit5 0x01:XE(55w), 0x02:QuadCore(45w), 0x03:DualCore(35w), 0x04: UL(25w), 0x00: ULV(17w)
#define  IGPU_EXIST     BIT6
#define  DGPU_EXIST     BIT7
// VGA Type : iGPU(bit6) 1:Exist 0:Non-exist
//            dGPU(bit7) 1:Exist 0:Non-exist

//            Bit[7:6] -- 0x01:UMA, 0x10:Discrete, 0x11:SG(Optimus/PX).
extern XBYTE  nBatteryStatH;           //_at_(OEMRAM2+0xE5);
 #define CMBS_DISCHARGE    BIT0        // Discharging
 #define CMBS_CHARGE       BIT1        // Charging
extern XBYTE  nBattAverTemp;           //_at_(OEMRAM2+0xE6);  // (XBYTE) Battery Average Temp.
extern XBYTE  nBattTempCnt;           //_at_(OEMRAM2+0xE7);  // (XBYTE) Battery current Temp sample counter
extern XBYTE  nBattOvrTempCnt;         //_at_(OEMRAM2+0xE8);  // (XBYTE) Count up if battery is arrived overtemp.
extern XBYTE  nBattErrorCnt;           //_at_(OEMRAM2+0xE9);  // (XBYTE) Communicatiion fail counter
extern XBYTE  nStopChgStat3L;          //_at_(OEMRAM2+0xEA);
 #define  ENSTOPCHG       BIT0        // Battery fast charging timeout
 #define  ENBADCELL       BIT1        // Battery bad cell
 #define  ENCOMMFAIL      BIT2
 #define  ENCHARGESUSP    BIT3        // use for detect battery charging suspend
 #define  ENCMDSTOPCHG    BIT4        // Battery command stop charge
 #define  ENCUSTOMER      BIT5        // Battery stop charge for Customer use
 #define  ENOVERVOLTAGE   BIT6
 #define  ENOVERTEMP      BIT7        // Battery over temperature
extern XBYTE  nStopChgStat3H;          //_at_(OEMRAM2+0xEB);
 #define ENTRITIMEOUT      BIT0        // Battery trickle charging timeout
 #define ENADPTHROTTLING   BIT1        // Stop charge of Adapter throttling
 #define PMTHRESHOLD       BIT2        // Stop Charge Threshold of PM AP
 #define HybridModeStopChg BIT3
 #define ECFLASH           BIT4        // Stop charge of flash
 #define NotLenovoBattery  BIT5        // Stop charge if it is not Lenovo battery
 #define EmStopChgarg      BIT6        // Battery health function used.Flag will set high for stop chgarg when battery chgarg full.
 #define ENBOOSTSTOPCHG    BIT7
extern XBYTE  rACCPAGE[];	           //_at_ OEMRAM2+0xEC ~ 0xF3;	
extern XWORD  cCSTPPBATPWR;           //_at_(OEMRAM2+0xF4);  // word  
extern XWORD  BIOS_SID;               // _at_(OEMRAM2+0xF6);    // word  
extern XWORD  BIOS_SID_REC;           // _at_(OEMRAM2+0xF8);    // word 
extern XBYTE  SMBusRBDataCount;       //         _at_(OEMRAM2+0xFA);
extern XBYTE  I2CRBDataCount;         //         _at_(OEMRAM2+0xFB);
extern XBYTE  LimitedSupportAdpWatt;  //         _at_(OEMRAM2+0xFC);
extern XBYTE  NotSupportAdpWatt;      //         _at_(OEMRAM2+0xFD);
extern XBYTE  Ccb42_States;           //         _at_(OEMRAM2+0xFE);
extern XBYTE  ADPI_MUL;		            //         _at_(OEMRAM2+0xFF);


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x300-0x3FF   OEM RAM 3
//-------------------------------------------------------------------------------
//*******************************************************************************

extern XWORD  uAPSLastX;    //_at_ OEMRAM3+0x00;  // word
extern XWORD  uAPSLastY;    //_at_ OEMRAM3+0x02;  // word

extern XBYTE  uCountX;    //_at_ OEMRAM3+0x04;  // byte
extern XWORD  uTotalX;    //_at_ OEMRAM3+0x05;  // word
extern XBYTE  uCountY;    //_at_ OEMRAM3+0x07;  // byte
extern XWORD  uTotalY;    //_at_ OEMRAM3+0x08;  // word

extern XBYTE  uAPSThresholdX;  //_at_ OEMRAM3+0x0A;  // byte
extern XBYTE  uAPSThresholdY;  //_at_ OEMRAM3+0x0B;  // byte
extern XBYTE  uAPSTimeOut;  //_at_ OEMRAM3+0x0C;  // byte
extern XBYTE  uAPStickCount;  //_at_ OEMRAM3+0x0D;  // byte

extern XWORD  AD_Xdiff;    //_at_ OEMRAM3+0x0E;  // word
extern XWORD  AD_Ydiff;    //_at_ OEMRAM3+0x10;  // word

 #define  BUF1_SIZE 4
 #define  BUF2_SIZE 4

typedef struct _GSensor
{
  BYTE Status;     // G-sensor Status
  BYTE Buf1X_S;    // Start point for buffer 1 of X-chanel
  BYTE Buf1X_E;    // End point for buffer 1 of X-chanel
  BYTE Buf1X_N;    // Store number for buffer 1 of X-chanel
  BYTE Buf1Y_S;    // Start point for buffer 1 of Y-chanel
  BYTE Buf1Y_E;    // End point for buffer 1 of Y-chanel
  BYTE Buf1Y_N;    // Store number for buffer 1 of Y-chanel


  BYTE Buf2X_S;    // Start point for buffer 2 of X-chanel
  BYTE Buf2X_E;    // End point for buffer 2 of X-chanel
  BYTE Buf2X_N;    // Store number for buffer 2 of X-chanel
  BYTE Buf2Y_S;    // Start point for buffer 2 of Y-chanel
  BYTE Buf2Y_E;    // End point for buffer 2 of Y-chanel
  BYTE Buf2Y_N;    // Store number for buffer 2 of Y-chanel


  //BYTE DIO[0x1C];
  //In APS spec. offset=10h~23h
  // Lenovo maybe use 0xF8B0 ~ 0xF8BF some day
  WORD SampleRate;  // Sampling Frequency (Hz)
  BYTE Factor;    // Moving average factor
  WORD Buf1X[BUF1_SIZE];  // ADC for X-chanel
  WORD Buf1Y[BUF1_SIZE];  // ADC for Y-chanel

  WORD Buf2X[BUF2_SIZE];  // Calculate AD for X-chanel
  WORD Buf2Y[BUF2_SIZE];  // Calculate AD for X-chanel

} GSensor;
// G-Monitor Status
extern GSensor GS;        // _at_ OEMRAM3+0x12;    //0x312 ~ 0x342
#define  GS_Enable     BIT0  // 1: Enabled, 0: Disabled
#define  GS_Request    BIT1  // 1: Last Enable request pending, 0:No enable request
#define  GS_Process    BIT2  // 1:Under Data Processing, 0:Wait Raw Data
#define  GS_DiagRun    BIT3  // 1:Diag running, 0:Diag completed
#define  GS_Init       BIT7  // 1:GS init completed, 0:GS no init 

extern XBYTE  ADTOTALHDDCount;    //_at_ OEMRAM3+0x44;
extern XBYTE  APSHDDFLAG;        //_at_ OEMRAM3+0x45;
 #define  APSHDDPRT_STA    BIT0  // 1 = Start APS HDD Protection
 #define  APSHDDPRT_CHK    BIT1  // 1 = Check APS HDD Protection
 #define  APSHDDPRT_SEC    BIT2  // 1 = APS HDD Protection count to 1Sec OK
 #define  APSHDDPRT_MIN    BIT3  // 1 = APS HDD Protection count to 1Min OK
 #define  b4QuickPortect   BIT4  // 1 = Quick Protection.
 #define  b5SlowPortect    BIT5  // 1 = Slow Protection.
 #define  b6PMEWAKE        BIT6  // 1 = PME WAKE.
 #define  b7RTCWAKE        BIT7  // 1 = RTC WAKE.
extern XBYTE  APSHDDCount;    //_at_ OEMRAM3+0x46;
extern XWORD  X_Max180;        //_at_ OEMRAM3+0x47;
extern XWORD  X_Min180;        //_at_ OEMRAM3+0x49;
extern XWORD  Y_Max180;        //_at_ OEMRAM3+0x4B;
extern XWORD  Y_Min180;        //_at_ OEMRAM3+0x4D;
extern XWORD  X_Avg;          //_at_ OEMRAM3+0x4F;
extern XWORD  Y_Avg;          //_at_ OEMRAM3+0x51;
extern XWORD  GS_VOUTX;                        // _at_ OEMRAM3+0x53
extern XWORD  GS_VOUTY;                        // _at_ OEMRAM3+0x55
extern XBYTE  GSSmbusFailCnt;                  //_at_ OEMRAM3+0x57;  //G-sensor communication fail count
extern XBYTE  GSDeviceID;                      //_at_ OEMRAM3+0x58;  //G-sensor Device ID LIS3DH:0x33, LIS3DSH:0x3F, Kx023_1025:0x15
extern XBYTE  StableDelay;                     //_at_ OEMRAM3+0x59;  //Ex85 add for common code;

extern XBYTE cGPUThrottlingSet;                //_at_(OEMRAM3+0x5F);  

extern XWORD Tamper_ram0;                      // _at_(OEMRAM3+0x60);
extern XBYTE Tamper_ram1;                      // _at_(OEMRAM3+0x62);
extern XBYTE Tamper_ram2;                      // _at_(OEMRAM3+0x63);

extern XBYTE AOU_IFG_Debounce_Cnt;             //_at_(OEMRAM3+0x64);

extern XBYTE InputCurrent_Watt;                //_at_(OEMRAM3+0x65);
extern XBYTE ACCutoff_Watt;                    //_at_(OEMRAM3+0x66);

extern XBYTE MpcCpuThrottlingPoint_L;          //_at_(OEMRAM3+0x67);
extern XBYTE MpcCpuThrottlingRecoverPoint_L;   //_at_(OEMRAM3+0x68);
extern XBYTE MpcGpuThrottlingPoint_L;          //_at_(OEMRAM3+0x69);
extern XBYTE MpcGpuThrottlingRecoverPoint_L;   //_at_(OEMRAM3+0x6A);

extern XBYTE  Recovery2Sec;                    //_at_(OEMRAM3+0x6B);
  #define RecoveryCPU         BIT0
  #define RecoveryGPU         BIT2
extern XBYTE  GPUAlarm;                        //_at_(OEMRAM3+0x6C);
  #define GPUAlarm_GPUThrRe   BIT0
  #define GPUAlarm_BATOCP     BIT1
  #define GPUAlarm_ADPOUT     BIT3
  #define GPUAlarm_ADPLimRe   BIT5
  #define GPUAlarm_BATOTP     BIT6
  #define GPUAlarm_TubONOFF   BIT7
extern XBYTE  ACTypeCAlarm;                    //_at_(OEMRAM3+0x6D);
extern XBYTE  cGPUThrottling;                  //_at_(OEMRAM3+0x6E);
extern XBYTE  GPU2Sec;                         //_at_(OEMRAM3+0x6F);

extern XWORD  BatLowVoltage;                   //_at_(OEMRAM3+0x70);
extern XBYTE  BIOS_WDT_Cnt;                    //_at_(OEMRAM3+0x72);

extern XBYTE  ENHANCED_PC_BEEP_RAM0[8];        //_at_(OEMRAM3+0x80);
extern XBYTE  ENHANCED_PC_BEEP_RAM1;	       //_at_(OEMRAM3+0x88);
extern XBYTE  ENHANCED_PC_BEEP_RAM2;	       //_at_(OEMRAM3+0x89);
extern XBYTE  ENHANCED_PC_BEEP_RAM3;	       //_at_(OEMRAM3+0x8A);
extern XBYTE  ENHANCED_PC_BEEP_RAM4;	       //_at_(OEMRAM3+0x8B);
extern XBYTE  ENHANCED_PC_BEEP_RAM5;	       //_at_(OEMRAM3+0x8C);
extern XBYTE  ENHANCED_PC_BEEP_RAM6;	       //_at_(OEMRAM3+0x8D);
extern XBYTE  ENHANCED_PC_BEEP_RAM7;	       //_at_(OEMRAM3+0x8E);
extern XBYTE  ENHANCED_PC_BEEP_RAM8;	       //_at_(OEMRAM3+0x8F);  

extern XBYTE  I2C_DATA;     				   //_at_(OEMRAM3+0xC2);
extern XBYTE  I2C_DATA1[];       		       //_at_(OEMRAM3+0xC3);

extern XBYTE  GSensorSelfTestState;     	   //_at_(OEMRAM3+0xD4);
  #define  DataReady BIT6    // 1:Ready   0:done
  #define  StartTest BIT7    // 1:start   0:done
extern XBYTE  GSensorSelfTestCountX;     	   //_at_(OEMRAM3+0xD5); // wait 50 ms for BMA255 count
extern XBYTE  GSensorTmpX_1;     			   //_at_(OEMRAM3+0xD6);
extern XBYTE  GSensorTmpX_2;     			   //_at_(OEMRAM3+0xD7);
extern XBYTE  GSensorTmpY_1;     			   //_at_(OEMRAM3+0xD8);
extern XBYTE  GSensorTmpY_2;     			   //_at_(OEMRAM3+0xD9);

extern XBYTE  uMBID;     				       //_at_(OEMRAM3+0xDF);
  #define  IS14Inch IS_MASK_SET(uMBID, BIT6)
  #define  IS15Inch IS_MASK_CLEAR(uMBID, BIT6)
// 0x3E0~0x3F7 reserved for BIOS usage   for Think engine
extern XBYTE BiosReserved3E2;       // _at_  OEMRAM3+0xE2;


extern XBYTE Workaround_LPCcount;       // _at_  OEMRAM3+0xF8;
extern XBYTE FANTSSeg;									
//extern XBYTE TEMP_TS_1;									
//extern XBYTE TEMP_TS_2;									

extern XBYTE Debug_Test_3F9;       // _at_  OEMRAM3+0xF9;
extern XBYTE Debug_Test_3FA;       // _at_  OEMRAM3+0xFA;
extern XBYTE Debug_Test_3FB;       // _at_  OEMRAM3+0xFB;   //for MAX power control
extern XBYTE Debug_Test_3FC;       // _at_  OEMRAM3+0xFC;   //for FAN ID
extern XBYTE Debug_Test_3FD;       // _at_  OEMRAM3+0xFD;   //for VGA temperature debug
extern XBYTE Debug_Test_3FE;       // _at_  OEMRAM3+0xFE;   //for CPU temperature debug
extern XBYTE Debug_Test_3FF;       // _at_  OEMRAM3+0xFF;   //for Qevent debug

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x400-0x4FF   OEM RAM 4
//-------------------------------------------------------------------------------
//*******************************************************************************
extern XBYTE  TYPE_C_RAM0;                        //_at_(OEMRAM4+0x00);    //for type-c used
extern XBYTE  TYPE_C_RAM1;                        //_at_(OEMRAM4+0x01);    //for type-c used
extern XBYTE  TYPE_C_RAM2;                        //_at_(OEMRAM4+0x02);    //for type-c used
extern XBYTE  TYPE_C_RAM3;                        //_at_(OEMRAM4+0x03);    //for type-c used
extern XBYTE  TYPE_C_RAM4;                        //_at_(OEMRAM4+0x04);    //for type-c used
extern XBYTE  TYPE_C_RAM5;                        //_at_(OEMRAM4+0x05);    //for type-c used
extern XBYTE  TYPE_C_RAM6;                        //_at_(OEMRAM4+0x06);    //for type-c used
extern XBYTE  TYPE_C_RAM7;                        //_at_(OEMRAM4+0x07);    //for type-c used
extern XBYTE  TYPE_C_RAM8;                        //_at_(OEMRAM4+0x08);    //for type-c used
extern XBYTE  TYPE_C_RAM9;                        //_at_(OEMRAM4+0x09);    //for type-c used
extern XWORD  TYPE_C_RAM10;                       //_at_(OEMRAM4+0x0A);    //for type-c used
extern XWORD  TYPE_C_RAM11;                       //_at_(OEMRAM4+0x0C);    //for type-c used
extern XBYTE  TYPE_C_RAM12;                       //_at_(OEMRAM4+0x0E);    //for type-c used
extern XBYTE  TYPE_C_RAM13;                       //_at_(OEMRAM4+0x0F);    //for type-c used
extern XBYTE  TYPE_C_RAM14;                       //_at_(OEMRAM4+0x10);    //for type-c used

extern XBYTE  TYPE_C_DOCKING_RAM0;                //_at_(OEMRAM4+0x11);
extern XBYTE  TYPE_C_DOCKING_RAM1;                //_at_(OEMRAM4+0x12);
extern XBYTE  TYPE_C_DOCKING_RAM2[4];             //_at_(OEMRAM4+0x13);    // 0x0413~0x0416
extern XBYTE  TYPE_C_RAM15[2];   			      //_at_(OEMRAM4+0x17);
extern XBYTE  TYPE_C_RAM16[2];   			      //_at_(OEMRAM4+0x19);

//UCSI
#if Support_UCSI
extern XWORD  UCSI_RAM0;						  //_at_(OEMRAM4+0x20);    //for UCSI used
extern XWORD  UCSI_RAM1;						  //_at_(OEMRAM4+0x22);    //for UCSI used
extern XBYTE  UCSI_RAM2[4];						  //_at_(OEMRAM4+0x24);    //for UCSI used
extern XBYTE  UCSI_RAM3[8];						  //_at_(OEMRAM4+0x28);    //for UCSI used
extern XBYTE  UCSI_RAM4[16];					  //_at_(OEMRAM4+0x30);    //for UCSI used
extern XBYTE  UCSI_RAM5[16];					  //_at_(OEMRAM4+0x40);    //for UCSI used
extern USB_POWER_STATUS Usb_Pdc_Power_Status[1];  //_at_(OEMRAM4+0x50);    //for UCSI used
extern USB_STATUS Usb_Pdc_Status[1];			  //_at_(OEMRAM4+0x51);    //for UCSI used

extern XBYTE  UCSI_RDO1;       					  //_at_(OEMRAM4+0x52);
extern XBYTE  UCSI_RDO2;       					  //_at_(OEMRAM4+0x53);
extern XBYTE  UCSI_RDO3;      					  //_at_(OEMRAM4+0x54);
extern XBYTE  UCSI_RDO4;     					  //_at_(OEMRAM4+0x55);
extern XBYTE  UCSI_RTS5455_CMD;       			  //_at_(OEMRAM4+0x56);
extern XBYTE  UCSI_RTS5455_WR_DATA_LEN;           //_at_(OEMRAM4+0x57);
extern XBYTE  UCSI_RTS5455_CMD_WAIT;        	  //_at_(OEMRAM4+0x58);
extern XBYTE  UCSI_RTS5455_CMD_WAIT_CNT;       // _at_  OEMRAM4+0x59;
extern XBYTE  UCSI_RTS5455_CMD_ERROR;       	  //_at_(OEMRAM4+0x5A);
extern XBYTE  UCSI_POWER_SOURCE;       // _at_  OEMRAM4+0x05B;
extern XBYTE  UCSI_POWER_DIRECTION_CHANGE;  // _at_  OEMRAM4+0x5C;
extern XBYTE  UCSI_DATA_ROLE;       // _at_  OEMRAM4+0x5D;
extern XBYTE  UCSI_CONNECTOR_PARTNER_CHANGE;       // _at_  OEMRAM4+0x5E;
//extern XBYTE  UCSI_NEGOTIATED_POWER_LEVEL_CHANGE;       // _at_  OEMRAM4+0x5F;
extern XBYTE  UCSI_BATTERY_CHARGE_STATUS_CHANGE;       // _at_  OEMRAM4+0x60;
//extern XBYTE  UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE;       // _at_  OEMRAM4+0x61;
extern XBYTE  UCSI_GET_CONNECTOR_STATUS_RTS5455[9];       // _at_  OEMRAM4+0x62;
extern XBYTE  UCSI_NOTIFICATION_ENABLE[2];       // _at_  OEMRAM4+0x6B;
extern XBYTE  UCSI_OPTIONAL_FEATURE;       // _at_  OEMRAM4+0x6D;
extern XWORD  Ucsi_Connector_Change_Status[NUMBER_OF_USB_PDC];       // _at_  OEMRAM4+0x6E;
//extern XBYTE UCSI_PD_RESET_COMPLETE_CHANGE;       // _at_  OEMRAM4+0x70;
extern XBYTE Ucsi_Connector_Number_Changed;       // _at_  OEMRAM4+0x71;
extern XBYTE UCSI_SUPPORTED_CAM_CHANGE;       // _at_  OEMRAM4+0x72;
extern XBYTE Ucsi_Device_Connected;       // _at_  OEMRAM4+0x73;
extern XBYTE ReTry_P2P;       // _at_  OEMRAM4+0x74;
extern XBYTE  UCSI_RTS5455_CMD_ERROR_CNT;       	  //_at_(OEMRAM4+0x75);
extern XBYTE  Detect_Adaptor_Watt_SKIP;       	  //_at_(OEMRAM4+0x76);
extern XBYTE  P2P_Charge_Disable;       	  //_at_(OEMRAM4+0x77);
extern XBYTE  WlanWakeStatusEEPROM;       	  //_at_(OEMRAM4+0x79);
extern XBYTE  LENOVOP2PSTATUS;       	  //_at_(OEMRAM4+0x7A);
#define  LENOVOStatusFlag BIT0    // 1:p2p   0:no

#endif
extern XBYTE  TYPE_C_RAM17;                     //_at_(OEMRAM4+0x80);    //for type-c used
extern XBYTE  TYPE_C_RAM18;   			   //_at_(OEMRAM4+0x81);
extern XBYTE  TYPE_C_RAM19;   			   //_at_(OEMRAM4+0x82);
extern XBYTE  TYPE_C_RAM20;                     //_at_(OEMRAM4+0x1E);    //for type-c used
extern XBYTE  TYPE_C_RAM21;                     //_at_(OEMRAM4+0x1F);    //for type-c used
extern XBYTE  TYPE_C_RAM22;                     //_at_(OEMRAM4+0xE7);    //for type-c used



//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x500-0x5FF   OEM RAM 5
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAM5        OEMRAM5
extern XWORD  OEM_MpcCP;                  
extern XWORD  OEM_MpcCpuTurboDisablePoint;
extern XWORD  OEM_MpcCpuTurboRecoverPoint;
extern XWORD  OEM_MpcCpuThrottlingPoint;
extern XWORD  OEM_MpcCpuThrottlingRecoverPoint;
extern XWORD  OEM_MpcGpuThrottlingPoint;
extern XWORD  OEM_MpcGpuThrottlingRecoverPoint;
extern XWORD  OEM_MpcCpuThrottlingPoint_L;
extern XWORD  OEM_MpcCpuThrottlingRecoverPoint_L;
extern XWORD  OEM_MpcGpuThrottlingPoint_L;
extern XWORD  OEM_MpcCP_107;
extern XWORD  OEM_MpcGpuThrottlingRecoverPoint_L;
extern XWORD  OEM_MpcAcCpuThrottlingPoint;
extern XWORD  OEM_MpcAcCpuThrottlingRecoverPoint;
extern XWORD  OEM_MpcAcGpuThrottlingPoint;
extern XWORD  OEM_MpcAcGpuThrottlingRecoverPoint;
extern XWORD  OEM_ACACOFFEn;
extern LWORD  OEM_Batt_Output_Power;
#if Support_COMPAL_MFG_TOOL
extern XBYTE  Step0_L;            //_at_(OEMRAM5+0x03);
extern XBYTE  Step0_H;            //_at_(OEMRAM5+0x04);
extern XBYTE  Step0_DAC;          //_at_(OEMRAM5+0x05);
extern XBYTE  Step1_L;            //_at_(OEMRAM5+0x06);
extern XBYTE  Step1_H;            //_at_(OEMRAM5+0x07);
extern XBYTE  Step1_DAC;          //_at_(OEMRAM5+0x08);
extern XBYTE  Step2_L;            //_at_(OEMRAM5+0x09);
extern XBYTE  Step2_H;            //_at_(OEMRAM5+0x0A);
extern XBYTE  Step2_DAC;          //_at_(OEMRAM5+0x0B);
extern XBYTE  Step3_L;            //_at_(OEMRAM5+0x0C);
extern XBYTE  Step3_H;            //_at_(OEMRAM5+0x0D);
extern XBYTE  Step3_DAC;          //_at_(OEMRAM5+0x0E);
extern XBYTE  Step4_L;            //_at_(OEMRAM5+0x0F);
extern XBYTE  Step4_H;            //_at_(OEMRAM5+0x10);
extern XBYTE  Step4_DAC;          //_at_(OEMRAM5+0x11);                                   
#endif

#if Support_COMPAL_MFG_TOOL
extern XBYTE  EXT_NAMESPACE_INDEX;       //_at_(OEMRAM5+0x12;  // 0x5D
extern XBYTE  EXT_NAMESPACE_BANK;      //_at_(OEMRAM5+0x13);  // 0x5E
extern XBYTE  EXT_NAMESPACE_DATA;      //_at_(OEMRAM5+0x14); // 0x5F
#endif

#if Support_COMPAL_MFG_TOOL
extern XBYTE MMIO_rDATA_index;         //_at_ OEMRAM5+0x15;  
extern XBYTE MMIO_CMD_Ctrl;            //_at_ OEMRAM5+0x16;  
 #define MMIO_Processing_flage  BIT0
 #define MMIO_Start_flage       BIT1
extern XBYTE MMIO_rDATA[];             //_at_ OEMRAM5+0x17;  

extern XBYTE MMIO_iCMD;                //_at_ OEMRAM5+0x1F;  
extern XBYTE MMIO_iDATA8C;             //_at_ OEMRAM5+0x20;  
extern XBYTE MMIO_iDATA8D;             //_at_ OEMRAM5+0x21;  
extern XBYTE MMIO_iDATA8E;             //_at_ OEMRAM5+0x22;  
extern XBYTE MMIO_iDATA8F;             //_at_ OEMRAM5+0x23;  
extern XBYTE MMIO_iDATA90;             //_at_ OEMRAM5+0x24;  
#endif

//0x580 ~ 0x5FF reserved for project usage


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x600-0x6FF   OEM RAM 6
//-------------------------------------------------------------------------------
//*******************************************************************************


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
extern XBYTE KB_RAM_Table[128];          //_at_ OEMRAM8+0x00;  // 0x00
extern XBYTE ExKB_RAM_Table[24];        //_at_ OEMRAM8+0x80;  // 0x80

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0x900-0x9FF   OEM RAM 9
//-------------------------------------------------------------------------------
//*******************************************************************************
 #define  PWDSIZE  8
 #define  PWDNUM  16
extern XBYTE  edgePassword[];           //_at_ OEMRAM9 + 0x00;  // 0x0900 ~ 0x097F
extern XBYTE  edgePasswordID;           //_at_ OEMRAM9 + 0x80;
 #define  ePWDIDRead          BIT6
 #define  ePWDIDSet           BIT7

#if Lenovo_Authentication
extern XBYTE  uAuthenRand1;            //_at_ OEMRAM9+0x88;
extern XBYTE  uAuthenRand2;            //_at_ OEMRAM9+0x89;
extern XBYTE  uAuthenRand3;            //_at_ OEMRAM9+0x8A;
extern XBYTE  uAuthenRand4;            //_at_ OEMRAM9+0x8B;
extern XBYTE  uAuthenRPCL;             //_at_ OEMRAM9+0x8C;
extern XBYTE  uAuthenRPCH;             //_at_ OEMRAM9+0x8D;
extern XBYTE  uAuthenRPackL;           //_at_ OEMRAM9+0x8E;
extern XBYTE  uAuthenRPackH;           //_at_ OEMRAM9+0x8F;
extern XWORD  uAuthenNPack;            //_at_ OEMRAM9+0x90;
extern LWORD  uAuthenNPCtemp;          //_at_ OEMRAM9+0x92;
extern XWORD  uAuthenNPC;              //_at_ OEMRAM9+0x96;
extern XBYTE  uAuthenFlag;             //_at_ OEMRAM9+0x98;
 #define  uDF_Write       BIT0         // EC write InQueue OK
 #define  uDF_Read        BIT1         // EC read InQueue OK
 #define  uDF_Return      BIT2         // Battery return calculate result to EC is OK
 #define  uDF_ReadFail    BIT3         // Battery return calculate result to EC is Fail (Communication failed)
 #define  uDF_ErrorReturn BIT4
 #define  uDF_Done        BIT5
 #define  uDF_HWstart     BIT6
 #define  uDF_SWstart     BIT7
extern XBYTE  uAuthenReadAfter1Sec;    //_at_ OEMRAM9+0x99; // EC write Challenge to battery and read after 1 sec
extern XBYTE  uAuthenFailCnt;          //_at_ OEMRAM9+0x9A;
extern XWORD  uAuthenIdx3rd;           //_at_ OEMRAM9+0x9B;
extern XWORD  uAuthenNPack3rd;         //_at_ OEMRAM9+0x9D;
extern XWORD  uAuthenIdx3rdtemp;       //_at_ OEMRAM9+0x9F;
extern LWORD  uAuthenIdx3rdtemp2;      //_at_ OEMRAM9+0xA1;
extern XWORD  uAuthenNPC3rd;           //_at_ OEMRAM9+0xA5;
extern XBYTE  uAuthenFlag3rd;          //_at_ OEMRAM9+0xA7;
 #define  uDF_Write        BIT0        // EC write InQueue OK
 #define  uDF_Read         BIT1        // EC read InQueue OK
 #define  uDF_Return       BIT2        // Battery return calculate result to EC is OK
 #define  uDF_ReadFail     BIT3        // Battery return calculate result to EC is Fail (Communication failed)
 #define  uDF_ErrorReturn  BIT4
 #define  uDF_Done         BIT5
 #define  uDF_3rdStart     BIT6
 #define  uDF_bit7         BIT7
extern XBYTE  uAuthenFailCnt3rd;       //_at_ OEMRAM9+0xA8;
#endif

#if Lenovo_Authentication
extern XBYTE  LV_SW_SIGNERTURE[16];      //_at_ OEMRAM9+0xA9;   //0x9A9~0x9B8
extern XBYTE  edgeProject4;          //_at_ OEMRAM9+0xB9;
 #define uHWAuthTruePass    BIT0
 #define uECAuthTruePass    BIT1
 #define uAuthComplete    BIT2
 #define uLENOVO_Error    BIT3
 #define uLenovo_JapanError  BIT4
 #define uAuthSWstatus    BIT5
 #define uSWDone        BIT6
 #define usLenovo_Error    BIT7
extern XBYTE  p2AuthFailCnt;                 //_at_ OEMRAM9+0xBA;
extern XBYTE  pAuthCommFailCnt;              //_at_ OEMRAM9+0xBB;
#endif

#if Thinkpad_Battery_HW_Auth
extern UWORD  LV_HW_BatteryReserved6;        //_at_ OEMRAM9+0xBC; // 0x03BC ~ 0x03BD
extern UWORD  LV_HW_BatteryResponseX[];      //_at_ OEMRAM9+0xBE; // 0x03BE ~ 0x03C5
extern UWORD  LV_HW_BatteryResponseZ[];      //_at_ OEMRAM9+0xC6; // 0x03C6 ~ 0x03D7
extern XBYTE  LV_HW_BatteryControl;          //_at_ OEMRAM9+0xD8;
 #define  LV_HW_BatteryAuthPassed    BIT0
 #define  LV_HW_BatteryAuthFinished  BIT2
 #define  LV_HW_BatteryDataReady     BIT7
extern XBYTE  LV_HW_BatteryChallengeKey[];    //_at_ OEMRAM9+0xD9; // 0x03D9 ~ 0x03EA
extern UWORD  LV_HW_BatteryCheckValue[];      //_at_ OEMRAM9+0xB3; // 0x03EB ~ 0x03FC
#endif


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xA00-0xAFF   OEM RAM A
//-------------------------------------------------------------------------------
//*******************************************************************************

/***********   TPG Customer RAM   ****************/

extern XBYTE  u4Cfgspace0;         // _at_ OEMRAMA+0x00
 #define  GlobalAtt      BIT1  
 #define  FnFunc         BIT2
 #define  FnSticky       BIT3
 #define  GlobalAttGET   GET_MASK(u4Cfgspace0,GlobalAtt)  
 #define  FnFuncGET      GET_MASK(u4Cfgspace0,FnFunc)
 #define  FnFuncSET      SET_MASK(u4Cfgspace0,FnFunc)
 #define  FnFuncCLR      CLEAR_MASK(u4Cfgspace0,FnFunc)
 #define  FnStickyGET    GET_MASK(u4Cfgspace0,FnSticky)

extern XBYTE  u4Cfgspace1;       //  _at_ OEMRAMA+0x01
 #define  LIDBKSYNC                  BIT0
 #define  CS1_POWER_ON_BY_AC		 BIT1
 #define  CS1_DISABLE_KBD_LIGHT		 BIT2
 #define  CS1_DISABLE_KBD_BACK_LIGHT BIT3
 #define  PBTNWARN                   BIT4
 #define  PalBKLidSyncGET            GET_MASK(u4Cfgspace1,LIDBKSYNC)
 #define  PowerOnByAcGET 			 IS_MASK_SET(u4Cfgspace1,CS1_POWER_ON_BY_AC)
 #define  DisableKbdLightGET 	     IS_MASK_SET(u4Cfgspace1,CS1_DISABLE_KBD_LIGHT)
 #define  DisableKbdBackLightGET     IS_MASK_SET(u4Cfgspace1,CS1_DISABLE_KBD_BACK_LIGHT)
 #define  PwrBtnWarnGET              GET_MASK(u4Cfgspace1,PBTNWARN)

extern XBYTE  u4Cfgspace2;       //   _at_ OEMRAMA+0x02
 #define  USBALWOn        BIT3
 #define  DockingWOLEn    BIT4
 #define  SMART_NumLock   BIT5
 #define  USBALWONGet     GET_MASK(u4Cfgspace2,USBALWOn)
 #define  SrtNumLockGet   GET_MASK(u4Cfgspace2,SMART_NumLock)
extern XBYTE  u4Cfgspace3;       //   _at_ OEMRAMA+0x03
 #define PhantomKeyIgnor	  BIT1
 #define AirplaneMode     BIT3 // Set power control mode to Airplane mode
// When this bit is set, maximum charge current is 1.0A in normal state and is 2.2A in suspend and off state. 
// The capability of the adapter is showed correctly but set a 65W AC adapter mode to DC-DC.
 #define FN_STICKY_LED    BIT4 // 1:Fn Lock LED turns on for Fn Sticky/Lock
 #define GET_FN_STICKY_LED  GET_MASK(u4Cfgspace3,FN_STICKY_LED)
 #define SET_FN_STICKY_LED  SET_MASK(u4Cfgspace3,FN_STICKY_LED)
 #define CLR_FN_STICKY_LED  CLEAR_MASK(u4Cfgspace3,FN_STICKY_LED)

extern XBYTE  u4SoundMask1;      //   _at_ OEMRAMA+0x04
 #define  Reserved      BIT0    // Reserved
 #define  CriLowBatt    BIT1    // Critical low
 #define  LowBatt       BIT2    // Low batt
 #define  Suspend       BIT3    // Suspend
 #define  VMSuspend     BIT4    // VMSsupend
 #define  Resume        BIT5    // Resume
 #define  DCInOut       BIT6    // DC in/out
 #define  PowerOff      BIT7    // Power off
 #define  BeepCriLowGET GET_MASK(u4SoundMask1,CriLowBatt)
 #define  BeepLowGET    GET_MASK(u4SoundMask1,LowBatt)
 #define  SuspendGET    GET_MASK(u4SoundMask1,Suspend)
 #define  VMSuspendGET  GET_MASK(u4SoundMask1,VMSuspend)
 #define  ResumeGET     GET_MASK(u4SoundMask1,Resume)
 #define  DCInOutGET    GET_MASK(u4SoundMask1,DCInOut)
 #define  PowerOffGET   GET_MASK(u4SoundMask1,PowerOff)
extern XBYTE  u4SoundMask2;   //       _at_ OEMRAMA+0x05
 #define  PowerOffWarn  BIT0    // Power off warn
 #define  NoHDDWarn     BIT1    // No HDD warn
 #define  Dead          BIT2    // Dead
 #define  F440Hz        BIT3    // 440Hz
 #define  F315Hz        BIT4    // 315Hz
 #define  F315Hz2Times  BIT5    // 315Hz 2 times
 #define  F315Hz3Times  BIT6    // 315Hz 2 times
 #define  InhibitSwap   BIT7    // Inhibit swap
 #define  PowerOffWarnGET  GET_MASK(u4SoundMask2,PowerOffWarn)
 #define  NoHDDWarnGET     GET_MASK(u4SoundMask2,NoHDDWarn)
 #define  DeadGET          GET_MASK(u4SoundMask2,Dead)
 #define  F440HzGET        GET_MASK(u4SoundMask2,F440Hz)
 #define  F315HzGET        GET_MASK(u4SoundMask2,F315Hz)
 #define  F315Hz2TimesGET  GET_MASK(u4SoundMask2,F315Hz2Times)
 #define  F315Hz3TimesGET  GET_MASK(u4SoundMask2,F315Hz3Times)
 #define  InhibitSwapGET   GET_MASK(u4SoundMask2,InhibitSwap)
extern XBYTE  u4BeepID;        // _at_ OEMRAMA+0x06
extern XBYTE  u4BeepRepeat;    // _at_ OEMRAMA+0x07
extern XBYTE  u4BeepMask3;     // _at_ OEMRAMA+0x08
extern XBYTE  u4KBDID;         // _at_ OEMRAMA+0x09
 #define  KBD_WITH_NUMPAD                 0x00           // Other than Japanese Keyboard
 #define  JP_KBD_WITH_NUMPAD              0x01           // Japanese Keyboard
 #define  CLASSIC_TOP_ROW                 0x04           // Fn key as primary
 #define  NO_KBD                          0x07
 #define  MODERN_TOP_ROW                  0x08           // Media key as primary
 
extern XBYTE  u4Reserve0A;     // _at_ OEMRAMA+0x0A
extern XBYTE  KEYBOARD_TYPE;     // _at_ OEMRAMA+0x0B
// #define KEYBOARD_TYPE          BIT0
 #define GET_KEYBOARD_TYPE  GET_MASK(u4Reserve0B,KEYBOARD_TYPE)   //0: US KBD , 1: JP KBD

extern XBYTE  u4LEDcontrol;    // _at_ OEMRAMA+0x0C
 #define  LED_TURN_ON          BIT7
 #define  LED_Blinking         BIT6
 #define  LED_Blink_gradually  BIT5
 #define  LED_ID  BIT0 + BIT1 + BIT2 + BIT3
 #define  u4LEDcontrolGet    GET_MASK(u4LEDcontrol,LED_TURN_ON)
extern XBYTE  u4PeriCtrl3;     // _at_ OEMRAMA+0x0D
 #define  USB_PWR          BIT0
 #define  RST_TRACK_POINT  BIT2
 #define  BYPASS_CLICKPAD  BIT5
 #define  PC3_TURN_ON_KEYBOARD_BACK_LIGHT  			(u4PeriCtrl3&(BIT6 + BIT7))
 #define  PC3_TURN_ON_KEYBOARD_BACK_LIGHT_MASK  	(BIT6 + BIT7)
 #define  USB_PWRGET           GET_MASK(u4PeriCtrl3,USB_PWR)
 #define  USB_PWREn            SET_MASK(u4PeriCtrl3,USB_PWR)
 #define  USB_PWRDis           CLEAR_MASK(u4PeriCtrl3,USB_PWR)
 #define  RST_TRACK_POINT_GET  GET_MASK(u4PeriCtrl3,RST_TRACK_POINT)
 #define  BYPASS_CLICKPAD_GET  GET_MASK(u4PeriCtrl3,BYPASS_CLICKPAD)

extern XBYTE  u4PeriSTS4;       // _at_ OEMRAMA+0x0E
// BIT[1:0] 
//  Fn key status 
//  00: unlock
//  01: sticky
//  10: lock
//  11: reserved
 #define  STICKY_MODE    BIT0
 #define  STICKY_LOCK    BIT1
 #define  GSENSOR_ERROR  BIT2
 #define  GSENSOR_DIAG_DONE  BIT5
 #define  STICKY_MODE_GET    GET_MASK(u4PeriSTS4,STICKY_MODE)
 #define  STICKY_MODE_SET    SET_MASK(u4PeriSTS4,STICKY_MODE)
 #define  STICKY_MODE_CLR    CLEAR_MASK(u4PeriSTS4,STICKY_MODE) 
 #define  STICKY_LOCK_GET    GET_MASK(u4PeriSTS4,STICKY_LOCK)
 #define  STICKY_LOCK_SET    SET_MASK(u4PeriSTS4,STICKY_LOCK)
 #define  STICKY_LOCK_CLR    CLEAR_MASK(u4PeriSTS4,STICKY_LOCK) 
 #define  GSENSOR_ERROR_SET  SET_MASK(u4PeriSTS4,GSENSOR_ERROR)
 #define  GSENSOR_ERROR_CLR  CLEAR_MASK(u4PeriSTS4,GSENSOR_ERROR)
 #define  GSENSOR_DIAG_DONE_SET  SET_MASK(u4PeriSTS4,GSENSOR_DIAG_DONE)
 #define  GSENSOR_DIAG_DONE_CLR  CLEAR_MASK(u4PeriSTS4,GSENSOR_DIAG_DONE)

//
// BIT2 Gravity sensor diag error
// BIT3 Inhibit charging (this is one while at least one battery is inhibit to charge)
 #define Inhibit_charging  BIT3
// BIT4 External power saving mode
// BIT5 Gravity sensor diagnostic program is running now
// BIT[7:6]
//  Gravity sensor ID
//  00: no sensor
//  01: STMicro
//  10: Memsic
//  11: ADI
 #define  Inhibit_chargingSET   SET_MASK(u4PeriSTS4,Inhibit_charging)
 #define  Inhibit_chargingCLR   CLEAR_MASK(u4PeriSTS4,Inhibit_charging)
 #define  Inhibit_chargingGet   GET_MASK(u4PeriSTS4,Inhibit_charging)
extern XBYTE  u4PeriSTS5;        // _at_ OEMRAMA+0x0F
 #define  PriBattinhibit    BIT0       // The primary battery is inhibited to charge
 #define  SecBattinhibit    BIT1       // The secondary battery is inhibited to charge
 #define  PriBattDisChg     BIT2       // The primary battery is discharging to refresh itself.
 #define  SecBattDisChg     BIT3       // The secondary battery is discharging to refresh itself.
 #define  b4HDDdetached     BIT4       // HDD was detached in suspend.
 #define  BayHDDdetached    BIT5       // Bay HDD was detached in suspend
 #define  NumLockSTS        BIT6       // NumLock state of the embedded keyboad
 #define  PriBattDisChgEn   SET_MASK(u4PeriSTS5,PriBattDisChg)
 #define  PriBattDisChgDis  CLEAR_MASK(u4PeriSTS5,PriBattDisChg)
 #define  PriBattDisChgGet  GET_MASK(u4PeriSTS5,PriBattDisChg)
 #define  PriBattinhibitEn  SET_MASK(u4PeriSTS5,PriBattinhibit)
 #define  PriBattinhibitDis CLEAR_MASK(u4PeriSTS5,PriBattinhibit)
 #define  PriBattinhibitGet GET_MASK(u4PeriSTS5,PriBattinhibit)
 #define  NumLockEn         SET_MASK(u4PeriSTS5,NumLockSTS)
 #define  NumLockDis        CLEAR_MASK(u4PeriSTS5,NumLockSTS)
 #define  NumLockGet        GET_MASK(u4PeriSTS5,NumLockSTS)
extern XBYTE  AttentionOffset;   // _at_ OEMRAMA+0x10
extern XBYTE  u4Reserve11;       // _at_ OEMRAMA+0x11
extern XBYTE  u4Reserve12;       // _at_ OEMRAMA+0x12
extern XBYTE  u4Reserve13;       // _at_ OEMRAMA+0x13
extern XBYTE  u4Reserve14;       // _at_ OEMRAMA+0x14
extern XBYTE  u4Reserve15;       // _at_ OEMRAMA+0x15
extern XBYTE  u4Reserve16;       // _at_ OEMRAMA+0x16
extern XBYTE  u4Reserve17;       // _at_ OEMRAMA+0x17
extern XBYTE  u4Reserve18;       // _at_ OEMRAMA+0x18
extern XBYTE  u4Reserve19;       // _at_ OEMRAMA+0x19
extern XBYTE  u4Reserve1A;       // _at_ OEMRAMA+0x1A
extern XBYTE  u4Reserve1B;       // _at_ OEMRAMA+0x1B
extern XBYTE  u4Reserve1C;       // _at_ OEMRAMA+0x1C
extern XBYTE  u4Reserve1D;       // _at_ OEMRAMA+0x1D
extern XBYTE  u4Reserve1E;       // _at_ OEMRAMA+0x1E
extern XBYTE  u4Reserve1F;       // _at_ OEMRAMA+0x1F
extern XBYTE  u4Reserve20;       // _at_ OEMRAMA+0x20
extern XBYTE  u4InhChgTimerL;    // _at_ OEMRAMA+0x21
extern XBYTE  u4InhChgTimerH;    // _at_ OEMRAMA+0x22
extern XBYTE  u4ExtCommand;      // _at_ OEMRAMA+0x23
extern XBYTE  uThresholdSetting; // _at_ OEMRAMA+0x24 // Battery charge start/ stop threshold setting for the main battery
extern XBYTE  u4Reserve25;       // _at_ OEMRAMA+0x25 // Battery charge start/ stop threshold setting for the secondary battery
 #define  SET_CHARGE_CAPACITY_FLAG BIT7
// 0xA24 and 0xA25
// Battery Charge start/stop threshold setting of the primary(0x24)/seconday(0x25) battery.
// When bit 2 of configuration space 3 is 0, bit 0 - 6 represent the capacity to start to charge.
// Otherwise bit 0-6 represent the capacity to stop charging. 
// Bit 7 is needed to set to 1 when a value is written to this byte. 
// When the command is done, bit 7 is cleared.

extern XBYTE  TamperSTS;          // _at_ OEMRAMA+0x26
 #define b0HDDReset        BIT0
 #define b0mSataReset      BIT1
 #define b0mSata2Reset     BIT2
 #define b0BayHDDReset     BIT3
extern XBYTE  u4Reserve27;        // _at_ OEMRAMA+0x27
extern XBYTE  p2IBMChargeflag;    // _at_ OEMRAMA+0X28
 #define  p2SetInhChgTimer  BIT0
 #define  p2BattExistTemp   BIT1
 #define  p2PeakShiftFlag   BIT2
 #define  p2Reserve28_bit3  BIT3
 #define  p2Reserve28_bit4  BIT4
 #define  p2Reserve28_bit5  BIT5
 #define  p2Reserve28_bit6  BIT6
 #define  p2Reserve28_bit7  BIT7
 #define  p2BattExistTempGet  GET_MASK(p2IBMChargeflag,p2BattExistTemp)
 #define  p2BattExistTempEn   SET_MASK(p2IBMChargeflag,p2BattExistTemp)
 #define  p2BattExistTempDis  CLEAR_MASK(p2IBMChargeflag,p2BattExistTemp)
 #define  p2SetInhChgTimerGet GET_MASK(p2IBMChargeflag,p2SetInhChgTimer)
 #define  p2SetInhChgTimerEn  SET_MASK(p2IBMChargeflag,p2SetInhChgTimer)
 #define  p2SetInhChgTimerDis CLEAR_MASK(p2IBMChargeflag,p2SetInhChgTimer)
extern XBYTE  p2InhChgTimer;                 // _at_ OEMRAMA+0x29
extern XBYTE  u4AttentionReq;                // _at_ OEMRAMA+0x2A
extern XBYTE  u4TripPointCapacity0H;         // _at_ OEMRAMA+0x2B
extern XBYTE  u4TripPointCapacity0L;         // _at_ OEMRAMA+0x2C
extern XBYTE  u4TripPointCapacity1H;         // _at_ OEMRAMA+0x2D
extern XBYTE  u4TripPointCapacity1L;         // _at_ OEMRAMA+0x2E
extern XBYTE  u4Fanspeedcontrol;   // _at_ OEMRAMA+0x2F
 #define  FanAutomaticmode     BIT7     //Automatic mode (Fan speed is controlled by the thermal level)
 #define  FanMaximumspeed      BIT6     //Maximum speed (run at fastest speed)
 #define  Fanspeedlevel        BIT0 + BIT1 + BIT2  //fan speed level (0: stop, 7: highest speed)
 #define  FanAutomaticmodeEn     SET_MASK(u4Fanspeedcontrol,BIT7)
 #define  FanAutomaticmodeDis    CLEAR_MASK(u4Fanspeedcontrol,BIT7)    
 #define  FanAutomaticmodeGet    GET_MASK(u4Fanspeedcontrol,BIT7) 
 #define  FanMaximumspeedSet     SET_MASK(u4Fanspeedcontrol,BIT6)      
 #define  FanMaximumspeedCLR     CLEAR_MASK(u4Fanspeedcontrol,BIT6)      
 #define  FanMaximumspeedGet     GET_MASK(u4Fanspeedcontrol,BIT6)  
extern XBYTE  u4Audio;             // _at_ OEMRAMA+0x30
 #define  Mutevolume   BIT6      //
 #define  MutevolumeEGet    GET_MASK(u4Audio,Mutevolume)  
extern XBYTE  u4Peripheralcontrol2;         // _at_ OEMRAMA+0x31
extern XBYTE  EventMask0;          // _at_ OEMRAMA+0x32
 #define  EnvntWOLAC     BIT0  // Wake On LAN. (AC)
 #define  CritLowBatt    BIT1
 #define  LID39          BIT2
 #define  EjectBtn       BIT3
 #define  FnKeyPress     BIT4
 #define  PortfWake      BIT5
 #define  ModemRing      BIT6  //AOAC
 #define  UltraBayUnlock BIT7
 #define  LidOpenGET     GET_MASK(EventMask0,LID39)
 #define  WOLACWakeEnGet GET_MASK(EventMask0,EnvntWOLAC)
 #define  LidWakeEnGet   GET_MASK(EventMask0,LID39)
extern XBYTE  EventMask1;          // _at_ OEMRAMA+0x33
 #define  LeftBayUnlock  BIT0  // Left Bay Unlock
 #define  EnvntWOLDC     BIT1  // Wake On LAN. (DC)
 #define  WOLDCWakeEnGet GET_MASK(EventMask1,EnvntWOLDC)
extern XBYTE  u4PeriSTS2;          // _at_ OEMRAMA+0x34
 #define  APS2_KBD_LIGHT BIT0
 #define  Beeping        BIT1
 #define  SMBbusy        BIT2
 #define  FanOK          BIT4
 #define  GSexist        BIT5
 #define  APS2_KBD_BACKLIGHT  BIT6
 #define  PWRconWarn     BIT7
 #define  KbdLightExistEN   SET_MASK(u4PeriSTS2,APS2_KBD_LIGHT)
 #define  KbdLightExistDIS  CLEAR_MASK(u4PeriSTS2,APS2_KBD_LIGHT)
 #define  BeepingEN      SET_MASK(u4PeriSTS2,Beeping)
 #define  BeepingDIS     CLEAR_MASK(u4PeriSTS2,Beeping)
 #define  FanExistEN     SET_MASK(u4PeriSTS2,FanOK)
 #define  FanExistDIS    CLEAR_MASK(u4PeriSTS2,FanOK)
 #define  FanExistGET    GET_MASK(u4PeriSTS2,FanOK)
 #define  GExistEN       SET_MASK(u4PeriSTS2,GSexist)   
 #define  GExistDIS      CLEAR_MASK(u4PeriSTS2,GSexist)
 #define  GExistGET      GET_MASK(u4PeriSTS2,GSexist)
 #define  KbdBackLightExistEN   SET_MASK(u4PeriSTS2,APS2_KBD_BACKLIGHT)
 #define  KbdBackLightExistDIS  CLEAR_MASK(u4PeriSTS2,APS2_KBD_BACKLIGHT)

extern XBYTE  u4PeriSTS3;            // _at_ OEMRAMA+0x35
 #define  FanRstOK        BIT2
 #define  AttTempOFF      BIT3  // CMW 20121031 for attention function
 #define  FanErr          BIT4
 #define  UNSUPPORTED_AC  BIT5
 #define  ThermalErr      BIT7
 #define  FanInitEN       SET_MASK(u4PeriSTS3,FanRstOK)
 #define  FanInitDIS      CLEAR_MASK(u4PeriSTS3,FanRstOK)
 #define  FanInitGET      GET_MASK(u4PeriSTS3,FanRstOK)
 #define  AttTempOFFEN    SET_MASK(u4PeriSTS3,AttTempOFF)  // CMW 20121031 for attention function
 #define  AttTempOFFDIS   CLEAR_MASK(u4PeriSTS3,AttTempOFF)
 #define  AttTempOFFGET   GET_MASK(u4PeriSTS3,AttTempOFF)
 #define  FanErrEN        SET_MASK(u4PeriSTS3,FanErr)
 #define  FanErrDIS       CLEAR_MASK(u4PeriSTS3,FanErr)
 #define  FanErrGET       GET_MASK(u4PeriSTS3,FanErr)
 #define  FintekErrEN     SET_MASK(u4PeriSTS3,ThermalErr)
 #define  FintekErrDIS    CLEAR_MASK(u4PeriSTS3,ThermalErr)
 #define  FintekErrGET    GET_MASK(u4PeriSTS3,ThermalErr)
extern XBYTE  u4Reserve36;     //    _at_ OEMRAMA+0x36
extern XBYTE  u4Reserve37;     //    _at_ OEMRAMA+0x37
extern XBYTE  PriBattSTS;      //    _at_ OEMRAMA+0x38
 #define  CapLevel1  BIT0
 #define  CapLevel2  BIT1
 #define  CapLevel3  BIT2
 #define  NonGenBatt BIT3
 #define  BattError  BIT4
 #define  Charging   BIT5
 #define  DisChg     BIT6
 #define  BattExist  BIT7
 #define  NonGenuineBattEn  SET_MASK(PriBattSTS,NonGenBatt)   //(TBD)
 #define  NonGenuineBattDis CLEAR_MASK(PriBattSTS,NonGenBatt)   //(TBD)
 #define  BattErrorEn       SET_MASK(PriBattSTS,BattError)
 #define  BattErrorDis      CLEAR_MASK(PriBattSTS,BattError)
 #define  ChargingEn        SET_MASK(PriBattSTS,Charging)
 #define  ChargingDis       CLEAR_MASK(PriBattSTS,Charging)
 #define  DisChgEn          SET_MASK(PriBattSTS,DisChg)
 #define  DisChgDis         CLEAR_MASK(PriBattSTS,DisChg)
 #define  BattExistEn       SET_MASK(PriBattSTS,BattExist)
 #define  BattExistGet      GET_MASK(PriBattSTS,BattExist)
 #define  BattExistDis      CLEAR_MASK(PriBattSTS,BattExist)
extern XBYTE  SecBattSTS;      // _at_ OEMRAMA+0x39
extern XBYTE  u4PeriCtrl0;     // _at_ OEMRAMA+0x3A
 #define  BTsts      BIT4
 #define  WLANsts    BIT5
 #define  WWANsts    BIT6
 #define  BTstsEn    SET_MASK(u4PeriCtrl0,BTsts)
 #define  BTstsDis   CLEAR_MASK(u4PeriCtrl0,BTsts)
 #define  BTstsGet   GET_MASK(u4PeriCtrl0,BTsts)
 #define  WLANstsEn  SET_MASK(u4PeriCtrl0,WLANsts)
 #define  WLANstsDis CLEAR_MASK(u4PeriCtrl0,WLANsts)
 #define  WLANstsGet GET_MASK(u4PeriCtrl0,WLANsts)
 #define  WWANstsEn  SET_MASK(u4PeriCtrl0,WWANsts)
 #define  WWANstsDis CLEAR_MASK(u4PeriCtrl0,WWANsts)
 #define  WWANstsGet GET_MASK(u4PeriCtrl0,WWANsts)
extern XBYTE    u4PeriCtrl1;      // _at_ OEMRAMA+0x3B
 #define  b0Mute     BIT0
 #define  KBlight    BIT1
 #define  FNLK_STATE BIT3       // Set Fn+Esc lock state, lock if 1, unlock if 0
 #define  USBon      BIT4
 #define  Edge_PriBattInhib  BIT5
 #define  Edge_SecBattInhib  BIT6
 #define  b0MuteGET     GET_MASK(u4PeriCtrl1,b0Mute)    
 #define  b0MuteCPL     CPL_MASK(u4PeriCtrl1,b0Mute)
 #define  b0MuteSET     SET_MASK(u4PeriCtrl1,b0Mute)    
 #define  KBlightGET    GET_MASK(u4PeriCtrl1,KBlight)
 #define  KBlightCPL    CPL_MASK(u4PeriCtrl1,KBlight)
 #define  KBlightSET    SET_MASK(u4PeriCtrl1,KBlight)
 #define  KBlightCLR    CLEAR_MASK(u4PeriCtrl1,KBlight)
 #define  FNLKSTATEGET  GET_MASK(u4PeriCtrl1,FNLK_STATE)    
 #define  FNLKSTATECLR  CLEAR_MASK(u4PeriCtrl1,FNLK_STATE)
 #define  FNLKSTATESET  SET_MASK(u4PeriCtrl1,FNLK_STATE)   
 #define  PriBattInhibGET  GET_MASK(u4PeriCtrl1,Edge_PriBattInhib)
 #define  TurnOnUSBGET  GET_MASK(u4ReserveFD,USBon)
 #define  TurnOnUSBEn   SET_MASK(u4ReserveFD,USBon)
 #define  TurnOnUSBDis  CLEAR_MASK(u4ReserveFD,USBon)
extern XBYTE  u4Reserve3C;     // _at_ OEMRAMA+0x3C
extern XBYTE  u4PassCtrl;      // _at_ OEMRAMA+0x3D
 #define  PassCtrlBusy  BIT7
 #define  PassCtrlBusyDIS CLEAR_MASK(u4PassCtrl,PassCtrlBusy)
 #define  PassCtrlBusyGET GET_MASK(u4PassCtrl,PassCtrlBusy)
extern XBYTE  u4PassData1;     // _at_ OEMRAMA+0x3E
extern XBYTE  u4PassData2;     // _at_ OEMRAMA+0x3F
extern XBYTE  u4PassData3;     // _at_ OEMRAMA+0x40
extern XBYTE  u4PassData4;     // _at_ OEMRAMA+0x41
extern XBYTE  u4PassData5;     // _at_ OEMRAMA+0x42
extern XBYTE  u4PassData6;     // _at_ OEMRAMA+0x43
extern XBYTE  u4PassData7;     // _at_ OEMRAMA+0x44
extern XBYTE  u4PassData8;     // _at_ OEMRAMA+0x45
extern XBYTE  u4SenseSTS0;     // _at_ OEMRAMA+0x46
 #define  Edge_LidStatus  BIT2
 #define  ACSts           BIT4
 #define  LidStsEn        SET_MASK(u4SenseSTS0,Edge_LidStatus)   // Lid is open
 #define  LidStsDis       CLEAR_MASK(u4SenseSTS0,Edge_LidStatus)   // Lid is closed
 #define  ACEn            SET_MASK(u4SenseSTS0,ACSts)
 #define  ACDis           CLEAR_MASK(u4SenseSTS0,ACSts)
 #define  AC_MODE         GET_MASK(u4SenseSTS0,ACSts)
 
extern XBYTE  u4SenseSTS1;     // _at_ OEMRAMA+0x47
 #define HDDisinthebay   BIT3
 #define b6mSatadet      BIT7

extern XBYTE  u4SenseSTS2;     // _at_ OEMRAMA+0x48
 #define Kill_SW         BIT1
 #define DISVGA          BIT4
 #define b6HDDdet        BIT6
 #define Kill_SWEn       SET_MASK(u4SenseSTS2,Kill_SW)
 #define Kill_SWDis      CLEAR_MASK(u4SenseSTS2,Kill_SW)
 #define DisVGAEn        SET_MASK(u4SenseSTS2,DISVGA)
 #define DisVGADis       CLEAR_MASK(u4SenseSTS2,DISVGA)
extern XBYTE  u4SenseSTS3;    // _at_ OEMRAMA+0x49
 #define SomeAcAdapterIn BIT2
extern XBYTE  u4Reserve4A;    // _at_ OEMRAMA+0x4A
extern XBYTE  u4Reserve4B;    // _at_ OEMRAMA+0x4B
extern XBYTE  u4Reserve4C;    // _at_ OEMRAMA+0x4C
extern XBYTE  u4Reserve4D;    // _at_ OEMRAMA+0x4D
extern XBYTE  EventSTS;       // _at_ OEMRAMA+0x4E
 #define EvPME          BIT0
 #define EvCritLowBatt  BIT1
 #define EvLID39        BIT2
 #define EvEjectBtn     BIT3
 #define EvFnKeyPress   BIT4
 #define EvPortfWake    BIT5
 #define EvModemRing    BIT6    //AOAC
 #define EvDockingWOL   BIT7
 #define PMEWakeEn      SET_MASK(EventSTS,EvPME)
 #define PMEWakeGet     GET_MASK(EventSTS,EvPME)
 #define LLBWakeEn      SET_MASK(EventSTS,EvCritLowBatt)
 #define LidWakeEn      SET_MASK(EventSTS,EvLID39)
 #define LidWakeGet     GET_MASK(EventSTS,EvLID39)
 #define KeyWakeEn      SET_MASK(EventSTS,EvFnKeyPress)
 #define EvDockingWOLOn	SET_MASK(EventSTS,EvDockingWOL)
extern XBYTE  u4Reserve4F;    // _at_ OEMRAMA+0x4F
extern XBYTE  uSMBusPrtcl;    // _at_ OEMRAMA+0x50
extern XBYTE  uSMBusSts;      // _at_ OEMRAMA+0x51
 #define SMB_STS_ALARM	BIT6    //AOAC
 #define SMB_STS_DONE	BIT7
extern XBYTE  uSMBusAddr;     // _at_ OEMRAMA+0x52
extern XBYTE  uSMBusCmd;      // _at_ OEMRAMA+0x53
extern XBYTE  uSMBusData0;    // _at_ OEMRAMA+0x54
extern XBYTE  uSMBusData1;    // _at_ OEMRAMA+0x55
extern XBYTE  uSMBusData2;    // _at_ OEMRAMA+0x56
extern XBYTE  uSMBusData3;    // _at_ OEMRAMA+0x57
extern XBYTE  uSMBusData4;    // _at_ OEMRAMA+0x58
extern XBYTE  uSMBusData5;    // _at_ OEMRAMA+0x59
extern XBYTE  uSMBusData6;    // _at_ OEMRAMA+0x5A
extern XBYTE  uSMBusData7;    // _at_ OEMRAMA+0x5B
extern XBYTE  uSMBusData8;    // _at_ OEMRAMA+0x5C
extern XBYTE  uSMBusData9;    // _at_ OEMRAMA+0x5D
extern XBYTE  uSMBusData10;   // _at_ OEMRAMA+0x5E
extern XBYTE  uSMBusData11;   // _at_ OEMRAMA+0x5F
extern XBYTE  uSMBusData12;   // _at_ OEMRAMA+0x60
extern XBYTE  uSMBusData13;   // _at_ OEMRAMA+0x61
extern XBYTE  uSMBusData14;   // _at_ OEMRAMA+0x62
extern XBYTE  uSMBusData15;   // _at_ OEMRAMA+0x63
extern XBYTE  uSMBusData16;   // _at_ OEMRAMA+0x64
extern XBYTE  uSMBusData17;   // _at_ OEMRAMA+0x65
extern XBYTE  uSMBusData18;   // _at_ OEMRAMA+0x66
extern XBYTE  uSMBusData19;   // _at_ OEMRAMA+0x67
extern XBYTE  uSMBusData20;   // _at_ OEMRAMA+0x68
extern XBYTE  uSMBusData21;   // _at_ OEMRAMA+0x69
extern XBYTE  uSMBusData22;   // _at_ OEMRAMA+0x6A
extern XBYTE  uSMBusData23;   // _at_ OEMRAMA+0x6B
extern XBYTE  uSMBusData24;   // _at_ OEMRAMA+0x6C
extern XBYTE  uSMBusData25;   // _at_ OEMRAMA+0x6D
extern XBYTE  uSMBusData26;   // _at_ OEMRAMA+0x6E
extern XBYTE  uSMBusData27;   // _at_ OEMRAMA+0x6F
extern XBYTE  uSMBusData28;   // _at_ OEMRAMA+0x70
extern XBYTE  uSMBusData29;   // _at_ OEMRAMA+0x71
extern XBYTE  uSMBusData30;   // _at_ OEMRAMA+0x72
extern XBYTE  uSMBusData31;   // _at_ OEMRAMA+0x73
extern XBYTE  uSMBusBcnt;     // _at_ OEMRAMA+0x74
extern XBYTE  uSMBusAlarAddr; // _at_ OEMRAMA+0x75
extern XBYTE  uSMBusAlarData; // _at_ OEMRAMA+0x76
extern XBYTE  u4Reserve77;    // _at_ OEMRAMA+0x77
extern XBYTE  u4CpuThermal;   // _at_ OEMRAMA+0x78
extern XBYTE  u4Reserve79;    // _at_ OEMRAMA+0x79
extern XBYTE  u4Reserve7A;    // _at_ OEMRAMA+0x7A
extern XBYTE  u4Reserve7B;    // _at_ OEMRAMA+0x7B
extern XBYTE  u4Reserve7C;    // _at_ OEMRAMA+0x7C
extern XBYTE  u4Reserve7D;    // _at_ OEMRAMA+0x7D
extern XBYTE  u4Reserve7E;    // _at_ OEMRAMA+0x7E
extern XBYTE  u4Reserve7F;    // _at_ OEMRAMA+0x7F
extern XBYTE  u4AttentionCtrl;  // _at_ OEMRAMA+0x80
extern XBYTE  u4BattInfoID;     // _at_ OEMRAMA+0x81
extern XBYTE  u4Reserve82;      // _at_ OEMRAMA+0x82
extern XBYTE  u4Reserve83;      // _at_ OEMRAMA+0x83
extern XBYTE  u4FanSpdL;        // _at_ OEMRAMA+0x84
extern XBYTE  u4FanSpdH;        // _at_ OEMRAMA+0x85
extern XBYTE  u4PasswordSts1;   // _at_ OEMRAMA+0x86
extern XBYTE  u4PasswordSts2;   // _at_ OEMRAMA+0x87
extern XBYTE  u4Reserve88;      // _at_ OEMRAMA+0x88
extern XBYTE  u4Reserve89;      // _at_ OEMRAMA+0x89
extern XBYTE  u4Reserve8A;      // _at_ OEMRAMA+0x8A
extern XBYTE  u4Reserve8B;      // _at_ OEMRAMA+0x8B
extern XBYTE  u4Reserve8C;      // _at_ OEMRAMA+0x8C
extern XBYTE  u4Reserve8D;      // _at_ OEMRAMA+0x8D
extern XBYTE  u4Reserve8E;      // _at_ OEMRAMA+0x8E
extern XBYTE  u4ManualFanPRM;   // _at_ OEMRAMA+0x8F
extern XBYTE  u4Reserve90;      // _at_ OEMRAMA+0x90
extern XBYTE  u4Reserve91;      // _at_ OEMRAMA+0x91
extern XBYTE  u4Reserve92;      // _at_ OEMRAMA+0x92
extern XBYTE  u4Reserve93;      // _at_ OEMRAMA+0x93
extern XBYTE  u4Reserve94;      // _at_ OEMRAMA+0x94
extern XBYTE  u4Reserve95;      // _at_ OEMRAMA+0x95
extern XBYTE  u4Reserve96;      // _at_ OEMRAMA+0x96
extern XBYTE  u4Reserve97;      // _at_ OEMRAMA+0x97
extern XBYTE  u4Reserve98;      // _at_ OEMRAMA+0x98
extern XBYTE  u4Reserve99;      // _at_ OEMRAMA+0x99
extern XBYTE  u4Reserve9A;      // _at_ OEMRAMA+0x9A
extern XBYTE  u4Reserve9B;      // _at_ OEMRAMA+0x9B
extern XBYTE  u4Reserve9C;      // _at_ OEMRAMA+0x9C
extern XBYTE  u4Reserve9D;      // _at_ OEMRAMA+0x9D
extern XBYTE  u4Reserve9E;      // _at_ OEMRAMA+0x9E
extern XBYTE  u4Reserve9F;      // _at_ OEMRAMA+0x9F
extern XBYTE  u4BattInfo1;      // _at_ OEMRAMA+0xA0
extern XBYTE  u4BattInfo2;      // _at_ OEMRAMA+0xA1
extern XBYTE  u4BattInfo3;      // _at_ OEMRAMA+0xA2
extern XBYTE  u4BattInfo4;      // _at_ OEMRAMA+0xA3
extern XBYTE  u4BattInfo5;      // _at_ OEMRAMA+0xA4
extern XBYTE  u4BattInfo6;      // _at_ OEMRAMA+0xA5
extern XBYTE  u4BattInfo7;      // _at_ OEMRAMA+0xA6
extern XBYTE  u4BattInfo8;      // _at_ OEMRAMA+0xA7
extern XBYTE  u4BattInfo9;      // _at_ OEMRAMA+0xA8
extern XBYTE  u4BattInfo10;     // _at_ OEMRAMA+0xA9
extern XBYTE  u4BattInfo11;     // _at_ OEMRAMA+0xAA
extern XBYTE  u4BattInfo12;     // _at_ OEMRAMA+0xAB
extern XBYTE  u4BattInfo13;     // _at_ OEMRAMA+0xAC
extern XBYTE  u4BattInfo14;     // _at_ OEMRAMA+0xAD
extern XBYTE  u4BattInfo15;     // _at_ OEMRAMA+0xAE
extern XBYTE  u4BattInfo16;     // _at_ OEMRAMA+0xAF
extern XBYTE  u4MainBattChgOnCap;  // _at_ OEMRAMA+0xB0
extern XBYTE  u4MainBattChgOffCap; // _at_ OEMRAMA+0xB1
extern XBYTE  u4ReserveB2;         // _at_ OEMRAMA+0xB2
extern XBYTE  u4ReserveB3;         // _at_ OEMRAMA+0xB3
extern XBYTE  u4MainBattCtrl;      // _at_ OEMRAMA+0xB4
extern XBYTE  u4ReserveB5;         // _at_ OEMRAMA+0xB5
extern XBYTE  u4ReserveB6;         // _at_ OEMRAMA+0xB6
extern XBYTE  u4ReserveB7;         // _at_ OEMRAMA+0xB7
extern XBYTE  u4ReserveB8;         // _at_ OEMRAMA+0xB8
extern XBYTE  u4ReserveB9;         // _at_ OEMRAMA+0xB9
extern XBYTE  u4ReserveBA;         // _at_ OEMRAMA+0xBA
extern XBYTE  u4ReserveBB;         // _at_ OEMRAMA+0xBB
extern XBYTE  u4ReserveBC;         // _at_ OEMRAMA+0xBC
extern XBYTE  u4ReserveBD;         // _at_ OEMRAMA+0xBD
extern XBYTE  u4ReserveBE;         // _at_ OEMRAMA+0xBE
extern XBYTE  u4ReserveBF;         // _at_ OEMRAMA+0xBF
extern XBYTE  u4ReserveC0;         // _at_ OEMRAMA+0xC0
extern XBYTE  u4ReserveC1;         // _at_ OEMRAMA+0xC1
extern XBYTE  u4ReserveC2;         // _at_ OEMRAMA+0xC2
extern XBYTE  u4ReserveC3;         // _at_ OEMRAMA+0xC3
 #define DOCKONLAN          BIT0
extern XBYTE  u4ReserveC4;         // _at_ OEMRAMA+0xC4
extern XBYTE  u4ReserveC5;         // _at_ OEMRAMA+0xC5
extern XBYTE  u4ReserveC6;         // _at_ OEMRAMA+0xC6
extern XBYTE  u4ReserveC7;         // _at_ OEMRAMA+0xC7
extern XBYTE  u4ReserveC8;         // _at_ OEMRAMA+0xC8
extern XBYTE  u4ACAdaptorCap;      // _at_ OEMRAMA+0xC9
extern XBYTE  u4ReserveCA;         // _at_ OEMRAMA+0xCA
extern XBYTE  u4ReserveCB;         // _at_ OEMRAMA+0xCB
extern XBYTE  u4AC_PwrH;           // _at_ OEMRAMA+0xCC
extern XBYTE  u4AC_PwrL;           // _at_ OEMRAMA+0xCD
extern XBYTE  u4Cfgspace4;         // _at_ OEMRAMA+0xCE
 #define  u4Cfgspace4FnCtrl  BIT4 // 0: normal, 1: Fn/Ctrl swap
 #define  FnCtrlSWAPGet      GET_MASK(u4Cfgspace4,u4Cfgspace4FnCtrl)
extern XBYTE  u4ReserveCF;         // _at_ OEMRAMA+0xCF
extern XBYTE  u4ReserveD0;         // _at_ OEMRAMA+0xD0
extern XBYTE  u4PriBattType;       // _at_ OEMRAMA+0xD1
//bit 0 - 3: reserved
#define  u4PriBattTypeHWauthenticated  BIT4
#define  u4PriBattTypeAuthorizedBatt   BIT5
#define  u4PriBattTypeGenuineBatt      BIT6
#define  u4PriBattTypeValid            BIT7
extern XBYTE  u4ReserveD2;          // _at_ OEMRAMA+0xD2
extern XBYTE  u4ReserveD3;          // _at_ OEMRAMA+0xD3
extern XBYTE  u4ReserveD4;          // _at_ OEMRAMA+0xD4
extern XBYTE  u4ReserveD5;          // _at_ OEMRAMA+0xD5
extern XBYTE  u4ReserveD6;          // _at_ OEMRAMA+0xD6
extern XBYTE  u4ReserveD7;          // _at_ OEMRAMA+0xD7
extern XBYTE  u4ReserveD8;          // _at_ OEMRAMA+0xD8
extern XBYTE  u4ReserveD9;          // _at_ OEMRAMA+0xD9
extern XBYTE  u4ReserveDA;          // _at_ OEMRAMA+0xDA
extern XBYTE  u4ReserveDB;          // _at_ OEMRAMA+0xDB
extern XBYTE  u4ReserveDC;          // _at_ OEMRAMA+0xDC
extern XBYTE  u4ReserveDD;          // _at_ OEMRAMA+0xDD
extern XBYTE  u4ReserveDE;          // _at_ OEMRAMA+0xDE
extern XBYTE  u4ReserveDF;          // _at_ OEMRAMA+0xDF
extern XBYTE  u4ReserveE0;          // _at_ OEMRAMA+0xE0
extern XBYTE  u4ReserveE1;          // _at_ OEMRAMA+0xE1
extern XBYTE  u4ReserveE2;          // _at_ OEMRAMA+0xE2
extern XBYTE  u4ReserveE3;          // _at_ OEMRAMA+0xE3
extern XBYTE  u4ReserveE4;          // _at_ OEMRAMA+0xE4
extern XBYTE  u4ReserveE5;          // _at_ OEMRAMA+0xE5
extern XBYTE  u4ReserveE6;          // _at_ OEMRAMA+0xE6
extern XBYTE  u4ReserveE7;          // _at_ OEMRAMA+0xE7
extern XBYTE  u4VersionH;           // _at_ OEMRAMA+0xE8
extern XBYTE  u4VersionL;           // _at_ OEMRAMA+0xE9
extern XBYTE  u4VersionT;           // _at_ OEMRAMA+0xEA
extern XBYTE  u4SpecversionL;       // _at_ OEMRAMA+0xEB
extern XBYTE  u4EcCapability0;      // _at_ OEMRAMA+0xEC
 #define capSmartBatt    BIT2          // True: Smart Battery Data V1.1 Support
 #define capTwoBatt      BIT5          // True: Secondary Battery Support
 #define capRefFunc      BIT6          // True: Discharge with AC battery function is supported.
 #define capSmartBattEn  SET_MASK(u4EcCapability0, capSmartBatt)
 #define capSmartBattDis CLEAR_MASK(u4EcCapability0, capSmartBatt)
 #define capTwoBattEn    SET_MASK(u4EcCapability0, capTwoBatt)
 #define capTwoBattDis   CLEAR_MASK(u4EcCapability0, capTwoBatt)
 #define capRefFuncEn    SET_MASK(u4EcCapability0, capRefFunc)
 #define capRefFuncDis   CLEAR_MASK(u4EcCapability0,capRefFunc)
extern XBYTE  u4EcCapability1;      // _at_ OEMRAMA+0xED
 #define capNoFAN      BIT0          // this system has no fan.
 #define capBattLVL    BIT1          // Highest battery level byte (0EEh) is valid.
 #define capFnDual     BIT2          // Fn Key can function as another key.
 #define capEnhTWR     BIT3          // version information of the interface is gotten through TWR/Access Bus interface.
 #define capATM        BIT5          // adaptive thermal management functions are available.
 #define Fn_primary    BIT7         // Fn key as primary support
 #define capNoFANEn    SET_MASK(u4EcCapability1, capNoFAN)
 #define capNoFANDis   CLEAR_MASK(u4EcCapability1, capNoFAN)
 #define capBattLVLEn  SET_MASK(u4EcCapability1, capBattLVL)
 #define capBattLVLDis CLEAR_MASK(u4EcCapability1, capBattLVL)
 #define capFnDualEn   SET_MASK(u4EcCapability1, capFnDual)
 #define capFnDualDis  CLEAR_MASK(u4EcCapability1, capFnDual)
 #define capEnhTWREn   SET_MASK(u4EcCapability1, capEnhTWR)
 #define capEnhTWRDis  CLEAR_MASK(u4EcCapability1, capEnhTWR)
 #define capATMEn      SET_MASK(u4EcCapability1, capATM)
 #define capATMDis     CLEAR_MASK(u4EcCapability1, capATM)
extern XBYTE  u4HighBattLV;   // _at_ OEMRAMA+0xEE
extern XBYTE  u4SpecversionH; // _at_ OEMRAMA+0xEF
extern XBYTE  u4BuildID1;     // _at_ OEMRAMA+0xF0
extern XBYTE  u4BuildID2;     // _at_ OEMRAMA+0xF1
extern XBYTE  u4BuildID3;     // _at_ OEMRAMA+0xF2
extern XBYTE  u4BuildID4;     // _at_ OEMRAMA+0xF3
extern XBYTE  u4BuildID5;     // _at_ OEMRAMA+0xF4
extern XBYTE  u4BuildID6;     // _at_ OEMRAMA+0xF5
extern XBYTE  u4BuildID7;     // _at_ OEMRAMA+0xF6
extern XBYTE  u4BuildID8;     // _at_ OEMRAMA+0xF7
extern XBYTE  u4BuildDate1;   // _at_ OEMRAMA+0xF8
extern XBYTE  u4BuildDate2;   // _at_ OEMRAMA+0xF9
extern XBYTE  u4BuildTime1;   // _at_ OEMRAMA+0xFA
extern XBYTE  u4BuildTime2;   // _at_ OEMRAMA+0xFB
extern XBYTE  SMB_PRTC0;    // _at_ OEMRAMA+0xFC
extern XBYTE  u4ReserveFD;    // _at_ OEMRAMA+0xFD
extern XBYTE  u4ReserveFE;    // _at_ OEMRAMA+0xFE
extern XBYTE  u4ReserveFF;    // _at_ OEMRAMA+0xFF

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xB00-0xBFF   OEM RAM B
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAMB        OEMRAMB


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xC00-0xCFF   OEM RAM C
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAMC        OEMRAMC
#define ECRAMD        OEMRAMD

#if Thinkpad_Battery_HW_Auth
extern XBYTE    LV_HW_BatteryReserved1;        //_at_ ECRAMC+0x01;
extern gf2n_t  gf2n_t_a;              //_at_ ECRAMC+0x02; // 0x0C02 ~ 0x0C13
extern UWORD  LV_HW_BatteryReserved2;        //_at_ ECRAMC+0x14; // 0x0C14 ~ 0x0C15
extern UWORD  gf2n_temp[];            //_at_ ECRAMC+0x16; // 0x0C16 ~ 0x0C37
extern UWORD  LV_HW_BatteryReserved3;        //_at_ ECRAMC+0x38; // 0x0C38 ~ 0x0C39
extern UWORD  LV_HW_BatteryReserved4;        //_at_ ECRAMC+0x4C; // 0x0C4C ~ 0x0C4D
extern UWORD  LV_HW_BatteryReserved5;        //_at_ ECRAMC+0x4E; // 0x0C4E ~ 0x0C4F
#endif  // Thinkpad_Battery_HW_Auth


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xD00-0xDFF   OEM RAM D
//-------------------------------------------------------------------------------
//*******************************************************************************

//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xE00-0xEFF   OEM RAM E
//-------------------------------------------------------------------------------
//*******************************************************************************
/***********   EDGE Customer RAM for Battery  ****************/
//extern XBYTE EDGE_BATTERY_INFO[];        //_at_ OEMRAME+0x00;
extern XBYTE TWR00;                      // EDGE_BATTERY_INFO+0x00
//#define TWR00       EDGE_BATTERY_INFO[0x00]
 #define  TWR_BIT0                    BIT0
 #define  TWR_BIT1                    BIT1
 #define  TWR_BIT2                    BIT2
 #define  AttachedPowerNOTsupport     BIT3
 #define  AuthenticationFailed        BIT4
 #define  SecondaryBattAttached       BIT5
 #define  MainBattAttached            BIT6
 #define  ACattached                  BIT7
 #define  AttachedPowerNOTsupportEn   SET_MASK(TWR00, AttachedPowerNOTsupport)
 #define  AttachedPowerNOTsupportDis  CLEAR_MASK(TWR00, AttachedPowerNOTsupport)
 #define  AttachedPowerNOTsupportGET  GET_MASK(TWR00, AttachedPowerNOTsupport)
 #define  AuthenticationFailedEn      SET_MASK(TWR00, AuthenticationFailed)
 #define  AuthenticationFailedDis     CLEAR_MASK(TWR00, AuthenticationFailed)
 #define  AuthenticationFailedGET     GET_MASK(TWR00, AuthenticationFailed)
 #define  SecondaryBattAttachedEn     SET_MASK(TWR00, SecondaryBattAttached)
 #define  SecondaryBattAttachedDis    CLEAR_MASK(TWR00, SecondaryBattAttached)
 #define  SecondaryBattAttachedGET    GET_MASK(TWR00, SecondaryBattAttached)
 #define  MainBattAttachedEn          SET_MASK(TWR00, MainBattAttached)
 #define  MainBattAttachedDis         CLEAR_MASK(TWR00, MainBattAttached)
 #define  MainBattAttachedGET         GET_MASK(TWR00, MainBattAttached)
 #define  ACattachedEn                SET_MASK(TWR00, ACattached)
 #define  ACattachedDis               CLEAR_MASK(TWR00, ACattached)
 #define  ACattachedGET               GET_MASK(TWR00, ACattached)
extern XBYTE  TWR01;                      // EDGE_BATTERY_INFO+0x01
//#define TWR01       EDGE_BATTERY_INFO[0x01]
 #define  BatteryCritical         BIT0
 #define  Error                   BIT1
 #define  CapacityMode            BIT2
 #define  CriticalLow             BIT3
 #define  Discharge               BIT4
 #define  Charge                  BIT5
 #define  Commestablished         BIT6
 #define  TWRvalid                BIT7
 #define  BatteryCriticalEn       SET_MASK(TWR01, BatteryCritical)
 #define  BatteryCriticalDis      SET_MASK(TWR01, BatteryCritical)
 #define  BatteryCriticalGET      GET_MASK(TWR01, BatteryCritical)
 #define  ErrorEn                 SET_MASK(TWR01, Error)
 #define  ErrorDis                CLEAR_MASK(TWR01, Error)
 #define  ErrorGET                GET_MASK(TWR01, Error)
 #define  CapacityModeEn          SET_MASK(TWR01, CapacityMode)
 #define  CapacityModeDis         SET_MASK(TWR01, CapacityMode)
 #define  CapacityModeGET         GET_MASK(TWR01, CapacityMode)
 #define  CriticalLowEn           SET_MASK(TWR01, CriticalLow)
 #define  CriticalLowDis          CLEAR_MASK(TWR01, CriticalLow)
 #define  CriticalLowGET          GET_MASK(TWR01, CriticalLow)
 #define  CommestablishedEn       SET_MASK(TWR01, Commestablished)
 #define  CommestablishedDis      CLEAR_MASK(TWR01, Commestablished)
 #define  CommestablishedGET      GET_MASK(TWR01, Commestablished)
 #define  TWRvalidEn              SET_MASK(TWR01, TWRvalid)
 #define  TWRvalidDis             SET_MASK(TWR01, TWRvalid)
 #define  TWRvalidGET             GET_MASK(TWR01, TWRvalid)
extern XBYTE  MajorVer;                      // EDGE_BATTERY_INFO+0x02
//#define MajorVer    EDGE_BATTERY_INFO[0x02]
extern XBYTE  MinorVer;                      // EDGE_BATTERY_INFO+0x03
//#define MinorVer    EDGE_BATTERY_INFO[0x03]
extern XBYTE  TWRInfo;                      // EDGE_BATTERY_INFO+0x04
//#define TWRInfo     EDGE_BATTERY_INFO[0x04]
extern XBYTE  AdpWatt;                      // EDGE_BATTERY_INFO+0x05
////#define AdpWatt     EDGE_BATTERY_INFO[0x05]
extern XBYTE  TWR06;                    // EDGE_BATTERY_INFO+0x06
//#define TWR06       EDGE_BATTERY_INFO[0x06]
 #define  TWR06Reserve_BIT0    BIT0
 #define  TWR06Reserve_BIT1    BIT1
 #define  TWR06Reserve_BIT2    BIT2
 #define  TWR06Reserve_BIT3    BIT3
 #define  HWauthSupport        BIT4
 #define  TWR06Reserve_BIT5    BIT5
 #define  TWR06Reserve_BIT6    BIT6
 #define  TWR06Reserve_BIT7    BIT7
extern XBYTE  u3Reserve07;                  // EDGE_BATTERY_INFO+0x07
//#define u3Reserve07 EDGE_BATTERY_INFO[0x07]
extern XBYTE  u3Reserve08;                  // EDGE_BATTERY_INFO+0x08
extern XBYTE  u3Reserve09;                  // EDGE_BATTERY_INFO+0x09
extern XBYTE  u3Reserve0A;                  // EDGE_BATTERY_INFO+0x0A
extern XBYTE  u3Reserve0B;                  // EDGE_BATTERY_INFO+0x0B
extern XBYTE  u3Reserve0C;                  // EDGE_BATTERY_INFO+0x0C
extern XBYTE  u3Reserve0D;                  // EDGE_BATTERY_INFO+0x0D
extern XBYTE  u3Reserve0E;                  // EDGE_BATTERY_INFO+0x0E
extern XBYTE  u3Reserve0F;                  // EDGE_BATTERY_INFO+0x0F
extern XBYTE  TWR10;                    // EDGE_BATTERY_INFO+0x10
//#define TWR10   EDGE_BATTERY_INFO[0x10]
extern XBYTE  TWR11;                    // EDGE_BATTERY_INFO+0x11
//#define TWR11   EDGE_BATTERY_INFO[0x11]
extern XBYTE  BattModeL;                  // EDGE_BATTERY_INFO+0x12
//#define BattModeL   EDGE_BATTERY_INFO[0x12]
extern XBYTE  BattModeH;                  // EDGE_BATTERY_INFO+0x13
//#define BattModeH   EDGE_BATTERY_INFO[0x13]
extern XBYTE  TemperatureL;                  // EDGE_BATTERY_INFO+0x14
extern XBYTE  TemperatureH;                  // EDGE_BATTERY_INFO+0x15
extern XBYTE  VoltageL;                      // EDGE_BATTERY_INFO+0x16
extern XBYTE  VoltageH;                      // EDGE_BATTERY_INFO+0x17
extern XBYTE  CurrentL;                      // EDGE_BATTERY_INFO+0x18
extern XBYTE  CurrentH;                      // EDGE_BATTERY_INFO+0x19
extern XBYTE  AvgCurrentL;                  // EDGE_BATTERY_INFO+0x1A
extern XBYTE  AvgCurrentH;                  // EDGE_BATTERY_INFO+0x1B
extern XBYTE  RelativeStateOfChgL;            // EDGE_BATTERY_INFO+0x1C
extern XBYTE  RelativeStateOfChgH;            // EDGE_BATTERY_INFO+0x1D
extern XBYTE  RemainCapL;                  // EDGE_BATTERY_INFO+0x1E
extern XBYTE  RemainCapH;                  // EDGE_BATTERY_INFO+0x1F 
extern XBYTE  TWR20;        // EDGE_BATTERY_INFO+0x20
//#define TWR20   EDGE_BATTERY_INFO[0x20]
extern XBYTE  TWR21;        // EDGE_BATTERY_INFO+0x21
//#define TWR21   EDGE_BATTERY_INFO[0x21]
extern XBYTE  FullChgCapL;         // EDGE_BATTERY_INFO+0x22
extern XBYTE  FullChgCapH;         // EDGE_BATTERY_INFO+0x23
extern XBYTE  RunTimeToEmptyL;     // EDGE_BATTERY_INFO+0x24
extern XBYTE  RunTimeToEmptyH;     // EDGE_BATTERY_INFO+0x25
extern XBYTE  AvgTimeToEmptyL;     // EDGE_BATTERY_INFO+0x26
extern XBYTE  AvgTimeToEmptyH;     // EDGE_BATTERY_INFO+0x27
extern XBYTE  AvgTimeToFullL;      // EDGE_BATTERY_INFO+0x28
extern XBYTE  AvgTimeToFullH;      // EDGE_BATTERY_INFO+0x29
extern XBYTE  BattStatusL;         // EDGE_BATTERY_INFO+0x2A  
 #define SMBerror0    BIT0          // error code bit0
 #define SMBerror1    BIT1          // error code bit1
 #define SMBerror2    BIT2          // error code bit2
 #define SMBerror3    BIT3          // error code bit3
 #define FullyDsg     BIT4          // statusfully discharged
 #define FullyChg     BIT5          // statusfully charged
 #define Dsg          BIT6          // statusdischarging
 #define Initialized  BIT7          // statusgauge initialized
extern XBYTE  BattStatusH;         // EDGE_BATTERY_INFO+0x2B
extern XBYTE  CycleCountL;         // EDGE_BATTERY_INFO+0x2C
extern XBYTE  CycleCountH;         // EDGE_BATTERY_INFO+0x2D
extern XBYTE  FETTemperatureL;     // EDGE_BATTERY_INFO+0x2E
extern XBYTE  FETTemperatureH;     // EDGE_BATTERY_INFO+0x2F
extern XBYTE  TWR30;               // EDGE_BATTERY_INFO+0x30
//#define TWR30   EDGE_BATTERY_INFO[0x30]
extern XBYTE  TWR31;               // EDGE_BATTERY_INFO+0x31
//#define TWR31   EDGE_BATTERY_INFO[0x31]
extern XBYTE  DesignCapL;          // EDGE_BATTERY_INFO+0x32
extern XBYTE  DesignCapH;          // EDGE_BATTERY_INFO+0x33
extern XBYTE  DesignVoltL;         // EDGE_BATTERY_INFO+0x34
extern XBYTE  DesignVoltH;         // EDGE_BATTERY_INFO+0x35
extern XBYTE  SpecInfoL;           // EDGE_BATTERY_INFO+0x36
extern XBYTE  SpecInfoH;           // EDGE_BATTERY_INFO+0x37
extern XBYTE  ManufactureDateL;    // EDGE_BATTERY_INFO+0x38
extern XBYTE  ManufactureDateH;    // EDGE_BATTERY_INFO+0x39
extern XBYTE  SerialNumberL;       // EDGE_BATTERY_INFO+0x3A
extern XBYTE  SerialNumberH;       // EDGE_BATTERY_INFO+0x3B
extern XBYTE  WarrantyPeriodL;     // EDGE_BATTERY_INFO+0x3C
extern XBYTE  WarrantyPeriodH;     // EDGE_BATTERY_INFO+0x3D
extern XBYTE  ACsourceInforL;      // EDGE_BATTERY_INFO+0x3E
 #define TypeC_ADP      BIT0           
 #define Classic_ADP    BIT1
extern XBYTE  ACsourceInforH;      // EDGE_BATTERY_INFO+0x3F
extern XBYTE  TWR40;               // EDGE_BATTERY_INFO+0x40
//#define TWR40   EDGE_BATTERY_INFO[0x40]
extern XBYTE  TWR41;               // EDGE_BATTERY_INFO+0x41
//#define TWR41   EDGE_BATTERY_INFO[0x41]
extern XBYTE  ManufacturerName[];  // EDGE_BATTERY_INFO+0x42
extern XBYTE  TWR50;               // EDGE_BATTERY_INFO+0x50
//#define TWR50   EDGE_BATTERY_INFO[0x50]
extern XBYTE  TWR51;               // EDGE_BATTERY_INFO+0x51
//#define TWR51   EDGE_BATTERY_INFO[0x51]
extern XBYTE  DeviceName[];        // EDGE_BATTERY_INFO+0x52

extern XBYTE  TWR60;               // EDGE_BATTERY_INFO+0x60
//#define TWR60   EDGE_BATTERY_INFO[0x60]
extern XBYTE  TWR61;               // EDGE_BATTERY_INFO+0x61
//#define TWR61   EDGE_BATTERY_INFO[0x61]
extern XBYTE  DeviceChemistry[];   // EDGE_BATTERY_INFO+0x62

extern XBYTE  TWR70;               // EDGE_BATTERY_INFO+0x70
//#define TWR70   EDGE_BATTERY_INFO[0x70]
extern XBYTE  TWR71;               // EDGE_BATTERY_INFO+0x71
//#define TWR71   EDGE_BATTERY_INFO[0x71]
extern XBYTE  OptMfgFunction5[];   //_at_ OEMRAME+0x72;    // EDGE_BATTERY_INFO+0x72
extern XBYTE  TWR80;               // EDGE_BATTERY_INFO+0x80
//#define TWR80   EDGE_BATTERY_INFO[0x80]
extern XBYTE  TWR81;               // EDGE_BATTERY_INFO+0x81
//#define TWR81   EDGE_BATTERY_INFO[0x81]

extern XBYTE  OptMfgFunction1L;    // EDGE_BATTERY_INFO+0x82
extern XBYTE  OptMfgFunction1H;    // EDGE_BATTERY_INFO+0x83
extern XBYTE  OptMfgFunction2L;    // EDGE_BATTERY_INFO+0x84
 #define  DUAL_MODE_CAPABILITY  BIT5
 #define  DUAL_MODE_SELECT      BIT6
// OptMfgFunction2L
// BIT0: CAPACITY_CORRECTION  1: enable 0: disable
// BIT1-2: Preservation (Open for system use)
// BIT3: EXPRESS_CHARGE
// BIT4: CLEAN_SHEET2       1: enable 0: disable
// BIT5: Dual Mode Capability (0: No Dual Mode Capability, 1:Dual Mode Capability, Read Only)
// BIT6: Dual Mode Select(0: Long runtime mode(Default), 1:Long Lifespan mode, 
// This bit must be kept until next write operation and must not be reset at OFF 
// State)
//
// Bit7: Non-replaceable battery
// 0 : Replaceable battery
// 1 : Non-replaceable battery
extern XBYTE  OptMfgFunction2H;    // EDGE_BATTERY_INFO+0x85
 #define COMMON_DUAL_MODE    BIT0
// OptMfgFunction2H
// BIT0 : Common Dual Mode, 0 : No Common Dual Mode Capability, 1 : Common Dual Mode Capability)
// 
extern XBYTE  FirmwareUpdateStatusL;  // EDGE_BATTERY_INFO+0x86
extern XBYTE  FirmwareUpdateStatusH;  // EDGE_BATTERY_INFO+0x87
extern XBYTE  BatterymakerIDL;  // EDGE_BATTERY_INFO+0x88
extern XBYTE  BatterymakerIDH;  // EDGE_BATTERY_INFO+0x89
extern XBYTE  HardwareIDL;      // EDGE_BATTERY_INFO+0x8A
extern XBYTE  HardwareIDH;      // EDGE_BATTERY_INFO+0x8B
extern XBYTE  FirmwareversionL;  // EDGE_BATTERY_INFO+0x8C
extern XBYTE  FirmwareversionH;  // EDGE_BATTERY_INFO+0x8D
extern XBYTE  DataVersionL;      // EDGE_BATTERY_INFO+0x8E
extern XBYTE  DataVersionH;      // EDGE_BATTERY_INFO+0x8F
extern XBYTE  TWR90;        // EDGE_BATTERY_INFO+0x90
//#define TWR90   EDGE_BATTERY_INFO[0x90]
extern XBYTE  TWR91;        // EDGE_BATTERY_INFO+0x91
//#define TWR91   EDGE_BATTERY_INFO[0x91]

extern XBYTE  ManufactureAccessL;  // EDGE_BATTERY_INFO+0x92
extern XBYTE  ManufactureAccessH;  // EDGE_BATTERY_INFO+0x93
extern XBYTE  MaxErroL;          // EDGE_BATTERY_INFO+0x94
extern XBYTE  MaxErroH;          // EDGE_BATTERY_INFO+0x95
extern XBYTE  ChargeCurrL;      // EDGE_BATTERY_INFO+0x96
extern XBYTE  ChargeCurrH;      // EDGE_BATTERY_INFO+0x97
extern XBYTE  ChargeVoltL;      // EDGE_BATTERY_INFO+0x98
extern XBYTE  ChargeVoltH;      // EDGE_BATTERY_INFO+0x99
extern XBYTE  u3Reserve9A;      // EDGE_BATTERY_INFO+0x9A
extern XBYTE  u3Reserve9B;      // EDGE_BATTERY_INFO+0x9B
extern XBYTE  u3Reserve9C;      // EDGE_BATTERY_INFO+0x9C
extern XBYTE  u3Reserve9D;      // EDGE_BATTERY_INFO+0x9D
extern XBYTE  u3Reserve9E;      // EDGE_BATTERY_INFO+0x9E
extern XBYTE  u3Reserve9F;      // EDGE_BATTERY_INFO+0x9F
extern XBYTE  TWRA0;        // EDGE_BATTERY_INFO+0xA0
//#define TWRA0   EDGE_BATTERY_INFO[0xA0]
extern XBYTE  TWRA1;        // EDGE_BATTERY_INFO+0xA1
//#define TWRA1   EDGE_BATTERY_INFO[0xA1]
extern XBYTE  ManufactureData[];  //_at_ OEMRAME+0xA2;    // EDGE_BATTERY_INFO+0xA2
extern XBYTE  Major;        // EDGE_BATTERY_INFO+0xAE
extern XBYTE  Minor;        // EDGE_BATTERY_INFO+0xAF
extern XBYTE  TWRB0;        // EDGE_BATTERY_INFO+0xB0
//#define TWRB0   EDGE_BATTERY_INFO[0xB0]
extern XBYTE  TWRB1;        // EDGE_BATTERY_INFO+0xB1
//#define TWRB1   EDGE_BATTERY_INFO[0xB1]
extern XBYTE  BattUsageCondition[];  //_at_ OEMRAME+0xB2;    // EDGE_BATTERY_INFO+0xB2
extern XBYTE  u3ReserveBC;      // EDGE_BATTERY_INFO+0xBC
extern XBYTE  u3ReserveBD;      // EDGE_BATTERY_INFO+0xBD
extern XBYTE  u3ReserveBE;      // EDGE_BATTERY_INFO+0xBE
extern XBYTE  u3ReserveBF;      // EDGE_BATTERY_INFO+0xBF
extern XBYTE  TWRC0;          // EDGE_BATTERY_INFO+0xC0
//#define TWRC0   EDGE_BATTERY_INFO[0xC0]
extern XBYTE  TWRC1;          // EDGE_BATTERY_INFO+0xC1
//#define TWRC1   EDGE_BATTERY_INFO[0xC1]
extern XBYTE  BattAuthStat;      // EDGE_BATTERY_INFO+0xC2
//#define BattAuthStat    EDGE_BATTERY_INFO[0xC2]
 #define  TWRC2Reserve_BIT0    BIT0
 #define  TWRC2Reserve_BIT1    BIT1
 #define  TWRC2Reserve_BIT2    BIT2
 #define  TypeCADPauth         BIT3
 #define  TypeC4APPpass        BIT4
 #define  HWauthFail           BIT5
 #define  OEMauthPass          BIT6
 #define  SWauthPass           BIT7
extern XBYTE  u3ReserveC3;      // EDGE_BATTERY_INFO+0xC3
extern XBYTE  u3ReserveC4;      // EDGE_BATTERY_INFO+0xC4
extern XBYTE  u3ReserveC5;      // EDGE_BATTERY_INFO+0xC5
extern XBYTE  u3ReserveC6;      // EDGE_BATTERY_INFO+0xC6
extern XBYTE  u3ReserveC7;      // EDGE_BATTERY_INFO+0xC7
extern XBYTE  u3ReserveC8;      // EDGE_BATTERY_INFO+0xC8
extern XBYTE  u3ReserveC9;      // EDGE_BATTERY_INFO+0xC9
extern XBYTE  u3ReserveCA;      // EDGE_BATTERY_INFO+0xCA
extern XBYTE  u3ReserveCB;      // EDGE_BATTERY_INFO+0xCB
extern XBYTE  u3ReserveCC;      // EDGE_BATTERY_INFO+0xCC
extern XBYTE  u3ReserveCD;      // EDGE_BATTERY_INFO+0xCD
extern XBYTE  u3ReserveCE;      // EDGE_BATTERY_INFO+0xCE
extern XBYTE  u3ReserveCF;      // EDGE_BATTERY_INFO+0xCF
extern XBYTE  u3ReserveD0;      // EDGE_BATTERY_INFO+0xD0
extern XBYTE  u3ReserveD1;      // EDGE_BATTERY_INFO+0xD1
extern XBYTE  u3ReserveD2;      // EDGE_BATTERY_INFO+0xD2
extern XBYTE  u3ReserveD3;      // EDGE_BATTERY_INFO+0xD3
extern XBYTE  u3ReserveD4;      // EDGE_BATTERY_INFO+0xD4
extern XBYTE  u3ReserveD5;      // EDGE_BATTERY_INFO+0xD5
extern XBYTE  u3ReserveD6;      // EDGE_BATTERY_INFO+0xD6
extern XBYTE  u3ReserveD7;      // EDGE_BATTERY_INFO+0xD7
extern XBYTE  u3ReserveD8;      // EDGE_BATTERY_INFO+0xD8
extern XBYTE  u3ReserveD9;      // EDGE_BATTERY_INFO+0xD9
extern XBYTE  u3ReserveDA;      // EDGE_BATTERY_INFO+0xDA
extern XBYTE  u3ReserveDB;      // EDGE_BATTERY_INFO+0xDB
extern XBYTE  u3ReserveDC;      // EDGE_BATTERY_INFO+0xDC
extern XBYTE  u3ReserveDD;      // EDGE_BATTERY_INFO+0xDD
extern XBYTE  u3ReserveDE;      // EDGE_BATTERY_INFO+0xDE
extern XBYTE  u3ReserveDF;      // EDGE_BATTERY_INFO+0xDF
extern XBYTE  u3ReserveE0;      // EDGE_BATTERY_INFO+0xE0
extern XBYTE  u3ReserveE1;      // EDGE_BATTERY_INFO+0xE1
extern XBYTE  u3ReserveE2;      // EDGE_BATTERY_INFO+0xE2
extern XBYTE  u3ReserveE3;      // EDGE_BATTERY_INFO+0xE3
extern XBYTE  u3ReserveE4;      // EDGE_BATTERY_INFO+0xE4
extern XBYTE  u3ReserveE5;      // EDGE_BATTERY_INFO+0xE5
extern XBYTE  u3ReserveE6;      // EDGE_BATTERY_INFO+0xE6
extern XBYTE  u3ReserveE7;      // EDGE_BATTERY_INFO+0xE7
extern XBYTE  u3ReserveE8;      // EDGE_BATTERY_INFO+0xE8
extern XBYTE  u3ReserveE9;      // EDGE_BATTERY_INFO+0xE9
extern XBYTE  u3ReserveEA;      // EDGE_BATTERY_INFO+0xEA
extern XBYTE  u3ReserveEB;      // EDGE_BATTERY_INFO+0xEB
extern XBYTE  u3ReserveEC;      // EDGE_BATTERY_INFO+0xEC
extern XBYTE  u3ReserveED;      // EDGE_BATTERY_INFO+0xED
extern XBYTE  u3ReserveEE;      // EDGE_BATTERY_INFO+0xEE
extern XBYTE  u3ReserveEF;      // EDGE_BATTERY_INFO+0xEF
extern XBYTE  u3ReserveF0;      // EDGE_BATTERY_INFO+0xF0
extern XBYTE  u3ReserveF1;      // EDGE_BATTERY_INFO+0xF1
extern XBYTE  u3ReserveF2;      // EDGE_BATTERY_INFO+0xF2
extern XBYTE  u3ReserveF3;      // EDGE_BATTERY_INFO+0xF3
extern XBYTE  u3ReserveF4;      // EDGE_BATTERY_INFO+0xF4
extern XBYTE  u3ReserveF5;      // EDGE_BATTERY_INFO+0xF5
extern XBYTE  u3ReserveF6;      // EDGE_BATTERY_INFO+0xF6
extern XBYTE  u3ReserveF7;      // EDGE_BATTERY_INFO+0xF7
extern XBYTE  u3ReserveF8;      // EDGE_BATTERY_INFO+0xF8
extern XBYTE  u3ReserveF9;      // EDGE_BATTERY_INFO+0xF9
extern XBYTE  u3ReserveFA;      // EDGE_BATTERY_INFO+0xFA
extern XBYTE  u3ReserveFB;      // EDGE_BATTERY_INFO+0xFB
extern XBYTE  u3ReserveFC;      // EDGE_BATTERY_INFO+0xFC
extern XBYTE  u3ReserveFD;      // EDGE_BATTERY_INFO+0xFD
extern XBYTE  u3ReserveFE;      // EDGE_BATTERY_INFO+0xFE
extern XBYTE  u3ReserveFF;      // EDGE_BATTERY_INFO+0xFF


//*******************************************************************************
//-------------------------------------------------------------------------------
//  0xF00-0xFFF   OEM RAM F
//-------------------------------------------------------------------------------
//*******************************************************************************
extern XBYTE    KB_LOC_Table[128];//            _at_ OEMRAMF+0x80;  // 0x80   //ivan

#endif
