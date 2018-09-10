/*-----------------------------------------------------------------------------
 * TITLE: OEM_GPIO.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_GPIO_H
#define OEM_GPIO_H

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
#define HighActive      1
#define LowActive       0

//-----------------------------------------------------------------
// GPIO Registers define
//-----------------------------------------------------------------
/*  General Control Register (GCR)  */
// BIT 7     -  GPIOB5 HIGH Follow LPCRST# HIGH-to-LOW
// BIT 6     -  WUI7 Enabled
// BIT 5     -  WUI6 Enabled
// BIT [4:3] -  Reserved
// BIT [2:1] -  [00] Reserved
//              [01] LPCRST# in GPIOB7
//              [10] LPCRST# in GPIOD2
//              [11] LPCRST# is dsiabled
// BIT 0     -  GFLE Set (GFLES0)

#define GCR_Init      0x06             // LPCRST# is dsiabled

/*  General Control 2 Register (GCR2)  */
// BIT 7     -  TACH2Enable (TACH2E)
// BIT 6     -  Reserved
// BIT 5     -  SMBus Channel 3 Enable (SMB3E), GPIOH1(94) as SM_CLK, GPIOH2(95) as SM_DAT
// BIT 4     -  PECI Enable (PECIE)
// BIT 3     -  TMB1 Enable (TMB1EN)
// BIT 2     -  TMB0 Enable (TMB0EN)
// BIT 1     -  TMA1 Enable (TMA1EN)
// BIT 0     -  TMB0 Enable (TMA0EN)

#define GCR2_Init     0x20             // Turn on SMBus4 function.

#define GCR21_Init    0x08            //GPG6(PCH_ENBL) is 1.8V 

#define GPOTE_Init    0x05            //GPE0 GPE2 set as OD; 
#define GPOTH_Init    0x01            //GPH0 set as OD; 
#define GPOTJ_Init	  0x10

#define B_phase       FALSE            // if B_phase == TRUE , then GPIO is for B_phase use only.
#define C_phase       FALSE            // if C_phase == TRUE , then GPIO is for C_phase use only.

//[-start-130918-T130124_001-modify]//
//-----------------------------------------------------------------
// GPIO Port A Registers define
//-----------------------------------------------------------------
#define PortA_Init    0x00             // PORT Data

#define PortA0_Ctrl   OUTPUT            // 24   LOGO_LED#:POWER LED,Low active
#define PortA1_Ctrl   OUTPUT            // 25   KB_BLK_PWM
#define PortA2_Ctrl   OUTPUT            // 28   EC_ON2
#define PortA3_Ctrl   OUTPUT            // 29   PWRBTN_LED#
#define PortA4_Ctrl   OUTPUT            // 30   VR_ON
#define PortA5_Ctrl   OUTPUT            // 31   EC_FAN_PWM
#define PortA6_Ctrl   OUTPUT            // 32   BEEP#
#define PortA7_Ctrl   OUTPUT            // 34   VCCST_PG_EC // init low
//-----------------------------------------------------------------
#define KB_BLK_PWM()    IS_MASK_SET(GPDRA,BIT(1))

#if (PLATFORM_CHIP == INTEL)
#define EC_ON2_HI()     SET_MASK(GPDRA,BIT(2))
#define EC_ON2_LOW()    CLEAR_MASK(GPDRA,BIT(2));
#define Read_EC_ON2()      IS_MASK_SET(GPDRA,BIT(2))
#endif

#define EC_ON2_3V_HI()     SET_MASK(GPDRA,BIT(2))
#define EC_ON2_3V_LOW()    CLEAR_MASK(GPDRA,BIT(2));
#define Read_EC_ON2_3V()   IS_MASK_SET(GPDRA,BIT(2))

#if (PLATFORM_CHIP == INTEL)
//
// CPU Vcore
//
// Remove VR_ON pin for one chip setting
#define VR_ON_OUTPUT      GPCRA4 = OUTPUT
#define VR_ON_ON()        SET_MASK(GPDRA,BIT(4))
#define VR_ON_OFF()       CLEAR_MASK(GPDRA,BIT(4))
#define Read_VR_ON()      IS_MASK_SET(GPDRA,BIT(4))
#endif

#define APUPWR_EN_HI()        SET_MASK(GPDRA,BIT(4))
#define APUPWR_EN_LOW()       CLEAR_MASK(GPDRA,BIT(4))
#define Read_APUPWR_EN()      IS_MASK_SET(GPDRA,BIT(4))


#define FAN_PWM           DCR5
#define FAN_PWM_INDW      GPCRA5 = (INPUT | PULL_DW)  // Set FAN_PWM input+pull down.
#define FAN_PWM_ALT       GPCRA5 = ALT                // Set FAN_PWM Alt.
#define FAN_PWM_OUT       GPCRA5 = OUTPUT             // Set FAN_PWM OUTPUT.
#define EC_FAN_PWM_HI()   SET_MASK(GPDRA,BIT(5))

#define PWM_BEEP            DCR6
#define BEEP_INIT           (GPCRA6 = PortA6_Ctrl)    // init PWM Beep.
#define BEEP_ALT            (GPCRA6 = ALT)            // Set PWM BEEP ALT.

