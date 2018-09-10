/*-----------------------------------------------------------------------------
 * TITLE: OEM_Tamper.C
 *
 * Author : Ivan Chen
 *
 * Note : This is tamper function. 
 *
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#if (Support_Tamper_function == Tamper_ADC)
/********************************************************************************
* Function name: Get_tamper_status (ADC)
*
* Descriptoin: This is get device status by ADC (follow below table) to check whether have device be removed under S3
*
*     ----------------------------- 
*     | HDD | SSD |BayHDD|        |
*     -----------------------------
*     |  0  |  0  |  0  |  3V     |
*     -----------------------------
*     |  0  |  0  |  1  |  2.819V |
*     -----------------------------
*     |  0  |  1  |  0  |  2.589V |
*     -----------------------------
*     |  0  |  1  |  1  |  2.283V |
*     -----------------------------
*     |  1  |  0  |  0  |  2.074V |
*     -----------------------------
*     |  1  |  0  |  1  |  1.856V |
*     -----------------------------
*     |  1  |  1  |  0  |  1.756V |
*     -----------------------------
*     |  1  |  1  |  1  |  1.594V |
*     -----------------------------
* Arguments: 
*        Tamper_status : ADC value
*        Tamper_Dev_Ste : BTT6 last HDD status
*                         BIT5 last SSD status
*                         BIT4 last Bay HDD status
*                         BIT2 current HDD status
*                         BIT1 current SSD status
*                         BIT0 current Bay HDD status
*
* Return Values: TRUE : there are device change under S3
*                FALSE : no device change under S3
*       
*******************************************************************************/
BOOL Get_tamper_status (void)
{
   WORD Temp32;
   XBYTE Temp8;
   if(IS_MASK_SET(VCH0CTL, BIT7)){                                   //get new datas
     VCH0CTL |= 0x80;        // write clear data vaild flag
     Temp32 = ((WORD)VCH0DATM << 8) + VCH0DATL;
     if (Temp32 > Tamper_status){
       if ((Temp32 - Tamper_status) < Tamper_detect_tolerance) {     //no device change  
         CLEAR_MASK(Tamper_Dev_Ste, Tamper_DEBOUNCE);
         return FALSE;
       }
     }else {
       if ((Tamper_status - Temp32) < Tamper_detect_tolerance) {     //no device change
         CLEAR_MASK(Tamper_Dev_Ste, Tamper_DEBOUNCE);     
         return FALSE;
       }
     }

   }else{
     CLEAR_MASK(Tamper_Dev_Ste, Tamper_DEBOUNCE);
     return FALSE;
   }
  
//check current status
   if(Temp32 >= TamperADC_LV0){
      Temp8 = 0;
   }   
   else if((Temp32 < TamperADC_LV0)&& (Temp32 >= TamperADC_LV1)){
      Temp8 = 1;   
   } 
   else if((Temp32 < TamperADC_LV1)&& (Temp32 >= TamperADC_LV2)){
      Temp8 = 2;   
   }
   else if((Temp32 < TamperADC_LV2)&& (Temp32 >= TamperADC_LV3)){
      Temp8 = 3;   
   }
   else if((Temp32 < TamperADC_LV3)&& (Temp32 >= TamperADC_LV4)){
      Temp8 = 4;   
   }
   else if((Temp32 < TamperADC_LV4)&& (Temp32 >= TamperADC_LV5)){
      Temp8 = 5;   
   } 
   else if((Temp32 < TamperADC_LV5)&& (Temp32 >= TamperADC_LV6)){
      Temp8 = 6;   
   }    
   else if(Temp32 < TamperADC_LV6){
      Temp8 = 7;   
   }  
//update device status   
   if (IS_MASK_SET(Tamper_Dev_Ste, Tamper_DEBOUNCE)){   //2nd detect
     CLEAR_MASK(Tamper_Dev_Ste, Tamper_DEBOUNCE);
     if (Tamper_Dev_temp == Temp8){                     //1st = 2nd ???
       Tamper_status = Temp32;                          //device change       
       Tamper_Dev_Ste = Tamper_Dev_Ste << 4;            //update device status
       Tamper_Dev_Ste |= Tamper_Dev_temp;
       return TRUE;
     }    
   }else{                                         //1st detect       
     Tamper_Dev_temp = Temp8;
     SET_MASK(Tamper_Dev_Ste, Tamper_DEBOUNCE);
   }  
   return FALSE;
}
#endif //(Support_Tamper_function == Tamper_ADC)


