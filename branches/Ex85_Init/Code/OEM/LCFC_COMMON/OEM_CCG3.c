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
* Filename: OEM_CCG3.C
* 
* Description:
*    CYPD2122(CCG2) Cypress Type-C controller process function.
* 
* History:
*   01/15/2016    Isaac Lin
*     created as new
********************************************************************************
*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#if (Support_TYPE_C == CCG3)

/*
*******************************************************************************
*       Local Macros
*******************************************************************************
*/
//extern static USB_POWER_STATUS      Usb_Pdc_Power_Status[NUMBER_OF_USB_PDC];
//extern static USB_STATUS            Usb_Pdc_Status[NUMBER_OF_USB_PDC];

/*
*******************************************************************************
*       Local Data Type
*******************************************************************************
*/

enum __TYPEC_DETECTION {
    GET_EVENT_NUM = 0,
    DEASSERT_INTR_PIN,
    CHECK_DEVICE_MODE,
    SETTING_EVENT_MASK,
    CHECK_ADAPTOR_EXIST_DELAY,
    CHECK_ADAPTOR_EXIST,
    DETECT_CURRENT_PDO,
    ADAPTOR_DETECT_FINISH,
    ERROR_STATE,
};

/*
*******************************************************************************
*       Local Variable
*******************************************************************************
*/

/*
*******************************************************************************
*       Externals
*******************************************************************************
*/

/*
*******************************************************************************
*       Local Function Prototypes
*******************************************************************************
*/

/*
*******************************************************************************
* Function name: OEM_TYPE_C_ADPID_HOOK           
*
* Descriptoin: 
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_ADPID_HOOK(void)
{
	if(Read_AC_IN())
	{
		if((Read_DCIN_ATTACHED2() != IS_MASK_SET(TypeCStatus,OldDcinAttached2))
			&& (Read_DCIN_ATTACHED2() || (!Read_DCIN_ATTACHED2() && IS_MASK_SET(TypeCStatus,TypeCAdpReady)))
			)
		{	//If DCIN_ATTACHED2 status change,that means adaptor ID need re-setting.
			if(Read_DCIN_ATTACHED2())
			{
				SET_MASK(TypeCStatus,OldDcinAttached2);
				SendPowerModeChangeToPdc();
			}
			else
			{
				CLEAR_MASK(TypeCStatus,OldDcinAttached2);
			}
			ADPID_ON_EN();
			CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
		}
	}

}


/*
*******************************************************************************
* Function name: DetectAdaptorWatt           
*
* Descriptoin: Get the currend PDO and calculate current adaptor output watt
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void DetectAdaptorWatt(void)
{
	if(TypeCAdpPdStableCheck != 0)
	{
		I2C_DATA = PD_STATUS;
		I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
		if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
		{
			TypeCCommFailCnt++;
			TypeCAdpPdStableCheck--;
		}
		else
		{
			TypeCCommFailCnt = 0;
			if(IS_MASK_SET(SMB3_DATA[1],CONTRACT_STATE))
			{
				TypeCAdpPdStableCheck = 0;
				I2C_DATA = CURRENT_PDO;
				I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x04,&SMB3_DATA, 0) == FALSE)
				{
					TypeCCommFailCnt++;
				}
				else
				{
					TypeCCommFailCnt = 0;
					CurrentTypeCV = ((SMB3_DATA[2]&0x0F)<<6)+(SMB3_DATA[1]>>2);	//Unit:50mV
					CurrentTypeCI = ((SMB3_DATA[1]&0x03)<<8)+SMB3_DATA[0];			//Unit:10mA
					CurrentTypeCWatt = (XBYTE)((LWORD)(CurrentTypeCV)*CurrentTypeCI/2000);	//Watt=(V*50mV)*(I*10mA)=(V*50/1000)*(I*10/1000)=V*I/2000
					if( IS_MASK_SET(cCheatAD,cCAD_TypeCADPID) )
        			{
            			CurrentTypeCWatt = cCADP_ID&0x00FF;
        			}
					if(CurrentTypeCWatt >= 230)
					{
						TypeCAdpID = AdapterID_230W;
					}
					else if(CurrentTypeCWatt >= 170)
					{
						TypeCAdpID = AdapterID_170W;
					}
					else if(CurrentTypeCWatt >= 135)
					{
						TypeCAdpID = AdapterID_135W;
					}
					else if(CurrentTypeCWatt >= 90)
					{
						TypeCAdpID = AdapterID_90W;
					}
					else if(CurrentTypeCWatt >= 65)
					{
						TypeCAdpID = AdapterID_65W;
					}				
					else if(CurrentTypeCWatt >= 60)
					{
						TypeCAdpID = AdapterID_60W;
					}
					else if(CurrentTypeCWatt >= 45)
					{
						TypeCAdpID = AdapterID_45W;
					}
					else if(CurrentTypeCWatt >= 36)
					{
						TypeCAdpID = AdapterID_36W;
					}
					else if(CurrentTypeCWatt > 0)
					{
						TypeCAdpID = AdapterID_Lower36W;
					}
					else
					{
						TypeCAdpID = AdapterID_NON_SUPPORT;
					}
				//Re-setting adaptor parameter
				ADPID_ON_EN();
				CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
				SET_MASK(TypeCStatus,TypeCAdpReady);
				SET_MASK(ACsourceInforL,TypeC_ADP);
				SET_MASK(BattAuthStat,TypeC4APPpass);
				}
			}
		}
	}
}

/*
*******************************************************************************
*       Global Function Declarations
*******************************************************************************
*/

