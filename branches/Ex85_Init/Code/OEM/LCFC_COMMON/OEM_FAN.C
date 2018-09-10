/*-----------------------------------------------------------------------------
 * TITLE: OEM_FAN.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
extern FanTableList code * data FanTblPtr;

/*****************************************************************************/
// Procedure: ThmIC_Temperature                TimeDiv: 500mSec
// Description: Read RAM temperature
// GPIO:
// Referrals:
/*****************************************************************************/
void ThmIC_Temperature(void)
{
  BYTE BsData,BTmlCmd;

  if( SystemNotS0 )
  { return; }

  switch( TmlICStep & 0x03 )
  {
    case 0:
      BTmlCmd = VRAM_Temp;
      break;
    case 1:
      BTmlCmd = DRAM_Temp;
      break;
    case 2:
      BTmlCmd = CPUcore_Temp;
      break;
    case 3:
#if (PLATFORM_CHIP == INTEL)
      if(IS_MASK_CLEAR(pProject3,pIsMeFlashing)){ //Enable ME	//To avoid the PCH communication fail during ME flashing.      	
		Tml_SMLink();
      }else{    //Disable ME
        TmlICStep = 0;
      }
#else
      TmlICStep = 0;
#endif // PLATFORM_CHIP
      return;
    default:
      TmlICStep = 0;
      return;
  }

  if(!bRWSMBus(SMbusCh3, SMbusRB, TmlIC_Addr, BTmlCmd, &BsData, 0))
  {
    TMErrCnt++;
    if( IS_MASK_CLEAR(ERR_THMSTS, b0ThmICError) )// Check bit0 is Error.
    {
      if ( TMErrCnt > 7 )    // 3 Sec.
      {
        SET_MASK(ERR_THMSTS, b0ThmICError);  // Set bit0 Thermal IC read temperature error.
      }
    }
    else
    {
#if Thermal_30shutdown    
      if ( TMErrCnt > 68 )  // 30 Sec.
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(THERMALCOMMFAIL_ID);  // 0x0B shutdown ID.
        TMErrCnt = 0;          // Clear error count.
        RSMRST_shutdown();
      }
#endif
    }
    ResetSMBus(SMbusCh3);
  }
  else
  {
      switch( TmlICStep & 0x03 )
      {
        case 0:
          nRamTemp = BsData;  // Save VRAM temperature.//cpu
          TMErrCnt = 00;    // Clear error count.
          CLEAR_MASK(ERR_THMSTS,b0ThmICError);  // Clear bit0.
          TmlICStep++;    // next step 1.
          break;
        case 1:
         nVramTemp = BsData;  // Save DRAM temperature.//charge
          TMErrCnt = 00;    // Clear error count.
          CLEAR_MASK(ERR_THMSTS,b0ThmICError);  // Clear bit0.
          TmlICStep++;    // next step 2.
          break;
        case 2:
          nCpuTempLocal = BsData;  // Save Cpu Local temperature.
          TMErrCnt = 00;      // Clear error count.
          CLEAR_MASK(ERR_THMSTS,b0ThmICError);  // Clear bit0.
          TmlICStep++;      // next step 3.
          //break;
        default:
          break;
      }
  }

}
//-----------------------------------------------------------------------------
// Service short time PWM output, base on 50ms.
//-----------------------------------------------------------------------------
#if (PLATFORM_CHIP == INTEL)
void Tml_SMLink(void)
{
  if( !bRSMBusBlock(SMbusCh4, SMbusRBK, Intel_PCH_Addr, 0x40, &SMB3_DATA))//, 0x14, TRUE))
  {
    PCHErrCnt++;
    if( IS_MASK_CLEAR(ERR_THMSTS, b4PCHThmError) )// Check bit4 is Error.
    {
      if ( PCHErrCnt > 7 )    // 3 Sec.
      {
        SET_MASK(ERR_THMSTS, b4PCHThmError);  // Set bit4 PCH Thermal read temperature error.
      }
    }
    else
    {
#if Thermal_30shutdown 
      if ( PCHErrCnt > 68 )  // 30 Sec.
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(THERMALCOMMFAIL_ID);  // 0x0B shutdown ID.
        PCHErrCnt = 0;          // Clear error count.
        RSMRST_shutdown();
      }
#endif
    }
    ResetSMBus(SMbusCh4);
  }
  else
  {
  	*(LWORD*)&MaxCPU_MCHTemp0 = *(LWORD*)&SMB3_DATA[0];
    PCH_Temp3 = MaxCPU_MCHTemp0;        // Save PCH temperature.
    CLEAR_MASK(ERR_THMSTS,b4PCHThmError);  // Clear bit4.
    PCHErrCnt = 0;              // Clear error count.
    TmlICStep = 0;              // Reset Step.
  }
}
#endif // PLATFORM_CHIP

