/*-----------------------------------------------------------------------------
 * TITLE: OEM_LCD.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//-----------------------------------------------------------------------------
// Initialize Brightness Frequency.
//-----------------------------------------------------------------------------
void Init_LCD_Freq(void)
{
#if EC_Brightness_Ctrl
  WORD BLCDFreq;

  if ( (SYS_STATUS & 0x07) == 0x03 )  // Win7
  { BLCDFreq = (WORD)(Bri_Table[cPanelId][BRIGHT_MAX_STEP + 2] << 8) | Bri_Table[cPanelId][BRIGHT_MAX_STEP + 1]; }
  else
  { BLCDFreq = (WORD)(WIN8_Bri_Tab[cPanelId][WIN8_BRI_MAX_STEP + 2] << 8) | WIN8_Bri_Tab[cPanelId][WIN8_BRI_MAX_STEP + 1]; }

  C7CPRS = (24 * 1000) / BLCDFreq;
  if ( BLCDFreq > 5000 )
  { C7CPRS++; }
#endif  // EC_Brightness_Ctrl
}

/* ========================================================================
 * Brightness table define here
 * ======================================================================== */
#if EC_Brightness_Ctrl
BYTE code Bri_Table[BriTable_MAX][BRIGHT_MAX_STEP + 4] =
{
// Dark <------------------------------------------------> Light,  DAC_BRIG, Freq(2 byte), BL_Stable_time
  1,  2,  2,  3,  5,  7,  10,  14,  19,  24,  31,  39,  46,  56,  67,  100,  00,  0xF4, 0x01,      0x03,  //0x00 Default 500Hz
  1,  2,  2,  3,  5,  7,  10,  14,  19,  24,  31,  39,  46,  56,  67,  100,  00,   0x10, 0x27,      0x03,  //0x01 AUO B156XTN02 10KHz
  2,  2,  3,  4,  6,  9,  13,  18,  23,  30,  38,  45,  55,  65,  78,  100,  00,   0x58, 0x02,      0x03,  //0x02 LGD LP156WH4-TLN1 600Hz
  1,  2,  3,  5,  7,  11,  15,  21,  27,  34,  42,  52,  63,  73,  86,  100,  00,   0x58, 0x02,      0x03,  //0x03 LGD LP156WF1-TLB2(FHD_Anti) 600Hz
  1,  2,  3,  4,  6,  8,  12,  16,  22,  28,  35,  43,  52,  61,  73,  100,  00,   0x58, 0x02,      0x03,  //0x04 SEC LTN156AT24-L01 600Hz
  2,  2,  3,  4,  6,  10,  14,  19,  25,  31,  39,  48,  57,  68,  79,  100,  00,   0xE8, 0x03,      0x03,  //0x05 CMI N156BGE-L21  1KHz
  2,  2,  3,  4,  6,  10,  14,  19,  25,  32,  41,  50,  61,  74,  88,  100,  00,   0x58, 0x02,      0x03,  //0x06 AUO B140XTN03 600Hz
  2,  2,  3,  4,  7,  10,  14,  19,  25,  33,  41,  50,  61,  73,  86,  100,  00,   0x58, 0x02,      0x03,  //0x07 LGD LP140WH2-TLE2 600Hz
  2,  3,  3,  5,  7,  11,  16,  21,  28,  36,  44,  54,  65,  76,  89,  100,  00,   0x58, 0x02,      0x03,  //0x08 SEC LTN140AT20-L01 600Hz
  2,  3,  3,  5,  7,  11,  15,  21,  27,  35,  43,  53,  63,  75,  87,  100,  00,   0x2C, 0x01,      0x03,  //0x09 CMI N140BGE-L42  300Hz
  5,  6,  7,  8,  9,  10,  14,  19,  25,  31,  39,  48,  57,  68,  79,  100,  00,   0x58, 0x02,      0x03,  //0x0A LGD LP156WF1-TLC1(FHD_Glare) 600Hz
  1,  2,  2,  3,  5,  8,  12,  17,  22,  28,  36,  44,  53,  63,  75,  100,  00,   0xE8, 0x03,      0x03,  //0x0B BOE HB156WX1-100 1KHz
};

