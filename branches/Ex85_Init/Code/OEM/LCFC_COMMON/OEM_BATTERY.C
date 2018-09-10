/*-----------------------------------------------------------------------------
 * TITLE: OEM_BATTERY.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//(JSN-1001)+
void UpdateBatteryLevel (void)
{
//
// Update battery level
//
  BYTE BatteryLevel;

  BatteryLevel = 0;

  if( RelativeStateOfChgL >= 80 )
  {
//     PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
//     PriBattSTS |= BATMAXLVL;
    //(TBD)TWR01.bit.BatteryCritical = 0;
    BatteryLevel = SBC_BATTERY_LEVEL_HIGH;
    CLEAR_MASK(TWR01,BatteryCritical);
  }
  else if( (RelativeStateOfChgL >= 20) && (RelativeStateOfChgL < 80) )
  {
//     PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
//     PriBattSTS |= (BATMAXLVL-1);
    BatteryLevel = SBC_BATTERY_LEVEL_MIDDLE_HIGH;
    CLEAR_MASK(TWR01,BatteryCritical);
    if( IS_MASK_CLEAR(pEdgeProjectFlag2,pEdgeBATTOver20) )
    {
      SET_MASK(pEdgeProjectFlag2,pEdgeBATTOver20);
      CLEAR_MASK(pEdgeProjectFlag2,pEdgeBATTUnder20);
      ECSMI_SCIEvent(LLB_22);
    }
  }
  else if( (RelativeStateOfChgL >= 5) && (RelativeStateOfChgL < 20) )
  {
//     PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
//     PriBattSTS |= (BATMAXLVL-2);
    BatteryLevel = SBC_BATTERY_LEVEL_MIDDLE;
    CLEAR_MASK(TWR01,BatteryCritical);
    if( IS_MASK_CLEAR(pEdgeProjectFlag2,pEdgeBATTUnder20) )
    {
      SET_MASK(pEdgeProjectFlag2,pEdgeBATTUnder20);
      CLEAR_MASK(pEdgeProjectFlag2,pEdgeBATTOver20);
      ECSMI_SCIEvent(LLB_22);
    }

  }
  else if( (RelativeStateOfChgL < 5) && (RemainCapL  >= 30) )
  {
//     PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
//     PriBattSTS |= (BATMAXLVL-3);
    BatteryLevel = SBC_BATTERY_LEVEL_LOW;
    CLEAR_MASK(TWR01,BatteryCritical);
  }
  else if( (RemainCapL  < 30) && (RemainCapL  >= 20) )
  {
    // remaining capacity
    if( IS_MASK_CLEAR(TWR01,BatteryCritical) )
    {
      ECQEvent(LLB_22);
    }
//     PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
//     PriBattSTS |= (BATMAXLVL-4);
    BatteryLevel = SBC_BATTERY_LEVEL_WARNING;
    SET_MASK(TWR01,BatteryCritical);
  }
  else if( (RemainCapL  < 20) && (RemainCapL  > 0) )
  {
//     PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
//     PriBattSTS |= (BATMAXLVL-5);
    if( IS_MASK_CLEAR(TWR01,BatteryCritical) )
    {
      ECQEvent(LLB_22);
    }
    BatteryLevel = SBC_BATTERY_LEVEL_CRITICAL_LOW;
    SET_MASK(TWR01,BatteryCritical);
  }
  else
  {
//     PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
    if( IS_MASK_CLEAR(TWR01,BatteryCritical) )
    {
      ECQEvent(LLB_22);
    }
    BatteryLevel = SBC_BATTERY_LEVEL_EMPTY;
    SET_MASK(TWR01,BatteryCritical);
  }

  /*  (From TPG sbcomm.c Update_Battery_Status_In_Interface_Space() )
  *   leave level as 0 if battery level is less than low
  *   and adjust it start from 1 for low level.
  */
  PriBattSTS &= ~(BIT2 | BIT1 | BIT0);
  if (SBC_BATTERY_LEVEL_LOW <= BatteryLevel) {
    if (BM_MAX_LEVEL < (BatteryLevel - SBC_BATTERY_LEVEL_LOW + 1)) {
      PriBattSTS |= BM_MAX_LEVEL;
    } else {
      PriBattSTS |= (BatteryLevel - SBC_BATTERY_LEVEL_LOW + 1);
    }
  }
  

}

void UpdateEdgeNameSpace(void)
{
  BYTE i,j;
//    if(Edge_NAMESPACE_SW!=1)
//    {
  if(BattExistGet || IS_MASK_SET(EVT_STATUS1,bat_in))  // Battery1 exists
  {
    //(TBD) determine whether battery2 exists too

    if( PriBattInhibGET )
    {
      BattExistDis;
      MainBattAttachedDis;
      if( IS_MASK_CLEAR(pEdgeProjectFlag2,pEdgeProjectBTFWUP) )
      {
        CurrentL = 0;
        CurrentH =0 ;
        Bat0x0BFakeCnt = 0;
        nBattErrorCnt = 0;
        SET_MASK(pEdgeProjectFlag2,pEdgeProjectBTFWUP);
        SET_MASK(BATTUPDATEFW,PriBattInhib);
      }
    }
    else
    {
      MainBattAttachedEn;
      BattExistEn;
      if( IS_MASK_SET(pEdgeProjectFlag2,pEdgeProjectBTFWUP) )
      {
        CLEAR_MASK(pEdgeProjectFlag2,pEdgeProjectBTFWUP);
        FirstGetBatData();
        Service_Auth_Step = 1;        // start AUTH
        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
        CLEAR_MASK(BATTUPDATEFW,PriBattInhib);
      }
    }


    if( p2BattExistTempGet != PriBattInhibGET )
    {
      //p2IBMChargeflag.bit.p2BattExistTemp = PriBattInhibGET;
      if(PriBattInhibGET)
      {
      	pFKTemp = 0;
      	Batt0x3CFailRetry5sTimer = 0;
        p2BattExistTempEn;
      }
      else
      {
        p2BattExistTempDis;
      }

    SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);
    }

    //MainBattAttachedEn; //TWR00.bit.MainBattAttached = 1;

  UpdateBatteryLevel();


//
// Update battery charge/discharge status
//

//            if( !Read_AC_IN() || Read_ACOFF())
    if( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) || Read_ACOFF())
    {
        SET_MASK(nBatteryStatH, CMBS_DISCHARGE);
        CLEAR_MASK(nBatteryStatH, CMBS_CHARGE);
    }

    else if ((IS_MASK_CLEAR(CurrentH, BIT7) && ((CurrentH > 0) || (CurrentL > 0))) // In charging and current > 0
          || (IS_MASK_CLEAR(BattStatusL,FullyChg) && (pFakeCHGCnt != 0) && IS_MASK_SET(SYS_STATUS,AC_ADP))) //)
    {
      SET_MASK(nBatteryStatH, CMBS_CHARGE);
      CLEAR_MASK(nBatteryStatH, CMBS_DISCHARGE);
      //(TBD)CLEAR_MASK(nBatteryStatL, CMBS_LOWBATT);
      //(TBD)CLEAR_MASK(nBatteryStatH, CMBS_CRITICALLOW);
    }
   // else
   // {
     // nBatteryStatH &= 0x80;
     // SET_MASK(nBatteryStatH, CMBS_DISCHARGE);
     // CLEAR_MASK(nBatteryStatH, CMBS_CHARGE);
   // }

//
// Charge threshold
//

//
// If pInhitTimer != 0, battery can be inhibit charged.
// If pInhitTimer == 0, battery should be charged immediately.
//
    if( RelativeStateOfChgL <= pEdgeBATStartCharge )
    {
      if (pInhitTimer == 0x00) {
        PriBattinhibitDis;
      }
      CLEAR_MASK(nStopChgStat3H, PMTHRESHOLD);
    }
    else if( RelativeStateOfChgL >= pEdgeBATStopCharge )
    {
      if (pInhitTimer != 0x00) {
        PriBattinhibitEn;
      }
      SET_MASK(nStopChgStat3H, PMTHRESHOLD);
      // TODO: Is it need to discharge? Wait to sync with TPG projects.
    }


//
// Set Charge/Discharge status into TWR register.
// If Charge and Dischage both set '0', it means "No Activity".
//
    if( nBatteryStatH & CMBS_CHARGE )  // Battery Charging
    {
      ChargingEn;
      DisChgDis;
      //BattErrorDis;
      SET_MASK(TWR01,Charge);
      CLEAR_MASK(TWR01,Discharge);
      if(IS_MASK_SET(pProject3,uReSendBat1ChgQevent) && (inhibit2sec == 0)){
        CLEAR_MASK(pProject3,uReSendBat1ChgQevent);
        SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATChange);
      }

    }
    else
    {
      ChargingDis;
      //BattErrorDis;
      CLEAR_MASK(TWR01,Charge);
    }
    if( nBatteryStatH & CMBS_DISCHARGE )  // Battery Discharge
    {
      DisChgEn;
      ChargingDis;
      //BattErrorDis;
      CLEAR_MASK(TWR01,Charge);
      if ((IS_MASK_SET(BattStatusL,FullyChg) || ( !Read_ACOFF() && (nStopChgStat3L | nStopChgStat3H) ))
        &&(IS_MASK_SET(SYS_STATUS,AC_ADP))
        )
      {
//         if ((nStopChgStat3H & PMTHRESHOLD) != PMTHRESHOLD) {
          CLEAR_MASK(TWR01,Discharge);
//         }
      }
      else
      {
        SET_MASK(TWR01,Discharge);
      }

      if(
          ( RelativeStateOfChgL > pEdgeBATStartCharge ) 
          && ( RelativeStateOfChgL <= pEdgeBATStopCharge )
          && (IS_MASK_SET(SYS_STATUS,AC_ADP))
        )
      {
        CLEAR_MASK(TWR01,Discharge);
      }
    }
    else
    {
      DisChgDis;
      //BattErrorDis;
      CLEAR_MASK(TWR01,Discharge);
    }
  }
  else    // Battery not exist
  {
    PriBattSTS = 0x00;
    MainBattAttachedDis;    //TWR00.bit.MainBattAttached = 0
    BattExistDis;
  }

  if (IS_MASK_SET(nStopChgStat3L,ENCOMMFAIL))
  {
    if(IS_MASK_CLEAR(PriBattSTS,BattError))
    {
      CommestablishedDis;
    }
  }
  else
    CommestablishedEn;

  //Mos: copy battery data that we don't collect, It's should be fix at future.
  if(BattExistGet)  // Battery1 exists
  {
    Major = TPGPM_Major;
    Minor = TPGPM_Minor;
    if((ManufactureAccessL & 0x03)!= 0x00)
    {
      BattErrorEn;
    }
    else
    {
      BattErrorDis;
    }
  }

  //SET_MASK(BattAuthStat,OEMauthPass);
  //SET_MASK(BattAuthStat,SWauthPass);
  if (Batteryin_delay !=0 )
  {
    Batteryin_delay -- ;
    if (BattExistGet)
    return;
  }

  if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeProjectBATIn) )
  {
    IBMbatteryStore();  // update info
    CLEAR_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);
    ECSMI_SCIEvent(ACPI_BAT0IN_SCI);
  }
  else if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeProjectBATChange) )
  {
    IBMbatteryStore();  // update info
    CLEAR_MASK(pEdgeProjectFlag1,pEdgeProjectBATChange);
    ECSMI_SCIEvent(ACPI_BAT0CHG_SCI);
  }

  if( PriBattDisChgGet )
  {
    if( ( RelativeStateOfChgL <= 1 ) && IS_MASK_SET(pBattLearn,uRefreshmode) )
    {
      CLEAR_MASK(pBattLearn,uACoffstchg);
      SET_MASK(pBattLearn,uACoffdschg);
		  if( Read_BATLEARN() )
		  {
      	BAT_LEARN_OFF();
		  }
		  if( nAcOffCmdGET )
		  {
		  	BATLEARNProcess(nAcOffCmdDIS);
		  }
      CLEAR_MASK(nStopChgStat3L,ENCUSTOMER);
    }
    else if( IS_MASK_SET(BattStatusL,FullyChg) && IS_MASK_SET(pBattLearn,uACoffdschg) )
    {
      if( Read_BATLEARN() )
		  {
      	BAT_LEARN_OFF();
		  }
		  if( nAcOffCmdGET )
		  {
		  	BATLEARNProcess(nAcOffCmdDIS);
		  }
      CLEAR_MASK(pBattLearn,uACoffstchg);
      CLEAR_MASK(pBattLearn,uACoffdschg);
      PriBattDisChgDis;
      CLEAR_MASK(pBattLearn,uRefreshmode);
      CLEAR_MASK(nStopChgStat3L,ENCUSTOMER);
    }
  }
//    }
}

//(JSN-1001)-

void UpdateNameSpace(void)
{
  nBattTsVolt = (BYTE)(BATTEMP>>2);
  if (IS_MASK_SET(BattStatusL,FullyChg))
    {

        if(RelativeStateOfChgL == 101) // 100%
        {
            if(IS_MASK_CLEAR(LENOVOPMFW_Temp,BATTERY_FULLED_100))
            {
                SET_MASK(LENOVOPMFW_Temp,BATTERY_FULLED_100);
                //ECSMI_SCIEvent(ACPI_BAT0IN_SCI); // Notify BIOS Update Batt Status
                SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);
            }
        }
    }
  else
    {
        CLEAR_MASK(LENOVOPMFW_Temp,BATTERY_FULLED_100);
    if( (pFakeCHGCnt != 0) && IS_MASK_SET(SYS_STATUS,AC_ADP) )
    {
        CLEAR_MASK(BattStatusL,Dsg);
    }
  }


    // Reset Battery Protection Status when AC off or battery plugout
    if (IS_MASK_SET(StatusKeeper, BatteryProtectCHG))
    {
        if (!Read_AC_IN())
            CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
    }

  if (IS_MASK_SET(LENOVOPMFW,BATTERY_CYCLE) && IS_MASK_CLEAR(LENOVOPMFW,BATTERY_MAIN_CAL))
  {
    // return RSOC(0x0d)=0 to BIOS if battery error
    if ((ChkBattery_abnormal_status == ChkBattery_abnormal_status_error)
      || (Chk_Trickle_Current_status == Chk_Trickle_Current_status_error))
    {
      //RemainCapL = 0;
      //RemainCapH = 0;
      // Disable report RSOC(0x0d) = 0 to BIOS, need to modify spec
      
//      RemainCapH = Bat0x0FTempH;
    }
    else
    {

//      RemainCapH = Bat0x0FTempH;
    }

        SET_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);

    if (RelativeStateOfChgL > 60)
    {
      SET_MASK(ACOFF_SOURCE, BATTLEARN);
	  if( !Read_BATLEARN() )
	  {
	  	BAT_LEARN_ON();
	  }
   }
    else if (RelativeStateOfChgL >= 55 && RelativeStateOfChgL <= 60)
    {
      if ( IS_MASK_SET(ACOFF_SOURCE, BATTLEARN) )
      {
        //ECSMI_SCIEvent(ACPI_BAT0IN_SCI);
        SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);
      }
      CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
	  if( Read_BATLEARN() )
	  {
	  	BAT_LEARN_OFF();
	  }
      // Change Status bit to avoid bit has been reset
            if (IS_MASK_CLEAR(StatusKeeper, BatteryProtectCHG))
            {
                SET_MASK(nStopChgStat3H, EmStopChgarg);
            }
            else
            {
                CLEAR_MASK(nStopChgStat3H, EmStopChgarg);
                if (RelativeStateOfChgL == 60)
        {
          if (KeepBattRemineCap == 0)
          { KeepBattRemineCap = (WORD)(RemainCapH << 8) + RemainCapL; }
          else
          {
            // Avoid that stop charger but OS show god damn 59%...
            if (((WORD)(RemainCapH << 8) + RemainCapL) > (KeepBattRemineCap + 10))
            {
              KeepBattRemineCap = 0;
              CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
              //ECSMI_SCIEvent(ACPI_BAT0IN_SCI); // Notify BIOS Update Batt Status
              SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);
            }
          }
        }
            }
    }
    else if (RelativeStateOfChgL < 55)
    {
      CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
	  if( Read_BATLEARN() )
	  {
	  	BAT_LEARN_OFF();
	  }
      // Change Status bit to avoid bit has been reset
            SET_MASK(StatusKeeper, BatteryProtectCHG);
      CLEAR_MASK(nStopChgStat3H,EmStopChgarg);
    }
  }
  else
  {
    if (IS_MASK_SET(BattStatusL,FullyChg)&&(IS_MASK_SET(SYS_STATUS,AC_ADP)))
    {
//			RemainCapH = Bat0x0FTempH;
    }
    else
    {
      // return RSOC(0x0d)=0 to BIOS if battery error
      if (ChkBattery_abnormal_status == ChkBattery_abnormal_status_error)
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        RemainCapL = 0;
        RemainCapH = 0;
      }
      else
      {
//        RemainCapH = Bat0x0FTempH;
      }
    }

        if(IS_MASK_SET(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING))
        {
            CLEAR_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);

            if(IS_MASK_SET(ACOFF_SOURCE,BATTLEARN))
            {
                CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
				if( Read_BATLEARN() )
		  		{
          			BAT_LEARN_OFF();
		  		}
            }
        }

    // Clear BATTERY_CYCLE_fulchg when change to Longest Battery Mode
        CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
    CLEAR_MASK(nStopChgStat3H,EmStopChgarg);
  }

}