// ----------------------------------------------------------------------------
// The function of PECI3.0 CPU temperature read
// ----------------------------------------------------------------------------
void SBTSI_ReadCPUTemp(void)
{
  BYTE cputemp;

  if(SystemNotS0) {
    return;
  }
  if(!bRWSMBus(SMbusCh4, SMbusRB, AMD_SBTSI_Addr, 0x01, &cputemp, 0))
  {
    PECI_Err_CNT++;  // increase read fail count
    if( IS_MASK_CLEAR(ERR_THMSTS, b3PECITempEr) ) // Check bit3 is Error.
    {
      if ( PECI_Err_CNT > 3 )    // 3 Sec.
      {
        SET_MASK(ERR_THMSTS, b3PECITempEr);  // Set bit3 CPU read temperature error.
      }
    }
    else
    {
      if ( PECI_Err_CNT > 34 )    // 30 Sec.
      {
#if !EN_PwrSeqTest
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(CPUCOMMFAIL_ID);  // 0x22 shutdown ID.
        PECI_Err_CNT = 0;      // Clear error count.
        RSMRST_shutdown();
#endif  // EN_PwrSeqTest
      }
    }
    ResetSMBus(SMbusCh4);
  }
  else
  {
    u4CpuThermal = cputemp;   // Get CPU temperature.
    PECI_Err_CNT = 0;   // Clear error count.
    CLEAR_MASK(ERR_THMSTS, b3PECITempEr); // Clear bit3 CPU read temperature error.
    if ( IS_MASK_CLEAR(Fan_Debug_Temp,b0CPU_Temp) )  // Debug CPU Temperature, Engineer myself control.
    {
    
     TEMP_Buff_7 = TEMP_Buff_6;
	 TEMP_Buff_6 = TEMP_Buff_5;
	 TEMP_Buff_5 = TEMP_Buff_4;
	  TEMP_Buff_4 = TEMP_Buff_2;
      TEMP_Buff_2 = TEMP_Buff_1;
      TEMP_Buff_1 = u4CpuThermal;
      TEMP_Buff_3 = (TEMP_Buff_1 + TEMP_Buff_2 + TEMP_Buff_4+ TEMP_Buff_5+ TEMP_Buff_6+ TEMP_Buff_7)/6;  // CPU average temperature.
    }
  }
}
/*****************************************************************************/
// Procedure: Oem_Thermal_Control                TimeDiv: 1 Sec
// Description: Read CPU temperature
// GPIO: GPIOF6(118)
// Referrals:
/*****************************************************************************/
void Oem_Thermal_Control(void)
{
  if(SystemIsS0)
  {
#if (PLATFORM_CHIP == INTEL)
//
// Intel PECI
//
    if(TJMAX == 0x00)
    {
      PECI_ReadCPUTJMAX();
      CPU_TYPE = CPU_TYPE & 0xFC; //
      if (TJMAX == 105)      //105
        CPU_TYPE |= 0x03;
      else if (TJMAX == 100)    //100
        CPU_TYPE |= 0x02;
      else if (TJMAX == 90)    //90
        CPU_TYPE |= 0x01;
    }
    else
    { PECI_ReadCPUTemp(); }
#else // PLATFORM_CHIP
//
// AMD SB-TSI
//
    if(TJMAX == 0x00) {
      //
      // TBD -- To determine TJMAX
      // Current, set default TJMAX = 100 for AMD Kaveri AAxE1 projects.
      //
      TJMAX = 100;
//       CPU_TYPE = CPU_TYPE & 0xFC;
//       if (TJMAX == 105)      //105
//         CPU_TYPE |= 0x03;
//       else if (TJMAX == 100)    //100
//         CPU_TYPE |= 0x02;
//       else if (TJMAX == 90)    //90
//         CPU_TYPE |= 0x01;
    } else {
      SBTSI_ReadCPUTemp();
    }
#endif // PLATFORM_CHIP

  }
}

/*****************************************************************************/
// Procedure: Show Real PWM duty,Target PWM duty (BCD),
//                 Real FAN RPM , Target FAN RPM (BCD)
/*****************************************************************************/
//unsigned char hex2bcd (unsigned char x)
//{
//   unsigned char y;
//    y = (x / 10) << 4;
//    y = y | (x % 10);
//    return (y);
//}

BYTE hex2bcd (BYTE x)
{
    BYTE y,z;
    y = x / 10 ;
  z = x - (y*10);
    x = (y << 4) | z;
    return (x);
}

void Show_realPWM(void)
{
  FAN_PWM_BCD = FAN_PWM >>1;              // duty = PWM/2
//  FAN_PWM_BCD = ((WORD)((FAN_PWM+1) *100))/CTR2;    // duty = PWM/185 *100
  FAN_PWM_BCD = hex2bcd(FAN_PWM_BCD);
  u4FanSpdH = hex2bcd(nAtmFanSpeed);
}

void Show_targetduty(void)
{
  Target_Duty_BCD = Target_Duty >>1;              // Target_Duty/2
//  Target_Duty_BCD = ((WORD)((Target_Duty+1) *100))/CTR2;    // ((Target_Duty+1)*100)/200
  Target_Duty_BCD = hex2bcd(Target_Duty_BCD);
  TargetRPM_BCD= hex2bcd(TargetRPM);
}

