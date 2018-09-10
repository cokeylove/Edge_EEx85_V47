/*----------------------------------------------------------------------------
 * Filename: OEM_PROJECT.H  For Chipset: ITE.IT85XX
 *
 * Function: Project function selection.
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 *---------------------------------------------------------------------------*/
  
#ifndef OEM_Project_H
#define OEM_Project_H

//*****************************************************************************
// Kernel code feature
//*****************************************************************************
#define Changing_timing_base   TRUE 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ITE EC Function define
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define Support_EC_Sleep      TRUE    // Support EC sleep function.
#define Support_External_IO   FALSE   // Support EC External IO function.

#define WDT_Support           FALSE    // Disable for SDV's SMT phase.

// CMW 20121022 +
#define Support_Mirror_Code   FALSE  
#define Support_Mirror_Always_EN   TRUE  //Ture : mirror always enable, the first boot after EC flash will clear mirror size, This is follow LBG design
#define Support_VC0_Function  FALSE  // Support Voltage Compare 0 Function.
#define Support_VC1_Function  FALSE  // Support Voltage Compare 1 Function.
#define Support_VC2_Function  FALSE  // Support Voltage Compare 2 Function.
// CMW 20121022 -

#define Support_EC_Port80_Debug TRUE
#define Support_RAM_Debug       TRUE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ITE EC Function Setting and selection
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CMW 20121022 +
#if Support_Mirror_Code
#define EC_Signature_Flag     0x14    // For 8518 crystal free
#else
//IT89966-------------------------
#define EC_Signature_Flag     0x14  
//IT89966-------------------------


//IT8586/8986-------------------------
//#define EC_Signature_Flag     0xB5   
//IT8586/8986-------------------------
#endif


#if Support_Mirror_Code
#define EC_Mirror_Size        0xE7   //232K
#else
//IT89966-------------------------
#define EC_Mirror_Size        0x00
//IT89966-------------------------

//IT8586/8986-------------------------
//#define EC_Mirror_Size        0xAA
//IT8586/8986-------------------------

#endif

// CMW 20121022 -

//-----------------------------------------------------------------------------
// PS2 interface
//-----------------------------------------------------------------------------
#define TouchPad_only         TRUE    // True : Only touch pad device in this project
                                      /* Note : If TouchPad_only is set, remeber to call
                                          PowerOffClearTPPendingData() function in power off sequnce
                                          include (S3, S4, and S5)!C*/

#if TouchPad_only
#define MultiplexedModes      FALSE   // 0 : Doesn't support multiplexed modes.
#else
#define MultiplexedModes      TRUE    // 1 : Support multiplexed modes.
#endif

//-----------------------------------------------------------------------------
// Power sequence only
//-----------------------------------------------------------------------------
#define EN_PwrSeqTest         FALSE  // For SDV's SMT phase used.
                                     // Disable other functions beside power sequence.

//-----------------------------------------------------------------------------
// Keyboard scan
//-----------------------------------------------------------------------------
#define Support_Keyboard      CS16KB    //support CS13KB/CS16KB
#define KB_FnFuncKey          TRUE
#define KB_FnSticky_Key       TRUE
#define KB_Primary_Key        TRUE

#define SUPPORT_KB_Matix      Matrixtable2   //Matrixtable1 : KB matrix is follow up Original E series, Matrixtable2 follow up Keyboard design
#define SUPPORT_KB_RAM_TABLE  TRUE   // KB matrix placed at RAM(TRUE) or ROM(FALSE).

#define ACTION                0x03

//*****************************************************************************
// UART debug interface
// Timer 2 baudrate = Fosc / 32 / (0x10000-RCAP2)
//*****************************************************************************
#define UART_Debug        // Define : enable Tx debug (GPIO)

#ifdef UART_Debug
// 9600 bps-------------------------
//#define R2HV                  0xFF
//#define R2LV                  0xE2
// 9600 bps-------------------------

// 19200 bps------------------------
//#define R2HV                  0xFF
//#define R2LV                  0xF1
// 19200 bps------------------------