void ChkLENOVOPMFW(void)
{
  if (IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL))      // check Calibration Cycle enable
  {
    if (IS_MASK_CLEAR(cBattFlag0,cBF0_FullToTarget))
    {
         if (IS_MASK_CLEAR(LENOVOPMFW,BATTERY_CAL_END))
         {
        SET_MASK(cBattFlag0,cBF0_FullToTarget);    // charge to 100% then discharge to 1%
        CLEAR_MASK(cBattFlag0,cBF0_Full);
        CLEAR_MASK(cBattFlag0,cBF0_GoTarget);
        cTargetGauge = 1;
        //CLEAR_MASK(LENOVOPMFW,BATTERY_CYCLE);
         }
    }
  }
  else if (IS_MASK_SET(LENOVOPMFW,BATTERY_CAL_END))
  {
    CLEAR_MASK(LENOVOPMFW,BATTERY_CAL_END);
    CLEAR_MASK(cBattFlag0,cBF0_FullToTarget);
    CLEAR_MASK(cBattFlag0,cBF0_Full);
    CLEAR_MASK(cBattFlag0,cBF0_GoTarget);
  }
}

void ChkGoTarget(void)
{
  if (IS_MASK_SET(cBattFlag0,cBF0_GoTarget))
  {
    if ((cTargetGauge == RelativeStateOfChgL) ||  (RelativeStateOfChgL < cTargetGauge))
    {
      CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
	  if( Read_BATLEARN() )
	  {
	  	BAT_LEARN_OFF();
	  }
      CLEAR_MASK(cBattFlag0,cCmdAcOff);
      if (IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL))
      {
        SET_MASK(LENOVOPMFW,BATTERY_CAL_END);
        CLEAR_MASK(LENOVOPMFW,BATTERY_MAIN_CAL);
                cTargetGauge=0;
                SET_MASK(LENOVOPMFW_Temp,BATTERY_CALIBRATION_OK);   // release Calibration mode
                ChkLENOVOPMFW();
      }
      return;
    }
    if (RelativeStateOfChgL > cTargetGauge)
    {
      SET_MASK(ACOFF_SOURCE, BATTLEARN);
	  if( !Read_BATLEARN() )
	  {
	  	BAT_LEARN_ON();
	  }
      SET_MASK(cBattFlag0,cCmdAcOff);
    }
  }
  else if (IS_MASK_SET(cBattFlag0,cBF0_FullToTarget))
  {
    if (IS_MASK_CLEAR(cBattFlag0,cBF0_Full))
    {
      if (IS_MASK_SET(BattStatusL,FullyChg))
      {
        SET_MASK(cBattFlag0,cBF0_Full);
        SET_MASK(cBattFlag0,cBF0_GoTarget);          // Discharge or Charge to cTargetGauge%
      }
    }
  }
}

void ChkS3ResumeRSOC(void)
{
  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
  {
    if ( SystemIsS3 )
    {
#if BATS3WAKEcondition
      if( (RemainCapL <= (BATS3WAKEcondition & 0xFF)) && (RemainCapH == (BATS3WAKEcondition >> 8)))  //check  battery under 300mWh.
#else
      if (RelativeStateOfChgL <= S3RSOCPercentage)  // check battery under 5%.    
#endif      
      {
        //PWSeqStep = 1;
        //PowSeqDelay = 1;
        //SysPowState=SYSTEM_S3_S0;
        CriticalLowBATWakeS3();
      }
    }
  }
}

void RSMRST_shutdown(void)          //shutdown (RSMRST HI-->low-->HI)

{
#if !Support_STDNProct
  return;
#endif

  RSMRST_LOW();              //shutdown (remark first)
  Delay1MS(1);
  RSMRST_HI();

  RSMshutdownCnt++;
}

void DownGPUPState()
{
  if ((cGPUThrottling != 0)&&(GPUAlarm == 0)){

#if (PLATFORM_DGPU == NVidia)	
    if(GPU2Sec == 0){
    GPU2Sec = 200;
    cGPUThrottling--;
    SET_MASK(Recovery2Sec,RecoveryGPU);
    }
#endif

#if (PLATFORM_DGPU == AMD)
    GPU2Sec = 0;
    cGPUThrottling = 0;
#endif

  }
  
}

void UpGPUPState()
{
		  	
#if (PLATFORM_DGPU == NVidia)
  if (IS_MASK_SET(Recovery2Sec,RecoveryGPU)){
    GPU2Sec = 0;
  }
#endif

  if (cGPUThrottling != GPUMAXPSTATE) {
    if(GPU2Sec == 0){
      GPU2Sec = 200;
      cGPUThrottling++;
      CLEAR_MASK(Recovery2Sec,RecoveryGPU);
    }
  }
}  

void DownBatteryPState()
{
  if ((cBATTThrottling !=0)&&(BatteryAlarm == 0)){
    if(ADPI2Sec == 0){
      ADPI2Sec = 200;
      cBATTThrottling--;
      SET_MASK(Recovery2Sec,RecoveryCPU);
    }
  }
}

void UpBatteryPState()
{
  if (IS_MASK_SET(Recovery2Sec,RecoveryCPU)){
    ADPI2Sec = 0;
  }

#if (PLATFORM_DGPU == NVidia)
  if (cBATTThrottling < CPUMAXPSTATE) 
#endif

#if (PLATFORM_DGPU == AMD)
  if (cBATTThrottling != CPUMAXPSTATE) 
#endif	

  {
    if(ADPI2Sec == 0){
      ADPI2Sec = 200;
      cBATTThrottling++;
      CLEAR_MASK(Recovery2Sec,RecoveryCPU);
    }
  }
}  

void ChkBattery_OTP()
{
  ITempW01 = (WORD)((TemperatureH<<8)+TemperatureL );    // get temperature
  if (ITempW01 > 2730)
    {
    nBattAverTemp = (ITempW01 - 2730) / 10;
    }
  else
    return;

  if ((SystemIsS0)&&(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))
    {
    if (nBattAverTemp >= BatteryOTPHi)        // 60
      {
        SET_MASK(nStopChgStat3L,ENOVERTEMP);
        SET_MASK(BatteryAlarm,BATOTP);
        cBATTThrottling = CPUMAXPSTATE;
        cGPUThrottling = GPUMAXPSTATE;
        SET_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        SET_MASK(GPUAlarm,GPUAlarm_BATOTP);
//        RemainCapL = 0;              // follow Compal SPEC, when battery temperature over 68 degree C, remain capacity report 0% to BIOS and then shutdown system after 2minutes
//        RemainCapH = 0;
		SET_MASK(throttle_1, b2_OTP);
        if (nBattAverTemp >= BatteryOTPSD)       //70    // 2 min
        {BatteryOTPdebounce++;
		if(BatteryOTPdebounce>5)
			{
        BatteryOTPdebounce=0;
#if !EN_PwrSeqTest
          SET_MASK(SysStatus,ERR_ShuntDownFlag);
          ProcessSID(BATTOVERTEMP_ID);
          RSMRST_shutdown();
          CLEAR_MASK(BatteryAlarm,BATOTP);
          CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
          CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
#endif
			}
        }
        else
          nBattOvrTempCnt ++;
      }

    if (nBattAverTemp <= BatteryOTPLow)      //55
    {
    BatteryOTPdebounce=0;
      CLEAR_MASK(nStopChgStat3L,ENOVERTEMP);
	  CLEAR_MASK(throttle_1, b2_OTP);
      nBattOvrTempCnt =0;        
      if(GPU2Sec == 0){
        if((cGPUThrottling != 0) && ((GPUAlarm & (~BIT6)) == 0)){
          GPU2Sec = GPU_Sec;

#if (PLATFORM_DGPU == NVidia)		  
          cGPUThrottling --;
#else	  
          cGPUThrottling = 0;
#endif

        }else{
          CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
        }
      }
      if(ADPI2Sec == 0){
        if((cBATTThrottling != 0) && ((BatteryAlarm & (~BATOTP)) == 0)){
          ADPI2Sec = ADPI_Sec;
          cBATTThrottling--;
        }else{
          CLEAR_MASK(BatteryAlarm,BATOTP);
        }
      }
    }
  }
  else
  {
    if( (SystemIsS0)
//        && IS_MASK_SET(CHGIC_ReadCmd0x12L, TurboBoost)
      && IS_MASK_SET(SYS_STATUS,AC_ADP)
      && IS_MASK_SET(BattStatusL,Dsg)
      && (CurrentH & BIT7)
      )
    {
      if( nBattAverTemp >= BATHYBITOTPSHUT )
      {
      BatteryOTPdebounce++;
		if(BatteryOTPdebounce>5)
			{
#if !EN_PwrSeqTest
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BATTOVERTEMP_ID);
        RSMRST_shutdown();
        CLEAR_MASK(BatteryAlarm,BATOTP);
//          CLEAR_MASK(pEdgeProjectFlag2,pEdgeGPUOTTHROT);
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
#endif
			}
      }
      else if(nBattAverTemp >= BATHYBITOTPTHOEN)
      {
      BatteryOTPdebounce=0;
        SET_MASK(BatteryAlarm,BATOTP);
        cBATTThrottling = CPUMAXPSTATE;
        cGPUThrottling = GPUMAXPSTATE;
//          SET_MASK(pEdgeProjectFlag2,pEdgeGPUOTTHROT);
        SET_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        SET_MASK(GPUAlarm,GPUAlarm_BATOTP);
      }
      else if ( nBattAverTemp <= BATHYBITOTPTHODIS )
      {
//          CLEAR_MASK(pEdgeProjectFlag2,pEdgeGPUOTTHROT);
BatteryOTPdebounce=0;
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        if(GPU2Sec == 0){
          if((cGPUThrottling != 0) && ((GPUAlarm & (~BIT6)) == 0)){
            GPU2Sec = GPU_Sec;
			
#if (PLATFORM_DGPU == NVidia)
          cGPUThrottling --;	
#else		  	  		  
          cGPUThrottling = 0;
#endif

          }else{
            CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
          }
        }
        if(ADPI2Sec == 0){
          if((cBATTThrottling != 0) && ((BatteryAlarm & (~BATOTP)) == 0)){
            ADPI2Sec = ADPI_Sec;
            cBATTThrottling--;
          }else{
            CLEAR_MASK(BatteryAlarm,BATOTP);
          }
        }
      }
    }
    else
    {
      CLEAR_MASK(BatteryAlarm,BATOTP);
	  BatteryOTPdebounce=0;
//        CLEAR_MASK(pEdgeProjectFlag2,pEdgeGPUOTTHROT);
      CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
      CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
	 CLEAR_MASK(throttle_1, b2_OTP);
    }
    CLEAR_MASK(nStopChgStat3L,ENOVERTEMP);
    nBattOvrTempCnt =0;
  }


if ((SystemIsS0)&&(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))
{
  if(RelativeStateOfChgL > 20)
			{
			CLEAR_MASK(throttle_1,b4_battery_low);
			if (nBattAverTemp <= 60)
				{
				if(pwr_sys > 563) //1024/3*1.65=563
				{
					SET_MASK(throttle_1,b3_pwr_sys);
				}
				else if(pwr_sys < 512) //1024/3*1.5=512
				{
					CLEAR_MASK(throttle_1,b3_pwr_sys);
				}
				}
			}
			else
			{
				CLEAR_MASK(throttle_1,b3_pwr_sys);
				SET_MASK(throttle_1,b4_battery_low);
			}
}
else
{
CLEAR_MASK(throttle_1,b4_battery_low);
CLEAR_MASK(throttle_1,b3_pwr_sys);

}

if(SystemIsS0)
{
if(IS_MASK_SET(throttle_1,b4_battery_low))
{
if(IS_MASK_CLEAR(throttle_1,b6_battery_loweventflag))
{
	SET_MASK(throttle_1,b6_battery_loweventflag);
	 ECSMI_SCIEvent(Battery_low_92);

}

}
else
{
	if(IS_MASK_SET(throttle_1,b6_battery_loweventflag))
		{
		CLEAR_MASK(throttle_1,b6_battery_loweventflag);
		 ECSMI_SCIEvent(Battery_nonlow_93);
		}

}
}
else
{CLEAR_MASK(throttle_1,b6_battery_loweventflag);

}
}

void ChkBattery_OCP()
{
  LWORD BattPresentVolt,BattNowCurrent;
 
  if (!Read_AC_IN() || Read_ACOFF())
  {
    // Battery only mode
    if (CurrentH & BIT7) // battery is discharging
    {
      BattNowCurrent = 0xFFFF - (WORD)((CurrentH<<8)+CurrentL); // mA
      BattPresentVolt = (WORD)((VoltageH << 8) + VoltageL); // mV
      XWTemp1 = (BattPresentVolt * BattNowCurrent)/1000000; // Watt

      if ((IS15Inch && (XWTemp1 >= OCPCapacity15)) || (IS14Inch && (XWTemp1 >= OCPCapacity)))
      {
        SET_MASK(BatteryAlarm,BATOCP);
	
#if (PLATFORM_DGPU == NVidia)
		 if(pCPUBasePStage != 0){
          if(cBATTThrottling < CPUturboDisPstage){
            cBATTThrottling = CPUturboDisPstage;
          }else{
            UpBatteryPState();
          }
        }		 
#else
            UpBatteryPState();
#endif

//        SET_MASK(pEdgeProjectFlag2,pEdgeGPUOCTHROT);
        SET_MASK(GPU_THROTTLE_SRC, BAT_OCP);
        SET_MASK(GPUAlarm,GPUAlarm_BATOCP);
        UpGPUPState();
      }

      if ((IS15Inch && (XWTemp1 <= OCPCapacityRelease15)) || (IS14Inch && (XWTemp1 <= OCPCapacityRelease)))
      {
//        CLEAR_MASK(pEdgeProjectFlag2,pEdgeGPUOCTHROT);
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_OCP);
        if(GPU2Sec == 0){
          if((cGPUThrottling != 0) && ((GPUAlarm & (~BIT1)) == 0)){
            GPU2Sec = GPU_Sec;

#if (PLATFORM_DGPU == NVidia)
          cGPUThrottling --;
#else		  		  
          cGPUThrottling = 0;
#endif

          }else{
          CLEAR_MASK(GPUAlarm,GPUAlarm_BATOCP);
          }
        }
        if(ADPI2Sec == 0){
          if((cBATTThrottling != 0) && ((BatteryAlarm & (~BATOCP)) == 0)){
            ADPI2Sec = ADPI_Sec;
            cBATTThrottling--;
          }else{
            CLEAR_MASK(BatteryAlarm,BATOCP);
          }
        }
      }
      if( (IS15Inch && (BattNowCurrent >= (16000 * 0.95))) || (IS14Inch && (BattNowCurrent >= (16000 * 0.95))) )
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BATTOVERCURR_ID);
        RSMRST_shutdown();
        CLEAR_MASK(BatteryAlarm,BATOCP);
//        CLEAR_MASK(pEdgeProjectFlag2,pEdgeGPUOCTHROT);
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_OCP);
        CLEAR_MASK(GPUAlarm,GPUAlarm_BATOCP);
      }
    }
  }
}

void ChkBattery_FCCchg()
{
  ChkBattery_FCCchg_count++;
  if (ChkBattery_FCCchg_count >= 100) // 10 Sec(100ms 100times)
  {
    ChkBattery_FCCchg_count = 0;

    // Load default
    if ((ChkBattery_FCCchg_lastFCCL == 0) && (ChkBattery_FCCchg_lastFCCH == 0))
    {
      ChkBattery_FCCchg_lastFCCL = FullChgCapL;
      ChkBattery_FCCchg_lastFCCH = FullChgCapH;
    }

    // Check FCC and Notify OS if FCC change for each 10 sec
    if ((ChkBattery_FCCchg_lastFCCL != FullChgCapL)
      && (ChkBattery_FCCchg_lastFCCH != FullChgCapH))
    {
      //ECSMI_SCIEvent(ACPI_BAT0IN_SCI);
      SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATIn);
      ChkBattery_FCCchg_lastFCCL = FullChgCapL;
      ChkBattery_FCCchg_lastFCCH = FullChgCapH;
    }
  }
}