/*****************************************************************************/
// Procedure: Oem_Fan_Speed                    TimeDiv: 1 Sec
// Description: Read Fan RPM
// GPIO: GPIOD6(47)
// Referrals:
/*****************************************************************************/
void Oem_Fan_Speed(void)
{
  WORD Curr_Fan1Tachometer;
  if(SystemIsS0)
  {
    if ( (F1TMRR == 0) && (F1TLRR == 0) )
    {
      nAtmFanSpeed = 0;
    }
    else
    {
//      Curr_Fan1Tachometer = (36000/((WORD)(F1TMRR<<8) + F1TLRR))*60;
      Curr_Fan1Tachometer = (60000/((WORD)(F1TMRR<<8) + F1TLRR))*36;    // 2013.4.1 CAC. update
      nAtmFanSpeed = ( (Curr_Fan1Tachometer+50) / 100 );          // 2013.4.1 CAC. update
    }
    if( (Fan_ON_Count!=0) && (FAN_PWM!=0) )
    {
      if( nAtmFanSpeed > 0 )
      {
        FanExistEN;
        //[Project][CC20130503][Start]Fix abnormal show fan error message on power on status.
        FanInitEN;
        //[Project][CC20130503][End]Fix abnormal show fan error message on power on status.
        if( nAtmFanSpeed < 6 )
        {
          FanErrEN;
        }
        else
        {
          FanErrDIS;
        }
      }
      else
      {
        FanExistDIS;
      }
    }
    if ( IS_MASK_CLEAR(uReserve07,nFanManual))                  // 2013.4.18 CAC. update
    {
      if  (PWM_CHG_delay==0)
      {
        if ((nAtmFanSpeed != TargetRPM)&&(Fan_ON_Count == 0))
        {
          if( nAtmFanSpeed > TargetRPM )
          {
            if( FAN_PWM > 0 )
            { FAN_PWM--; }
          }
          else
          {
            if(FAN_PWM < FAN_PWM_Max)
            { FAN_PWM++; }
            else
            { FAN_PWM = FAN_PWM_Max; }
          }
        }
      }
      else
      {
        PWM_CHG_delay--;
      }
    }
      Show_realPWM();                              // 2013.4.18 CAC. just for test
      Show_targetduty();
  }
}

//-----------------------------------------------------------------------------
// Fan Table, Check Duty offset 1% or 2%.
//-----------------------------------------------------------------------------
void Fan_Offset_Chk(BYTE Duty_Offset, XBYTE *BDCRx)
{
  if ( Duty_Offset != *BDCRx )
  {
    *BDCRx = Duty_Offset;
  }
}


//-----------------------------------------------------------------------------
// FAN table save to ram.
//-----------------------------------------------------------------------------
void Clr_Thermal_Tab(void)
{
  PORT_BYTE_PNTR Thermal_pntr;

  Thermal_pntr = &FAN_Tab_OFF;  // Base on address 0x0160.
  for( ITempW01=0; ITempW01<=9; ITempW01++ )  // Clear 0160~0169.
  {
    *Thermal_pntr=0;
        Thermal_pntr++;
  }

#if Support_COMPAL_MFG_TOOL
  Thermal_pntr = &Step0_L;    // Base on address 0x0170.
  for( ITempW01=0; ITempW01<=9; ITempW01++ )  // Clear 0170~0179.
  {
    *Thermal_pntr=0;
        Thermal_pntr++;
  }
#endif  
}


/*****************************************************************************/
// Procedure: ThrottlingControl                TimeDiv: 100mSec
// Description: Chekc all throttling status.
// GPIO:
// Referrals:
/*****************************************************************************/
void ThrottlingControl(void)
{
  if ( SystemIsS0 )
  {
    cThrottlingSet &= (BIT6 + BIT7);  // Clear Throttling status.

    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < cADPThrottling )  // Compare current and ADP Throttling
    {
      cThrottlingSet |= cADPThrottling;
    }
    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < cTHERMThrottling )  // Compare current and Thermal throttling.
    {
      cThrottlingSet &= (BIT6 + BIT7);
      cThrottlingSet |= cTHERMThrottling;
    }
    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < cBATTThrottling )  // Compare current and BAT throttling.
    {
      cThrottlingSet &= (BIT6 + BIT7);
      cThrottlingSet |= cBATTThrottling;
    }
    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < nThrottlingAPSet )  // For AP or debug
    {
      cThrottlingSet &= (BIT6 + BIT7);
      cThrottlingSet |= nThrottlingAPSet;
    }

    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) != ( REAL_THROTTLING_INDEX & ~(BIT6 + BIT7) ) )
    {  // Cpu throttling for power status change
      if( ( (nRealThermalPinGET) && (( cThrottlingSet & ~(BIT6 + BIT7) ) >= 0x01 ))
      || (!nRealThermalPinGET) )
      {
        SET_MASK(cThrottlingSet,cTHRM_SW);
        REAL_THROTTLING_INDEX = (REAL_THROTTLING_INDEX & (BIT6 + BIT7)) | (cThrottlingSet & ~(BIT6 + BIT7));
        if( (REAL_THROTTLING_INDEX & ~(BIT6 + BIT7) ) == 0x00 )
        {
          nSWThermalPinDIS;
        }
        else
        {
          nSWThermalPinEN;
        }
      }
    }
    if( IS_MASK_SET(cThrottlingSet,cTHRM_SW) )
    {
      CLEAR_MASK(cThrottlingSet,cTHRM_SW);
      if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )  // Check OS mode.
      {
        ECQEvent(CPU_SLOW_AD);   // 0x7B inform bios.
      }
    }
    
    if( ( cGPUThrottlingSet & ~(BIT6 + BIT7) ) != cGPUThrottling )  // Compare current and GPU Throttling
    {
      cGPUThrottlingSet &= (BIT6 + BIT7);
      cGPUThrottlingSet = cGPUThrottling;
      SET_MASK(cGPUThrottlingSet,BIT6);
    }
    if( IS_MASK_SET(cGPUThrottlingSet,BIT6)){
      CLEAR_MASK(cGPUThrottlingSet,BIT6);
      if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )  // Check OS mode.
      {
      //  ECQEvent(GPU_pwrlmt_ON);   // 0x88 inform bios.
      }
    }
    
  }
}

