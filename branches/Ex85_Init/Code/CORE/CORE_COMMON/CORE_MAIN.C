/*-----------------------------------------------------------------------------
 * TITLE: CORE_MAIN.C - Main Program for KBC firmware.
 *
 * Main processing loop (interrupt driven) for the keyboard controller/scanner.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

/* ----------------------------------------------------------------------------
 *  FUNCTION: main - Main service loop.
 *
 *  Wait in idle state until an IRQ causes an exit from idle.  If the IRQ
 *  handler posted a service request (via bSERVICE) then dispatch control to
 *  the appropriate service handler.  Otherwise, go back to idle state.  After
 *  all service requests have been handled, return to idle state.
 * ------------------------------------------------------------------------- */
void main(void)
{
  DisableAllInterrupt();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  //
  // Dynamic Caches Disabled (DYNDIS)
  // 00b: Dynamic Caches Enable.
  // 11b: Dynamic Caches Disable.
  //
  DCache = 0x03;

  //
  // Setting stack pointer
  //
  SP     = 0xC0;

  CLEAR_MASK(RAM_16FF,BIT0);  // for AX
  SCRA2H = 3;             // Disable SCRA2H

  //
  // Exit from follow mode or EC scatch ROM
  //
  if(Hook_ECRetunrMainFuncKeepCondition()==0x33) {
    //
    // disable scatch ROM
    //
    CLEAR_MASK(FBCFG,SSMC);

    _nop_();
    //
    // Clearing Dynamic Caches (Ref to spec 7.1.10.5)
    //
    MPRECF = 0x01;
    _nop_();
    MPRECF = 0x01;
    _nop_();
    MPRECF = 0x01;
    _nop_();
    MPRECF = 0x01;
    _nop_();

    _nop_();
    WinFlashMark = exitFollowMode;	
    WinFlashMark2 = 0x00;
    ResetBANKDATA();    // init bank mechanism to code bank 0
    Hook_ECExitFollowMode();
    Init_Timers();
    EnableModuleInterrupt();
#if chPWSW1WDT
  	OEM_PWRSWWDTEn();		
#endif  //chPWSW1WDT				
  } else {
    ECSleepCount = 3;
	if(RSTStatus & 0x02)
	{
		pLastSID  = WDT_WARMBOOT_ID;
	}
	else{
    	pLastSID  = COLDBOOT_ID;
	}
    Core_Init_Regs();
    if( CheckCrisisMode() )
    { SET_MASK(SYS_MISC1,Crisis_On); }
    else
    { CLEAR_MASK(SYS_MISC1,Crisis_On); }
#if Support_Mirror_Code
#if Support_Mirror_Always_EN
       Check_Mirror_Occurs(); 
       if(Read_Eflash_Byte(0x00, 0x00, 0x4D) != 00)
       {
           Do_Eflash_Write_1Byte(0x00, 0x00, 0x00, 0x4D);         
   			   RamDebug(0x0D);
	         FLHCTRL3R |= 0x80;	
	         HINSTC1 |= 0x40;                           
       }
#else
    if (Check_Mirror_Occurs())
    {
      Do_Eflash_Write_1Byte(0x00, 0x00, 0x00, 0x4D);
      SET_MASK(FLHCTRL3R,MIRRORMIS);
      SET_MASK(FLHCTRL3R,FFSPITRI);
      SET_MASK(HINSTC1,SCECB);
    }
#endif 
#endif

    Core_Initialization();
    Oem_Initialization();
    InitEnableInterrupt();
#ifdef UART_Debug
    Init_UART();
    printf("\n************************************");
    printf("\nEC Init OK !!!");
    printf("\n************************************\n\r");
#endif
  }
#if WDT_Support
  EnableInternalWDT();
#endif
  while(1)
     {
#if chPWSW1WDT
		  	if (IS_MASK_SET(pProject4,pPWSWdisable)) 
	     	{
		  		OEM_RESETPWSWTIMER1();
		 	}
#endif  //chPWSW1WDT				
        if(OEM_SkipMainServiceFunc()==Normal_MainService)
        {
        main_service();
        EnableModuleInterrupt();
        _nop_();
        _nop_();
        _nop_();
        _nop_();

            if(PS2CheckPendingISR()==0x00)
            {
                ScanAUXDeviceStep();
            }

            #ifdef SMBusServiceCenterFunc
        if((Service==0x00)&&(Service1==0x00)&&(CheckSMBusNeedService()==SMBus_ClearService))
            #else
            if((Service==0x00)&&(Service1==0x00))
            #endif
            {
                if (TH0< Timer_1ms>>8)
                    TH0 = Timer_1ms>>8;

#if Support_EC_Sleep
        if( CheckCanEnterDeepSleep() )
              {
          InitEnterDeepSleep();
                  EnableAllInterrupt();   // enable global interrupt
                  PLLCTRL = 0x01;
                  Sleep_Now();
                  IER3 = 0x00;
                  InitWakeFromDeepSleep();
                  WakeUp_DO_Function();
              }
        else
          {
               PCON=1;  // Enter idle mode
          }
#else
        PCON=1;  // Enter idle mode
#endif  // Support_EC_Sleep
            }
        }
    }
}
void Sleep_Now(void)
{
    PCON    = 0x02;         /* Enter Deep Sleep mode.  */
    _nop_();                /* Wake-up delay.  */
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: main_service - Check for new/more service requests.
 *
 * Check for a request flag.  The check is done according to priority.  If a
 * request flag is set, handle the request and loop back to get the flags
 * again.  Do not follow through to check the next flag.  The flags are to be
 * checked in order.
 * ------------------------------------------------------------------------- */
void main_service(void)
{
    #ifdef SMBusServiceCenterFunc
    while((Service!=0x00)||(Service1!=0x00)||(CheckSMBusNeedService()==SMBus_NeedService))
    #else
    while((Service!=0x00)||(Service1!=0x00))
    #endif
    {
        //-----------------------------------
        // Host command/data service
        //-----------------------------------
        if(F_Service_PCI)
        {
            F_Service_PCI=0;
            service_pci1();
            continue;
        }

        //-----------------------------------
        // Service unlock
        //-----------------------------------
        if(F_Service_UNLOCK)
        {
            F_Service_UNLOCK=0;
            service_unlock();
            continue;
        }

        //-----------------------------------
        // Send byte from KBC
        //-----------------------------------
        if(F_Service_SEND)
        {
            F_Service_SEND=0;
            service_send();
            continue;
        }

        //-----------------------------------
        // Send PS2 interface data
        //-----------------------------------
        if(F_Service_Send_PS2)
        {
            F_Service_Send_PS2=0;
            service_PS2_data();
            continue;
        }

        //-----------------------------------
        // process PS2 interface data
        //-----------------------------------
        if(F_Service_PS2)
        {
            F_Service_PS2=0;
            service_ps2();
            continue;
        }

        //-----------------------------------
        // process SMBus interface data
        //-----------------------------------
        #ifdef SMBusServiceCenterFunc
        ServiceSMBus();
        #endif

    //-----------------------------------
    // Secondary Host command/data service
    //-----------------------------------
    if(F_Service_PCI2)
    {
       F_Service_PCI2=0;
       service_pci2();
       continue;
    }

        //-----------------------------------
        // 1 millisecond elapsed
        //-----------------------------------
        if(F_Service_MS_1)
        {
            F_Service_MS_1=0;
            service_1mS();
            continue;
        }

        //-----------------------------------
        // Keyboard scanner service
        //-----------------------------------
        if(F_Service_KEY)
        {
            F_Service_KEY=0;
            service_scan();
            continue;
        }

        //-----------------------------------
        //
        //-----------------------------------
        Hook_main_service_H();

        //-----------------------------------
        // Low level event
        //-----------------------------------
        if(F_Service_Low_LV)
        {
            F_Service_Low_LV=0;
            service_Low_LVEvent();
            continue;
        }

        //-----------------------------------
        // Third Host command/data service
        //-----------------------------------
        if(F_Service_PCI3)
        {
            F_Service_PCI3=0;
            service_pci3();
            continue;
        }

        //-----------------------------------
        // CIR IRQ
        //-----------------------------------
        if(F_Service_CIR)
        {
            F_Service_CIR=0;
            service_cir();
            continue;
        }

        //-----------------------------------
        // fourth command/data service
        //-----------------------------------
        if(F_Service_PCI4)
        {
            F_Service_PCI4=0;
            service_pci4();
            continue;
        }

        //------------------------------------
        // service_OEM_1
        //------------------------------------
        if(F_Service_OEM_1)
        {
            F_Service_OEM_1=0;
            service_OEM_1();
            continue;
        }

        //------------------------------------
        // service_OEM_2
        //------------------------------------
        if(F_Service_OEM_2)
        {
            F_Service_OEM_2=0;
            service_OEM_2();
            continue;
        }

        //------------------------------------
        // service_OEM_3
        //------------------------------------
        if(F_Service_OEM_3)
        {
            F_Service_OEM_3=0;
            service_OEM_3();
            continue;
        }

        //------------------------------------
        // service_OEM_4
        //------------------------------------
        if(F_Service_OEM_4)
        {
            F_Service_OEM_4=0;
            service_OEM_4();
            continue;
        }

        //-----------------------------------
        //
        //-----------------------------------
        Hook_main_service_L();
    }
}

static void service_Low_LVEvent(void)
{
  if((KBPendingTXCount != KBPendingRXCount )||(scan.kbf_head != scan.kbf_tail))
  {
    SetServiceSendFlag();
  }

    if(IS_MASK_SET(KBHISR,P_IBF))
    {
        F_Service_PCI = 1;
    }

    if(IS_MASK_SET(PM1STS,P_IBF))
    {
        F_Service_PCI2 = 1;
    }
}

//----------------------------------------------------------------------------
// FUNCTION: service_unlock
// Unlock aux devices and re-enable Host interface IRQ if it is ok to do so.
//----------------------------------------------------------------------------
static void service_unlock(void)
{
  Unlock_Scan();
}

//------------------------------------------------------------
// Polling events
//------------------------------------------------------------
void service_1mS(void)
{
  Timer1msEvent();
  Timer1msCnt++;
    if(Timer1msCnt>=10)
    {
        Timer1msCnt = 0x00;
    }

    if(Hook_Only_Timer1msEvent()==Only_Timer1msEvent)
    {
        return;
    }
#if Changing_timing_base
    if((Timer1msCnt%5)==0x04)
    {
      Timer5msEvent();	//5msEvent start timing shift to 4ms,That means working timing in 4ms,9ms,14ms,19ms...
    }
#endif //Changing_timing_base
    if((Timer1msCnt%5)==0x00)
    {
#if !Changing_timing_base	
      Timer5msEvent();
#endif //Changing_timing_base	  
      Timer5msCnt++;
      if ( Timer5msCnt & 1 )    // 10ms events
      {
            Timer10msEventA();
      }
      else
      {
        Timer10msEventB();
#if Changing_timing_base
           switch( Timer5msCnt )   // Share Loading Branch Control
          {
               case 2: //Timer50msEventA();
                    break;
                case 4: //Timer50msEventB();
                   break;
              case 6: //Timer50msEventC();
                    break;
                case 8: //Timer100msCntB++;
//                 if ( Timer100msCntB & 1 )
//                   {
//                        Timer100msEventA();
//                    }
//                   else
//                   {
//                         Timer100msEventB();
//                    }
                     break;

                 default:
            Timer5msCnt=0;
                    break;
           }

          if ( Timer5msCnt == 0x00 )
          {             // 50msec
                Timer100msCnt ++;
                if ( Timer100msCnt & 1 )
               {
                   ;
                }
               else
             {           // 100msec
                  switch( Timer100msCnt )
                    {
                      case 2:  //Timer500msEventA();
                         break;
                       case 4:  //Timer500msEventB();
                            break;
                       case 6:  //Timer500msEventC();
                           break;
                       case 8:  //Timer1SecEventA();
                           break;
              case 10:   //Timer1SecEventB();
                           break;
                       case 12:  //Timer500msEventA();
                            break;
                      case 14:  //Timer500msEventB();
                            break;
                       case 16:   //Timer500msEventC();
                            break;
                       case 18:   //Timer1SecEventC();
                           break;
                        default:        // 1 Sec
                            Timer100msCnt = 0;
                          Timer1SecCnt ++;
                          if ( Timer1SecCnt == 60 )
                            {
                               Timer1MinEvent();
                               Timer1SecCnt=0;
                           }
                          break;
                  }
                }
           }
       }
  }
    if((Timer1msCnt%5)==0x01)
    {
      if ( Timer5msCnt & 1 )    // 10ms events
      {
            ;
      }
      else
      {
#endif //Changing_timing_base 
           switch( Timer5msCnt )   // Share Loading Branch Control
          {
               case 2: Timer50msEventA();
                    break;
                case 4: Timer50msEventB();
                   break;
              case 6: Timer50msEventC();
                    break;
                case 8: Timer100msCntB++;
                 if ( Timer100msCntB & 1 )
                   {
                        Timer100msEventA();
                    }
                   else
                   {
                         Timer100msEventB();
                    }
                     break;
#if !Changing_timing_base
                 default:
            Timer5msCnt=0;
                    break;
#endif //Changing_timing_base
           }

          if ( Timer5msCnt == 0x00 )
          {             // 50msec
#if !Changing_timing_base		  
                Timer100msCnt ++;
#endif //Changing_timing_base
                if ( Timer100msCnt & 1 )
               {
                   Timer100msEventC();
                }
               else
             {           // 100msec
                  switch( Timer100msCnt )
                    {
                      case 2:  Timer500msEventA();
                         break;
                       case 4:  Timer500msEventB();
                            break;
                       case 6:  Timer500msEventC();
                           break;
                       case 8:  Timer1SecEventA();
                           break;
              case 10:   Timer1SecEventB();
                           break;
                       case 12:  Timer500msEventA();
                            break;
                      case 14:  Timer500msEventB();
                            break;
                       case 16:   Timer500msEventC();
                            break;
                       case 18:   Timer1SecEventC();
                           break;
#if !Changing_timing_base					   
                        default:        // 1 Sec
                            Timer100msCnt = 0;
                          Timer1SecCnt ++;
                          if ( Timer1SecCnt == 60 )
                            {
                               Timer1MinEvent();
                               Timer1SecCnt=0;
                           }
                          break;
#endif //Changing_timing_base
                  }
                }
           }
       }
  }
}

//------------------------------------------------------------
// 1ms events
//------------------------------------------------------------
void Timer1msEvent(void)
{
    ReSendPS2PendingData();
    Hook_Timer1msEvent(Timer1msCnt);
}

//------------------------------------------------------------
// 5ms events
//------------------------------------------------------------
void Timer5msEvent(void)
{
    F_Service_Low_LV = 1;
  if (Timer_A.fbit.TMR_SCAN)
  {
      F_Service_KEY = 1;    // Request scanner service.
    }
    Hook_Timer5msEvent();
}

//------------------------------------------------------------
// 10ms events
//------------------------------------------------------------
void Timer10msEventA(void)
{
    SetStartScanAUXFlag();
  Hook_Timer10msEventA();
}

//------------------------------------------------------------
// 10ms events
//------------------------------------------------------------
void Timer10msEventB(void)
{
  Hook_Timer10msEventB();
}

//------------------------------------------------------------
// 50ms events A
//------------------------------------------------------------
void Timer50msEventA(void)
{
  Hook_Timer50msEventA();
}

//------------------------------------------------------------
// 50ms events B
//------------------------------------------------------------
void Timer50msEventB(void)
{
  Hook_Timer50msEventB();
}

//------------------------------------------------------------
// 50ms events C
//------------------------------------------------------------
void Timer50msEventC(void)
{
  Hook_Timer50msEventC();
}

//------------------------------------------------------------
// 100ms events A
//------------------------------------------------------------
void Timer100msEventA(void)
{
  Hook_Timer100msEventA();
}

//------------------------------------------------------------
// 100ms events B
//------------------------------------------------------------
void Timer100msEventB(void)
{
  Hook_Timer100msEventB();
}

//------------------------------------------------------------
// 100ms events C
//------------------------------------------------------------
void Timer100msEventC(void)
{
  Hook_Timer100msEventC();
}

//------------------------------------------------------------
// 500ms events A
//------------------------------------------------------------
void Timer500msEventA(void)
{
  Hook_Timer500msEventA();
}

//------------------------------------------------------------
// 500ms events B
//------------------------------------------------------------
void Timer500msEventB(void)
{
  Hook_Timer500msEventB();
}

//------------------------------------------------------------
// 500ms events C
//------------------------------------------------------------
void Timer500msEventC(void)
{
  Hook_Timer500msEventC();
}

//------------------------------------------------------------
// 1sec events A
//------------------------------------------------------------
void Timer1SecEventA(void)
{
  Hook_Timer1SecEventA();
}

//------------------------------------------------------------
// 1sec events B
//------------------------------------------------------------
void Timer1SecEventB(void)
{
  Hook_Timer1SecEventB();
}

//------------------------------------------------------------
// 1sec events C
//------------------------------------------------------------
void Timer1SecEventC(void)
{
  Hook_Timer1SecEventC();
}

//------------------------------------------------------------
// 1min events
//------------------------------------------------------------
void Timer1MinEvent(void)
{
    Hook_Timer1MinEvent();
}

bit CheckCanEnterDeepSleep()
{
  //if(Read_AC_IN()) return FALSE;
  if ( IS_MASK_SET(SYS_STATUS,AC_ADP) )  { return FALSE; }

  //if( (IS_MASK_SET(cCmd, bPCHPWR_Keep)) || (IS_MASK_SET(cCmd, b1Chg_VGAMode)) )
  //{ return FALSE; }
if(IS_MASK_SET(LENOVOP2PSTATUS,LENOVOStatusFlag))
	{ return FALSE; }
    if ( AutoTimer != 0
    ||( pWorkAround > 0)
    )
  { return FALSE; }

  if(SystemIsS5)
  {
    if ( (ECSleepCount != 0)||(LOWBATT_3TIMES != 0) ) 
    {
    	Check_EC_ON();
    	return FALSE; 
    }
    if(IS_MASK_SET(LENOVOPMFW_Temp,b3EC_ONStatus))
    {
      CLEAR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
      return FALSE;
    }
    return TRUE;
  }

  return FALSE;
}

//-----------------------------------------------------------------------------
void InitEnterDeepSleep(void)
{
    //EC_DeepSleep_Temp0 = ADCSTS;
    //EC_DeepSleep_Temp1 = ADCCFG;
    //EC_DeepSleep_Temp2 = DACPWRDN;
    //EC_DeepSleep_Temp3 = CGCTRL1R;

    //EC_DeepSleep_TempPortA = GPDRA;
    //EC_DeepSleep_TempPortB = GPDRB;
    //EC_DeepSleep_TempPortC = GPDRC;
    //EC_DeepSleep_TempPortD = GPDRD;
    //EC_DeepSleep_TempPortE = GPDRE;
    //EC_DeepSleep_TempPortF = GPDRF;
    //EC_DeepSleep_TempPortG = GPDRG;
    //EC_DeepSleep_TempPortH = GPDRH;
    //EC_DeepSleep_TempPortI = GPDRI;
    //EC_DeepSleep_TempPortJ = GPDRJ;

    EA = 0;
  KSOL  = 0x00;
  KSOH1  = 0x00;
  KSOH2  = 0x00;
  KSICTRL = 0x00;

  SetACIN_Int();  // IER13 AC in bit2
  SetPWRSW_Int();  // IER1 power switch bit6
  SetPD_Int();

    //if(IS_MASK_SET(LENOVOPMFW_Temp,b3EC_ONStatus))
///    {
///  //  CLEAR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
///      SetExTimer1();
///    }
  if( Read_AOU_EN() )
  {
    SetExTimer1();
  }

  ADCSTS  = 0x00;
  ADCCFG  = 0x20;
  DACPWRDN = 0xFF;
  CGCTRL1R = 0x0F;
  
  #if (PLATFORM_CHIP == INTEL) 
  if (!Read_EC_ON())
  #endif // PLATFORM_CHIP  
  {
  PM_PWRBTN_LOW();
  }

  GPOTF = 0x00;
  GPCRA0 = INPUT; //Prevent PWM low pulse for issue "LOGO LED will flash in deepsleep mode".
#if Support_PWRBTN_LED      
  GPCRA3 = INPUT; //Prevent PWM low pulse for issue "PWR LED will flash in deepsleep mode".
#endif //Support_PWRBTN_LED  
//cwy
//    GPCRA1 = OUTPUT;
//    GPCRA2 = OUTPUT;
  #if 0
  GPCRJ3 = OUTPUT;    // CMW Temp
  GPCRJ4 = OUTPUT;    // CMW Temp

    GPDRA = 0x07;
    //GPDRB = 0x02;
    GPDRC = 0x3E;//0xBE;  // CMW Temp
    GPDRD = 0x21;
    //GPDRE = 0x41;
    GPDRF = 0x80; //0x01
    GPDRG = 0x00;
    //GPDRH = 0x09;  // CMW Temp
    //GPDRI = 0x94;  // CMW Temp

  if ( IS_MASK_SET(LENOVOPMFW_Temp,b5EC_ON_EN) )
  {
    GPDRB = 0x02;      // Turn off EC_ON.
    GPDRE = 0x00;      // Turn off charger IC.
    GPDRI |= 0xEF;      // Turn off EC_ON.
    GPDRJ = 0x00;      // Turn off mainpower pin.
    LID_readytime = 10 ;  // LID switch wakeup time = 100ms
    CLEAR_MASK(POWER_FLAG1,wait_PSW_off);
  }
  else
  {
    GPDRB = 0x12;      //   Turn on EC_ON.
    GPDRE = 0x08;      // Set charger IC to fast charge.
    GPDRI &= 0x10;      // Turn on EC_ON.
    GPDRJ = 0x08;      // Turn on mainpower pin.
    SetExTimer1();
  }
  #endif

  WUESR1  = 0xFF;  // power switch bit6
  WUESR2  = 0xFF;
  WUESR3  = 0xFF;  // ExTimer1 Bit6
  WUESR4  = 0xFF;
  WUESR7  = 0xFF;
  WUESR10  = 0xFF;  // AC in bit2
  WUESR14  = 0xFF;  // Novo bit0

  ISR1 = 0xFF;  // power switch bit6
  ISR2 = 0xFF;
  ISR3 = 0xFF;  // ExTimer1 Bit6
  ISR4 = 0xFF;
  ISR13= 0xFF;   // AC in bit2
  ISR16= 0xFF;   // Novo bit0
}

//-----------------------------------------------------------------------------
void InitWakeFromDeepSleep(void)
{
    //GPDRA = EC_DeepSleep_TempPortA;
    //GPDRB = EC_DeepSleep_TempPortB;
    //GPDRC = EC_DeepSleep_TempPortC;
    //GPDRD = EC_DeepSleep_TempPortD;
    //GPDRE = EC_DeepSleep_TempPortE;
    //GPDRF = EC_DeepSleep_TempPortF;
    //GPDRG = EC_DeepSleep_TempPortG;
    //GPDRH = EC_DeepSleep_TempPortH;
    //GPDRI = EC_DeepSleep_TempPortI;
    //GPDRJ = EC_DeepSleep_TempPortJ;

  IER0   = 0x00;  // 7 ~ 0
  IER1   = 0x00;  // 15 ~ 8
  IER2   = 0x00;  // 23 ~ 16
  IER3   = 0x00;
  IER4    = 0x00;
    IER9    = 0x00;
  IER13  = 0x00; // AC in
  IER16   = 0x00; // Novo

   ISR0    = 0xFF;
   ISR1    = 0xFF;
   ISR2   = 0xFF;
   ISR3    = 0xFF;
  ISR4  = 0xFF;
    ISR9    = 0xFF;
    ISR13   = 0xFF; // AC in
    ISR16   = 0xFF; // Novo

  WUESR1  = 0xFF;
  WUESR2  = 0xFF;
  WUESR3  = 0xFF;
  WUESR4  = 0xFF;
  WUESR7  = 0xFF;
  WUESR10  = 0xFF; // AC in
  WUESR14  = 0xFF; // Novo
  #if 0
    GPCRA1 = ALT;
    GPCRA2 = ALT;
  GPCRC3  = ALT + PULL_UP;
  GPCRC5  = ALT + PULL_UP;
  #endif
    //ADCSTS = EC_DeepSleep_Temp0;
    //ADCCFG = EC_DeepSleep_Temp1;
    //DACPWRDN = EC_DeepSleep_Temp2;
    //CGCTRL1R = EC_DeepSleep_Temp3;

  Init_Kbd();
  Core_Init_Regs();
  Init_Regs();
  Enable_Any_Key_Irq();  //msmart

  GPCRA0 = ALT; //Set to PWM again
#if Support_PWRBTN_LED       
  GPCRA3 = ALT; //Set to PWM again  
#endif //Support_PWRBTN_LED       
}

//----------------------------------------------------------------------------
// Output : 1 Crisis mode
//          0 Normal mode
//----------------------------------------------------------------------------
bit CheckCrisisMode(void)
{  // To add condition here
  BYTE BKSOL,BKSOH1,BKSOH2;
    BKSOL = KSOL;
    BKSOH1 = KSOH1;
    BKSOH2 = KSOH2;
 

  KSOL  = 0xEF;
  KSOH1 = 0xFF;
  KSOH2 = 0xFF;      // "R" key (scan line b4)
  WNCKR = 0x00;           // Delay 15.26 us
  WNCKR = 0x00;           // Delay 15.26 us
  if ((KSI&0x04) != 0)  // "R" key (data line b2)
  {
    KSOL  = 0xFF;

        KSOL  = BKSOL;
        KSOH1 = BKSOH1;
        KSOH2 = BKSOH2;
      return FALSE;
  }

  KSOL  = 0xFF;

    KSOL  = BKSOL;
    KSOH1 = BKSOH1;
    KSOH2 = BKSOH2;

  return TRUE;
  
}