#if (PLATFORM_CHIP == INTEL)
//
// Intel +3V PCH
//
#define PCH_PWR_EN_OFF() // SET_MASK(GPDRH,BIT(4))
#define LAN_PWRON_ON()  //SET_MASK(GPDRH,BIT(4))
#define LAN_PWRON_OFF()   //CLEAR_MASK(GPDRH,BIT(4))
//#define Read_LAN_PWRON() IS_MASK_SET(GPDRH,BIT(4))
//
// Intel +1.05V PCH
//
//#define VSPGOOD_EC_LOW()  CLEAR_MASK(GPDRA,BIT(3))
//#define VSPGOOD_EC_HI() SET_MASK(GPDRA,BIT(3))
//[-start-131029-T130124-modify]//
//#define Read_ALL_PWRGD() IS_MASK_SET(GPDRA,BIT(3))
//[-end-131029-T130124-modify]//
#define VCCSTPG_OUTPUT      GPCRA7 = OUTPUT
#define VCCSTPG_INPUT      GPCRA7 = INPUT
#define VCCSTPG_LOW()  CLEAR_MASK(GPDRA,BIT(7))
#define VCCSTPG_HI() SET_MASK(GPDRA,BIT(7))
#define Read_VCCSTPG() IS_MASK_SET(GPDRA,BIT(7))
#else // PLATFORM_CHIP
#define EC_MUTE_ACT()     CLEAR_MASK(GPDRA,BIT(7))
#define EC_MUTE_INACT()   SET_MASK(GPDRA,BIT(7))
#endif // PLATFORM_CHIP



#if EC_Brightness_Ctrl
#define Read_EC_INVT_PWM()  IS_MASK_SET(GPDRA,BIT(7))
#define EC_INVT_PWM_ALT     GPCRA7 = ALT
#define EC_INVT_PWM_OUTPUT  GPCRA7 = OUTPUT
#define EC_INVT_PWM_HI()    SET_MASK(GPDRA,BIT(7))
#endif  // EC_Brightness_Ctrl

//-----------------------------------------------------------------
// GPIO Port B Registers define
//-----------------------------------------------------------------
#define PortB_Init    0x00              // PORT Data

#define PortB0_Ctrl   INPUT             // 108  ACIN
#define PortB1_Ctrl   INPUT             // 109  LID_SW#
#define PortB2_Ctrl   INPUT             // 123  AOU_DET#
#define PortB3_Ctrl   INPUT             // 110  ON/OFFBTN#
#define PortB4_Ctrl   (INPUT | PULL_DW) // 111  NC
#if (PLATFORM_CHIP == INTEL) 
#define PortB5_Ctrl   OUTPUT            // 126  OTG
#else
#define PortB5_Ctrl   INPUT             // 126
#endif

#define PortB6_Ctrl   OUTPUT            // 4    KBRST#
//-----------------------------------------------------------------
#define Read_AC_IN()      IS_MASK_SET(GPDRB,BIT(0))
#define AC_IN_ALT         GPCRB0 = ALT              // Set AC_IN Alternate.
#define AC_IN_INPUT       GPCRB0 = INPUT            // Set AC_IN INPUT.

#define Read_LID_SW_IN()  IS_MASK_SET(GPDRB,BIT(1))

#define Read_AOU_DET()    IS_MASK_SET(GPDRB,BIT(2))

#if Support_TYPE_C 
#define Read_DCIN_ATTACHED2()    0//IS_MASK_SET(GPDRB,BIT(2))
#endif //Support_TYPE_C

#define Read_EC_PWRBTN()  IS_MASK_CLEAR(GPDRB,BIT(3))

#if (PLATFORM_CHIP == INTEL) 
#define GPU_VR_HOT_ON()  SET_MASK(GPDRB,BIT(5))	
#define GPU_VR_HOT_OFF() CLEAR_MASK(GPDRB,BIT(5))
#endif

#if Support_GATEA20
#define GATEA20_ON()    SET_MASK(GPDRB,BIT(5))
#define GATEA20_OFF()   CLEAR_MASK(GPDRB,BIT(5))
#define GATEA20_HI()    SET_MASK(GPDRB,BIT(5))
#define GATEA20_LOW()   CLEAR_MASK(GPDRB,BIT(5))
#endif

#define KBRST_ON()      CLEAR_MASK(GPDRB,BIT(6))
#define KBRST_OFF()     SET_MASK(GPDRB,BIT(6))
//-----------------------------------------------------------------
// GPIO Port C Registers define
//-----------------------------------------------------------------
#define PortC_Init    0x28              // PORT Data

#define PortC0_Ctrl   OUTPUT            // 113  TP_RESET
#define PortC1_Ctrl   ALT               // 115  _EC_SMB_CK1
#define PortC2_Ctrl   ALT               // 116  _EC_SMB_DA1
#define PortC3_Ctrl   (ALT | PULL_UP)   // 56   _KSO16_SMOSI
#define PortC4_Ctrl   OUTPUT            // 120  OTP_RESET // 14" only, high active
#define PortC5_Ctrl   (ALT | PULL_UP)   // 57   _KSO17_SMISO
#define PortC6_Ctrl   OUTPUT            // 124  SUSP#
#define PortC7_Ctrl   INPUT             // 16   EC_RX
//-----------------------------------------------------------------
#if Support_TP_RESET_control
#define TP_RESET_ON()     SET_MASK(GPDRC,BIT(0))
#define TP_RESET_OFF()    CLEAR_MASK(GPDRC,BIT(0))
#endif //Support_TP_RESET_control

#define OTP_RESET_LOW()		CLEAR_MASK(GPDRC,BIT(4))
#define OTP_RESET_HI()    SET_MASK(GPDRC,BIT(4))
#define Read_OTP_RESET()  GET_MASK(GPDRC, BIT(4))