/*
*******************************************************************************
* Function name: ECResetCCG2
*
* Descriptoin: This is to reset CCG2 by EC command.                   
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void ECResetCCG3(void) {
	if(IS_MASK_CLEAR(PDStatus,TypeCFwUpdating))
	{
		I2C_DATA = TYPEC_RESET;
		I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
		I2C_DATA1[1] = 0x52;
		I2C_DATA1[2] = 0x01;
		if(I2C_Write(SMbusChA, TypeC_Addr, 0x04,&I2C_DATA)==FALSE)
	  {
	    CHGIC_SMbusFailCnt++;
	  }
	  else
	  {
	    CHGIC_SMbusFailCnt = 0;
	  }
	}
}
/*
*******************************************************************************
* Function name: InitialCCG2
*
* Descriptoin: This is to detect CCG2 in ready and initial CCG2 to auto mode.                   
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void InitialCCG3(void) {
	BYTE TempRomData;

	if(TypeCIniDelay == 0)
	{
		switch(TypeCProcessStep)
		{
	    case GET_EVENT_NUM:
		  	//Get Event number
				if(Read_TYPEC_INT())
				{
					if((PDStatus & 0x03) == 0)
					{
						//Detect INTR type
						I2C_DATA = INTR_REG;
						I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
						if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
						{
							TypeCCommFailCnt++;
						}
						else
						{
							PDStatus = PDStatus | (WSMbusTemp01 & 0x07);	//Store current DEV_INTR and PORT0_INTR and PORT1_INTR status.
							TypeCCommFailCnt = 0;
						}
				  }
				  else
				  {
				  	if(IS_MASK_SET(PDStatus,CurrentDEV_INTR))
				  	{
							I2C_DATA = RESPONSE_REGISTER;
							I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
							if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x02,&WSMbusTemp01, 0) == FALSE)
							{
								TypeCCommFailCnt++;
								if(TypeCCommFailCnt == 3)
								{
				   		            TypeCProcessStep= ERROR_STATE;
						    	    TypeCCommFailCnt = 0;
								}
							}
							else
							{
								TypeCCommFailCnt = 0;
								TypeCEventNum = WSMbusTemp01;
								I2CRBDataCount = WSMbusTemp02;
					      //if(TypeCEventNum == 0x80)
					      {
					      	TypeCProcessStep++;
					      }
					    }
					  }
					  else
					  {
							I2C_DATA = PORT0_RESPONSE_REGISTER & 0x00FF;
							I2C_DATA1[0] = PORT0_RESPONSE_REGISTER >> 8;
							if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
							{
								TypeCCommFailCnt++;
								if(TypeCCommFailCnt == 3)
								{
				   		            TypeCProcessStep= ERROR_STATE;
						    	    TypeCCommFailCnt = 0;
								}
							}
							else
							{
								TypeCCommFailCnt = 0;
								TypeCEventNum = SMB3_DATA[0];
								I2CRBDataCount = (SMB3_DATA[03]<<8)+SMB3_DATA[2];
					      //if(TypeCEventNum == 0x80)
					      {
					      	TypeCProcessStep++;
					      }
							}
					  }
				  }
			  }
			  else
			  {
			    //Confirm power on by dead battery or just EC power be cut(Type-C IC power no be cut)
					I2C_DATA = PD_STATUS;
					I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
//			  	RamDebug(0xC0);
					if(I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
					{					
						TypeCCommFailCnt++;
						if(TypeCCommFailCnt == 3)
						{
				   		   TypeCProcessStep= ERROR_STATE;
				    	   TypeCCommFailCnt = 0;
						}
					}else{
//			  	RamDebug(0xC1);
						if(IS_MASK_SET(SMB3_DATA[1],CONTRACT_STATE))
						{
							TypeCProcessStep = 2;	//The Step jump to "CHECK_DEVICE_MODE";
							//Power team's HW protect design and that will make PCH hangup, need send RSMRST to restart PCH for work around.
							SET_MASK(TypeCStatus2,ClassicAdpDetachShutdown);
			  			//RSMRST_LOW();              //shutdown (remark first)
//			  	RamDebug(0xC2);
						}
							//SET_MASK(TypeCStatus2,ClassicAdpDetachShutdown);
			  			//RSMRST_LOW();              //shutdown (remark first)
					}
			  }
	      break;
	    case DEASSERT_INTR_PIN:
      	//To de-assert INTR pin to high.
				WSMbusTemp01 = 0x01;
				I2C_DATA = INTR_REG;
				I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				I2C_DATA1[1] = 0x00;
				if(IS_MASK_SET(PDStatus,CurrentDEV_INTR))
				{
					I2C_DATA1[1] = CurrentDEV_INTR;
					CLEAR_MASK(PDStatus,CurrentDEV_INTR);
				}
				else if(IS_MASK_SET(PDStatus,CurrentPORT0_INTR))
				{
					I2C_DATA1[1] = CurrentPORT0_INTR;
					CLEAR_MASK(PDStatus,CurrentPORT0_INTR);
				}
				if(I2C_Write(SMbusChA, TypeC_Addr, 0x03,&I2C_DATA)==FALSE)
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
					TypeCCommFailCnt = 0;
					if(TypeCEventNum == 0x80)
					{
		      	TypeCProcessStep++;
		      }
		      else
		      {
		      	TypeCProcessStep = GET_EVENT_NUM;
		      }
		    }
	      break;
	    case CHECK_DEVICE_MODE:
	    	//Check Device mode is normal mode
				I2C_DATA = DEVICE_MODE;
				I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
				  TypeCCommFailCnt = 0;
  				if((WSMbusTemp01& 0x03) > 0)
  				{
      			TypeCProcessStep++;
				  }
				}
	      break;
	    case SETTING_EVENT_MASK:
	    	//Setting EVENT mask
				I2C_DATA = EVENT_MASK;
				I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
				I2C_DATA1[1] = 0xFF;//0x98;
				I2C_DATA1[2] = 0xF9;//0x01;
				I2C_DATA1[3] = 0xFE;//0x00;
				I2C_DATA1[4] = 0xFF;//0x00;
				if(I2C_Write(SMbusChA, TypeC_Addr, 0x06,&I2C_DATA)==FALSE)
			  {
  				TypeCCommFailCnt++;
  			}
  			else
  			{
					TypeCCommFailCnt = 0;
					I2C_DATA = PD_CONTROL;
					I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
					I2C_DATA1[1] = 0x10;
					if(I2C_Write(SMbusChA, TypeC_Addr, 0x03,&I2C_DATA)==FALSE)
				  {
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
  					TypeCCommFailCnt = 0;
						TypeCProcessStep++;
		      }
				}
	      break;
	    case CHECK_ADAPTOR_EXIST_DELAY:
	    	TypeCIniDelay = 2;
	    	TypeCProcessStep++;
	      break;
	    case CHECK_ADAPTOR_EXIST:
				I2C_DATA = TYPE_C_STATUS;
				I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
  				TypeCCommFailCnt = 0;
  				if(((WSMbusTemp01&0x1C) >> 2) == 0x02)
  				{
	      		SET_MASK(TypeCStatus,TypeCAdpExist);
  				}
					TypeCProcessStep++;
		    }
	      break;
	    case DETECT_CURRENT_PDO:
	    	//Detect current PDO(Current adaptor output watt)
    		if(IS_MASK_SET(TypeCStatus,TypeCAdpExist))
    		{
	  			TypeCAdpPdStableCheck = 3;
					DetectAdaptorWatt();
//					if(TypeCCommFailCnt == 0)
//					{
						TypeCProcessStep++;
//					}
    		}
    		else
    		{
					TypeCProcessStep++;
    		}
	      break;
	    case ADAPTOR_DETECT_FINISH:
				//Initial finish
	      SET_MASK(TypeCStatus,TypeCIniOK);
        TempRomData = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_TYPEC_STATUS);
        if((TempRomData & 0x01) == 0x01)
        {
	        eEEPROMAddrsss = EEPROM_TYPEC_STATUS;
	        eEEPROMBank = EEPROM_Bank7;
	        eEEPROMData  = TempRomData & 0xFE;	//Clear bit0:Type-C chip exist
            SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
  			Update_EEPROMMark();
            CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
        }
		    TypeCProcessStep = 0;	//Clear for new process flow step restart.
	      break;
		case ERROR_STATE:  

	      break;
	    default:
		    TypeCProcessStep = 0;	//Clear for new process flow step restart.
	      break;
	  }
  }
  else
  {
  	TypeCIniDelay--;
  }
}

/*
*******************************************************************************
* Function name: CCG2Process           
*
* Descriptoin: This is invoked when the system is powered on                  
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void CCG3Process(void) {
BYTE	Temp;
	if(Read_TYPEC_INT())
	{
		if((PDStatus & 0x03) == 0)
		{
			//Detect INTR type
			I2C_DATA = INTR_REG;
			I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
			if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
			{
				TypeCCommFailCnt++;
			}
			else
			{
				PDStatus = PDStatus | (WSMbusTemp01 & 0x07);	//Store current DEV_INTR and PORT0_INTR and PORT1_INTR status.
				TypeCCommFailCnt = 0;
			}
	  }
	  else
	  {
	  	//Get Event number
	  	if(IS_MASK_SET(PDStatus,CurrentDEV_INTR))
	  	{
				I2C_DATA = RESPONSE_REGISTER;
				I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x02,&WSMbusTemp01, 0) == FALSE)
				{
					TypeCCommFailCnt++;
					if(TypeCCommFailCnt == 3)
					{
			    	TypeCIniDelay = 200;
			    	TypeCCommFailCnt = 0;
					}
				}
				else
				{
					TypeCCommFailCnt = 0;
					TypeCEventNum = WSMbusTemp01;
					I2CRBDataCount = WSMbusTemp02;
		    }
	  	}
	  	else
	  	{
				I2C_DATA = PORT0_RESPONSE_REGISTER & 0x00FF;
				I2C_DATA1[0] = PORT0_RESPONSE_REGISTER >> 8;
				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
				{
					TypeCCommFailCnt++;
					if(TypeCCommFailCnt == 3)
					{
			    	TypeCIniDelay = 200;
			    	TypeCCommFailCnt = 0;
					}
				}
				else
				{
					TypeCCommFailCnt = 0;
					TypeCEventNum = SMB3_DATA[0];
					I2CRBDataCount = (SMB3_DATA[03]<<8)+SMB3_DATA[2];
				}
			}
    	//Process Event
			switch(TypeCEventNum)
			{
	      case CMD_SUCCESS:
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case RESET_COMPLETE:
	      	// Type-C need re-initial after Type-C IC power(Valw) once off.
	  			OEM_TYPE_C_init_hook();
		      //SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case PORT_CONNECT_DETECTED:
					I2C_DATA = TYPE_C_STATUS;
					I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  				if(((WSMbusTemp01&TYPR_OF_DEVICE_ATTACHED_MASK) >> TYPR_OF_DEVICE_ATTACHED_SHIFT) == STATUS_CONN_STATE_SOURCE)
	  				{
		      		SET_MASK(TypeCStatus,TypeCAdpExist);
	  				}
			    }
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case PORT_DISCONNECT_DETECTED:
	      	if(IS_MASK_SET(TypeCStatus,TypeCAdpExist))
	      	{
			      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
			      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
			      TypeCAdpID = 0;
			      CurrentTypeCV = 0;
			      CurrentTypeCI = 0;
			      CurrentTypeCWatt = 0;
    				ADPI_MUL = ClassicAdaptor_ADPI_MUL;	//
			    }
			  CLEAR_MASK(TypeCStatus2,TypeC_Docking_Attached);
			  CLEAR_MASK(ACsourceInforL,TypeC_ADP);
			  CLEAR_MASK(BattAuthStat,TypeC4APPpass);
			    UsbPdcGetPowerStatus(0);
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case SWAP_STATUS:
	      	if(IS_MASK_CLEAR(TypeCStatus2,PdcRequestDone))
	      	{
						I2C_DATA = SWAP_RESPONSE;
						I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
						if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
		  			{
		  				TypeCCommFailCnt++;
		  			}
		  			else
		  			{
		  				TypeCCommFailCnt = 0;
		  				if(WSMbusTemp01 & (DR_SWAP_RESPONSE_RESULT_MASK | PR_SWAP_RESPONSE_RESULT_MASK))
		  				{
		  					if(WSMbusTemp01 & DR_SWAP_RESPONSE_RESULT_MASK)
		  					{
		  						Temp = WSMbusTemp01 & DR_SWAP_RESPONSE_RESULT_MASK;
		  					}
		  					else
		  					{
		  						Temp = (WSMbusTemp01 & PR_SWAP_RESPONSE_RESULT_MASK) >> 2;
		  					}
								switch(Temp)
								{
						      case SWAP_RESPONSE_ACCEPT:
	      						SET_MASK(TypeCStatus2,PdcRequestDone);
						        break;
						      case SWAP_RESPONSE_REJECT:
	      						SET_MASK(TypeCStatus2,PdcRequestDone);
						        break;
						      case SWAP_RESPONSE_WAIT:
						        break;
						      case SWAP_RESPONSE_NOT_EXPECT:
						      default:
	      						SET_MASK(TypeCStatus2,PdcRequestDone);
						        break;
						    }
						    UsbPdcGetPowerStatus(0);
						   }
						   else
						   {
	      					SET_MASK(TypeCStatus2,PdcRequestDone);
						   }
		  			}
			    }
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case ACCEPT_MESSAGE_RECEIVED:
	      	if((Pdc_Data == SEND_HARD_RESET) || 
	      		 (Pdc_Data == SEND_SOFT_RESET) || 
	      		 (Pdc_Data == READ_SRC_PDO) || 
	      		 (Pdc_Data == READ_SINK_PDO)
	      		)
	      	{
          	SET_MASK(TypeCStatus2,PdcRequestDone);
	      	}
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case REJECT_MESSAGE_RECEIVED:
	      	if((Pdc_Data == SEND_HARD_RESET) || 
	      		 (Pdc_Data == SEND_SOFT_RESET) || 
	      		 (Pdc_Data == READ_SRC_PDO) || 
	      		 (Pdc_Data == READ_SINK_PDO)
	      		)
	      	{
          	SET_MASK(TypeCStatus2,PdcRequestDone);
	      	}
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case CONTRACT_INFO:
					I2C_DATA = PORT0_READ_DATA_AREA & 0x00FF;
					I2C_DATA1[0] = PORT0_READ_DATA_AREA >> 8;
					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE)
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  			}
	  			TypeCAdpPdStableCheck = 3;
//					I2C_DATA = PD_STATUS;
//					I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
//					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE)
//	  			{
//	  				TypeCCommFailCnt++;
//	  				TypeCAdpPdStableCheck = 3;
//	  			}
//	  			else
//	  			{
//	  				TypeCCommFailCnt = 0;
//	  				TypeCAdpPdStableCheck = 1;
//	  			}
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case VDM_RECEIVED:
	      	if((Pdc_Data == RETRIEVE_SOURCE_CAPABILITIES) || 
	      		 (Pdc_Data == RETRIEVE_SINK_CAPABILITIES) || 
	      		 (Pdc_Data == READ_SRC_PDO) || 
	      		 (Pdc_Data == READ_SINK_PDO)
	      		)
	      	{
						I2C_DATA = PORT0_READ_DATA_AREA & 0x00FF;
						I2C_DATA1[0] = PORT0_READ_DATA_AREA >> 8;
						if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE)
		  			{
		  				TypeCCommFailCnt++;
		  			}
		  			else
		  			{
		  				TypeCCommFailCnt = 0;
		  				for(Temp=0;Temp<28;Temp++)
		  				{
		  					Ucsi_message_in[Temp] = SMB3_DATA[Temp+4];
		  				}
		  			}
		  		}
					I2C_DATA = PORT0_READ_DATA_AREA & 0x00FF;
					I2C_DATA1[0] = PORT0_READ_DATA_AREA >> 8;
					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE)
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  				//The Byte0~3 is CCG3's "Header", Device's VDM infromation beging from Byte4
	  				if((SMB3_DATA[7] == 0x17) && (SMB3_DATA[6] == 0xEF))	//Lenovo's USB-IF ID is "17EFh"
	  				{
	  					OldDockingACKStatus = SMB3_DATA[8];
	  					OldDockingEvent = SMB3_DATA[11];
	  					OldVDO2[0] = SMB3_DATA[12];
	  					OldVDO2[1] = SMB3_DATA[13];
	  					OldVDO2[2] = SMB3_DATA[14];
	  					OldVDO2[3] = SMB3_DATA[15];
	  					if((SMB3_DATA[4]& 0x1F) == 0x10)
	  					{
	  						if(SMB3_DATA[11] & TYPE_C_DOCKING_EVENT)
	  						{	// Is WOL/Button action?
	  							if(SMB3_DATA[11] & TYPE_C_DOCKING_EVENT_BUTTON_WOL)
	  							{	// Is WOL action?
	  								if(IS_MASK_SET(u4Cfgspace2,DockingWOLEn))
	  								{
	  									SET_MASK(TypeCStatus2,DockingEvent);
	  									LanWakeLow();
	  								}
	  							}
	  							else if(SMB3_DATA[11] & TYPE_C_DOCKING_EVENT_BUTTON_MAKE)
	  							{	// Is Button Make action?
	  								PSWPressed();
	  							}
	  							else if(SMB3_DATA[11] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK)
	  							{	// Is Button Break action?
	  								PSWReleased();
	  							}
	  						}
	  						
	  						//ACK
//	  						SMB3_DATA[6] = SMB3_DATA[7];
//	  						SMB3_DATA[6] |= 0x01;	//Set system ACK
//	  						for(i=0;i<12;i++)
//	  						{
//	  							I2C_DATA1[i] = SMB3_DATA[i+4];
//	  						}
//	  						SendPdcVdm(12);
	  					}
	  					
	  					if((SMB3_DATA[4] == 0x44) || (SMB3_DATA[4] == 0x06))	//0x44 for User VID Alt mode enter;0x06 for Lenovo Attention VDM
	  					{	//Send "Get Lenovo status" package
	  						SET_MASK(TypeCStatus2,TypeC_Docking_Attached);
	  						SendPowerModeChangeToPdc();
	  					}
	  				}
	  			}
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case SOURCE_CAPABILITIES_MESSAGE_RECEIVED:
					I2C_DATA = PORT0_READ_DATA_AREA & 0x00FF;
					I2C_DATA1[0] = PORT0_READ_DATA_AREA >> 8;
					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE)
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  			}
					//HOCTL2_A &= 0xFD;
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      default:
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	    }
      if(IS_MASK_SET(TypeCStatus,TypeCProcessOK))
      {
      	//To de-assert INTR pin to high.
				WSMbusTemp01 = 0x01;
				I2C_DATA = INTR_REG;
				I2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				I2C_DATA1[1] = 0x00;
				if(IS_MASK_SET(PDStatus,CurrentDEV_INTR))
				{
					//I2C_DATA1[1] = I2C_DATA1[1] | CurrentDEV_INTR;
					I2C_DATA1[1] = 0x01;
					CLEAR_MASK(PDStatus,CurrentDEV_INTR);
				}
				else if(IS_MASK_SET(PDStatus,CurrentPORT0_INTR))
				{
					//I2C_DATA1[1] = I2C_DATA1[1] | CurrentPORT0_INTR;
					I2C_DATA1[1] = 0x02;
					CLEAR_MASK(PDStatus,CurrentPORT0_INTR);
				}
				if(I2C_Write(SMbusChA, TypeC_Addr, 0x03,&I2C_DATA)==FALSE)
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
  				TypeCCommFailCnt = 0;
  				//Process finish      	
		      CLEAR_MASK(TypeCStatus,TypeCProcessOK);
		    }
      }
    }
  }
}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_init_hook           
*
* Descriptoin: Oem_Initialization for TYPE-C                 
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_init_hook(void)
{
	TypeCIniDelay = TypeC_Ini_Delay_time;	//Delay 80ms for type C stable
  TypeCStatus = 0;
  TypeCCommFailCnt = 0;
  TypeCAdpID = 0;
  CurrentTypeCV = 0;
  CurrentTypeCI = 0;
  CurrentTypeCWatt = 0;
  TypeCStatus2 = 0;
	OldDockingACKStatus = 0;
	OldDockingEvent = 0;
	OldVDO2[0] = 0;
	OldVDO2[1] = 0;
	OldVDO2[2] = 0;
	OldVDO2[3] = 0;
}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_HOOK           
*
* Descriptoin: TYPE-C hook for Hook_Timer10msEventA               
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_HOOK(void)
{
	if(IS_MASK_CLEAR(PDStatus,TypeCFwUpdating))
	{
		if(IS_MASK_SET(TypeCStatus,TypeCIniOK))
		{
			CCG3Process();
			DetectAdaptorWatt();
		}
		else
		{
			InitialCCG3();
		}
	}
}


/*
*******************************************************************************
* Function name: OEM_TYPE_C_S3_HOOK           
*
* Descriptoin: TYPE-C hook for S3            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S3_HOOK(void)
{
   CLEAR_MASK(PDStatus,TypeCFwUpdating);
}


/*
*******************************************************************************
* Function name: OEM_TYPE_C_S5_HOOK           
*
* Descriptoin: TYPE-C hook for S5            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S5_HOOK(void)
{
   CLEAR_MASK(PDStatus,TypeCFwUpdating);
}

/*
*******************************************************************************
* Function name: SendPdcVdm           
*
* Descriptoin: Send VDM message to PDC            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void SendPdcVdm(BYTE DataCnt)
{
	if(IS_MASK_SET(PDStatus,TypeCFwUpdating))
	{
		return;
	}
	I2C_DATA = PORT0_WRITE_DATA_AREA & 0x00FF;
	I2C_DATA1[0] = PORT0_WRITE_DATA_AREA >> 8;
	if(I2C_Write(SMbusChA, TypeC_Addr, (DataCnt+2),&I2C_DATA)==FALSE)
	{
		TypeCCommFailCnt++;
	}
	else
	{
		TypeCCommFailCnt = 0;
		I2C_DATA = VDM_CONTROL;
		I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
		I2C_DATA1[1] = 0x00;
		I2C_DATA1[2] = 0x0C;
		if(I2C_Write(SMbusChA, TypeC_Addr, 0x04,&I2C_DATA)==FALSE)
	  {
	    CHGIC_SMbusFailCnt++;
	  }
	  else
	  {
	    CHGIC_SMbusFailCnt = 0;
	  }
	}
}

/*
*******************************************************************************
* Function name: SendPowerModeChangeToPdc           
*
* Descriptoin: Send power mode change to PDC            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void SendPowerModeChangeToPdc(void)
{
	BYTE i;
	if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
	|| IS_MASK_CLEAR(TypeCStatus2,TypeC_Docking_Attached)
	|| IS_MASK_SET(PDStatus,TypeCFwUpdating)
	)
	{
		return;
	}
	I2C_DATA = PORT0_WRITE_DATA_AREA & 0x00FF;
	I2C_DATA1[0] = PORT0_WRITE_DATA_AREA >> 8;
	for(i=0;i<16;i++)
	{
		I2C_DATA1[i+1] = 0;
	}
	I2C_DATA1[5] = OldDockingACKStatus;
	I2C_DATA1[5] &= 0xFE;	//Clear Docking ACK flag
	I2C_DATA1[8] = OldDockingEvent;
	I2C_DATA1[7] = OldDockingEvent;
	I2C_DATA1[7] |= 0x01;	//Set System ACK flag
	I2C_DATA1[9] = OldVDO2[0];
	I2C_DATA1[10] = OldVDO2[1];
	I2C_DATA1[11] = OldVDO2[2];
	I2C_DATA1[12] = OldVDO2[3];
	
	//Header
	I2C_DATA1[1] = 0x10;
	I2C_DATA1[2] = 0xA0;
	I2C_DATA1[3] = 0xEF;
	I2C_DATA1[4] = 0x17;
	
  switch (SysPowState)
  {	//Set power status
    case SYSTEM_S4 :
    case SYSTEM_S5 :
			I2C_DATA1[6] |= (0x05 << 1);
      break;
    case SYSTEM_S3 :
			I2C_DATA1[6] |= (0x03 << 1);
      break;
    case SYSTEM_S0 :
			I2C_DATA1[6] |= (0x01 << 1);
      break;
    default :
      break;
  }
  if(ACPOWER_ON)
  {	//Set power mode
		I2C_DATA1[6] |= (0x01 << 4); 
  }
	I2C_DATA1[6] |= 0x01;	//Set power status change
	SendPdcVdm(12);
}
/*
*******************************************************************************
* Function name: UsbPdcGetPowerStatus
*
* Descriptoin: 
*          This is used to get power status.
* 
* Arguments:
*       int usb_pdc_id
* Return Values:
*       USB_POWER_STATUS 
*******************************************************************************
*/

