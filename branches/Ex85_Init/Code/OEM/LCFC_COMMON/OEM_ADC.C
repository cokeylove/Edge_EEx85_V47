/*-----------------------------------------------------------------------------
 * TITLE: OEM_ADC.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// The function of disabling ADC channel
//----------------------------------------------------------------------------
void DisableADCChannel(void)
{
  CLEAR_MASK(ADCCFG, BIT0);  // Disable ADC module
}

//----------------------------------------------------------------------------
// The function of Enabling ADC channel
//----------------------------------------------------------------------------
void EnableADCChannel(void)
{
    SET_MASK(ADCCFG, BIT0);    // Enable ADC module
}

//----------------------------------------------------------------------------
// The function of scaning ADC channel
//----------------------------------------------------------------------------
void ScanADCChannel(void)
{
  BYTE i;
  LWORD ADP_I_SUM = 0;

//BATT_TEMP   ADP2
    if(IS_MASK_SET(VCH2CTL, BIT7))
    {
        VCH2CTL |= 0x80;        // write clear data vaild flag
        ADC_Data2= ((WORD)VCH2DATM << 8) + VCH2DATL;
    }
//Fan_ID     ADP4
    if(IS_MASK_SET(VCH4CTL, BIT7))
    {
        VCH4CTL |= 0x80;        // write clear data vaild flag
        ADC_Data4 = ((WORD)VCH4DATM << 8) + VCH4DATL;
    }

//ADP_I     ADP5
    if(IS_MASK_SET(VCH5CTL, BIT7))
    {
       VCH5CTL |= 0x80;        // write clear data vaild flag
       ADC_Data5= (((WORD)VCH5DATM << 8) + VCH5DATL);//*ADPI_MUL;
    }
	
#if (PortI6_Ctrl == ALT)  
//pwr_sys     ADP6
    if(IS_MASK_SET(VCH6CTL, BIT7))
    {
       VCH6CTL |= 0x80;        // write clear data vaild flag
       ADC_Data6= (((WORD)VCH6DATM << 8) + VCH6DATL);//*ADPI_MUL;
    }
#endif
//API_ID    ADP7
    if( IS_MASK_SET(VCH7CTL, BIT7) )
    {
        VCH7CTL |= 0x80;        // write clear data vaild flag
        ADC_Data7 = ((WORD)VCH7DATM << 8) + VCH7DATL;
    	if( IS_MASK_SET(cCheatAD,cCAD_EN) )
    	{
        	if( IS_MASK_SET(cCheatAD,cCAD_ADPID) )
        	{
            	API_ID = cCADP_ID;
        	}
    	}
    }
#if  Support_SMTADP
            CovertADPID();
#endif  // Suport_SMTADP

    if( IS_MASK_SET(cCheatAD,cCAD_EN) )
    {
        if( IS_MASK_SET(cCheatAD,cCAD_ADPI) )
        {
            ADP_I = cCADP_I;
        }
    }
    if (SysPowState == SYSTEM_S0)
    {
//        Chk_ACOP();
        //ADPI2Sec++;
        //for (i=3; i>0; i--)
        //{
        //    ADPI_Data[i] = ADPI_Data[i-1];
        //    ADP_I_SUM += ADPI_Data[i];
        //}
        //ADPI_Data[0] = ADP_I;
        //ADP_I_SUM += ADPI_Data[0];
//         ADPI_AvgData = ADP_I_SUM / 4;
        //ADPI_AvgData = ADP_I;

        if(((SDV_PHASE() || FVT_PHASE()) && BoardID_readyGET)){
          ADPI_AvgData = ADP_I;
        }else if(BoardID_readyGET){
        ADPI_AvgData = ADP_I;
        	if(ADPI_MUL == 0)
        	{
        		ADPI_MUL = ClassicAdaptor_ADPI_MUL;
        	}
        }
        //ADPI_AvgData = (ADP_I + ADPI_AvgData)/2;
        //if (ADPI2Sec == 0 ){
          //DownBatteryPState();
          if (BattExistGet){
            Chk_Hybrid_STPP();
          }else{
            Chk_AC_STPP(0);
          }
        //}else{
        //  ADPI2Sec--;
        //}
        /*if (ADPI2Sec > 200)
        {
            DownBatteryPState();
            if (BattExistGet)
            {
                Chk_Hybrid_STPP();
            }
            else
            {
                Chk_AC_STPP(0);
            }
            ADPI2Sec=0;
        }*/
    }
}

//----------------------------------------------------------------------------
// Init ADC module
//----------------------------------------------------------------------------
void Init_ADC(void)
{
#if (PortI0_Ctrl == ALT)
  VCH0CTL=ADCChannel_0;  //CH0;      
#endif  
#if (PortI1_Ctrl == ALT)
  VCH1CTL=ADCChannel_1;  //CH1;    
#endif    
#if (PortI2_Ctrl == ALT)
  VCH2CTL=ADCChannel_2;  //CH2;      BATT_TEMP
#endif    
#if (PortI3_Ctrl == ALT)  
  VCH3CTL=ADCChannel_3;  //CH3;      Board_ID
#endif   
#if (PortI4_Ctrl == ALT)  
  VCH4CTL=ADCChannel_En; //CH4;      FAN_ID
#endif    
#if (PortI5_Ctrl == ALT)  
  VCH5CTL=ADCChannel_En; //CH5;      ADP_I
#endif    
#if (PortI6_Ctrl == ALT)  
  VCH6CTL=ADCChannel_En; //CH6;  
#endif    
#if (PortI7_Ctrl == ALT)  
  VCH7CTL=ADCChannel_En; //CH7;      ADP_ID
#endif  
  ADC_Module_Enable();
}

void ADC_Module_Enable(void)
{
  XBYTE *lp_adc_reg_1932h;

  if((ECHIPID1==0x85)&&((ECHIPID2==0x86)||((ECHIPID2==0x28))))
  {
    if(ECHIPVER<=0x06)
    {
      //
      // bit2 ~ bit0 : default 0x02, SAR ADC 2 - 7
      //
      ADCGCR |= (BIT0+BIT1+BIT2);

      //
      // 1932h : bit3 ~ bit1 : interval time of SAR ADC 2 - 7
      //
      lp_adc_reg_1932h = 0x1932;
      SET_MASK((*lp_adc_reg_1932h), (BIT3+BIT2+BIT1));
    }
  }

  //
  // bit 5 : adc conversion time select 0 (ADCCTS0)
  // ADC channel conversion time
  // ADCCTS1, ADCCTS0     :   11b                 01b                 00b or 10b
  // conversion time (us) :   200*(SCLKDIV+1)     103.2*(SCLKDIV+1)   30.8*(SCLKDIV+1)
  //
  // bit 0 : adc module enable/disable
  //
  ADCCFG = (BIT5+BIT0);

}

// CMW 20121022 +
//---------------------------------------------------------------------------
// Init Voltage Compare Function
// Input: 1 Group. choice the init VC channel  0: VC0 1: VC1 2:VC2 3:All VC Channel
//---------------------------------------------------------------------------
void Init_VC(BYTE Group)
{
  int tGroup = (Group&0x03);
  #if Support_VC0_Function
  // Set the CPU Thermal VC Setting
  if( (tGroup == 0x00) || (tGroup == 0x03) )
  {
    VCMP0CTL &= 0x3F;  // Disable VC0 & Interrupt.
    CMP0THRDATM = (VC0TriggerValue>>8);    // threshold value buffer
    CMP0THRDATL = VC0TriggerValue;
    VCMP0CTL &= 0xF8;  // Clear ADC Channel & set to Channel 0.
    VCMP0CTL &= 0xDF;  // Set the Less Then Voltage trigger.
    VCMP0CTL |= 0x08;  // Set Active-Low.
    GCR15 |= 0x01;    // Enable GPJ3 to be VC1 GPO.
    GPCRJ3 = 0x00;
    VCMP0CTL |= 0x40;  // Enable Interrupt.
    VCMP0CTL |= 0x80;
  }
  #endif

  #if Support_VC1_Function
  // Set the ADP_I VC setting
  if( (tGroup == 0x01) || (tGroup == 0x03) )
  {
    VCMP1CTL &= 0x3F;  // Disable VC1 & Interrupt.
    //CMP1THRDATM = (VC1TriggerValue>>8);  // threshold value buffer
    //CMP1THRDATL = VC1TriggerValue;
    SetTurbo_V_Thro();  // Set Turbo_V trigger point.
    VCMP1CTL &= 0xF8;  // Clear ADC Channel
    VCMP1CTL |= 0x01;  // Connect to ADC Channel 1.
    VCMP1CTL |= 0x20;  // Set the Over Voltage trigger.
    VCMP1CTL &= 0xF7;  // Set Active-High.
    GCR15 |= 0x02;    // Enable GPJ4 to be VC1 GPO.
    GPCRJ4 = 0x00;
    VCMP1CTL |= 0x40;  // Enable Interrupt.
    VCMP1CTL |= 0x80;  // Enable VC1
  }
  #endif

  #if Support_VC2_Function
  // Set the xxx VC setting.
  if( (tGroup == 0x02) || (tGroup == 0x03) )
  {
  }
  #endif

  if( (VCMP0CTL&0x80) || (VCMP1CTL&0x80) || (VCMP2CTL&0x80) )
  {
    IER18 |= 0x80;
  }
}
// CMW 20121022 -


