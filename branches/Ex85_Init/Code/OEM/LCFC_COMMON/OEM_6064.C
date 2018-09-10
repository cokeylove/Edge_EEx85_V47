/*-----------------------------------------------------------------------------
 * TITLE: OEM_6064.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

void Cmd_B4(void)
{
  //Mos: Stop charger when EC and Bios flash
  SET_MASK(nStopChgStat3H, ECFLASH);
  FAN_PWM_OUT;    // Set FAN_PWM OUTPUT.
  EC_FAN_PWM_HI();  // Set FAN Full On.


  CHGIC_ptr = 2;    //reminder: call to write to SmartChg
  WriteSmartChgIC();
//[Project][CC20130423][Start] Add EC idle function for 68/6C port.
  if (PM2DI == 0xB4){
    RamProgram(01);}
  else{
    RamProgram(00);}
//[Project][CC20130423][End] Add EC idle function for 68/6C port.
}

void GetData2(BYTE nPort)
{
  TR1 = 0;          // Disable timer1
  ET1 = 0;          // Disable timer1 interrupt
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  TH1 = Timer_20ms>>8;    // Set timer1 counter 20ms
  TL1 = Timer_20ms;      // Set timer1 counter 20ms
  TF1 = 0;          // Clear overflow flag
  TR1 = 1;          // Enable timer1

  while (!TF1)
  {
    if (nPort == 0x60)
    {
      if(IS_MASK_SET(KBHISR,IBF))
      {
        if(IS_MASK_CLEAR(KBHISR,C_D))  // Data Port
        {
          CmdData2 = KBHIDIR;
        }
        TR1 = 0;      // Disable timer 1
        TF1 = 0;      // clear overflow flag
        ET1 = 1;      // Enable timer1 interrupt
        return;
      }
    }
    else if (nPort == 0x62)
    {
      if(IS_MASK_SET(PM1STS,IBF))
      {
        if(IS_MASK_CLEAR(PM1STS,C_D))  // Data Port
        {
          CmdData2 = PM1DI;
        }
        TR1 = 0;      // Disable timer 1
        TF1 = 0;      // clear overflow flag
        ET1 = 1;      // Enable timer1 interrupt
        return;
      }
    }
  }
  TR1 = 0;          // Disable timer 1
  TF1 = 0;          // clear overflow flag
  ET1 = 1;          // Enable timer1 interrupt
}


//----------------------------------------------------------------------------
// The function of 60 port
//  Note :
//  KBHIStep != 0x00 is necessary.
//----------------------------------------------------------------------------
void Hook_60Port(BYTE KBHICmd)
{
    BYTE i, Temp8;
#if Support_RFID
	BYTE j;
#endif //Support_RFID		
	
	

  switch(KBHICmd)
  {
#if Support_RFID
	case 0x01:		
	  if( KBHIStep == 0x01 )
	  {
	  	i = Cmd_01(KBHIData1,KBHIData,&j);
		if( i == TRUE )
		Data2Port(0x60,j);
	  }
	  break;
	case 0x02:		
	  if( KBHIStep == 0x01 )
	  {
	  	i = Cmd_02(KBHIData2,KBHIData1,KBHIData);
		if( i == TRUE )
			Data2Port(0x60,0x00);
	  }
	  break;
	case 0x09:		
	  if( KBHIStep == 0x01 )
	  {
	  	i = Cmd_09(KBHIData,&j);
		if( i == TRUE )
			Data2Port(0x60,j);
	  }
	  break;
	case 0x0A:		
	  if( KBHIStep == 0x01 )
	  {
	  	Cmd_0A(KBHIData1,KBHIData);
	  }
	  break;
#endif //Support_RFID
    case 0x40:
      Cmd_40(KBHIData);
      break;
    case 0x41:
      if(KBHIData==0xA0) Cmd_41_A0(0x60);
      if(KBHIData==0xA1) Cmd_41_A1(0x60);
      if(KBHIData==0xA2) Cmd_41_A2(0x60);
      if(KBHIData==0xA3) Cmd_41_A3(0x60);
      break;
    case 0x42:
      Cmd_42(KBHIData);
      break;
    case 0x43:
      Cmd_43(0x60,KBHIData);
      break;
    case 0x45:
      Cmd_45(0x60,KBHIData);
      break;
#if Support_COMPAL_MFG_TOOL
    case 0x46:
      Cmd_46(0x60,KBHIData);
      break;
#endif//Support_COMPAL_MFG_TOOL      
    case 0x47:
      // msmart 20100208 ++
      Cmd_47(0x60,KBHIData);
      break;
      // msmart
      break;
    case 0x49:
      break;
    case 0x4B:
      if (KBHIStep == 0x04)
      {
        i = KBHIData;
        break;
      }
      if (KBHIStep == 0x03)
      {
        //eEEPROMAddrsss = KBHIData;
        pTempAddress = KBHIData;
        //if ( eEEPROMAddrsss == 0xE0 )  // Check MBID address high byte.
        if ( pTempAddress == 0xE0 )  // Check MBID address high byte.
        { SET_MASK(MBID_Reload, b0MBID_High); }
        break;
      }
      if (KBHIStep == 0x02)
      {
        //eEEPROMBank = KBHIData;
        pTempBank = KBHIData;
        //if ( eEEPROMBank == 0x07 )  // Check MBID address low byte.
        if ( pTempBank == 0x07 )  // Check MBID address low byte.
        { SET_MASK(MBID_Reload, b1MBID_LOW); }
        break;
      }
      if (KBHIStep == 0x01)
      {
        Temp8  = KBHIData;            
        WRITE_DATA2EEPROM(&Temp8, 1, (EEPROM_Bank0 | pTempBank),pTempAddress);
        Data2Port(0x60, 0x00);
      }

      if ( (MBID_Reload & 0x03) == 0x03 )
      {
        LoadKBMatrixToRAM();  // Reload KB matrix.
        MBID_Reload = 0;
      }
      else
      { MBID_Reload = 0; }
      break;

    case 0x4C:
      if (KBHIStep == 0x03)
      {
        i = KBHIData;
        break;
      }
      if (KBHIStep == 0x02)
      {
        //eEEPROMAddrsss = KBHIData;
        pTempAddress = KBHIData;
        break;
      }
      if (KBHIStep == 0x01)
      {
        eEEPROMAddrsss = pTempAddress;
        eEEPROMBank = KBHIData  | EEPROM_Bank0; 
        Cmd_4E(0x60,eEEPROMAddrsss);
      }
      break;

    case 0x4D:
      if (KBHIStep == 0x02)
      {
        //eEEPROMAddrsss = KBHIData;
        pTempAddress = KBHIData;
        break;
      }
      if (KBHIStep == 0x01)
      {
        eEEPROMAddrsss = pTempAddress;  
        eEEPROMData  = KBHIData;
        Update_EEPROMMark();
      }
      break;
    case 0x4E:
      eEEPROMAddrsss = KBHIData;
      Cmd_4E(0x60,eEEPROMAddrsss);

      break;
    case 0x52:
      if(KBHIData==0xA0) Cmd_52_A0(0x60);
      if(KBHIData==0xA3) Cmd_52_A3(0x60);
      if(KBHIData==0xA4) Cmd_52_A4(0x60);
      if(KBHIData==0xA5) Cmd_52_A5(0x60);
      break;
    case 0x53:
      if (KBHIStep == 0x01)
      {
        Cmd_53(0x60,KBHIData2,KBHIData1,KBHIData);
      }
      break;
    case 0x56:
    break;
    case 0x58:
      Cmd_58(KBHIData);
      break;
    case 0x59:
      Cmd_59(0x60,KBHIData,0);
      break;
#if EC_Brightness_Ctrl      
    case 0x5C:
      Cmd_5C(0x60);
      break;
    case 0x5D:
      Cmd_5D(KBHIData);
      break;
#endif      
    case 0x97:
      Cmd_97(KBHIData);
      break;
    case 0xB0:
      Cmd_B0(0x60,KBHIData);
      break;
    case 0xB2:
      if( KBHIStep == 0x01 )
      {
        Cmd_B2(KBHIData2,KBHIData1,KBHIData);
      }
      break;
    case 0xB3:
      if (KBHIStep == 0x01)
      {
        Cmd_B3(0x60,KBHIData1,KBHIData);
      }
      break;
    case 0xB7:
      Cmd_B7(0x60,KBHIData);
      break;
  }
}

//----------------------------------------------------------------------------
// The function of 64 port
//----------------------------------------------------------------------------
void Hook_64Port(BYTE KBHICmd)
{
  switch(KBHICmd)
  {
#if Support_RFID
  	case 0x01:		
  	  KBHIStep=0x02;
	  break;
	case 0x02:		
	  KBHIStep=0x03;
	  break;
	case 0x09:		
	  KBHIStep=0x01;
	  break;
	case 0x0A:
	  KBHIStep=0x02;
	  break;
#endif //Support_RFID
    case 0x40:
      KBHIStep=0x01;
      break;
    case 0x41:
      KBHIStep=0x01;
      break;
    case 0x42:
      KBHIStep=0x01;
      break;
    case 0x43:
      KBHIStep=0x01;
      break;
    case 0x44:
      Cmd_44(0x60);
      break;
    case 0x45:
      KBHIStep=0x01;
      break;
#if Support_COMPAL_MFG_TOOL      
    case 0x46:
      KBHIStep=0x01;
      break;
#endif //Support_COMPAL_MFG_TOOL      
    case 0x47:
      KBHIStep=0x01;
      break;
    case 0x49:
      break;
    case 0x4B:
      KBHIStep=0x04;
      break;
    case 0x4C:
      KBHIStep=0x03;
      break;
    case 0x4D:
      KBHIStep=0x02;
      break;
    case 0x4E:
      KBHIStep=0x01;
      break;
    case 0x4F:
      Erase_EEPROMAll();
      break;
    case 0x51:
      Cmd_51(0x60);
      break;
    case 0x52:
      KBHIStep=0x01;
      break;
    case 0x53:
#if Support_RFID
      KBHIStep=0x03;	
#else
      KBHIStep=0x02;	 
#endif //Support_RFID	   
      break;
    case 0x56:
      Cmd_56(0x60);
      break;
    case 0x58:
      KBHIStep=0x01;
      break;
    case 0x59:
      KBHIStep=0x01;
      break;
    case 0x5C:
      break;
    case 0x5D:
      KBHIStep=0x01;
      break;
    case 0x97:
      KBHIStep=0x01;
      break;
    case 0xB0:
      KBHIStep=0x01;
      break;
    case 0xB2:
      KBHIStep=0x03;
      break;
    case 0xB3:
      KBHIStep=0x02;
      break;
    case 0xB4:
      Cmd_B4();
      break;
    case 0xB7:
      KBHIStep=0x01;
      break;
    case 0x77:
//      CacheDma(1,FSCEHighLevel);
            DisableAllInterrupt();
      Erase_Eflash_1K(0x01,0x20,0x00);
            EnableAllInterrupt();
      break;
    case 0x88:
//      CacheDma(3,FSCEHighLevel);
//      Copy_Eflash_1K(0x00,0x00,0x00, 0x01,0x20,0x00);
      break;

    default:
      break;
  }
}

//-----------------------------------------------------------------------
// kbcmd : keyboard command from 0x60 port
//-----------------------------------------------------------------------
void Hook_Keyboard_Cmd(BYTE kbcmd)
{

}

//-----------------------------------------------------------------------
// kbcmdp : keyboard command parameter from 0x60 port
// for example keyboard command (0xED, 0xF0, 0xF3)
//-----------------------------------------------------------------------
void Hook_Keyboard_Cmd_Parameter(BYTE kbcmdp)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0xD4 command
//-----------------------------------------------------------------------
void Hook_Mouse_D4Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x90 command
//-----------------------------------------------------------------------
void Hook_Mouse_90Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x91 command
//-----------------------------------------------------------------------
void Hook_Mouse_91Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x92 command
//-----------------------------------------------------------------------
void Hook_Mouse_92Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x93 command
//-----------------------------------------------------------------------
void Hook_Mouse_93Cmd(BYTE mscmd)
{

}

void Cmd_40(BYTE sCount)
{
  AutoTimer = sCount;
  if ( AutoTimer != 0 )
  { SET_MASK(ACPI_HOTKEY, b6Cmd_NoShut); }
  else
  { CLEAR_MASK(ACPI_HOTKEY, b6Cmd_NoShut); }
}

void Cmd_41_A0(BYTE nPort) {
  Data2Port(nPort, REV0_BYTE0);
}

void Cmd_41_A1(BYTE nPort) {     //return platform id
  int i;
  Data2Port(nPort, OEM_PLATFORMID[0]);
  for(i=1 ;i<(sizeof(OEM_PLATFORMID)-1);i++)  {
    MultiB2Port(nPort, OEM_PLATFORMID[i]);
  }   
}

void Cmd_41_A2(BYTE nPort) {  //	Reserved
  Data2Port(nPort, 0);
}

void Cmd_41_A3(BYTE nPort) {  //	Reserved
  Data2Port(nPort, 0);
}

void Cmd_42(BYTE eFlashBank)  // Bank assign for EEPROM
{
  eEEPROMBank = eFlashBank | EEPROM_Bank0;  
}

void Cmd_43(BYTE nPort,BYTE nData)
{
  Data2Port(nPort,( *((XBYTE *)(OEMRAM6|nData)) ));
}

void Cmd_44(BYTE nPort) {
  Data2Port(nPort, ProjectID0L);
  MultiB2Port(nPort, ProjectID0H);
}

void Cmd_45(BYTE nPort, BYTE sCount)
{
  switch( sCount )
  {
#if chPWSW1WDT
    case 0x20:	//Enable power switch WDT function
     	OEM_PWRSWWDTEn();
			CLEAR_MASK(pProject4,pPWSWdisable);
	 		break;	  
    case 0x21:	//Disable power switch WDT function
			SET_MASK(pProject4,pPWSWdisable);	 
      break;
#endif  //chPWSW1WDT	  	

#if Support_TYPE_C
    case 0x22:	//Enable Type-C FW update
	  SET_MASK(PDStatus,TypeCFwUpdating);
	  break;	  
    case 0x23:	//Disable Type-C FW update
	  CLEAR_MASK(PDStatus,TypeCFwUpdating);
      break;
#endif  //Support_TYPE_C	  

    case 0x7C:  // Enable BIOS WDT.
    	BIOS_WDT_Cnt = 14;	//Set 10 sec.
      break;
    case 0x7D:  // Disable BIOS WDT.
    	BIOS_WDT_Cnt = 0;
      break;
	  case 0x7E:  // Enable BIOS smi.
    	WorkaroundforBIOSSMI = 1;	//Set 10 sec.
      break;
    case 0x7F:  // Disable BIOS smi
    	WorkaroundforBIOSSMI = 0;	
      break;
    case 0x80:  // Disable ME Flash.
  #if (PLATFORM_CHIP == INTEL)
      pchME_FLAS_OUT;
      pchME_FLAS_HI();
	  SET_MASK(pProject3,pIsMeFlashing);
  #endif // PLATFORM_CHIP
      break;
    case 0x81:  // Enable ME Flash.
  #if (PLATFORM_CHIP == INTEL)
      pchME_FLAS_LOW();
      pchME_FLAS_INDW;
	  CLEAR_MASK(pProject3,pIsMeFlashing);
	  PCHErrCnt = 0;          // Clear error count.
  #endif // PLATFORM_CHIP
      break;

    case 0x83:
      SET_MASK(SysStatus,LidKBIgnore);
      Ccb42_DISAB_KEY = 1;
      Flag.SCAN_INH = 1;        // Inhibit scanner (internal keyboard).
      Lock_Scan();          // Lock scanner
      ECSend2Port(2, 0xF5);      // Disable TouchPad.
      break;

#if Support_RFID	  
    case 0x93:
      unlockRFIDProtect();
      break;
#endif //Support_RFID

    case 0xA0:	//BIOS notify CP reset finish
	    #if SupportReadTPID
	    ReadTPid();
	    #endif
      break;
 
    case 0xAB:                // Return MBID
      Data2Port(nPort, uMBID);
      break;

    case 0xAC:                // Return Board ID
      Data2Port(nPort, (BoardID & ~BoardID_ready));
      break;

    case 0xF6:      // Post Hotkey
      //FnFuncCLR;    // Disable Hotkey
      SET_MASK(pProject0,b6PostHotKey);
      break;
    case 0xF7:      // Post Hotkey
      //FnFuncSET;    // Enable Hotkey
      CLEAR_MASK(pProject0,b6PostHotKey);
      break;
  }
}

#if Support_COMPAL_MFG_TOOL
void Cmd_46(BYTE nPort,BYTE nData)
{
  BYTE BRPM_Manual;
  switch( nData )
  {
    case 0x81:
      Data2Port(nPort, nAtmFanSpeed);    //Reading FAN speed from FAN1
      break;
    case 0x82:
      Data2Port(nPort, FAN2PRM);      //Reading FAN speed from FAN2
      break;
    case 0x84:
      FanAutomaticmodeEn;
      CLEAR_MASK(uReserve07,b7Fan2Manual);
#if (!FAN_TABLE_Already)
      FAN_PWM_OUT;      // Set FAN_PWM OUTPUT.
      EC_FAN_PWM_HI();
#endif  // FAN_TABLE_Already      
      break;
    case 0x89:
      SET_MASK(uReserve07,b7Fan2Manual);
      break;

    default:
      FanAutomaticmodeDis;
      if ( nData >= 160 )
      {
        nData -= 160;
        ManualFan2PRM = nData;
      }
      else
      {
        u4ManualFanPRM = nData;
        FAN_PWM_ALT;
        FAN_PWM = u4ManualFanPRM + 2;
        FAN_SPEED_ALT;
      }

      if (IS_MASK_CLEAR(uReserve07,b7Fan2Manual))
       { BRPM_Manual = u4ManualFanPRM; }    // Common FAN1 PRM value.
      else
       { BRPM_Manual = ManualFan2PRM; }  // Respective FAN PRM value.
      break;
  }
}
#endif 

void Cmd_47(BYTE nPort, BYTE nData)
{
//for AFT LED test
  switch( nData )
  {
    case 0x80:      // Return back the control right to EC
      LedPowerOn(); 
      break;
    case 0x82:      // Power management LED on and control by OS
      u4LEDcontrol = LED_TURN_ON | LED_Blinking | POWER_LED;
      TPG_LED_control();
      u4LEDcontrol = LED_TURN_ON | LED_Blinking | THINKPAD_LOGO_LED;
      TPG_LED_control();      
      break;
  }
}

void Cmd_4E(BYTE nPort, BYTE nOffSet)
{
  BYTE i,j,k;
  if ((eEEPROMBank == EEPROM_Bank7) && (nOffSet > 0xEF) && (nOffSet!=0xF1))  
  {
    nOffSet = nOffSet & 0x0F;
    for (i=0; i<4 ;i++)
    {
      for (j=0; j< 0x10 ;j++)
      {
        k = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_16Byte + i),(j*16));
        if(k == 0xFF)
        {
          break;
        }
        else
        {
          eEEPROMData = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_16Byte + i),((j*16) | nOffSet));
        }
      }
      if (k == 0xFF)
        break;
    }
  }
  else
  {
    eEEPROMData=Read_Eflash_Byte(EEPROMA2,(eEEPROMBank|EEPROM_Bank0),nOffSet);
  }
  Data2Port(nPort, eEEPROMData);
}


void Cmd_51(BYTE nPort) {
  Data2Port(nPort, REV0_BYTE0);
  MultiB2Port(nPort, REV0_BYTE1);
  MultiB2Port(nPort, REV0_BYTE3);
  Unlock_ShipMode();
}

void Cmd_52_A0(BYTE nPort) {
  int i;
  if (IS14Inch){    //14"
     Data2Port(nPort, OEM_PROJECT1[0]);
     for(i=1 ;i<(sizeof(OEM_PROJECT1)-1);i++)
     {
        MultiB2Port(nPort, OEM_PROJECT1[i]);
     }  
  }
  else if (IS15Inch){    //15"
     Data2Port(nPort, OEM_PROJECT2[0]);
     for(i=1 ;i<(sizeof(OEM_PROJECT2)-1);i++)
     {
        MultiB2Port(nPort, OEM_PROJECT2[i]);
     }  
  }
}

void Cmd_52_A3(BYTE nPort) {
  Data2Port(nPort, (0x10+(CHIP_TYPE0&0x0F)));
}

void Cmd_52_A4(BYTE nPort)
{
  if( IS_MASK_SET(SYS_MISC1,Crisis_On) )
  {
    Data2Port(nPort,0x80);
    CLEAR_MASK(SYS_MISC1,Crisis_On);
  }
  else
  {
    Data2Port(nPort,0x00);
  }
}

void Cmd_52_A5(BYTE nPort) {
  Data2Port(nPort, REV0_BYTE4);
}

void Cmd_53(BYTE nPort,BYTE nDataH,BYTE nDataM,BYTE nDataL)
{
  eEEPROMData=Read_Eflash_Byte(nDataH,nDataM,nDataL);
  Data2Port(nPort, eEEPROMData);
}

/* ========================================================================
 * Function Name : Command 56
 * Get SMI trigger source
 * Input : BYTE     data           cmd value
 * Output: BYTE     One byte
 * ======================================================================== */
