/*-----------------------------------------------------------------------------
 * TITLE: OEM_BATTERY.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef BATTERY_H
#define BATTERY_H

/*-----------------------------------------------------------------------------
 * Data Parameter definition
 *---------------------------------------------------------------------------*/
#define SMbusTOC    200     // SMbus re-try counter

#define SMbusChA    0x00    // SMbus channel A index,SMbusCh1.
#define SMbusChB    0x01    // SMbus channel B index,SMbusCh2.
#define SMbusChC    0x02    // SMbus channel C index,SMbusCh3.
#define SMbusChD    0x03    // SMbus channel D index,SMbusCh4.


//-----------------------------------------------------------------
// Define smart battery command
//-------------------------------------------- --------------------
#define C_access             0x00    // manufacture access
#define C_RCalarm            0x01   // remaining capacity alarm
#define C_RTalarm            0x02     // remaining time alarm
#define C_mode               0x03    // battery mode
#define C_AtRate             0x04     // AtRate
#define C_AtRateFull         0x05    // AtRate time to full
#define C_AtRateEmpty        0x06    // AtRate time to empty
#define C_AtRateOK           0x07     // AtRate OK
#define C_temp               0x08     // temperature
#define C_volt               0x09     // voltage
#define C_current            0x0A     // current
#define C_AVcurrent          0x0B     // average current
#define C_MaxError           0x0C    // maximum error
#define C_RSOC               0x0D    // relative state of charge
#define C_ASOC               0x0E     // absolute state of charge
#define C_RMcap              0x0F     // remaining capacity
#define C_FCcap              0x10     // full charge capacity
#define C_RunTimeEmpty       0x11   // run time to empty
#define C_AVTimeEmpty        0x12    // average time to empty
#define C_AVTimeFull         0x13     // average time to full
#define C_ChargingI          0x14    // charging current
#define C_ChargingV          0x15     // charging voltage
#define C_BatStatus          0x16     // battery status
#define C_CycleCount         0x17     // cycle count
#define C_DCap               0x18     // design capacity
#define C_DVolt              0x19     // design voltage
#define C_Spec               0x1A   // spec information
#define C_Date               0x1B     // manufacture date
#define C_SerialNo           0x1C     // serial number
#define C_Mname              0x20   // manufacturer name
#define C_Dname              0x21    // device name
#define C_Dchem              0x22    // device chemistry
#define C_Mdata              0x23   // manufacture data
#define C_SmartCharge        0x26   // Smart Charge(Quick Charge)
#define C_OptMfgFunc5        0x2F   // OptionalMfgFunction5
#define C_BattUsageCond      0x30   // Battery Usage Condition
#define C_SnapShotData       0x31   // Snap shot data at permanent failure
#define C_BattLogData1       0x32   // Battery log data #1
#define C_BattLogData2       0x33   // Battery log data #2
#define C_D_FET              0x34   // Ship Mode and Additional Over Discharge Mode.

#define C_FW_Status          0x35    // FW Update Status.
#define C_FW_Data            0x36    // FW Update Data.
#define C_BAT_Info           0x37    // Get Battery Information.

#define C_WarrantyPeriod     0x38   // Warranty Period
#define C_BattMode2          0x39   // Battery Mode 2
//#define Reserved           0x3A   // Reserved
#define C_FETTemp            0x3B   // FET Temperature
#define C_BattLogData2       0x33   // Battery log data #2

//[Project][CC20130425][Start]Add first used date feature.
#define FirstUsedDate        0x3F   // FirstUsedDate
//[Project][CC20130425][End]Add first used date feature.

//#define C_Cell1              0x3F   // manufacture data
#define C_Cell2              0x3E   // manufacture data  //OptionalMfgFunction2
#define C_LVMfgFun2          0x3E  // LV Smart Battery
#define C_Cell3              0x3D   // manufacture data
#if Lenovo_Authentication
#define AuthenticationCmd    0x3C
#define delayReadCnt    15
#define LeftRotateBYTE(x,bit) (((XBYTE)(x) << (bit)) | ((XBYTE)(x) >> (8-(bit))))
#else
#define C_Cell4              0x3C   // manufacture data
#endif

