/*
********************************************************************************
* LCFC Confidential                                                          
*                                                                                             
* Microcode Source Materials                                                         
*  
* ThinkPad Embedded Controller Microcode
* 
* COPYRIGHT LCFC 2016 ALL RIGHTS RESERVED
*
* Filename: OEM_EnhancedPcBeep.h
* 
* Description:
*    Definition of Enhanced PC Beep.
* 
* History:
*   08/23/2016    Isaac Lin
*     created as new
********************************************************************************
*/
#define Support_EnhancedPcBeep                  TRUE    // Support Enhanced PC Beep function

/*
*********************************************************
*     #EC ram define
*********************************************************
*/

#define  EnhancedPcBeep_DATA          ENHANCED_PC_BEEP_RAM0             //_at_ OEMRAM5+0x2B~0x32;
#define  EnhancedPcBeep_Status        ENHANCED_PC_BEEP_RAM1             //_at_ OEMRAM5+0x33;	//bit0~bit3:Process step
	#define     boot_done					 	BIT4
	#define     MilePostInterrupt 	BIT5
	#define     BeepActive 	  			BIT6
	#define     BeepBusy		  			BIT7
#define  EnhancedPcBeep_ID		        ENHANCED_PC_BEEP_RAM2             //_at_ OEMRAM5+0x34;
#define  EnhancedPcBeep_CheckSum      ENHANCED_PC_BEEP_RAM3             //_at_ OEMRAM5+0x35;
#define  EnhancedPcBeep_TimerCnt      ENHANCED_PC_BEEP_RAM4             //_at_ OEMRAM5+0x36; //Each code beep 200ms
#define  Mp_Fsm									      ENHANCED_PC_BEEP_RAM5             //_at_ OEMRAM5+0x37;
#define  EnhancedPcBeep_DelayCnt      ENHANCED_PC_BEEP_RAM6             //_at_ OEMRAM5+0x38;
#define  EnhancedPcBeep_repeat_time   ENHANCED_PC_BEEP_RAM7             //_at_ OEMRAM5+0x48;
#define  EnhancedPcBeep_Data_Cnt	    ENHANCED_PC_BEEP_RAM8             //_at_ OEMRAM5+0x49;

/*
*********************************************************
*     #define Constants
*********************************************************
*/
#define OPTION_TP1_NEC          0
/*
 * milepost define
 */
#define MP_RESERVED             0x00          // milepost reserved value
#define MP_BDS_START            0x61          // BDS Start
#define MP_START_OF_NONE_ERR    0x6A          // the smallest milepost which indicate no error (reserved)
#define MP_EFI_BOOT             0x6D          // EFI Boot
#define MP_LEGACY_BOOT          0x6E          // Legacy Boot
#define MP_START_IMAGE          0x6F          // Start Image
#define MP_END_OF_NONE_ERR      0x7F          // the largest milepost which indicate no error (reserved)

/*
 * info id define
 */
#define INFO_ID_01              0x01          // INFO ID: Thinkpad Error Code Ver.1
#define INFO_ID_02              0x02          // INFO ID: ThinkPad System Unit Serial Number

/*
 * error code define
 */
#define ERR_CODE_RESERVED       0x0000        // error code reserved value
#define ERR_CODE_0001           0x0001        // platform reset is not asserted
#define ERR_CODE_PREFIX_02      0x0200        // prefix byte of error code

/*
 * mask
 */
#define MSB_MASK                0x80          // mask be used to check MSB of milepost
#define NVS_BLOCK_SIZE          0x100
#define NVS_HEADER_SIZE         0x10
#define NVS_HEADER_ID           "SER#"
#define PNP_IDENTIFIER          0x00
#define SI_BlOCk_COUNT          0x04
#define SI_MAX_ENTRIES          0x05
#define SI_VERSION_ID           0x06
#define SI_AREA_CHECKSUM        0x08
#define MTM_DATA_LENGTH         0x0A
#define SN_DATA_LENGTH          0x08
#define MTM_SN_PREFIX1          0xB1
#define MTM_SN_PREFIX2          0x53
#define DEVICE_TYPE_LENOVO      0xC0
#define DEVICE_TYPE_NEC         0xC2
#define NVS_VER_ID              0x02
#define SN_CHAR_DUMMY           0x58        // Dummy Serial# ("X")
#define NUM_OF_INFO_ID_DIGIT    2           // digit number of info id: 2
#define NUM_OF_ERR_CODE_DIGIT   4           // digit number of error code: 4
#define NUM_OF_SN_CODE_DIGIT    8           // digit number of error code: 8
#define ENH_BEEP_PERIOD         200         // period per digit: 200 msec
#define FREQ_BLANK              0x10        // frequency of Blank: 2020Hz, ID define to 0x10