void Cmd_56(BYTE nPort)
{
  CLEAR_MASK(PM1STS,BIT6);  // Clear SMI Flag
  Data2Port(nPort,pSMIEvent);
}


void Cmd_58(BYTE sCount)
{
  #if EC_Brightness_Ctrl
  if( sCount >= BriTable_MAX )
  { cPanelId = BriTable_MAX - 1; }  // Set max. level.
  else
  { cPanelId = sCount; }
  Init_LCD_Freq();
  #endif  // EC_Brightness_Ctrl
}

void Cmd_59(BYTE nPort, BYTE nData, BYTE nData2)
{
  switch( nData )
  {
    case 0x60:  // Keep PCH power for RTC
      SET_MASK(cCmd,cC_PCHPWR_Keep);
      break;

    case 0x64:  // Enable scancode for test button
      SET_MASK(cCmd,cC_TestBtnEn);
      break;

    case 0x65:  // Disable scancode for test button
      CLEAR_MASK(cCmd,cC_TestBtnEn);
      break;

    case 0x6E:
      uCritBattWakeThre = nData;
#if Support_IFFS
      SET_MASK(uCritBattWakeThre,IFFS_Enable);
      uIffsCnt = 40;
#endif // Support_IFFS

      break;

#if Support_COMPAL_MFG_TOOL
        case 0x76:
      FanAutomaticmodeEn;      // return EC control.
      CLEAR_MASK(uReserve07,b7Fan2Manual);
      FanMaximumspeedCLR;                  // Disable Full on.
      break;
        case 0x77:
      FanAutomaticmodeEn;      // return EC control.
      CLEAR_MASK(uReserve07,b7Fan2Manual);
      FanMaximumspeedSet;  // Enable Full on.
      break;
#endif //Support_COMPAL_MFG_TOOL

    case 0x7A:
      EnablePS2Port_2();
      break;
    case 0x7B:
      DisablePS2Port_2();
      break;

        case 0x7C: // Enable Lid Keyboard function
            CLEAR_MASK(SysStatus,LidKBIgnore);
            Ccb42_DISAB_KEY = 0;
      Flag.SCAN_INH = 0;        // Not Inhibit scanner (internal keyboard).
      Unlock_Scan();          // UnLock scanner
            break;

        case 0x7D: // Disable Lid Keyboard function
            SET_MASK(SysStatus,LidKBIgnore);
            Ccb42_DISAB_KEY = 1;
      Flag.SCAN_INH = 1;        // Inhibit scanner (internal keyboard).
      Lock_Scan();          // Lock scanner
            break;

    case 0x80:  // set battery charge to 100% then discharge to xx% and keep
      SET_MASK(cBattFlag0,cBF0_FullToTarget);
      CLEAR_MASK(cBattFlag0,cBF0_Full);
      CLEAR_MASK(cBattFlag0,cBF0_GoTarget);
      GetData2(nPort);
      cTargetGauge = CmdData2;
      if ( cTargetGauge > 100 )
        cTargetGauge = 0;
      break;

    case 0x81:  //charge/discharge to xx% and keep
      CLEAR_MASK(cBattFlag0,cBF0_FullToTarget);
      CLEAR_MASK(cBattFlag0,cBF0_Full);
      SET_MASK(cBattFlag0,cBF0_GoTarget);
      GetData2(nPort);
      cTargetGauge = CmdData2;
      if ( cTargetGauge > 100 )
        cTargetGauge = 0;
      break;
    case 0x82:  //disable battery charge xx% feature
      CLEAR_MASK(cBattFlag0,cBF0_FullToTarget);
      CLEAR_MASK(cBattFlag0,cBF0_Full);
      CLEAR_MASK(cBattFlag0,cBF0_GoTarget);
      break;

    case 0x94:
      b0MuteSET;
      EC_MUTE_ACT();          // Mute
      break;
    case 0x95:
      b0MuteCPL;
      EC_MUTE_INACT();        // Unmute
      break;

    case 0x9A:              // Disable AC power source
      SET_MASK(cBattFlag0,cCmdAcOff);
      SET_MASK(ACOFF_SOURCE, ACMD);
      CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
      ACOFF_HI();
      ECSMI_SCIEvent(ACPI_ACOUT_SCI);
      break;
    case 0x9B:              // Enable AC power source
      CLEAR_MASK(cBattFlag0,cCmdAcOff);
      CLEAR_MASK(ACOFF_SOURCE, ACMD);
      SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
      ACOFF_LOW();
      ECSMI_SCIEvent(ACPI_ACIN_SCI);
      break;
    case 0x9C:              // Enable lid switch resume function
    
      break;

    case 0x9D:              // Disable lid switch resume function
    
      break;

    case 0x9E:              //Disable Internal keyboard.
      Flag.SCAN_INH = 1;        // Inhibit scanner (internal keyboard).
      Lock_Scan();          // Lock scanner
      break;

    case 0xA2:              // System shutdown
    return;
      SET_MASK(SysStatus,ERR_ShuntDownFlag);
      ProcessSID(COMMAND_ID);     // Shutdown ID 0x04.

	#if (PLATFORM_CHIP == INTEL)
      PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState=SYSTEM_S0_S5;
	#else // PLATFORM_CHIP
      pAMDPWRBTNCnt = AMDPWRBTNWK;
	#endif // PLATFORM_CHIP

      break;

     case 0xC1:  // Force battery in learning state with AC
      SET_MASK(cBattFlag0,cBF0_BLMode);
      break;
    case 0xC2:  // Disable battery in learning state with AC
      CLEAR_MASK(cBattFlag0,cBF0_BLMode);
//      AcOffProcess(nAcOffCmdDIS);  // Disable AcOff pin
      break;

    case 0xD2:  // Beep alarm 200ms
      PWM_BEEP = 0x7F;      // Out beep
      break;

    case 0xD7:  // Battery stop charge enable
      SET_MASK(nStopChgStat3L,ENCUSTOMER);
      break;

    case 0xD8:  // Battery stop charge disable
      CLEAR_MASK(nStopChgStat3L,ENCUSTOMER);
      break;

    case 0xD9:   // Set System Flag
      SET_MASK(KBHISR,SYSF);
      break;

    case 0xDA:   // Clear system Flag
      CLEAR_MASK(KBHISR,SYSF);
      break;

    case 0xE1:              // Turn on LCD backlight
      CLEAR_MASK(cCmd,cC_BkOff);
      break;

    case 0xE2:               // Turn off LCD backlight
      SET_MASK(cCmd,cC_BkOff);
      break;

    case 0xE4:   // Selection BR matrix
      cKBStatus |= 0x03;  // Set BR keyboard.
      LoadKBMatrixToRAM();
      break;
    case 0xE5:   // Selection US matrix
      cKBStatus &= 0xFC;  // Set US keyboard.
      LoadKBMatrixToRAM();
      break;
    case 0xE6:   // Selection JP matrix
      cKBStatus &= 0xFC;
      cKBStatus |= 0x02;  // Set JP keyboard.
      LoadKBMatrixToRAM();
      break;
    case 0xE7:   // Selection UK matrix
      cKBStatus &= 0xFC;
      cKBStatus |= 0x01;  // Set UK keyboard.
      LoadKBMatrixToRAM();
      break;

    case 0xE8:               // enable ACPI mode for Embedded control
      SET_MASK(SYS_MISC1,ACPI_OS);
//       if (pFKTemp >= 30 && IS_MASK_SET(BattAuthStat, (OEMauthPass|SWauthPass) ) ) {
//         //
//         // A workaround for battery w/ special case. Ex. Panasonic has TDA protection, the BMU would not connectable while low voltage.
//         // Set to auth pass while BIOS POST and re-auth again once entered ACPI OS.
//         // TODO: What about non-ACPI OS mode?
//         //
//         pFKTemp = 0;  // Set pFKTemp to 0 to active LAuthProcess() in OEM_MAIN().
//         Service_Auth_Step = 1;
//       }

//      ECQEvent(GSENSOR_PWRON_7D);

      break;
    case 0xE9:               // disable ACPI mode for Embedded control
      CLEAR_MASK(SYS_MISC1,ACPI_OS);
      break;

    case 0xF6:
    case 0xF7:
    case 0xF8:
    case 0xF9:
      if (IS_MASK_SET(SYS_MISC1,ACPI_OS))
      {
        cSysActionFlag = SysAction_SysShutdown_EC_AutoBoot;
      }
      else
      {
        if( nData == 0xF6 )      // ACTION
          cSysActionFlag = SysAction_EC_Restart;
        else if( nData == 0xF7 )  // Fixed system action to 3 - Restart system
          cSysActionFlag = SysAction_EC_Restart;
        else if( nData == 0xF8 )  // Fixed system action to 2 - Shutdown system
          cSysActionFlag = SysAction_EC_Shutdown;
        else if( nData == 0xF9 )  // Fixed system action to 1 - Nothing
          cSysActionFlag = SysAction_Nothing;
      }

        if (cSysActionFlag==SysAction_EC_Shutdown || cSysActionFlag==SysAction_EC_Restart)
        {
          PWSeqStep = 1;
          PowSeqDelay = 200;
          SysPowState=SYSTEM_S0_S5;
        }
      break;
  }
}