BYTE code WIN8_Bri_Tab[BriTable_MAX][WIN8_BRI_MAX_STEP + 4] =
{
// Dark <-----------------------------------------------------------------------> Light, DAC_BRIG, Freq(2 byte), BL_Stable_time
  1,  2,  2,  3,  3,  5,  6,  7,  8,  10,  12,  14,  16,  19,  22,  26,  33,  42,  53,  66,  100,  00,    0x10, 0x27,    0x03,  //0x00 Default 500Hz
  1,  2,  2,  3,  3,  5,  6,  7,  8,  10,  12,  14,  16,  19,  22,  26,  33,  42,  53,  66,  100,  00,   0x10, 0x27,    0x03,  //0x01 AUO B156XTN02 10KHz
  2,  3,  3,  4,  5,  6,  7,  9,  11,  13,  15,  18,  21,  24,  29,  33,  40,  51,  63,  77,  100,   00,   0x58, 0x02,    0x03,  //0x02 LGD LP156WH4-TLN1 600Hz
  3,  3,  4,  4,  5,  5,  7,  8,  9,  10,  12,  13,  16,  20,  23,  27,  33,  43,  55,  68,  100,     00,   0x58, 0x02,    0x03,  //0x03 LGD LP156WF1-TLB2(FHD_Anti) 600Hz
  1,  2,  2,  3,  4,  4,  6,  7,  9,  11,  13,  14,  17,  20,  24,  28,  33,  42,  52,  63,  100,     00,   0x58, 0x02,    0x03,  //0x04 SEC LTN156AT24-L01 600Hz
  2,  2,  3,  4,  5,  6,  7,  9,  11,  14,  16,  18,  21,  26,  30,  34,  41,  52,  64,  77,  100,     00,   0xE8, 0x03,    0x03,  //0x05 CMI N156BGE-L21  1KHz
  2,  2,  3,  3,  4,  6,  7,  9,  11,  13,  16,  18,  21,  25,  29,  34,  41,  54,  67,  83,  100,  00,   0x58, 0x02,    0x03,  //0x06 AUO B140XTN03 600Hz
  2,  3,  3,  4,  4,  6,  7,  9,  11,  13,  16,  18,  21,  26,  29,  34,  41,  53,  65,  79,  100,    00,   0x58, 0x02,    0x03,  //0x07 LGD LP140WH2-TLE2 600Hz
  2,  2,  3,  4,  4,  6,  7,  9,  11,  13,  15,  17,  20,  24,  27,  32,  38,  48,  60,  72,  100,    00,   0x58, 0x02,    0x03,  //0x08 SEC LTN140AT20-L01 600Hz
  2,  3,  3,  4,  5,  7,  8,  10,  13,  15,  18,  20,  24,  28,  33,  38,  45,  58,  71,  85,  100,    00,   0x2C, 0x01,    0x03,  //0x09 CMI N140BGE-L42  300Hz
  2,  2,  3,  3,  4,  5,  6,  7,  9,  11,  12,  14,  16,  19,  23,  26,  31,  41,  49,  60,  100,  00,   0x58, 0x02,    0x03,  //0x0A LGD LP156WF1-TLC1(FHD_Glare) 600Hz
  2,  2,  3,  3,  4,  6,  7,  9,  11,  13,  15,  17,  20,  24,  28,  33,  39,  50,  61,  74,  100,  00,   0xE8, 0x03,    0x03,  //0x0B BOE HB156WX1-100 1KHz
};
#endif  // EC_Brightness_Ctrl

void Load_PWMTimeOut_Timer(void)
{
  #if EC_Brightness_Ctrl
    TR1 = 0;                    // Disable timer1
    ET1 = 0;                    // Disable timer1 interrupt
    TL1 = Timer_5ms;          // Reload timer
    TH1 = Timer_5ms>>8;     // 5ms
  TF1 = 0;          // Clear overflow flag
  TR1 = 1;          // Enable timer1
  #endif  // EC_Brightness_Ctrl
}

void Set_PWM_Value(BYTE PWM_Value)
{
  #if EC_Brightness_Ctrl
  if ((DCR7 != 0) && (DCR7 != CTR3))
  {
    Load_PWMTimeOut_Timer();
    while(!TF1)
    {
      if ( Read_EC_INVT_PWM() == HighActive)
      {
        break;
      }
    }
    Load_PWMTimeOut_Timer();
    while(!TF1)
    {
      if (Read_EC_INVT_PWM()== LowActive)
      {
        DCR7 = PWM_Value;
        return;
      }
    }
  }
  else
  { DCR7 = PWM_Value; }
  #endif  // EC_Brightness_Ctrl
}

