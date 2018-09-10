/*-----------------------------------------------------------------------------
 * TITLE: OEM_LPC.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

/*****************************************************************************/
// Procedure: MuteProcess                    TimeDiv: 50mSec
// Description:
// GPIO: GPIOJ1
// Referrals:
/*****************************************************************************/
void MuteProcess(void)
{
  //if ( SystemIsS0 )
  if(Read_PCH_PWR_GOOD())
  {  // Check ISCT and command 94 status.
    if( MuteCount == 0 )
    {
      //if ( (IS_MASK_CLEAR(uISCT_2, b3ISCT_MUTE)) && (IS_MASK_CLEAR(cCmd, b5VolMute)) )
      if ((!MutevolumeEGet)
#if SW_ISCT
        && (IS_MASK_CLEAR(uISCT_2, b3ISCT_MUTE))
#endif // SW_ISCT
        )
      {
        EC_MUTE_INACT();
        return;
      }
    }
    else
    { MuteCount--; }
  }
  EC_MUTE_ACT();
}

/*****************************************************************************/
// Procedure: CameraProcess                  TimeDiv: 100mSec
// Description:
// GPIO: 
// Referrals:
/*****************************************************************************/
void CameraProcess(void)
{
  #if Support_WebCam
  if ( SystemIsS0 )
  {

  }
  #endif  // Support_WebCam
}

/*****************************************************************************/
// Procedure: WirelessProcess                  TimeDiv: 100mSec
// Description:
// GPIO: 
// Referrals: disable and enable Wireless.
/*****************************************************************************/
void WirelessProcess(void)
{

  if ( SystemIsS0 && (IS_MASK_SET(SYS_MISC1,ACPI_OS)) )
  {
#if Support_WLAN_ON_OFF_control  
  //WLAN
    if( WLANstsGet )
    {
      BTRF_OFF_ON();
    }
    else
    {
      BTRF_OFF_OFF();
    }
#endif    

#if Support_BT_ON_OFF_control
   //BT
    if( BTstsGet )
    {
      BT_OFF_ON();
    }
    else
    {
      BT_OFF_OFF();
    }   
#endif 
  }
 
}

#if Support_IFFS
void IFFSProcess(void)
{
  if (uIffsCnt == 0 ) return;
  else uIffsCnt--;

  if (uIffsCnt==0)
  {
    CLEAR_MASK(uCritBattWakeThre,IFFS_Enable);
    if (SystemIsS3)
    {
      if (RelativeStateOfChgL < uCritBattWakeThre)
      {
        SET_MASK(uCritBattWakeThre,IFFS_Enable);
              PWSeqStep = 1;
        PowSeqDelay = 1;
              SysPowState=SYSTEM_S3_S0;
      }
    }
  }
}
#endif // Support_IFFS

void SetACIN_Int(void)
{
  if( SysPowState == SYSTEM_S0 || SysPowState == SYSTEM_S5)
  {
    ISR13 = Int_WKO101;
    SET_MASK(IELMR13,Int_WKO101);
    SET_MASK(WUEMR10,WUC_WUI53);
    if (!Read_AC_IN() || SysPowState == SYSTEM_S5)
    {
      CLEAR_MASK(IPOLR13,Int_WKO101);    // rising
    }
    else
    {
      SET_MASK(IPOLR13,Int_WKO101);    // falling
    }
    ISR13 = Int_WKO101;

    SET_MASK(IER13, Int_WKO101);
  }
}

void SetPWRSW_Int(void)
{
///  GPCRB3  = ALT;  //  ON/OFF
  SET_MASK(IELMR1,Int_WKO25);
  SET_MASK(WUEMR2,WUC_PWRSW);
///  SET_MASK(WUENR2,WUC_PWRSW);
  CLEAR_MASK(IPOLR1,Int_WKO25);        // falling
  SET_MASK(IER1, Int_WKO25);        // Enable PWRSW Interrupt
}
void SetPD_Int(void)
{
///  GPCRB3  = ALT;  //  ON/OFF
  SET_MASK(IELMR16,Int_WKO128);
  SET_MASK(WUEMR14,WUC_WUI80);
///  SET_MASK(WUENR2,WUC_PWRSW);
  CLEAR_MASK(IPOLR16,Int_WKO128);        // falling
  SET_MASK(IER16, Int_WKO128);        // Enable PWRSW Interrupt
}