void ChkAvgCurrent()
{
  // Modify for meet specification
  //Average current report to OS
  //Timer<=60 seconds(The timer starts counting when AC adapter plug out.)
  //Report "0x00" to EC name space 0xd2, 0xd3 by one time, and then
  //Report battery Current(0x0a) to EC name space 0xd2, 0xd3
  //Reset condition:When Timer>60 seconds,Report battery AverageCurrent(0x0b) to EC name space 0xd2, 0xd3
  if ( Bat0x0BFakeCnt == 0)
  {
    
//    AvgCurrentH = Bat0x0BTempH;
  }
  else if(Bat0x0BFakeCnt < 60)
  {
    AvgCurrentL = CurrentL;
    AvgCurrentH = CurrentH;
  }
  else if(Bat0x0BFakeCnt == 60)
  {
    AvgCurrentL = 0;
    AvgCurrentH = 0;
  }
}

void RST_ChgTimeOutCnt(void)
{
  TrickleChgTimeOutCnt = 0;
  FastChgTimeOutCnt = 0;
  CLEAR_MASK(nStopChgStat3L,ENSTOPCHG);
  CLEAR_MASK(nStopChgStat3L,ENTRITIMEOUT);
}

void Battery100ms(void)
{

  if (inhibit2sec!=0)
  {
    inhibit2sec--;
    if (inhibit2sec==0)
    {
      CHGIC_ptr = 2;
    }
  }
  else
    {CLEAR_MASK(nStopChgStat3L,ENCHARGESUSP);}    // use for detect battery charging suspend

  if( pFakeCHGCnt != 0 )
    {pFakeCHGCnt--;}

}


void Battery1Sec(void)
{
  if (Bat0x0BFakeCnt != 0)
    Bat0x0BFakeCnt--;

  if (BatSMbusFailCount != 0)
  {
      SET_MASK(Bat1_FPChgFlag,BIT(0));
      nBattErrorCnt++;
      if (nBattErrorCnt > 250)
        nBattErrorCnt = 251;
      if (BatSMbusFailCount > 220)
        BatSMbusFailCount = 221;
  }
  else
  {
    nBattErrorCnt = 0;
    if (CurrentL|CurrentH)      // if  CurrentL != 0
    {
      if ((CurrentH&0x80) == 0)    // nNowCurrent > 0, charging
      {                //current(0x0A)>0mA&polling time10cycles
        nBattTempCnt ++;
        if (nBattTempCnt >= 10)
          nBattTempCnt = 11;      // nBattTempCnt > 10
      }
      else                // discharging
      {
        nBattTempCnt = 0;
        RST_ChgTimeOutCnt();
      }
    }
    else
    {
      nBattTempCnt = 0;
    }
  }
}

void SbcSetLongLifeMode (
  BYTE long_life_span_mode
) 
{
//
//  long_life_span_mode -- 0: Long run time mode, 1:Long life span mode
//

  if (!(OptMfgFunction2H & COMMON_DUAL_MODE)) {
    // The battery is no dual mode capability, just return.
    return;
  }
//
// Bit 6: Dual Mode Select(0: Long runtime mode(Default), 1:Long Lifespan mode, 
// This bit must be kept until next write operation and must not be reset at OFF 
// State
//

  if (long_life_span_mode) {
    OptMfgFunction2L  |= BIT6;    // Long Lifespan mode
  } else {
    OptMfgFunction2L  &= (~BIT6); // Long runtime mode
  }

  if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_LVMfgFun2, &OptMfgFunction2L , SMBus_NeedPEC))
  {
    BatSMbusFailCount++;
  }
  //
  // TODO: It needs to add error condition for handling SMBus fail.
  //
}
//(JSN)+
void IBMChargeCondition(void)
{
  switch(u4MainBattCtrl)
  {
    case 0x00:
      
      break;
    case 0x01:  // Reset Battery Charge Inhibit
      p2SetInhChgTimerDis;
      Inhibit_chargingCLR;
      PriBattinhibitDis;
      CLEAR_MASK(nStopChgStat3H,ENBOOSTSTOPCHG);
      break;
    case 0x02:  // Peakshift Inhibit Charge
      PriBattinhibitEn;
      Inhibit_chargingSET;
      SET_MASK(nStopChgStat3H,ENBOOSTSTOPCHG);
      //if( !p2SetInhChgTimerGet )
      //{
      //  //p2InhChgTimer
      //  pInhitTimer = (u4InhChgTimerL + ( u4InhChgTimerH * 256 ))*30;
      //  p2SetInhChgTimerEn;
      //}

      //if( pInhitTimer )
      //{
      //  PriBattinhibitEn;
      //  u4PeriSTS4 |= 0x08;
      //  SET_MASK(nStopChgStat3H,ENBOOSTSTOPCHG);
      //  pInhitTimer--;
      //}
      //else
      //{
      //  PriBattinhibitDis;
      //  u4PeriSTS4 &= 0xF7;
      //  CLEAR_MASK(nStopChgStat3H,ENBOOSTSTOPCHG);
      //}
      break;
    case 0x03: // Stop peakshift discharge and Refresh

      break;
    case 0x04: // PeakShiftDischarge

      break;
    case 0x05: // Inhibit to charge battery
      if( ((ChargeCurrL + ChargeCurrH * 256) != 0) && BattExistGet)
      {
        if( RelativeStateOfChgL <= u4MainBattChgOnCap )  // Start Charge.
        {
          CLEAR_MASK(nStopChgStat3H,ENBOOSTSTOPCHG);
        }
        else if( RelativeStateOfChgL >= u4MainBattChgOffCap )  // Stop Charge.
        {
          SET_MASK(nStopChgStat3H,ENBOOSTSTOPCHG);
        }
      }
      break;
    case 0x06: // Refresh battery

      break;
    case 0x07: // Set long life span mode
      SbcSetLongLifeMode(TRUE);
      break;

    case 0x08: // Set long run mode
      SbcSetLongLifeMode(FALSE);
      break;

    case 0x09: // Turn off the battery
      Lock_ShipMode();
      break;

    default:
      p2SetInhChgTimerDis;;
      CLEAR_MASK(nStopChgStat3H,ENBOOSTSTOPCHG);
      break;
  }
  u4MainBattCtrl = 0; // clear
}
//(JSN)-

const sRSmbusBStruct code ABatCommandTable [] =
{//command          REG                 no.
{ C_mode,           &BattModeL          ,0x00},   //Batpollstep1 = 1    WORD   
{ C_ChargingV,      &ChargeVoltL        ,0x00},   //Batpollstep1 = 2    WORD
{ C_DCap,           &DesignCapL         ,0x00},   //Batpollstep1 = 3    WORD    0~8 initial only
{ C_DVolt,          &DesignVoltL        ,0x00},   //Batpollstep1 = 4    WORD
{ C_Spec,           &SpecInfoL          ,0x00},   //Batpollstep1 = 5    WORD
{ C_Date,           &ManufactureDateL   ,0x00},   //Batpollstep1 = 6    WORD
{ C_SerialNo,       &SerialNumberL      ,0x00},   //Batpollstep1 = 7    WORD
{ C_Mname,          &ManufacturerName   ,0x0A},   //Batpollstep1 = 8    BLOCK
{ C_Dname,          &DeviceName         ,0x0A},   //Batpollstep1 = 9    BLOCK
{ C_Dchem,          &DeviceChemistry    ,0x04},   //Batpollstep1 = 10   BLOCK
{ C_Mdata,          &ManufactureData    ,0x0C},   //Batpollstep1 = 11   BLOCK
{ C_OptMfgFunc5,    &OptMfgFunction5    ,0x0E},   //Batpollstep1 = 12   BLOCK
{ C_BattUsageCond,  &BattUsageCondition ,0x0A},   //Batpollstep1 = 13   BLOCK
{ C_WarrantyPeriod, &WarrantyPeriodL    ,0x00},   //Batpollstep1 = 14   WORD
{ FirstUsedDate,    &OptMfgFunction1L   ,0x00},   //Batpollstep1 = 15   WORD     //First Used Date
{ C_access,         &ManufactureAccessL ,0x00},   //Batpollsetp1 = 16   WORD
{ C_temp,           &TemperatureL       ,0x00},   //Batpollstep1 = 17   WORD
{ C_volt,           &VoltageL           ,0x00},   //Batpollstep1 = 18   WORD
{ C_current,        &CurrentL           ,0x00},   //Batpollstep1 = 19   WORD
{ C_AVcurrent,      &AvgCurrentL        ,0x00},   //Batpollstep1 = 20   WORD
{ C_RSOC,           &RelativeStateOfChgL,0x00},   //Batpollstep1 = 21   WORD
{ C_RMcap,          &RemainCapL         ,0x00},   //Batpollstep1 = 22   WORD  9~19 normal polling
{ C_FCcap,          &FullChgCapL        ,0x00},   //Batpollstep1 = 23   WORD
{ C_RunTimeEmpty,   &RunTimeToEmptyL    ,0x00},   //Batpollstep1 = 24   WORD
{ C_AVTimeEmpty,    &AvgTimeToEmptyL    ,0x00},   //Batpollstep1 = 25   WORD
{ C_AVTimeFull,     &AvgTimeToFullL     ,0x00},   //Batpollstep1 = 26   WORD
{ C_ChargingI,      &ChargeCurrL        ,0x00},   //Batpollstep1 = 27   WORD
{ C_BatStatus,      &BattStatusL        ,0x00},   //Batpollstep1 = 28   WORD
{ C_CycleCount,     &CycleCountL        ,0x00},   //Batpollstep1 = 29   WORD
{ C_LVMfgFun2,      &OptMfgFunction2L   ,0x00},   //Batpollsetp1 = 30   WORD
{ C_FETTemp,        &FETTemperatureL    ,0x00},   //Batpollstep1 = 31   WORD
{ C_ChargingV,      &ChargeVoltL        ,0x00},   //Batpollstep1 = 32   WORD
//  { C_D_FET,      &SHIPMODE_L        ,0x00},    //Batpollstep1 = 15   WORD
};
//-----------------------------------------------------------------------------
// get 1 battery data
//-----------------------------------------------------------------------------
void GetBatData(BYTE index)
{
  BYTE i;
  if(ABatCommandTable[index].block_limit != 0)
    {
    if(  !bRSMBusBlock(SMbusChB, SMbusRBK, SmBat_Addr,
      ABatCommandTable[index].cmd,
      ABatCommandTable[index].smbdataA))
    {  // SMBUS FAIL
      BatSMbusFailCount++;
    }
    else
    {  // SMBUS OK
      BatSMbusFailCount=0;
    }
  }
  else
  {
    if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr,
       ABatCommandTable[index].cmd,
       ABatCommandTable[index].smbdataA,
       SMBus_NoPEC))
    {  // SMBUS FAIL
      BatSMbusFailCount++;
    }
    else
    {  // SMBUS OK
      BatSMbusFailCount=0;
    }
  }

//remove "LNV" prefix"
 if (ABatCommandTable[index].cmd == C_Dname)
  {
    for( i=0; i < 14; i++ )
    {
      if (i < 8)
       {DeviceName[i] = DeviceName[i+4];}
      else
       {DeviceName[i] = 0;}
    }
  }
}


void ChkBattery_Percl()
{
    bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr,C_RSOC,&RelativeStateOfChgL ,SMBus_NoPEC);
}

//-----------------------------------------------------------------------------
// battery plug in first fast update all information
//-----------------------------------------------------------------------------
void FirstGetBatData(void)
{
    BYTE i;

//  Unlock_ShipMode();
//    for (j=0;j<=2;j++) // initial get all battery information
    Batpollstep1 = 0;
    for (i=0;i<(sizeof(ABatCommandTable)/4);i++)
    {
      GetBatData(Batpollstep1);
      Batpollstep1++;
    }
  //[Project][CC20130530][Start] Fix ECR172864 issue for "FRU Part Number contains extra "LNV" prefix".

    Batpollstep1=15;
  //[Project][CC20130530][End] Fix ECR172864 issue for "FRU Part Number contains extra "LNV" prefix".
  SHA1_SEED = (WORD)((TL0+RemainCapL )<<8);

  UpdateNameSpace();
  Chk_Battery_Full();
  nBattErrorCnt = 0;
  SHA1_SEED = SHA1_SEED + (WORD)(TL0);
  srand(SHA1_SEED);
  
#if (BatLowVot_Rule == LowVot_LGNotLG)
  if( (ManufacturerName[0] == 'L' ) && ( ManufacturerName[1] == 'G' ) )
  { BatLowVoltage = LG_3SLowVot ; }    // Shutdown Voltage 8.25V  for LG 3S battery
  else
  { BatLowVoltage = Sanyo_3SLowVot; }  // Shutdown Voltage 8.5V for Sanyo battery
#endif

#if (BatLowVot_Rule == LowVot_3S4S)
  ITempW01 = (WORD)((DesignVoltH<<8)+DesignVoltL);    //get design voltage

  if (ITempW01 >= BAT3S4SVot_Divide)
  { BatLowVoltage = BAT4SLowVot; }   // Shutdown Voltage 11.9V  for 4S battery
  else
  { BatLowVoltage = BAT3SLowVot; }   // Shutdown Voltage 8.9V  for 3S battery
#endif

  if (BatSMbusFailCount==0)
  {
    Battdata_ready = 1;
    WSMbusTemp01 = BattModeH;
    if ((WSMbusTemp01^0xE0)&0xE0)        // check bit15,bit14,bit13=1  ??
    {
      WSMbusTemp01=BattModeL;
      WSMbusTemp02=BattModeH;
      WSMbusTemp02|=0xE0;
      if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_mode, &WSMbusTemp01,SMBus_NoPEC))
      {
        BatSMbusFailCount++;
      }
    }
  }
}

void Chk_Battery_Full(void)
{
  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))    // discharge
  {
      CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);  //clear  battery charging flag
      SET_MASK(nBatteryStatH,CMBS_DISCHARGE);  //set battery discharging flag
      return;
  }

  if (IS_MASK_SET(cBattFlag0,cCmdAcOff)||Read_ACOFF()||IS_MASK_SET(BattStatusL,FullyChg)||((nStopChgStat3L|nStopChgStat3H)!=0))
  {
    CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);    //clear  battery charging flag
    CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE);  //clear battery discharging flag
    return;
  }
  else
  {
    if (IS_MASK_SET(cBattFlag0,cBF0_GoTarget) && (cTargetGauge == RelativeStateOfChgL))
    {
      CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);    //clear  battery charging flag
      CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE);  //clear battery discharging flag
      return;
    }
  }

  if (IS_MASK_CLEAR(LENOVOPMFW,BATTERY_CYCLE))
  {
    if (IS_MASK_SET(BattStatusL,FullyChg))  // || IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit))
    {
      CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);    //clear  battery charging flag
      SET_MASK(nBatteryStatH,CMBS_DISCHARGE);   //set battery discharging flag
    }
    else
    {
      SET_MASK(nBatteryStatH,CMBS_CHARGE);      //set battery charging flag
      CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE);   //clear battery discharging flag
    }
  }
  else
  {
    if (IS_MASK_SET(BattStatusL,FullyChg))  // || IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit))
    {
      CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);    //clear  battery charging flag
    }
    else
    {
      SET_MASK(nBatteryStatH,CMBS_CHARGE);      //set battery charging flag
      CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE);   //clear battery discharging flag
    }
  }
}

void Unlock_ShipMode(void)
{
/*  BYTE sCmd;
  WORD sData;

// first command = 0x34, data1 = 0x2000, data2 = 0x4000
    sCmd = C_D_FET;
    sData = 0x0020;   // word form L/H
    if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    {
      SMbusFailCnt3++;
    }

    sCmd = C_D_FET;
  sData = 0x0040;   // word form L/H
    if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    {
      SMbusFailCnt2++;
    }
*/
//   if(IS_MASK_SET(OptMfgFunction2L, BIT7)) // Replaceable(0)/Non-replaceable(1)
//   {
//     if(bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
//     {
//       if((WSMbusTemp01 == 0x00) && (WSMbusTemp02 == 0x01)){
        WSMbusTemp01=0x00;
        WSMbusTemp02=0x20;
        if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
        {
          SMbusFailCnt3++;
        }
        else{
          WSMbusTemp01=0x00;
          WSMbusTemp02=0x40;
          if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
          {
            SMbusFailCnt2++;
          }
        }
//       }
//   }
// }
}