void UsbPdcGetPowerStatus(BYTE usb_pdc_id) {
//    USB_POWER_STATUS rc;
		BYTE Temp,StatusTemp;
		USB_POWER_STATUS Temp2;
		USB_STATUS	Temp3;
    BYTE Type_C_Status_Data;

		Temp2 = Usb_Pdc_Power_Status[usb_pdc_id];
		Temp3 = Usb_Pdc_Status[usb_pdc_id];
						
    Usb_Pdc_Power_Status[usb_pdc_id].power_connection = FALSE;
    StatusTemp = 0;
    if (NUMBER_OF_USB_PDC > usb_pdc_id) {
        if (IS_MASK_SET(TypeCStatus,TypeCIniOK)) {
RamDebug(0xB0);
				I2C_DATA = TYPE_C_STATUS;
				I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&SMB3_DATA[0], 0) == FALSE)
				{
					TypeCCommFailCnt++;
				}
				else
				{
					TypeCCommFailCnt = 0;
					Type_C_Status_Data = SMB3_DATA[0];
				}
        	
				I2C_DATA = PD_STATUS;
				I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
					{
						TypeCCommFailCnt++;
					}
					else
					{
RamDebug(0xB1);
						TypeCCommFailCnt = 0;
						Temp = (SMB3_DATA[0] & TYPR_OF_DEVICE_ATTACHED_MASK) >> TYPR_OF_DEVICE_ATTACHED_SHIFT;
RamDebug(SMB3_DATA[0]);
RamDebug(Temp);
	          if (( Temp == STATUS_CONN_STATE_POWER)
	          		|| (Temp == STATUS_CONN_STATE_SINK)
	          		|| (Temp == STATUS_CONN_STATE_SOURCE)
	          	 )
	          	 {
RamDebug(0xB2);
	              /*
	              *    there is power connection
	              */
	              Usb_Pdc_Power_Status[usb_pdc_id].power_connection = TRUE;
	                  /*
	                  *    Get Type-C current
	                  */
	              Usb_Pdc_Power_Status[usb_pdc_id].bc12_status = USB_BC12_NONE;
RamDebug(Usb_Pdc_Power_Status[usb_pdc_id].power_connection);
RamDebug(Usb_Pdc_Power_Status[usb_pdc_id].bc12_status);
	              switch ((SMB3_DATA[0]&0xC0) >> 6) {
	                  case POWER_STATUS_TYPE_C_CURRENT_DEFAULT: 
	                      Usb_Pdc_Power_Status[usb_pdc_id].type_c_current = USB_TYPE_C_CURRENT_DEFAULT;
	                      break;
	                  case POWER_STATUS_TYPE_C_CURRENT_1_5_A: 
	                      Usb_Pdc_Power_Status[usb_pdc_id].type_c_current = USB_TYPE_C_CURRENT_1_5_A;
	                      break;
	                  case POWER_STATUS_TYPE_C_CURRENT_3_0_A:  
	                      Usb_Pdc_Power_Status[usb_pdc_id].type_c_current = USB_TYPE_C_CURRENT_3_0_A;
	                      break;
	                  case POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT:
	                      Usb_Pdc_Power_Status[usb_pdc_id].type_c_current = USB_TYPE_C_CURRENT_PD_CONTRACT;
	                      break;
	                  default:
	                      Usb_Pdc_Power_Status[usb_pdc_id].type_c_current = USB_TYPE_C_CURRENT_DEFAULT;
	              }
RamDebug(Usb_Pdc_Power_Status[usb_pdc_id].type_c_current);
	              /*
	              *    Get Source/Sink
	              */
	              if(Temp == STATUS_CONN_STATE_SOURCE)
	              {
RamDebug(0xB3);
	                  Usb_Pdc_Power_Status[usb_pdc_id].power_source = TRUE;
	              } else {
	                  Usb_Pdc_Power_Status[usb_pdc_id].power_source = FALSE;
	              }
RamDebug(Usb_Pdc_Power_Status[usb_pdc_id].power_source);
	          }
RamDebug(0xB4);
RamDebug(Type_C_Status_Data);
	          if (Type_C_Status_Data & PORT_PARTNER_CONNECTION_STATUS) {
	              /*
	              *    A device is connected
	              */
	              Usb_Pdc_Status[usb_pdc_id].device_connected = TRUE;
	              /*
	              *    Set Data Role
	              */
	              if (SMB3_DATA[0]&CURRENT_PORT_DATA_ROLE) {
RamDebug(0xB5);
	                  Usb_Pdc_Status[usb_pdc_id].dfp = TRUE;
	              } else {
	                  Usb_Pdc_Status[usb_pdc_id].dfp = FALSE;
	              }
	              /*
	              *    Set connection state
	              */
	              switch ((SMB3_DATA[0]&TYPR_OF_DEVICE_ATTACHED_MASK) >> TYPR_OF_DEVICE_ATTACHED_SHIFT) {
	                  case STATUS_CONN_STATE_AUDIO: 
	                       Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_AUDIO_ACCESSORY;
	                       break;
	                  case STATUS_CONN_STATE_DEBUG:
	                       Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_DEBUG_ACCESSORY;
	                       break;
	                  case STATUS_CONN_STATE_SINK:
	                  case STATUS_CONN_STATE_SOURCE:
	                  case STATUS_CONN_STATE_POWER:
	                  case STATUS_CONN_STATE_Unsupport:
	                       Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_CONNECTED;
	                       break;
	                  default:
	                       Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_NONE;
	              }
	              /*
	              *    Set host state
	              */
//	                switch (data) {
//	                    case TPS65982_STATUS_USB_HOST_PRESENT_PD_NOT_USB:
//	                         Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_PD_NO_USB;
//	                         break;
//	                    case TPS65982_STATUS_USB_HOST_PRESENT_NOT_PD:
//	                         Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_NONE_PD;
//	                         break;
//	                    case TPS65982_STATUS_USB_HOST_PRESENT_PD:
//	                         Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_PD;
//	                         break;
//	                    default:
//	                      Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_NONE;
//	              }
	          }
	          UcsiUsbPdcStatusChanged(Temp2, Temp3);
					}

        }
    }