void Chk_Wrong_ADP(void)
{
  if( (SysPowState == SYSTEM_S0) || (SystemNotS0 && (Chk_Wrong_ADP_Status != 0)) )
  {

    if((ACIN_FallINT_Count > 0) && (Chk_Wrong_ADP_Status == 0))
    {
      Chk_Wrong_ADP_Status = Chk_Wrong_ADP_Status_wait_2sec;
    }

    switch(Chk_Wrong_ADP_Status)
    {
    case Chk_Wrong_ADP_Status_wait_2sec:
      Chk_Wrong_10ms_Count++;
      if (Chk_Wrong_10ms_Count  > 200)
      {
        CLEAR_MASK(ACOFF_SOURCE, ADPOVP);
        ACOFF_LOW();
        CLEAR_MASK(cBattFlag0,cCmdAcOff);       //Resume Charger
        //CHGIC_ptr = 2;            //reminder: call to write to SmartChg
        //WriteSmartChgIC();
        Chk_Wrong_10ms_Count = 0;    //
        ACIN_FallINT_Count = 0;
        Chk_Wrong_ADP_Status = Chk_Wrong_ADP_Status_wait_10sec;
      }
      else
      {
        SET_MASK(cBattFlag0,cCmdAcOff);       //Stop charger
        //CHGIC_ptr = 2;            //reminder: call to write to SmartChg
        //WriteSmartChgIC();
        SET_MASK(ACOFF_SOURCE, ADPOVP);
//        ACOFF_HI();
      }
      break;
    case Chk_Wrong_ADP_Status_wait_10sec:
      if (Chk_Wrong_10ms_Count > 1000)  //Over 10s not detect 5 times ACIN drop.
      {
        Chk_Wrong_10ms_Count = 0;     //Reset all counter
        ACIN_FallINT_Count = 0;
        Chk_Wrong_ADP_Status=0;       //Reset Status
      }
      else
      {
        if (ACIN_FallINT_Count >= 5)      //falling 5 times
        {
          Chk_Wrong_ADP_Status = Chk_Wrong_ADP_Status_ACOFF_Lock;
          ACIN_FallINT_Count = 0;
          Chk_Wrong_10ms_Count = 0;
          SET_MASK(cBattFlag0,cCmdAcOff);     //Stop charger
          //CHGIC_ptr = 2;          //reminder: call to write to SmartChg
          //WriteSmartChgIC();
          SET_MASK(ACOFF_SOURCE, ADPOVP);
//          ACOFF_HI();            //Pull High AC off until reset

        }
        Chk_Wrong_10ms_Count++;
      }
      break;
    case Chk_Wrong_ADP_Status_ACOFF_Lock:
      //Reset AC OFF Lock after Plug out ACIN 20ms
      if (!Read_AC_IN())
      {
        if (Chk_Wrong_10ms_Count >2)
        {
          Chk_Wrong_ADP_Status = 0;       //Reset Status and counter
          Chk_Wrong_10ms_Count = 0;
          ACIN_FallINT_Count =0;
          CLEAR_MASK(ACOFF_SOURCE, ADPOVP);
          ACOFF_LOW();
          CLEAR_MASK(cBattFlag0,cCmdAcOff);       //Resume Charger
          //CHGIC_ptr = 2;            //reminder: call to write to SmartChg
          //WriteSmartChgIC();
        }
        Chk_Wrong_10ms_Count ++;
      } else {
        if (pLastSID != AC_WRONG_ADP_ID) {
          SET_MASK(SysStatus,ERR_ShuntDownFlag);
          ProcessSID (AC_WRONG_ADP_ID);
        }
      }
      break;
    default:
      Chk_Wrong_ADP_Status = 0;
      Chk_Wrong_10ms_Count = 0;
      ACIN_FallINT_Count = 0;
      break;
    }
  }
  else
  {
    Chk_Wrong_ADP_Status = 0;
    Chk_Wrong_10ms_Count = 0;
    ACIN_FallINT_Count = 0;
  }
}

/*  The memory used in Chk_ACOP function.
typedef enum
{
  Chk_ACOP_Status_wait_10sec,
  Chk_ACOP_Status_ACOFF_Lock,
}Chk_ACOP_Status_Type;
extern XBYTE  Chk_ACOP_Status;          //_at_(ECRAM1+0x94);
extern XWORD Chk_ACOP_10ms_Count;        //_at_(ECRAM1+0x95);*/
void Chk_ACOP(void)
{
  // Does not check ACOP when TurboBoost enable
  if (IsChargerTurboOn())    //for new ChargerIC 24780s    //if (IS_MASK_SET(CHGIC_ReadCmd0x12L, TurboBoost))    //for old ChargerIC 24780  
  {
    return;
  }

  if (IS_MASK_SET(CurrentH, BIT7))
  {
    Chk_ACOP_Bat_Chg_Current = 0xFFFF - (WORD)((CurrentH << 8) + CurrentL);//mA
  }
  else
  {
    Chk_ACOP_Bat_Chg_Current = 0;
  }

  // When AC_IN and Battery is discharging, the current > 25mA
  if (IS_MASK_SET(CurrentH, BIT7)) // Battery is discharging
  {
    if(Read_AC_IN() && (Chk_ACOP_Status == 0) && Chk_ACOP_Bat_Chg_Current > 25)
    {
      Chk_ACOP_Status = Chk_ACOP_Status_wait_10sec;
    }
  }

  switch(Chk_ACOP_Status)
  {
    case Chk_ACOP_Status_wait_10sec:
      Chk_ACOP_10ms_Count++;
      if (Chk_ACOP_10ms_Count >= 1000)         //ACOP over 10s
      {
        Chk_ACOP_Status = Chk_ACOP_Status_ACOFF_Lock;
        Chk_ACOP_10ms_Count = 0;
      }
      else
      {
        if (!(Read_AC_IN() && (Chk_ACOP_Bat_Chg_Current < 25)))
        {
          Chk_ACOP_Status = 0;
        }
      }
      break;
    case Chk_ACOP_Status_ACOFF_Lock:
      if (!Read_AC_IN())
      {
        Chk_ACOP_10ms_Count++;
        if (Chk_ACOP_10ms_Count >= 20)           //ACOP over 200ms
        {
          Chk_ACOP_Status = Chk_ACOP_Status_Null;
          Chk_ACOP_10ms_Count = 0;
          CLEAR_MASK(ACOFF_SOURCE, ACOP);
          ACOFF_LOW();
        }
      }
      else
      {
        SET_MASK(ACOFF_SOURCE, ACOP);
        ACOFF_HI();
      }
      break;
    default:
      Chk_ACOP_Status = 0;
      Chk_ACOP_10ms_Count = 0;
      break;
  }
}

void ChargerTurbo (
  BYTE OnOff
)
{
  //
  // 0: Stop charger turbo, 1: Start charger turbo
  //
#if (CHARGER_IC == BQ24780S)   
  if (BattExistGet) {
    if (RelativeStateOfChgL <= BAT_HYBOOST_DIS) {
      // Battery RSOC <= 10%, Charger Turbo should be disabled.
      CLEAR_MASK (CHGIC_ReadCmd0x37L, TurboBoost);
    } else if (RelativeStateOfChgL > BAT_HYBOOST_EN) {
      // Battery RSOC > 10%, Charger Turbo can be enabled.
      if (OnOff) {
        SET_MASK (CHGIC_ReadCmd0x37L, TurboBoost);     
      }
    }

    if (OnOff == MPC_OFF) {
        CLEAR_MASK (CHGIC_ReadCmd0x37L, TurboBoost);     
    }

    if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption3, &CHGIC_ReadCmd0x37L,SMBus_NoPEC))   
    {
      CHGIC_SMbusFailCnt++;
    }
    else
    {
      CHGIC_SMbusFailCnt = 0;
    }
  }