#if EC_Brightness_Ctrl
void Cmd_5C(BYTE nPort)
{
  //Get brightness level
    Data2Port(nPort, nBrightValue);
}

void Cmd_5D(BYTE sCount)
{
  if ( sCount != nBrightValue )  // unequal value.
  {
    if ( (SYS_STATUS & 0x07) == 0x04 )  // Win8
    {
      if( sCount >= WIN8_BRI_MAX_STEP )
      { nBrightValue = WIN8_BRI_MAX_STEP - 1; }  // Set max. level.
      else
      { nBrightValue = sCount; }
    }
    else
    {
      if( sCount >= BRIGHT_MAX_STEP )
      { nBrightValue = BRIGHT_MAX_STEP - 1; }    // Set max. level.
      else
      { nBrightValue = sCount; }
    }
  }
}
#endif

void Cmd_97(BYTE nData)
{

}

#if Support_RFID
BYTE Cmd_01(BYTE nDataH,BYTE nDataL,XBYTE* ref)
{
	BYTE offset,nTemp,nBank;
	if(nDataL > 0x7F )
	{
		offset = 1;
	}
	else
	{
		offset = 0;
	}
	
	switch(nDataH)
	{
		case 0x00:
			nBank = 0+offset;
			break;
		case 0x01:
			nBank = 2+offset;
			break;
		case 0x02:
			nBank = 4+offset;
			break;
		case 0x03:
			nBank = 6+offset;
			break;
		default:
			return FALSE;		
	}
	
	if( !(rACCPAGE[nBank]&0x02) )
	{
		*ref = readRFIDEEprom(nBank,(nDataL-0x80*offset));
	
		
		return TRUE;
	}
	return FALSE;
}