#define SUSP_OUTPUT       GPCRC6 = OUTPUT
#define SUSP_OFF()        CLEAR_MASK(GPDRC,BIT(6))
#define SUSP_ON()         SET_MASK(GPDRC,BIT(6))
#define Read_SUSP()       IS_MASK_SET(GPDRC,BIT(6))


#define EC_RX_ALT         GPCRC7 = ALT              // Set EC_RX Alternate.
#define EC_RX_OUTPUT      GPCRC7 = OUTPUT           // Set EC_RX OUTPUT.
#define EC_RX_INPUT       GPCRC7 = INPUT            // Set EC_RX INPUT.
#define DEBUG_RxCLK_HI()  SET_MASK(GPDRC,BIT(7))
#define DEBUG_RxCLK_LO()  CLEAR_MASK(GPDRC,BIT(7))
//-----------------------------------------------------------------
// GPIO Port D Registers define
//-----------------------------------------------------------------
#define PortD_Init    0x00              // PORT Data

#define PortD0_Ctrl   INPUT             // 18   PM_SLP_S3#
#define PortD1_Ctrl   INPUT             // 21   PM_SLP_S4#
#define PortD2_Ctrl   INPUT             // 22   PLT_RST#
#define PortD3_Ctrl   OUTPUT            // 23   EC_SCI#
#define PortD4_Ctrl   INPUT             // 15   //Ex85:SMI for BIOS; //PD_VBUS_C_CTRL1_EC //Need to add code to control 
#define PortD5_Ctrl   OUTPUT            // 33   USB_ON#
#define PortD6_Ctrl   (INPUT | PULL_DW) // 47   EC_FAN_SPEED
#define PortD7_Ctrl   OUTPUT            // 48   AOU_CTL3
//-----------------------------------------------------------------
#define Read_SLPS3()    IS_MASK_SET(GPDRD,BIT(0))

#if (PLATFORM_CHIP == INTEL)
#define Read_SLPS4()      IS_MASK_SET(GPDRD,BIT(1))
#endif // PLATFORM_CHIP

#define Read_LAN_WAKE_IN()  IS_MASK_CLEAR(GPDRD,BIT(1))

#if (PLATFORM_CHIP == INTEL)
#define Read_LPC_RST()  IS_MASK_SET(GPDRD,BIT(2))
#else
#define Read_LPC_RST()  IS_MASK_SET(GPDRD,BIT(2))
#endif // PLATFORM_CHIP

#define SCI_ON()        CLEAR_MASK(GPDRD,BIT(3))
#define SCI_OFF()       SET_MASK(GPDRD,BIT(3))
#define SCI_LOW()       CLEAR_MASK(GPDRD,BIT(3))
#define SCI_HI()        SET_MASK(GPDRD,BIT(3))

#define SMI_ON()        CLEAR_MASK(GPDRD,BIT(3))
#define SMI_OFF()       SET_MASK(GPDRD,BIT(3))

#define ChkHDD_Detect_SDV()     IS_MASK_CLEAR(GPDRD,BIT(4)) // Check HDD detect.

#if Support_TYPE_C 
#define PD_VBUS_CTRL1_EN()  SET_MASK(GPDRD,BIT(4))
#define PD_VBUS_CTRL1_DIS() CLEAR_MASK(GPDRD,BIT(4))
#endif //Support_TYPE_C

#define USB_ON_INPUT      GPCRD5 = INPUT            // SET USB_ON# INPUT.
#define USB_ON_OUTPUT     GPCRD5 = OUTPUT           // SET USB_ON# OUTPUT.
#define USB_ON_LOW()      CLEAR_MASK(GPDRD,BIT(5))    // Turn On USB_ON#.

#define FAN_SPEED_ALT     GPCRD6 = ALT                // Set FAN Speed Alt.
#define FAN_SPEED_INDW    GPCRD6 = (INPUT | PULL_DW)  // Set FAN Speed input+pull down.

#define AOU_CTL3_HI()     SET_MASK(GPDRD,BIT(7))    // Clr SDP mode.
#define AOU_CTL3_LOW()    CLEAR_MASK(GPDRD,BIT(7))    // Set SDP mode.
//-----------------------------------------------------------------
// GPIO Port E Registers define
//-----------------------------------------------------------------
#define PortE_Init    0x00              // PORT Data

#define PortE0_Ctrl   OUTPUT            // 19   //TPM_PLT_RST#   //FN_LED#
#define PortE1_Ctrl   INPUT            // 82    Ex85:0.9VALW_PWRGOOD //PCH_SYSPWROK  
#define PortE2_Ctrl   OUTPUT            // 83   EC_ON
#define PortE3_Ctrl   OUTPUT            // 84   AOU_CTL1
#define PortE4_Ctrl   OUTPUT            // 107  SYSON
#define PortE5_Ctrl   OUTPUT            // 35   CP_RESET# -> LID_CLOSE# 
#define PortE6_Ctrl   OUTPUT            // 17   EC_TX
#define PortE7_Ctrl   INPUT             // 20   KB_FN
//-----------------------------------------------------------------

#define TPM_PLT_RST_HI()   SET_MASK(GPDRE,BIT(0))
#define TPM_PLT_RST_LOW()  CLEAR_MASK(GPDRE,BIT(0))
#define Read_TPM_PLT_RST() IS_MASK_CLEAR(GPDRE,BIT(0))

#if Support_WLAN_WAKE
#define Read_WLAN_WAKE_IN() IS_MASK_CLEAR(GPDRE,BIT(0))
#endif //Support_WLAN_WAKE

#if Support_FnLKLED
#define FN_LED_ON()       CLEAR_MASK(GPDRE,BIT(0))
#define FN_LED_OFF()      SET_MASK(GPDRE,BIT(0))
#endif