void Lock_ShipMode(void)
{
/*
  BYTE sCmd;
  WORD sData;

// first command = 0x34, data1 = 0x0000, data2 = 0x1000
    sCmd = C_D_FET;
    sData = 0x0000;   // word form L/H
    if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    {
      SMbusFailCnt3++;
    }

    sCmd = C_D_FET;
  sData = 0x0010;   // word form L/H
    if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    {
      SMbusFailCnt2++;
    }
*/
  if(IS_MASK_SET(OptMfgFunction2L, BIT7)) // Replaceable(0)/Non-replaceable(1)
  {
    if(bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
    {
      if((WSMbusTemp01 == 0x00) && (WSMbusTemp02 == 0x00)){
        WSMbusTemp01=0x00;
        WSMbusTemp02=0x00;

        if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
        {
          SMbusFailCnt3++;
        }
        else{
          WSMbusTemp01=0x00;
          WSMbusTemp02=0x10;

          if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
          {
            SMbusFailCnt2++;
          }
        }
      }
    }
  }
}



//-----------------------------------------------------------------------------
// read all battery about information
// polling time : 1sec
//-----------------------------------------------------------------------------
void OEM_PollingBatData_TASK(void)
{
  XBYTE i,j;
  XBYTE *ptr;

  if(IS_MASK_CLEAR(BATTUPDATEFW,PriBattInhib))
  {
    GetBatData(Batpollstep1);

  	if (inhibit2sec > 0)
  	{
    	CurrentL = 0;
    	CurrentH = 0;
  	}

	#if (PLATFORM_CHIP == AMD)
  	if(BatSMbusFailCount == 0)
  	{
		switch(ABatCommandTable[Batpollstep1].cmd)
		{
			case C_volt:
		    if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))    // discharge
		    {
		      Chk_Shutdown_Volt();
		    }
				break;
		  default:
		    break;
		}
  	}
	#endif

    Batpollstep1++;

    if(Batpollstep1 >= (sizeof(ABatCommandTable)/4))
    {
    ////////////////////////
    // Battery Debounce Block
    //Get 9 entry from battery table, loop and XOR each byte, calculate a hash byte
    //If hash result same as previous, then counter +1
    //If counter > 3 times, mean battery data stable, keep Batpollstep1 = 9 to skip first 9 entry in battery table
    //otherwise, clean counter and loop the table again.
    if (Get_Batt_debounce_count < 3)
    {
      Get_Batt_debounce_hash2 = Get_Batt_debounce_hash1;
      Get_Batt_debounce_hash1 = 0x00;
      for (i=0; i<9; i++)
      {
        if(ABatCommandTable[i].block_limit == 0x00)
          Get_Batt_debounce_hash1 ^= *ABatCommandTable[i].smbdataA;
        else
        {
          ptr = ABatCommandTable[i].smbdataA;
          for(j=0; j<ABatCommandTable[i].block_limit; j++)
          {
            Get_Batt_debounce_hash1 ^= *ptr;
            ptr++;
          }
        }
      }

      if (Get_Batt_debounce_hash2 == Get_Batt_debounce_hash1)
        Get_Batt_debounce_count++;
      else
        Get_Batt_debounce_count = 0;

      Batpollstep1=0; //revert Batpollstep1 for start over
      // Fill nBatteryStatL after Battery Data stable.     
    }
    else
    {
        Batpollstep1=15;
    }
    ////////////////////////////
    UpdateNameSpace();
    ChkLENOVOPMFW();
    ChkGoTarget();
    Chk_Battery_Full();
    ChkBattery_OTP();
    ChkBattery_OCP();
    ChkBattery_FCCchg();
    ChkAvgCurrent();
    ChkS3ResumeRSOC();
//[Project][CC20130425][Start]Add first used date feature.
    ChkFirstUsedDate();
//[Project][CC20130425][End]Add first used date feature.
	Battery_RSOCLowNotifyOS();

    }
  }
}


//-----------------------------------------------------------------
// Service Charger SMBus
//-----------------------------------------------------------------
void WriteSmartChgIC(void)
{
  BYTE sCmd, *sData;
  WORD AirplaneModeChargeCurrentLimit;
  WORD ChargeCurrentLimit;
  WORD AdpWattTemp;      
  BYTE WTemp;

  if (!Read_AC_IN())
    return;

#if (CHARGER_IC == BQ25700)
  if( CHGIC_ptr>=3 ) 
  {
  	CHGIC_ptr=0;
  }
  else
  {
  	CHGIC_ptr++;
  }
#endif //BQ25700  

#if (CHARGER_IC == BQ24780S)
  if( CHGIC_ptr>=4 ) CHGIC_ptr=0;       
  else CHGIC_ptr++;
#endif //BQ24780S  
  switch( CHGIC_ptr )
    {
      case 0x00:
        sCmd = C_ChargeCurrent;
        sData = &ChargeCurrL;
        if (IS_MASK_SET(u4Cfgspace3, AirplaneMode)) {
          if (SystemIsS0) {
            AirplaneModeChargeCurrentLimit = (CHARGE_CURRENT_1A_L << 8) | CHARGE_CURRENT_1A_H;
          } else if (SystemIsS3 || SystemIsS5) {
            AirplaneModeChargeCurrentLimit = (CHARGE_CURRENT_2_2A_L << 8) | CHARGE_CURRENT_2_2A_H;
          }
          sData = &AirplaneModeChargeCurrentLimit;
        } else {
          //
          // Not in AirPlane mode.
          //
        }
        break;

      case 0x01:
        sCmd = C_ChargeVoltage;
		if(IS_MASK_SET(BattStatusL,FullyChg))
			{ChargeVoltL=0X30;
		ChargeVoltH=0X31;
			}
		
        sData = &ChargeVoltL;
        break;
      case 0x02:
 /*
#if Support_TYPE_C
				if(!Read_DCIN_ATTACHED2())
				{	//Write the Type C adaptor output current value to Charger IC "InputCurrent" register.
#if (CHARGER_IC == BQ25700)
					if(CurrentTypeCI != 0)
					{
						if(IS14Inch)
						{ //14"
							WTemp = 0xFF;
						}
						else
						{ //15"
						  if(nSwitchableBTNGET)
						  {	//DIS is 65 watt system
						#if (PLATFORM_CHIP == INTEL)
						    if (N16P_GT){
							    	WTemp = 90;
						    }else
						#endif
						    {
							    	WTemp = 65;
						    }
						  }
						  else
						  {	//UMA is 45 watt system.
							    	WTemp = 45;
						  }
						}
						if(IS_MASK_CLEAR(SYS_MISC1,G3Mode) && IS15Inch && (CurrentTypeCWatt >= WTemp))
						{

						#if (PLATFORM_CHIP == INTEL)
					  		if(WTemp == 90)
					  		{
					      		if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
					          	(BattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
									CHGIC_InputCurrentH = A90WINPUTCURRH_AD - Intel15InputCurrentAD;
								}
								else
								{
	          						CHGIC_InputCurrentH = A90WINPUTCURRH_A - Intel15InputCurrentA;
								}
									CHGIC_InputCurrentL = A90WINPUTCURRL;
	            					InputCurrent_Watt =90;
					  		}
							else if(WTemp == 65)
						#else
					  		if(WTemp == 65)
						#endif
					  		{
								CHGIC_InputCurrentH = A65WINPUTCURRH;
								CHGIC_InputCurrentL = A65WINPUTCURRL;
	            				InputCurrent_Watt =65;
					  		}
					  		else
					  		{ //45W
								CHGIC_InputCurrentH = A45WINPUTCURRH;
								CHGIC_InputCurrentL = A45WINPUTCURRL;
	            				InputCurrent_Watt =45;
					  		}
						}
						else
						{	//Setting the real adaptor input current to charger IC when G3 to S5.							

						#if (PLATFORM_CHIP == INTEL)
							if(CurrentTypeCWatt >= 90)
							{	//Setting the 90W adaptor input current to charger IC for over 90W adaptor.
					      		if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
					          	(BattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
									CHGIC_InputCurrentH = A90WINPUTCURRH_AD - Intel15InputCurrentAD;
								}
								else
								{
		        					CHGIC_InputCurrentH = A90WINPUTCURRH_A - Intel15InputCurrentA;
								}
								CHGIC_InputCurrentL = A90WINPUTCURRL;
		            			InputCurrent_Watt =90;
							}
							else if(CurrentTypeCWatt >= 65)
						#else
							if(CurrentTypeCWatt >= 65)
						#endif	
							{
	              				CHGIC_InputCurrentH = A65WINPUTCURRH;
	              				CHGIC_InputCurrentL = A65WINPUTCURRL;
	              				InputCurrent_Watt =65;
							}
							else if(CurrentTypeCWatt >= 60)
							{
	              				CHGIC_InputCurrentH = A60WINPUTCURRH;
	              				CHGIC_InputCurrentL = A60WINPUTCURRL;
	              				InputCurrent_Watt =60;
							}
							else if(CurrentTypeCWatt >= 45)
							{
								CHGIC_InputCurrentH = A45WINPUTCURRH;
								CHGIC_InputCurrentL = A45WINPUTCURRL;
		            			InputCurrent_Watt =45;
							}
							else
							{
								AdpWattTemp = CurrentTypeCI / 5;
						    	CHGIC_InputCurrentL = AdpWattTemp >> 8;
						    	CHGIC_InputCurrentH = AdpWattTemp & 0x00FF;
				       			InputCurrent_Watt =CurrentTypeCWatt;
				      		}
					  	}
				  }
				  else
				  {
            		CHGIC_InputCurrentH = A45WINPUTCURRH;
            		CHGIC_InputCurrentL = A45WINPUTCURRL;
            		InputCurrent_Watt =45;

				  }
#else
						AdpWattTemp = CurrentTypeCI / 5;	//InputCurrent = I / 50mA(unit) = TypeC_Adaptor_Current(Unit:10mA) / 50mA = CurrentTypeCI * 10/50 = CurrentTypeCI / 5;(For 25700)
				    CHGIC_InputCurrentL = AdpWattTemp >> 8;
				    CHGIC_InputCurrentH = AdpWattTemp & 0x00FF;
				  }
				  else
				  {
			      CHGIC_InputCurrentH = A45WINPUTCURRH;
			      CHGIC_InputCurrentL = A45WINPUTCURRL;
				  }


#endif   //BQ25700
#if (CHARGER_IC == BQ24780S)
		// Need implement
#endif   //BQ24780S
				}
				else
#endif //Support_TYPE_C
				{
#if (PLATFORM_CHIP == INTEL)
#ifdef OEM_Hook_Power_90WSystem_support
				OEM_Hook_Power_90WSystem();
#endif
        if (!(nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) )&& IS15Inch ) {  //DIS 
          if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap >= 45)){
            CHGIC_InputCurrentH = A45WINPUTCURRH;
            CHGIC_InputCurrentL = A45WINPUTCURRL;
            InputCurrent_Watt =45;
          }
				}else{				
          if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
              (BattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
            if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
              CHGIC_InputCurrentH = A45WINPUTCURRH;
              CHGIC_InputCurrentL = A45WINPUTCURRL;
              InputCurrent_Watt =45;
            }else if (u4ACAdaptorCap == 65){
              CHGIC_InputCurrentH = A65WINPUTCURRH;
              CHGIC_InputCurrentL = A65WINPUTCURRL;
              InputCurrent_Watt =65;
		#if (PLATFORM_CHIP == INTEL)
            }else if (u4ACAdaptorCap>=90){
              CHGIC_InputCurrentH = A90WINPUTCURRH_AD - Intel15InputCurrentAD;
              CHGIC_InputCurrentL = A90WINPUTCURRL;
              InputCurrent_Watt =90;
		#endif
            }
	
          }else{                                                         
            if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
              CHGIC_InputCurrentH = A45WINPUTCURRH;
              CHGIC_InputCurrentL = A45WINPUTCURRL;
              InputCurrent_Watt =45;
            }else if (u4ACAdaptorCap == 65){
              CHGIC_InputCurrentH = A65WINPUTCURRH;
              CHGIC_InputCurrentL = A65WINPUTCURRL;
              InputCurrent_Watt =65;
		#if (PLATFORM_CHIP == INTEL)
            }else if (u4ACAdaptorCap>=90){
              CHGIC_InputCurrentH = A90WINPUTCURRH_A - Intel15InputCurrentA;
              CHGIC_InputCurrentL = A90WINPUTCURRL;
              InputCurrent_Watt =90;
		#endif
            }
          }
				}
#endif*/
		#if (PLATFORM_CHIP == AMD)
        if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
          CHGIC_InputCurrentH = A45WINPUTCURRH;
          CHGIC_InputCurrentL = A45WINPUTCURRL;
          InputCurrent_Watt = 45;
        }else if (u4ACAdaptorCap >= 65){
          CHGIC_InputCurrentH = A65WINPUTCURRH;
          CHGIC_InputCurrentL = A65WINPUTCURRL;
          InputCurrent_Watt = 65;
        }  
		#endif

		//	}
        sCmd = C_InputCurrent;
        sData = &CHGIC_InputCurrentL;
        break;
      case 0x03:

        if ((CHGIC_ReadCmd0x12L!=0) || (CHGIC_ReadCmd0x12H!=0))
        {
          if (IS_MASK_SET(cBattFlag0,cCmdAcOff)||Read_ACOFF()||IS_MASK_SET(BattStatusL,FullyChg))  //  || IS_MASK_CLEAR(nBatteryStatH,CMBS_CHARGE))
          {
            SET_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Inhibit Charge
          }
          else
          {
            if (IS_MASK_SET(cBattFlag0,cBF0_GoTarget) && (cTargetGauge == RelativeStateOfChgL))
            {
              SET_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Inhibit Charge
            }
            else
            {
              CLEAR_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Enable Charge
            }
          }
      // Due to must be stop charger before set AC OFF. add ACOFF_SOURCE in condition.
          if (nStopChgStat3L|nStopChgStat3H|inhibit2sec|ACOFF_SOURCE)          // nStopChgStat3L,nStopChgStat3H,Charge_inhibit2sec != 0
          {
            SET_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);   //Inhibit Charge
          }
            sCmd = C_ChargerMode;
            sData = &CHGIC_ReadCmd0x12L;
        }
        else
        {
          return;
        }
        break;

      case 0x04:
#if (CHARGER_IC == BQ25700)	  
        CLEAR_MASK(CHGIC_ReadCmd0x31L,IFAULT_HI);
        SET_MASK(CHGIC_ReadCmd0x31L,IFAULT_LOW);
        sCmd = C_ChargeOption2;
        sData = &CHGIC_ReadCmd0x31L;
#endif //BQ25700

#if (CHARGER_IC == BQ24780S)
        CLEAR_MASK(CHGIC_ReadCmd0x37H,ACOKDeglitchTime);
        CLEAR_MASK(CHGIC_ReadCmd0x37L,IFAULT_HI);
        SET_MASK(CHGIC_ReadCmd0x37L,IFAULT_LOW);
        sCmd = C_ChargeOption3;
        sData = &CHGIC_ReadCmd0x37L;
#endif //BQ24780S
        break;

      default:
        return;
        break;
    }
    if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, sCmd, sData,SMBus_NoPEC))
    {
      CHGIC_SMbusFailCnt++;
    }
/*
#if (CHARGER_IC == BQ25700)	  	
    if( IS_MASK_CLEAR(CHGIC_ReadCmd0x31L,EN_IDCHG_REG) || IS_MASK_CLEAR(CHGIC_ReadCmd0x30H,BIT7) )
    {
      bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L,SMBus_NoPEC);
      SET_MASK(CHGIC_ReadCmd0x31L, EN_IDCHG_REG);      //SET_MASK(CHGIC_ReadCmd0x3BL,BIT2);  // BIT2  IDCHG_REG_EN
      bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L, SMBus_NoPEC);
      SET_MASK(CHGIC_ReadCmd0x30H,BIT7);  // BIT15 IDCHG_EN

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
*/
#if (CHARGER_IC == BQ24780S)
    if( IS_MASK_CLEAR(CHGIC_ReadCmd0x37H,EN_IDCHG_REG) || IS_MASK_CLEAR(CHGIC_ReadCmd0x3BH,BIT3) )
    {
      bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargeOption3, &CHGIC_ReadCmd0x37L,SMBus_NoPEC);
      SET_MASK(CHGIC_ReadCmd0x37H, EN_IDCHG_REG);      //SET_MASK(CHGIC_ReadCmd0x3BL,BIT2);  // BIT2  IDCHG_REG_EN
      bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargerMode3B, &CHGIC_ReadCmd0x3BL, SMBus_NoPEC);
      SET_MASK(CHGIC_ReadCmd0x3BH,BIT3);  // BIT11 IDCHG_EN

      CHGIC_ReadCmd0x39L = 0x00;
      CHGIC_ReadCmd0x39H = 0x0A; // Battery discharge limit to 2.5A, ref to bq24780_datasheet_preview_v3p0_LCFC.PDF
      if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_DisChargeCurr, &CHGIC_ReadCmd0x39L,SMBus_NoPEC))
      {
        CHGIC_SMbusFailCnt++;
      }
      if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode3B, &CHGIC_ReadCmd0x3BL,SMBus_NoPEC))
      {
        CHGIC_SMbusFailCnt++;
      }
      if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption3, &CHGIC_ReadCmd0x37L,SMBus_NoPEC))
      {
        CHGIC_SMbusFailCnt++;
      }
    }
