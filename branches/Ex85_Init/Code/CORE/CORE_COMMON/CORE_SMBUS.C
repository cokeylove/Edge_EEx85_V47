/*----------------------------------------------------------------------------
 * TITLE: CORE_SMBUS.C - the System Management SMBus Protocol drivers.
 *
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// Read/write SMbus byte/word function
//----------------------------------------------------------------------------
const sSMBus code asSMBus[]=
{
  { &HOCTL_A, &TRASLA_A, &HOCMD_A, &HOSTA_A, &D0REG_A, &D1REG_A,  &HOBDB_A,  &IER1,  &ISR1,  Int_SMBUS0,   &PECERC_A},
  { &HOCTL_B, &TRASLA_B, &HOCMD_B, &HOSTA_B, &D0REG_B, &D1REG_B,  &HOBDB_B,  &IER1,  &ISR1,  Int_SMBUS1,   &PECERC_B},
  { &HOCTL_C, &TRASLA_C, &HOCMD_C, &HOSTA_C, &D0REG_C, &D1REG_C,  &HOBDB_C,  &IER2,  &ISR2,  Int_SMBUS2,   &PECERC_C},
  { &HOCTL_D, &TRASLA_D, &HOCMD_D, &HOSTA_D, &D0REG_D, &D1REG_D,  &HOBDB_D,  &IER0,  &ISR0,  Int_SMBUS3,   &PECERC_D}
};

const sResetSMBusS code asResetSMBusS[]=
{
  { &HOSTA_A, &HOCTL2_A, &SMBPCTL_A, &GPCRF2, &GPCRF3, &GPDRF,  (BIT2+BIT3),   &SMBus1RecoverCunt },
  { &HOSTA_B, &HOCTL2_B, &SMBPCTL_B, &GPCRC1, &GPCRC2, &GPDRC,  (BIT1+BIT2),   &SMBus2RecoverCunt },
  { &HOSTA_C, &HOCTL2_C, &SMBPCTL_C, &GPCRF6, &GPCRF7, &GPDRF,  (BIT6+BIT7),   &SMBus3RecoverCunt },
  { &HOSTA_D, &HOCTL2_D, &SMBPCTL_D, &GPCRH1, &GPCRH2, &GPDRH,  (BIT1+BIT2),   &SMBus4RecoverCunt }
};

#if SMB_Channel_AUTO_detect
 const sSMBChannel code SMBDETCH[]= {SMB_AUTO_Channel};

BYTE SMB_CH_DET (BYTE Addr, Channel)
{
  BYTE i;
  for(i=0x00;i<sizeof(SMBDETCH)/sizeof(sSMBChannel);i++){
    if (Addr == SMBDETCH[i].SMBAddr){
      return SMBDETCH[i].SMBChannel;
    }
  }
  return Channel;
}
#endif  //SMB_Channel_AUTO_detect

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DelayInact(void)
{
    BYTE index;
    BYTE counter;
   for(index=0;index<200;index++)
   {
        counter++;
   }
}

//----------------------------------------------------------------------------
// The function of reseting SMbus
//----------------------------------------------------------------------------
void ResetSMBus(BYTE Channel)
{
    #ifdef PECI_Support
    if(Channel==SMbusCh3)
    {
        return;
    }
    #endif

    SET_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_KILL);
    CLEAR_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_KILL);
  //RSTC3=0x0F;                      // Reset all SMBus module
  *asResetSMBusS[Channel].SMBusSTA = 0xFE;  // clear bits
    *asResetSMBusS[Channel].SMBusPin0 = OUTPUT;
  *asResetSMBusS[Channel].SMBusPin1 = OUTPUT;
  DelayInact();
  *asResetSMBusS[Channel].GPIOReg |= asResetSMBusS[Channel].GPIOData;
  DelayInact();
    *asResetSMBusS[Channel].SMBusPin0 = ALT;
  *asResetSMBusS[Channel].SMBusPin1 = ALT;
  *asResetSMBusS[Channel].SMBusCTL2 = 0x02;
  *asResetSMBusS[Channel].SMBusPinCTL = 0x03;
  DelayInact();
  *asResetSMBusS[Channel].SMBusPinCTL = 0x07;
  *asResetSMBusS[Channel].SMBusCTL2 = 0x01;

    #ifdef SMBusServiceCenterFunc
    *asSSC[Channel].inusing=0x00;
    CLEAR_MASK(*asSMBus[Channel].SMBusIER, asSMBus[Channel].IERFlag);
    *asSMBus[Channel].SMBusISR = asSMBus[Channel].IERFlag;
    *asSSC[Channel].serviceflag=0x00;
    //Init_SMBus_Regs();

    if(DB_SMBusAck1==ACK_Start)     // For SMBus utility
    {
        DB_SMBusAddr=0x00;
        if(Channel==0x00)
        {
          DB_SMBusFlag1=0x00;
        }
        else if(Channel==0x01)
        {
            DB_SMBusFlag2=0x00;
        }
        else if(Channel==0x02)
        {
            DB_SMBusFlag3=0x00;
        }

      DB_SMBusACDID=0x00;
        DB_SMBusAck1 = ACK_Error;   // For SMBus utility error ACK.
    }
    #endif
}

//----------------------------------------------------------------------------
// SMbus read byte/word and write byte/word function
//----------------------------------------------------------------------------
BYTE bRWSMBus(BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,XBYTE *Var,BYTE PECSupport)
{
  BYTE counter;
  BYTE error;
    BYTE status;
    BYTE resutl;
#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel); 
#endif //SMB_Channel_AUTO_detect        
    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==SMBus_CanNotUse)
    {
        resutl = FALSE;                             // SMBus interface can't be used.
    }
    else
    {
        error = 0xEE;                               // Pre-set error
        resutl = FALSE;                             // Pre-set result is fail

        SMBCRC8_A=0x00;                             // Clear CRC variable

      if (Protocol&0x80)                          // Write byte function
      {
          if(PECSupport)
            {
                CalcCRC8(Addr);                     // To calculate crc8
            }
        *asSMBus[Channel].SMBusADR = Addr;      // set address with writing bit
          if(PECSupport)
            {
                CalcCRC8(Comd);                     // To calculate crc8
            }
          *asSMBus[Channel].SMBusCMD=Comd;      // Set command
        Protocol&=(~0x80);
            if(PECSupport)
            {
                CalcCRC8(*Var);                     // To calculate crc8
            }
        *asSMBus[Channel].SMBusData0 = *Var;    // set data1
        if(Protocol==SMbusRW)                // write word function
        {
            if(PECSupport)
                {
                    CalcCRC8(*(Var+0x01));          // To calculate crc8
                }
          *asSMBus[Channel].SMBusData1 = *(Var+0x01);
        }                            // set data2
      }
      else                             // Read function
      {
        *asSMBus[Channel].SMBusADR=Addr|0x01;
            *asSMBus[Channel].SMBusCMD=Comd;      // Set command
      }                              // set address with reading bit


      *asSMBus[Channel].SMBusSTA=0xFE;        // clear bits
      if(PECSupport)
        {
            *asSMBus[Channel].SMBusPEC=SMBCRC8_A;
          *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT|HOCTL_PEC_EN);   // Start transaction
        }
        else
        {
          *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT);  // Start transaction
        }

      TR1 = 0;                              // disable timer1
        ET1 = 0;                                    // Disable timer1 interrupt
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        TH1 = Timer_26ms>>8;                        // Set timer1 counter 26ms
        TL1 = Timer_26ms;                           // Set timer1 counter 26ms
        TF1 = 0;                                    // Clear overflow flag
        TR1 = 1;                                   // Enable timer1

      while (!TF1)
      {
            status = *asSMBus[Channel].SMBusSTA;    // Read SMBus Host Status
            if(IS_MASK_SET(status, (HOSTA_FINTR+HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
            {
                TR1=0;
          TF1=0;
                ET1=1;                            // Enable timer1 interrupt
                break;
            }
      }

      if(TF1)                                     // 26ms time-out and no any status bit is set.
      {
            TR1=0;
        TF1=0;
            ET1=1;                                  // Enable timer1 interrupt
            ResetSMBus(Channel);
            error = 0xEE;
      }
        else
        {
            if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE)))
            {
                if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                {
                    ResetSMBus(Channel);
                }
                error = 0xEE;
            }
            else                                    // Only Finish Interrupt bit is set.
            {
                error = 0x00;
            }
        }

      if(error == 0xEE)                           // Fail
      {
        resutl = FALSE;
      }
        else                                        // OK
        {
          if ((*asSMBus[Channel].SMBusADR & 0x01 != 0x00))
          {
            *Var = *asSMBus[Channel].SMBusData0;    // read data1
            if(Protocol==SMbusRW)
            {
              *(Var+0x01) = *asSMBus[Channel].SMBusData1;
            }                          // read data2
          }
            resutl = TRUE;
        }

      *asSMBus[Channel].SMBusSTA=0xFE;              // clear bits
    }

  return(resutl);
}

//----------------------------------------------------------------------------
// Read SMbus block function
//----------------------------------------------------------------------------
BYTE bRSMBusBlock(BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,XBYTE *Var)
{
  BYTE ack;
  BYTE bcount;
  BYTE status;
  BYTE InterfaceType;    

#if SMB_Channel_AUTO_detect    
  Channel = SMB_CH_DET(Addr, Channel);  
#endif //SMB_Channel_AUTO_detect    
	switch(Channel)
	{
    case SMbusChA:
			InterfaceType = HOCTL2_A & 0x02;
       break;
    case SMbusChB:
			InterfaceType = HOCTL2_B & 0x02;
       break;
    case SMbusChC:
			InterfaceType = HOCTL2_C & 0x02;
       break;
    case SMbusChD:
			InterfaceType = HOCTL2_D & 0x02;
       break;
    default:
     	InterfaceType = 0x00;
       break;
  }

  if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==SMBus_CanNotUse)
  {
    ack = FALSE;                            // SMBus interface can't be used.
  }
  else
  {
    ack = FALSE;                  // pr-set flag
    *asSMBus[Channel].SMBusADR=Addr|0x01;   // set address with reading bit
    *asSMBus[Channel].SMBusCMD=Comd;      // Set command
    *asSMBus[Channel].SMBusSTA=0xFE;      // clear bits
    *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT);    // Start transaction

    TR1 = 0;                          // disable timer1
    ET1 = 0;                                // Disable timer1 interrupt
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    TH1 = Timer_26ms>>8;                    // Set timer1 counter 26ms
    TL1 = Timer_26ms;                       // Set timer1 counter 26ms
    TF1 = 0;                                // Clear overflow flag
    TR1 = 1;                               // Enable timer1

    if(InterfaceType)
    {	//I2C mode
      if(I2CRBDataCount > 32)
      {
      		I2CRBDataCount = 32;
      }
      bcount = I2CRBDataCount;            // Set byte counter
	    SMBusRBDataCount = I2CRBDataCount;
	  }
	  else
	  {	//SMBus mode
    		bcount = 0x00;                      // clear byte counter
	  }
    while(!TF1)                             // Not time out
    {
       status = *asSMBus[Channel].SMBusSTA;    // Read SMBus Host Status
       if(IS_MASK_SET(status, (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
       {
         if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE)))
         {
           if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
           {
              ResetSMBus(Channel);
           }
           TR1 = 0;              // disable timer1
           TF1 = 0;              // clear overflow flag
           ET1 = 1;              // Enable timer1 interrupt
           ack = FALSE;
           break;
         }
         else                            // Only Byte Done Status bit is set.
         {
          	if(InterfaceType)
           	{	//I2C mode
	           if(bcount != 0x00)
	           {
	             	if(bcount == I2CRBDataCount)
	             	{
	                *Var = *asSMBus[Channel].SMBusData0; // get the first data
	                bcount--;
	                Var++;
	                *Var = *asSMBus[Channel].SMBusBData;    // get 2nd data
	                bcount--;
	                Var++;
	              }
	              else
	              {  
	                *Var = *asSMBus[Channel].SMBusBData;    // get data from 3th
	                bcount--;
	                Var++;
	              }
	
	              if(bcount == 0x01)
	              {                // set last byte flag
	                SET_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_LABY);
	              }
	              	*asSMBus[Channel].SMBusSTA=0xFE;    // clear bits
	            }
           	}
           	else
           	{	//SMBus mode
           	  if(bcount ==0x00)
             {
                bcount = *asSMBus[Channel].SMBusData0; // get the data of byte count
                Hook_GetSMBusReadBlockByteCount(&bcount); 
                SMBusRBDataCount = bcount;
             }
	           else
	           {
		           *Var = *asSMBus[Channel].SMBusBData;    // get data
                bcount--;
                Var++;
                if(bcount == 0x01)
                {                // set last byte flag
                   SET_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_LABY);
                }
                *asSMBus[Channel].SMBusSTA=0xFE;    // clear bits
              }
		       }
           if(bcount == 0x00)
           {
             TR1 = 0;          // disable timer1
             TF1 = 0;          // clear overflow flag
             ET1 = 1;          // Enable timer1 interrupt
             ack = TRUE;
             I2CRBDataCount = 0;   
             break;                  // SMBus read block done.
           }
         }
       }
     }
     if(TF1)                                 // time-out
     {
       TR1 = 0;                      // disable timer1
       TF1 = 0;                      // clear overflow flag
       ET1 = 1;                      // Enable timer1 interrupt
       ResetSMBus(Channel);
       ack = FALSE;
     }

     CLEAR_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_LABY);    // clear last byte flag
     *asSMBus[Channel].SMBusSTA=0xFE;        // clear bits
  }
  return(ack);
}

//----------------------------------------------------------------------------
// Write SMbus block function
//----------------------------------------------------------------------------
BYTE bWSMBusBlock(BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,XBYTE *Var,BYTE ByteCont,BYTE PECsupport)
{
  BYTE ack;
    BYTE BCTemp;
    BYTE status;
#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel);  
#endif //SMB_Channel_AUTO_detect    
    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==SMBus_CanNotUse)
    {
        ack = FALSE;                            // SMBus interface can't be used.
    }
    else
    {
      ack = FALSE;                  // pre-set flag
      BCTemp = 0x00;                      // clear byte count

        if(PECsupport)
        {
            SMBCRC8_A=0x00;
            CalcCRC8(Addr);
            CalcCRC8(Comd);
            CalcCRC8(ByteCont);
            CalcCRC8(*Var);
        }

      Protocol&=(~0x80);
      *asSMBus[Channel].SMBusADR=Addr;      // set address with writing flag
        *asSMBus[Channel].SMBusCMD=Comd;      // Set command
        *asSMBus[Channel].SMBusData0=ByteCont;  // set byte count
        BCTemp=ByteCont;                      // sync byte count
        *asSMBus[Channel].SMBusBData=*Var;      // set first data
        *asSMBus[Channel].SMBusSTA=0xFE;      // clear bits
      if(PECsupport)
        {
            *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT|HOCTL_PEC_EN);// Start transaction
        }
        else
        {
            *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT);// Start transaction
        }

      TR1 = 0;                          // disable timer1
        ET1 = 0;                                // Disable timer1 interrupt
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        TH1 = Timer_26ms>>8;                    // Set timer1 counter 26ms
        TL1 = Timer_26ms;                       // Set timer1 counter 26ms
        TF1 = 0;                                // Clear overflow flag
        TR1 = 1;                               // Enable timer1

        while(!TF1)                             // Not time out
        {
            status = *asSMBus[Channel].SMBusSTA;    // Read SMBus Host Status
            if(IS_MASK_SET(status, (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
            {
                if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE)))
                {
                    if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                    {
                        ResetSMBus(Channel);
                    }
                    TR1 = 0;              // disable timer1
                   TF1 = 0;              // clear overflow flag
                  ET1 = 1;              // Enable timer1 interrupt
                  ack = FALSE;
                    break;
                }
                else                            // Only Byte Done Status bit is set.
                {
                  BCTemp--;
                  Var++;              // point to next address of variable

                  if(BCTemp != 0x00)
                  {
                        if(PECsupport)
                        {
                            *asSMBus[Channel].SMBusBData=*Var;
                            CalcCRC8(*Var);
                            if(BCTemp==0x01)    // last byte of byte count
                            {
                                *asSMBus[Channel].SMBusPEC = SMBCRC8_A;
                            }
                        }
                        else
                        {
                        *asSMBus[Channel].SMBusBData=*Var;
                        }
                        *asSMBus[Channel].SMBusSTA=0xFE;    // clear bits
                  }
                    else                        // write block transaction done
                    {
                      TR1 = 0;          // disable timer1
                       TF1 = 0;          // clear overflow flag
                      ET1 = 1;          // Enable timer1 interrupt
                      ack = TRUE;
                      break;
                    }
                }
            }
        }

        if(TF1)                                 // time-out
        {
            TR1 = 0;                      // disable timer1
           TF1 = 0;                      // clear overflow flag
          ET1 = 1;                      // Enable timer1 interrupt
        ResetSMBus(Channel);
        ack = FALSE;
        }

      *asSMBus[Channel].SMBusSTA=0xFE;        // clear bits
    }

  return(ack);
}

//----------------------------------------------------------------------------
// The function of SMbus send byte
//----------------------------------------------------------------------------
BYTE bSMBusSendByte(BYTE Channel,BYTE Addr,BYTE SData)
{
  BYTE error;
    BYTE result;
    BYTE status;
#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel);  
#endif //SMB_Channel_AUTO_detect    
    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==SMBus_CanNotUse)
    {
        result = FALSE;                             // SMBus interface can't be used.
    }
    else
    {
        error = 0xEE;                               // Pre-set error
        result = FALSE;                             // Pre-set result is fail

      *asSMBus[Channel].SMBusADR = Addr;          // set address with writing bit
      *asSMBus[Channel].SMBusCMD = SData;          // Set command
      *asSMBus[Channel].SMBusSTA = 0xFE;          // clear bits
      *asSMBus[Channel].SMBusCTL = (0x04|HOCTL_SRT);  // Start transaction

        TR1 = 0;                                    // Disable timer1
        ET1 = 0;                                    // Disable timer1 interrupt
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        TH1 = Timer_26ms>>8;                        // Set timer1 counter 26ms
        TL1 = Timer_26ms;                           // Set timer1 counter 26ms
        TF1 = 0;                                    // Clear overflow flag
        TR1 = 1;                                   // Enable timer1

      while (!TF1)
      {
            status = *asSMBus[Channel].SMBusSTA;    // Read SMBus Host Status
            if(IS_MASK_SET(status, (HOSTA_FINTR+HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
            {
                TR1=0;                              // Disable timer1
          TF1=0;                              // Clear overflow flag
                ET1=1;                            // Enable timer1 interrupt
                break;
            }
      }

      if(TF1)                                     // 26ms time-out and no any status bit is set.
      {
            TR1=0;
        TF1=0;
            ET1=1;                                  // Enable timer1 interrupt
            ResetSMBus(Channel);
            error = 0xEE;
      }
        else
        {
            if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE)))
            {
                if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                {
                    ResetSMBus(Channel);
                }
                error = 0xEE;
            }
            else                                    // Only Finish Interrupt bit is set.
            {
                error = 0x00;
            }
        }

      if(error == 0xEE)
      {
        result = FALSE;
      }
        else
        {
            result = TRUE;
        }
      *asSMBus[Channel].SMBusSTA=0xFE;  // clear bits
    }

  return(result);
}

//----------------------------------------------------------------------------
// The function of SMbus receive byte
//----------------------------------------------------------------------------
BYTE bSMBusReceiveByte(BYTE Channel,BYTE Addr,XBYTE *Var)
{
  BYTE error;
    BYTE result;
    BYTE status;
#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel);
#endif //SMB_Channel_AUTO_detect    
    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==SMBus_CanNotUse)
    {
        result = FALSE;                             // SMBus interface can't be used.
    }
    else
    {
        error = 0xEE;                               // Pre-set error
        result = FALSE;                             // Pre-set result is fail

      *asSMBus[Channel].SMBusADR = (Addr|0x01);   // set address with writing bit
      *asSMBus[Channel].SMBusSTA = 0xFE;        // clear bits
      *asSMBus[Channel].SMBusCTL = (0x04|HOCTL_SRT);  // Start transaction

      TR1 = 0;                              // disable timer1
        ET1 = 0;                                    // Disable timer1 interrupt
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        TH1 = Timer_26ms>>8;                        // Set timer1 counter 26ms
        TL1 = Timer_26ms;                           // Set timer1 counter 26ms
        TF1 = 0;                                    // Clear overflow flag
        TR1 = 1;                                   // Enable timer1

      while (!TF1)
      {
            status = *asSMBus[Channel].SMBusSTA;    // Read SMBus Host Status
            if(IS_MASK_SET(status, (HOSTA_FINTR+HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
            {
                TR1=0;
          TF1=0;
                ET1=1;                            // Enable timer1 interrupt
                break;
            }
      }

      if(TF1)                                     // 26ms time-out and no any status bit is set.
      {
            TR1=0;
        TF1=0;
            ET1=1;                                  // Enable timer1 interrupt
            ResetSMBus(Channel);
            error = 0xEE;
      }
        else
        {
            if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE)))
            {
                if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                {
                    ResetSMBus(Channel);
                }
                error = 0xEE;
            }
            else                                    // Only Finish Interrupt bit is set.
            {
                error = 0x00;
            }
        }

      if(error == 0xEE)
      {
        result = FALSE;
      }
        else
        {
          *Var = *asSMBus[Channel].SMBusData0;    // read data
          result = TRUE;
        }
      *asSMBus[Channel].SMBusSTA=0xFE;      // clear bits
    }

  return(result);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CalcCRC8(BYTE sourcebyte)
{
    BYTE temp;
  SMBCRC8_A ^=sourcebyte;
    temp = SMBCRC8_A;

    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        SMBCRC8_A ^= temp;
    }
    else
    {
        temp = temp<<1;
        SMBCRC8_A ^= 0x09;
        SMBCRC8_A ^= temp;
    }

    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        SMBCRC8_A ^= temp;
    }
    else
    {
        temp = temp<<1;
        SMBCRC8_A ^= 0x07;
        SMBCRC8_A ^= temp;
    }
}

//----------------------------------------------------------------------------
// The function of Checking SMbus clock and data pins are both high
//----------------------------------------------------------------------------
BYTE CheckSMBusFree(BYTE channel)
{
    BYTE pinstatus;

    pinstatus = *asResetSMBusS[channel].SMBusPinCTL;

    if((pinstatus&=0x03)==0x03) // Check BIT0 and BIT1
    {
        return(SMBus_Free);
    }
    else
    {
        return(SMBus_Busy);
    }
}

//----------------------------------------------------------------------------
// The function of Checking SMbus pins are all alt
//----------------------------------------------------------------------------
BYTE CheckSMBusInterfaceIsALT(BYTE channel)
{
    if(((*asResetSMBusS[channel].SMBusPin0 & 0xFB)!=0x00)||((*asResetSMBusS[channel].SMBusPin1 & 0xFB)!=0x00))
    {
        return(SMBusPin_GPIO);
    }
    else
    {
        return(SMBusPin_ALT);
    }
}

//----------------------------------------------------------------------------
// The function of Checking SMbus pins are all alt
//----------------------------------------------------------------------------
BYTE CheckSMBusInterfaceCanbeUse(BYTE channel, BYTE accesstype)
{
    BYTE checksmbus;

    checksmbus = SMBus_CanUse;                  // Pre-set smbus interface can be used

    #ifdef PECI_Support
    if(channel==SMbusCh3)                       // If PECI function enable, return.
    {
        checksmbus = SMBus_CanNotUse;           // Interface can't be used
        return(checksmbus);                     // return result
    }
    #endif

    if(CheckSMBusInterfaceIsALT(channel)==SMBusPin_GPIO)    // SMBus interface is GPIO function
    {
        checksmbus = SMBus_CanNotUse;           // Interface can't be used
    }
    else                                        // SMBus interface is ALT function
    {
        if(accesstype==SMBus_AccessType_Fucn)
        {
            #ifdef SMBusServiceCenterFunc
            WatiSMBusCHxFree(channel);          // Check smbus is in used or not.
            #endif
        }

        if(CheckSMBusFree(channel)==SMBus_Busy)
        {
            checksmbus = SMBus_CanNotUse;       // Interface can't be used
            if( (*asResetSMBusS[channel].recoverCunt)++ >SMBus_BusyRecover)
            {
                ResetSMBus(channel);
            }
        }
        else
        {
            *asResetSMBusS[channel].recoverCunt=0x00;
            checksmbus = SMBus_CanUse;          // smbus interface can be used
        }
    }

    return(checksmbus);                         // return result
}

//----------------------------------------------------------------------------
// To init. SMBus setting
//----------------------------------------------------------------------------
void Core_Init_SMBus(void)
{
    SMBus_BusyRecover = 20;         // Reset SMBus interface if bus clock pin or data pin is low greater than SMBus_BusyRecover times.
}


//============================================================================
// I2C protocol
//============================================================================
//#define SUPPORT_I2C_PROTOCOL

/**
 * ****************************************************************************
 * I2C-compatible read command
 *
 * @return
 * 0, fail
 * 1, OK
 *
 * @parameter
 * p_channel, i2c channel selection        0 ~ 2
 * p_address, device address with r/w bit
 * p_length, read length
 * *p_data_buffer, data buffer address
 * P_first_word_is_read_length
 *
 * ****************************************************************************
 */