#endif //BQ24780S
}

void BatteryCharge (
  BYTE OnOff
)
{
  //
  // 0: Stop charge, 1: Start charge
  //
  if (OnOff) {
    CLEAR_MASK(nStopChgStat3H, HybridModeStopChg);
    CLEAR_MASK(cBattFlag0,cCmdAcOff);       //Resume Charger
    CLEAR_MASK(nStopChgStat3H, ENADPTHROTTLING);
  } else {
    SET_MASK(nStopChgStat3H, HybridModeStopChg);
    SET_MASK(cBattFlag0,cCmdAcOff);       //Stop Charger
    SET_MASK(nStopChgStat3H, ENADPTHROTTLING);
  }
}

void CpuTurbo (
  BYTE OnOff
)
{ 
  if (BattExistGet && !Read_AC_IN()) {
    //
    // Battery condition
    //
    if (RelativeStateOfChgL <= BAT_CPU_TURBO_DIS) {
      // Battery RSOC <= 10%, CPU Turbo should be turned off.
      if (IS_MASK_CLEAR(pProject0, bRSOCAPUTURBO)) {
        SET_MASK(pProject0, bRSOCAPUTURBO);
	      SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CPUtuborONOFF(Disable);
      }
        UpGPUPState();
    } else if (RelativeStateOfChgL > BAT_CPU_TURBO_EN) {
      // Battery RSOC > 10%, CPU Turbo can be turned on.
      if (OnOff)
      {
        if ( IS_MASK_SET(pProject0, (bHYBAPUTURBO|bRSOCAPUTURBO)) ) {
          CLEAR_MASK(pProject0,bRSOCAPUTURBO);
          CLEAR_MASK(pProject0,bHYBAPUTURBO);
          CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
          CPUtuborONOFF(Enable);
        }
      }
    }

    if (OnOff == MPC_OFF) 
    {
      if (IS_MASK_CLEAR(pProject0, bHYBAPUTURBO)) {
        SET_MASK(pProject0,bHYBAPUTURBO);
        SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CPUtuborONOFF(Disable);
      }
        UpGPUPState();
    }
  } else {
    //
    // Adapter condition
    //
    if (IS_MASK_SET(pProject0,bAPUTURBO)) {
      return;
    } else {	  	
	  #if (PLATFORM_CHIP == AMD)	
      if(( AdapterID != LimitedSupportAdpWatt ) || ( AdapterID != AdapterID_NON_SUPPORT ))
      #else
      if( AdapterID > LimitedSupportAdpWatt)
      #endif
	  {
        if (OnOff)
        {
          if ( IS_MASK_SET(pProject0, (bHYBAPUTURBO|bRSOCAPUTURBO)) ) {
            CLEAR_MASK(pProject0,bRSOCAPUTURBO);
            CLEAR_MASK(pProject0,bHYBAPUTURBO);
            CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
            CPUtuborONOFF(Enable);
            CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
          }
        } else {
          if (IS_MASK_CLEAR(pProject0, bHYBAPUTURBO)) {
            SET_MASK(pProject0,bHYBAPUTURBO);
            SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
            CPUtuborONOFF(Disable);
            SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
          }
            UpGPUPState();
        }
      }else{
        if(BattExistGet){
            if (RelativeStateOfChgL <= BAT_CPU_TURBO_DIS) {
      // Battery RSOC <= 10%, CPU Turbo should be turned off.
        SET_MASK(pProject0, bRSOCAPUTURBO);
	      SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CPUtuborONOFF(Disable);
            SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
        UpGPUPState();
    } else if (RelativeStateOfChgL > BAT_CPU_TURBO_EN) {
      // Battery RSOC > 10%, CPU Turbo can be turned on.
          CLEAR_MASK(pProject0,bRSOCAPUTURBO);
          CLEAR_MASK(pProject0,bHYBAPUTURBO);
          CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
          CPUtuborONOFF(Enable);
          CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
        }
        }
      }
      
    }
  }
}

void Chk_Hybrid_STPP(void)
{
  LWORD BattPresentVolt;
  LWORD BattNowCurrent;
  LWORD Batt_Output_Power;

  #if !Support_MAXPWR
  return;
  #endif

  if (inhibit2sec > 0)
  {
    CurrentL = 0;
    CurrentH = 0;
  }

  if (ADPI2Sec != 0){
    ADPI2Sec--;
  }

  if (GPU2Sec!= 0){
    GPU2Sec--;
  }
  CLEAR_MASK(BatteryAlarm,ADPOUT);
  CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);
//
// Adapter in + Battery in mode
//

//   if(Read_AC_IN() && IS_MASK_SET(CHGIC_ReadCmd0x12L,TurboBoost))//(RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge)) //Check Battery over 20%
  if(Read_AC_IN() && !Read_ACOFF() && (RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge)) //Check Battery over 20%
  {
    if (IsChargerTurboOn() == FALSE) {
      ChargerTurbo(MPC_ON);
    }

    BattPresentVolt = (WORD)((VoltageH << 8) + VoltageL);
    if (IS_MASK_SET(CurrentH, BIT7))
    {
      BattNowCurrent = 0xFFFF - (WORD)((CurrentH << 8) + CurrentL);
    }
    else
    {
      BattNowCurrent = 0;
    }
    Batt_Output_Power = (BattPresentVolt * BattNowCurrent); // Ex. 12,000 mV * 2,500 mA = 30,000,000uW = 30W
    if( IS_MASK_SET(cCheatAD,cCAD_EN) && IS_MASK_SET(cCheatAD,cCAD_BATPWR))  //for debug
    {
       Chk_Hybrid_STPP_Batt_Output_Power = cCSTPPBATPWR;
    }else
    {
      if (OEM_Batt_Output_Power != Batt_Output_Power){
        Chk_Hybrid_STPP_Batt_Output_Power = (Batt_Output_Power * 0.00006826667) / ADPI_MUL; // The result almost equals 'ADC' value.
        OEM_Batt_Output_Power = Batt_Output_Power;
      }
    }
    
    // Ex. 30,000,000uW = 30W, (30W / 20V) = 1.5A -> 1.5*0.2=0.3v(ADP_I), 0.3v / 3v * 1024(step) = 102.4(ADC value)
    // Aadc = 30W * 68.2666667 / Vsys -> if Vsys=20V -> Aadc = 30 *68.26667 /20 = 102.4

    // System total power=ADP_I/0.4*20 + Battery total power
#if Hybrid_debug
      MpcSystemTotalPower = (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power) * 0.0146484375 *ADPI_MUL;//  (/ 1024 * 3 / 0.2 *Vsys);	//For Debug.
#endif //Hybrid_debug

    switch (MaxPwrCtrlFsm) {

      case SystemOverCP:
        if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcCP, &OEM_MpcCP)) {
          MAX_power_state = 0;
          //
          // System under CP:
          //
          // 1. Set CPU turbo on.
          // 2. Set GPU throttling off.
          // 3. Set CPU throttling off.
          // 4. Set charger turbo off.
          // 5. Resume battery charge.
          //
        
          CpuTurbo(MPC_ON);

#if (PLATFORM_DGPU == NVidia)
          GpuThrottling(MPC_OFF);
#endif

          DownGPUPState();
          DownBatteryPState();
#if (CHARGER_IC == BQ25700)
          if( IS_MASK_SET(CHGIC_ReadCmd0x31L,EN_IDCHG_REG) || IS_MASK_SET(CHGIC_ReadCmd0x30H,BIT7) )  
          {
            bRWSMBus(SMbusChB,SMbusRW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L, SMBus_NoPEC);
            CLEAR_MASK(CHGIC_ReadCmd0x31L,EN_IDCHG_REG);      //CLEAR_MASK(CHGIC_ReadCmd0x3BL,BIT2);  // BIT2  IDCHG_REG_EN
            bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L, SMBus_NoPEC);
            CLEAR_MASK(CHGIC_ReadCmd0x30H,BIT7);  // BIT15 IDCHG_EN

            if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L,SMBus_NoPEC))
            {
              CHGIC_SMbusFailCnt++;
            }
            if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L,SMBus_NoPEC))
            {
              CHGIC_SMbusFailCnt++;
            }
          }