BYTE Cmd_02(BYTE nDataH,BYTE nDataL,BYTE nData)
{
	BYTE offset,nTemp,nBank;
	if(nDataL > 0x7F )
	{
		offset = 1;
	}
	else
	{
		offset = 0;
	}
	switch(nDataH)
	{
		case 0x00:
			nBank = 0+offset;
			
			break;
		case 0x01:
			nBank = 2+offset;
			
			break;
		case 0x02:
			nBank = 4+offset;
			
			break;
		case 0x03:
			nBank = 6+offset;
			
			break;
		default:
			return FALSE;			
	}

	if( !(rACCPAGE[nBank]&0x01) )
	{
		writeRFIDEEprom(nBank,(nDataL-0x80*offset),nData);
		return TRUE;
	}

	return FALSE;
}


BYTE Cmd_09(BYTE nData,XBYTE* ref)
{
	
	if( nData < 8 )
	{
		*ref = rACCPAGE[nData];
		
		return TRUE;
	}
	return FALSE;
}


void Cmd_0A(nDataH,nDataL)
{
	BYTE accData = 0;
	
	if( nDataH < 8 )
	{	
		if( (rACCPAGE[nDataH] != nDataL) )
		{
			if( (rACCPAGE[nDataH] & 0x80) )
			{
				;
			}
			else
			{
				if( (nDataL&0x01) )
				{
					accData |= 0x01;	
				}
				if( (nDataL&0x02) )
				{
					accData |= 0x02;	
				}
				rACCPAGE[nDataH] = nDataL;
				
				writeRFIDProtect(nDataH,accData);
			}
		}
	}
}
#endif //Support_RFID