/*****************************************************************************/
// Procedure: Chk_FAN_RPM_Control              TimeDiv: 50mSec
// Description: Check FAN is AP control.
// GPIO:
// Referrals:
/*****************************************************************************/
void Chk_FAN_RPM_Control(void)
{
  BYTE BRPM_Manual;
  if (IS_MASK_SET(uReserve07,nFanManual))
  {
    if (nAtmFanSpeed != ManualFanPRM)
    {
      if( nAtmFanSpeed > ManualFanPRM )
      {
        if( FAN_PWM > 0 )
        { FAN_PWM--; }
      }
      else
      {
        if(FAN_PWM < FAN_PWM_Max)
        {
          if(FAN_PWM == 0)
            {FAN_PWM = FAN_PWM_Min;}          
          else
            { FAN_PWM++; }
        }
        else
        { FAN_PWM = FAN_PWM_Max; }
      }
    }

  }

}


void Fan_control_update(void)
{
   BYTE Fanlevel;
//for fan1
   if (FanAutomaticmodeGet){                  //auto mode
      CLEAR_MASK(uReserve07,nFanManual);
      CLEAR_MASK(Thro_Status2, b4FAN1_FullOn);
   } else {                                   //manual mode  
      if (FanMaximumspeedGet){
        CLEAR_MASK(uReserve07,nFanManual);
        SET_MASK(Thro_Status2, b4FAN1_FullOn);
      }
      else{
        SET_MASK(uReserve07,nFanManual);
        CLEAR_MASK(Thro_Status2, b4FAN1_FullOn);
        Fanlevel = u4Fanspeedcontrol & Fanspeedlevel;
        if (Fanlevel == 0){         //fan speed level 0: stop, control by 8Fh
          ManualFanPRM = u4ManualFanPRM;
        }
        else {    //fan speed level 1~7: control by thermal table
          if( IS_MASK_SET(SYS_MISC1, ACPI_OS) ){     //WIN
             if (Fanlevel >= FanTblPtr[FAN_ID_TYPE].WinTblCount){
               Fanlevel = FanTblPtr[FAN_ID_TYPE].WinTblCount - 1;
             }
             ManualFanPRM = FanTblPtr[FAN_ID_TYPE].WinTbl[Fanlevel].RPMValue;
          }
          else {                              //DOS
             if (Fanlevel >= FanTblPtr[FAN_ID_TYPE].DosTblCount){
               Fanlevel = FanTblPtr[FAN_ID_TYPE].DosTblCount - 1;
             }            
             ManualFanPRM = FanTblPtr[FAN_ID_TYPE].DosTbl[Fanlevel].RPMValue;
          }
        }
      }
   } 
}

/*****************************************************************************/
// Procedure: Oem_Fan_Contro                  TimeDiv: 1 Sec
// Description: Main FAN Control Center
// GPIO:
// Referrals:  CPU/GPU Temperature
/*****************************************************************************/
//-----------------------------------------------------------------------------
void Oem_Fan_Control(void)
{
  BYTE BLEVEL,BPWMDuty;

  // Check VGA and CPU Temperature error bit and full on bit.
  if ( (IS_MASK_SET(ERR_THMSTS, (b1VGATempEr+b3PECITempEr))) ||
      (IS_MASK_SET(Thro_Status2, b4FAN1_FullOn)) )
  {
    FAN_PWM = Target_Duty = FAN_PWM_Max;  // Fan turn on max.
    return;
  }

  if ( Fan_ON_Count != 0 )          // Turn on Fan x sec.
  {
    Fan_ON_Count--;
    return;
  }

  if( SystemNotS0 )  // Not S0 status.
  {
    FAN_PWM = Target_Duty = 0;  // Turn off FAN.
    FanMaximumspeedCLR;         //clean Fan Maximum speed 
    return;
  }
#if Support_COMPAL_MFG_TOOL
  FANTabletoSTPM();
#endif
//  FanInitEN;                  // remove by CAC. 2013.05.06
//	if(nVramTemp>66)
	//{
	//Fan_Offset_Chk(FAN_PWM_Max, &FAN_PWM);
	//}
	//else if(nVramTemp<63)
	//{
    if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )
    { FanCtrlDos(); }  // DOS mode.
    else
    { FanCtrlWin(); }  // OS mode.
	//}
}