// 57600 bps------------------------
#define R2HV                    0xFF
#define R2LV                    0xFB
// 57600 bps------------------------
#endif

//*****************************************************************************
// OEM code feature
//*****************************************************************************
#define Init_FMSSR            0x1F  // 0x1F 2M Bytes Share Rom
#define Init_BADR             0x01  // Base address select 0b00:2E 2F 0b01:4E 4F 0b10:User define SWCBALR | SWCBAHR
#define Init_SWCBAHR          0x00  // I/O Port High Byte
#define Init_SWCBALR          0x4E  // I/O Port Low  Byte
#define Init_I2EC             0x43  // Enable I2EC R/W and log 80 port
#define INIT_H2RAM_BASS_ADDRESS     0x00  // Share memory EC decode Range setting(0x00: FEXX_X000 or Default 0x01:FFXX_X000)
#define PLATFORM_CHIP         AMD   // INTEL or AMD platform
#define PLATFORM_DGPU         AMD    // NVidia or AMD platform
#define EC_CHIP               IT8996 // Support IT8586/IT8986
#define Auto_Detect_ITE_chip  FALSE
#define SUPPORT_DSx           TRUE  // Support Deep Sx

#define PMIC_SUPPORT          TRUE
#define Support_EnhancedPcBeep TRUE//FALSE
#if (PLATFORM_CHIP == AMD)
#define AMDPWRBTNWK           100
#define AMDPWRBTNPULSE        5
#endif // PLATFORM_CHIP


#define SUPPORT_CPUMAXPSTATE  TRUE  // Support CPU Max state obtained by BIOS
#if SUPPORT_CPUMAXPSTATE
#define CPUMAXPSTATE          THROTTLING_HW
#define GPUMAXPSTATE          5
#else
#define CPUMAXPSTATE          0x0F
#endif
//-----------------------------------------------------------------------------
// OEM define.
//-----------------------------------------------------------------------------
// VGA feature
#define NV_GC6                FALSE // Set TRUE to support nV VGA GC6 feature

// Platform feature
#define SW_ISCT               TRUE  // Support ISCT function. (AOAC/IOIC)
#define Support_IFFS          TRUE  // Support IFFS function. (INTEL RapidStart)

// EC feature
#define Support_EC_MainPwr    FALSE    //EC mian power control by power design
#define Support_EC_GS_ON      FALSE   // Support G-sensor.
#define SW_ChkTimeScale       TRUE   // Support Timer.

#define Support_WebCam        FALSE  // Support WebCam enable/disable by EC.
#define ACPI_Smbus            TRUE   // Support ACPI command
#define RTS5400_Smbus                       TRUE
#define EC_Brightness_Ctrl    FALSE  // Support EC brightness control. (?)
#define Support_EC_LANWake    TRUE   // Support Lan Wake by EC control.
#define Support_G3ToS5_LANWake TRUE  // Support Lan Wake by EC control.

#define Support_EC_KBFN       TRUE   // Support Edge KB FN GPIO Pin.
#define Support_SMTADP        TRUE  // Support Smart Adaptor

#define Support_HUAshutdown	  FALSE  // shutdown function for hang on power sequence
#define Support_WLAN_WAKE	  FALSE  // Support WLAN wake

#define chPWSW1WDT			  TRUE	 // power button function for BIOS flashing protect.
#define CHARGER_IC            BQ25700//ChargerIC support BQ24780S : BQ25700
#define BatLowVot_Rule        LowVot_3S4S //Battery low votlage shutdown protect rule
// 8996 +
#define EFLASH256KB			  0x03
#define EFLASH192KB			  0x02
#define EFLASH128KB			  0x01
#define EFLASHSIZE		  	  EFLASH256KB	// define the eflash size which is 256KB or 192KB or 128KB
// 8996 -

//-----------------------------------------------------------------------------
// Thermal define.
//-----------------------------------------------------------------------------
#define FAN_TABLE_Already     TRUE    // FAN Table already. FALSE = fan full speed.
#define Support_FANLESSS      FALSE  // system with fanless
#if (PLATFORM_CHIP == INTEL)
#define Support_CPUCOMMFAIL_shutdown      FALSE  // for Intel platform, if set "false" communication fail get temperature from BIOS
#endif