#define  NONE_BAT        0x00
#define  PWR_CHANGE      0x01
#define  CHARGE_BAT      0x02
#define  DISCHARGE_BAT   0x03
#define  PRE_CHARGE      0x04
#define  BAT_ERR         0x05

//-----------------------------------------------------------------
// Define charger command
//-------------------------------------------- --------------------
#define C_ChargerMode     0x12    // Set charger mode M+
#define C_ChargeCurrent   0x14    // Set charge curretn
#define C_ChargeVoltage   0x15    // Set charge voltage
#if (CHARGER_IC == BQ25700)
#define C_ChargeOption1   0x30
#define C_ChargeOption2   0x31
#define C_ChargeOption3   0x32
#define C_ProchotOption0  0x33
#define C_ProchotOption1  0x34
#define C_InputVoltage    0x3D
#endif 
#if (CHARGER_IC == BQ24780S)
#define C_ChargeOption3   0x37             
#define C_DisChargeCurr   0x39
#define C_ChargerMode3B   0x3B
#endif 
#define C_InputCurrent    0x3F    // Set input current

#define C_MFID            0xFE  // Get Manufacturer ID
#define C_DeviceID        0xFF  // Get device ID
#define ChargeDeviceID    0x08
#define ChargeMFID        0x4D


typedef struct RSmbusBStruct
{
    BYTE    cmd;
    XBYTE   *smbdataA;
  BYTE    block_limit;
} sRSmbusBStruct;
//-----------------------------------------------------------------
// Charge and discharge charger rules
//-------------------------------------------- --------------------
#define ChangeChgBattery    0x82
#define ChangeDischgBattery 0x20
#define ABatteryLow         0x05
//#define BBatteryLow         0x05
#define ACriticalLow        0x02
//#define BCriticalLow        0x02
#define AOverVoltage        13400
//#define BOverVoltage        13400

#if (BatLowVot_Rule == LowVot_LGNotLG)
#define LG_3SLowVot         8900
#define Sanyo_3SLowVot      8900
#define BATLowVotCut_Time   BATLGNotLG_T 	
#endif
#if (BatLowVot_Rule == LowVot_3S4S)
#define BAT3S4SVot_Divide   14000
#define BAT3SLowVot         8900
#define BAT4SLowVot         11900
#define BATLowVotCut_Time   BAT3S4S_T	
#endif

#if (PLATFORM_CHIP == INTEL)
#define BATLGNotLG_T    4         //6*500msec=3 sec
#define BAT3S4S_T       4 
#endif

#if (PLATFORM_CHIP == AMD)
#define BATLGNotLG_T    2         //2*(1~2)sec=2~4 sec
#define BAT3S4S_T       2
#endif

#if (PLATFORM_CHIP == INTEL)
#define TWR_00    0x88 
#else
#define TWR_00    0x80 
#endif
//#if 0
//#define CHG_ABattery()  GPIOA.GPin5 = 1;GPIOA.GPin6 = 0;
//#define CHG_BBattery()  GPIOA.GPin5 = 0;GPIOA.GPin6 = 1;
//#define DISCHG_ABattery()  GPIOA.GPin7 = 1;
//#define DISCHG_BBattery()  GPIOA.GPin7 = 0;
//#endif
//#define CHG_ABattery()    DACDAT0=0xE7;   //Set charge current
//#define CHG_BBattery()    ;
//#define DISCHG_ABattery()  DACDAT0=0;     //off charge current
//#define DISCHG_BBattery()  ;

