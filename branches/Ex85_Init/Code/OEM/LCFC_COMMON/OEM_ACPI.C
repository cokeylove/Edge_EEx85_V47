/*-----------------------------------------------------------------------------
 * TITLE: OEM_ACPI.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// The hook function of 62 port data
//  Note :
//  PM1Step != 0x00 is necessary.
//----------------------------------------------------------------------------
void Hook_62Port(BYTE PM1Cmd)
{
  BYTE i;
#if   Support_RFID
  BYTE j;
#endif //Support_RFID  

  switch(PM1Cmd)
  {
#if   Support_RFID
	case 0x01:		
	  if( PM1Step == 0x01 )
	  {
	  	i = Cmd_01(PM1Data1,PM1Data,&j);
		if( i == TRUE )
			Data2Port(0x62,j);
	  }
	  break;
	case 0x02:		
	  if( PM1Step == 0x01 )
	  {
	  	i = Cmd_02(PM1Data2,PM1Data1,PM1Data);
		if( i == TRUE )
			Data2Port(0x62,0x00);
	  }
	  break;
	case 0x09:		
	  if( PM1Step == 0x01 )
	  {
	  	i = Cmd_09(PM1Data,&j);
		if( i == TRUE )
			Data2Port(0x62,j);
	  }
	  break;
    case 0x0A:		
	  if( PM1Step == 0x01 )
	  {
	  	Cmd_0A(PM1Data1,PM1Data);
	  }
	  break;
#endif //Support_RFID
    case 0x40:
      Cmd_40(PM1Data);
      break;
    case 0x41:
      if(PM1Data==0xA1) Cmd_41_A1(0x62);
      break;
    case 0x42:
      Cmd_42(PM1Data);
      break;
    case 0x43:
      Cmd_43(0x62,PM1Data);
      break;
    case 0x45:
       Cmd_45(0x62,PM1Data);
       break;
    case 0x46:
      break;
    case 0x47:
      Cmd_47(0x62,PM1Data);
      break;
    case 0x49:
      break;
    case 0x4B:
      if (PM1Step == 0x04)
      {
        i = PM1Data;
        break;
      }
      if (PM1Step == 0x03)
      {
        eEEPROMAddrsss = PM1Data;
        if ( PM1Data == 0xE0 )  // Check MBID address high byte.
        { SET_MASK(MBID_Reload, b0MBID_High); }
        break;
      }
      if (PM1Step == 0x02)
      {
        eEEPROMBank = PM1Data | EEPROM_Bank0;  
        if ( PM1Data == 0x07 )  // Check MBID address low byte.
        { SET_MASK(MBID_Reload, b1MBID_LOW); }
        break;
      }
      if (PM1Step == 0x01)
      {
        eEEPROMData  = PM1Data;
        if ( (MBID_Reload & 0x03) == 0x03 )
        { uMBID = PM1Data; }
        Update_EEPROMMark();
        Data2Port(0x62, 0x00);
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
      if (PM1Step == 0x03)
      {
        i = PM1Data;
        break;
      }
      if (PM1Step == 0x02)
      {
        eEEPROMAddrsss = PM1Data;
        break;
      }
      if (PM1Step == 0x01)
      {
        eEEPROMBank = PM1Data | EEPROM_Bank0;  
        Cmd_4E(0x62,eEEPROMAddrsss);
      }
      break;

    case 0x4D:
      if (PM1Step == 0x02)
      {
        eEEPROMAddrsss = PM1Data;
        break;
      }
      if (PM1Step == 0x01)
      {
        eEEPROMData  = PM1Data;
        Update_EEPROMMark();
        Data2Port(0x62, 0x00);
      }
      break;

    case 0x4E:
      eEEPROMAddrsss = PM1Data;
      Cmd_4E(0x62,eEEPROMAddrsss);
      break;

    case 0x51:
      break;

    case 0x52:
      if(PM1Data==0xA0) Cmd_52_A0(0x62);
      if(PM1Data==0xA3) Cmd_52_A3(0x62);
      if(PM1Data==0xA4) Cmd_52_A4(0x62);
      if(PM1Data==0xA5) Cmd_52_A5(0x62);
      break;

    case 0x53:
      if (PM1Step == 0x01)
      {
        Cmd_53(0x62,PM1Data2,PM1Data1,PM1Data);
      }
      break;
    case 0x56:
      break;
    case 0x58:
      Cmd_58(PM1Data);
      break;
    case 0x59:
      Cmd_59(0x62,PM1Data,0);
      break;
#if EC_Brightness_Ctrl      
    case 0x5C:
      Cmd_5C(0x62);
      break;
    case 0x5D:
      Cmd_5D(PM1Data);
      break;
#endif      
    case 0xB0:
      Cmd_B0(0x62,PM1Data);
      break;
//[Project][CC20130425][Start]Add first used date feature.
    case 0xB2:
      if (PM1Step == 0x01)
      {
        Cmd_B2(PM1Data2,PM1Data1,PM1Data);
      }
      break;
//[Project][CC20130425][End]Add first used date feature.
    case 0xB3:
      if (PM1Step == 0x01)
      {
        Cmd_B3(0x62,PM1Data1,PM1Data);
      }
      break;
  }
}

//----------------------------------------------------------------------------
// The hook function of 66 port command.
//----------------------------------------------------------------------------
void Hook_66Port(BYTE PM1Cmd)
{
  switch(PM1Cmd)
  {
#if Support_RFID
  	case 0x01:		
  	  PM1Step=0x02;
	  break;
	case 0x02:		
	  PM1Step=0x03;
	  break;
	case 0x09:		
	  PM1Step=0x01;
	  break;
	case 0x0A:		
	  PM1Step=0x02;
	  break;
#endif //Support_RFID
    case 0x40:
      PM1Step=0x01;
      break;
    case 0x41:
      PM1Step=0x01;
      break;
    case 0x42:
      PM1Step=0x01;
      break;
    case 0x43:
      PM1Step=0x01;
      break;
    case 0x44:
      Cmd_44(0x62);
      break;
    case 0x45:
      PM1Step=0x01;
      break;
    case 0x46:
    break;
    case 0x47:
      PM1Step=0x01;
      break;
    case 0x49:
    break;
    case 0x4B:
      PM1Step=0x04;
      break;
    case 0x4C:
      PM1Step=0x03;
      break;
    case 0x4D:
      PM1Step=0x02;
      break;
    case 0x4E:
      PM1Step=0x01;
      break;

    case 0x4F:
      Erase_EEPROMAll();
      break;

    case 0x51:
      Cmd_51(0x62);
      break;
    case 0x52:
      PM1Step=0x01;
      break;
    case 0x53:
      PM1Step=0x02;
      break;
    case 0x56:
      Cmd_56(0x62);
      break;
    case 0x58:
      PM1Step=0x01;
      break;
    case 0x59:
      PM1Step=0x01;
      break;
    case 0x5C:
      break;
    case 0x5D:
      PM1Step=0x01;
      break;
    case 0xB0:
      PM1Step=0x01;
      break;
//[Project][CC20130425][Start]Add first used date feature.
    case 0xB2:
      PM1Step=0x03;
      break;
//[Project][CC20130425][End]Add first used date feature.
    case 0xB3:
      PM1Step=0x02;
      break;
  }
}


#if ACPI_Smbus
BYTE vOSSMbusReceiveByte(void)
{
    return (bSMBusReceiveByte(SMbusChB, uSMBusAddr, &uSMBusData0));
}
BYTE vOSSMbusWBlcok(void)
{
///    return (bWSMBusBlock(SMbusCh1, SMbusRBK, uSMBusAddr, uSMBusCmd, &uSMBusData0, uSMBusBcnt, (uSMBusPrtcl & 0x80)));
    return (bWSMBusBlock(SMbusChB, SMbusRBK, uSMBusAddr, uSMBusCmd, &uSMBusData0, uSMBusBcnt,SMBus_NeedPEC));
}

BYTE vOSSMbusRBlcok(void)
{
  return (bRSMBusBlock(SMbusChB, SMbusRBK, uSMBusAddr, uSMBusCmd, &uSMBusData0));
}

BYTE vOSSMbusRByte(void)
{
  return (bRWSMBus(SMbusChB, SMbusRB, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NeedPEC));
}

BYTE vOSSMbusRWord(void)
{
  return (bRWSMBus(SMbusChB, SMbusRW, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NeedPEC));
}

BYTE vOSSMbusWBYTE(void)
{
  return (bRWSMBus(SMbusChB, SMbusWB, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NeedPEC));
}

BYTE vOSSMbusWWord(void)
{
  return (bRWSMBus(SMbusChB, SMbusWW, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NeedPEC));
}

void DummyFunction(void)
{
}

const FUNCT_PTR_B_V code vOSSMbusComd_table[16] =
{                //the low nibble of uSMBusPrtcl
  DummyFunction,    //0x00
  DummyFunction,    //0x01
  DummyFunction,    //0x02-Write Quick Command
  DummyFunction,    //0x03-Read Quick Command
  DummyFunction,    //0x04-Send Byte
  vOSSMbusReceiveByte,    //0x05-Receive Byte
  vOSSMbusWBYTE,    //0x06-Write Byte
  vOSSMbusRByte,    //0x07-Read Byte
  vOSSMbusWWord,    //0x08-Write Word
  vOSSMbusRWord,    //0x09-Read Word
  vOSSMbusWBlcok,    //0x0A-Write Block
  vOSSMbusRBlcok,    //0x0B-Read Block
  DummyFunction,    //0x0C-Process Call
  DummyFunction,    //0x0D-Write Block-Read Block Process Call
  DummyFunction,    //0x0E
  DummyFunction    //0x0F
};

// ACPI response to writes to SMBus Protocol register.
void OSSMbusComd(void)
{

	if( (uSMBusPrtcl>=0x01) &&  ((uSMBusPrtcl&0xF0)!= 0x10))	//Due to battery APP will send the 8xH protocol cmd and not 0xH.Setting 0xH and 2xH~FxH are all support SMBus process.
  	{
    	uSMBusSts = 0x00;

        if((vOSSMbusComd_table[(uSMBusPrtcl & 0x0F)])()) {
         uSMBusSts = 0x80;
        }else {
          if( IS_MASK_SET(HOSTA_B,BIT0) ) { uSMBusSts = 0x1A; }
          else if( IS_MASK_SET(HOSTA_B,BIT2) ) { uSMBusSts = 0x17; }
          else if( IS_MASK_SET(HOSTA_B,BIT3) ) { uSMBusSts = 0x07; }
          else if( IS_MASK_SET(HOSTA_B,BIT4) ) { uSMBusSts = 0x19; }
          else if( IS_MASK_SET(HOSTA_B,BIT5) ) { uSMBusSts = 0x10; }
          else if( IS_MASK_SET(HOSTA_B,BIT6) ) { uSMBusSts = 0x18; }
    	}
        uSMBusPrtcl = 0x00;
    //ECSMI_SCIEvent(ACPI_ACIN_SCI);  // toggle HOST
    }
}

#endif  // ACPI_Smbus

#if RTS5400_Smbus
BYTE vRTSSMbusWBlcok(void)
{
    return (bWSMBusBlock(SMbusCh1, SMbusWBK, uSMBusAddr, uSMBusCmd, &uSMBusData0, uSMBusBcnt,SMBus_NoPEC));
}

BYTE vRTSSMbusRBlcok(void)
{
	return (bRSMBusBlock(SMbusCh1, SMbusRBK, uSMBusAddr, uSMBusCmd, &uSMBusData0));
}

BYTE vRTSSMbusRByte(void)
{
	return (bRWSMBus(SMbusCh1, SMbusRB, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NoPEC));
}

BYTE vRTSSMbusRWord(void)
{
	return (bRWSMBus(SMbusCh1, SMbusRW, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NoPEC));
}

BYTE vRTSSMbusWBYTE(void)
{
	return (bRWSMBus(SMbusCh1, SMbusWB, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NoPEC));
}

BYTE vRTSSMbusWWord(void)
{
	return (bRWSMBus(SMbusCh1, SMbusWW, uSMBusAddr, uSMBusCmd, &uSMBusData0, SMBus_NoPEC));
}

BYTE vRTSSMbusSendBYTE(void)
{
	return(bSMBusSendByte(SMbusCh1, uSMBusAddr, uSMBusCmd ));
}

BYTE vRTSSMbusReciveBYTE(void)
{
    return (bSMBusReceiveByte(SMbusCh1, uSMBusAddr, &uSMBusData0));  
}


void RTSSMbusComd(void)
{
	BYTE result;
	if( SMB_PRTC0 != 0x00 )
	{
  		uSMBusSts = 0x00;
	
		SMB_PRTC0 = SMB_PRTC0 & 0x0F;

		if(SMB_PRTC0 == 0x04)		//0x04-Send Byte
		{
			result = vRTSSMbusSendBYTE();
		}
		else if(SMB_PRTC0 == 0x05)		//0x05-Receive Byte
		{
			result = vRTSSMbusReciveBYTE();
		}
		else if(SMB_PRTC0 == 0x06)		//0x06-Write Byte
		{
			result = vRTSSMbusWBYTE();
		}
		else if(SMB_PRTC0 == 0x07)		//0x07-Read Byte
		{
			result = vRTSSMbusRByte();
		}
		else if(SMB_PRTC0 == 0x08)		//0x08-Write Word
		{
			result = vRTSSMbusWWord();
		}
		else if(SMB_PRTC0 == 0x09)		//0x09-Read Word
		{
			result =  vRTSSMbusRWord();
		}
		else if(SMB_PRTC0 == 0x0A)		//0x0A-Write Block
		{
			result = vRTSSMbusWBlcok();
		}
		else if(SMB_PRTC0 == 0x0B)		//0x0B-Read Block
		{
			result = vRTSSMbusRBlcok();
		}
		if(result)
		{
			uSMBusSts = 0x80;
		}
		else
		{
			if( IS_MASK_SET(HOSTA_A,BIT0) ) { uSMBusSts = 0x1A; }
      		else if( IS_MASK_SET(HOSTA_A,BIT2) ) { uSMBusSts = 0x17; }
      		else if( IS_MASK_SET(HOSTA_A,BIT3) ) { uSMBusSts = 0x07; }
      		else if( IS_MASK_SET(HOSTA_A,BIT4) ) { uSMBusSts = 0x19; }
      		else if( IS_MASK_SET(HOSTA_A,BIT5) ) { uSMBusSts = 0x10; }
      		else if( IS_MASK_SET(HOSTA_A,BIT6) ) { uSMBusSts = 0x18; }
			else { uSMBusSts = 0x5A; }	// alex 
		}
		
      	SMB_PRTC0 = 0x00;

  	}
}

#endif
#define ACPI_I2CBus	TRUE
#ifdef ACPI_I2CBus
BYTE vOSI2CWrite(void)
{
	return(I2C_Write(SMbusChA, uSMBusAddr, uSMBusBcnt,&uSMBusCmd));
}

BYTE vOSI2CRead(void)
{
	return(I2C_Read(SMbusChA, uSMBusAddr, uSMBusBcnt,&uSMBusData0, 0x00));	
}

BYTE vOSI2CWrite2Read(void)
{
	I2C_DATA = uSMBusCmd;
	I2C_DATA1[0] = uSMBusData0;			
	return(I2C_Write_to_Read(SMbusChA, uSMBusAddr, 0x02,&I2C_DATA, uSMBusBcnt,&uSMBusData0, 0));						
}

const FUNCT_PTR_B_V code vOSI2CbusComd_table[16] =
{                	//the low nibble of uSMBusPrtcl
  DummyFunction,    //0x00
  DummyFunction,    //0x01
  DummyFunction,    //0x02
  DummyFunction,    //0x03
  DummyFunction,    //0x04
  DummyFunction,    //0x05
  vOSI2CWrite,    	//0x06-I2C Write
  vOSI2CRead,    	//0x07-I2C Read
  vOSI2CWrite2Read, //0x08-I2C Write to Read
  DummyFunction,    //0x09
  DummyFunction,    //0x0A
  DummyFunction,    //0x0B
  DummyFunction,    //0x0C
  DummyFunction,    //0x0D
  DummyFunction,    //0x0E
  DummyFunction    	//0x0F
};

// ACPI response to writes to SMBus Protocol register.
void OSI2CbusComd(void)
{
	if( (uSMBusPrtcl>=0x11) &&  (uSMBusPrtcl<= 0x1F))
  	{
    	uSMBusSts = 0x00;
        if((vOSI2CbusComd_table[(uSMBusPrtcl & 0x0F)])())
		{
         	uSMBusSts = 0x80;
        }
		else
		{
        if( IS_MASK_SET(HOSTA_A,BIT0) ) { uSMBusSts = 0x1A; }
        else if( IS_MASK_SET(HOSTA_A,BIT2) ) { uSMBusSts = 0x17; }
        else if( IS_MASK_SET(HOSTA_A,BIT3) ) { uSMBusSts = 0x07; }
        else if( IS_MASK_SET(HOSTA_A,BIT4) ) { uSMBusSts = 0x19; }
        else if( IS_MASK_SET(HOSTA_A,BIT5) ) { uSMBusSts = 0x10; }
        else if( IS_MASK_SET(HOSTA_A,BIT6) ) { uSMBusSts = 0x18; }
    }
        uSMBusPrtcl = 0x00;

    //ECSMI_SCIEvent(ACPI_ACIN_SCI);  // toggle HOST
    }
}

#endif  // ACPI_I2CBus

//----------------------------------------------------------------------------
// Hook function of ACPI command
//----------------------------------------------------------------------------
void Hook_ACPICommand(void)
{



#if Support_UCSI 
	BYTE length;
	BYTE status;
  if (EC_I2C_ADDR == uSMBusAddr)
  {
    if(0x7f & uSMBusPrtcl)
    {
    	status = EcCmd((0x7f & uSMBusPrtcl), uSMBusCmd, uSMBusBcnt, &uSMBusData0, &length);
    	Set_Smbus_Communication_Done(length, status);
    }
  }
#endif  
if(RTS5455_I2C_ADDR == uSMBusAddr)
	{
		
		#if RTS5400_Smbus
		RTSSMbusComd();
		#endif
		
	}
#if ACPI_Smbus
  OSSMbusComd();    // ACPI SMBus command  
#endif
	
#ifdef ACPI_I2CBus
	OSI2CbusComd();	
#endif	
}

//-------------------------------------------------------------------------------
// parameter "QeventSCI" only, For a pulsed SCI
//------------------------------------------------------------------------------
void OEM_ACPI_Gen_Int(void)
{
    ACPI_Gen_Int(QeventSCI);
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void ResetSCIEvent(void)
{

    CLEAR_MASK(PM1STS,SCIEVT);
    ECCheckBurstMode = 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: ECQEvent -Setup cause flag for an SCI and start the ACPI_Timer.
 *
 * sci_number = 1 through 255.  The Host uses this number to determine the
 * cause of the SCI.
 * ------------------------------------------------------------------------- */