#endif //BQ24780S
}

void StopChgIC(void)
{
  SET_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Inhibit Charge
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }
}

#if (CHARGER_IC == BQ25700)
const sRSmbusBStruct code ReadChgIcCmdTable [] =
{   //     command                   REG         no.
  { C_ChargeCurrent,    &CHGIC_ReadCmd0x14L      ,0x00},
  { C_ChargeVoltage,    &CHGIC_ReadCmd0x15L      ,0x00},
  { C_InputCurrent,     &CHGIC_ReadCmd0x3FL      ,0x00},
  { C_ChargerMode,      &CHGIC_ReadCmd0x12L      ,0x00},
  { C_ChargeOption2,    &CHGIC_ReadCmd0x31L      ,0x00},      
};
#endif  //BQ25700

#if (CHARGER_IC == BQ24780S)
const sRSmbusBStruct code ReadChgIcCmdTable [] =
{   //     command                   REG         no.
  { C_ChargeCurrent,    &CHGIC_ReadCmd0x14L      ,0x00},
  { C_ChargeVoltage,    &CHGIC_ReadCmd0x15L      ,0x00},
  { C_InputCurrent,     &CHGIC_ReadCmd0x3FL      ,0x00},
  { C_ChargerMode,      &CHGIC_ReadCmd0x12L      ,0x00},
  { C_ChargeOption3,    &CHGIC_ReadCmd0x37L      ,0x00},      
};
#endif  //BQ24780S
void ReadSmartChgIC(void)
{
  {
    if(!bRWSMBus(SMbusChB, SMbusRW, Charger_Addr,
           ReadChgIcCmdTable[CHGIC_ptr].cmd,
           ReadChgIcCmdTable[CHGIC_ptr].smbdataA,
             SMBus_NoPEC))
    {  // SMBUS FAIL
      CHGIC_SMbusFailCnt++;
    }
  }
}

void Chk_Shutdown_Volt(void)
{

#if (BatLowVot_Rule == LowVot_LGNotLG)
  if( (ManufacturerName[0] == 'L' ) && ( ManufacturerName[1] == 'G' ) )
  { BatLowVoltage = LG_3SLowVot ; }    // Shutdown Voltage 8.25V  for LG 3S battery
  else
  { BatLowVoltage = Sanyo_3SLowVot; }  // Shutdown Voltage 8.5V for Sanyo battery
#endif

#if (BatLowVot_Rule == LowVot_3S4S)
  ITempW01 = (WORD)((DesignVoltH<<8)+DesignVoltL);    //get design voltage

  if (ITempW01 >= BAT3S4SVot_Divide)
  { BatLowVoltage = BAT4SLowVot; }   // Shutdown Voltage 11.9V  for 4S battery
  else
  { BatLowVoltage = BAT3SLowVot; }   // Shutdown Voltage 8.9V  for 3S battery
#endif

  ITempW01 = (WORD)((VoltageH<<8)+VoltageL);  // get now voltage

  if (ITempW01 <= BatLowVoltage)  //  shutdown Voltage
  {
    BatLowCnt++;
    if ( (BatLowCnt>=BATLowVotCut_Time)&&((SystemIsS0)||(SystemIsS3)) )
    {
      SET_MASK(SysStatus,ERR_ShuntDownFlag);
      ProcessSID(BATTLOWVOLT_ID);
      RSMRST_shutdown();  //shutdown (RSMRST HI-->low-->HI)
    }
    else if ( (BatLowCnt>=BATLowVotCut_Time)&&(SystemIsS5)&&(Read_EC_ON()) )
    {
        SET_MASK(Thro_Status2,b5BAT_TooLow);
        Check_EC_ON();
     }
  }
  else
  { BatLowCnt =0; }
}


void Chk_BAT1PERCL_5(void)
{
  if ((RelativeStateOfChgL <= 5)&&(IS_MASK_SET(BattStatusL,Dsg)))  // BAT1PERCL <= 5%
  {
    cBATTThrottling = CPUMAXPSTATE;
    SET_MASK(BatteryAlarm,BATPercl_5);
  }
  else
  {
    if (IS_MASK_SET(BatteryAlarm,BATPercl_5))
    {
      CLEAR_MASK(BatteryAlarm,BATPercl_5);
      if (BatteryAlarm == 0)
      {
        cBATTThrottling = 0;
      }
    }
  }
}


void Chk_BatSMbusFailCount(void)
{
  BYTE sCmd, *sData;

  if (nBattErrorCnt==0)
  {
    CLEAR_MASK(nStopChgStat3L,ENCOMMFAIL);      // clear bat communication fail and clear STOP charge
    CLEAR_MASK(Bat1_FPChgFlag,BIT(0));
    Chk_BAT1PERCL_5();

  #if (PLATFORM_CHIP == INTEL) 
    if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))    // discharge
    {
      Chk_Shutdown_Volt();
    }
  #endif

    if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeProjectBATWake) )
    {
      if( pWakeCnt  <= 4 )
      {
        pWakeCnt++;
      }
      else
      {
        CLEAR_MASK(pEdgeProjectFlag1,pEdgeProjectBATWake);
        pWakeCnt=0;
      }
    }

    return;
  }

  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))    // discharge
  {

    RST_ChgTimeOutCnt();

    if (nBattErrorCnt==30)
    {
      cBATTThrottling = CPUMAXPSTATE;

      SET_MASK(SysStatus,ERR_ShuntDownFlag);
      RemainCapL = 0;
      RemainCapH = 0;
      SMbusFailCnt2++;
    }
    if (nBattErrorCnt>=150)
    {
#if !EN_PwrSeqTest
      cBATTThrottling = CPUMAXPSTATE;
      SMbusFailCnt3++;
      nBattErrorCnt = 151;
      if ((SysPowState==SYSTEM_S0)||(SysPowState==SYSTEM_S3))
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BATTCOMMFAIL_ID);
        RSMRST_shutdown();
      }
#endif
    }
  }
  else
  {
    if (IS_MASK_SET(SYS_STATUS,AC_ADP)&&(!Read_ACOFF()))          // battery charge
    {
      if( pWakeCnt != 0)
      {
        pWakeCnt = 0;
      }
      if ((nBattErrorCnt != 0)&&(nBattErrorCnt < 30))
      {
        //ChargeCurrL = 0x00;      // 256mA
        //ChargeCurrH = 0x01;
        //ChargeVoltL = 0xE0;        // 12V (wake up voltage)  2012.04.13 update by AC.
        //ChargeVoltH = 0x2E;
//        CHGIC_InputCurrentL = 0x00;
//        CHGIC_InputCurrentH = 0x0C;
        SMbusFailCnt3++;
      }
      if ((nBattErrorCnt >= 30))        // disable charge
      {
//        ChargeCurrL = 0x00;      // 0 mA
//        ChargeCurrH = 0x00;
//        ChargeVoltL = 0x00;        // 0 V
//        ChargeVoltH = 0x00;
//        CHGIC_InputCurrentL = 0x00;
//        CHGIC_InputCurrentH = 0x00;
//        RST_ChgTimeOutCnt();
//        SET_MASK(nStopChgStat3L,ENCOMMFAIL);  // Set bat communication fail and STOP charge.
                            //  charge inhibit
        SMbusFailCnt2++;
      }
    }
  }
}


void Chk_CHG_TimeOut(void)      // every 1 min
{
  //if (nBattTempCnt < 10)
  if( (nBattTempCnt < 10)&&(IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeProjectBATWake)) )
    return;
  if (IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit))
    return;
                      // nBattCharCurrent >= 0x01F4  (500mA)
  if((ChargeCurrH > 1) || ((ChargeCurrH == 1) && (ChargeCurrL >= 0xF4)))
  {
    TrickleChgTimeOutCnt = 0;
    FastChgTimeOutCnt++;
    if (FastChgTimeOutCnt >= 720)    // 12hour
    {
      FastChgTimeOutCnt = 721;
      SET_MASK(nStopChgStat3L,ENSTOPCHG);
    }
  }
  else                  // nBattCharCurrent < 0x01F4  (500mA)
  {
    FastChgTimeOutCnt = 0;
    TrickleChgTimeOutCnt++;
    if (TrickleChgTimeOutCnt >= 480)    // 8hour
    {
      TrickleChgTimeOutCnt = 481;
      SET_MASK(nStopChgStat3H,ENTRITIMEOUT);
    }
  }
}

void ChkBattery_abnormal(void)
{
  if(IS_MASK_SET(BATTUPDATEFW,PriBattInhib))
    return;

  switch(ChkBattery_abnormal_status)
  {
    case ChkBattery_abnormal_status_normal:
      if(IS_MASK_SET(CHGIC_ReadCmd0x12L, ChargeInhibit)
        && IS_MASK_CLEAR(CurrentH, BIT7) && (CurrentL > 100) )
      {
        ChkBattery_abnormal_count++;
        if (ChkBattery_abnormal_count>5)
        {
          ChkBattery_abnormal_status = ChkBattery_abnormal_status_error;
        }
      }
      else
      {
        ChkBattery_abnormal_status = ChkBattery_abnormal_status_normal;
        ChkBattery_abnormal_count = 0;
      }
      break;
    case ChkBattery_abnormal_status_error:
      // Reset until EC power down
      SET_MASK(ACOFF_SOURCE, CHARGECURRENT);
      ACOFF_HI();
      break;
    default:
      ChkBattery_abnormal_status = ChkBattery_abnormal_status_normal;
      ChkBattery_abnormal_count = 0;
      break;
  }
}



BYTE SendChallengeToBat(void)
{
  BYTE SMBus_work;
#if Thinkpad_Battery_HW_Auth
  SMBus_work = bWSMBusBlock(SMbusChB, SMbusWBK, SmBat_Addr, 0x27, &LV_HW_BatteryChallengeKey, 17, TRUE);
#endif
  if( SMBus_work ) return TRUE;
  return FALSE;
}

BYTE GetChallengeFromBat(void)
{
#if Thinkpad_Battery_HW_Auth
  if( bRSMBusBlock(SMbusChB, SMbusRBK, SmBat_Addr, 0x28, &LV_HW_BatteryResponseX) ) return TRUE;
#endif
  return FALSE;
}