void Chk_RPM_for_load_PWM(void)
{

  if( nAtmFanSpeed == TargetRPM )
    return;
  if( nAtmFanSpeed > TargetRPM )
  {
    if ((nAtmFanSpeed - TargetRPM) >= 3)
    {
      Fan_Offset_Chk(Target_Duty, &FAN_PWM);
      PWM_CHG_delay = 5;
    }
  }
  else
  {
    if ((TargetRPM - nAtmFanSpeed) >= 3)
    {
      Fan_Offset_Chk(Target_Duty, &FAN_PWM);
      PWM_CHG_delay = 5;
    }
  }
}

/*****************************************************************************/
// Procedure: OEM_Throttling_Ctrl                TimeDiv: 1 Sec
// Description: System CPU/GPU Throttling Data
// GPIO:
// Referrals:  CPU/GPU Temperature
/*****************************************************************************/
//-----------------------------------------------------------------------------
void OEM_Throttling_Ctrl(void)
{
  if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )
  { ThrottlingDos(); }  // DOS mode.
  else
  { ThrottlingWin(); }  // OS mode.

//-----------------------------------------------------------------------------
// Throttling: Check throttling status.
//-----------------------------------------------------------------------------
if(SystemIsS0||(SysPowState ==SYSTEM_S3_S0)||(SysPowState ==SYSTEM_S5_S0))
{
  if ( IS_MASK_CLEAR(Thro_Status, (b0ProCH_CPU+b1ProCH_VGA+b2ProCH_EXTVGA)) && IS_MASK_CLEAR(pEdgeProjectFlag2,pEdgeACCUTOFF)&&BattExistGet
  	&&IS_MASK_CLEAR(throttle_1, b0_adpi+b1_OCP+b2_OTP+b3_pwr_sys))//&&(S0CPUProchotONCnt==0))//+b4_battery_low))
  {  // turn off prochot.
    H_PROCHOT_LOW();
    nRealThermalPinDIS;
    CLEAR_MASK(pPROCHOT, b0Thermal_PRCOHOTon);  // for AP display.
  }
  else
   {  // turn on prochot.
    H_PROCHOT_HI();
    nRealThermalPinEN;
    SET_MASK(pPROCHOT, b0Thermal_PRCOHOTon);  // for AP display.
  }
}
else
{
 H_PROCHOT_HI();
}
//-----------------------------------------------------------------------------
// Throttling: Check CPU/VGA Turbo status.
//-----------------------------------------------------------------------------
  if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
  {

    //
    //  Behavior updated by Lonestar_Gordon_FAN_Control_Table_PWM_AVC_v04_0314 and 
    //                      Lonestar_Gordon_FAN_Control_Table_PWM_Delta_v04_0314
    //
    //  Changed from P1 to Pmax/2.

    if ( IS_MASK_CLEAR(Thro_Status, (b3Turbo_CPU+b4Turbo_VGA+b5Turbo_EXTVGA)) )
    { cTHERMThrottling &= (BIT6 + BIT7); }  // Not need throttling.
#if (PLATFORM_CHIP == INTEL)
    else
    {
      cTHERMThrottling &= 0xC0;  // Clear thermal throttling status.
      cTHERMThrottling |= CPUturboDisPstage;  // Set P-State level.
    }
#endif    
    if ( IS_MASK_SET(Thro_Status, b7PMAXD2_VGA) || IS_MASK_SET(Thro_Status, b3Turbo_CPU))
    {
      if  (CPUMAXPSTATE != 0)
      {
#if (PLATFORM_CHIP == INTEL)
        if ((cTHERMThrottling & !(BIT6 + BIT7)) < (((CPUMAXPSTATE+1-pCPUBasePStage)/2)+pCPUBasePStage) )
        {
          cTHERMThrottling &= (BIT6 + BIT7);
          cTHERMThrottling |= ((CPUMAXPSTATE+1-pCPUBasePStage)/2)+pCPUBasePStage;  // Set (1/2 (P-State MAX - P-State Base)) + P-State Base.
        }
#else
        if ((cTHERMThrottling & ~(BIT6 + BIT7)) < ((CPUMAXPSTATE+1)/2) )
        {
          cTHERMThrottling &= (BIT6 + BIT7);
          cTHERMThrottling |= ((CPUMAXPSTATE+1)/2);  // Set 1/2 P-State MAX.
        }
#endif        
      }
      else
      {
        cTHERMThrottling &= (BIT6 + BIT7);            // if BIOS no write P-stage MAX
        cTHERMThrottling |= 4;              // Set 1/2 P-State MAX = 4 .
      }
    }

    if ( IS_MASK_CLEAR(Thro_Status, (b4Turbo_VGA+b5Turbo_EXTVGA)) )
    {
      if ( IS_MASK_SET(uVGATurboFun,uDisVGATurboOK) )    // bit2.
      {
        CLEAR_MASK(uVGATurboFun, uDisVGATurboOK);      // Set bit2.
        //ECQEvent(DIS_TURBO_63);             // 0x63 VGA Disable turbo.
//        VGA_AC_BATT_ON(); // GPU throtle off
      }
    }
    else
    {
       if ( IS_MASK_CLEAR(uVGATurboFun,uDisVGATurboOK) )  // bit2.
       {
        SET_MASK(uVGATurboFun, uDisVGATurboOK);      // Set bit2.
        //ECQEvent(EN_TURBO_62);               // 0x62 VGA Enable turbo.
//        VGA_AC_BATT_OFF(); // GPU throtle on
       }
    }


	 if ( IS_MASK_CLEAR(Thro_Status, b6ProCH_TURBO) )
    {
      if ( IS_MASK_SET(uVGATurboFun,uSTPturboOK) )  
      	{
        CLEAR_MASK(uVGATurboFun, uSTPturboOK);     
        ECQEvent(APU_STP_ENTURBO);             
      }
    }
    else
    {
       if ( IS_MASK_CLEAR(uVGATurboFun,uSTPturboOK) )  
       {
        SET_MASK(uVGATurboFun, uSTPturboOK);     
        ECQEvent(APU_STP_DISTURBO);              
       }
    }
  }
}