#if (Support_Tamper_function == Tamper_GPIO)
/*******************************************************************************
/********************************************************************************
* Function name: Get_tamper_status (GPIO)
*
* Descriptoin: This is get device status by GPIO to check whether have device be removed under S3
*
* Arguments: 
*        Tamper_status : ADC value
*        Tamper_Dev_Ste : BTT6 last HDD status
*                         BIT5 last SSD status
*                         BIT4 last Bay HDD status
*                         BIT2 current HDD status
*                         BIT1 current SSD status
*                         BIT0 current Bay HDD status
*
* Return Values: TRUE : there are device change under S3
*                FALSE : no device change under S3
*       
*******************************************************************************/     
BOOL Get_tamper_status (void)
{
  Tamper_Dev_Ste = Tamper_Dev_Ste << 4;
  Tamper_Dev_Ste |= (ChkHDD_Detect() << 2);
#if Support_Chk_mSATA    
  Tamper_Dev_Ste |= (ChkmSata_Detect() << 1);
#endif //Support_Chk_mSATA   
#if Support_Chk_BayHDD
//  Tamper_Dev_Ste |= (ChkmBayHDD_Detect());
#endif //Support_Chk_BayHDD  
  if( (Tamper_Dev_Ste && 0x0F) == (Tamper_Dev_Ste >> 4)) {   //no device be remvoed 
    return FALSE;   
  }else {      
    return TRUE;
  }
}
#endif //(Support_Tamper_function == Tamper_GPIO)

#if Support_Tamper_function
/*******************************************************************************
* Function name: Tamper_DET
*
* Descriptoin: This is tamper function that check device Whether be removed under S3,
*              add update current status to last status.
*
* Arguments: 
*
*
*
* Return Values: none 
*       
*******************************************************************************/
void Tamper_DET(void)  // TimeBase: 5ms.
{
//Reset status under S5
  if ((SystemIsS5 || (SysPowState==SYSTEM_S5_S0)) && Read_EC_ON()){ 
    Get_tamper_status();
//BayHDD (Bay HDD)
#if Support_Chk_BayHDD
    CLEAR_MASK(u4PeriSTS5,BayHDDdetached);
    CLEAR_MASK(TamperSTS, b0BayHDDReset);
#endif //Support_Chk_BayHDD    
      
//SSD      
#if Support_Chk_mSATA
    CLEAR_MASK(TamperSTS, b0mSataReset); 
#endif //Support_Chk_mSATA

//HDD    
    CLEAR_MASK(u4PeriSTS5,b4HDDdetached);
    CLEAR_MASK(TamperSTS, b0HDDReset);
  }

//check device whether be changed
  if (SystemIsS3){
    if (Get_tamper_status()){   //device be removed ?
//BayHDD
#if Support_Chk_BayHDD
      if ((Tamper_Dev_Ste & (Tamper_BayHDD + (Tamper_BayHDD << 4))) == (Tamper_BayHDD << 4)){    // remvoe BayHDD
        SET_MASK(TamperSTS, b0BayHDDReset);
        SET_MASK(u4PeriSTS5,BayHDDdetached);
      }
#endif //Support_Chk_BayHDD

//SSD   
#if Support_Chk_mSATA
      if ((Tamper_Dev_Ste & (Tamper_SSD + (Tamper_SSD << 4))) == (Tamper_SSD << 4)){    // remvoe SSD
        SET_MASK(TamperSTS, b0mSataReset);
      }
#endif //Support_Chk_mSATA   

//HDD   
      if ((Tamper_Dev_Ste & (Tamper_HDD + (Tamper_HDD << 4))) == (Tamper_HDD << 4)){    // remove HDD
        SET_MASK(TamperSTS, b0HDDReset);
        SET_MASK(u4PeriSTS5,b4HDDdetached);
      }
    }
  }

//update device status to name space
//HDD
  if( Tamper_Dev_Ste & Tamper_HDD )
    { SET_MASK(u4SenseSTS2,b6HDDdet); }  // Enable HDD.
  else
    { CLEAR_MASK(u4SenseSTS2,b6HDDdet); }  // Disable HDD.
    
//SSD    
#if Support_Chk_mSATA
  if( Tamper_Dev_Ste & Tamper_SSD )
    { SET_MASK(u4SenseSTS1,b6mSatadet); }  // Enable mSATA.
  else
    { CLEAR_MASK(u4SenseSTS1,b6mSatadet); }  // Disable mSATA.
#endif //Support_Chk_mSATA

//BayHDD
#if Support_Chk_BayHDD
  if( Tamper_Dev_Ste & Tamper_BayHDD )
    { SET_MASK(u4SenseSTS1,HDDisinthebay); }  // Enable Bay HDD.
  else
    { CLEAR_MASK(u4SenseSTS1,HDDisinthebay); }  // Disable Bay HDD.
#endif //Support_Chk_BayHDD
}
#endif //Support_Tamper_function