void LV_BAT_Authentication(void)
{
#if Thinkpad_Battery_HW_Auth
  //unsigned long xdata * byte_sha1_pntr;
  int j;

  if(((WORD)((VoltageH<<8)+VoltageL))< BatLowVoltage)
  { return; }
  //===================================================================
  // Service SHA1 algorithm
  //===================================================================
  switch(Service_Auth_Step)
  {
    case 1:
//      InitializeFix();    // initial H and W(0~4/9~15)
      //SetRandomKey(1);    // initial W(4~8)
      for (j=0;j<4;j++){
        LV_HW_BatteryResponseX[j] = 0x0000;
      }
      for (j=0;j<9;j++){
        LV_HW_BatteryResponseZ[j] = 0x0000;
      }
      CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
      CLEAR_MASK(u4PriBattType,u4PriBattTypeHWauthenticated);
      CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
      AuthenticationFailedDis;
      //Service_Auth_Step=2;
      Service_Auth_Step=21;
      SET_MASK(TWR06,HWauthSupport);
      SET_MASK(uAuthenFlag,uDF_SWstart);
      break;
    case 21:          // SHA1 start send data delay 10 sec
      if(BuildInBat || IS_MASK_SET(OptMfgFunction2L, BIT7)) // Replaceable(0)/Non-replaceable(1)
      {
        //
        // Embedded built-in battery (Non-replaceable)
        // Only support SW authentication.
        //
        if ( IS_MASK_SET(edgeProject4,uECAuthTruePass) && IS_MASK_SET(edgeProject4,uAuthComplete))
        {
          CLEAR_MASK(TWR00,AuthenticationFailed);
          CLEAR_MASK(nStopChgStat3H,NotLenovoBattery);
          NonGenuineBattDis;
          SET_MASK(BattAuthStat,OEMauthPass);
          SET_MASK(BattAuthStat,SWauthPass);
        }
        else
        {
          if ( (pFKTemp >= 30) 
            && (Batt0x3CFailRetry5sTimer != 0xFF) 
            && (Batt0x3CFailRetry5sTimer > 0) ) 
          {
            //
            // A workaround for battery w/ special case. Ex. Panasonic has TDA protection, the BMU would not connectable while low voltage.
            // Set to auth pass while BIOS POST and re-auth again once after 5s.
            //

//            CLEAR_MASK(TWR00,AuthenticationFailed);
            CLEAR_MASK(nStopChgStat3H,NotLenovoBattery); // Keep charging to power up BMU
            NonGenuineBattDis;  // BIOS check this flag for SW auth result.
//            SET_MASK(BattAuthStat,OEMauthPass);
//            SET_MASK(BattAuthStat,SWauthPass);
          } else {
            SET_MASK(TWR00,AuthenticationFailed);
            SET_MASK(nStopChgStat3H,NotLenovoBattery); 
            NonGenuineBattEn;  // BIOS check this flag for SW auth result. 
            CLEAR_MASK(BattAuthStat,OEMauthPass);
            CLEAR_MASK(BattAuthStat,SWauthPass);
          }
        }
        Service_Auth_Step=0;
      }
      else
      {
        // Processed Send challenge key only if We get CH and CV from BIOS.
        if (IS_MASK_SET(LV_HW_BatteryControl,LV_HW_BatteryDataReady))
        {
          SendChallengeToBat();  // send to battery
          if(SHA1failCnt == 0) {
            Service_Auth_Step=22;
          } else {
            Service_Auth_Step=24;
          }
        }
      }
      break;
    case 22:
    case 23:
    case 24:
      Service_Auth_Step++;
      break;
    case 25:
      GetChallengeFromBat();
      Service_Auth_Step=0;
      // Inverse Hi/Lo Byte
      for (j=0;j<4;j++) {
        LV_HW_BatteryResponseX[j] = (((LV_HW_BatteryResponseX[j] & 0xFF00) >> 8)\
                                   | ((LV_HW_BatteryResponseX[j] & 0x00FF) << 8));
      }
      for (j=0;j<9;j++) {
        LV_HW_BatteryResponseZ[j] = (((LV_HW_BatteryResponseZ[j] & 0xFF00) >> 8)\
                                   | ((LV_HW_BatteryResponseZ[j] & 0x00FF) << 8));
      }
      //Compare_Auth_Result();
      if (Ecc_VerifyResponse(LV_HW_BatteryResponseX,LV_HW_BatteryResponseZ,LV_HW_BatteryCheckValue))
      {
        SET_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
        CLEAR_MASK(BattAuthStat,HWauthFail);
        CLEAR_MASK(u4PriBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.
      }
      else
      {
        AuthenticationFailedEn;
        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
        SET_MASK(BattAuthStat,HWauthFail);
        SET_MASK(u4PriBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.

        SHA1failCnt ++;
        if (SHA1failCnt < 3)
        {
          for (j=0;j<4;j++){
            LV_HW_BatteryResponseX[j] = 0x0000;
          }
          for (j=0;j<9;j++){
            LV_HW_BatteryResponseZ[j] = 0x0000;
          }
          Service_Auth_Step = 21;           //if  SHA1failCnt <3, retry       
        }
        else
        {	// Set fail if retry 3
	        AuthenticationFailedEn;
	        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
	        SET_MASK(BattAuthStat,HWauthFail);
	        SET_MASK(u4PriBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.
        
        }
      }
      
      /*if( IS_MASK_SET(edgeProject4,uAuthSWstatus) && IS_MASK_SET(edgeProject4,uSWDone) )
      {
        CLEAR_MASK(BattAuthStat,HWauthFail);
        CLEAR_MASK(u4PriBattType,u4PriBattTypeHWauthenticated);  // for bios check
      }*/

      SET_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
      //SET_MASK(u4PriBattType,u4PriBattTypeHWauthenticated);
      if( IS_MASK_SET(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed)) {
        if ( IS_MASK_SET(edgeProject4,uECAuthTruePass) && IS_MASK_SET(edgeProject4,uAuthComplete) )
        {
          //TWR00.bit.AuthenticationFailed = 0;
          CLEAR_MASK(nStopChgStat3H,NotLenovoBattery); // Keep charging to power up BMU
          CLEAR_MASK(TWR00,AuthenticationFailed);
          SET_MASK(BattAuthStat,OEMauthPass);
          SET_MASK(BattAuthStat,SWauthPass);
          NonGenuineBattDis;
        }
      }
      else
      {
        if ( (pFKTemp >= 30) 
          && (Batt0x3CFailRetry5sTimer != 0xFF) 
          && (Batt0x3CFailRetry5sTimer > 0) ) 
        {
          //
          // A workaround for battery w/ special case. Ex. Panasonic has TDA protection, the BMU would not connectable while low voltage.
          // Set to auth pass while BIOS POST and re-auth again once after 5s.
          //

//            CLEAR_MASK(TWR00,AuthenticationFailed);
          CLEAR_MASK(nStopChgStat3H,NotLenovoBattery); // Keep charging to power up BMU
          NonGenuineBattDis;  // BIOS check this flag for SW auth result.
          CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
//            SET_MASK(BattAuthStat,OEMauthPass);
//            SET_MASK(BattAuthStat,SWauthPass);
        } else {
	        if (SHA1failCnt >= 3)
	        {
          	  //TWR00.bit.AuthenticationFailed = 1;
          	  SET_MASK(TWR00,AuthenticationFailed);
          	  SET_MASK(nStopChgStat3H,NotLenovoBattery); 
          	  CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
          	  SET_MASK(BattAuthStat,HWauthFail);
              SET_MASK(u4PriBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.
	        }
        }
      }
      //Mos: If We're in ACPI_OS mode, Send Q78 to notify and get new Data
      //Otherwise, We need clear LV_HW_BatteryDataReady flag before POST,
      // And get new Data while POST procedure.
      if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
      {
        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryDataReady);
        ECSMI_SCIEvent(GEN_CH_CV_EVENT_78);
      }
      break;
    case 0:
      Service_Auth_Step=0;
      break;
    default:
      Service_Auth_Step ++;
      break;
  }
#endif // Thinkpad_Battery_HW_Auth

}

void Chk_Trickle_Current(void)
{
/*When Current(0x0a)>=700mA,and Charging Current(0x14)<500mA,and Polling cycle=5 cycles,then AC_OFF high, AND
Report RSOC(0x0d)=0 to BIOS.
Reset condition:EC Power down.*/

/*typedef enum
{
  Chk_Trickle_Current_status_normal,
  Chk_Trickle_Current_status_error,
}Chk_Trickle_Current_status_type;
extern XBYTE  Chk_Trickle_Current_status;      //_at_(ECRAM1+0x45);
extern XBYTE  Chk_Trickle_Current_count;      //_at_(ECRAM1+0x46);
extern XWORD  Chk_Trickle_Current_Now_Current;    //_at_(ECRAM1+0x47); //0x47~0x48
extern XWORD  Chk_Trickle_Current_Chg_Current;    //_at_(ECRAM1+0x49); //0x49~0x4A*/

  XWORD Batt_Now_current;
  XWORD BattChargeCurrent;


    if(IS_MASK_SET(BATTUPDATEFW,PriBattInhib))
        return;


  switch(Chk_Trickle_Current_status)
  {
    case Chk_Trickle_Current_status_normal:
      if (IS_MASK_CLEAR(CurrentH, BIT7))
      {
        Batt_Now_current = (WORD)((CurrentH << 8) + CurrentL);//mA
        BattChargeCurrent = (WORD)((ChargeCurrH<< 8) + ChargeCurrL);//mA
        if((Batt_Now_current > Chk_Trickle_Current_Now_Current)
          && (BattChargeCurrent < Chk_Trickle_Current_Chg_Current))
        {
          Chk_Trickle_Current_count++;
        }
        else
        {
          Chk_Trickle_Current_status = Chk_Trickle_Current_status_normal;
          Chk_Trickle_Current_count = 0;
        }
      }
      else
      {
        Chk_Trickle_Current_status = Chk_Trickle_Current_status_normal;
        Chk_Trickle_Current_count = 0;
      }

      if (Chk_Trickle_Current_count>=5)
      {
        Chk_Trickle_Current_status = Chk_Trickle_Current_status_error;
      }
      break;
    case Chk_Trickle_Current_status_error:
      // Reset until EC power down
      SET_MASK(ACOFF_SOURCE, LEAKCURRENT);
      ACOFF_HI();
      break;
    default:
      Chk_Trickle_Current_status = Chk_Trickle_Current_status_normal;
      ChkBattery_abnormal_count = 0;
      break;
  }
}


void ACOffProcess(BYTE ACOffState)
{
  if (ACOffState)
  {
    SET_MASK(cBattFlag0,cCmdAcOff);
    //SET_MASK(ACOFF_SOURCE, ACMD);
    CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
    ACOFF_HI();
    ECSMI_SCIEvent(ACPI_ACOUT_SCI);
  }
  else if (!ACOffState)
  {
    CLEAR_MASK(cBattFlag0,cCmdAcOff);
    //CLEAR_MASK(ACOFF_SOURCE, ACMD);
    SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
    ACOFF_LOW();
    ECSMI_SCIEvent(ACPI_ACIN_SCI);
  }
}

void BATLEARNProcess(BYTE ACOffState)
{
  if (ACOffState)
  {
    SET_MASK(cBattFlag0,cCmdAcOff);
    CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
    ECSMI_SCIEvent(ACPI_ACOUT_SCI);
  }
  else if (!ACOffState)
  {
    CLEAR_MASK(cBattFlag0,cCmdAcOff);
    SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
    ECSMI_SCIEvent(ACPI_ACIN_SCI);
  }
}


//[Project][CC20130425][Start]Add first used date feature.
void ChkFirstUsedDate(void)
{
  if( IS_MASK_SET(uProject0,uFUDWriteEN) )
  {
    if(bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, FirstUsedDate, &p2FUDayTemp,SMBus_NeedPEC))
    {
      OptMfgFunction1H= p2FUDayTemp & 0xFF;
      OptMfgFunction1L= (p2FUDayTemp >> 8) & 0xFF;
    }
    if( (OptMfgFunction1L == 0x00) && (OptMfgFunction1H == 0x00) && (p2FUDay != 0) )
    {
      p2FUDayTemp = (p2FUDay & 0xff) << 8 ;
      p2FUDayTemp = p2FUDayTemp +((p2FUDay & 0xff00) >> 8) ;
      if(bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, FirstUsedDate, &p2FUDayTemp,SMBus_NeedPEC))
      {
        bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, FirstUsedDate, &p2FUDayTemp,SMBus_NeedPEC);
        OptMfgFunction1H= p2FUDayTemp & 0xFF;
        OptMfgFunction1L= (p2FUDayTemp >> 8) & 0xFF;
        if( (OptMfgFunction1L != 0x00) && (OptMfgFunction1H != 0x00) )
        {
          CLEAR_MASK(uProject0,uFUDWriteEN);
          p2FUD_Year = 0;
          p2FUD_Month = 0;
          p2FUD_Day = 0;
          p2FUDayTemp = 0;
          p2FUDay = 0;
        }
      }
    }
    else if( (OptMfgFunction1L != 0x00) && (OptMfgFunction1H != 0x00) )
    {
      CLEAR_MASK(uProject0,uFUDWriteEN);
      p2FUD_Year = 0;
      p2FUD_Month = 0;
      p2FUD_Day = 0;
      p2FUDayTemp = 0;
      p2FUDay = 0;
    }
  }
}

void BatteryFirstUseDay(void)
{
  if( IS_MASK_SET(uProject0,uSysTimeOK) )
  {
    p2FUDayTemp = p2FUD_Year - 0x7BC;  //Year -1980
    if( p2FUDayTemp <= 127 )
    {
      p2FUDay = p2FUDayTemp << 9 ;
    }
    else
    {
      SET_MASK(uProject0,uFUdayFail);
    }
    p2FUDayTemp = p2FUD_Month;  //Month
    if( p2FUDayTemp <= 12 )
    {
      p2FUDay = p2FUDay + (p2FUDayTemp << 5);
    }
    else
    {
      SET_MASK(uProject0,uFUdayFail);
    }
    p2FUDayTemp = p2FUD_Day;  //Day
    if( p2FUDayTemp <= 31 )
    {
      p2FUDay = p2FUDay + p2FUDayTemp;
    }
    else
    {
      SET_MASK(uProject0,uFUdayFail);
    }


  }
  if( IS_MASK_SET(uProject0,uSysTimeOK) && IS_MASK_CLEAR(uProject0,uFUdayFail) )
  {
    SET_MASK(uProject0,uFUDWriteEN);
    CLEAR_MASK(uProject0,uSysTimeOK);
  }
}
//[Project][CC20130425][End]Add first used date feature.


//#if Lenovo_Authentication
/*===========================================================================
 * Name : Authentication
 *
 * Authentication of Lenovo battery
 *
 * Input  :     NONE
 * Output :     NONE
 *===========================================================================*/

void Authentication(void)
{

  uAuthenRPCL = uAuthenRand2;
  uAuthenRPCH = uAuthenRand1;

  if( IS_MASK_SET(uAuthenFlag,uDF_Return) )
  {
    uAuthenNPCtemp = ( ( ( uAuthenRPCL + ( uAuthenRPackL * 0x10 ) )*( uAuthenRPCH + (uAuthenRPackH * 0x10 ) ) ) + 0x339E );//(Rpc_L+Rpack_L*16)*(Rpc_H+Rpack_H*16)+0x339E)
    uAuthenNPC = uAuthenNPCtemp;
  }
  // Verify
  if( ((IS_MASK_SET(uAuthenFlag,uDF_Return) && (uAuthenNPC != uAuthenNPack))|| IS_MASK_SET(uAuthenFlag,uDF_ReadFail)
    || IS_MASK_SET(uAuthenFlag,uDF_ErrorReturn)) && IS_MASK_CLEAR(nStopChgStat3H,NotLenovoBattery) )
  {
    uAuthenRand1 = LeftRotateBYTE(uAuthenRand1,1);
    uAuthenRand2 = LeftRotateBYTE(uAuthenRand2,2);
    uAuthenRand3 = LeftRotateBYTE(uAuthenRand3,3);
    uAuthenRand4 = LeftRotateBYTE(uAuthenRand4,4);
    uAuthenFailCnt++;
    uAuthenFlag = 0;
    CLEAR_MASK(edgeProject4,uLENOVO_Error);
    CLEAR_MASK(edgeProject4,uLenovo_JapanError);
    CLEAR_MASK(edgeProject4,usLenovo_Error);
    //p2OrigaFlag.bit.p2AuthFinish = 0;
    CLEAR_MASK(edgeProject4,uSWDone);  // Retry when fail.
    SET_MASK(uAuthenFlag,uDF_HWstart);
    SET_MASK(uAuthenFlag,uDF_SWstart);  // SW authentication
  }
  else if( IS_MASK_SET(uAuthenFlag,uDF_Return) && (uAuthenNPC == uAuthenNPack) )
  {
    uAuthenFailCnt = 0;
    if(BattExistGet)
    {
      CLEAR_MASK(uAuthenFlag,uDF_HWstart);
      //SET_MASK(BattAuthStat,SWauthPass);
      //CLEAR_MASK(TWR00,AuthenticationFailed);
      CLEAR_MASK(uAuthenFlag,uDF_SWstart);  // SW authentication done.
      CLEAR_MASK(edgeProject4,uAuthSWstatus);  // SW isn't fail, then HW fail won't be set.
      SET_MASK(edgeProject4,uSWDone);
      SET_MASK(edgeProject4,uECAuthTruePass);
      SET_MASK(edgeProject4,uAuthComplete);
    }
  }

  if( uAuthenFailCnt >= 2 )
  {
    //uAuthenFailCnt = 0;
    CLEAR_MASK(uAuthenFlag,uDF_HWstart);
    CLEAR_MASK(uAuthenFlag,uDF_SWstart);
    //BattAuthStat.bit.SWauthPass = 0;    // PM Bit7
    SET_MASK(edgeProject4,uAuthSWstatus);    // SW is fail, then HW fail will set.
    SET_MASK(edgeProject4,uSWDone);
    CLEAR_MASK(edgeProject4,uECAuthTruePass);
    //SET_MASK(TWR00,AuthenticationFailed); 
    
    SET_MASK(TWR00,AuthenticationFailed);
    SET_MASK(nStopChgStat3H,NotLenovoBattery); 
    NonGenuineBattEn;  // BIOS check this flag for SW auth result. 
    CLEAR_MASK(BattAuthStat,OEMauthPass);
    CLEAR_MASK(BattAuthStat,SWauthPass);

  }

  
}

void LAuthProcess(void)
{
  if(((WORD)((VoltageH<<8)+VoltageL))< BatLowVoltage)
  { return; }
  if(BattExistGet && IS_MASK_CLEAR(uAuthenFlag,uDF_Done))
  {
    if( IS_MASK_CLEAR(uAuthenFlag,uDF_Write))
    {
      if( bWSMBusBlock(SMbusChB, SMbusWBK, SmBat_Addr, AuthenticationCmd, &uAuthenRand1, 4, TRUE) )
      {
        SET_MASK(uAuthenFlag,uDF_Write);
        uAuthenReadAfter1Sec = 0;
        pFKTemp=0;
      }
      else
      {
        pFKTemp++;
        if (pFKTemp >= 30) {
          if (Batt0x3CFailRetry5sTimer == 0) {
            Batt0x3CFailRetry5sTimer = 5;
          } else if (Batt0x3CFailRetry5sTimer == 0xFF) {
            // After a 5s retry, 0x3C cmd is still failed.
            // Set battery is illegal.
            SET_MASK(TWR00,AuthenticationFailed);
            SET_MASK(nStopChgStat3H,NotLenovoBattery);
            NonGenuineBattEn;  // BIOS check this flag for SW auth result.			
            CLEAR_MASK(BattAuthStat,OEMauthPass);
            CLEAR_MASK(BattAuthStat,SWauthPass);
          }
        }
      }
    }
    else if( IS_MASK_CLEAR(uAuthenFlag,uDF_Read))
    {
      if( uAuthenReadAfter1Sec >= delayReadCnt )
      {
        if( bRSMBusBlock(SMbusChB, SMbusRBK, SmBat_Addr, AuthenticationCmd, &LV_SW_SIGNERTURE) )
        {
          SET_MASK(uAuthenFlag,uDF_Read);
          SET_MASK(uAuthenFlag,uDF_Done);

          if((LV_SW_SIGNERTURE[0] != 0x4C) || (LV_SW_SIGNERTURE[1] != 0x45) ||
            (LV_SW_SIGNERTURE[2] != 0x4E) || (LV_SW_SIGNERTURE[3] != 0x4F) ||
            (LV_SW_SIGNERTURE[4] != 0x56) || (LV_SW_SIGNERTURE[5] != 0x4F))
          {
            SET_MASK(edgeProject4,uLENOVO_Error);
          }

          if((LV_SW_SIGNERTURE[0] != 0x4C) || (LV_SW_SIGNERTURE[1] != 0x65) ||
          (LV_SW_SIGNERTURE[2] != 0x6E) || (LV_SW_SIGNERTURE[3] != 0x6F) ||
          (LV_SW_SIGNERTURE[4] != 0x76) || (LV_SW_SIGNERTURE[5] != 0x6F) ||
          (LV_SW_SIGNERTURE[6] != 0x20) || (LV_SW_SIGNERTURE[7] != 0x4A) ||
          (LV_SW_SIGNERTURE[8] != 0x61) || (LV_SW_SIGNERTURE[9] != 0x70) ||
          (LV_SW_SIGNERTURE[10] != 0x61) || (LV_SW_SIGNERTURE[11] != 0x6E))
          {
            SET_MASK(edgeProject4,uLenovo_JapanError);
          }
          
          if((LV_SW_SIGNERTURE[0] != 0x4C) || (LV_SW_SIGNERTURE[1] != 0x65) ||
          (LV_SW_SIGNERTURE[2] != 0x6E) || (LV_SW_SIGNERTURE[3] != 0x6F) ||
          (LV_SW_SIGNERTURE[4] != 0x76) || (LV_SW_SIGNERTURE[5] != 0x6F))
          {
            SET_MASK(edgeProject4,usLenovo_Error);
          }

          if( IS_MASK_CLEAR(edgeProject4,uLENOVO_Error) )
          {
            uAuthenRPackL = LV_SW_SIGNERTURE[6];
            uAuthenRPackH = LV_SW_SIGNERTURE[8];
            uAuthenNPack =  (LV_SW_SIGNERTURE[7]+(LV_SW_SIGNERTURE[9]*256));
            SET_MASK(uAuthenFlag,uDF_Return);
          }
          else if( IS_MASK_CLEAR(edgeProject4,uLenovo_JapanError) )
          {
            uAuthenRPackL = LV_SW_SIGNERTURE[12];
            uAuthenRPackH = LV_SW_SIGNERTURE[14];
            uAuthenNPack = (LV_SW_SIGNERTURE[13]+(LV_SW_SIGNERTURE[15]*256));
            SET_MASK(uAuthenFlag,uDF_Return);
          }
          else if( IS_MASK_CLEAR(edgeProject4,usLenovo_Error) )
          {
            uAuthenRPackL = LV_SW_SIGNERTURE[6];
            uAuthenRPackH = LV_SW_SIGNERTURE[8];
            uAuthenNPack = (LV_SW_SIGNERTURE[7]+(LV_SW_SIGNERTURE[9]*256));
            SET_MASK(uAuthenFlag,uDF_Return);
          }

          if( IS_MASK_SET(edgeProject4,uLENOVO_Error) && IS_MASK_SET(edgeProject4,uLenovo_JapanError) && IS_MASK_SET(edgeProject4, usLenovo_Error))
          {
            SET_MASK(uAuthenFlag,uDF_ErrorReturn);
          }

            Authentication();
        }
      }
      else
      {
        uAuthenReadAfter1Sec++;
      }
    }
  }
}