#if Support_EC_GS_ON    
#define EC_GS_ON()        CLEAR_MASK(GPDRE,BIT(0))
#define EC_GS_OFF()       {SET_MASK(GPDRE,BIT(0));  CLEAR_MASK(GS.Status, GS_Init);}
#else
#define EC_GS_OFF()       CLEAR_MASK(GS.Status, GS_Init)
#endif //Support_EC_GS_ON 

#if (PLATFORM_CHIP == INTEL)
#define PCH_SYSPWROK_HI()   SET_MASK(GPDRE,BIT(1))
#define PCH_SYSPWROK_LOW()  CLEAR_MASK(GPDRE,BIT(1))
#endif // PLATFORM_CHIP

#define Read_0P9VALW_PWRGD() IS_MASK_SET(GPDRE,BIT(1))
#define Read_VDDAPWROK()  TRUE

#if NV_GC6
#define FB_CLAMP_ON()     SET_MASK(GPDRE,BIT(1))
#define FB_CLAMP_OFF()    CLEAR_MASK(GPDRE,BIT(1))
#define Read_FB_CLAMP()   IS_MASK_SET(GPDRE,BIT(1))

#define Read_GC6Event_ON() IS_MASK_CLEAR(GPDRE,BIT(2))
#endif // NV_GC6

#if (PLATFORM_CHIP == INTEL)
#define EC_ON_HI()        {SET_MASK(GPDRE,BIT(2)); EC_ON2_Cnt = 10;}
#define EC_ON_LOW()       CLEAR_MASK(GPDRE,BIT(2))
#define Read_EC_ON()      IS_MASK_SET(GPDRE,BIT(2))
#endif

#define EC_ON_5V_HI()        SET_MASK(GPDRE,BIT(2));
#define EC_ON_5V_LOW()       CLEAR_MASK(GPDRE,BIT(2))
#define Read_EC_ON_5V()      IS_MASK_SET(GPDRE,BIT(2))

#define EC_ON_HI()           EC_ON_5V_HI()
#define EC_ON_LOW()          EC_ON_5V_LOW()  
#define Read_EC_ON()         Read_EC_ON_5V() 

#define AOU_CTL1_HI()     SET_MASK(GPDRE,BIT(3))    // Clr SDP mode.
#define AOU_CTL1_LOW()    CLEAR_MASK(GPDRE,BIT(3))    // Set SDP mode.

#define SYSON_OUTPUT      GPCRE4 = OUTPUT
#define SYSON_LOW()       CLEAR_MASK(GPDRE,BIT(4))
#define SYSON_HI()        SET_MASK(GPDRE,BIT(4))

#if Support_Chk_mSATA
#define ChkmSata_Detect()     IS_MASK_CLEAR(GPDRE,BIT(5)) // Check mSATA detect.
#endif //Support_Chk_mSATA

#if Support_CP_RESET_control
#define CP_RESET_ON()     CLEAR_MASK(GPDRE,BIT(5))
#define CP_RESET_OFF()    SET_MASK(GPDRE,BIT(5))
#endif //Support_CP_RESET_control

#define EC_TX_ALT         GPCRE6 = ALT              // Set EC_TX Alternate.
#define EC_TX_OUTPUT      GPCRE6 = OUTPUT           // Set EC_TX OUTPUT.
#define EC_TX_INPUT       GPCRE6 = INPUT            // Set EC_RX INPUT.
#define DEBUG_TxDAT_HI()  SET_MASK(GPDRE,BIT(6))
#define DEBUG_TxDAT_LO()  CLEAR_MASK(GPDRE,BIT(6))
#define Chk_DEBUG_IN()    (IS_MASK_SET(GPDRE,BIT(6)) && Read_SUSP())

#define EC_KBFNON()         IS_MASK_CLEAR(GPDRE,BIT(7))
//-----------------------------------------------------------------
// GPIO Port F Registers define
//-----------------------------------------------------------------
#define PortF_Init    0x02              // PORT Data

#define PortF0_Ctrl   OUTPUT            // 85   AOU_EN
#define PortF1_Ctrl   OUTPUT            // 86   PBTN_OUT#
#define PortF2_Ctrl   ALT				// 88   EC_SMB_CK2
#define PortF3_Ctrl   ALT				// 88   EC_SMB_DA2
#define PortF4_Ctrl   (INPUT | PULL_DW) // 89   CP_CLK
#define PortF5_Ctrl   (INPUT | PULL_DW) // 90   CP_DATA
#define PortF6_Ctrl   ALT             // 117  SMbus for PMIC //PECI
#define PortF7_Ctrl   ALT             // 118  SMBus for PMIC //LAN_WAKE#
//-----------------------------------------------------------------
#define AOU_EN_OFF()      CLEAR_MASK(GPDRF,BIT(0))
#define AOU_EN_ON()       SET_MASK(GPDRF,BIT(0))
#define Read_AOU_EN()     IS_MASK_SET(GPDRF,BIT(0))

#define PM_PWRBTN_LOW()   CLEAR_MASK(GPDRF,BIT(1))
#define PM_PWRBTN_HI()    SET_MASK(GPDRF,BIT(1))
#define Read_PM_PWRBTN()  IS_MASK_SET(GPDRF,BIT(1))

#if ((PLATFORM_CHIP == INTEL)&&(SUPPORT_DSx))
#define Read_SUSWARN()    //IS_MASK_SET(GPDRJ,BIT(2))
#define Read_SUSACK()     IS_MASK_SET(GPDRF,BIT(3))
#else
#define Read_SUSPWRDNACK()  //  IS_MASK_SET(GPDRJ,BIT(2))
#endif // PLATFORM_CHIP