//


void DetectFanId (
  void
)
{
  if ( FAN_ID > FAN_VEN_BOUNDARY_1 ) {         // Delta Fan.
    FAN_ID_TYPE = Delta;
  } else if ( FAN_ID > FAN_VEN_BOUNDARY_2 ) {  // Sunon Fan.
    FAN_ID_TYPE = Sunon;
  } else {                                     // AVC Fan.
    FAN_ID_TYPE = AVC;
  }
  //
  // Hook for fan table.
  //
  oem_FanTblPtrHook();
}

void FanCtrlWin(void)
{
  BYTE BCPULevel, BTSLevel, BVGALevel,Level_chg;
  // Level_chg bit0: BCPULevel change flag
  // Level_chg bit1: BVGALevel change flag

  Level_chg = 0;
	do
	{
		BTSLevel = (FANTSSeg & 0x0F);

		if(BTSLevel <= 0)
		{
			BTSLevel = 0;

			if(nRamTemp >=  FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOn) { BTSLevel++; }
		}
		else if (BTSLevel >= (FanTblPtr[FAN_ID_TYPE].TsTblCount - 1))
		{
			BTSLevel = (FanTblPtr[FAN_ID_TYPE].TsTblCount - 1);
			if(nRamTemp <= FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOff) { BTSLevel--; }
		}
		else
		{
			if(nRamTemp >= FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOn) { BTSLevel++; }
			if(nRamTemp <= FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOff) { BTSLevel--; }
		}
		if (FANTSSeg != BTSLevel) { SET_MASK(Level_chg,BIT3); }     // BCPULevel change flag
		FANTSSeg = BTSLevel;
		//if thermal sensor fan run,need not detect cpu temperature
		if(BTSLevel > 0)
		{
			FANCPUSeg = 0;
			FAN_Tab_LOW   = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOn;
			FAN_Tab_HI    = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOff;
			FAN_Tab_DAC    = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].RPMValue;
			break;
		}
	
		BCPULevel = (FANCPUSeg & 0x0F);  // Now CPU Fan Step

  if(BCPULevel <= 0)
  {
    BCPULevel = 0;
    if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOn) { BCPULevel++; }
  }
  else if (BCPULevel >= (FanTblPtr[FAN_ID_TYPE].WinTblCount - 1))
     {
      BCPULevel = (FanTblPtr[FAN_ID_TYPE].WinTblCount - 1);
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
     else
     {
      if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOn) { BCPULevel++; }
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
  if (FANCPUSeg != BCPULevel) { SET_MASK(Level_chg,BIT0); }     // BCPULevel change flag
  FANCPUSeg = BCPULevel;
  FAN_Tab_LOW   = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOn;
  FAN_Tab_HI    = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOff;
  FAN_Tab_DAC    = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue;
	}while(0);

  BVGALevel = (FANVGASeg & 0x0F);  // Now CPU Fan Step
  if(BVGALevel <= 0)
  {
    BVGALevel = 0;
    if(VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOn) { BVGALevel++; }
  }
  else if (BVGALevel >= (FanTblPtr[FAN_ID_TYPE].WinVgaTblCount - 1))
     {
      BVGALevel = (FanTblPtr[FAN_ID_TYPE].WinVgaTblCount - 1);
      if(VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOff) { BVGALevel--; }
     }
     else
     {
      if(VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOn) { BVGALevel++; }
      if(VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOff) { BVGALevel--; }
     }
  if (FANVGASeg != BVGALevel) { SET_MASK(Level_chg,BIT1); }    // BVGALevel change flag
  FANVGASeg = BVGALevel;
  VFAN_Tab_HI   = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOn;
  VFAN_Tab_LOW  = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOff;
  VFAN_Tab_DAC  = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].RPMValue;