#define BEEP_800HZ     PwmModulator(50,57,6)			//PwmModulator(50,(92000/(800*2)),6)
#define BEEP_850HZ     PwmModulator(50,53,6)			//PwmModulator(50,(92000/(850*2))-1,6)
#define BEEP_900HZ     PwmModulator(50,50,6)			//PwmModulator(50,(92000/(900*2))-1,6)
#define BEEP_960HZ     PwmModulator(50,47,6)//48			//PwmModulator(50,(92000/(960*2)),6)
#define BEEP_1010HZ     PwmModulator(50,45,6)			//PwmModulator(50,(92000/(1010*2))-1,6)
#define BEEP_1070HZ     PwmModulator(50,42,6)			//PwmModulator(50,(92000/(1070*2))-1,6)
#define BEEP_1140HZ     PwmModulator(50,39,6)			//PwmModulator(50,(92000/(1140*2))-1,6)
#define BEEP_1200HZ     PwmModulator(50,37,6)			//PwmModulator(50,(92000/(1200*2))-1,6)
#define BEEP_1270HZ     PwmModulator(50,35,6)//35			//PwmModulator(50,(92000/(1270*2))-1,6)
#define BEEP_1350HZ     PwmModulator(50,33,6)			//PwmModulator(50,(92000/(1350*2))-1,6)
#define BEEP_1430HZ     PwmModulator(50,31,6)			//PwmModulator(50,(92000/(1430*2))-1,6)
#define BEEP_1520HZ     PwmModulator(50,29,6)//29			//PwmModulator(50,(92000/(1520*2))-1,6)
#define BEEP_1600HZ     PwmModulator(50,28,6)			//PwmModulator(50,(92000/(1600*2)),6)
#define BEEP_1700HZ     PwmModulator(50,26,6)			//PwmModulator(50,(92000/(1700*2))-1,6)
#define BEEP_1800HZ     PwmModulator_2(50,25,6)//25			//PwmModulator(50,(92000/(1800*2)),6)
#define BEEP_1910HZ     PwmModulator(50,23,6)			//PwmModulator(50,(92000/(1910*2))-1,6)
#define BEEP_2020HZ     PwmModulator(50,22,6)//22			//PwmModulator(50,(92000/(2020*2)),6)

#define Read_FnKey_Status     EC_KBFNON()

/*
*********************************************************
*     Global Macros
*********************************************************
*/


/*
*********************************************************
*     Global Data Types
*********************************************************
*/

/*
*********************************************************
*     Global Variables
*********************************************************
*/

/*
*********************************************************
*     Externals
*********************************************************
*/

/*
*********************************************************
*     Global Function Prototypes
*********************************************************
*/
extern void PlayErrorCodeBeep(void);
extern void DataToFrequence(BYTE BeepNumber);
//extern void InitialCCG3(void);
extern void EnhancedPcBeepProcess(void);
//extern void OEM_TYPE_C_init_hook(void);
extern BOOL MpHousekeeper(BYTE task_id);
extern BOOL Mp_Check_Milepost(BYTE milepost, BYTE *err_code);
extern BOOL Mp_Check_SerialInfo(void);
extern void Mp_Set_Sn_Dummy(BYTE *sn_char);
extern BOOL Mp_Get_SerialNumber(BYTE *sn_char);
extern void OEM_ENHANCED_PC_BEEP_HOOK(void);
extern void OEM_ENHANCED_PC_BEEP_Init_HOOK(void);
extern void OEM_ENHANCED_PC_BEEP_S3_HOOK(void);
extern void OEM_ENHANCED_PC_BEEP_S5_HOOK(void);