#if ((PLATFORM_CHIP == INTEL)&&(SUPPORT_DSx))
#define SUSACK_LOW()        //CLEAR_MASK(GPDRF,BIT(3))
#define SUSACK_HI()         //SET_MASK(GPDRF,BIT(3))
#endif // PLATFORM_CHIP

#define TP_CLK_INDW       GPCRF4 = (INPUT | PULL_DW)  // Set TP CLK Input+Pull down.
#define TP_CLK_ALT        GPCRF4 = ALT                // Set TP CLK Alternate.

#define TP_DATA_INDW      GPCRF5 = (INPUT | PULL_DW)  // Set TP DATA Input+Pull down.
#define TP_DATA_ALT       GPCRF5 = ALT                // Set TP DATA Alternate.

#if (PLATFORM_CHIP == AMD)
//#define Read_VDDAPWROK()  IS_MASK_SET(GPDRF,BIT(6))
#endif // PLATFORM_CHIP

#if (PLATFORM_CHIP == INTEL)
#define PECI_INDW         GPCRF6 = INPUT              // Set PECI INPUT.
#define PECI_ALT          GPCRF6 = ALT                // Set PECI Alternate.

#define Read_LAN_WAKE_IN()  IS_MASK_CLEAR(GPDRF,BIT(7))
#endif // PLATFORM_CHIP

//-----------------------------------------------------------------
// GPIO Port G Registers define
//-----------------------------------------------------------------
#define PortG_Init    0x00              // PORT Data

#define PortG0_Ctrl   INPUT             // 125  
#define PortG1_Ctrl   INPUT             // 122  GSENSE_INT
#if !Support_Mirror_Code
#define PortG2_Ctrl   OUTPUT            // 100  EC_MUTE#
#else
#define PortG2_Ctrl   INPUT            // 100   For Mirror //EC_MUTE#
#endif

#define PortG3_Ctrl   INPUT             // 101  SPI_FSCE#
#define PortG4_Ctrl   INPUT             // 102  SPI_FMOSI#
#define PortG5_Ctrl   INPUT             // 103  SPI_FMISO#
#if (PLATFORM_CHIP == INTEL)
#define PortG6_Ctrl   OUTPUT            // 104  GND
#else
#define PortG6_Ctrl   INPUT            // 104   PCH_ENBKL
#endif
#define PortG7_Ctrl   INPUT             // 105  SPI_FSCK
//-----------------------------------------------------------------

#if (PLATFORM_CHIP == INTEL)
#define pchME_FLAS_INDW   GPCRG6 = (INPUT | PULL_DW)    // SET ME Input+Pull down.
#define pchME_FLAS_OUT    GPCRG6 = OUTPUT               // SET ME Output.
#endif // PLATFORM_CHIP

#if (PLATFORM_CHIP == INTEL)
#define BKOFF_OFF()       CLEAR_MASK(GPDRG,BIT(6))
#define BKOFF_ON()        SET_MASK(GPDRG,BIT(6))
#define Read_BKOFF()      GET_MASK(GPDRG, BIT(6))
#else
#define Read_ENBKL_IN()   IS_MASK_SET(GPDRG,BIT(6))
#endif


//-----------------------------------------------------------------
// GPIO Port H Registers define
//-----------------------------------------------------------------
#define PortH_Init    0x00              // PORT Data

#define PortH0_Ctrl   OUTPUT            // 93   EC_RSMRST#
#define PortH1_Ctrl   (INPUT | PULL_DW) // 94   EC_SMB_CK3
#define PortH2_Ctrl   (INPUT | PULL_DW) // 95   EC_SMB_DA3
#define PortH3_Ctrl   OUTPUT            // 96   BATT_CHG_LED#
#define PortH4_Ctrl   OUTPUT            // 97   PWR_STATUS_LED#
#define PortH5_Ctrl   OUTPUT            // 98   ACOFF
#define PortH6_Ctrl   OUTPUT            // 99   PCH_PWROK
#if (PLATFORM_CHIP == INTEL)
#define PortH7_Ctrl   OUTPUT            // 3    ME_FLASH
#else
#define PortH7_Ctrl   INPUT             // 3    //Ex85:PLT_RST //ME_FLASH
#endif
//-----------------------------------------------------------------
#define RSMRST_LOW()      CLEAR_MASK(GPDRH,BIT(0))
#define RSMRST_HI()       SET_MASK(GPDRH,BIT(0))
#define Read_RSMRST()     IS_MASK_SET(GPDRH,BIT(0))

#define SMBus3_CLK_INDW   GPCRH1 = (INPUT | PULL_DW)  // Set SMBus3 CLK input+pull down.
#define SMBus3_CLK_ALT    GPCRH1 = ALT                // Set SMBus3 CLK Alternate.
#define SMBus3_DAT_INDW   GPCRH2 = (INPUT | PULL_DW)  // Set SMBus3 DATA input+pull down.
#define SMBus3_DAT_ALT    GPCRH2 = ALT                // Set SMBus3 DATA Alternate.

#if (PLATFORM_CHIP == INTEL)
//#if (SUPPORT_DSx)
#define DPWROK_EC_LOW()   CLEAR_MASK(GPDRH,BIT(3))
#define DPWROK_EC_HI()    SET_MASK(GPDRH,BIT(3))
#define Read_DPWROK()     IS_MASK_SET(GPDRH,BIT(3))
//#endif
//#define PCH_APWROK_ON()   SET_MASK(GPDRF,BIT(7))
//#define PCH_APWROK_OFF()  CLEAR_MASK(GPDRF,BIT(7))
#endif