//    return Usb_Pdc_Power_Status[usb_pdc_id];
}

/*
*******************************************************************************
* Function name: UsbPdcGetStatus
*
* Descriptoin: 
*          This is used to get tatus.
* 
* Arguments:
*       int usb_pdc_id
* Return Values:
*       USB_STATUS 
*******************************************************************************
*/

//void UsbPdcGetStatus(BYTE usb_pdc_id) {
////    USB_STATUS rc;
//    BYTE Type_C_Status_Data;
//
//    Usb_Pdc_Status[usb_pdc_id].device_connected = FALSE;
//    if ((NUMBER_OF_USB_PDC > usb_pdc_id) && (0 <= usb_pdc_id)) {
//        if (IS_MASK_SET(TypeCStatus,TypeCIniOK)) {
//				I2C_DATA = TYPE_C_STATUS;
//				I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
//				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x01,&SMB3_DATA[0], 0) == FALSE)
//				{
//					TypeCCommFailCnt++;
//				}
//				else
//				{
//					TypeCCommFailCnt = 0;
//					Type_C_Status_Data = SMB3_DATA[0];
//				}
//				
//				I2C_DATA = PD_STATUS;
//				I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
//				if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&I2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
//				{
//					TypeCCommFailCnt++;
//				}
//				else
//				{
//					TypeCCommFailCnt = 0;
//				}
//
//            if ((Type_C_Status_Data & 0x01) == 0x01) {
//                /*
//                *    A device is connected
//                */
//                Usb_Pdc_Status[usb_pdc_id].device_connected = TRUE;
//                /*
//                *    Set Data Role
//                */
//                if (SMB3_DATA[0]&0x40) {
//                    Usb_Pdc_Status[usb_pdc_id].dfp = TRUE;
//                } else {
//                    Usb_Pdc_Status[usb_pdc_id].dfp = FALSE;
//                }
//                /*
//                *    Set connection state
//                */
//                switch ((SMB3_DATA[0]&0x1C) >> 2) {
//                    case STATUS_CONN_STATE_AUDIO: 
//                         Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_AUDIO_ACCESSORY;
//                         break;
//                    case STATUS_CONN_STATE_DEBUG:
//                         Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_DEBUG_ACCESSORY;
//                         break;
//                    case STATUS_CONN_STATE_SINK:
//                    case STATUS_CONN_STATE_SOURCE:
//                    case STATUS_CONN_STATE_POWER:
//                    case STATUS_CONN_STATE_Unsupport:
//                         Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_CONNECTED;
//                         break;
//                    default:
//                         Usb_Pdc_Status[usb_pdc_id].connect_status = USB_CONNECT_NONE;
//                }
//                /*
//                *    Set host state
//                */
////                switch (data) {
////                    case TPS65982_STATUS_USB_HOST_PRESENT_PD_NOT_USB:
////                         Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_PD_NO_USB;
////                         break;
////                    case TPS65982_STATUS_USB_HOST_PRESENT_NOT_PD:
////                         Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_NONE_PD;
////                         break;
////                    case TPS65982_STATUS_USB_HOST_PRESENT_PD:
////                         Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_PD;
////                         break;
////                    default:
//                        Usb_Pdc_Status[usb_pdc_id].usb_host = USB_HOST_NONE;
//                //}
//            }
//        }
//    }
////    return Usb_Pdc_Status[usb_pdc_id];
//}