BYTE I2C_Read(BYTE p_channel, BYTE p_address, WORD p_length,
                XBYTE *p_data_buffer, BYTE P_first_word_is_read_length)
{
    BYTE status;
    BYTE ack;
    BYTE l_read_length[2];
    BYTE l_read_length_index;

    /* pre-set fail. */
    ack = FALSE;

    if(P_first_word_is_read_length)
    {
        l_read_length_index = 0x02;
    }

    /* illegal parameter */
    if(p_length == 0x00)
    {
        ack = FALSE;
    }
    else
    {
        /* to check interface can be used or not */
        if(CheckSMBusInterfaceCanbeUse(p_channel, SMBus_AccessType_Fucn)
            ==SMBus_CanNotUse)
        {
            /* SMBus interface can't be used. */
            ack = FALSE;
        }
        else
        {
            /* I2C enable */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x13;

            /* block read address */
            *asSMBus[p_channel].SMBusADR = (p_address|0x01);

            /* only read one byte */
            if(p_length==0x01)
            {
                /* set last byte flag */
                SET_MASK(*asSMBus[p_channel].SMBusCTL,HOCTL_LABY);
            }

            /* i2c read, Start transaction */
            *asSMBus[p_channel].SMBusCTL = 0x5C;

            /* enable 26ms time-out */
            TR1 = 0;			    /* disable timer1 */
            ET1 = 0;                /* Disable timer1 interrupt */
            TH1 = Timer_26ms>>8;    /* Set timer1 counter 26ms */
            TL1 = Timer_26ms;       /* Set timer1 counter 26ms */
            TF1 = 0;                /* Clear overflow flag */
            TR1 = 1;                /* Enable timer1 */

            /* no time out */
            while(!TF1)
            {
                /* Read SMBus Host Status */
                status = *asSMBus[p_channel].SMBusSTA;

                /* any status bit */
                if(IS_MASK_SET(status, (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+
                                HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE+HOSTA_FINTR)))
                {
                    /* any error bit */
                    if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                                                HOSTA_NACK+HOSTA_TMOE)))
                    {
                        /* bus fail or error */
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(p_channel);
                        }

                        /* disable timer1 */
                        TR1 = 0;

                        /* clear overflow flag */
        	            TF1 = 0;
                        break;
                    }

                    /* Byte Done or finish Status bit is set */
                    else
                    {
                        if(IS_MASK_SET(status,HOSTA_BDS))
                        {
                            /* read data */
                            if(p_length)
                            {
                                *p_data_buffer = *asSMBus[p_channel].SMBusBData;
                                p_data_buffer++;

                                p_length--;

                                if(P_first_word_is_read_length)
                                {
                                    if(l_read_length_index)
                                    {
                                        l_read_length_index--;
                                        l_read_length[l_read_length_index] =
                                            *(p_data_buffer -1);

                                        if(l_read_length_index==0)
                                        {
                                            p_length = l_read_length[1] +
                                                (l_read_length[0]*256);
                                            if(p_length>2)
                                            {
                                                p_length-=2;
                                            }
                                        }
                                    }
                                }

                            }

                            /* next cycle is last byte */
                            if(p_length==0x01)
                            {
                                /* set last byte flag */
            	                SET_MASK(*asSMBus[p_channel].SMBusCTL,
                                    HOCTL_LABY);
                            }

                            /* clear status bits for next byte */
                            *asSMBus[p_channel].SMBusSTA=0xFC;

                            /* reset timer 1 */
                            TH1 = Timer_26ms>>8;
                            TL1 = Timer_26ms;
                            TF1 = 0;                /* Clear overflow flag */
                        }

                        /* finish */
                        else
                        {
                            /* disable timer1 */
                            TR1 = 0;

                            /* clear overflow flag */
        	                TF1 = 0;

        	                if(p_length==0)
        	                {
                                /* i2c read success */
        	                    ack = TRUE;
        	                }
                            break;
                        }
                    }
                }
            }

            /* 26ms time-out and no any status bit is set. */
            if(TF1)
            {
                ResetSMBus(p_channel);
            }

            /* disable I2C mode */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x11;

            /* w/c status */
            *asSMBus[p_channel].SMBusSTA=0xFE;

            /* timer 1 stop */
            TR1=0;
            TF1=0;
            ET1=1;
        }
    }
    return(ack);
}