#endif //BQ25700

#if (CHARGER_IC == BQ24780S)
          if( IS_MASK_SET(CHGIC_ReadCmd0x37H,EN_IDCHG_REG) || IS_MASK_SET(CHGIC_ReadCmd0x3BH,BIT3) )  
          {
            bRWSMBus(SMbusChB,SMbusRW, Charger_Addr, C_ChargeOption3, &CHGIC_ReadCmd0x37L, SMBus_NoPEC);
            CLEAR_MASK(CHGIC_ReadCmd0x37H,EN_IDCHG_REG);      //CLEAR_MASK(CHGIC_ReadCmd0x3BL,BIT2);  // BIT2  IDCHG_REG_EN
            bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargerMode3B, &CHGIC_ReadCmd0x3BL, SMBus_NoPEC);
            CLEAR_MASK(CHGIC_ReadCmd0x3BH,BIT3);  // BIT11 IDCHG_EN

            if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode3B, &CHGIC_ReadCmd0x3BL,SMBus_NoPEC))
            {
              CHGIC_SMbusFailCnt++;
            }
            if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption3, &CHGIC_ReadCmd0x37L,SMBus_NoPEC))
            {
              CHGIC_SMbusFailCnt++;
            }
          }
#endif //BQ25700

          BatteryCharge(MPC_ON);
          //break;
        } else {
          MAX_power_state = 1;
          // Step1. System is overing CP(charge current limit)
          // System is overing CP and battery charge is off, but not reached charger turbo point.
          // -> No case break here, go to next check point.
        }

      case SystemOverChargerTurboPoint_ChargerTurboOff:
        if ( ADPI_AvgData > WATT_TO_ADC_BASE((MpcCP * 1.07), &OEM_MpcCP_107) ) {
          MAX_power_state = 2;
          // Step2. System is over charger turbo point when turbo boost(AC+DC) is not actived.

          if (IsChargerTurboOn() == FALSE) {
            // -> start charger turbo, set turbo enable bit in ChargeOption().
            ChargerTurbo(MPC_ON);

            //break;
          } else {
            // System is overing ChargerTurboPoint and charger turbo is on.
            // -> No case break here, go to next check point. 
          }
        }

      case SystemOverChargerTurboPoint_ChargerTurboOn:

        //if ( (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power) >= WATT_TO_ADC_BASE((MpcCP * 1.07)) ) {
          MAX_power_state = 3;
          // Step3. Already started charger turbo, active some actions to reduce system power consumption.
          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power <= WATT_TO_ADC_BASE(MpcGpuThrottlingRecoverPoint, &OEM_MpcGpuThrottlingRecoverPoint)) {
            MAX_power_state = 5;
            // AC + DC < GPU throttling recover point
              CLEAR_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              DownGPUPState();
	
#if (PLATFORM_DGPU == NVidia)			  
              GpuThrottling(MPC_OFF);
#endif

          }
          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power <= WATT_TO_ADC_BASE(MpcCpuThrottlingRecoverPoint, &OEM_MpcCpuThrottlingRecoverPoint)) {
            MAX_power_state = 6;
            // AC + DC < CPU throttling recover point
              CpuThrottling(MPC_OFF);
          }
          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power >= WATT_TO_ADC_BASE(MpcGpuThrottlingPoint, &OEM_MpcGpuThrottlingPoint)) {
            MAX_power_state = 7;
            // AC + DC > GPU throttling enable
              SET_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              UpGPUPState();

#if (PLATFORM_DGPU == NVidia)	
              GpuThrottling(MPC_ON);
#endif

          }
          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power >= WATT_TO_ADC_BASE(MpcCpuThrottlingPoint, &OEM_MpcCpuThrottlingPoint)) {
            MAX_power_state = 9;
            // AC + DC > CPU throttling enable
            CpuThrottling(MPC_ON); // If system loading still over, then transform P1 to P2 or P3 or P4... etc.
          }
        //}

      default:
        MaxPwrCtrlFsm = SystemOverCP;
        break;

    } // switch (MaxPwrCtrlFsm)
  }
  else // if (Read_AC_IN() && !Read_ACOFF() && (RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge))
  {
//
// Apapter in + Battery(<=20%) in mode
//
    // Battery RSOC <= 20%
    // Clean all flag and status

//     Chk_Hybrid_STPP_Status = Chk_Hybrid_STPP_Status_CP;
    MaxPwrCtrlFsm = SystemOverCP;

//     CLEAR_MASK(nStopChgStat3H, HybridModeStopChg);
//     cBattFlag0.fbit.cCmdAcOff=0;       //Resume Charger
//     CLEAR_MASK(nStopChgStat3H, ENADPTHROTTLING);
    BatteryCharge (MPC_ON);

//     CLEAR_MASK(CHGIC_ReadCmd0x12L, TurboBoost);
    ChargerTurbo (MPC_OFF);

//     CLEAR_MASK(BatteryAlarm,DISTURBO);
//    CpuTurbo (MPC_ON);

    AdpCUTCnt = 0;
    // Consider Battery gasguage less then 20%
    if( Read_AC_IN() && !Read_ACOFF() )
    {
      // AC + DC(<=20%)
      Chk_AC_STPP(1);
    }
  } // end else // if (Read_AC_IN() && !Read_ACOFF() && (RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge))


//
// Battery only mode
//
  if(BattExistGet && (!Read_AC_IN() || Read_ACOFF())) {

    //
    // Battery RSOC condition is in function.
    //
    CpuTurbo(MPC_ON);

    // CPU/GPU throttling is polling by ChkBattery_OCP() in OEM_BATTERY.C
  }

//
// Code as below is doing battery RSOC process. We already processed it in CpuTurbo().
//

//   if( Read_AC_IN() && !Read_ACOFF() )
//   {
//     if( RelativeStateOfChgL > BATTURBOTEN )
//     {
// #if (PLATFORM_CHIP == INTEL)
//       CLEAR_MASK(BatteryAlarm,RSOCDISTURBO);
// #else
//       if( IS_MASK_CLEAR(pProject0,bRSOCAPUTURBO) )
//       {
//         ECSMI_SCIEvent(APU_DISTURBO);
//         SET_MASK(pProject0,bRSOCAPUTURBO);
//       }
// #endif
//     }
//     else if( RelativeStateOfChgL <= BATTRUBODIS )
//     {
// #if (PLATFORM_CHIP == INTEL)
//       SET_MASK(BatteryAlarm,RSOCDISTURBO);
//       cBATTThrottling |= 0x01;
// #else
//       if( IS_MASK_SET(pProject0,bRSOCAPUTURBO) )
//       {
//         ECSMI_SCIEvent(APU_ENTURBO);
//         CLEAR_MASK(pProject0,bRSOCAPUTURBO);
//       }
// #endif
//     }
//   }
//   else
//   {
// #if (PLATFORM_CHIP == INTEL)
//     CLEAR_MASK(BatteryAlarm,RSOCDISTURBO);
// #else
//     if( IS_MASK_SET(pProject0,bRSOCAPUTURBO) )
//     {
//       ECSMI_SCIEvent(APU_ENTURBO);
//       CLEAR_MASK(pProject0,bRSOCAPUTURBO);
//     }
// #endif
//   }
}

void Chk_AC_STPP( BYTE AcDcMode )
{
  BYTE BatteryAlarmFlag;
  BYTE pProject0Flag;
  //
  // AcDcMode -- 0: AC only,
  //             1: AC + DC(<=10%)
  //
  if (AcDcMode) {
    BatteryAlarmFlag = HybridSTPP;
    pProject0Flag = bHYBAPUTURBO;    
  } else {
    BatteryAlarmFlag = ACSTPP;
    pProject0Flag = bAPUTURBO;
  }
  
  if (Read_AC_IN())
  {
    CpuTurbo(MPC_ON);
    if (ADPI2Sec != 0 && !AcDcMode){
      ADPI2Sec--;
    }
    if (GPU2Sec != 0 && !AcDcMode){
      GPU2Sec--;
    }
    CLEAR_MASK(BatteryAlarm,ADPOUT);
    CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);

    if(AcDcMode){
      if (ADPI_AvgData >= WATT_TO_ADC_BASE(MpcCpuThrottlingPoint_L, &OEM_MpcCpuThrottlingPoint_L) )
      {
          SET_MASK(BatteryAlarm,BatteryAlarmFlag);
          UpBatteryPState();
        } 
      else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcCpuThrottlingRecoverPoint_L, &OEM_MpcCpuThrottlingRecoverPoint_L))
      {
          CLEAR_MASK(BatteryAlarm,BatteryAlarmFlag);
          DownBatteryPState();
        }