#if (PLATFORM_CHIP == INTEL)
  if (( PCH_Temp3 >= 85 )&&(FANPCHSeg ==0))
  {
    if (TargetRPM < RPM_PCH_Max)
    {
      if ((RPM_PCH_Max - TargetRPM) > 4 )
      {
        Target_Duty = FAN_PCH_Max;
        Fan_Offset_Chk(Target_Duty, &FAN_PWM);
      }
    }
    TargetRPM = RPM_PCH_Max;
    FANPCHSeg = 1;
    return;
  }
  if ( FANPCHSeg !=0 )
  {
    if (PCH_Temp3 <= 70)
    {
      FANPCHSeg = 0;
      SET_MASK(Level_chg,BIT0);    // BCPULevel change flag
      SET_MASK(Level_chg,BIT1);    // BVGALevel change flag
    }
    else
    {
      TargetRPM = RPM_PCH_Max;
      return;
    }
  }
#endif // PLATFORM_CHIP

 /* if ( (FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue) > (FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].RPMValue) )
  {
    TargetRPM = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue;
    if (IS_MASK_SET(Level_chg,BIT0))    // Level change flag be set, then load the FAN PWM initial duty
    {
      Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].PWMDuty,FAN_PWM_Max);
      Chk_RPM_for_load_PWM();
    }
  }
  else
  {
    TargetRPM = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].RPMValue;
    if (IS_MASK_SET(Level_chg,BIT1))    // Level change flag be set, then load the FAN PWM initial duty
    {
      Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].PWMDuty,FAN_PWM_Max);
      Chk_RPM_for_load_PWM();
    }
  }*/

if(BTSLevel)
	{
		TargetRPM = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].RPMValue;
	}
	else
	{
		TargetRPM = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue;
	}

}


void FanCtrlDos(void)
{
  BYTE BCPULevel,Level_chg;
  // Level_chg bit0: BCPULevel change flag
  // Level_chg bit1: BVGALevel change flag

  Level_chg = 0;
  BCPULevel = (FANCPUSeg & 0x0F);  // Now VGA Fan Step
  if(BCPULevel <= 0)
  {
    BCPULevel = 0;
    if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOn) { BCPULevel++; }
  }
  else if (BCPULevel >= (FanTblPtr[FAN_ID_TYPE].DosTblCount - 1))
     {
      BCPULevel = (FanTblPtr[FAN_ID_TYPE].DosTblCount - 1);
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
     else
     {
      if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOn) { BCPULevel++; }
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
  if (FANCPUSeg != BCPULevel) { SET_MASK(Level_chg,BIT0); }    // BCPULevel change flag
  FANCPUSeg = BCPULevel;
  FAN_Tab_LOW   = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOn;
  FAN_Tab_HI    = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOff;
  FAN_Tab_DAC    = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].RPMValue;

  TargetRPM = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].RPMValue;
  if (IS_MASK_SET(Level_chg,BIT0))    // Level change flag be set, then load the FAN PWM initial duty
  {
    Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].PWMDuty,FAN_PWM_Max);
    Chk_RPM_for_load_PWM();
  }
}


void ThrottlingWin(void)
{
  THR_PRO_OFF   = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Off;
  THR_PRO_ON    = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_On;
  THR_Turo_Rem  = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Rem;
  THR_Turo_OFF  = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Off;
  THR_Tab_Shut  = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Shut;

  // Set Internal VGA.
  if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].PMAX_D2_on )    // Check P_MAX/2 turn ON.
    { 
      SET_MASK(Thro_Status, b7PMAXD2_VGA);
    }
  else if ( VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].PMAX_D2_off )  // Check P-MAX/2 turn OFF.
    { 
      CLEAR_MASK(Thro_Status, b7PMAXD2_VGA);
    }

  if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Thr_On )  // Check prochot turn ON.
    {
#if (PLATFORM_CHIP == INTEL)
      SET_MASK(Thro_Status, b1ProCH_VGA);
#endif
      SET_MASK(GPU_THROTTLE_SRC, THERMAL_POLICY);
    }
  else if ( VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Thr_Off )    // Check prochot turn OFF.
    {
#if (PLATFORM_CHIP == INTEL)
      CLEAR_MASK(Thro_Status, b1ProCH_VGA); 
#endif
      CLEAR_MASK(GPU_THROTTLE_SRC, THERMAL_POLICY);
    }



//*********************************************************
//     Thermal Policy For Thermal sensor
//*********************************************************

  if (nCpuTempLocal >= Ext_Sen1_On ||nVramTemp >= Ext_Sen3_On)  // Check prochot turn ON.
    { 
      SET_MASK(GPU_THROTTLE_SRC, External_Sensor);
    }
  else if ( nCpuTempLocal <= Ext_Sen1_Off || nVramTemp <= Ext_Sen3_Off )    // Check prochot turn OFF.
    { 
      CLEAR_MASK(GPU_THROTTLE_SRC, External_Sensor);
    }

  if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Thr_Shut )    // Check shutdown status.
  { Thro_Count_Shut(&VGA_Shut_Cnt,TEMP_TYPE_VGA); }  // Check x times for shutdown protection.
  else
  { VGA_Shut_Cnt = 0; }
  if (nRamTemp >=Ext_Sen2_On )
  	{SET_MASK(Thro_Status, b6ProCH_TURBO);
  	}
  else if(nRamTemp <=Ext_Sen2_Off)
  	{CLEAR_MASK(Thro_Status, b6ProCH_TURBO);
  	}

  

  // Set Internal CPU.
  if ( TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Off )    // Check prochot turn OFF.
  { CLEAR_MASK(Thro_Status, b0ProCH_CPU); }
  else if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_On )  // Check prochot turn ON.
  { SET_MASK(Thro_Status, b0ProCH_CPU); }

  if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Shut )    // Check shutdown status.
  { Thro_Count_Shut(&CPU_Shut_Cnt,TEMP_TYPE_CPU); }    // Check x times for shutdown protection.
  else
  { CPU_Shut_Cnt = 0; }

  if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
  {
    // Set Internal VGA Turbo.
    if ( VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Turbo_Rem )    // Check turbo resume.
    { CLEAR_MASK(Thro_Status, b4Turbo_VGA); }
//     else if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Turbo_Off )  // Check turbo Off.
//     { SET_MASK(Thro_Status, b4Turbo_VGA); }

    // Set Internal CPU Turbo.
    if ( TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Rem )    // Check turbo resume.
    { CLEAR_MASK(Thro_Status, b3Turbo_CPU); }
    else if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Off )// Check turbo Off.
    { SET_MASK(Thro_Status, b3Turbo_CPU); }
  }

}