//#endif
// [20140304 ITE CM.Wu]
void Update_ChargeIC(void)
{
  BYTE BakCHGIC_InputCurrentL, BakCHGIC_InputCurrentH;
  WORD AdpWattTemp;    //ivan type-c
  BYTE WTemp;

  // Save origin values.
  BakCHGIC_InputCurrentL = CHGIC_InputCurrentL;
  BakCHGIC_InputCurrentH = CHGIC_InputCurrentH;
  /*
#if Support_TYPE_C
	if(!Read_DCIN_ATTACHED2())
	{	//Write the Type C adaptor output current value to Charger IC "InputCurrent" register.
#if (CHARGER_IC == BQ25700)
		if(CurrentTypeCI != 0)
		{
			if(IS14Inch)
			{ //14"
				WTemp = 0xFF;
			}
			else
			{ //15"
			  if(nSwitchableBTNGET)
			  {	//DIS is 65 watt system
			#if (PLATFORM_CHIP == INTEL)
			    if (N16P_GT){
				    	WTemp = 90;
			    }else
			#endif
			    {
				    	WTemp = 65;
			    }
			  }
			  else
			  {	//UMA is 45 watt system.
				    	WTemp = 45;
			  }
			}
			if(IS_MASK_CLEAR(SYS_MISC1,G3Mode) && IS15Inch && (CurrentTypeCWatt >= WTemp))
			{
			#if (PLATFORM_CHIP == INTEL)
		  		if(WTemp == 90)
		  		{
		    		if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
		        		(BattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
						CHGIC_InputCurrentH = A90WINPUTCURRH_AD - Intel15InputCurrentAD;
			  		}
					else
					{
      					CHGIC_InputCurrentH = A90WINPUTCURRH_A - Intel15InputCurrentA;
					}
						CHGIC_InputCurrentL = A90WINPUTCURRL;
        				InputCurrent_Watt =90;
		  		}
		  		else if(WTemp == 65)
			#else
				if(WTemp == 65)
			#endif
		  		{
					CHGIC_InputCurrentH = A65WINPUTCURRH;
					CHGIC_InputCurrentL = A65WINPUTCURRL;
        			InputCurrent_Watt =65;
		  		}
		  		else
		  		{ //45W
					CHGIC_InputCurrentH = A45WINPUTCURRH;
					CHGIC_InputCurrentL = A45WINPUTCURRL;
        			InputCurrent_Watt =45;
		  		}
			}
			else
			{	//Setting the real adaptor input current to charger IC when G3 to S5.		
			#if (PLATFORM_CHIP == INTEL)
				if(CurrentTypeCWatt >= 90)
				{	//Setting the 90W adaptor input current to charger IC for over 90W adaptor.
		      		if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
		          	(BattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
						CHGIC_InputCurrentH = A90WINPUTCURRH_AD - Intel15InputCurrentAD;
					}
					else
					{
      					CHGIC_InputCurrentH = A90WINPUTCURRH_A - Intel15InputCurrentA;
					}
						CHGIC_InputCurrentL = A90WINPUTCURRL;
          				InputCurrent_Watt =90;
				}
				else if(CurrentTypeCWatt >= 65)		
			#else
				if(CurrentTypeCWatt >= 65)	
			#endif
					
				{
          			CHGIC_InputCurrentH = A65WINPUTCURRH;
          			CHGIC_InputCurrentL = A65WINPUTCURRL;
          			InputCurrent_Watt =65;
				}
				else if(CurrentTypeCWatt >= 60)
				{
          			CHGIC_InputCurrentH = A60WINPUTCURRH;
          			CHGIC_InputCurrentL = A60WINPUTCURRL;
          			InputCurrent_Watt =60;
				}
				else if(CurrentTypeCWatt >= 45)
				{
					CHGIC_InputCurrentH = A45WINPUTCURRH;
					CHGIC_InputCurrentL = A45WINPUTCURRL;
          			InputCurrent_Watt =45;
				}
				else
				{
					AdpWattTemp = CurrentTypeCI / 5;
			    	CHGIC_InputCurrentL = AdpWattTemp >> 8;
			    	CHGIC_InputCurrentH = AdpWattTemp & 0x00FF;
	        		InputCurrent_Watt =CurrentTypeCWatt;
	      		}
		  }
	  }
	  else
	  {
		CHGIC_InputCurrentH = A45WINPUTCURRH;
		CHGIC_InputCurrentL = A45WINPUTCURRL;
    	InputCurrent_Watt =45;
	  }
    #else	
        AdpWattTemp = CurrentTypeCI / 5;	//InputCurrent = I / 50mA(unit) = TypeC_Adaptor_Current(Unit:10mA) / 50mA = CurrentTypeCI * 10/50 = CurrentTypeCI / 5;(For 25700)
	    CHGIC_InputCurrentL = AdpWattTemp >> 8;
	    CHGIC_InputCurrentH = AdpWattTemp & 0x00FF;
	  }
	  else
	  {
		CHGIC_InputCurrentH = A45WINPUTCURRH;
		CHGIC_InputCurrentL = A45WINPUTCURRL;
	  }
 
	  
#endif   //BQ25700
#if (CHARGER_IC == BQ24780S)
		// Need implement
#endif   //BQ24780S
	}
	else
#endif //Support_TYPE_C
	{
#if (PLATFORM_CHIP == INTEL)
#ifdef OEM_Hook_Power_90WSystem_support
    OEM_Hook_Power_90WSystem();
#endif
    if (!(nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) )&& IS15Inch ) {  //DIS 
      if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap >= 45)){
        CHGIC_InputCurrentH = A45WINPUTCURRH;
        CHGIC_InputCurrentL = A45WINPUTCURRL;
        InputCurrent_Watt =45;
      }
	  
    }else{				
      if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
          (BattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
        if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
          CHGIC_InputCurrentH = A45WINPUTCURRH;
          CHGIC_InputCurrentL = A45WINPUTCURRL;
          InputCurrent_Watt =45;
        }else if (u4ACAdaptorCap == 65){
          CHGIC_InputCurrentH = A65WINPUTCURRH;
          CHGIC_InputCurrentL = A65WINPUTCURRL;
          InputCurrent_Watt =65;			
	#if (PLATFORM_CHIP == INTEL)
        }else if (u4ACAdaptorCap>=90){
          CHGIC_InputCurrentH = A90WINPUTCURRH_AD - Intel15InputCurrentAD;
          CHGIC_InputCurrentL = A90WINPUTCURRL;
          InputCurrent_Watt =90;
	#endif
        }
      }else{                                                         
        if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
          CHGIC_InputCurrentH = A45WINPUTCURRH;
          CHGIC_InputCurrentL = 0x00;
          InputCurrent_Watt =45;
        }else if (u4ACAdaptorCap == 65){
          CHGIC_InputCurrentH = A65WINPUTCURRH;
          CHGIC_InputCurrentL = A65WINPUTCURRL;
          InputCurrent_Watt =65;	
	#if (PLATFORM_CHIP == INTEL)
        }else if (u4ACAdaptorCap>=90){
          CHGIC_InputCurrentH = A90WINPUTCURRH_A - Intel15InputCurrentA;
          CHGIC_InputCurrentL = A90WINPUTCURRL;
          InputCurrent_Watt =90;
	#endif
        }
      }
    }
	#endif*/
	#if (PLATFORM_CHIP == AMD)
    if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
      CHGIC_InputCurrentH = A45WINPUTCURRH;
      CHGIC_InputCurrentL = A45WINPUTCURRL;
      InputCurrent_Watt = 45;
    }else if (u4ACAdaptorCap >= 65){
      CHGIC_InputCurrentH = A65WINPUTCURRH;
      CHGIC_InputCurrentL = A65WINPUTCURRL;
      InputCurrent_Watt = 65;
    }  
	#endif

 // }   
  if ((CHGIC_InputCurrentH == BakCHGIC_InputCurrentH) && (CHGIC_InputCurrentL == BakCHGIC_InputCurrentL)) {
    // Values are not changed, no need to do SMBUS write.
    return;
  }

  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_InputCurrent, &CHGIC_InputCurrentL,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }
}
// [20140304 ITE CM.Wu] -
//=============================================================================
void CriticalLowBATWakeS3(void)
{
  LLBWakeEn;
  EC_WAKE_OUT_LOW();
  Delay1MS(1);
  EC_WAKE_OUT_HI();
}

void IBMbatteryStore(void)
{
  if( u4BattInfoID & 0x10 )
  {
    u4BattInfo1 = 0x00;
    u4BattInfo2 = 0x00;
    u4BattInfo3 = 0x00;
    u4BattInfo4 = 0x00;
    u4BattInfo5 = 0x00;
    u4BattInfo6 = 0x00;
    u4BattInfo7 = 0x00;
    u4BattInfo8 = 0x00;
    u4BattInfo9 = 0x00;
    u4BattInfo10 = 0x00;
    u4BattInfo11 = 0x00;
    u4BattInfo12 = 0x00;
    u4BattInfo13 = 0x00;
    u4BattInfo14 = 0x00;
    u4BattInfo15 = 0x00;
    u4BattInfo16 = 0x00;
  }
  else
  {
    switch((u4BattInfoID & 0x0F))
    {
      case 0x00:
        u4BattInfo1 = RemainCapL;
        u4BattInfo2 = RemainCapH;
        u4BattInfo3 = FullChgCapL;
        u4BattInfo4 = FullChgCapH;
        //We do not fetch this data in previous project
        //Battery CMD 0x12
        u4BattInfo5 = AvgTimeToEmptyL;
        u4BattInfo6 = AvgTimeToEmptyH;
        //In K7DE017A project,
        //They're using battery cmd 0x0E to get ABSOLUTE STATE OF CHARGE
        //u4BattInfo7 = RelativeStateOfChgL;
        //u4BattInfo8 = RelativeStateOfChgH;
        //We're using cmd 0x0D to get RELATIVE STATE OF CHARGE
        u4BattInfo7 = RelativeStateOfChgL;
        u4BattInfo8 = RelativeStateOfChgH;
        u4BattInfo9 = AvgCurrentL;
        u4BattInfo10 = AvgCurrentH;
        u4BattInfo11 = VoltageL;
        u4BattInfo12 = VoltageH;
        //Battery CMD 0x13
        u4BattInfo13 = AvgTimeToFullL;
        u4BattInfo14 = AvgTimeToFullH;
        u4BattInfo15 = BattStatusL;//BattStatusL;
        u4BattInfo16 = BattStatusH;//BattStatusH;
        break;
      case 0x01:
        u4BattInfo1 = BattModeL;//BattModeL;
        u4BattInfo2 = BattModeH;//BattModeH;
        u4BattInfo3 = ManufactureAccessL ;//ManufactureAccessL;
        u4BattInfo4 = ManufactureAccessH;//ManufactureAccessH;
        u4BattInfo5 = CycleCountL;//CycleCountL;
        u4BattInfo6 = CycleCountH;//CycleCountH;
        u4BattInfo7 = CurrentL;//CurrentL;
        u4BattInfo8 = CurrentH;//CurrentH;
        u4BattInfo9 = TemperatureL ;//TemperatureL;
        u4BattInfo10 = TemperatureH;//TemperatureH;
        //Battery CMD 0x11
        u4BattInfo11 = RunTimeToEmptyL;
        u4BattInfo12 = RunTimeToEmptyH;
        //Battery CMD 0x3B, Not define in Previous project
        u4BattInfo13 = FETTemperatureL;
        u4BattInfo14 = FETTemperatureH;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x02:
        u4BattInfo1 = DesignCapL;//DesignCapL;
        u4BattInfo2 = DesignCapH;//DesignCapH;
        u4BattInfo3 = DesignVoltL;//DesignVoltL;
        u4BattInfo4 = DesignVoltH;//DesignVoltH;
        //Battery CMD 0x3F
        u4BattInfo5 = OptMfgFunction1L; //First Used Date;
        u4BattInfo6 = OptMfgFunction1H; //First Used Date;
        //Battery CMD 0x1A
        u4BattInfo7 = 0;//SpecInfoL;
        u4BattInfo8 = 0;//SpecInfoH;
        u4BattInfo9 = ManufactureDateL;//ManufactureDateL;
        u4BattInfo10 = ManufactureDateH;//ManufactureDateH;
        u4BattInfo11 = SerialNumberL;//SerialNumberL;
        u4BattInfo12 = SerialNumberH;//SerialNumberH;
        //Battery CMD 0x0C
        u4BattInfo13 = 0;//MaxErroL;
        u4BattInfo14 = 0;//MaxErroH;
        u4BattInfo15 = OptMfgFunction2L ;//OptMfgFunction2L;
        u4BattInfo16 = OptMfgFunction2H ;//OptMfgFunction2H;
        break;
      case 0x04:
        //We do not keep DeviceChemistry at Yx00 project
        u4BattInfo1 = DeviceChemistry[0];//DeviceChemistry1;
        u4BattInfo2 = DeviceChemistry[1];//DeviceChemistry2;
        u4BattInfo3 = DeviceChemistry[2];//DeviceChemistry3;
        u4BattInfo4 = DeviceChemistry[3];//DeviceChemistry4;
        u4BattInfo5 = DeviceChemistry[4];//DeviceChemistry5;
        u4BattInfo6 = DeviceChemistry[5];//DeviceChemistry6;
        u4BattInfo7 = DeviceChemistry[6];//DeviceChemistry7;
        u4BattInfo8 = DeviceChemistry[7];//DeviceChemistry8;
        u4BattInfo9 = DeviceChemistry[8];//DeviceChemistry9;
        u4BattInfo10 = DeviceChemistry[9];//DeviceChemistry10;
        u4BattInfo11 = DeviceChemistry[10];//DeviceChemistry11;
        u4BattInfo12 = DeviceChemistry[11];//DeviceChemistry12;
        u4BattInfo13 = DeviceChemistry[12];//DeviceChemistry13;
        u4BattInfo14 = DeviceChemistry[13];//DeviceChemistry14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x05:
        u4BattInfo1 = ManufacturerName[0];//ManufacturerName1;
        u4BattInfo2 = ManufacturerName[1];//ManufacturerName2;
        u4BattInfo3 = ManufacturerName[2];//ManufacturerName3;
        u4BattInfo4 = ManufacturerName[3];//ManufacturerName4;
        u4BattInfo5 = ManufacturerName[4];//ManufacturerName5;
        u4BattInfo6 = ManufacturerName[5];//ManufacturerName6;
        u4BattInfo7 = ManufacturerName[6];//ManufacturerName7;
        u4BattInfo8 = ManufacturerName[7];//ManufacturerName8;
        u4BattInfo9 = ManufacturerName[8];//ManufacturerName9;
        u4BattInfo10 = 0;//ManufacturerName10;
        u4BattInfo11 = 0;//ManufacturerName11;
        u4BattInfo12 = 0;//ManufacturerName12;
        u4BattInfo13 = 0;//ManufacturerName13;
        u4BattInfo14 = 0;//ManufacturerName14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x06:
        u4BattInfo1 = DeviceName[0];//DeviceName0;
        u4BattInfo2 = DeviceName[1];//DeviceName1;
        u4BattInfo3 = DeviceName[2];//DeviceName2;
        u4BattInfo4 = DeviceName[3];//DeviceName3;
        u4BattInfo5 = DeviceName[4];//DeviceName4;
        u4BattInfo6 = DeviceName[5];//DeviceName5;
        u4BattInfo7 = DeviceName[6];//DeviceName6;
        u4BattInfo8 = DeviceName[7];//DeviceName7;
        u4BattInfo9 = 0;
        u4BattInfo10 = 0;
        u4BattInfo11 = 0;
        u4BattInfo12 = 0;
        u4BattInfo13 = 0;
        u4BattInfo14 = 0;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x07:
        //Battery CMD 0x2F, Not Define in  previous project
        u4BattInfo1 = OptMfgFunction5[0];//OptMfgFunction5_1;
        u4BattInfo2 = OptMfgFunction5[1];//OptMfgFunction5_2;
        u4BattInfo3 = OptMfgFunction5[2];//OptMfgFunction5_3;
        u4BattInfo4 = OptMfgFunction5[3];//OptMfgFunction5_4;
        u4BattInfo5 = OptMfgFunction5[4];//OptMfgFunction5_5;
        u4BattInfo6 = OptMfgFunction5[5];//OptMfgFunction5_6;
        u4BattInfo7 = OptMfgFunction5[6];//OptMfgFunction5_7;
        u4BattInfo8 = OptMfgFunction5[7];//OptMfgFunction5_8;
        u4BattInfo9 = OptMfgFunction5[8];//OptMfgFunction5_9;
        u4BattInfo10 = OptMfgFunction5[9];//OptMfgFunction5_10;
        u4BattInfo11 = OptMfgFunction5[10];//OptMfgFunction5_11;
        u4BattInfo12 = OptMfgFunction5[11];//OptMfgFunction5_12;
        u4BattInfo13 = OptMfgFunction5[12];//OptMfgFunction5_13;
        u4BattInfo14 = OptMfgFunction5[13];//OptMfgFunction5_14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x08:
        //Battery CMD 0x35
        u4BattInfo1 = FirmwareUpdateStatusL;
        u4BattInfo2 = FirmwareUpdateStatusH;
        //Battery CMD 0x37, But we did not fetch data by this command
        u4BattInfo3 = BatterymakerIDL;
        u4BattInfo4 = BatterymakerIDH;
        //argument of Battery CMD 0x37
        u4BattInfo5 = HardwareIDL;
        u4BattInfo6 = HardwareIDH;
        //argument of Battery CMD 0x37
        u4BattInfo7 = FirmwareversionL;
        u4BattInfo8 = FirmwareversionH;
        //argument of Battery CMD 0x37
        u4BattInfo9 = DataVersionL;
        u4BattInfo10 = DataVersionH;
        //Not define in previous project
        u4BattInfo11 = WarrantyPeriodL;
        u4BattInfo12 = WarrantyPeriodH;
        u4BattInfo13 = ChargeCurrL;
        u4BattInfo14 = ChargeCurrH;
        u4BattInfo15 = ChargeVoltL;
        u4BattInfo16 = ChargeVoltH;
        break;
      case 0x0C:
        //Battery CMD 0x35
        u4BattInfo1 = FirmwareUpdateStatusL;
        u4BattInfo2 = FirmwareUpdateStatusH;
        //Battery CMD 0x37, But we did not fetch data by this command
        u4BattInfo3 = BatterymakerIDL;
        u4BattInfo4 = BatterymakerIDH;
        //argument of Battery CMD 0x37
        u4BattInfo5 = HardwareIDL;
        u4BattInfo6 = HardwareIDH;
        //argument of Battery CMD 0x37
        u4BattInfo7 = FirmwareversionL;
        u4BattInfo8 = FirmwareversionH;
        // argument of Battery CMD 0x37
        u4BattInfo9 = DataVersionL;
        u4BattInfo10 = DataVersionH;
        // Not define in previous project
        u4BattInfo11 = WarrantyPeriodL;
        u4BattInfo12 = WarrantyPeriodH;
        u4BattInfo13 = ChargeCurrL;
        u4BattInfo14 = ChargeCurrH;
        u4BattInfo15 = ChargeVoltL;
        u4BattInfo16 = ChargeVoltH;
        break;
    }

  }
}