void SetExTimer1(void)
{

  ETPSR = 0x00;
  ETCNTLHR = 0x7F;//0x06;
  ETCNTLLR = 0xF8;//0x66;          //Period 1 sec  //Period 50ms
  IER3 |= 0x40;              //ExTimer1
  /*
    ETPSR = 0x01;
    ETCNTLHR = 0x04;
    ETCNTLLR = 0x00;          //Period 31.25ms(26.91ms, Do Not Modify)
    IER3 |= 0x40;            //ExTimer1
  */


}

void SetVGA_AC_DET(void)
{
#if ( (PLATFORM_CHIP == INTEL) && (SUPPORT_DSx) )
  if( SystemIsS0 || SystemIsS3 || (Read_DPWROK()&&SystemIsS5))  //DS3
#else // PLATFORM_CHIP
  if( SystemIsS0 || SystemIsS3 || SystemIsS5)  //DS3
#endif // PLATFORM_CHIP
  {
    //if( pACDCCnt == 0 )
    {
//       if (IS_MASK_SET(pEdgeProjectFlag2,pEdgeGPUOTTHROT)//(!Read_AC_IN() || IS_MASK_SET(pEdgeProjectFlag2,pEdgeGPUOTTHROT)
//         || IS_MASK_SET(pEdgeProjectFlag2,pEdgeGPUOCTHROT) || Read_ACOFF() )
      if ((GPU_THROTTLE_SRC != 0) || Read_ACOFF())
      {
        AC_PRESENT_LOW();
        //VGA_AC_BATT_OFF();
        //VGA_AC_DET_LO();
        pACDCCnt = 20;
      }
      else
      {
        if(Read_AC_IN()) {
          AC_PRESENT_HI();
		  	
		#if (PLATFORM_CHIP == AMD)		  
        }else{
          AC_PRESENT_LOW();
		#endif

        }
        //VGA_AC_BATT_ON();
        //if(SystemIsS0)
        //{ VGA_AC_DET_HI(); }
        //else
        //{ VGA_AC_DET_LO(); }
        pACDCCnt = 20;
      }
    }
  }
}


void WakeUp_DO_Function(void)
{
  if(ExtTimeCnt == 1)
  { ExtTimeCnt=0; }
  else
  { ExtTimeCnt++; }

  ChkBattery_Percl();

  Init_VC(3);    // CMW Temp
}

/*****************************************************************************/
// Procedure: ISCT_Process                  TimeDiv: 500mSec
// Description: Intel Smart Connect Technology process
// GPIO:
// Referrals:
/*****************************************************************************/
#if SW_ISCT
void ISCT_Process(void)
{
  if( (IS_MASK_SET(AOAC_STATUS, ISCT_Enable)) && (SystemNotS5) )    // Check ISCT enable?
  {
    if(BattExistGet)       // Check BAT1 exist.
    {
      if( RelativeStateOfChgL <= ISCT_BAT_LOW )      // Check BAT under 20%.
      { SET_MASK(AOAC_STATUS, ISCT_BAT_Capy); }  // Battery under 20%
      else
      { CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Capy); }  // Battery over 20%

      // Check BAT over 50 degree amd under -5 degree.
      //if( ( ( nBattAverTemp >= ISCT_BAT_OvTpLimit ) && !( nBattAverTemp & BATUnZeroTemp ) )
      //  || ( (nBattAverTemp & BATUnZeroTemp) && ( nBattAverTemp <= ISCT_BAT_UnTpLimit ) ) )
      // Check BAT over 50 degree.
      if( nBattAverTemp >= ISCT_BAT_OvTpLimit )
      { SET_MASK(AOAC_STATUS, ISCT_BAT_Temp); }  // Battery Over 50 and under -5 degree.
      else
      { CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Temp); }  // Battery under 50 and over -5 degree.
    }
    else
    {
      CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Capy);    // Clear battery capacity of fail bit.
      CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Temp);    // Clear battery temperature of fail bit.
    }

    if( SystemIsS3 )
    {
      if ( (AOAC_STATUS & 0xC0) != 0 )
      {
        SET_MASK(AOAC_STATUS, ISCT_S0_ISCT);  // Set S0_ISCT
        PM_PWRBTN_LOW();
      }
      ISCT_Behavior();
    }
  }
  else
  {
    AOAC_STATUS &= 0xC0;    // Clear ISCT status.
    ISCT_Behavior();
  }

  //#endif  // SW_ISCT
}