#define Support_STDNProct     TRUE
#define Support_MAXPWR        TRUE

#define Thermal_30shutdown    FALSE       //False: Disable 30s shutdown

// Battery
#define Support_TWO_BATT      FALSE
#define S3RSOCPercentage      0x05    // 5%
#define BatteryOTPSD          70
#define BatteryOTPHi          60      // 60 degree
#define BatteryOTPLow         55      // 55 degree
#define FastChgTimeOut        720     // 12 * 60 = 720min
#define Thinkpad_Battery_HW_Auth  TRUE  // Battery authencation by EC HW(CHIP).
#define Lenovo_Authentication TRUE
#define BATS3WAKEcondition    30        //default 0 : battery under 5% wake,  1 => Remain cap 10mW 2 = >Remain cap 20mW  ......
#define SupportCHKtrippoint   FALSE  


#define OCPCapacity15           50
#define OCPCapacity             60    
#define OCPCapacityRelease      OCPCapacity * 0.9
#define OCPCapacityRelease15    OCPCapacity15 * 0.9

#define OCPCurrentShutDown      7200
#define OCPCurrentShutDown15    5200    


//[CC20131118][Project][Start] Add shutdown function for hang on power sequence.
#define PWRFAILCnt     0x08    //500ms * 8 = 4 Seconds
//[CC20131118][Project][End] Add shutdown function for hang on power sequence.


//-----------------------------------------------------------------------------
// Support vPro.
//-----------------------------------------------------------------------------
#define Support_vPro    FALSE  

/*===========================================================================
 * TYPE-C          support chip CCG2 CCG3
 *===========================================================================*/
#define Support_TYPE_C    RTS5455

/*===========================================================================
 * UCSI          support UCSI function
 *===========================================================================*/
#define Support_UCSI    TRUE
#define DEBUG_UCSI_RTK        FALSE    //UCSI UART2 Debug
#define DEBUG_UCSI_CMD        FALSE    //UCSI UART2 Debug
#define DEBUG_UCSI        FALSE    //UCSI UART2 Debug

/*===========================================================================
 * LED control
 *===========================================================================*/
#define Support_MuteLED              TRUE
#define Support_FnLKLED              FALSE
#define Support_Microphone_Mute_LED  FALSE
#define Support_PWRBTN_LED           TRUE
#define Support_BATTCHGLED           TRUE
#define Support_BATTCHGLED_BLINK     FALSE  //ROSC 0% ~ 20% LED Blink, this is optional function. need check with SW PM
#define Support_CAPSLOCKLED          TRUE

#define Support_NUMLOCKLED           FALSE
#define Support_KBD_BL_LED           TRUE
#define Support_KBD_BL_DETECT        TRUE

/*===========================================================================
 * temp remove MFG tool
 *===========================================================================*/
#define Support_COMPAL_MFG_TOOL        FALSE

/*===========================================================================
 * GATEA20 support
 *===========================================================================*/
#define Support_GATEA20             FALSE

/*===========================================================================
 *WLAN BT ON/OFF control by EC
 *===========================================================================*/
#define Support_WLAN_ON_OFF_control           FALSE
#define Support_BT_ON_OFF_control             FALSE

/*===========================================================================
 * RFID support
 *===========================================================================*/
#define Support_RFID                  TRUE

/*===========================================================================
 * AOU support
 *===========================================================================*/
#define Support_AOU5_V1_2                  TRUE      //1: support v1.2      0: support v1.0

/*===========================================================================
 * Gsensor support
 *===========================================================================*/
#define Support_LenovoAPS           TRUE && !EN_PwrSeqTest  // Support Lenovo APS
#define Support_GSNS_I2C            TRUE      //1: Gsensor => I2C       0: Gsensor => ADC
#define Support_GSNS_VS_PWR         TRUE     //G-sensor power source is Vs, need delay after Vs turn on.
#define Support_BMA255           	TRUE
/*===========================================================================
 * SMBUS => SMCLK0/1/2 and SMDAT0/1/2 timing
 *===========================================================================*/