void TWR_BATClear(void)
{
  PORT_BYTE_PNTR bat_pntr;
  BYTE i,j;

  for( i=0; i < 0x0C; i++)
  {
    bat_pntr = ((&TWR10+2)+i*16); // Base on address 0x0AA0.
    for( j=2; j<=16; j++ ) // Clear 0AA0F~0AAF.
    {
      *bat_pntr=0;
      bat_pntr++;
    }
  }
}

void TPG_EXTBATCmd(void)
{
  /*==============A39 Ext Command for Battery Start====================================*/
  switch(u4ExtCommand)
  {
    case 0x01:
      // Reset battery charge inhibit
        CLEAR_MASK(nStopChgStat3L,ENCMDSTOPCHG);
        Inhibit_chargingCLR;
        PriBattinhibitDis;//PriBattinhibitEn;
        pInhitTimer = 0; // clear Inhibit timer.
        u4InhChgTimerL = 0;
        u4InhChgTimerH = 0;
//        p2SetInhChgTimerDis;
        break;
    case 0x02:
      // Peakshift inhibit charge.
        if (pInhitTimer != 0x0000) {
          SET_MASK(nStopChgStat3L,ENCMDSTOPCHG);
          Inhibit_chargingSET;  
          PriBattinhibitEn;
        }
        break;
    case 0x03:
      // Stop 'Peakshift discharge' and 'battery refresh'.
        if( Read_BATLEARN() )
        {
          BAT_LEARN_OFF();
        }
        if( nAcOffCmdGET )
        {
          BATLEARNProcess(nAcOffCmdDIS);
        }
      //    u4PeriSTS4 &= 0xEF;
        PriBattDisChgDis;
      //    uBattMode.bit.uThreshold = 0;
      //    uBattMode.bit.uRefreshmode = 0;
        CLEAR_MASK(pBattLearn,uACoffstchg);
        CLEAR_MASK(pBattLearn,uRefreshmode);
        CLEAR_MASK(nStopChgStat3L,ENCUSTOMER);
        break;
    case 0x04:
      // Peakshift discharge
        //if( !pwrACactive || cSysStus.bit.cSS_S3 || (nBatteryStatH(CM_1st)&0x04) )
        if( !Read_AC_IN() || (SysPowState == SYSTEM_S3) ||  IS_MASK_CLEAR(PriBattSTS, BattExist) )
        {
      if( Read_BATLEARN() )
      {
            BAT_LEARN_OFF();
      }
      if( nAcOffCmdGET )
      {
        BATLEARNProcess(nAcOffCmdDIS);
      }
          u4PeriSTS4 &= 0xEF;
        }
        else
        {
      if( (!Read_BATLEARN()) )
      {
        BAT_LEARN_ON();
      }
      if( (!nAcOffCmdGET) )
      {
        BATLEARNProcess(nAcOffCmdEN);
      }
          u4PeriSTS4 |= BIT4;
        }
        break;
    case 0x05:
      // Inhibit to charge a primary battery
        Inhibit_chargingSET;
        PriBattinhibitEn;
        break;
    case 0x07:
    //    u4PeriSTS4 |= 0x14;
        PriBattDisChgEn;
        CLEAR_MASK(pBattLearn,us4ResumeACOFF);
        SET_MASK(pBattLearn,uACoffstchg);
        SET_MASK(pBattLearn,uRefreshmode);
        if( !Read_BATLEARN() )
        {
          BAT_LEARN_ON();
        }
        if( (!nAcOffCmdGET) )
        {
          BATLEARNProcess(nAcOffCmdEN);
        }
        SET_MASK(nStopChgStat3L,ENCUSTOMER);
        break;
    case 0x10:  // Start gravity sensor(G-sensor) diagnotic program
		GSensor_DIAG();

        break;
  }
  if( u4ExtCommand )
  {
    u4ExtCommand = 0;
  }
  /*==============A39 Ext Command for Battery End====================================*/
}


void EDGEPMBATTProcess(void)
{
  if( ( u4Cfgspace3 & BIT2 ) == BIT2 )  // 0x03 bit2=1,Charge stop capacity
  {
    if( uThresholdSetting == 0x80 )
    {
      pEdgeBATStopCharge  = 101;
    }
    else if (uThresholdSetting & SET_CHARGE_CAPACITY_FLAG)
    {
      pEdgeBATStopCharge = ( uThresholdSetting & 0x7F );
    }
    if( pEdgeBATStopCharge != (Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStopCharge))){
      WRITE_DATA2EEPROM(&pEdgeBATStopCharge , 1 , EEPROM_Bank7, EEPROM_BATStopCharge);
    }
  }
  else  // 0x03 bit2=0,Charge start capacity
  {
    if( uThresholdSetting == 0x80 )
    {
      pEdgeBATStartCharge  = 95;
    }
    else if (uThresholdSetting & SET_CHARGE_CAPACITY_FLAG)
    {
      pEdgeBATStartCharge = ( uThresholdSetting & 0x7F );
    }
    if( pEdgeBATStartCharge != (Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_BATStartCharge))){
      WRITE_DATA2EEPROM(&pEdgeBATStartCharge , 1 , EEPROM_Bank7, EEPROM_BATStartCharge);      
    }
  }

  uThresholdSetting &= (~SET_CHARGE_CAPACITY_FLAG); // clear BIT7
// 0xA24 and 0xA25
// Battery Charge start/stop threshold setting of the primary(0x24)/seconday(0x25) battery.
// When bit 2 of configuration space 3 is 0, bit 0 - 6 represent the capacity to start to charge.
// Otherwise bit 0-6 represent the capacity to stop charging. 
// Bit 7 is needed to set to 1 when a value is written to this byte. 
// When the command is done, bit 7 is cleared.

  //if( RelativeStateOfChgL <= pEdgeBATStartCharge )
  //{
  //  PriBattinhibitDis;//PriBattinhibitEn;
  //  CLEAR_MASK(nStopChgStat3H, PMTHRESHOLD);
  //}
  //else if( RelativeStateOfChgL >= pEdgeBATStopCharge )
  //{
  //  PriBattinhibitEn;//PriBattinhibitDis;
  //  SET_MASK(nStopChgStat3H, PMTHRESHOLD);
  //}
}

//(JSN)+
void TWR_Initial(void)
{
  BYTE i;
  BYTE * ptr;
//    if(Edge_NAMESPACE_SW!=1)
//    {
        if(BattExistGet)
        {
#if !Support_TWO_BATT        
          SecondaryBattAttachedDis;    //TWR00.bit.SecondaryBattAttached = 0;
#endif

          //if( p2OrigaFlag.bit.p2AuthFinish )
#if Thinkpad_Battery_HW_Auth
  #if 0
          if( IS_MASK_SET(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished))
          {
            //if( IS_MASK_SET(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed))
            if( IS_MASK_SET(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed)
          && ( IS_MASK_SET(edgeProject4,uECAuthTruePass) && IS_MASK_SET(edgeProject4,uAuthComplete) )
          )
            {
              //TWR00.bit.AuthenticationFailed = 0;
          CLEAR_MASK(TWR00,AuthenticationFailed);
          SET_MASK(BattAuthStat,OEMauthPass);
          SET_MASK(BattAuthStat,SWauthPass);
              NonGenuineBattDis;
            }
            else
            {
              //TWR00.bit.AuthenticationFailed = 1;
              CLEAR_MASK(BattAuthStat,OEMauthPass);
          CLEAR_MASK(BattAuthStat,SWauthPass);
              SET_MASK(TWR00,AuthenticationFailed);
              NonGenuineBattEn;
            }
  #endif
#endif
          //AttachedPowerNOTsupportDis; //TWR00.bit.AttachedPowerNOTsupport = 0;
    //(TBD)      TWR01.bit.Commestablished = !(nBattCommFailGET(CM_1st));
            //TWR01.bit.CriticalLow = TWR01.bit.BatteryCritical;
          if( BatteryCriticalGET )
            {
                CriticalLowEn;
            }
            else
            {
                CriticalLowDis;
            }
          ErrorDis;   //TWR01.bit.Error = 0;
          TWRvalidEn; //TWR01.bit.TWRvalid = 1;
    //(TBD)      TWR01.bit.CapacityMode = (BattModeH & 0x80);
        }
        else
        {
          TWR00 &= TWR_00;
          TWR01 = 0x00;
        }

  for (i=1;i<0x0D;i++)
  {
    ptr = &TWR00;
    *(ptr + (i*0x10)) = TWR00;
    *(ptr + (i*0x10) + 1) = TWR01;
  }

}

void Battery_Initial(void)
{    
  u4HighBattLV = BATMAXLVL;  // update battery hightest level
  capSmartBattEn;

  MajorVer = 0x01;
  MinorVer = 0x03;
  TWRInfo = 0xE0;
}
void Battery_RSOCLowNotifyOS(void)
{
if((RelativeStateOfChgL<11)&&(BattExistGet)&&(!Read_AC_IN())&&(IS_MASK_SET(SYS_MISC1, ACPI_OS))&&(IS_MASK_CLEAR(pEdgeProjectFlag3, pEdgeProjectRSOClow)))
{ 
SET_MASK(pEdgeProjectFlag1,pEdgeProjectBATChange);
SET_MASK(pEdgeProjectFlag3,pEdgeProjectRSOClow);

}
else
{
	CLEAR_MASK(pEdgeProjectFlag3,pEdgeProjectRSOClow);

}

}

void CHK_Capacity_trip_point (void)
{
#if SupportCHKtrippoint
   if (BattExistGet && (u4TripPointCapacity0L == RemainCapL) && (u4TripPointCapacity0H == RemainCapH))
    { ECSMI_SCIEvent(BAT0_Cap_event);}
   
#if Support_TWO_BATT
   if (SecBattExistGet && (u4TripPointCapacity1L == RemainCap1L) && (u4TripPointCapacity1H == RemainCap1H))
    { ECSMI_SCIEvent(BAT1_Cap_event);}
#endif

#endif //SupportCHKtrippoint
}

#if (CHARGER_IC == BQ25700)
void ChargerIC_Initial(void)
{  
#if (CHARGER_IC == BQ25700)
  for( CHGIC_ptr = 3; CHGIC_ptr < 0x05; CHGIC_ptr++ )
  {
		ReadSmartChgIC();
	}
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L, SMBus_NoPEC);
  SET_MASK(CHGIC_ReadCmd0x12H,SwitchingFreq);   //Set frequence to 800KHz
//  CLEAR_MASK(CHGIC_ReadCmd0x12L,EN_LDO);          //Cathy for power request.
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L, SMBus_NoPEC);
//  CLEAR_MASK(CHGIC_ReadCmd0x31L,IFAULT_HI);
//  SET_MASK(CHGIC_ReadCmd0x31L,IFAULT_LOW);
  CLEAR_MASK(CHGIC_ReadCmd0x31L,EN_EXTILIM);
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L, SMBus_NoPEC);
  CLEAR_MASK(CHGIC_ReadCmd0x30L,AUTO_WAKEUP_EN);
   SET_MASK(CHGIC_ReadCmd0x30H,BIT4);
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L, SMBus_NoPEC);
  SET_MASK(CHGIC_ReadCmd0x33L,VSYS_T0); 
  SET_MASK(CHGIC_ReadCmd0x33L,VSYS_T1);
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }

  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L, SMBus_NoPEC);
  CLEAR_MASK(CHGIC_ReadCmd0x34L,BATPRES); 
  CLEAR_MASK(CHGIC_ReadCmd0x34L,VSYS);
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL, SMBus_NoPEC);
  CHGIC_ReadTempH = 0x02;
  CHGIC_ReadTempL = 0xC0;
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  SET_MASK(CHGIC_Status,ChgIcInitial);
#endif //BQ25700  
}

void ChargeIC_InitACDC(void){
  
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L, SMBus_NoPEC);
  bRWSMBus(SMbusChB, SMbusRW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L, SMBus_NoPEC);
  
  if((IS_MASK_SET(SYS_STATUS, AC_ADP) || (ADPID_ON_Detect())) && !BattExistGet){
    CLEAR_MASK(CHGIC_ReadCmd0x12L,IDPM_EN);
    SET_MASK(CHGIC_ReadCmd0x12H,IDPM_AUTO_DIS);
  }
  if(BattExistGet){
    SET_MASK(CHGIC_ReadCmd0x12L,IDPM_EN);
    SET_MASK(CHGIC_ReadCmd0x12H,IDPM_AUTO_DIS);

  }
 
  if((IS_MASK_SET(SYS_STATUS, AC_ADP) || (ADPID_ON_Detect())) && BattExistGet){
  	SET_MASK(CHGIC_ReadCmd0x34L,BATPRES); 
    SET_MASK(CHGIC_ReadCmd0x34L,VSYS);
	 SET_MASK(CHGIC_ReadCmd0x34L,IDCHG);
	 CHGIC_ReadCmd0x34H=0X3D;
  	}else{
  	CLEAR_MASK(CHGIC_ReadCmd0x34L,BATPRES); 
    CLEAR_MASK(CHGIC_ReadCmd0x34L,VSYS);
	SET_MASK(CHGIC_ReadCmd0x34L,IDCHG);
	CHGIC_ReadCmd0x34H=0X3D;
  }
	
  
  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }  

  if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
}
#endif