#if Support_BATTCHGLED  
#define BATTCHG_LED_OUTPUT      GPCRH3 = OUTPUT
#define BATTCHG_LED_INPUT       GPCRH3 = INPUT        
//#define BATTCHG_LED_ON()     CLEAR_MASK(GPDRH,BIT(3))
//#define BATTCHG_LED_OFF()    SET_MASK(GPDRH,BIT(3))
#define BATTCHG_LED_ON()     SET_MASK(GPDRH,BIT(3))
#define BATTCHG_LED_OFF()    CLEAR_MASK(GPDRH,BIT(3))
#endif

#if Support_PWRBTN_LED        
//#define PWRSTATUS_LED_ON()   CLEAR_MASK(GPDRH,BIT(4))
//#define PWRSTATUS_LED_OFF()  SET_MASK(GPDRH,BIT(4))
#define PWRSTATUS_LED_ON()   SET_MASK(GPDRH,BIT(4))
#define PWRSTATUS_LED_OFF()  CLEAR_MASK(GPDRH,BIT(4))
#endif

#if (PLATFORM_CHIP != INTEL)
//#define APUPWR_EN_OFF()   CLEAR_MASK(GPDRH,BIT(4))
//#define APUPWR_EN_ON()    SET_MASK(GPDRH,BIT(4))
//#define Read_APUPWR_EN()  IS_MASK_SET(GPDRH,BIT(4))
#endif

#define ACOFF_LOW()       CLEAR_MASK(GPDRH,BIT(5))
#define ACOFF_HI()        SET_MASK(GPDRH,BIT(5))
#define Read_ACOFF()      (IS_MASK_SET(GPDRH,BIT(5)) || IS_MASK_SET(CHGIC_ReadCmd0x12L,BATLearn))

#if (PLATFORM_CHIP == INTEL)
#define PCH_PWROK_LOW()      CLEAR_MASK(GPDRH,BIT(6))
#define PCH_PWROK_HI()       SET_MASK(GPDRH,BIT(6))
#define Read_PCH_PWROK()     IS_MASK_SET(GPDRH,BIT(6))
#else // PLATFORM_CHIP
#define PCH_PWR_GOOD_LOW()   CLEAR_MASK(GPDRH,BIT(6))
#define PCH_PWR_GOOD_HI()    SET_MASK(GPDRH,BIT(6))
#define Read_PCH_PWR_GOOD()  IS_MASK_SET(GPDRH,BIT(6))
#endif // PLATFORM_CHIP

#if (PLATFORM_CHIP == INTEL)
#define pchME_FLAS_HI()   SET_MASK(GPDRH,BIT(7))        // ME Lock
#define pchME_FLAS_LOW()  CLEAR_MASK(GPDRH,BIT(7))      // ME unlock             // SET ME Output.
#else
#define Read_PLT_RST()  IS_MASK_SET(GPDRH,BIT(7))
#endif // PLATFORM_CHIP
//-----------------------------------------------------------------
// GPIO Port I Registers define
//-----------------------------------------------------------------
#define PortI_Init    0x00              // PORT Data

#define PortI0_Ctrl   ALT               // 66   TEMBER_DETECT# 
#define PortI1_Ctrl   OUTPUT            // 67   CP_BYPASS -> PAD_DISABLE#
#define PortI2_Ctrl   ALT               // 68   _ADC2 for BATT_TEMP
#define PortI3_Ctrl   OUTPUT            // 69   Ex85:EC_ON3_VDDP //KB_BLK_DTCT#
#define PortI4_Ctrl   ALT               // 70   FAN_ID
#define PortI5_Ctrl   ALT               // 71   ADP_I
#define PortI6_Ctrl   ALT                // 72   PWR_SYS
#define PortI7_Ctrl   ALT               // 73   
//-----------------------------------------------------------------
#if Support_KBD_BL_DETECT
#define Read_KBD_BL_DTCT()     IS_MASK_SET(GPDRI,BIT(0))
#endif
#if Support_BYPASS_PAD_control
#define BYPASS_PAD_ON()   SET_MASK(GPDRI,BIT(1))
#define BYPASS_PAD_OFF()  CLEAR_MASK(GPDRI,BIT(1))
#endif //Support_BYPASS_PAD_control

//#define Read_BAT_IN()     //IS_MASK_SET(GPDRI,BIT(2))
//#define Read_BOARD_IN()    // IS_MASK_SET(GPDRI,BIT(3))

//#define Read_KB_BLK_DTCT()     IS_MASK_SET(GPDRI,BIT(3))

#define EC_ON3_VDDP_HI()   SET_MASK(GPDRI,BIT(3))
#define EC_ON3_VDDP_LOW()  CLEAR_MASK(GPDRI,BIT(3))

#if Support_MuteLED
#define Mute_LED_ON()     CLEAR_MASK(GPDRI,BIT(6))
#define Mute_LED_OFF()    SET_MASK(GPDRI,BIT(6))
#endif

//#define VGA_AC_BATT_ON()  SET_MASK(GPDRI,BIT(6))	//Rename to VGA_PWR_LIM
//#define VGA_AC_BATT_OFF() CLEAR_MASK(GPDRI,BIT(6))	//Rename to VGA_PWR_LIM

//-----------------------------------------------------------------
// GPIO Port J Registers define
//-----------------------f------------------------------------------
#define PortJ_Init    0x00              // PORT Data