#if (PLATFORM_DGPU == AMD) 
      if(GPU2Sec == 0)
#endif
 	
	  {
    	  if (nSwitchableBTNGET) { // DIS
            if( ADPI_AvgData >= WATT_TO_ADC_BASE(MpcGpuThrottlingPoint_L, &OEM_MpcGpuThrottlingPoint_L)) {
              SET_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              UpGPUPState();
    //        GpuThrottling(MPC_ON);
            } else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcGpuThrottlingRecoverPoint_L, &OEM_MpcGpuThrottlingRecoverPoint_L) ){
              CLEAR_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              DownGPUPState();
    //        GpuThrottling(MPC_OFF);
          }
        }
      }
    }else{
      if (ADPI_AvgData >= WATT_TO_ADC_BASE(MpcAcCpuThrottlingPoint, &OEM_MpcAcCpuThrottlingPoint) )
      {
          SET_MASK(BatteryAlarm,BatteryAlarmFlag);
          UpBatteryPState();
        } 
      else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcAcCpuThrottlingRecoverPoint, &OEM_MpcAcCpuThrottlingRecoverPoint))
      {
          CLEAR_MASK(BatteryAlarm,BatteryAlarmFlag);
          DownBatteryPState();
        }

#if (PLATFORM_DGPU == AMD) 
      if(GPU2Sec == 0)
#endif
  	
	  	{
    	  if (nSwitchableBTNGET) { // DIS
            if( ADPI_AvgData >= WATT_TO_ADC_BASE(MpcAcGpuThrottlingPoint, &OEM_MpcAcGpuThrottlingPoint)) {
              SET_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              UpGPUPState();


#if (PLATFORM_DGPU == NVidia)			  
            GpuThrottling(MPC_ON);
#endif

            } else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcAcGpuThrottlingRecoverPoint, &OEM_MpcAcGpuThrottlingRecoverPoint) ){
              CLEAR_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              DownGPUPState();

#if (PLATFORM_DGPU == NVidia)			  
            GpuThrottling(MPC_OFF);
#endif
    
          }
        }
      }
    }
  }
}

void SetPowerBatteryparameter (void)
{
  {
    #if !Support_SMTADP
    AdapterID = AdapterID_90W;
    u4ACAdaptorCap = 90;
    AdpWatt = 90;	
    SET_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
    #endif
  }
#if (PLATFORM_CHIP == INTEL)
  if((u4ACAdaptorCap < LowerA45W_HIGH) && (u4ACAdaptorCap >= LowerA45W_LOW))
  {
    MpcCP                          = LowerA45W_CP;
    MpcCpuTurboDisablePoint        = LowerA45W_CPU_TURBO_BOOST_OFF_POINT;
    MpcCpuTurboRecoverPoint        = LowerA45W_CPU_TURBO_BOOST_ON_POINT;
    MpcCpuThrottlingPoint          = LowerA45W_CPU_THROTTLING_ON_POINT;
    MpcCpuThrottlingRecoverPoint   = LowerA45W_CPU_THROTTLING_OFF_POINT;
    MpcGpuThrottlingPoint          = LowerA45W_GPU_THROTTLING_ON_POINT;
    MpcGpuThrottlingRecoverPoint   = LowerA45W_GPU_THROTTLING_OFF_POINT;

    MpcCpuThrottlingPoint_L          = LowerA45W_CPU_THROTTLING_ON_POINT_L;
    MpcCpuThrottlingRecoverPoint_L   = LowerA45W_CPU_THROTTLING_OFF_POINT_L;
    MpcGpuThrottlingPoint_L          = LowerA45W_GPU_THROTTLING_ON_POINT_L;
    MpcGpuThrottlingRecoverPoint_L   = LowerA45W_GPU_THROTTLING_OFF_POINT_L;
	    
    MpcAcCpuTurboDisablePoint        = LowerA45W_AC_CPU_TURBO_BOOST_OFF_POINT;
    MpcAcCpuTurboRecoverPoint        = LowerA45W_AC_CPU_TURBO_BOOST_ON_POINT;
    MpcAcCpuThrottlingPoint          = LowerA45W_AC_CPU_THROTTLING_ON_POINT;
    MpcAcCpuThrottlingRecoverPoint   = LowerA45W_AC_CPU_THROTTLING_OFF_POINT;
    MpcAcGpuThrottlingPoint          = LowerA45W_AC_GPU_THROTTLING_ON_POINT;
    MpcAcGpuThrottlingRecoverPoint   = LowerA45W_AC_GPU_THROTTLING_OFF_POINT;
    
    Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
    Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W

    Chk_Trickle_Current_Now_Current = 700;//700mA
    Chk_Trickle_Current_Chg_Current = 500;//500mA

    ACACOFFEn = LowerA45W_ACOFF;
	ACCutoff_Watt = u4ACAdaptorCap;
  }
  else if (((AdapterID==AdapterID_45W) && IS14Inch) ||
     ((AdapterID >= AdapterID_45W) && (!(nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) )&& IS15Inch ) ) || 
     ((AdapterID == AdapterID_45W) && ((nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) ) && IS15Inch )) ||
     ((AdapterID == AdapterID_45W) && ((nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) ) && IS15Inch && N165_GTR )) ||
     (AdapterID==AdapterID_NON_SUPPORT)  || (AdapterID==AdapterID_Lower36W))
#else
  if ((AdapterID == AdapterID_45W) ||
      (AdapterID==AdapterID_NON_SUPPORT) || (AdapterID==AdapterID_36W) || (AdapterID==AdapterID_Lower36W))
#endif
  {
//     TurboBoostCP = A45WCP;
//     TurboBoostEn = A45Wchargerturbo;
//     TurboBoostthrottlEn = A45Wthrottling;
//     TurboBoostthrottlDis =A45Wthrottlingrecove;
//     TurboBoostDis = A45Wturborecover;
    MpcCP                          = A45W_CP;
    MpcCpuTurboDisablePoint        = A45W_CPU_TURBO_BOOST_OFF_POINT;
    MpcCpuTurboRecoverPoint        = A45W_CPU_TURBO_BOOST_ON_POINT;
    MpcCpuThrottlingPoint          = A45W_CPU_THROTTLING_ON_POINT;
    MpcCpuThrottlingRecoverPoint   = A45W_CPU_THROTTLING_OFF_POINT;
    MpcGpuThrottlingPoint          = A45W_GPU_THROTTLING_ON_POINT;
    MpcGpuThrottlingRecoverPoint   = A45W_GPU_THROTTLING_OFF_POINT;

    MpcCpuThrottlingPoint_L          = A45W_CPU_THROTTLING_ON_POINT_L;
    MpcCpuThrottlingRecoverPoint_L   = A45W_CPU_THROTTLING_OFF_POINT_L;
    MpcGpuThrottlingPoint_L          = A45W_GPU_THROTTLING_ON_POINT_L;
    MpcGpuThrottlingRecoverPoint_L   = A45W_GPU_THROTTLING_OFF_POINT_L;
	    
    MpcAcCpuTurboDisablePoint        = A45W_AC_CPU_TURBO_BOOST_OFF_POINT;
    MpcAcCpuTurboRecoverPoint        = A45W_AC_CPU_TURBO_BOOST_ON_POINT;
    MpcAcCpuThrottlingPoint          = A45W_AC_CPU_THROTTLING_ON_POINT;
    MpcAcCpuThrottlingRecoverPoint   = A45W_AC_CPU_THROTTLING_OFF_POINT;
    MpcAcGpuThrottlingPoint          = A45W_AC_GPU_THROTTLING_ON_POINT;
    MpcAcGpuThrottlingRecoverPoint   = A45W_AC_GPU_THROTTLING_OFF_POINT;
    
    Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
    Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W

    Chk_Trickle_Current_Now_Current = 700;//700mA
    Chk_Trickle_Current_Chg_Current = 500;//500mA

    ACACOFFEn = A45W_ACOFF;
	ACCutoff_Watt = 45;
  }