/*****************************************************************************/
// Procedure: ISCT_Behavior
// Description: Intel Smart Connect Technology mode behavior.
// GPIO:
// Referrals:
/*****************************************************************************/
void ISCT_Behavior(void)
{
  if( (IS_MASK_SET(AOAC_STATUS, ISCT_S0_ISCT)) && (SystemNotS5) )      // Can do AOAC.
  {
//      AOAC_PWRON();            // ISCT Turn on WLAN.
    uISCT |= 0x10;            // bit4:Disable FAN, bit5:Disable BKOFF.
    uISCT_2 |= 0x20;          // bit3:Disable Mute, bit4:Disable Camera, bit5:Disable Power LED.
    SET_MASK(pPROCHOT, b1ISCT_PROCHOTon);  // Set CPU Prochot function.
  }
  else
  {
//      AOAC_PWRON();            // ISCT Turn on WLAN.
    uISCT &= 0xCF;            // bit4:Enable FAN, bit5:Disable BKOFF.
    uISCT_2 |= 0xC7;          // bit3:Disable Mute, bit4:Enable Camera, bit5:Enable Power LED.
    CLEAR_MASK(pPROCHOT, b1ISCT_PROCHOTon);  // Clear CPU Prochot function.
  }
}

/*****************************************************************************/
// Procedure: ISCT_Timer                    TimeDiv: 1 Min
// Description: Intel Smart Connect Technology timer.
// GPIO:
// Referrals:
/*****************************************************************************/
void ISCT_TimerCnt(void)
{
  if ( (IS_MASK_SET(AOAC_STATUS, ISCT_Enable)) && (SystemIsS3) )  // Check ISCT enable?
  {
    if ( ISCT_Timer == 0 ) { return; }

    if ( ISCT_Timer == 1 )
    {
      ISCT_OPEN_APS();
      SET_MASK( APSHDDFLAG , APSHDDPRT_MIN);
    }
    ISCT_Timer--;
  }
}

//=============================================================================
void ISCT_MinutetoSec(void)  // TimeBase: 1Min.
{
  if ( (IS_MASK_SET(APSHDDFLAG , APSHDDPRT_MIN)) && (SystemIsS3)) // when ISCT enable, check count time on last 1 min.
  {
    if (APSHDDCount == ISCT_Again)  // Check ISCT Second.
    {
      SET_MASK( APSHDDFLAG , APSHDDPRT_SEC);
      CLEAR_MASK( APSHDDFLAG , APSHDDPRT_MIN);
      APSHDDCount = 0;
    }
    else
    { APSHDDCount++; }
  }
}
//=============================================================================
//=============================================================================
void ISCT_SECtoChk(void)  // TimeBase: 1Sec.
{
  if ( (IS_MASK_SET(APSHDDFLAG , APSHDDPRT_SEC)) && (SystemIsS3)) //APS power on
  {
    if (APSHDDCount == 1)  //Ready APS sample mode
    {
      if(IS_MASK_CLEAR(APSHDDFLAG , b6PMEWAKE)){
        SET_MASK( APSHDDFLAG , APSHDDPRT_CHK);
        CLEAR_MASK( APSHDDFLAG , b6PMEWAKE);
        SET_MASK( APSHDDFLAG , b7RTCWAKE);
      }
      CLEAR_MASK( APSHDDFLAG , APSHDDPRT_SEC);
      APSHDDCount = 0;
    }
    else
    { APSHDDCount++; }
  }
}
#endif  // SW_ISCT

//=============================================================================

void ISCT_OPEN_APS(void)
{
#if SW_ISCT && Support_LenovoAPS
#if Support_EC_GS_ON    
          EC_GS_ON();
#endif //Support_EC_GS_ON    
          ECQEvent(GSENSOR_PWRON_7D);
          SET_MASK(GS.Status,GS_Enable);
          CLEAR_MASK(GS.Status,GS_Request);

          // Initial after get enable request
          GS.Buf1X_S = 0;
          GS.Buf1X_E = 0;
          GS.Buf1X_N = 0;
          GS.Buf1Y_S = 0;
          GS.Buf1Y_E = 0;
          GS.Buf1Y_N = 0;

          GS.Buf2X_S = 0;
          GS.Buf2X_E = 0;
          GS.Buf2X_N = 0;
          GS.Buf2Y_S = 0;
          GS.Buf2Y_E = 0;
          GS.Buf2Y_N = 0;

          uAPSLastX=0;
          uAPSLastY=0;

          uCountX=0;
          uTotalX=0;
          uCountY=0;
          uTotalY=0;

          uAPSThresholdX=0x04;
          uAPSThresholdY=0x04;
          uAPSTimeOut=40;
          SET_MASK( APSHDDFLAG , APSHDDPRT_MIN);
#endif  // SW_ISCT          
}