/*****************************************************************************/
// Procedure: Brightness_Control                TimeDiv: 10mSec
// Description:
// GPIO:
// Referrals:
/*****************************************************************************/
void Brightness_Control(void)
{
  #if EC_Brightness_Ctrl
  BYTE BBri_Level;
  BYTE DCR7_Temp;

  if ( (SystemNotS0) || (!Read_ENBKL_IN()) )  // Check S0 state and LCD backlight.
  {
    if(!Read_BKOFF())
    {
      DCR7 = 0;
    }
    return;
  }

  if ( (SYS_STATUS & 0x07) == 0x03 )  // Win7
  {
    if ( nBrightValue >= BRIGHT_MAX_STEP )    // Over 16 Step duty.
    { nBrightValue = (BRIGHT_MAX_STEP - 1); }
    BBri_Level = Cal_PWM_Duty(Bri_Table[cPanelId][nBrightValue],CTR3);
  }
  else
  {
    if ( nBrightValue >= WIN8_BRI_MAX_STEP )  // Over 21 Step duty.
    { nBrightValue = (WIN8_BRI_MAX_STEP - 1); }
    BBri_Level = Cal_PWM_Duty(WIN8_Bri_Tab[cPanelId][nBrightValue],CTR3);
  }

  if ( DCR7 == BBri_Level ) { return; }

  DCR7_Temp = DCR7;
    if( DCR7 > BBri_Level )
  {
//    DCR7--;
    DCR7_Temp--;
    Set_PWM_Value(DCR7_Temp);
  }  // Over level.
  else
  {
//    DCR7++;
    DCR7_Temp++;
    Set_PWM_Value(DCR7_Temp);
  }  // Less level.
  #endif  // EC_Brightness_Ctrl
}

//=============================================================================
//   Calculate PWM duty.
//=============================================================================
BYTE Cal_PWM_Duty(BYTE BCalDuty, BYTE BPWMBase)
{
  return ( BCalDuty = (WORD)((BCalDuty * BPWMBase) / 100));
}

/*****************************************************************************/
// Procedure: Backlight_Control                  TimeDiv: 10mSec
// Description:
// GPIO: GPIOG2, GPIOF7
// Referrals:
/*****************************************************************************/
void Backlight_Control(void)
{
  if ( SystemIsS0
#if SW_ISCT
    && (IS_MASK_CLEAR(uISCT, b5ISCT_BKOFF))
#endif // SW_ISCT
    )  // Check S0 and ISCT BKOFF status.
  {
    //if ( (Read_ENBKL_IN()) && (IS_MASK_CLEAR(cCmd, b3BkOff)) && Read_LID_SW_IN())
    if ( (Read_ENBKL_IN()) && (IS_MASK_CLEAR(cCmd,b3BkOff)) &&
      ( Read_LID_SW_IN() || (!PalBKLidSyncGET) ) )
    //if ( (IS_MASK_CLEAR(cCmd, b3BkOff)) && Read_LID_SW_IN())
    {
#if EC_Brightness_Ctrl
      if (DCR7 == 0)
      {  // force Back light enable after PWM stable.
        if ( (SYS_STATUS & 0x07) == 0x03 )  // Win7
        { BackLight_En_Delay = Bri_Table[cPanelId][BRIGHT_MAX_STEP+3]; }
        else  // Win8
        { BackLight_En_Delay = WIN8_Bri_Tab[cPanelId][WIN8_BRI_MAX_STEP+3]; }
      }
#endif  // EC_Brightness_Ctrl
#if SW_ISCT
      CLEAR_MASK(AOAC_STATUS, ISCT_S0_ISCT);
#endif // SW_ISCT
      APSHDDFLAG = 0;

      // Enable Backlight after PWM stable
      if (BackLight_En_Delay != 0)
      {
        BackLight_En_Delay--;
      }
      else
      {
        BKOFF_ON();
        CLEAR_MASK(pProject0,b0DispToggleEn);
        return;
      }
    }
  }
  BKOFF_OFF();
}