#define SMB4P7USL_REG    0x16 
#define SMB4P0USH_REG    0x26        
#define SMB300NS_REG     0x03         
#define SMB250NS_REG     0x14         
#define SMB25MS_REG      0x19          
#define SMB45P3USL_REG   0xB7       
#define SMBS4P3USH_REG   0x01       
#define SMB4P7A4P0H_REG  0x00      

/*===========================================================================
 * 
Read Board ID by ADC
 *===========================================================================*/
#define Support_ADC_BoardID    TRUE 


/*===========================================================================
 * Tamper detection      
 *===========================================================================*/
#define Support_Tamper_function  Tamper_ADC //Tamper_ADC    // FALSE / Tamper_ADC / Tamper_GPIO
#define Support_Chk_mSATA	     TRUE   // Support Check mSATA
#define Support_Chk_BayHDD  	 TRUE   // Support Check BayHDD

/*===========================================================================
// Define SMBus device address
 *===========================================================================*/
#define EC_I2C_ADDR      0x02
#define SmBat_Addr       0x16    // smart battery address
#define Charger_Addr     0x12    // charger address
#define TmlIC_Addr       0x98    // Thermal IC SMBus address.
#define NV_VGA_Addr      0x9E    // nVIDIA SMBus address.
#define Intel_PCH_Addr   0x92    // Intel PCH SMbus address.
#define AMD_SBTSI_Addr   0x98    // AMD SMBus address.
#define AMD_VGA_Addr     0x82    // AMD GPU SMBus address. 
#define DGPU_Addr        AMD_VGA_Addr
#define TypeC_Addr       RTS5455_I2C_ADDR    // Type-C CCG2 I2C address
#define chSMB_TYPEC		SMbusChA

#define SMB_Channel_AUTO_detect FALSE
#define SMB_AUTO_Channel {SmBat_Addr    , SMbusChB}, \            
                         {Charger_Addr  , SMbusChB}, \
                         {TmlIC_Addr    , SMbusCh4}, \
                         {Intel_PCH_Addr, SMbusCh4}, \
                         {AMD_SBTSI_Addr, SMbusCh4}, \
                         {DGPU_Addr     , SMbusCh4}, \
                         {TypeC_Addr    , SMbusChA}, \
                         {RTS5455_DE_ASSERT_I2C_ADDR,SMbusChA}, \
                         {LIS3DH_ADDRESS, SMbusCh4}, \        
                         {LIS3DSH_ADDRESS,SMbusCh4}, \
                         {KX023_ADDRESS , SMbusCh4}

/*===========================================================================
 * TP/CP control 
 *===========================================================================*/
#define Support_TP_RESET_control             TRUE
#define Support_BYPASS_PAD_control           TRUE 
#define Support_CP_RESET_control             TRUE 

/*===========================================================================
 * Max power control setting
 *===========================================================================*/
#define LowerA45W_HIGH                              45//45W
#define LowerA45W_LOW                               15//15W
#define BatteryMaxWatt_14Inch                       60//60W
#define BatteryMaxWatt_15Inch                       50//50W

#define LowerA45W_CP                               u4ACAdaptorCap
#define LowerA45W_CHARGER_TURBO_POINT              (u4ACAdaptorCap * 1.07) // CP * 1.07
#define LowerA45W_CPU_TURBO_BOOST_OFF_POINT        70
#define LowerA45W_CPU_TURBO_BOOST_ON_POINT         65
#define LowerA45W_CPU_THROTTLING_ON_POINT          (u4ACAdaptorCap+( IS14Inch == 1 ? BatteryMaxWatt_14Inch : BatteryMaxWatt_15Inch ))
#define LowerA45W_CPU_THROTTLING_OFF_POINT         (u4ACAdaptorCap+(( IS14Inch == 1 ? BatteryMaxWatt_14Inch : BatteryMaxWatt_15Inch ) * 0.9))
#define LowerA45W_GPU_THROTTLING_ON_POINT          (u4ACAdaptorCap+(( IS14Inch == 1 ? BatteryMaxWatt_14Inch : BatteryMaxWatt_15Inch ) * 0.9))
#define LowerA45W_GPU_THROTTLING_OFF_POINT         (u4ACAdaptorCap+(( IS14Inch == 1 ? BatteryMaxWatt_14Inch : BatteryMaxWatt_15Inch ) * 0.85))