#if (PLATFORM_CHIP == INTEL)  
  else if(AdapterID==AdapterID_60W)
  {
    MpcCP                          = A60W_CP;
    MpcCpuTurboDisablePoint        = A60W_CPU_TURBO_BOOST_OFF_POINT;
    MpcCpuTurboRecoverPoint        = A60W_CPU_TURBO_BOOST_ON_POINT;
    MpcCpuThrottlingPoint          = A60W_CPU_THROTTLING_ON_POINT;
    MpcCpuThrottlingRecoverPoint   = A60W_CPU_THROTTLING_OFF_POINT;
    MpcGpuThrottlingPoint          = A60W_GPU_THROTTLING_ON_POINT;
    MpcGpuThrottlingRecoverPoint   = A60W_GPU_THROTTLING_OFF_POINT;
	
    MpcCpuThrottlingPoint_L          = A60W_CPU_THROTTLING_ON_POINT_L;
    MpcCpuThrottlingRecoverPoint_L   = A60W_CPU_THROTTLING_OFF_POINT_L;
    MpcGpuThrottlingPoint_L          = A60W_GPU_THROTTLING_ON_POINT_L;
    MpcGpuThrottlingRecoverPoint_L   = A60W_GPU_THROTTLING_OFF_POINT_L;

    MpcAcCpuTurboDisablePoint        = A60W_AC_CPU_TURBO_BOOST_OFF_POINT;
    MpcAcCpuTurboRecoverPoint        = A60W_AC_CPU_TURBO_BOOST_ON_POINT;
    MpcAcCpuThrottlingPoint          = A60W_AC_CPU_THROTTLING_ON_POINT;
    MpcAcCpuThrottlingRecoverPoint   = A60W_AC_CPU_THROTTLING_OFF_POINT;
    MpcAcGpuThrottlingPoint          = A60W_AC_GPU_THROTTLING_ON_POINT;
    MpcAcGpuThrottlingRecoverPoint   = A60W_AC_GPU_THROTTLING_OFF_POINT;

    Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
    Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W
    Chk_Trickle_Current_Now_Current = 700;//700mA
    Chk_Trickle_Current_Chg_Current = 500;//500mA

    ACACOFFEn = A60W_ACOFF;
	ACCutoff_Watt = 60;
  }
  else if(( AdapterID==AdapterID_65W && IS14Inch)||
          (( AdapterID>=AdapterID_65W) && ((nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) ) && IS15Inch && N165_GTR) ) ||
          (( AdapterID==AdapterID_65W) && ((nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) ) && IS15Inch && N16P_GT) ))
#else
  else if( (AdapterID>=AdapterID_65W) )
#endif
  {
//     TurboBoostCP = A65WCP;
//     TurboBoostEn = A65Wchargerturbo;
//     TurboBoostthrottlEn = A65Wthrottling;
//     TurboBoostthrottlDis =A65Wthrottlingrecove;
//     TurboBoostDis = A65Wturborecover;

    MpcCP                          = A65W_CP;
    MpcCpuTurboDisablePoint        = A65W_CPU_TURBO_BOOST_OFF_POINT;
    MpcCpuTurboRecoverPoint        = A65W_CPU_TURBO_BOOST_ON_POINT;
    MpcCpuThrottlingPoint          = A65W_CPU_THROTTLING_ON_POINT;
    MpcCpuThrottlingRecoverPoint   = A65W_CPU_THROTTLING_OFF_POINT;
    MpcGpuThrottlingPoint          = A65W_GPU_THROTTLING_ON_POINT;
    MpcGpuThrottlingRecoverPoint   = A65W_GPU_THROTTLING_OFF_POINT;
	
    MpcCpuThrottlingPoint_L          = A65W_CPU_THROTTLING_ON_POINT_L;
    MpcCpuThrottlingRecoverPoint_L   = A65W_CPU_THROTTLING_OFF_POINT_L;
    MpcGpuThrottlingPoint_L          = A65W_GPU_THROTTLING_ON_POINT_L;
    MpcGpuThrottlingRecoverPoint_L   = A65W_GPU_THROTTLING_OFF_POINT_L;

    MpcAcCpuTurboDisablePoint        = A65W_AC_CPU_TURBO_BOOST_OFF_POINT;
    MpcAcCpuTurboRecoverPoint        = A65W_AC_CPU_TURBO_BOOST_ON_POINT;
    MpcAcCpuThrottlingPoint          = A65W_AC_CPU_THROTTLING_ON_POINT;
    MpcAcCpuThrottlingRecoverPoint   = A65W_AC_CPU_THROTTLING_OFF_POINT;
    MpcAcGpuThrottlingPoint          = A65W_AC_GPU_THROTTLING_ON_POINT;
    MpcAcGpuThrottlingRecoverPoint   = A65W_AC_GPU_THROTTLING_OFF_POINT;

    Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
    Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W
    Chk_Trickle_Current_Now_Current = 700;//700mA
    Chk_Trickle_Current_Chg_Current = 500;//500mA

    ACACOFFEn = A65W_ACOFF;
	ACCutoff_Watt = 65;
  }
#if (PLATFORM_CHIP == INTEL)  
  else if( (AdapterID==AdapterID_90W) 
           || (AdapterID==AdapterID_135W) 
           || (AdapterID==AdapterID_170W) 
           || (AdapterID==AdapterID_230W) )
  {
      MpcCP                          = A90W_CP;
      MpcCpuTurboDisablePoint        = A90W_CPU_TURBO_BOOST_OFF_POINT;
      MpcCpuTurboRecoverPoint        = A90W_CPU_TURBO_BOOST_ON_POINT;
      MpcCpuThrottlingPoint          = A90W_CPU_THROTTLING_ON_POINT;
      MpcCpuThrottlingRecoverPoint   = A90W_CPU_THROTTLING_OFF_POINT;
      MpcGpuThrottlingPoint          = A90W_GPU_THROTTLING_ON_POINT;
      MpcGpuThrottlingRecoverPoint   = A90W_GPU_THROTTLING_OFF_POINT;
	  if(IS14Inch){
      MpcCpuThrottlingPoint_L          = A90W_CPU_THROTTLING_ON_POINT_14_L;
      MpcCpuThrottlingRecoverPoint_L   = A90W_CPU_THROTTLING_OFF_POINT_14_L;
      MpcGpuThrottlingPoint_L          = A90W_GPU_THROTTLING_ON_POINT_14_L;
      MpcGpuThrottlingRecoverPoint_L   = A90W_GPU_THROTTLING_OFF_POINT_14_L;
  	  }else{
      MpcCpuThrottlingPoint_L          = A90W_CPU_THROTTLING_ON_POINT_15_L;
      MpcCpuThrottlingRecoverPoint_L   = A90W_CPU_THROTTLING_OFF_POINT_15_L;
      MpcGpuThrottlingPoint_L          = A90W_GPU_THROTTLING_ON_POINT_15_L;
      MpcGpuThrottlingRecoverPoint_L   = A90W_GPU_THROTTLING_OFF_POINT_15_L;
	  }  

      MpcAcCpuTurboDisablePoint        = A90W_AC_CPU_TURBO_BOOST_OFF_POINT;
      MpcAcCpuTurboRecoverPoint        = A90W_AC_CPU_TURBO_BOOST_ON_POINT;
      MpcAcCpuThrottlingPoint          = A90W_AC_CPU_THROTTLING_ON_POINT;
      MpcAcCpuThrottlingRecoverPoint   = A90W_AC_CPU_THROTTLING_OFF_POINT;
      MpcAcGpuThrottlingPoint          = A90W_AC_GPU_THROTTLING_ON_POINT;
      MpcAcGpuThrottlingRecoverPoint   = A90W_AC_GPU_THROTTLING_OFF_POINT;

      Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
      Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W

      Chk_Trickle_Current_Now_Current = 700;//700mA
      Chk_Trickle_Current_Chg_Current = 500;//500mA

      ACACOFFEn = A90W_ACOFF;
	  ACCutoff_Watt = 90;

  }
#endif
}

void SetTurbo_V_Thro(void)
{
  if (AdapterID==AdapterID_95W || AdapterID==AdapterID_NON_SUPPORT)  //90W
      {
          CMP1THRDATM = (VC1TriggerValue_90W>>8);  // threshold value buffer
      CMP1THRDATL = VC1TriggerValue_90W;
      }
    else if (AdapterID==AdapterID_120W)      //120W
       {
            CMP1THRDATM = (VC1TriggerValue_120W>>8);    // threshold value buffer
        CMP1THRDATL = VC1TriggerValue_120W;
       }
       else if (AdapterID==AdapterID_170W)  //170W
          {
              CMP1THRDATM = (VC1TriggerValue_170W>>8);  // threshold value buffer
          CMP1THRDATL = VC1TriggerValue_170W;
          }
}