//extern bit bCheckChargeDeviceID(void);
//extern bit bCheckChargeMFID(void);
//extern bit bSetChargeCurrent(BYTE LowByte, BYTE HithByte);
//extern bit bSetChargeVoltage(BYTE LowByte, BYTE HithByte);
//extern bit bSetInputCurrent(BYTE LowByte, BYTE HithByte);
//extern bit bSetChargerMode(BYTE LowByte, BYTE HithByte);  // M+
extern void vPollingBatteryData(void);
extern void Charge_or_Discharge(void);
extern void  AC_DC_EVENT(void);
extern void  CheckAdapterInOut(void);
//extern void Get_All_PSID_Info(void);
//extern void bCheckBatteryIn(void);
//extern void vProcessDischarge(void);
//extern void vChkBatteyStatus(void);
//extern void ChkBatteryPercentChange(void);
//extern void vProcessCharge(void);
//extern bit bAOverVoltage(void);
//extern bit bBOverVoltage(void);
//extern bit bAOverTemperature(void);
//extern bit bBOverTemperature(void);
//extern void vCoypBatteryData(void);
//extern void vGetSelectorState(void);
extern void ChkBattery_Percl();

//-----------------------------------------------------------------
// PSID
//-------------------------------------------- --------------------
#define  PSID_FAIL    0
#define  PSID_GOOD    1
#define  ROM_SKIP    0xCC
//#define  ROM_READ    0x33
//#define  MEM_STATUS_READ  0xAA
#define  MEM_READ    0xF0

//----------------------------------------------------------------------------
// ROM Layout
//----------------------------------------------------------------------------
//#define PSID_DELL_HEADER      0x00     // 'D', 'E', 'L', 'L'
//#define PSID_REV_NUM          0x04     // 2 bytes ASCII
//#define PSID_CLASS_OF_PRODUCT 0x06     // 'AC090', 'PS130'
#define PSID_WATTS        0x08    // 3 bytes,
//#define PSID_VOLTAGE          0x0B     // 3 bytes, ACSII: '195' = 19.5V
//#define PSID_CURRENT          0x0E     // 3 bytes, ASCII: '045' = 4.5A
//#define PSID_PPID             0x11    // Label On AC Adapter
//#define PSID_PPID_MFGID    0x19  // Supplier MFG ID
//#define PSID_CHECKSUM         0x28  // 2 byte CRC = X^16 + X^15 + X^2 + 1
//----------------------------------------------------------------------------
// adapter types
//----------------------------------------------------------
//#define AC_TYPE_MAX           230
//#define AC_ADAPTER_MIN    65
#define AC_TYPE_UNKNOWN         0
#define AC_TYPE_45W             45
#define AC_TYPE_50W             50
#define AC_TYPE_60W             60
#define AC_TYPE_65W             65
#define AC_TYPE_70W             70
#define AC_TYPE_90W             90
#define AC_TYPE_120W            120
#define AC_TYPE_130W            130
#define AC_TYPE_150W            150
#define AC_TYPE_230W            230
#define AC_TYPE_240W            240
//#define ACAV_HW     bAdapter_In
#define BAT_HYBOOST_EN          10
#define BAT_HYBOOST_DIS         10
#define BAT_CPU_TURBO_EN        10
#define BAT_CPU_TURBO_DIS       10

#define BATHYBITOTPTHOEN        60
#define  BATHYBITOTPTHODIS      55
#define BATHYBITOTPSHUT         70

#define BuildInBat		IS14Inch
//
// Input Current Register (0x3F)
//
// BIT[6:0] : Not used. Value Ignored.
// BIT7 : 0 = Add 0mA of input current, 1 = Add 128mA of input current.
// BIT8 : 0 = Add 0mA of input current, 1 = Add 256mA of input current.
// BIT9 : 0 = Add 0mA of input current, 1 = Add 512mA of input current.
// BIT10 : 0 = Add 0mA of input current, 1 = Add 1024mA of input current.
// BIT11 : 0 = Add 0mA of input current, 1 = Add 2048mA of input current.
// BIT12 : 0 = Add 0mA of input current, 1 = Add 4096mA of input current.
// BIT[15:13] : Not used. 1 = invalid write.
//

//
// Charge Current Limit is referenced 'Adapter define' of ThinkPad China Dev Lab power max power control design spec_V1.1_20140328.xslx
//
// Ex: Assumed adapter output is 20V and 45W adapter's charge current limit is 36W, then 36W / 20V = 1.8A
//     which means BIT8+BIT9+BIT10 = 256 + 512 + 1024 = 1792mA should be set.
//
#if (CHARGER_IC == BQ25700)
// 45W adp: 41W charge current limit, input current should be 2.05A.
#define A45WINPUTCURRH    0x29 // 2050mA //AC only   //AC + DC S3 S4 S5 mode
#define A45WINPUTCURRL    0x00