#define LowerA45W_CPU_THROTTLING_ON_POINT_L        ((u4ACAdaptorCap-3) >= 1 ? (u4ACAdaptorCap-3):1)
#define LowerA45W_CPU_THROTTLING_OFF_POINT_L       ((u4ACAdaptorCap-8) >= 1 ? (u4ACAdaptorCap-8):1)
#define LowerA45W_GPU_THROTTLING_ON_POINT_L        ((u4ACAdaptorCap-8) >= 1 ? (u4ACAdaptorCap-8):1)
#define LowerA45W_GPU_THROTTLING_OFF_POINT_L       ((u4ACAdaptorCap-13) >= 1 ? (u4ACAdaptorCap-13):1)

#define LowerA45W_AC_CPU_TURBO_BOOST_OFF_POINT     45
#define LowerA45W_AC_CPU_TURBO_BOOST_ON_POINT      40
#define LowerA45W_AC_CPU_THROTTLING_ON_POINT       u4ACAdaptorCap
#define LowerA45W_AC_CPU_THROTTLING_OFF_POINT      (u4ACAdaptorCap * 0.9)
#define LowerA45W_AC_GPU_THROTTLING_ON_POINT       (u4ACAdaptorCap * 0.9)
#define LowerA45W_AC_GPU_THROTTLING_OFF_POINT      (u4ACAdaptorCap * 0.85)

#define LowerA45W_ACOFF                            (u4ACAdaptorCap * 1.2)

#define A45W_CP                               41
#define A45W_CHARGER_TURBO_POINT              43.87 // CP * 1.07
#define A45W_CPU_TURBO_BOOST_OFF_POINT        70
#define A45W_CPU_TURBO_BOOST_ON_POINT         65
#define A45W_CPU_THROTTLING_ON_POINT          70
#define A45W_CPU_THROTTLING_OFF_POINT         65
#define A45W_GPU_THROTTLING_ON_POINT          65
#define A45W_GPU_THROTTLING_OFF_POINT         60

#define A45W_CPU_THROTTLING_ON_POINT_L        38
#define A45W_CPU_THROTTLING_OFF_POINT_L       33
#define A45W_GPU_THROTTLING_ON_POINT_L        33
#define A45W_GPU_THROTTLING_OFF_POINT_L       28

#define A45W_AC_CPU_TURBO_BOOST_OFF_POINT     45
#define A45W_AC_CPU_TURBO_BOOST_ON_POINT      40
#define A45W_AC_CPU_THROTTLING_ON_POINT       45
#define A45W_AC_CPU_THROTTLING_OFF_POINT      40
#define A45W_AC_GPU_THROTTLING_ON_POINT       40
#define A45W_AC_GPU_THROTTLING_OFF_POINT      35

#define A45W_ACOFF                            54

#define A60W_CP                               55
#define A60W_CHARGER_TURBO_POINT              58.85 //CP * 1.07
#define A60W_CPU_TURBO_BOOST_OFF_POINT        85
#define A60W_CPU_TURBO_BOOST_ON_POINT         80
#define A60W_CPU_THROTTLING_ON_POINT          85
#define A60W_CPU_THROTTLING_OFF_POINT         80
#define A60W_GPU_THROTTLING_ON_POINT          80
#define A60W_GPU_THROTTLING_OFF_POINT         75

#define A60W_CPU_THROTTLING_ON_POINT_L        52
#define A60W_CPU_THROTTLING_OFF_POINT_L       47
#define A60W_GPU_THROTTLING_ON_POINT_L        47
#define A60W_GPU_THROTTLING_OFF_POINT_L       42