void ECQEvent(BYTE sci_number)
{
  if(IS_MASK_CLEAR(SYS_MISC1,ACPI_OS) || (SysPowState!=SYSTEM_S0))
  {
    return;
  }

  ITempB05 = SCI_Event_In_Index;       // Get the input index.
  SCI_Event_Buffer[ITempB05] = sci_number; // Put the SCI number in the buffer.
   ITempB05++;                  // Increment the index.
   if (ITempB05 >= EVENT_BUFFER_SIZE)
  {
       ITempB05 = 0;
   }
                       // If the buffer is not full, update the input index.
    if (ITempB05 != SCI_Event_Out_Index)
  {
       SCI_Event_In_Index = ITempB05;
    }
  SET_MASK(PM1STS,SCIEVT);
    OEM_ACPI_Gen_Int();
}

void ECSMI_SCIEvent(BYTE smi_number)
{
  if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
    ECQEvent(smi_number);
 else
   ECSMIEvent(smi_number);
}

/**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  <Function Name> SMI_INTERRUPT

  Parameters:   None

  Returns:     None

  Description:   Generate SMIs for Query event request
-----------------------------------------------------------------------------*/
void SMI_INTERRUPT(void)
{
  DisableAllInterrupt();
    SMI_ON();        // set SMI pin low
  WNCKR = 0x00;           // Delay 15.26 us
    SMI_OFF();        // set SMI pin high
    EnableAllInterrupt();
}
/**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  <Function Name> ECSMIEvent

  Parameters:   [NewSMI] -- SMI Query Event Value

  Returns:     [1]:TRUE
          [0]:FALSE

  Description:   Write SMI Query Value to Buffer
-----------------------------------------------------------------------------*/
BYTE ECSMIEvent(BYTE NewSMI)
{
if(SysPowState!=SYSTEM_S0)
  {
    return FALSE;
  }
if(SMIBIOSFLAG!=0)
{
    if( (NewSMI == 0x00))                    //check input SMI Number
    return FALSE;
if( ( NewSMI == 0x74)||(NewSMI == 0x43)||(NewSMI == 0xFF))  
{
  pSMIEvent = NewSMI;
    SET_MASK(PM1STS,BIT6);                  //Set SMI Query flag
    SMI_INTERRUPT();                    //Generate SMI Interrupt
    return FALSE;
	}
}
else if(NewSMI == 0xFF)
{ pSMIEvent = NewSMI;
    SET_MASK(PM1STS,BIT6);                  //Set SMI Query flag
    SMI_INTERRUPT();                    //Generate SMI Interrupt
    return FALSE;
}
return FALSE;

}