/*
*******************************************************************************
* Function name: UsbPqcRequest
*
* Descriptoin: 
*          This is used to enqueue the request block into USB PD queue
* 
* Arguments:
*       int         usb_pdc_id
*       USB_PDC_REQ *request_packet
*       UCHAR       command
*       USHORT      parm1
*       USHORT      parm2
*       void        *address
*       USB_PD_CALL_BACK *call_back
* Return Values:
*       UCHAR : return code of USB PD     
*******************************************************************************
*/

BYTE UsbPdcRequest(BYTE usb_pdc_id, USB_PDC_REQ *request_packet, BYTE cmd, WORD parm1, WORD parm2,void *address)
{
	BYTE	rc = USB_PDC_RC_OK;
	RamDebug(0xA1);
  if ((0 <= usb_pdc_id) && (NUMBER_OF_USB_PDC > usb_pdc_id)) {
      /*
      *     valid USB PD ID 
      */
//      int_mask = CmnGetIntMask();
//      CmnDisableInt();
	RamDebug(0xA2);
      if (TRUE == request_packet->status.busy) {
          /*
          *   PDC is busy
          */
          rc = USB_PDC_RC_BUSY;
	RamDebug(0xA3);
      }
      if (USB_PDC_RC_OK == rc) {
	RamDebug(0xA4);
	RamDebug(cmd);
          /*
          *   request packet is enqueued then set up the packet
          */
          //request_packet->status.done  = FALSE;
          CLEAR_MASK(TypeCStatus2,PdcRequestDone);
          request_packet->status.busy  = TRUE;
           switch (cmd) {
               case USB_PDC_REQ_HARD_RESET:
               			Pdc_Data = SEND_HARD_RESET;
                    break;
               case USB_PDC_REQ_SOFT_RESET:
               			Pdc_Data = SEND_SOFT_RESET;
                    break;
               case USB_PDC_REQ_SWAP_TO_SINK:
               			if(Usb_Pdc_Power_Status[usb_pdc_id].power_source)
               			{
	               			Pdc_Data = TRIGGER_POWER_ROLE_SWAP;
               			}
               			else
               			{
          						//request_packet->status.done  = TRUE;
          						SET_MASK(TypeCStatus2,PdcRequestDone);
               			}
                    break;
               case USB_PDC_REQ_SWAP_TO_SOURCE:
               			if(Usb_Pdc_Power_Status[usb_pdc_id].power_source)
               			{
          						//request_packet->status.done  = TRUE;
          						SET_MASK(TypeCStatus2,PdcRequestDone);
               			}
               			else
               			{
	               			Pdc_Data = TRIGGER_POWER_ROLE_SWAP;
               			}
                    break;
               case USB_PDC_REQ_SWAP_TO_UFP:
               			if(Usb_Pdc_Status[usb_pdc_id].dfp)
               			{
	               			Pdc_Data = TRIGGER_DATA_ROLE_SWAP;
               			}
               			else
               			{
          						//request_packet->status.done  = TRUE;
          						SET_MASK(TypeCStatus2,PdcRequestDone);
               			}
                    break;
               case USB_PDC_REQ_SWAP_TO_DFP:
               			if(Usb_Pdc_Status[usb_pdc_id].dfp)
               			{
          						//request_packet->status.done  = TRUE;
          						SET_MASK(TypeCStatus2,PdcRequestDone);
               			}
               			else
               			{
	               			Pdc_Data = TRIGGER_DATA_ROLE_SWAP;
               			}
                    break;
               case USB_PDC_REQ_GET_RX_SOURCE_PDO:
               			Pdc_Data = RETRIEVE_SOURCE_CAPABILITIES;
                    break;
               case USB_PDC_REQ_GET_RX_SINK_PDO:
               			Pdc_Data = RETRIEVE_SINK_CAPABILITIES;
                    break;
               case USB_PDC_REQ_GET_TX_SOURCE_PDO:
               			Pdc_Data = READ_SRC_PDO;
                    break;
               case USB_PDC_REQ_GET_TX_SINK_PDO:
               			Pdc_Data = READ_SINK_PDO;
                    break;
           }
          //if(request_packet->status.done == FALSE)
          if(IS_MASK_CLEAR(TypeCStatus2,PdcRequestDone))
          {
	RamDebug(0xA5);
	RamDebug(Pdc_Data);
						I2C_DATA = PD_CONTROL;
						I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
						I2C_DATA1[1] = Pdc_Data;
						if(I2C_Write(SMbusChA, TypeC_Addr, 0x03,&I2C_DATA)==FALSE)
		  			{
		  				TypeCCommFailCnt++;
		  			}
		  			else
		  			{
	RamDebug(0xA6);
							TypeCCommFailCnt = 0;
		           switch (request_packet->request) {
		               case USB_PDC_REQ_HARD_RESET:
		               case USB_PDC_REQ_SOFT_RESET:
		               case USB_PDC_REQ_SWAP_TO_SINK:
		               case USB_PDC_REQ_SWAP_TO_SOURCE:
		               case USB_PDC_REQ_SWAP_TO_UFP:
		               case USB_PDC_REQ_SWAP_TO_DFP:
		                    break;
		               case USB_PDC_REQ_GET_RX_SOURCE_PDO:	//RX:Device;TX:PDC
		               case USB_PDC_REQ_GET_RX_SINK_PDO:
		               case USB_PDC_REQ_GET_TX_SOURCE_PDO:
		               case USB_PDC_REQ_GET_TX_SINK_PDO:
          							//request_packet->status.done  = TRUE;
          							SET_MASK(TypeCStatus2,PdcRequestDone);
		                    break;
		           }
				    }
					}
					else
					{
          	request_packet->status.busy  = FALSE;
					}
          request_packet->request      = cmd;
          request_packet->parameter[0] = parm1;
          request_packet->parameter[1] = parm2;
          request_packet->address      = address;
      }
      //CmnSetIntMask(int_mask);
  } else {
      /*
      *    invalid USB PD ID
      */
      request_packet->status.done = TRUE;
      request_packet->status.busy = FALSE;
      rc                          = USB_PDC_RC_NOT_SUPPORTED;
  }
  if (USB_PDC_RC_OK == rc) {
      //TskActivateTask(TASK_USB_PDC_0 + usb_pdc_id);
  }
  return rc;
}

#endif //Support_TYPE_C