/* ========================================================================
 * Function Name : command B0
 * Write EC name space
 * Input : BYTE     00-ff          Address
 *         BYTE     00-ff          Data
 * Output: None
 * ======================================================================== */
void Cmd_B0(BYTE nPort,BYTE nData)
{
  Data2Port(nPort,( *((XBYTE *)(OEMRAM4|nData)) ));
}

/* ========================================================================
 * Function Name : command B1
 * Write EC name space
 * Input : BYTE     00-ff          Address
 *         BYTE     00-ff          Data
 * Output: None
 * ======================================================================== */
void Cmd_B1(BYTE nDataH, BYTE nDataL)
{

  *(unsigned char xdata *) (WORD)(OEMRAMA|nDataH) = nDataL;

  switch( nDataH)   //bit 3 - 0 LED ID
  {
    case 0x00:  
#if KB_FnSticky_Key   
      if (!FnStickyGET){ //disabe sticky status when disable sticky
        STICKY_MODE_CLR;
        STICKY_LOCK_CLR;
        Scanner_State &= ~Scanner_State_FN;
        temp_scanner_state.field.tFN = 0; 
      }
#endif //KB_FnSticky_Key
    break;
    case 0x03:   //u4Cfgspace3
    if (nDataL & BIT2) {
      if (pEdgeBATStopCharge == 100) { // default value
        uThresholdSetting = 0x00;
      } else {
        uThresholdSetting = pEdgeBATStopCharge;
      }
    } else {
      uThresholdSetting = pEdgeBATStartCharge;    
    }

    // For BIT3 Airplane mode.
    if (IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady)) {
      Update_ChargeIC();
      SetPowerBatteryparameter();
    }
    break;

    case 0x0C:   //u4LEDcontrol
      TPG_LED_control();
    break;

    case 0x0D:
#if Support_TP_RESET_control      
      if (RST_TRACK_POINT_GET)
        TP_RESET_ON();
      else 
        TP_RESET_OFF();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
      if (BYPASS_CLICKPAD_GET)
        BYPASS_PAD_ON();
      else
        BYPASS_PAD_OFF();  
      break;  
#endif //Support_BYPASS_PAD_control   

        Chk_pDevStus();

    case 0x22:   //u4InhChgTimerH
      if( (u4InhChgTimerL==0xFF) && (u4InhChgTimerH==0xFF) )
      {
        pInhitTimer=0xFFFF;
      }
      else
      {
        pInhitTimer = (u4InhChgTimerL + ( u4InhChgTimerH * 256 ))*30;  // 30sec
        if (pInhitTimer == 0) {
          PriBattinhibitDis;
          Inhibit_chargingCLR;
          CLEAR_MASK(nStopChgStat3L,ENCMDSTOPCHG);
        }
      }
    break;
    
    case 0x23:   //u4ExtCommand
      TPG_EXTBATCmd();
    break;
    case 0x24:   //uThresholdSetting
      EDGEPMBATTProcess();
    break;
    case 0x3D:   //u4PassCtrl
      EdgePasswordProcess();
    break;
    case 0x50:   //uSMBusPrtcl
      Hook_ACPICommand();
    break;
    case 0x81:   //u4BattInfoID
      IBMbatteryStore();
    break;
  }

}