/**
 * ****************************************************************************
 * I2C-compatible write command
 *
 * @return
 * 0, fail
 * 1, OK
 *
 * @parameter
 * p_channel, i2c channel selection        0 ~ 2
 * p_address, device address with r/w bit
 * p_length, write length
 * *p_data_buffer, data buffer address
 *
 * ****************************************************************************
 */
BYTE I2C_Write(BYTE p_channel, BYTE p_address, BYTE p_length,
                XBYTE *p_data_buffer)
{
    BYTE status;
    BYTE ack;

    /* pre-set fail. */
    ack = FALSE;

    /* illegal parameter */
    if(p_length == 0x00)
    {
        ack = FALSE;
    }
    else
    {
        /* to check interface can be used or not */
        if(CheckSMBusInterfaceCanbeUse(p_channel, SMBus_AccessType_Fucn)
            ==SMBus_CanNotUse)
        {
            /* SMBus interface can't be used. */
            ack = FALSE;
        }
        else
        {
            /* I2C enable */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x13;

            /* device address */
            *asSMBus[p_channel].SMBusADR = p_address;

            /* first byte */
            *asSMBus[p_channel].SMBusBData = *p_data_buffer;

            /* Start transaction */
            *asSMBus[p_channel].SMBusCTL=0x5C;

            /* enable 26ms time-out */
            TR1 = 0;			    /* disable timer1 */
            ET1 = 0;                /* Disable timer1 interrupt */
            TH1 = Timer_26ms>>8;    /* Set timer1 counter 26ms */
            TL1 = Timer_26ms;       /* Set timer1 counter 26ms */
            TF1 = 0;                /* Clear overflow flag */
            TR1 = 1;                /* Enable timer1 */

            /* no time out */
            while (!TF1)
    	    {
                /* Read SMBus Host Status */
                status = *asSMBus[p_channel].SMBusSTA;

                /* any status bit */
                if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                    HOSTA_NACK+HOSTA_TMOE+HOSTA_BDS+HOSTA_FINTR )))
                {
                    /* any error bit */
                    if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        HOSTA_NACK+HOSTA_TMOE)))
                    {
                        /* bus fail or error */
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(p_channel);
                        }
                        ack = FALSE;
                        break;
                    }
                    /* Finish or Byte Done bit is set. */
                    else
                    {
                        /* finish bit */
                        if(IS_MASK_SET(status,HOSTA_FINTR))
                        {
                            /* disable timer1 */
                            TR1 = 0;

                            /* clear overflow flag */
        	                TF1 = 0;

        	                if(p_length==0)
        	                {
                                /* i2c write success */
        	                    ack = TRUE;
        	                }
                            break;
                        }

                        /* Byte Done */
                        else
                        {
                            if(p_length)
                            {
                                p_length--;

                                if(p_length)
                                {
                                    /* to send next byte */
                                    p_data_buffer++;
                                    *asSMBus[p_channel].SMBusBData =
                                        *p_data_buffer;
                                }
                                else
                                {
                                    /* i2c write success and wait finish bit */

                                    /* disable I2C mode */
                                    *asResetSMBusS[p_channel].SMBusCTL2 = 0x11;
                                }
                                *asSMBus[p_channel].SMBusSTA=0xFC;
                            }

                            /* reset timer 1 */
                            TH1 = Timer_26ms>>8;
                            TL1 = Timer_26ms;
                            TF1 = 0;            /* Clear overflow flag */
                        }
                    }
                }
    	    }

            /* 26ms time-out and no any status bit is set. */
            if(TF1)
            {
                ResetSMBus(p_channel);
            }

            /* disable I2C mode */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x11;

            /* w/c status */
            *asSMBus[p_channel].SMBusSTA=0xFE;

            /* timer 1 stop */
            TR1=0;
            TF1=0;
            ET1=1;
        }
    }

    return(ack);
}