//----------------------------------------------------------------------------
// Read EC ram Space
//----------------------------------------------------------------------------
BYTE Hook_ReadMapECSpace(BYTE MapIndex)
{
  return( *((XBYTE *)(ECNameSpace|MapIndex)) );
}

//----------------------------------------------------------------------------
// Write EC ram Space
//----------------------------------------------------------------------------
void Hook_WriteMapECSpace(BYTE MapIndex, BYTE data1)
{
	if(MapIndex!=0xFD)
	{*((XBYTE *)(ECNameSpace|MapIndex)) = data1;
	}else
	{
	if(Read_BKOFF() )
	{*((XBYTE *)(ECNameSpace|MapIndex)) = data1;
	}
	}
 // *((XBYTE *)(ECNameSpace|MapIndex)) = data1;

  switch( MapIndex )   
  {
    case 0x03:   //u4Cfgspace3
      if (data1 & BIT2) {
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

    case 0x2F:   //fan control
      if (!FanAutomaticmodeGet){
        FAN_PWM = FAN_PWM_Min;   //WA for MFG test tool
        Fan_control_update();
      }
    break;
  
    case 0x3D:   //u4PassCtrl
      EdgePasswordProcess();
    break;

    case 0x81:   //u4BattInfoID
      IBMbatteryStore();
    break;

    case 0x8F:   //fan control
      Fan_control_update();
    break;
    
  }
}


void Data_To_PM1_nWait(BYTE data_byte)
{
    Data_To_PM1(data_byte);

    TR1 = 0;                    // Disable timer1
    ET1 = 0;                    // Disable timer1 interrupt
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    TH1 = Timer_85ms>>8;        // Set timer1 counter 26ms
    TL1 = Timer_85ms;           // Set timer1 counter 26ms
    TF1 = 0;                    // Clear overflow flag
    TR1 = 1;                   // Enable timer1

  while (!TF1)
  {
      if(IS_MASK_CLEAR(PM1STS, OBF))
        {
            break;
        }
 //       if(IS_MASK_SET(PM1STS, IBF))
 //       {
 //           break;
 //       }
  }

    TR1 = 0;      // disable timer1
   TF1 = 0;      // clear overflow flag
  ET1 = 1;      // Enable timer1 interrupt
}

#if Support_RFID
void Data_To_PM2_nWait(BYTE data_byte)
{
    Data_To_PM2(data_byte);

    TR1 = 0;                    // Disable timer1
    ET1 = 0;                  	// Disable timer1 interrupt
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    TH1 = Timer_85ms>>8;        // Set timer1 counter 26ms
    TL1 = Timer_85ms;           // Set timer1 counter 26ms
    TF1 = 0;                  	// Clear overflow flag
    TR1 = 1;                 	// Enable timer1

	while (!TF1)
	{
	    if(IS_MASK_CLEAR(PM2STS, OBF))
        {
            break;
        }
 //       if(IS_MASK_SET(PM1STS, IBF))
 //       {
 //           break;
 //       }
	}

    TR1 = 0;			// disable timer1
 	TF1 = 0;			// clear overflow flag
	ET1 = 1;			// Enable timer1 interrupt
}
#endif //Support_RFID

void Data2Port(BYTE nPort, BYTE rData)
{
  switch(nPort) {
    case 0x60:
//      Data_To_Host(rData);
      Data_To_Host_nWait(rData);
      break;
    case 0x62:
//      Data_To_PM1(rData);
      Data_To_PM1_nWait(rData);
#if !Support_RFID	  
      Delay1MS(5);	// 5ms  
#endif //!Support_RFID	  
      break;
    case 0x68:
      Data_To_PM2(rData);
      break;
    case 0xFE:
#if Support_COMPAL_MFG_TOOL      
      MMIO_rDATA[MMIO_rDATA_index] = rData;
      MMIO_rDATA_index++;
#endif      
      break;
  }
}

void MultiB2Port(BYTE nPort, BYTE rData)
{
  switch(nPort) {
    case 0x60:
//      KBC_DataPending(rData);
      Data_To_Host_nWait(rData);
      break;
    case 0x62:
//      PM1_DataPending(rData);
      Data_To_PM1_nWait(rData);
      break;
    case 0x68:
      PM2_DataPending(rData);
      break;
    case 0xFE:
#if Support_COMPAL_MFG_TOOL        
      MMIO_rDATA[MMIO_rDATA_index] = rData;
      MMIO_rDATA_index++;
#endif      
      break;
  }
}

/*
********************************************************************************
* Function name: Set_Smbus_Communication_Done
*
* Description:
*
*          This is used to set SMbus communication done to Status area.
*
* Arguments:
*    int           length 
*    unsigned char status : result of communication
* Return Values:
*    none
********************************************************************************
*/

void Set_Smbus_Communication_Done(BYTE length, BYTE status) {
  uSMBusBcnt  = length;
#if Support_UCSI 
  Update_Smbus_Status(status | SMB_STS_DONE);
#endif
  uSMBusPrtcl = 0;
	//ECSMI_SCIEvent(AM_SMB_EVENT);
}

/*
********************************************************************************
* Function name: Update_Smbus_Status
*
* Description:
*
*          This is used to modify Smbus Status byte.
*
* Arguments:
*    unsigned char status
* Return Values:
*    none
********************************************************************************
*/
#if Support_UCSI 
void Update_Smbus_Status(BYTE status) {
    //CmnDisableInt();
    uSMBusSts = status | (uSMBusSts & SMB_STS_ALARM);
    //CmnEnableInt();
}
#endif