// 60W adp:
#define A60WINPUTCURRH    0x37 // 2750mA //AC only   //AC + DC S3 S4 S5 mode
#define A60WINPUTCURRL    0x00  

// 65W adp: 55W charge current limit, input current should be 2.75A.
#define A65WINPUTCURRH    0x3C // 3000mA //AC only   //AC + DC S3 S4 S5 mode
#define A65WINPUTCURRL    0x00  

#if (PLATFORM_CHIP == INTEL)
// 90W adp: 80W charge current limit, input current should be 4A.
#define A90WINPUTCURRH_A  0x4B // 3750mA //AC only   //AC + DC S3 S4 S5 mode
#define A90WINPUTCURRL    0x00  

#define A90WINPUTCURRH_AD 0x55 // 4250mA //AC + DC S0 or DOS mode
#endif




#endif   //BQ25700

#if (CHARGER_IC == BQ24780S)
// 45W adp: 41W charge current limit, input current should be 2.05A.
#define A45WINPUTCURRH    0x08 // BIT11 = 2048mA
#define A45WINPUTCURRL    0x00

// 65W adp: 55W charge current limit, input current should be 2.75A.
#define A65WINPUTCURRH    0x0A // BIT11 + BIT9 = 2560mA
#define A65WINPUTCURRL    0x80 // BIT7 = 128mA

// 90W adp: 80W charge current limit, input current should be 4A.
#define A90WINPUTCURRH    0x0F // BIT8+9+10+11 = 3840mA
#define A90WINPUTCURRL    0x80 // BIT7 = 128mA
#endif   //BQ24780S

#define CHARGE_CURRENT_1A_H  0x04 // BIT10 = 1024mA
#define CHARGE_CURRENT_1A_L  0x00

#define CHARGE_CURRENT_2_2A_H  0x08 // BIT11 = 2048mA
#define CHARGE_CURRENT_2_2A_L  0x80 // BIT7 = 128mA

extern void OEM_PollingBatData_TASK(void);
extern void WriteSmartChgIC(void);
extern void ReadSmartChgIC(void);
extern void FirstGetBatData(void);
extern void Battery1Sec(void);
extern void Battery1Sec(void);  //(JSN)
extern void Chk_Shutdown_Volt(void);
extern void Chk_CHG_TimeOut(void);
extern void Chk_BatSMbusFailCount(void);
extern void Unlock_ShipMode(void);
extern void RST_ChgTimeOutCnt(void);
extern void Lock_ShipMode(void);
extern void Chk_Battery_Full(void);
extern void UpBatteryPState();
extern void DownBatteryPState();
extern void UpGPUPState();
extern void DownGPUPState();
extern void Battery100ms(void);
extern void ChkBattery_abnormal(void);
extern void LV_BAT_Authentication(void);
extern void OEMauthentication(void);
extern void RSMRST_shutdown(void);
extern void StopChgIC(void);
extern void UpdateEdgeNameSpace(void);
extern void ChargeIC_InitACDC(void);

#if Lenovo_Authentication
extern void LAuthProcess(void);
#endif

//[Project][CC20130425][Start]Add first used date feature.
extern void ChkFirstUsedDate(void);
//[Project][CC20130425][End]Add first used date feature.

extern void ACOffProcess(BYTE);
extern void BATLEARNProcess(BYTE);
extern void Update_ChargeIC(void);      // [20140304 ITE CM.Wu]
extern void CriticalLowBATWakeS3(void);

extern void IBMbatteryStore(void);
extern void TWR_BATClear(void);
extern void TWR_Initial(void);
extern void EDGEPMBATTProcess(void);
extern void TPG_EXTBATCmd(void);

extern void Battery_Initial(void);
extern void ChargerIC_Initial(void);
extern void CHK_Capacity_trip_point(void);
extern void Battery_RSOCLowNotifyOS(void);

#endif