#define PortJ0_Ctrl   INPUT             // 76   INT#_TYPEC
#define PortJ1_Ctrl   INPUT             // 77   PM_SLP_S5#
#define PortJ2_Ctrl   INPUT             // 78   Ex85:VR_APU_PWRGD  //VGATE
#define PortJ3_Ctrl   OUTPUT            // 79   Ex85:CAPSLK_LED# //MAINPWON
#define PortJ4_Ctrl   OUTPUT            // 80   H_PROCHOT_EC
#if (PLATFORM_CHIP == INTEL)
#define PortJ5_Ctrl   INPUT             // 81   PCH_ENBKL
#else
#define PortJ5_Ctrl   OUTPUT             // 81   BK_OFF
#endif

#define PortJ6_Ctrl   OUTPUT            // 128  AC_PRESENT_R
#define PortJ7_Ctrl   OUTPUT            // 2    EC_WAKE#
//[-end-130918-T130124_001-modify]//
//-----------------------------------------------------------------
#if Support_TYPE_C 
//#define INT_TYPEC_EN()      SET_MASK(GPDRJ,BIT(0))
//#define INT_TYPEC_DIS()     CLEAR_MASK(GPDRJ,BIT(0))
#define Read_TYPEC_INT()      IS_MASK_CLEAR(GPDRJ,BIT(0))	//Low active, means event occur
#endif //Support_TYPE_C

#define Read_SLPS5()      IS_MASK_SET(GPDRJ,BIT(1))
#if (PLATFORM_CHIP == AMD)
#define Read_SLPS4()      IS_MASK_SET(GPDRJ,BIT(1))
#endif 

#if (PLATFORM_CHIP == AMD)
//#define Read_IMVPPOK()    IS_MASK_SET(GPDRJ,BIT(2))
#endif // PLATFORM_CHIP

//#define Read_VGATE()    IS_MASK_SET(GPDRJ,BIT(2))
#define Read_APU_PWRGD()    IS_MASK_SET(GPDRJ,BIT(2))

#if Support_Microphone_Mute_LED
#define MICMute_LED_ON()  CLEAR_MASK(GPDRJ,BIT(3))
#define MICMute_LED_OFF() SET_MASK(GPDRJ,BIT(3))
#endif

#if Support_CAPSLOCKLED
#define CAPS_LED_ON()       CLEAR_MASK(GPDRJ,BIT(3))
#define CAPS_LED_OFF()      SET_MASK(GPDRJ,BIT(3))
#endif

#if (PLATFORM_CHIP == INTEL)
#define EC_MainPwr_ON()   SET_MASK(GPDRJ,BIT(3))
#define EC_MainPwr_OFF()  CLEAR_MASK(GPDRJ,BIT(3))
#define EC_MainPwr()      IS_MASK_SET(GPDRJ,BIT(3))
#endif

//#define H_PROCHOT_LOW()   CLEAR_MASK(GPDRJ,BIT(4))
//#define H_PROCHOT_HI()    SET_MASK(GPDRJ,BIT(4))
#define H_PROCHOT_LOW()   SET_MASK(GPDRJ,BIT(4))
#define H_PROCHOT_HI()    CLEAR_MASK(GPDRJ,BIT(4))

#if (PLATFORM_CHIP == INTEL)
#define Read_ENBKL_IN()   IS_MASK_SET(GPDRJ,BIT(5))
#else
#define BKOFF_OFF()       CLEAR_MASK(GPDRJ,BIT(5))
#define BKOFF_ON()        SET_MASK(GPDRJ,BIT(5))
#define Read_BKOFF()      GET_MASK(GPDRJ,BIT(5))
#endif

#define AC_PRESENT_LOW()  CLEAR_MASK(GPDRJ,BIT(6))
#define AC_PRESENT_HI()   SET_MASK(GPDRJ,BIT(6))

#define EC_WAKE_OUT_LOW()  CLEAR_MASK(GPDRJ,BIT(7)) // This is EC_WAKE#, just for LID out event use only.
#define EC_WAKE_OUT_HI()   SET_MASK(GPDRJ,BIT(7))
//-----------------------------------------------------------------
// Others:
//-----------------------f------------------------------------------
#if (PLATFORM_CHIP == INTEL)
//#define Read_VDDQ_PGOOD() IS_MASK_SET(GPDRI,BIT(7))
#endif // PLATFORM_CHIP

//#define Read_SUSP()       IS_MASK_SET(GPDRC,BIT(6))

//-----------------------------------------------------------------------------
// Output control
//-----------------------------------------------------------------------------
#define GS_ST_HI()        //SET_MASK(GPDRA,BIT(4))  // GS_SELFTEST
#define GS_ST_LOW()       //CLEAR_MASK(GPDRA,BIT(4))

#if SW_ISCT
#define AOAC_PWRON()      //SET_MASK(GPDRB,BIT(2))
#define AOAC_PWROFF()     //CLEAR_MASK(GPDRB,BIT(2))
//#define READ_AOAC_PWR()   IS_MASK_SET(GPDRB,BIT(2))
#endif // SW_ISCT

//#define VGA_AC_DET_HI() SET_MASK(GPDRA,BIT(7))
//#define VGA_AC_DET_LO()   CLEAR_MASK(GPDRA,BIT(7))

//#define LAN_PWR_ON_HI()   SET_MASK(GPDRF,BIT(7))
//#define LAN_PWR_ON_LO()   CLEAR_MASK(GPDRF,BIT(7))


//#define CMOS_Cam_ON()   CLEAR_MASK(GPDRG,BIT(7))// Camera ON.
//#define CMOS_Cam_OFF()  SET_MASK(GPDRG,BIT(7))  // Camera OFF.