#if  Support_SMTADP
void CovertADPID(void)
{
#if Support_TYPE_C
  OEM_TYPE_C_ADPID_HOOK();
#endif //Support_TYPE_C

  if( !Read_AC_IN() || (!ADPID_ON_Detect()) )
  {
    return;
  }

#if Support_TYPE_C
	if(!Read_DCIN_ATTACHED2())
	{
		cADPIDTemp &= 0x0F;
		cADPIDTemp |= (TypeCAdpID<<4);
		if(CurrentTypeCV!=0)
		{
			ADPI_MUL = CurrentTypeCV *5/100;	//Type-C voltage Unit:50mV
		}
		else
		{
			ADPI_MUL = ClassicAdaptor_ADPI_MUL;
		}
	}
	else
#endif //Support_TYPE_C
	{
	  cADPIDTemp &= 0x0F;
	  if( (API_ID > ADP230W_Upper) || ( API_ID < ADP45W_Lower ) )
	  {
	    cADPIDTemp |= (AdapterID_NON_SUPPORT<<4);
	  }
	  else if( (API_ID >= ADP230W_Lower) )
	  {
	    cADPIDTemp |= (AdapterID_230W<<4);
	  }
	  else if( (API_ID >= ADP170W_Lower) )
	  {
	    cADPIDTemp |= (AdapterID_170W<<4);
	  }
	  else if( (API_ID >= ADP135W_Lower) )
	  {
	    cADPIDTemp |= (AdapterID_135W<<4);
	  }
	  else if( API_ID >= ADP90W_Lower )
	  {
	    cADPIDTemp |= (AdapterID_90W<<4);
	  }
	  else if( API_ID >= ADP65W_Lower )
	  {
	    cADPIDTemp |= (AdapterID_65W<<4);
	  }
	  else if( API_ID >= ADP45W_Lower )
	  {
	    cADPIDTemp |= (AdapterID_45W<<4);
	  }
    ADPI_MUL = ClassicAdaptor_ADPI_MUL;
	} 

  ADP_ID_REC = API_ID; // Log the raw ADP_ID value for debug tracing.

  if( IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeADPIDReady) )
  {
    if( AdapterID == (cADPIDTemp>>4) )
    {
      cADPIDTemp++;
    }
    else
    {
      AdapterID = (cADPIDTemp>>4);
      cADPIDTemp &= 0xF0;
      CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
    }
    if( (cADPIDTemp&0x0F) <= 4 )
    {
      return;
    }
    else
    {
      switch(AdapterID)
      {
#if Support_TYPE_C
        case AdapterID_Lower36W:
          u4ACAdaptorCap = CurrentTypeCWatt;
          AdpWatt = CurrentTypeCWatt;
          break;
#endif  //Support_TYPE_C          
        case AdapterID_36W:
          u4ACAdaptorCap = 36;
          AdpWatt = 36;
          break;
        case AdapterID_45W:
          u4ACAdaptorCap = 45;
          AdpWatt = 45;          
          //AttachedPowerNOTsupportEn;
          break;
        case AdapterID_60W:
          u4ACAdaptorCap = 60;
          AdpWatt = 60;
          break;
	    case AdapterID_65W:
          u4ACAdaptorCap = 65;
          AdpWatt = 65;
          break;
        case AdapterID_90W:
          u4ACAdaptorCap = 90;
          AdpWatt = 90;
          break;
        case AdapterID_135W:
          u4ACAdaptorCap = 135;
          AdpWatt = 135;
          break;
        case AdapterID_170W:
          u4ACAdaptorCap = 170;
          AdpWatt = 170;
          break;
        case AdapterID_230W:
          u4ACAdaptorCap = 230;
          AdpWatt = 230;
          break;
        case AdapterID_NON_SUPPORT:
          AttachedPowerNOTsupportEn;
          u4ACAdaptorCap = 0;
          AdpWatt = 0;
          break;
      }
      SET_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
      Update_ChargeIC();        // [20140304 ITE CM.Wu]
      SetPowerBatteryparameter();
      SET_MASK(pEdgeProjectFlag2,pEdgeACINOUTTrigger);
//[-start-131213-T130124_9-add]//
      ADPID_ON_DIS();
//[-end-131213-T130124_9-add]//
    }
  }
}

