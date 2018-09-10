/*-----------------------------------------------------------------------------
 * TITLE: OEM_FAN.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_FAN_H
#define OEM_FAN_H

//=============================================================================
//   structure define
//=============================================================================
typedef struct _FanControlstruct
{
  BYTE FanOn;
  BYTE FanOff;
  BYTE RPMValue;
  BYTE PWMDuty;
} FanControlstruct;

typedef struct _CPUTHRstruct
{
  BYTE Thr_Off;
  BYTE Thr_On;          // CAC 2013.03.18
  BYTE Turbo_Rem;
  BYTE Turbo_Off;
  BYTE PMAX_D2_off;
  BYTE PMAX_D2_on;
  BYTE Thr_Shut;
} CPUTHRstruct;

typedef struct _FanTableList {
  FanControlstruct *TsTbl;  //A485D00092
  BYTE 		TsTblCount;
  FanControlstruct *WinTbl;
  BYTE             WinTblCount;
  FanControlstruct *VgaTbl;
  BYTE             WinVgaTblCount;
  FanControlstruct *DosTbl;
  BYTE             DosTblCount;
  CPUTHRstruct *CpuThrTbl;
  CPUTHRstruct *VgaThrTbl;
  CPUTHRstruct *CpuThrDosTbl;
} FanTableList;

//========Temperature==========================================================
#define TmlIC_Temp       0x00    // Thermal IC Temperature Data register.
#define VRAM_Temp        0x00    // Thermal IC Temperature Data register.
#define DRAM_Temp        0x01    // Thermal IC EXT 1 Temperature Data register.
#define TmlIC_Rate       0x04    // Thermal IC Conversion Rate register.
#define CPUcore_Temp     0x23    // Thermal IC EXT 2 Temperature Data register.

extern void ThmIC_Temperature(void);
extern void Oem_Thermal_Control(void);
extern void Clr_Thermal_Tab(void);
void Tml_SMLink(void);
void FanTab_ToRAM(BYTE BStep);
void THRTab_ToRAM(BYTE BStep);
void VFanTab_ToRAM(BYTE BStep);
void VTHRTab_ToRAM(BYTE BStep);
//=============================================================================
//==========F-A-N==============================================================
#define FAN_PWM_Max      CTR2  // PWM Maximum CTR2.
#define FAN_Boot_On      0x03  // 3 Sec.

// Temperature Data Index
#define TEMP_TYPE_CPU    0
#define TEMP_TYPE_VGA    1
#define TEMP_TYPE_EXT    2
#define TEMP_Compare     3

//#define FAN_VEN_BOUNDARY_1     0x0772   // ADC boundary
#define FAN_VEN_BOUNDARY_1     0x0300   // ADC boundary//3.3v
//#define FAN_VEN_BOUNDARY_2     0x0030   // ADC boundary
#define FAN_VEN_BOUNDARY_2     0x0200   // ADC boundary
//
// ADC channel(10bit) for FAN_ID
//
// ========   --> 0x03FF (~3.3v)
// |
// | FAN_VENDOR_1 > FAN_VEN_BOUNDARY_1
// |
// ========   --> 0x0364 (~2.8v) FAN_VEN_BOUNDARY_1
// |
// | FAN_VENDOR_2 > FAN_VEN_BOUNDARY_2
// |
// ========   --> 0x0030 FAN_VEN_BOUNDARY_2
// |
// | FAN_VENDOR_3 < FAN_VEN_BOUNDARY_2
// |
// ========   --> 0x0

enum {
  Delta = 0,  // 0 -- FAN_VENDOR_1
  Sunon,      // 1 -- FAN_VENDOR_2
  AVC         // 2 -- FAN_VENDOR_3
};

extern void Fan_control_update(void);
extern void Oem_Fan_Speed(void);  // Read Fan Speed RPM.
extern void Oem_Fan_Control(void);  // Main FAN control.
extern void SLI_FAN_Control(void);  // External SLI fan control.
extern void Chk_FAN_RPM_Control(void);  // Check FAN is AP control.
void DACFanCtrlCenter(void);
void DetectFanId (void);
void FanCtrlWin(void);
void FanCtrlDos(void);
void ThrottlingWin(void);
void ThrottlingDos(void);
#if Support_COMPAL_MFG_TOOL
void FANTabletoSTPM(void);
#endif

//=============================================================================
//=======Throttling============================================================
#define Thro_Shut_Cnt       3      // 3 times.

extern void OEM_Throttling_Ctrl(void);  // Temperature Protect Center.
void Thro_Count_Shut(XBYTE *BShut_Cnt, BYTE TempType);  // Throttling: Shutdown count used.
//=============================================================================

extern void ThrottlingControl(void);
#endif

//=============================================================================
//=======Thermal sensor Policy=================================================
#if (PLATFORM_CHIP == INTEL)
#define  Ext_Sen1_On   0x41
#define  Ext_Sen1_Off  0x3C
#define  Ext_Sen3_On   0x4D
#define  Ext_Sen3_Off  0x3C
#endif 

#if (PLATFORM_CHIP == AMD)
#define  Ext_Sen1_On   0x78
#define  Ext_Sen1_Off  0x00
#define  Ext_Sen3_On   0x50
#define  Ext_Sen3_Off  0x37
#define  Ext_Sen2_On   74
#define  Ext_Sen2_Off  62


#endif 

//=============================================================================