void Cmd_B2(BYTE nDataH, BYTE nDataL, BYTE nData)
{
//  nDataH = nDataH&0x0F;
  *(unsigned char xdata *) (WORD)((nDataH << 8) + nDataL) = nData;
    if (nDataH == 0x02) {
      if (nDataL == 0xAF) {
        if (nData & BIT0) {
          //
          // BIOS set DIS sku at 0x40B BIT0, 0 = UMA, 1 = DIS.
          //
          if (IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady)) {
            Update_ChargeIC();
            SetPowerBatteryparameter();
          }
        }
      }
    }
//[Project][CC20130425][Start]Add first used date feature.
    if( nDataH == 0xED )
    {
      switch( nDataL )
      {
        case 0x50:
          uSMBusPrtcl = nData;
          if (uSMBusPrtcl == 0x88){
//            if(bWSMBusBlock(SMbusChB, SMbusWBK, uSMBusAddr, uSMBusCmd, &uSMBusData0, uSMBusBcnt,SMBus_NeedPEC))
            if ((uSMBusBcnt == 2) && (uSMBusAddr == SmBat_Addr)&&(uSMBusCmd == FirstUsedDate))
            {
              SET_MASK(uProject0,uFUdayClr);
              uSMBusPrtcl = 0;
              uSMBusSts = 0;
              uSMBusAddr = 0;
              uSMBusCmd = 0;
              uSMBusData0 = 0;
              uSMBusBcnt = 0;
            }
          }
        break;

        case 0x51:
          uSMBusSts = nData;
        break;

        case 0x52:
          uSMBusAddr = nData;
        break;

        case 0x53:
          uSMBusCmd = nData;
        break;

        case 0x54:
          uSMBusData0 = nData;
        break;

        case 0x55:
          uSMBusData1 = nData;
        break;

        case 0x74:
          uSMBusBcnt = nData;
        break;

        default:
        break;
      }
    }
//[Project][CC20130425][End]Add first used date feature.
}

void Cmd_B3(BYTE nPort,BYTE nDataH,BYTE nDataL)
{
  if( nDataH == 0xEC &&(nDataL == 0x82 || nDataL == 0x83)) //Get frist used date for factory tools
  {
    nDataH = 0x0E;
  }
  if( nDataH == 0xED &&(nDataL >= 0xF0 && nDataL <= 0xF7)) //get EC version for factory tools
  {
    nDataH = 0x0A;
  }
  cSPIEEPROM = 0x00;    //msmart test

  nDataH = nDataH &0x0F;
  ITempW01 = (WORD)((nDataH<<8)+nDataL);
  Data2Port(nPort,( *((XBYTE *) ITempW01)));
}

void Cmd_B7(BYTE nPort,BYTE nData)
{

  Data2Port(nPort,( *((XBYTE *)(OEMRAMA|nData)) ));
  if( nData == 0x3D )
  {
    EdgePasswordProcess();
  }
}