/*****************************************************************************/
// Procedure: Lid_Process                  TimeDiv: 500mSec
// Description:
// GPIO: GPIOH0, GPIOH3
// Referrals:
/*****************************************************************************/
void Lid_Process(void)
{

  if ((SystemIsS5)||(DSxPowState == SYSTEM_DSxOK) )
  {
//    EC_WAKE_OUT_LOW();
    CLEAR_MASK(SysStatus,LidStatus);
    LID_DEBOUNCE_CNT =0;
    if (Read_LID_SW_IN()) {
      LidStsEn;
    }
  }
  else
  {
#if Support_IFFS
    if ( (IS_MASK_SET(ACPI_HOTKEY,b5BIOS_IFFS)) )
    { return; }
#endif // Support_IFFS
    if( Read_LID_SW_IN() )
    {  //LCD Cover Open (LID OPEN)
      LidStsEn;   //(JSN-2012_1005)

      if(IS_MASK_CLEAR(SysStatus,LidStatus))
      {
        LID_DEBOUNCE_CNT++;
        if( LID_DEBOUNCE_CNT > _LID_DEBOUNCE )  // Debounce 2 timer.
        {
#if Support_LenovoAPS
          GS_D10 |= 0x20;
#endif  // Support_LenovoAPS
          if( SystemIsS0 )
          {
            if( PalBKLidSyncGET )
            {
              ECQEvent(EV_LID_Open);
            }
          }
          // -
//          LidWakeEn;

          if (((DSxPowState == SYSTEM_DSxOK)||((DSxPowState == SYSTEM_S3S4)&&(SystemIsS3)))&&LidOpenGET)
          {
            LidWakeEn;
            EC_WAKE_OUT_LOW();
            CLEAR_MASK(SysStatus,LidStatus);
            PWRBTN_pressCnt = 6;    // delay 60ms.
          }
          else
          {
            SET_MASK(SysStatus,LidStatus);

            LID_DEBOUNCE_CNT = 0;
          }
          CLEAR_MASK(SysStatus,CloseLid);
		  if( IS_MASK_SET(SysStatus2,LidCloseKBMSDis) )
    	  {
     		CLEAR_MASK(SysStatus2,LidCloseKBMSDis);
   			Clear_Key();
   			Enable_Any_Key_Irq();
    	  }
        }
      }
      else
      { LID_DEBOUNCE_CNT =0; }
    }
    else
    {  //LCD Cover Close (LID Close)

      LidStsDis;  //(JSN-2012_1005)
     
      if(IS_MASK_SET(SysStatus,LidStatus))
      {
        LID_DEBOUNCE_CNT++;
        if( LID_DEBOUNCE_CNT > _LID_DEBOUNCE )
        {		

	      SET_MASK(SysStatus2,LidCloseKBMSDis);
          CLEAR_MASK(SysStatus,LidStatus);
#if Support_LenovoAPS
          GS_D10 |= 0x20;
#endif  // Support_LenovoAPS
          LID_DEBOUNCE_CNT = 0;
          if(SystemNotS3)
          {
            if(SystemIsS0)
            {
              SET_MASK(SysStatus,CloseLid);
              if( PalBKLidSyncGET )
              {
                ECQEvent(EV_LID_Close);
              }
            }  // For Deep S3 wake up.
          }
        }
      }
      else
      { LID_DEBOUNCE_CNT = 0; }
    }
  }
}

#if NV_GC6
/*****************************************************************************/
// Procedure: GC6_FBClamp                  TimeDiv: 1 mSec
// Description:
// GPIO:
// Referrals:
/*****************************************************************************/
void GC6Process(void)
{
  if( (IS_MASK_SET(SYS_MISC1, ACPI_OS)) && (SystemIsS0) )
  {
    if( IS_MASK_SET(ACPI_STAT, GC6ENTRY)
      && Read_GC6Event_ON()
      && IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeGC6EventHI)
      )
    {

      SET_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
      if( Read_FB_CLAMP() )
      {
        nGC6ClampOff;
        FB_CLAMP_OFF();
      }
      else
      {
        nGC6ClampOn;
        FB_CLAMP_ON();
      }

    }
  }
}
#endif // NV_GC6
