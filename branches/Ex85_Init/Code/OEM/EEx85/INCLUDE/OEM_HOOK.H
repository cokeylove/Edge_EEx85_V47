/*-----------------------------------------------------------------------------
 * TITLE: OEM_HOOK.H
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef  OEM_HOOK_H
#define OEM_HOOK_H

//=============================================================
//EC ram define for project
//0x580 ~ 0x5FF reserved for project usage
extern XBYTE EC_ON2_Cnt;             //_at_(OEMRAM5+0x80);
extern XBYTE Intel15InputCurrentA;  //_at_(OEMRAM5+0x81);        //AC only
extern XBYTE Intel15InputCurrentAD; //_at_(OEMRAM5+0x82);       //AC + DC
extern XBYTE FN_ANK;                 //_at_(OEMRAM5+0x83);
extern XBYTE PCHRSTCNT;              //_at_(OEMRAM5+0x84);

#if Support_KBD_BL_LED
extern XBYTE KBL_Interval_Cnt;			 //_at_(OEMRAM5+0x85);
extern XBYTE KBL_Interval_Cnt_Temp;		 //_at_(OEMRAM5+0x86);
extern XBYTE KBL_Current_Level_Duty;	 //_at_(OEMRAM5+0x87);
extern XBYTE KBL_Target_Level_Duty;	     //_at_(OEMRAM5+0x88);
extern XBYTE KBL_Status;				 //_at_(OEMRAM5+0x89);
 #define  TargetChange    				BIT0
 #define  Frone    						BIT1	//1:up,0:down
 #define  PowerOnCheck					BIT2	//1:Check mode,0:Normal mode
 #define  NeedUpdateAcpiKbdBlStatus		BIT3	//1:Check mode,0:Normal mode
#endif //Support keyboard backlight

extern XBYTE ClearTemp[4];				 //_at_(OEMRAM5+0x8A);8A~8D
extern XBYTE TypeCSMbusDataTemp[6];				 //_at_(OEMRAM5+0x8E);8E~93
extern XBYTE TypeCAdpDetectDelay;				 //_at_(OEMRAM5+0x94);




#if PMIC_SUPPORT

extern XBYTE PMIC_counter;    		  // _at_  OEMRAM5+0x90;
extern XBYTE BatteryStopPolling;    // _at_  OEMRAM5+0x91;
extern XBYTE OTP_TEMP;    			    // _at_  OEMRAM5+0x92;
  #define b0OTP_Occurr_Flag                  BIT0 //JIMAPLO02B

extern XBYTE PowerInitOK;       		//_at_  OEMRAM5+0x93;
  #define PMIC_Check_Start                   BIT0 
  #define PMIC_Check_Result_Flag             BIT1
  #define PMIC_Intial_Result_Flag            BIT2
  #define PMIC_Polling_Start_Flag            BIT3
  #define PMIC_Polling_Result_Flag           BIT4
  #define PMIC_SMBUS_Retry_Fail_Flag         BIT5  
  #define PMIC_Polling_Retry_Fail_Flag       BIT6  
  #define PMIC_Polling_End_Flag              BIT7  

extern XBYTE PowerInitErrorCMD;      //_at_  OEMRAM5+0x94;
extern XBYTE PowerInitErrorDATA;     //_at_  OEMRAM5+0x95;
extern XBYTE PowerInitError;         //_at_   OEMRAM5+0x96;
extern XBYTE PowerInitIndex;         //_at_   OEMRAM5+0x97;
extern XBYTE PowerReturnData;    	   // _at_  OEMRAM5+0x98;

extern XBYTE PowerVENDODID;    	     // _at_  OEMRAM5+0x99;
extern XBYTE PowerREVID;    		     // _at_  OEMRAM5+0x9A;
extern XBYTE PowerSDWN;    		       // _at_  OEMRAM5+0x9B;
extern XBYTE PowerEN_RW;    		     // _at_  OEMRAM5+0x9C;
extern XBYTE PowerEN_MASK;   	 	     // _at_  OEMRAM5+0x9D;
extern XBYTE PowerEN_DEGL_SEL1;      // _at_  OEMRAM5+0x9E;
extern XBYTE PowerEN_DEGL_SEL2;      // _at_  OEMRAM5+0x9F;
extern XBYTE PowerPG_STATE;   		   // _at_  OEMRAM5+0xA0;
extern XBYTE PowerOVP_REG;   		     // _at_  OEMRAM5+0xA1;
extern XBYTE PowerUVP_REG;    		   // _at_  OEMRAM5+0xA2;
extern XBYTE PowerOCP_REG;    		   // _at_  OEMRAM5+0xA3;
extern XBYTE PowerOTP_REG;    		   // _at_  OEMRAM5+0xA4;
extern XBYTE PowerBUCK1CTRL;    	   // _at_  OEMRAM5+0xA5;
extern XBYTE PowerBUCK2CTRL;    	   // _at_  OEMRAM5+0xA6;
extern XBYTE PowerBUCK3CTRL;    	   // _at_  OEMRAM5+0xA7;
extern XBYTE PowerLDO1CTRL;    		   // _at_  OEMRAM5+0xA8;
extern XBYTE PowerLDO2CTRL;   		   // _at_  OEMRAM5+0xA9;
extern XBYTE PowerDISCHCTRL1;    	   // _at_  OEMRAM5+0xAA;
extern XBYTE PowerDISCHCTRL2;    	   // _at_  OEMRAM5+0xAB;
extern XBYTE PowerOC_CTRL;    		   // _at_  OEMRAM5+0xAC;
extern XBYTE PowerBUCK_FSW;       	 // _at_  OEMRAM5+0xAD;
#endif
extern XBYTE throttle_1;             //_at_ OEMRAM5+0xAF; 
#define b0_adpi BIT0
#define b1_OCP BIT1
#define b2_OTP BIT2
#define b3_pwr_sys BIT3
#define b4_battery_low BIT4
#define b5_battery_shipmode BIT5
#define b6_battery_loweventflag BIT6

extern XBYTE ObjectPosition09V;			 //_at_(OEMRAM5+0xB0);
extern XWORD PDO09V;				     //_at_(OEMRAM5+0xB1);
extern XWORD PDO09I;				     //_at_(OEMRAM5+0xB3);
extern XBYTE ObjectPosition20V;			 //_at_(OEMRAM5+0xB5);
extern XWORD PDO20V;				     //_at_(OEMRAM5+0xB6);
extern XWORD PDO20I;				     //_at_(OEMRAM5+0xB8);
extern XBYTE CurrentRDO[4];				 //_at_(OEMRAM5+0xBA);
extern XWORD    pwr_sys;        //_at_ OEMRAM5+0xC0; //C0,C1
extern XBYTE  SmartCharge;          //_at_ OEMRAM5 + 0xEC;
extern XBYTE  SxS5_Record;          //_at_ OEMRAM5+0xF0;
extern XBYTE Adp_retry_cnt;              //_at_(OEMRAM5+0xF1);
extern XBYTE  VGA_15_TYPE;           //_at_(OEMRAM5+0xFE);
  #define N16P_GT  IS_MASK_SET(VGA_15_TYPE, BIT0)
  #define N165_GTR IS_MASK_CLEAR(VGA_15_TYPE, BIT0)
  

extern XBYTE WorkaroundforBIOSSMI;              //_at_(OEMRAM5+0xF2);
extern XBYTE WorkaroundforBIOSSMIcunt;              //_at_(OEMRAM5+0xF3);
extern XBYTE adapterremove;              //_at_(OEMRAM5+0xF4);
extern XBYTE UCSIEVENTDEBOUNCE;              //_at_(OEMRAM5+0xF5);
extern XBYTE DETECTDOCKORDAPTER;              //_at_(OEMRAM5+0xF6);
#define b0_ACORDOCK BIT0//1:DOCK
extern XBYTE testzhuangqi;              //_at_(OEMRAM5+0xF7);
extern XWORD PD_Ini_Stable_Delay;        //_at_(OEMRAM5+0xF8);
extern XBYTE BatteryOTPdebounce;        //_at_(OEMRAM5+0xF9);

extern XBYTE UCSI_50CMD_WA; 			//_at_(OEMRAM5+0xFA);

extern XBYTE  TEMP_Buff_4;             //_at_ OEMRAM5 + 0xFA;
extern XBYTE  TEMP_Buff_5;             //_at_ OEMRAM5 + 0xFB;
extern XBYTE  TEMP_Buff_6;             //_at_ OEMRAM5 + 0xFC;
extern XBYTE  TEMP_Buff_7;             //_at_ OEMRAM5 + 0xFD;
extern XBYTE  SMIBIOSFLAG;           //_at_ (OEMRAM5 + 0xDE);
extern XBYTE EventList[4];				 //_at_(OEMRAM5+0xc7);
//=============================================================



#define OEM_Hook_IRQ_INT31_WKO21_support
#define OEM_Hook_IRQ_INT1_WKO20_support
extern void OEM_Hook_IRQ_INT31_WKO21(void);
extern void OEM_Hook_IRQ_INT1_WKO20(void);
#define OEM_Hook_Timer1msEvent_support
extern void OEM_Hook_Timer1msEvent(void);
#define OEM_Hook_Power_90WSystem_support
extern void OEM_Hook_Power_90WSystem(void);
#define Oem_Initialization_hook_Support
extern void Oem_Initialization_hook(void);


#endif