void Edge_SMTADPProcess()
{
  #if !Support_MAXPWR
  return;
  #endif

  //
  // !! Attention !!
  // Code in here is for Edge Lonestar&Gordon specification only.
  // Required: 45W adapter is limited support in 65W shipped system(CPU 15W + dGPU 15~25W)
  // Limited Support: CPU Frequency is set to LFM under AC Adapter only(When battery capacity under 10%).
  //
  //Check System design watt and setting the limited support watt adaptor.
#if Support_TYPE_C
	if(!Read_DCIN_ATTACHED2() && Read_AC_IN())
	{
		if(BattExistGet && (RelativeStateOfChgL >= TypeCLimitedSupport_BatteryRSOC))
		//if(BattExistGet && (IsaacDebug01 >= TypeCLimitedSupport_BatteryRSOC))
		{
		  if(nSwitchableBTNGET)
		  {	//DIS is 65/90 watt system
		#if (PLATFORM_CHIP == INTEL)
		    if (N16P_GT){
		      	LimitedSupportAdpWatt = A90W_System_Limited_support_TypeC_Adapter;
		  	  	NotSupportAdpWatt = 0;	//No not-support adaptor				
				UnknowAdpWatt = A90W_System_Unknow_TypeC_Adapter;

		    }else
		#endif
		    {
		  	  	LimitedSupportAdpWatt = A65W_System_Limited_support_TypeC_Adapter;
		  	  	NotSupportAdpWatt = 0;	//No not-support adaptor		  	  	
				UnknowAdpWatt = A65W_System_Unknow_TypeC_Adapter;

		    }
		  }
		  else
		  {	//UMA is 45 watt system.
		  		LimitedSupportAdpWatt = A45W_System_Limited_support_TypeC_Adapter;
		  		NotSupportAdpWatt = 0;	//No not-support adaptor		  		
				UnknowAdpWatt = A45W_System_Unknow_TypeC_Adapter;

		  }
		}
		else
		{
		  if(nSwitchableBTNGET)
		  {	//DIS is 65/90 watt system
		#if (PLATFORM_CHIP == INTEL)
		    if (N16P_GT){
		      	LimitedSupportAdpWatt = A90W_System_Limited_support_TypeC_Adapter;
		  	  	NotSupportAdpWatt = A90W_System_Notsupport_TypeC_Adapter;
				UnknowAdpWatt = 0;
		    }else
		#endif
		    {
		  	  	LimitedSupportAdpWatt = A65W_System_Limited_support_TypeC_Adapter;
		  	  	NotSupportAdpWatt = A65W_System_Notsupport_TypeC_Adapter;
				UnknowAdpWatt = 0;
		    }
		  }
		  else
		  {	//UMA is 45 watt system.
		  		LimitedSupportAdpWatt = A45W_System_Limited_support_TypeC_Adapter;
		  		NotSupportAdpWatt = A45W_System_Notsupport_TypeC_Adapter;
				UnknowAdpWatt = 0;
		  }
		}
	}
	else
  	{
		LimitedSupportAdpWattUpper = 0;
#endif //Support_TYPE_C
  		if(nSwitchableBTNGET)
  		{	//DIS is 65 watt system
#if (PLATFORM_CHIP == INTEL)
    		if (N16P_GT){
      			LimitedSupportAdpWatt = A90W_System_Limited_support_Adapter;
  	  			NotSupportAdpWatt = A90W_System_Not_support_Adapter;
    		}else
#endif
    		{
  	  			LimitedSupportAdpWatt = A65W_System_Limited_support_Adapter;
  	  			NotSupportAdpWatt = A65W_System_Not_support_Adapter;
    		}
  		}
  		else
  		{	//UMA is 45 watt system.
  			LimitedSupportAdpWatt = A45W_System_Limited_support_Adapter;
  			NotSupportAdpWatt = A45W_System_Not_support_Adapter;
  		}
	}
  if(((AdpWatt==LimitedSupportAdpWatt)
#if Support_TYPE_C  	
  	&& Read_DCIN_ATTACHED2())
  	|| (((AdpWatt < LimitedSupportAdpWatt) && (AdpWatt >= NotSupportAdpWatt)) && !Read_DCIN_ATTACHED2())
#endif //Support_TYPE_C
  	) 
    { //dGPU is equipped which means it is 65W system.
    if (BattExistGet) {  //battery exist
      if (RelativeStateOfChgL <= BAT_HYBOOST_DIS) {
        //
        // We use b2ProCH_EXTVGA to turn on prochot.
        //
//        SET_MASK(pEdgeProjectFlag2, pEdgeGPUOCTHROT); // dGPU throttling
        SET_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
        SET_MASK(Thro_Status, b2ProCH_EXTVGA);        // CPU throttling
	      SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
        cBATTThrottling |= CPUMAXPSTATE;
        cGPUThrottling = GPUMAXPSTATE;
      }  else if( RelativeStateOfChgL > BAT_HYBOOST_EN ) {
//        CLEAR_MASK(pEdgeProjectFlag2, pEdgeGPUOCTHROT);
        if(IS_MASK_SET(Thro_Status,b2ProCH_EXTVGA)){
          cBATTThrottling = 0;
        }
        CLEAR_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
        CLEAR_MASK(Thro_Status, b2ProCH_EXTVGA);
				CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
      }
    } else { // battery detach, AC only mode.
//      SET_MASK(pEdgeProjectFlag2, pEdgeGPUOCTHROT);
      SET_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
      SET_MASK(Thro_Status, b2ProCH_EXTVGA);
	    SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
      SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
      cBATTThrottling |= CPUMAXPSTATE;
      cGPUThrottling = GPUMAXPSTATE;
    }
  }
  else if(IS_MASK_SET(uProject0,NeedRecoveryLimitedSupportADP))
  {
    if(IS_MASK_SET(Thro_Status,b2ProCH_EXTVGA)){
      cBATTThrottling = 0;
    }
    CLEAR_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
    CLEAR_MASK(Thro_Status, b2ProCH_EXTVGA);
		CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
    CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
  }

//  return;

  if(( IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady) && Read_DCIN_ATTACHED2()) //classic
#if Support_TYPE_C
  	|| (((AdpWatt < UnknowAdpWatt)||(AdpWatt == 0)) && !Read_DCIN_ATTACHED2())
#endif //Support_TYPE_C
  	)
	{
		  	
	#if (PLATFORM_CHIP == AMD)
    if( nSwitchableBTNGET )
	#endif
		
    {	// 65W system
      if( (AdapterID==AdapterID_NON_SUPPORT)||  (AdapterID == AdapterID_Lower36W) )
      {	// Unknow adaptor
	  	
		#if (PLATFORM_CHIP == AMD)      
        AttachedPowerNOTsupportEn;
		#endif

        if(( SystemIsS0 || (AdapterID==AdapterID_NON_SUPPORT)||  (AdapterID==AdapterID_Lower36W) ) && BattExistGet)
        {
          //SET_MASK(ACOFF_SOURCE, AIRADP);
          CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
        //  ACOFF_HI();
        }
        else
        {
          CLEAR_MASK(ACOFF_SOURCE, AIRADP);
          if( Read_AC_IN() )
          SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
          ACOFF_LOW();
        }
        SET_MASK(pEdgeProjectFlag2,pEdgeNonSUPPORTADP);
      }
      else
      {	//Normal support adaptor
	  	
		#if (PLATFORM_CHIP == AMD)
        AttachedPowerNOTsupportDis;
		#endif

        CLEAR_MASK(pEdgeProjectFlag2,pEdgeNonSUPPORTADP);
		  	
	#if (PLATFORM_CHIP == AMD)
      }
    }
    else
    {	// 45W system
      if(  SystemNotS0 && (AdapterID==AdapterID_NON_SUPPORT) )
      {	// S3/S4/S5 + unknow adaptor
        SET_MASK(nStopChgStat3L,ENOVERVOLTAGE);
      }
      else if( SystemIsS0 && IS_MASK_SET(SYS_MISC1,ACPI_OS) && (AdapterID==AdapterID_NON_SUPPORT) && BattExistGet)
      {
          //SET_MASK(ACOFF_SOURCE, AIRADP);
          CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
         // ACOFF_HI();
      }
      // else
      {
        if( !ACOFF_SOURCE || (SystemNotS0 && (AdapterID!=AdapterID_NON_SUPPORT)) )
        {
          CLEAR_MASK(ACOFF_SOURCE, AIRADP);
          if( Read_AC_IN() )
            SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
          ACOFF_LOW();
          CLEAR_MASK(pEdgeProjectFlag2,pEdgeNonSUPPORTADP);
        }
	#endif
	
      }
    }
	    if( (AdpWatt < NotSupportAdpWatt) && (AdpWatt > 0) )
	    {	// Not-support adaptor
	      AttachedPowerNOTsupportEn;
	      if( SystemIsS0 && BattExistGet )
	      {	// "Battery exist + S0" set to DC only mode
	       // SET_MASK(ACOFF_SOURCE, AIRADP);
	        CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
	       // ACOFF_HI();
	      }
	      else
	      { //S3/S4/S5 need charging
	        CLEAR_MASK(ACOFF_SOURCE, AIRADP);
	        if( Read_AC_IN() )
	        {
	        	SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
	        }
	        ACOFF_LOW();
	      }
	      SET_MASK(pEdgeProjectFlag2,pEdgeNonSUPPORTADP);
	      SET_MASK(uProject0,NeedRecoveryNotSupportADP);
	    }
	    else if((AdpWatt != 0) && IS_MASK_SET(uProject0,NeedRecoveryNotSupportADP))
	    {	//Normal support adaptor
	      AttachedPowerNOTsupportDis;
	      CLEAR_MASK(pEdgeProjectFlag2,pEdgeNonSUPPORTADP);
	        CLEAR_MASK(ACOFF_SOURCE, AIRADP);
	        if(!ACOFF_SOURCE)
	        {
		        if( Read_AC_IN() )
		        {
		        	SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
		        }
	        	ACOFF_LOW();
	        }
	      CLEAR_MASK(uProject0,NeedRecoveryNotSupportADP);
	    }
     }
}

#endif

void  ScanADC_BoardID(void)
{
#if Support_ADC_BoardID
  WORD BoardADCID;
  BYTE i = 0;

  if(BoardID_readyGET)
    return;
  
  if( IS_MASK_SET(VCH3CTL, BIT7) )
  {
    VCH3CTL |= 0x80;        // write clear data vaild flag
    BoardADCID = ((((WORD)VCH3DATM << 8) + VCH3DATL)*(0.92));

    if(BoardADCID <= BoardADC_LV0)
    {
      BoardID = SDV_board;//SDV
    }   
    else if((BoardADCID > BoardADC_LV0)&& (BoardADCID <= BoardADC_LV1))
    {
      BoardID = Board_ID1;//RESERVE1
    } 
    else if((BoardADCID > BoardADC_LV1)&& (BoardADCID <= BoardADC_LV2))
    {
      BoardID = FVT_board;//FVT
    }
    else if((BoardADCID > BoardADC_LV2)&& (BoardADCID <= BoardADC_LV3))
    {
      BoardID = SIT_board;//SIT
    }
    else if((BoardADCID > BoardADC_LV3)&& (BoardADCID <= BoardADC_LV4))
    {
      BoardID = Board_ID2;//RESERVE2
    }
    else if(BoardADCID > BoardADC_LV4)
    {
      BoardID = SVT_board;//SVT
    }
    BoardID_readyEN;
    }
#endif  
}


void CPUtuborONOFF(BYTE OnOff) 
{
  if (OnOff == Enable){
    cADPThrottling = 0x00;
    CLEAR_MASK(GPUAlarm,GPUAlarm_TubONOFF);
    DownGPUPState();
  }else{             
    SET_MASK(GPUAlarm,GPUAlarm_TubONOFF);
		
  #if (PLATFORM_CHIP == INTEL)  
    if(pCPUBasePStage != 0){
      cADPThrottling = CPUturboDisPstage; 
    }
  #else  //AMD
    cADPThrottling = CPUturboDisPstage_PWR; 
  #endif
	
  }
}

WORD WATT_TO_ADC_BASE(WORD IN_DATA, XWORD *OUT_DATA){
  if (*OUT_DATA == 0xFFFF){
     *OUT_DATA = ((XWORD)IN_DATA * 68.26667) / ADPI_MUL;
  }
  return *OUT_DATA;
}