void ThrottlingDos(void)
{
  THR_PRO_OFF   = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Off;
  THR_PRO_ON    = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_On;
  THR_Turo_Rem  = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Turbo_Rem;
  THR_Turo_OFF  = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Turbo_Off;
  THR_Tab_Shut  = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Shut;

  // Set Internal CPU.
  if ( TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Off )    // Check prochot turn OFF.
  { CLEAR_MASK(Thro_Status, b0ProCH_CPU); }
  else if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_On )  // Check prochot turn ON.
  { SET_MASK(Thro_Status, b0ProCH_CPU); }

  if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Shut )    // Check shutdown status.
  { Thro_Count_Shut(&CPU_Shut_Cnt,TEMP_TYPE_CPU); }      // Check x times for shutdown protection.
  else
  { CPU_Shut_Cnt = 0; }
}


//-----------------------------------------------------------------------------
// Throttling: Shutdown count used.
//-----------------------------------------------------------------------------
void Thro_Count_Shut(XBYTE *BShut_Cnt, BYTE TempType)
{
  if ( *BShut_Cnt >= Thro_Shut_Cnt )  // Read 3 times.
  {
    if ( TempType == 1 )
    { ProcessSID(VGAOVERTEMP_ID); }  // VGA Shutdown ID 0x12.
    else
    { ProcessSID(DTSOVERTEMP_ID); }  // CPU Shutdown ID 0x11.

    SET_MASK(SysStatus,ERR_ShuntDownFlag);

//[-start-131212-T130124_8-remove]//
//     PWSeqStep = 1;
//     PowSeqDelay = 0x00;
//     SysPowState = SYSTEM_S0_S5;      // EC force Shutdown.
//[-end-131212-T130124_8-remove]//
    *BShut_Cnt = 0;
    RSMRST_shutdown();

    //
    // AOU -- Unexpected power shutdown by thermal issue.
    // (The user needs to take off all power of the system for recovery from the thermal issue)
    //
    SET_MASK(USB_Charger, b2USB_TmlDis);  // Disable USB charger.

    //
    //   Dear Dustin,
    //     If we has embedded battery ,when happen unexpected shoutdow by thermal issue., the AOU function works also. 
    //   DavidMa (ThinkPad China Department Lab)
    //
    //   2014.06.23
    if (BattExistGet) {
      if (IS_MASK_SET(OptMfgFunction2L, BIT7)) { // Replaceable(0)/Non-replaceable(1)
        CLEAR_MASK(USB_Charger, b2USB_TmlDis);  // Enable USB charger.
      }
    }
  }
  else
  { (*BShut_Cnt)++; }
}
#if Support_COMPAL_MFG_TOOL
void FANTabletoSTPM(void)
{
  if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) ) {
    //
    // DOS mode.
    //
    Step0_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[0].FanOn;
    Step0_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[0].FanOff;
    Step0_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[0].RPMValue;
    Step1_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[1].FanOn;;
    Step1_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[1].FanOff;;
    Step1_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[1].RPMValue;
    Step2_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[2].FanOn;;
    Step2_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[2].FanOff;;
    Step2_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[2].RPMValue;
    Step3_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[3].FanOn;;
    Step3_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[3].FanOff;;
    Step3_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[3].RPMValue;
    Step4_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[4].FanOn;;
    Step4_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[4].FanOff;;
    Step4_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[4].RPMValue;
  } else {
    //
    // OS mode.
    //
    Step0_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[0].FanOn;
    Step0_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[0].FanOff;
    Step0_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[0].RPMValue;
    Step1_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[1].FanOn;;
    Step1_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[1].FanOff;;
    Step1_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[1].RPMValue;
    Step2_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[2].FanOn;;
    Step2_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[2].FanOff;;
    Step2_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[2].RPMValue;
    Step3_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[3].FanOn;;
    Step3_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[3].FanOff;;
    Step3_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[3].RPMValue;
    Step4_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[4].FanOn;;
    Step4_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[4].FanOff;;
    Step4_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[4].RPMValue;
  }
}
#endif 