/**
 * ****************************************************************************
 * I2C-compatible write to read command
 *
 * @return
 * 0, fail
 * 1, OK
 *
 * @parameter
 * p_channel, i2c channel selection        0 ~ 2
 * p_address, device addres
 * p_wlength, write length
 * *p_wdata_buffer, data buffer address for write
 * p_rlength, read length
 * *p_rdata_buffer, data buffer address for read
 *
 * ****************************************************************************
 */
BYTE I2C_Write_to_Read(BYTE p_channel, BYTE p_address, BYTE p_wlength,
    XBYTE *p_wdata_buffer, WORD p_rlength, XBYTE *p_rdata_buffer,
    BYTE P_first_word_is_read_length)
{
    BYTE status;
    BYTE ack;
    BYTE l_read_length[2];
    BYTE l_read_length_index;

    if(P_first_word_is_read_length)
    {
        l_read_length_index = 0x02;
    }

    /* pre-set fail. */
    ack = FALSE;

    /* illegal parameter */
    if(p_wlength == 0x00)
    {
        ack = FALSE;
    }
    else
    {
        /* to check interface can be used or not */
        if(CheckSMBusInterfaceCanbeUse(p_channel, SMBus_AccessType_Fucn)==
            SMBus_CanNotUse)
        {
            /* SMBus interface can't be used. */
            ack = FALSE;
        }
        else
        {
            /* I2C enable */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x13;

            /* device address (write) */
            *asSMBus[p_channel].SMBusADR = (p_address & 0xFE);

            /* first byte */
            *asSMBus[p_channel].SMBusBData = *p_wdata_buffer;

            /* Start transaction */
            *asSMBus[p_channel].SMBusCTL=0x5C;

            /* enable 26ms time-out */
            TR1 = 0;			    /* disable timer1 */
            ET1 = 0;                /* Disable timer1 interrupt */
            TH1 = Timer_26ms>>8;    /* Set timer1 counter 26ms */
            TL1 = Timer_26ms;       /* Set timer1 counter 26ms */
            TF1 = 0;                /* Clear overflow flag */
            TR1 = 1;                /* Enable timer1 */

            /* no time out */
            while (!TF1)
    	    {
                /* Read SMBus Host Status */
                status = *asSMBus[p_channel].SMBusSTA;

                /* any status bit */
                if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                    HOSTA_NACK+HOSTA_TMOE+HOSTA_BDS)))
                {
                    /* any error bit */
                    if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        HOSTA_NACK+HOSTA_TMOE)))
                    {
                        /* bus fail or error */
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(p_channel);
                        }
                        ack = FALSE;
                        break;
                    }
                    /* Byte Done or finish bit is set. */
                    else
                    {
                        /* finish bit */
                        if(IS_MASK_SET(status,HOSTA_FINTR))
                        {
                            /* disable timer1 */
                            TR1 = 0;

                            /* clear overflow flag */
        	                TF1 = 0;

                            /* i2c write success */
    	                    ack = FALSE;
                        }
                        else
                        {
                            if(p_wlength)
                            {
                                p_wlength--;

                                if(p_wlength!=0x00)
                                {
                                    /* to send next byte */
                                    p_wdata_buffer++;
                                    *asSMBus[p_channel].SMBusBData =
                                        *p_wdata_buffer;

                                    /* clear status bits for next byte */
                                    *asSMBus[p_channel].SMBusSTA=0xFC;
                                }
                                /* change direction. */
                                else
                                {
                                    /* to set I2C_SW_EN and I2C_SW_WAIT */
                                    *asResetSMBusS[p_channel].SMBusCTL2 |=
                                        (BIT2+BIT3);

                                    /* clear status bits */
                                    *asSMBus[p_channel].SMBusSTA=0xFC;

                                    if(p_rlength==0x01)
                                    {
                                        /* set last byte flag */
                    	                SET_MASK(*asSMBus[p_channel].SMBusCTL,
                                            HOCTL_LABY);
                                    }

                                    /* to clear I2C_SW_WAIT */
                                    *asResetSMBusS[p_channel].SMBusCTL2 &=
                                        ~(BIT2);

                                    ack = TRUE;
                                    break;
                                }

                                /* reset timer 1 */
                                TH1 = Timer_26ms>>8;
                                TL1 = Timer_26ms;
                                TF1 = 0;            /* Clear overflow flag */
                            }
                        }
                    }
                }
    	    }

            if(ack == TRUE)
            {
                /* pre-set fail. */
                ack = FALSE;

                /* reset timer 1 */
                TH1 = Timer_26ms>>8;
                TL1 = Timer_26ms;
                TF1 = 0;                /* Clear overflow flag */

                /* no time out */
                while(!TF1)
                {
                    /* Read SMBus Host Status */
                    status = *asSMBus[p_channel].SMBusSTA;

                    /* any status bit */
                    if(IS_MASK_SET(status, (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+
                        HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE+HOSTA_FINTR )))
                    {
                        /* any error bit */
                        if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+
                            HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE)))
                        {
                            /* bus fail or error */
                            if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                            {
                                ResetSMBus(p_channel);
                            }

                            /* disable timer1 */
                            TR1 = 0;

                            /* clear overflow flag */
            	            TF1 = 0;
                            break;
                        }

                        /* Only Byte Done Status bit is set (successful) */
                        else
                        {
                            if(IS_MASK_SET(status,HOSTA_BDS))
                            {
                                /* read data */
                                if(p_rlength)
                                {
                                    *p_rdata_buffer =
                                        *asSMBus[p_channel].SMBusBData;
                                    p_rdata_buffer++;

                                    p_rlength--;

                                    if(P_first_word_is_read_length)
                                    {
                                        if(l_read_length_index)
                                        {
                                            l_read_length_index--;
                                            l_read_length[l_read_length_index] =
                                                *(p_rdata_buffer -1);

                                            if(l_read_length_index==0)
                                            {
                                                p_rlength = l_read_length[1] +
                                                    (l_read_length[0]*256);
                                                if(p_rlength>2)
                                                {
                                                    p_rlength-=2;
                                                }
                                            }
                                        }
                                    }
                                }

                                /* next cycle is last byte */
                                if(p_rlength==0x01)
                                {
                                    /* set last byte flag */
                	                SET_MASK(*asSMBus[p_channel].SMBusCTL,
                                        HOCTL_LABY);
                                }

                                /* clear status bits for next byte */
                                *asSMBus[p_channel].SMBusSTA=0xFC;

                                /* reset timer 1 */
                                TH1 = Timer_26ms>>8;
                                TL1 = Timer_26ms;
                                TF1 = 0;
                            }
                            /* finish */
                            else
                            {
                                /* disable timer1 */
                                TR1 = 0;

                                /* clear overflow flag */
            	                TF1 = 0;

            	                if(p_rlength==0)
            	                {
                                    /* i2c write success */
            	                    ack = TRUE;
            	                }
                                break;
                            }

                        }
                    }
                }

            }

            /* 26ms time-out and no any status bit is set. */
            if(TF1)
            {
                ResetSMBus(p_channel);
            }

            /* w/c status */
            *asSMBus[p_channel].SMBusSTA=0xFE;

            /* disable I2C mode */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x11;

            /* timer 1 stop */
            TR1=0;
            TF1=0;
            ET1=1;
        }
    }

    return(ack);
}