#define A60W_AC_CPU_TURBO_BOOST_OFF_POINT     60
#define A60W_AC_CPU_TURBO_BOOST_ON_POINT      55
#define A60W_AC_CPU_THROTTLING_ON_POINT       60
#define A60W_AC_CPU_THROTTLING_OFF_POINT      55
#define A60W_AC_GPU_THROTTLING_ON_POINT       55
#define A60W_AC_GPU_THROTTLING_OFF_POINT      50

#define A60W_ACOFF                            70

#define A65W_CP                               60
#define A65W_CHARGER_TURBO_POINT              64.2 // CP * 1.07
#define A65W_CPU_TURBO_BOOST_OFF_POINT        90
#define A65W_CPU_TURBO_BOOST_ON_POINT         85
#define A65W_CPU_THROTTLING_ON_POINT          90
#define A65W_CPU_THROTTLING_OFF_POINT         85
#define A65W_GPU_THROTTLING_ON_POINT          85
#define A65W_GPU_THROTTLING_OFF_POINT         80

#define A65W_CPU_THROTTLING_ON_POINT_L        57
#define A65W_CPU_THROTTLING_OFF_POINT_L       52
#define A65W_GPU_THROTTLING_ON_POINT_L        52
#define A65W_GPU_THROTTLING_OFF_POINT_L       47

#define A65W_AC_CPU_TURBO_BOOST_OFF_POINT     65
#define A65W_AC_CPU_TURBO_BOOST_ON_POINT      60
#define A65W_AC_CPU_THROTTLING_ON_POINT       65
#define A65W_AC_CPU_THROTTLING_OFF_POINT      60
#define A65W_AC_GPU_THROTTLING_ON_POINT       60
#define A65W_AC_GPU_THROTTLING_OFF_POINT      55

#define A65W_ACOFF                            78

#define A90W_CP                               75
#define A90W_CHARGER_TURBO_POINT              80.25 // CP * 1.07
#define A90W_CPU_TURBO_BOOST_OFF_POINT       105
#define A90W_CPU_TURBO_BOOST_ON_POINT        100
#define A90W_CPU_THROTTLING_ON_POINT         105
#define A90W_CPU_THROTTLING_OFF_POINT        100
#define A90W_GPU_THROTTLING_ON_POINT         100
#define A90W_GPU_THROTTLING_OFF_POINT         95

#define A90W_CPU_THROTTLING_ON_POINT_14_L     72
#define A90W_CPU_THROTTLING_OFF_POINT_14_L    67
#define A90W_GPU_THROTTLING_ON_POINT_14_L     67
#define A90W_GPU_THROTTLING_OFF_POINT_14_L    62

#define A90W_CPU_THROTTLING_ON_POINT_15_L     60
#define A90W_CPU_THROTTLING_OFF_POINT_15_L    55
#define A90W_GPU_THROTTLING_ON_POINT_15_L     55
#define A90W_GPU_THROTTLING_OFF_POINT_15_L    50

#define A90W_AC_CPU_TURBO_BOOST_OFF_POINT     90
#define A90W_AC_CPU_TURBO_BOOST_ON_POINT      85
#define A90W_AC_CPU_THROTTLING_ON_POINT       90
#define A90W_AC_CPU_THROTTLING_OFF_POINT      85
#define A90W_AC_GPU_THROTTLING_ON_POINT       85
#define A90W_AC_GPU_THROTTLING_OFF_POINT      80

#define A90W_ACOFF                           109

#if Support_TYPE_C
#define A45W_System_Limited_support_TypeC_Adapter		45
#define A65W_System_Limited_support_TypeC_Adapter		65
#define A90W_System_Limited_support_TypeC_Adapter		90

#define A45W_System_Notsupport_TypeC_Adapter		    36
#define A65W_System_Notsupport_TypeC_Adapter		    45
#define A90W_System_Notsupport_TypeC_Adapter		    60

#define A45W_System_Unknow_TypeC_Adapter		         6
#define A65W_System_Unknow_TypeC_Adapter		        15
#define A90W_System_Unknow_TypeC_Adapter		        29
#endif //Support_TYPE_C