#if Support_WLAN_ON_OFF_control
#define BTRF_OFF_ON()
#define BTRF_OFF_OFF()
#endif //Support_WLAN_ON_OFF_control

#if Support_BT_ON_OFF_control
#define BT_OFF_ON()
#define BT_OFF_OFF()
#endif //Support_BT_ON_OFF_control

#define BAT_LEARN_ON()	{                    \
          SET_MASK(CHGIC_ReadCmd0x12L,BATLearn);    \
          CHGIC_ptr = 2;                \
          WriteSmartChgIC();            \
          }

#define BAT_LEARN_OFF()	{                    \
          CLEAR_MASK(CHGIC_ReadCmd0x12L,BATLearn);    \
          CHGIC_ptr = 2;                \
          WriteSmartChgIC();            \
          }

#define Read_BATLEARN()   IS_MASK_SET(CHGIC_ReadCmd0x12L,BATLearn)

#define TEMBER_DETECT()    // IS_MASK_CLEAR(GPDRI,BIT(0)) // Check HDD/SSD detect.

//#define CHK_GPUVRHOT()      IS_MASK_CLEAR(GPDRI,BIT(0))
#define ADPID_ON_EN()       //CLEAR_MASK(GPDRF,BIT(7))
#define ADPID_ON_DIS()      //SET_MASK(GPDRF,BIT(7))
#define ADPID_ON_Detect()	1//IS_MASK_CLEAR(GPDRF,BIT(7))
//-----------------------------------------------------------------------------
#define SERIRQ_OUTPUT      GPCRM6 = INPUT
#define SERIRQ_ALT      GPCRM6 = ALT

//-----------------------------------------------------------------------------
// DPGU
//-----------------------------------------------------------------------------
#if NV_GC6
#define EG_FB_CLAMP         0x10
#define EG_GC6_EVENT        0x14
#define EG_DGPU_PWROK       0x1C
#define EG_DGPU_PWR_EN      0x24
#define EG_DGPU_HOLD_RST    0x2C
#define EG_DGPU_GC6_EN      0x34
#define EG_NVDD_PWR_EN      0x48
#define EG_KBL_DET          0x50

#define EG_GPIO22           0x58
#define EG_S_NVDD_PWR_EN    0x60
#define EG_GPIO26           0x68
#define EG_S_DGPU_RST       0x6C
#define EG_S_DGPU_PWR_EN    0x74
#define EG_S_DGPU_PWROK     0x7C
#define EG_S_GC6_EVENT      0x84
#define EG_S_GC6_EN         0x8C

#define DGPU_PWR_EN_HI()    EGPC_Write(EG_DGPU_PWR_EN,  0x03)
#define DGPU_PWR_EN_LOW()   EGPC_Write(EG_DGPU_PWR_EN,  0x02)

#define DGPU_HOLD_RST_HI()  EGPC_Write(EG_DGPU_HOLD_RST,0x03)
#define DGPU_HOLD_RST_LOW() EGPC_Write(EG_DGPU_HOLD_RST,0x02)

#define DGPU_GC6_EN_HI()    EGPC_Write(EG_DGPU_GC6_EN,  0x03)
#define DGPU_GC6_EN_LOW()   EGPC_Write(EG_DGPU_GC6_EN,  0x02)

#define NVDD_PWR_EN_HI()    EGPC_Write(EG_NVDD_PWR_EN,  0x03)
#define NVDD_PWR_EN_LOW()   EGPC_Write(EG_NVDD_PWR_EN,  0x02)

#define S_NVDD_PWR_EN_HI()  EGPC_Write(EG_S_NVDD_PWR_EN,0x03)
#define S_NVDD_PWR_EN_LOW() EGPC_Write(EG_S_NVDD_PWR_EN,0x02)

#define S_DGPU_RST_HI()     EGPC_Write(EG_S_DGPU_RST,   0x03)
#define S_DGPU_RST_LOW()    EGPC_Write(EG_S_DGPU_RST,   0x02)

#define S_DGPU_PWR_EN_HI()  EGPC_Write(EG_S_DGPU_PWR_EN,0x03)
#define S_DGPU_PWR_EN_LOW() EGPC_Write(EG_S_DGPU_PWR_EN,0x02)

#define DGPU_FB_CLAMP_HI()  EGPC_Write(EG_S_DGPU_PWR_EN,0x03)
#define DGPU_FB_CLAMP_LOW() EGPC_Write(EG_S_DGPU_PWR_EN,0x02)

#define IS_DGPU_PWROK()     EGPC_Read(EG_DGPU_PWROK)
#define IS_S_DGPU_PWROK()   EGPC_Read(EG_S_DGPU_PWROK)
#endif //NV_GC6
//-----------------------------------------------------------------------------
// Input control
//-----------------------------------------------------------------------------
extern void Init_GPIO(void);
extern void Hook_SCION(void);
extern void Hook_SCIOFF(void);
extern void Hook_SMION(void);
extern void Hook_SMIOFF(void);
extern void Hook_A20ON(void);
extern void Hook_A20OFF(void);
extern void Hook_KBRSTON(void);
extern void Hook_KBRSTOFF(void);
extern void Hook_NUMLED_ON(void);
extern void Hook_NUMLED_OFF(void);
extern void Hook_CAPLED_ON(void);
extern void Hook_CAPLED_OFF(void);

typedef struct InitGPIOReg
  {
    XBYTE   *reg;
    BYTE    value;
  } sInitGPIOReg;

#endif
