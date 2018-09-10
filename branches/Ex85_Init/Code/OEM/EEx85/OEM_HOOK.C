/*-----------------------------------------------------------------------------
 * TITLE: OEM_HOOK.C
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//=============================================================
//EC ram define for project
//0x580 ~ 0x5FF reserved for project usage
XBYTE  EC_ON2_Cnt                  _at_(OEMRAM5+0x80);
XBYTE  Intel15InputCurrentA        _at_(OEMRAM5+0x81);
XBYTE  Intel15InputCurrentAD       _at_(OEMRAM5+0x82);
XBYTE  FN_ANK                      _at_(OEMRAM5+0x83);
XBYTE  PCHRSTCNT                   _at_(OEMRAM5+0x84);


#if Support_KBD_BL_LED
XBYTE  KBL_Interval_Cnt			   _at_(OEMRAM5+0x85);
XBYTE  KBL_Interval_Cnt_Temp       _at_(OEMRAM5+0x86);
XBYTE  KBL_Current_Level_Duty	   _at_(OEMRAM5+0x87);
XBYTE  KBL_Target_Level_Duty	   _at_(OEMRAM5+0x88);
XBYTE  KBL_Status				   _at_(OEMRAM5+0x89);
#endif //Support keyboard backlight




XBYTE  ClearTemp[4]				   _at_(OEMRAM5+0x8A);
//XBYTE  TypeCSMbusDataTemp[6]				   _at_(OEMRAM5+0x8E);
//XBYTE  TypeCAdpDetectDelay				   _at_(OEMRAM5+0x94);

#if PMIC_SUPPORT

XBYTE 	PMIC_counter        		  _at_  OEMRAM5+0x90;
XBYTE 	BatteryStopPolling        _at_  OEMRAM5+0x91;
XBYTE 	OTP_TEMP        			    _at_  OEMRAM5+0x92;
XBYTE 	PowerInitOK        			  _at_  OEMRAM5+0x93;
XBYTE 	PowerInitErrorCMD        	_at_  OEMRAM5+0x94;
XBYTE 	PowerInitErrorDATA        _at_  OEMRAM5+0x95;
XBYTE 	PowerInitError        		_at_  OEMRAM5+0x96;
XBYTE 	PowerInitIndex        		_at_  OEMRAM5+0x97;
XBYTE 	PowerReturnData 			    _at_  OEMRAM5+0x98;

XBYTE 	PowerVENDODID 			      _at_  OEMRAM5+0x99;
XBYTE 	PowerREVID 				        _at_  OEMRAM5+0x9A;
XBYTE 	PowerSDWN 				        _at_  OEMRAM5+0x9B;
XBYTE 	PowerEN_RW 			          _at_  OEMRAM5+0x9C;
XBYTE 	PowerEN_MASK 			        _at_  OEMRAM5+0x9D;
XBYTE 	PowerEN_DEGL_SEL1 		    _at_  OEMRAM5+0x9E;
XBYTE 	PowerEN_DEGL_SEL2 		    _at_  OEMRAM5+0x9F;
XBYTE 	PowerPG_STATE 			      _at_  OEMRAM5+0xA0;
XBYTE 	PowerOVP_REG 			        _at_  OEMRAM5+0xA1;
XBYTE 	PowerUVP_REG 			        _at_  OEMRAM5+0xA2;
XBYTE 	PowerOCP_REG 			        _at_  OEMRAM5+0xA3;
XBYTE 	PowerOTP_REG 			        _at_  OEMRAM5+0xA4;
XBYTE 	PowerBUCK1CTRL 			      _at_  OEMRAM5+0xA5;
XBYTE	  PowerBUCK2CTRL 			      _at_  OEMRAM5+0xA6;
XBYTE 	PowerBUCK3CTRL 			      _at_  OEMRAM5+0xA7;
XBYTE 	PowerLDO1CTRL 			      _at_  OEMRAM5+0xA8;
XBYTE 	PowerLDO2CTRL 			      _at_  OEMRAM5+0xA9;
XBYTE 	PowerDISCHCTRL1 		      _at_  OEMRAM5+0xAA;
XBYTE 	PowerDISCHCTRL2 		      _at_  OEMRAM5+0xAB;
XBYTE 	PowerOC_CTRL 			        _at_  OEMRAM5+0xAC;
XBYTE 	PowerBUCK_FSW 			      _at_  OEMRAM5+0xAD;

#endif
 XBYTE throttle_1             _at_ OEMRAM5+0xAF; 
XBYTE  ObjectPosition09V		   _at_(OEMRAM5+0xB0);
XWORD  PDO09V				       _at_(OEMRAM5+0xB1);
XWORD  PDO09I				       _at_(OEMRAM5+0xB3);
XBYTE  ObjectPosition20V		   _at_(OEMRAM5+0xB5);
XWORD  PDO20V				       _at_(OEMRAM5+0xB6);
XWORD  PDO20I				       _at_(OEMRAM5+0xB8);
XBYTE  CurrentRDO[4]			   _at_(OEMRAM5+0xBA);

XWORD    pwr_sys                   _at_ (OEMRAM5+0xC0); //C0,C1

XBYTE  SmartCharge                 _at_ OEMRAM5+0xEC;
XBYTE  SxS5_Record                 _at_ OEMRAM5+0xF0;

//XBYTE  ClearTemp[4]				   _at_(OEMRAM5+0xF1);
XBYTE  Adp_retry_cnt               _at_(OEMRAM5+0xF1);
XBYTE  TypeCSMbusDataTemp[6]				   _at_(OEMRAM5+0xd0);
XBYTE  TypeCAdpDetectDelay				   _at_(OEMRAM5+0xFC);


XBYTE  VGA_15_TYPE                 _at_ OEMRAM5+0xFE;
 XBYTE WorkaroundforBIOSSMI              _at_(OEMRAM5+0xF2);
 XBYTE WorkaroundforBIOSSMIcunt             _at_(OEMRAM5+0xF3);
 XBYTE adapterremove                       _at_(OEMRAM5+0xF4);
 XBYTE UCSIEVENTDEBOUNCE              _at_(OEMRAM5+0xF5);
 XBYTE DETECTDOCKORDAPTER              _at_(OEMRAM5+0xF6);
 XBYTE testzhuangqi              _at_(OEMRAM5+0xF7);
// XBYTE PD_Ini_Stable_Delay        _at_(OEMRAM5+0xF8);
 XBYTE BatteryOTPdebounce        _at_(OEMRAM5+0xF9);
 XBYTE UCSI_50CMD_WA			 _at_(OEMRAM5+0xFA);

 XWORD PD_Ini_Stable_Delay        _at_(OEMRAM5+0xD8);
 XBYTE  TEMP_Buff_4             _at_ (OEMRAM5 + 0xDA);
 XBYTE  TEMP_Buff_5             _at_ (OEMRAM5 + 0xDB);
 XBYTE  TEMP_Buff_6            _at_ (OEMRAM5 + 0xDC);
 XBYTE  TEMP_Buff_7             _at_ (OEMRAM5 + 0xDD);
 XBYTE  SMIBIOSFLAG            _at_ (OEMRAM5 + 0xDE);
 XBYTE EventList[4]			 _at_(OEMRAM5+0xc7);
//=============================================================



void OEM_Hook_IRQ_INT31_WKO21(void)
{
	SYSON_LOW();

	CLEAR_MASK(IER3, Int_WKO21);
	SET_MASK(ISR3, Int_WKO21);

	SET_MASK(WUESR2,WUC_WUI1);
}

void OEM_Hook_IRQ_INT1_WKO20(void)
{
#if (PLATFORM_CHIP == INTEL)
	VR_ON_OFF();
	SUSP_OFF();
	VCCSTPG_OUTPUT;
	VCCSTPG_LOW();

	CLEAR_MASK(IER0, Int_WKO20);
	SET_MASK(ISR0, Int_WKO20);
#endif
}

void OEM_Hook_Timer1msEvent(void)
{
#if (PLATFORM_CHIP == INTEL)
  if(Read_EC_ON() && EC_ON2_Cnt)
  {
    	if(EC_ON2_Cnt == 0x01)
	  {
		  EC_ON2_HI();
		  EC_ON2_Cnt = 0;
	  }
	  else
	  {
		  EC_ON2_Cnt--;
	  }
  }
#endif
}

void OEM_Hook_Power_90WSystem(void)
{
#if (PLATFORM_CHIP == INTEL)
  if (IS15Inch && N165_GTR){
    	Intel15InputCurrentA = 0x0F;
    	Intel15InputCurrentAD = 0x19;
  }else if(IS15Inch && N16P_GT){
    	Intel15InputCurrentA = 0x0D;
    	Intel15InputCurrentAD = 0x17;
  }else{
    Intel15InputCurrentA = 0;
    Intel15InputCurrentAD = 0;
  }
#endif
}

void Oem_Initialization_hook(void)
{
  InitPMICReg();
  InitPMICParameter();
}

//----------------------------------------------------------
//FOR FVT
//----------------------------------------------------------

void OEM_Hook_Timer10msEventA(void){
    BYTE index;
    if (SDV_PHASE() && BoardID_readyGET)
      {
       GPCRD4 = INPUT;
       GPCRI0 = OUTPUT;

    }
}