#define A45W_System_Limited_support_Adapter		36 //AdapterID_36W
#define A65W_System_Limited_support_Adapter		45//AdapterID_45W
#define A90W_System_Limited_support_Adapter		65//AdapterID_65W
#define Limited_support_Adapter               AdapterID_45W

#define A45W_System_Not_support_Adapter		36
#define A65W_System_Not_support_Adapter		45
#define A90W_System_Not_support_Adapter		65
#define ADPI_times                            3
#define ACcutoff_Ts                           9 // cut off times

#define Hybrid_debug          FALSE
#endif
// 8996 +
//////////////////////////////////////////////////////////////////
//// 1.8V Voltage & 2mA Currnet Selection
/////////////////////////////////////////////////////////////////
//#define __DEF_VOLTAGE_SELECTION_GPB5_1P8V__  
//#define __DEF_VOLTAGE_SELECTION_GPB6_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPC1_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPC2_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPC7_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPD0_1P8V__    
//#define __DEF_VOLTAGE_SELECTION_GPD1_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPD2_1P8V__    
//#define __DEF_VOLTAGE_SELECTION_GPD3_1P8V__  
//#define __DEF_VOLTAGE_SELECTION_GPD4_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPE0_1P8V__   
//#define __DEF_VOLTAGE_SELECTION_GPE6_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPE7_1P8V__   
//#define __DEF_VOLTAGE_SELECTION_GPF2_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPF3_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPF4_1P8V__    
//#define __DEF_VOLTAGE_SELECTION_GPF5_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPF6_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPF7_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPG1_1P8V__   
//#define __DEF_VOLTAGE_SELECTION_GPG6_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPH0_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPH1_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPH2_1P8V__
//#define __DEF_VOLTAGE_SELECTION_GPH0_1P05V__
//#define __DEF_VOLTAGE_SELECTION_GPE7_1P05V__
//#define __DEF_VOLTAGE_SELECTION_GPE6_1P05V__
//#define __DEF_VOLTAGE_SELECTION_GPE0_1P05V__   
//#define __DEF_VOLTAGE_SELECTION_GPD4_1P05V__
//#define __DEF_VOLTAGE_SELECTION_GPD1_1P05V__
//#define __DEF_VOLTAGE_SELECTION_GPD0_1P05V__  
//#define __DEF_VOLTAGE_SELECTION_GPB5_1P05V__
//#define __DEF_CURRENT_SELECTION_GPA7_2MA__
//#define __DEF_CURRENT_SELECTION_GPA6_2MA__
//#define __DEF_CURRENT_SELECTION_GPA5_2MA__
//#define __DEF_CURRENT_SELECTION_GPA4_2MA__
//#define __DEF_CURRENT_SELECTION_GPA3_2MA__
//#define __DEF_CURRENT_SELECTION_GPA2_2MA__
//#define __DEF_CURRENT_SELECTION_GPA1_2MA__
//#define __DEF_CURRENT_SELECTION_GPA0_2MA__
//#define __DEF_CURRENT_SELECTION_GPF0_2MA__
//#define __DEF_CURRENT_SELECTION_GPE3_2MA__
//#define __DEF_CURRENT_SELECTION_GPE2_2MA__
//#define __DEF_CURRENT_SELECTION_GPE1_2MA__
//#define __DEF_CURRENT_SELECTION_GPD5_2MA__
//#define __DEF_CURRENT_SELECTION_GPC5_2MA__
//#define __DEF_CURRENT_SELECTION_GPC3_2MA__
//#define __DEF_CURRENT_SELECTION_GPB2_2MA__
//#define __DEF_CURRENT_SELECTION_GPH6_2MA__
//#define __DEF_CURRENT_SELECTION_GPH5_2MA__
//#define __DEF_CURRENT_SELECTION_GPH4_2MA__
//#define __DEF_CURRENT_SELECTION_GPH3_2MA__
//#define __DEF_CURRENT_SELECTION_GPF1_2MA__
// 8996 -

