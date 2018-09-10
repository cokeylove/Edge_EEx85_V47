/*
********************************************************************************
* LCFC Confidential
*
* Microcode Source Materials
*
* ThinkPad Embedded Controller Microcode
*
* COPYRIGHT LCFC 2017 ALL RIGHTS RESERVED
*
* Filename: OEM_RTS5455.C
*
* Description:
*    RTS5455 RealTek Type-C PD controller process function.
*
* History:
*   04/28/2017    Isaac Lin
*     created as new
********************************************************************************
*/

//*****************************************************************************
// Include all header file
//*****************************************************************************
#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#if (Support_TYPE_C == RTS5455)

/*
*******************************************************************************
*       Local Macros
*******************************************************************************
*/


/*
*******************************************************************************
*       Local Data Type
*******************************************************************************
*/

enum __TYPEC_DETECTION {
		RTS5455_INIT_SEND_VENDOR_COMMAND_ENABLE = 0,
		RTS5455_INIT_SEND_VENDOR_COMMAND_ENABLE_WAIT_PING,
		RTS5455_INIT_SEND_EVENT_MASK,
		RTS5455_INIT_SEND_EVENT_MASK_WAIT_PING,
		RTS5455_INIT_DE_ASSERT,
		RTS5455_INIT_SEND_EVENT_CLEAR,
		RTS5455_INIT_SEND_EVENT_CLEAR_WAIT_PING,
		CHECK_POWER_STATUS_GET_PD_STATUS,
		CHECK_POWER_STATUS_GET_PD_STATUS_WAIT_PING,
		CHECK_POWER_STATUS,
    INITIAL_FINISH,
    RTS5455_ERROR_STATE,
};

enum __TYPEC_ADAPTOR_WATT_DETECTION {
		RTS5455_ADAPTOR_WATT_SEND_GET_RDO = 0,
		RTS5455_ADAPTOR_WATT_GET_RDO_WAIT_PING,
		RTS5455_ADAPTOR_WATT_GET_RDO_GOT_DATA,
		RTS5455_ADAPTOR_WATT_SEND_GET_PDO,
		RTS5455_ADAPTOR_WATT_GET_PDO_WAIT_PING,
		RTS5455_ADAPTOR_WATT_GET_PDO_GOT_DATA,
		RTS5455_ADAPTOR_WATT_CALCULATION_WATT,
};

enum __TYPEC_INT_PROCESS {
		RTS5455_INT_DE_ASSERT_INTR_PIN= 0,
		RTS5455_INT_GET_PD_STATUS ,
		RTS5455_INT_GET_PD_STATUS_WAIT_PING,
		RTS5455_INT_GET_PD_STATUS_CHECK_EVENT,
		RTS5455_INT_PROCESS_EVENT,
		RTS5455_INT_CLEAR_EVENT,
		RTS5455_INT_PROCESS_EVENT_WAIT_PING,
		RTS5455_INT_PROCESS_EVENT_PROCESS_DATA,
};

enum __TYPEC_GET_VERSION_PROCESS {
		RTS5455_GET_VERSION_SEND_CMD = 0,
		RTS5455_GET_VERSION_SEND_CMD_WAIT_PING,
		RTS5455_GET_VERSION_PROCESS,
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
	if(Read_DCIN_ATTACHED2() != IS_MASK_SET(TypeCStatus,OldDcinAttached2))
	{
//		if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, TPS65982_SYSTEM_CONFIG, &SMB3_DATA[0]))
//		{
//			TypeCCommFailCnt++;
//		}
//		else
//		{
//		  TypeCCommFailCnt = 0;
//		}
		if(Read_AC_IN()
			&& (Read_DCIN_ATTACHED2() || (!Read_DCIN_ATTACHED2() && IS_MASK_SET(TypeCStatus,TypeCAdpReady)))
			)
		{	//If DCIN_ATTACHED2 status change,that means adaptor ID need re-setting.
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
BYTE Temp,i;
WORD VoltageTemp;
	if(TypeCAdpDetectDelay)
	{
		TypeCAdpDetectDelay--;
		return;
	}
if((UCSI_RTS5455_CMD_WAIT_CNT!=0))// || (Detect_Adaptor_Watt_SKIP == 1))
{
    PDStatus &= 0xC0;
	RTS5455WaitingPingID = 0;
    Adp_retry_cnt = 0;
    return;
}
    if((RTS5455WaitingPingID != 0)&&(RTS5455WaitingPingID != 7))
  			{
  				return;
				}
	if(CurrentTypeCWatt>29)
		{
		TypeCAdpPdStableCheck=0;
        PDStatus &= 0xC0;
        Adp_retry_cnt = 0;
		return;
		}


	if(TypeCAdpPdStableCheck != 0)
	{
	//RamDebug(0XD1);

		switch(PDStatus&0x3F)
		{
	    case RTS5455_ADAPTOR_WATT_SEND_GET_RDO:
  		//	if(RTS5455WaitingPingID == 0)
  			{
					SMB3_DATA[0] = RTS5455_GET_RDO&0x00FF;
					SMB3_DATA[1] = RTS5455_PORT_NUM;
					if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_GET_RDO>>8), &SMB3_DATA[0], RTS5455_GET_RDO_WR_DATA_LEN, SMBus_NoPEC))
					{
						TypeCCommFailCnt++;
						TypeCAdpPdStableCheck--;
						if(TypeCCommFailCnt == 3)
						{
				    	TypeCIniDelay = 200;
				    	TypeCCommFailCnt = 0;
						}
					}
					else
					{
					TypeCAdpPdStableCheck--;
					if(TypeCAdpPdStableCheck==0)
						{RTS5455WaitingPingID = 0;
					PDStatus=0;
						}
					else
						{
						RTS5455WaitingPingID = 7;
						PDStatus++;
						}

						TypeCCommFailCnt = 0;
						//RTS5455WaitingPingID = 7;
						//PDStatus++;
					}
				}
	      break;
	    case RTS5455_ADAPTOR_WATT_GET_RDO_WAIT_PING:
  		//	if(RTS5455WaitingPingID == 7)
  			{
					if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
						{
						//	RTS5455WaitingPingID = 0;
							PDStatus++;
						}
						else
						{
							if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
							{
						//		RTS5455WaitingPingID = 0;
								PDStatus--;	//Retry command
							}
							//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
						}
					}
				}
	      break;
	    case RTS5455_ADAPTOR_WATT_GET_RDO_GOT_DATA:
				if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
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
					CurrentRDO[0] = SMB3_DATA[0];
					CurrentRDO[1] = SMB3_DATA[1];
					CurrentRDO[2] = SMB3_DATA[2];
					CurrentRDO[3] = SMB3_DATA[3];
				//	CurrentTypeCI = ((SMB3_DATA[2]&0x0F)<<6)+(SMB3_DATA[1]>>2);			//Unit:10mA
					CurrentTypeCV = ((SMB3_DATA[3]&0x70)>>4);	//Object position
					ObjectPosition20V = CurrentTypeCV;
					PDStatus++;
				}
	      break;
	    case RTS5455_ADAPTOR_WATT_SEND_GET_PDO:
			//RamDebug(0x42);
			//if(RTS5455WaitingPingID == 0)
				{
				SMB3_DATA[0] = RTS5455_GET_PDO&0x00FF;
				SMB3_DATA[1] = RTS5455_PORT_NUM;
				SMB3_DATA[2] = 0xE3;
				//SMB3_DATA[2] |= (CurrentTypeCV<<5);
				if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_GET_PDO>>8), &SMB3_DATA[0], RTS5455_GET_PDO_WR_DATA_LEN, SMBus_NoPEC))
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
					PDStatus++;
				}
				}
	      break;
	    case RTS5455_ADAPTOR_WATT_GET_PDO_WAIT_PING:
			//RamDebug(0x49);
				if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
					{
						PDStatus++;
					}
					else
					{
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
						{
							PDStatus--;	//Retry command
						}
						//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
					}
				}
	      break;
	    case RTS5455_ADAPTOR_WATT_GET_PDO_GOT_DATA:
				if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
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
				/*RamDebug(0x44);
				RamDebug(SMB3_DATA[0]);
				RamDebug(SMB3_DATA[1]);
				RamDebug(SMB3_DATA[2]);
				RamDebug(SMB3_DATA[3]);
				RamDebug(SMB3_DATA[4]);
				RamDebug(SMB3_DATA[5]);*/
				if(TRTS5455_Dual_Role_Power(SMB3_DATA[0])==Dule_role_power_Dock)//dock
					{
					//RamDebug(0x45);
					SET_MASK(DETECTDOCKORDAPTER,b0_ACORDOCK);
					}
				else//adapter
					{
					//RamDebug(0x46);
					CLEAR_MASK(DETECTDOCKORDAPTER,b0_ACORDOCK);
					}

					TypeCCommFailCnt = 0;
					Temp = (CurrentTypeCV-1) * 4;
					//Temp = (4-1) * 4;
					CurrentTypeCV = ((SMB3_DATA[Temp+2]&0x0F)<<6)+(SMB3_DATA[Temp+1]>>2);	//Unit:50mV
					CurrentTypeCI =((SMB3_DATA[Temp+1]&0x03)<<8)+(SMB3_DATA[Temp+0]);
					for(i=0;i<4;i++)
					{
						VoltageTemp = ((SMB3_DATA[(i*4)+2]&0x0F)<<6)+(SMB3_DATA[(i*4)+1]>>2);	//Unit:50mV
						if(VoltageTemp == 180)	//180*50mV=9000mV=9V
						{
							ObjectPosition09V = i+1;
							PDO09V = VoltageTemp;
							PDO09I = ((SMB3_DATA[(i*4)+1]&0x03)<<8)+SMB3_DATA[(i*4)];	//Unit:10mA
						}
						if(VoltageTemp == 400)	//400*50mV=20000mV=20V
						{
							ObjectPosition20V = i+1;
							PDO20V = VoltageTemp;
							PDO20I = ((SMB3_DATA[(i*4)+1]&0x03)<<8)+SMB3_DATA[(i*4)];	//Unit:10mA
						}
					}
					PDStatus++;
				}
	      break;
	    case RTS5455_ADAPTOR_WATT_CALCULATION_WATT:
				TypeCAdpPdStableCheck = 0;

				CurrentTypeCWatt = (BYTE)((ULONG)(CurrentTypeCV)*CurrentTypeCI/2000);	//Watt=(V*50mV)*(I*10mA)=(V*50/1000)*(I*10/1000)=V*I/2000
					if((CurrentTypeCWatt<30)&&(Adp_retry_cnt<=20))
				{
					if(Adp_retry_cnt == 0)
					{
						RamDebug(0xF4);
						RamDebug(CurrentTypeCWatt);
					}
  				TypeCAdpPdStableCheck = 5;
				RamDebug(0xcc);
	  			TypeCAdpDetectDelay = 10;
	  			Adp_retry_cnt++;
					PDStatus &= 0xC0;
					RTS5455WaitingPingID = 0;
					break;
				}
				if(Adp_retry_cnt)
				{
					Adp_retry_cnt = 0;
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

				PDStatus &= 0xC0;
				//Re-setting adaptor parameter
				ADPID_ON_EN();
				CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
				SET_MASK(TypeCStatus,TypeCAdpReady);
								RTS5455WaitingPingID = 0;
	      break;
	    default:
	      break;
    }
	}
    else
    {
        PDStatus &= 0xC0;
    }
}

/*
*******************************************************************************
*       Global Function Declarations
*******************************************************************************
*/

/*
*******************************************************************************
* Function name: ECResetTypecPD
*
* Descriptoin: This is to reset TypeC PD by EC command.
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void ECResetTypecPD(void) {

//	rI2C_DATA = TYPEC_RESET;
//	rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
//	rI2C_DATA1[1] = 0x52;
//	rI2C_DATA1[2] = 0x01;
//	if(I2C_Write(chSMB_TYPEC, TypeC_Addr, 0x04,&I2C_DATA)==FALSE)
//  {
//    CHGIC_SMbusFailCnt++;
//  }
//  else
//  {
//    CHGIC_SMbusFailCnt = 0;
//  }
}
/*
*******************************************************************************
* Function name: InitialTypecPD
*
* Descriptoin: This is to detect TypeC PD in ready and initial TypeC PD.
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void InitialTypecPD(void) {
	BYTE TempRomData, Temp8;//i,j,MarkTemp[4];

	if(TypeCIniDelay == 0)
	{
		switch(TypeCProcessStep)
		{
	    case RTS5455_INIT_SEND_VENDOR_COMMAND_ENABLE:
				/*
				*    Enable vendor command
				*/
				SMB3_DATA[0] = RTS5455_VENDOR_COMMAND_ENABLE_ID_LL;
				SMB3_DATA[1] = RTS5455_VENDOR_COMMAND_ENABLE_ID_LH;
				SMB3_DATA[2] = RTS5455_VENDOR_COMMAND_ENABLE_ID_HL;
				if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, RTS5455_VENDOR_COMMAND_ENABLE, &SMB3_DATA[0], RTS5455_VENDOR_COMMAND_ENABLE_WR_DATA_LEN, SMBus_NoPEC))
  			{
  				TypeCCommFailCnt++;
					if(TypeCCommFailCnt == 3)
					{
			    	TypeCIniDelay = 200;
			    	//TypeCProcessStep = RTS5455_ERROR_STATE;
			    	TypeCCommFailCnt = 0;
					}
  				}
  				else
  				{
				  	TypeCCommFailCnt = 0;
					RTS5455WaitingPingID = 1;
      				TypeCProcessStep++;
				}
	      break;
	    case RTS5455_INIT_SEND_VENDOR_COMMAND_ENABLE_WAIT_PING:
				if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
					{
						RTS5455WaitingPingID = 0;
	      		TypeCProcessStep++;
					}
					else
					{
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
						{
							RTS5455WaitingPingID = 0;
							TypeCProcessStep--;	//Retry command
						}
						//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
					}
				}
	      break;
	    case RTS5455_INIT_SEND_EVENT_MASK:
				/*
				*    Initialize event interrupt masks
				*/
				SMB3_DATA[0] = RTS5455_SET_NOTIFICATION_ENABLE&0x00FF;
				SMB3_DATA[1] = RTS5455_PORT_NUM;
				SMB3_DATA[2] = RTS5455_INT_EVENT_MASK_LL;
				SMB3_DATA[3] = RTS5455_INT_EVENT_MASK_LH;//BIT6+BIT5+BIT4;
				SMB3_DATA[4] = RTS5455_INT_EVENT_MASK_HL;//BIT4;
				SMB3_DATA[5] = RTS5455_INT_EVENT_MASK_HH;
				if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SET_NOTIFICATION_ENABLE>>8), &SMB3_DATA[0], RTS5455_SET_NOTIFICATION_ENABLE_WR_DATA_LEN, SMBus_NoPEC))
  			{
  				TypeCCommFailCnt++;
					if(TypeCCommFailCnt == 3)
					{
			    	TypeCIniDelay = 200;
			    	//TypeCProcessStep = RTS5455_ERROR_STATE;
			    	TypeCCommFailCnt = 0;
					}
  				}
  				else
 				{
				  	TypeCCommFailCnt = 0;
					RTS5455WaitingPingID = 2;
      			  	TypeCProcessStep++;
				}
	      break;
	    case RTS5455_INIT_SEND_EVENT_MASK_WAIT_PING:
				if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
					{
						RTS5455WaitingPingID = 0;
	      		TypeCProcessStep++;
					}
					else
					{
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
						{
							RTS5455WaitingPingID = 0;
							TypeCProcessStep--;	//Retry command
						}
						//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
					}
				}
	      break;
	    case RTS5455_INIT_DE_ASSERT:
	    	if(Read_TYPEC_INT())
	    	{
					if(!bSMBusReceiveByte(chSMB_TYPEC, RTS5455_DE_ASSERT_I2C_ADDR, &SMB3_DATA[0]))
	  			{
	  				TypeCCommFailCnt++;
						if(TypeCCommFailCnt == 3)
						{
				    	TypeCIniDelay = 200;
				    	//TypeCProcessStep = RTS5455_ERROR_STATE;
				    	TypeCCommFailCnt = 0;
						}
	  			}
	  			else
	  			{
					  TypeCCommFailCnt = 0;
	      		TypeCProcessStep++;
					}
				}
				else
				{
	      	TypeCProcessStep = CHECK_POWER_STATUS_GET_PD_STATUS;
				}
	      break;
	    case RTS5455_INIT_SEND_EVENT_CLEAR:
				/*
				*    Initialize event interrupt masks
				*/
//				SMB3_DATA[0] = RTS5455_ACK_CC_CI & 0x00FF;
//				SMB3_DATA[1] = RTS5455_PORT_NUM;
//				SMB3_DATA[2] = 0xFF;
//				SMB3_DATA[3] = 0xFF;
//				SMB3_DATA[4] = 0xFF;
//				SMB3_DATA[5] = 0xFF;
//				SMB3_DATA[6] = 0x01;	//Set bit0 for command complete ack
//				if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_ACK_CC_CI>>8), &SMB3_DATA[0], RTS5455_ACK_CC_CI_WR_DATA_LEN, SMBus_NoPEC))
//  				{
//  					TypeCCommFailCnt++;
//					if(TypeCCommFailCnt == 3)
//					{
//			    	TypeCIniDelay = 200;
////			    	TypeCProcessStep = RTS5455_ERROR_STATE;
//			    	TypeCCommFailCnt = 0;
//					}
//  				}
//  				else
//  				{
//				  	TypeCCommFailCnt = 0;
//					RTS5455WaitingPingID = 3;
      			  	TypeCProcessStep++;
//				}
	      break;
	    case RTS5455_INIT_SEND_EVENT_CLEAR_WAIT_PING:
				if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
					{
						CLEAR_MASK(TypeCStatus3,RTS5455WaitingPing);
						RTS5455WaitingPingID = 0;
	      		TypeCProcessStep++;
					}
					else
					{
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
						{
							RTS5455WaitingPingID = 0;
							TypeCProcessStep--;	//Retry command
						}
						//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
					}
				}
	      break;
	    case CHECK_POWER_STATUS_GET_PD_STATUS:
				SMB3_DATA[0] = 0x00;
				SMB3_DATA[1] = RTS5455_PORT_NUM;
				SMB3_DATA[2] = 11;	//Bytrs
				if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_GET_STATUS>>8), &SMB3_DATA[0], RTS5455_GET_STATUS_WR_DATA_LEN, SMBus_NoPEC))
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
					RTS5455WaitingPingID = 4;
      				TypeCProcessStep++;
				}
	      break;
	    case CHECK_POWER_STATUS_GET_PD_STATUS_WAIT_PING:
				if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
					{
						RTS5455WaitingPingID = 0;
	      				TypeCProcessStep++;
					}
					else
					{
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
						{
							RTS5455WaitingPingID = 0;
							TypeCProcessStep--;	//Retry command
						}
						//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
					}
				}
	      break;
	    case CHECK_POWER_STATUS:
		  	//Check power status for dead battery
				if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
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
                   /* TypeCEventNum = 0;

					ClearTemp[0] = TypeCSMbusDataTemp[0];
					ClearTemp[1] = TypeCSMbusDataTemp[1];
					ClearTemp[2] = TypeCSMbusDataTemp[2];
					ClearTemp[3] = TypeCSMbusDataTemp[3];
					ClearPdEvents(RTS5455_INT_CLEAR_EVENT);
					EventList[0] |= TypeCSMbusDataTemp[0];
					EventList[1] |= TypeCSMbusDataTemp[1];
					EventList[2] |= TypeCSMbusDataTemp[2];
					EventList[3] |= TypeCSMbusDataTemp[3];
					if((EventList[0]&RTS5455_INT_EVENT_MASK_LL)||
						(EventList[1]&RTS5455_INT_EVENT_MASK_LH)||
						(EventList[2]&RTS5455_INT_EVENT_MASK_HL)||
						(EventList[3]&RTS5455_INT_EVENT_MASK_HH)
						)
					{
	//RamDebug(0xC8);
						SET_MASK(TypeCStatus3,AllEventsNotClear);
					}
					else
					{
	//RamDebug(0xC9);
						CLEAR_MASK(TypeCStatus3,AllEventsNotClear);
					}

					if(TRTS5455_INT_VDM_RECEIVED(EventList[0]))
					{
						EventList[3] &= 0xFD;	//~0x02
						TypeCEventNum        = TRTS5455_INT_VDM_RECEIVED_Num;
					}
					else
					{
						MarkTemp[0] = RTS5455_INT_EVENT_MASK_LL;
						MarkTemp[1] = RTS5455_INT_EVENT_MASK_LH;
						MarkTemp[2] = RTS5455_INT_EVENT_MASK_HL;
						MarkTemp[3] = RTS5455_INT_EVENT_MASK_HH;
						for(i = 0; RTS5455_INT_DATA_LENGTH > i; i++)
						{
						     if ((EventList[i]&MarkTemp[i]) != 0)
						   {
						       for(j = 0; 8 > j; j++)
						       {
						            if (((EventList[i]&MarkTemp[i]) & (1 << j)) != 0)
						           {
													EventList[i] &= ~(0x01 << j);
													TypeCEventNum = (i * 8) + j;
													TypeCProcessStep++;
													return;
						           }
						       }
						   }
						}
					}*/
				if(TRTS5455_LENOVO_LAPTOP_CHECK(SMB3_DATA[0])==TRTS5455_LENOVO_LAPTOP_CHECK_LENOVO_LAPTOP_ATTACHED)
					{
					SET_MASK(LENOVOP2PSTATUS,LENOVOStatusFlag);
					}
				else
					{
					CLEAR_MASK(LENOVOP2PSTATUS,LENOVOStatusFlag);
					}
			// RamDebug(0X32);
		      if(TRTS5455_CONNECT_STATUS(SMB3_DATA[0]) == TRTS5455_CONNECT_STATUS_ATTACHED)
		      {
		        /*
		        *   Connection is present
		        */
			      if(TRTS5455_POWER_DIRECTION(SMB3_DATA[0]) == TRTS5455_POWER_DIRECTION_CONSUMER)
			      {
		          /*
		          *    USB PD act as Sink
		          */
				      SET_MASK(TypeCStatus,TypeCAdpExist);
					  	if(TypeCAdpPdStableCheck == 0)
					  	{
			  				TypeCAdpPdStableCheck = 5;
							RamDebug(0xbb);
			  				TypeCAdpDetectDelay = 0;
							PD_Ini_Stable_Delay = 0XFFF0;
			  			}
							//DetectAdaptorWatt();
							//if(TypeCAdpPdStableCheck == 0)
					  	{
			  				TypeCProcessStep++;
			  			}
			  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
			  			{
								CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
								SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
								CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
							}
				    }
				    else
				    {
			  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
			  			{
								      	//RamDebug(0xDD);
								SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
								CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
								CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
					      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
					      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
								RTS5455WaitingPingID = 0;
					      TypeCAdpID = 0;
					      CurrentTypeCWatt = 0;
					      TypeCAdpPdStableCheck = 0;
					      CurrentTypeCV = 0;
					      CurrentTypeCI = 0;
							}
			  			TypeCProcessStep++;
				    }
				  }
				  else
				  {
			  		TypeCProcessStep++;
				  }
				}
	      break;
	    case INITIAL_FINISH:
				//Initial finish
	      SET_MASK(TypeCStatus,TypeCIniOK);
        TempRomData = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_TYPEC_STATUS);
        if((TempRomData & 0x01) == 0x01)
        {
          Temp8 = TempRomData & 0xFE;	//Clear bit0:Type-C chip exist
          WRITE_DATA2EEPROM(&Temp8 , 1 , EEPROM_Bank7, EEPROM_TYPEC_STATUS);
        }
			SET_MASK(TypeCStatus3,AllEventsNotClear);
		    TypeCProcessStep = 0;	//Clear for new process flow step restart.
	      break;
	    case RTS5455_ERROR_STATE:
				//Type-C communiction error
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
* Function name: TypecPDProcess
*
* Descriptoin: This is invoked when the system is powered on
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void TypecPDProcess(void) {
BYTE i,j,MarkTemp[4];

  			if((RTS5455WaitingPingID != 0)&&(RTS5455WaitingPingID != 5))
  			{
  			return;
  			}
if(UCSI_RTS5455_CMD_WAIT_CNT!=0)
{
return;}
//if(IS_MASK_CLEAR(TypeCStatus3,BIT5)&SystemIsS0)
//{return;
//}
	//if(Read_TYPEC_INT())
	if(Read_TYPEC_INT()||IS_MASK_SET(TypeCStatus3,AllEventsNotClear))
	{
						RTS5455WaitingPingID = 5;
		switch(TypeCProcessStep)
		{
		 case RTS5455_INT_DE_ASSERT_INTR_PIN:
	    	if(Read_TYPEC_INT())
	    	{
		    	//De-assert INT gpio
					if(!bSMBusReceiveByte(chSMB_TYPEC, RTS5455_DE_ASSERT_I2C_ADDR, &SMB3_DATA[0]))
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
						SET_MASK(TypeCStatus3,AllEventsNotClear);
	      		TypeCProcessStep++;
					}
	    	}
	    	else
	    	{
	      	        TypeCProcessStep++;
					/*if((EventList[0]&RTS5455_INT_EVENT_MASK_LL)||
						(EventList[1]&RTS5455_INT_EVENT_MASK_LH)||
						(EventList[2]&RTS5455_INT_EVENT_MASK_HL)||
						(EventList[3]&RTS5455_INT_EVENT_MASK_HH)
						)
					{
	RamDebug(0xB8);
						SET_MASK(TypeCStatus3,AllEventsNotClear);
                        TypeCProcessStep = RTS5455_INT_PROCESS_EVENT;
					}
					else
					{
	RamDebug(0xB9);
						CLEAR_MASK(TypeCStatus3,AllEventsNotClear);
                        TypeCProcessStep = 0;
						RTS5455WaitingPingID = 0;
                        return;
					}
					if(TRTS5455_INT_VDM_RECEIVED(EventList[0]))
					{
	RamDebug(0xBA);
						EventList[3] &= 0xFD;	//~0x02
						TypeCEventNum        = TRTS5455_INT_VDM_RECEIVED_Num;					//Event number for "INT_VDM_RECEIVED" is 13.
					}
					else
					{
					RamDebug(0xBB);
						MarkTemp[0] = RTS5455_INT_EVENT_MASK_LL;
						MarkTemp[1] = RTS5455_INT_EVENT_MASK_LH;
						MarkTemp[2] = RTS5455_INT_EVENT_MASK_HL;
						MarkTemp[3] = RTS5455_INT_EVENT_MASK_HH;
						for(i = 0; RTS5455_INT_DATA_LENGTH > i; i++)
						{
						     if ((EventList[i]&MarkTemp[i]) != 0)
						   {
						       for(j = 0; 8 > j; j++)
						       {
						            if (((EventList[i]&MarkTemp[i]) & (1 << j)) != 0)
						           {
													EventList[i] &= ~(0x01 << j);
													TypeCEventNum = (i * 8) + j;
													TypeCProcessStep = RTS5455_INT_PROCESS_EVENT;
													return;
						           }
						       }
						   }
						}
					}*/
				/*	if(IS_MASK_SET(TypeCStatus3,AllEventsNotClear))
					{RamDebug(0xBA);
						TypeCProcessStep = RTS5455_INT_PROCESS_EVENT;
					}
					else
					{
						TypeCProcessStep = 0;
						RTS5455WaitingPingID = 0;
					}*/
	    	}
	      break;
	    case RTS5455_INT_GET_PD_STATUS:
  			//Send Get Event number command
  	//		if(RTS5455WaitingPingID == 0)
  			{
					SMB3_DATA[0] = 0x00;
					SMB3_DATA[1] = RTS5455_PORT_NUM;
					SMB3_DATA[2] = 11;//5;	//Bytrs
					if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_GET_STATUS>>8), &SMB3_DATA[0], RTS5455_GET_STATUS_WR_DATA_LEN, SMBus_NoPEC))
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
	//					RTS5455WaitingPingID = 5;
	      		TypeCProcessStep++;
					}
				}
	      break;
	    case RTS5455_INT_GET_PD_STATUS_WAIT_PING:
  //			if(RTS5455WaitingPingID == 5)
  			{
					if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
						{
		      		TypeCProcessStep++;
						}
						else
						{
							if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
							{
		//						RTS5455WaitingPingID = 0;
								TypeCProcessStep--;	//Retry command
							}
							//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
						}
					}
				}
	      break;
	    case RTS5455_INT_GET_PD_STATUS_CHECK_EVENT:
				if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &TypeCSMbusDataTemp[0]))
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
		//					RTS5455WaitingPingID = 0;
					TypeCEventNum = 0;
	/*RamDebug(0xC7);
	RamDebug(TypeCSMbusDataTemp[0]);
	RamDebug(TypeCSMbusDataTemp[1]);
	RamDebug(TypeCSMbusDataTemp[2]);
	RamDebug(TypeCSMbusDataTemp[3]);
	RamDebug(TypeCSMbusDataTemp[4]);
	RamDebug(TypeCSMbusDataTemp[5]);*/

					ClearTemp[0] = TypeCSMbusDataTemp[0];
					ClearTemp[1] = TypeCSMbusDataTemp[1];
					ClearTemp[2] = TypeCSMbusDataTemp[2];
					ClearTemp[3] = TypeCSMbusDataTemp[3];
													ClearPdEvents(RTS5455_INT_CLEAR_EVENT);
					EventList[0] |= TypeCSMbusDataTemp[0];
					EventList[1] |= TypeCSMbusDataTemp[1];
					EventList[2] |= TypeCSMbusDataTemp[2];
					EventList[3] |= TypeCSMbusDataTemp[3];
					if((EventList[0]&RTS5455_INT_EVENT_MASK_LL)||
						(EventList[1]&RTS5455_INT_EVENT_MASK_LH)||
						(EventList[2]&RTS5455_INT_EVENT_MASK_HL)||
						(EventList[3]&RTS5455_INT_EVENT_MASK_HH)
						)
					{
	//RamDebug(0xC8);
						SET_MASK(TypeCStatus3,AllEventsNotClear);
					}
					else
					{
	//RamDebug(0xC9);
						CLEAR_MASK(TypeCStatus3,AllEventsNotClear);
					}

					 //if((TRTS5455_POWER_DIRECTION(TypeCSMbusDataTemp[0]) == TRTS5455_POWER_DIRECTION_CONSUMER)&&(TRTS5455_POWER_OPERATION_MODE(TypeCSMbusDataTemp[0])==0))
					 	//{RamDebug(0x88);
					// TypeCProcessStep = 0;
					 //	}
					// else
					 //{
					 //	RamDebug(0x89);
					 //	}
					if(TRTS5455_INT_VDM_RECEIVED(EventList[0]))
					{
	//RamDebug(0xCA);
						//ClearTemp[0] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_LL;
						//ClearTemp[1] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_LH;
						//ClearTemp[2] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_HL;
						//ClearTemp[3] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_HH;
						//ClearTemp[3] |= 0x02;
						EventList[3] &= 0xFD;	//~0x02
						TypeCEventNum        = TRTS5455_INT_VDM_RECEIVED_Num;					//Event number for "INT_VDM_RECEIVED" is 13.
					}
					else
					{
						MarkTemp[0] = RTS5455_INT_EVENT_MASK_LL;
						MarkTemp[1] = RTS5455_INT_EVENT_MASK_LH;
						MarkTemp[2] = RTS5455_INT_EVENT_MASK_HL;
						MarkTemp[3] = RTS5455_INT_EVENT_MASK_HH;
						//ClearTemp[0] = RTS5455_INT_CLEAR_EVENT_MASK_LL;
						//ClearTemp[1] = RTS5455_INT_CLEAR_EVENT_MASK_LH;
						//ClearTemp[2] = RTS5455_INT_CLEAR_EVENT_MASK_HL;
						//ClearTemp[3] = RTS5455_INT_CLEAR_EVENT_MASK_HH;
						for(i = 0; RTS5455_INT_DATA_LENGTH > i; i++)
						{
						     if ((EventList[i]&MarkTemp[i]) != 0)
						   {
						       for(j = 0; 8 > j; j++)
						       {
						            if (((EventList[i]&MarkTemp[i]) & (1 << j)) != 0)
						           {
													/*
													*    find bit 1 in the event
													*    then clear it and convert to bit number
													*/
	//RamDebug(0xCC);
	//RamDebug(i);
	//RamDebug(j);
														//ClearTemp[0] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_LL;
														//ClearTemp[1] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_LH;
														//ClearTemp[2] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_HL;
														//ClearTemp[3] = 0x00;//RTS5455_INT_CLEAR_EVENT_MASK_HH;

													//ClearTemp[i] = 0x00;
													EventList[i] &= ~(0x01 << j);
 #if DEBUG_UCSI_RTK
    RamDebug(0xCE);
    RamDebug(ClearTemp[0]);
    RamDebug(ClearTemp[1]);
#endif
													TypeCEventNum = (i * 8) + j;
													//ClearPdEvents(RTS5455_INT_CLEAR_EVENT);
													TypeCProcessStep++;
													return;
						           }
						       }
						   }
						}
					}
					//ClearPdEvents(RTS5455_INT_CLEAR_EVENT);
					if(IS_MASK_SET(TypeCStatus3,AllEventsNotClear))
					{//RamDebug(0xCA);
						TypeCProcessStep++;
					}
					else
					{
						TypeCProcessStep = 0;
						RTS5455WaitingPingID = 0;
					}
				}
	      break;
	    case RTS5455_INT_PROCESS_EVENT:
		    	//Process Event
#if DEBUG_UCSI_RTK
	RamDebug(0xCB);
	RamDebug(TypeCEventNum);
#endif

					switch(TypeCEventNum)
					{
			      case TRTS5455_INT_EXTERNAL_SUPPLY_CHANGE_Num:
                    #if DEBUG_UCSI_RTK
                        RamDebug(0xC0);
                    #endif
                    GetConnectorStatus_RTS5455(0);
                    Ucsi_Connector_Change_Status[0] = (UCSI_GET_CONNECTOR_STATUS_RTS5455[0] + (UCSI_GET_CONNECTOR_STATUS_RTS5455[1]<<8));
                    #if DEBUG_UCSI_RTK
                        RamDebug(UCSI_GET_CONNECTOR_STATUS_RTS5455[0]);
                    RamDebug(UCSI_GET_CONNECTOR_STATUS_RTS5455[1]);
                    #endif
                    if (0 != (Ucsi_Connector_Change_Status[0] & UCSI_SUPPORTED_CAM_CHANGE_NOTIFICATION_ENABLE)) {
                        //RamDebug(0xCD);
                        UCSI_SUPPORTED_CAM_CHANGE = 1;
                        }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						RTS5455WaitingPingID = 0;
                    break;
                   /*case TRTS5455_INT_PD_RESET_COMPLETE_Num:
                    RamDebug(0xC2);
                    UCSI_PD_RESET_COMPLETE_CHANGE = 1;
                    SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
                    break;*/
                   case TRTS5455_INT_SUPPORTED_CAM_CHANGE_Num:
                    #if DEBUG_UCSI_RTK
                        RamDebug(0xC1);
                    #endif
                    UCSI_SUPPORTED_CAM_CHANGE = 1;
                    SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						RTS5455WaitingPingID = 0;
                    break;
			      case TRTS5455_INT_PORT_PARTNER_CHANGED_Num:
                    #if DEBUG_UCSI_RTK
                        RamDebug(0xC2);
                    #endif
                    //GetConnectorStatus_RTS5455(0);
                    UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                    Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);

                    SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						RTS5455WaitingPingID = 0;
                    break;
			      case TRTS5455_INT_POWER_DIRECTION_CHANGED_Num:
					      	//RamDebug(0xDA);
					      	//RamDebug(TypeCSMbusDataTemp[4]);
					      	 RamDebug(0xC3);
					      	#if DEBUG_UCSI_RTK
                        RamDebug(0xC3);
                    #endif
                            //GetConnectorStatus_RTS5455(0);
					      	UCSI_POWER_DIRECTION_CHANGE = 1;
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
//				      if(TRTS5455_CONNECT_STATUS(TypeCSMbusDataTemp[0]) == TRTS5455_CONNECT_STATUS_ATTACHED)
//				      {
//				        /*
//				        *   Connection is present
//				        */
//					      if(TRTS5455_POWER_DIRECTION(TypeCSMbusDataTemp[0]) == TRTS5455_POWER_DIRECTION_CONSUMER)
//					      {
//				          /*
//				          *    USB PD act as Sink
//				          */
//					      SET_MASK(TypeCStatus,TypeCAdpExist);
//				  			TypeCAdpPdStableCheck = 5;
//							RamDebug(0xdd);
        //				  			TypeCAdpDetectDelay = 50;
//					  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
//					  			{
//										CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
//										SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
//										CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
//									}
//				      	}
 //             else
  //             {
//				          /*
//				          *    USB PD act as Source
//				          */
//					  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
//					  			{
//										SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
//										CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
//							      //SendUnitInformationToPdc();
//										CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
//					      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
//					      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
//					      TypeCAdpID = 0;
//					      CurrentTypeCWatt = 0;
//					      TypeCAdpPdStableCheck = 0;
//					      CurrentTypeCV = 0;
//					      CurrentTypeCI = 0;
//									}
//				        }
 //             }
				      SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						//RTS5455WaitingPingID = 0;
			        break;
			      case TRTS5455_INT_CONNECT_CHANGE_Num:
				      if(TRTS5455_CONNECT_STATUS(TypeCSMbusDataTemp[0]) == TRTS5455_CONNECT_STATUS_ATTACHED)
				      {
				        /*
				        *   Connection is present
				        */
				         RamDebug(0x66);
				        #if DEBUG_UCSI_RTK
                        RamDebug(0xC4);
                    #endif
				          Usb_Pdc_Status[0].device_connected = TRUE;
					 Ucsi_Connector_Change_Status[0] |= UCSI_CONNECTOR_STATUS_CHANGE_CONNECT;
					 Ucsi_Connector_Number_Changed = 1;
				//	 TypeCAdpPdStableCheck = 3;
//					      if(TRTS5455_POWER_DIRECTION(TypeCSMbusDataTemp[0]) == TRTS5455_POWER_DIRECTION_CONSUMER)
//					      {
//				          /*
//				          *    USB PD act as Sink
//				          */
//                          if(Usb_Pdc_Power_Status[0].power_source)
//                          {
//                            Usb_Pdc_Power_Status[0].power_source = 0;
//                            //UCSI_POWER_DIRECTION_CHANGE = 1;
//                          }
//					      	SET_MASK(TypeCStatus,TypeCAdpExist);
//				  			TypeCAdpPdStableCheck = 3;
//				  			TypeCAdpDetectDelay = 50;
//					  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
//					  			{
//										CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
//										SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
//										CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
//									}
//				        }
//				        else
//				        {
//				          /*
//				          *    USB PD act as Source
//				          */
//                          if(!Usb_Pdc_Power_Status[0].power_source)
//                          {
//                            Usb_Pdc_Power_Status[0].power_source = 1;
//                            //UCSI_POWER_DIRECTION_CHANGE = 1;
//                          }
//					  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
//					  			{
//										SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
//										CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
//										CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
//					      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
//					      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
//					      TypeCAdpID = 0;
//					      CurrentTypeCWatt = 0;
////					      TypeCAdpPdStableCheck = 0;
//					      CurrentTypeCV = 0;
//					      CurrentTypeCI = 0;
//									}
//				          }
				        /*
				        *   Lenovo dock check
				        */
					      if(TRTS5455_LENOVO_DOCK_CHECK(TypeCSMbusDataTemp[0]) == TRTS5455_LENOVO_DOCK_CHECK_LENOVO_DOCK)
					      {
				          /*
				          *    Lenovo dock
				          */
		  						SET_MASK(TypeCStatus2,TypeC_Docking_Attached);
		  						SendPowerModeChangeToPdc();
				        }
					      if(TRTS5455_LENOVO_LAPTOP_CHECK(TypeCSMbusDataTemp[0]) == TRTS5455_LENOVO_LAPTOP_CHECK_LENOVO_LAPTOP_ATTACHED)
					      {
				          /*
				          *    Lenovo Laptop for P2P
				          */
									RTS5455WaitPingDelay = 10;
						  SET_MASK(LENOVOP2PSTATUS,LENOVOStatusFlag);
						  PdPowerStableDelay = 101;
		  						SET_MASK(TypeCStatus2,TypeC_Laptop_Attached);
						      if(TRTS5455_POWER_DIRECTION(TypeCSMbusDataTemp[0]) == TRTS5455_POWER_DIRECTION_PROVIDER)
						      {
					          /*
					          *    USB PD act as Source
					          */
                              Usb_Pdc_Power_Status[0].power_source = 1;
							      SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
										CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
							      //SendUnitInformationToPdc();
							      SET_MASK(TypeCStatus3,NeedUpdateToPartner);
					      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
					      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
					      TypeCAdpID = 0;
					      CurrentTypeCWatt = 0;
					      TypeCAdpPdStableCheck = 0;
					      CurrentTypeCV = 0;
					      CurrentTypeCI = 0;
					        }
					        else
					        {
					          /*
					          *    USB PD act as Sink
					          */
                              Usb_Pdc_Power_Status[0].power_source = 0;
						      	CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
										SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
							      //SET_MASK(TypeCStatus3,NeedUpdateToPartner);
                                  CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
					          }
				          }

				      }
				      else
				      {
				        /*
				        *   Connection is un-attach
				        */
				         RamDebug(0x65);
				        #if DEBUG_UCSI_RTK
                        RamDebug(0xC5);
                    #endif

				          Usb_Pdc_Status[0].device_connected = FALSE;
                    Ucsi_Connector_Change_Status[0] |= UCSI_CONNECTOR_STATUS_CHANGE_CONNECT;
					 Ucsi_Connector_Number_Changed = 1;
                          Usb_Pdc_Status[0].connect_status = USB_CONNECT_NONE;
                        UCSI_RDO1 = 0;
                        UCSI_RDO2 = 0;
                        UCSI_RDO3 = 0;
                        UCSI_RDO4 = 0;
					      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
					      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
		  					CLEAR_MASK(TypeCStatus2,TypeC_Docking_Attached);
		  					CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Attached);
		  					CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
		  					CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
		  					CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
		  					CLEAR_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
		  					CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
		  					AnoutherUnitInformation[0] = 0;
		  					AnoutherUnitInformation[1] = 0;
							LENOVOP2PSTATUS=0;
							CLEAR_MASK(DETECTDOCKORDAPTER,b0_ACORDOCK);
					      TypeCAdpID = 0;
					      CurrentTypeCWatt = 0;
//					      TypeCAdpPdStableCheck = 0;
					      CurrentTypeCV = 0;
					      CurrentTypeCI = 0;
				      }
				      SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						//RTS5455WaitingPingID = 0;
			        break;
			      case TRTS5455_INT_VDM_RECEIVED_Num:
                    #if DEBUG_UCSI_RTK
                        RamDebug(0xC6);
                    #endif
							//if(RTS5455WaitingPingID == 0)
							{
								SMB3_DATA[0] = RTS5455_GET_VDM & 0x00FF;
								SMB3_DATA[1] = RTS5455_PORT_NUM;
								if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_GET_VDM>>8), &SMB3_DATA[0], RTS5455_GET_VDM_WR_DATA_LEN, SMBus_NoPEC))
								{
				  				TypeCCommFailCnt++;
				  			}
				  			else
				  			{
				  				TypeCCommFailCnt = 0;
								//	RTS5455WaitingPingID = 6;
									TypeCProcessStep = RTS5455_INT_PROCESS_EVENT_WAIT_PING;
				  			}
				  		}
			        break;
			      case TRTS5455_INT_ALTERNATE_FLOW_CHANGE_Num:
				      SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						RTS5455WaitingPingID = 0;
			        break;
			      case TRTS5455_INT_NEGOTIATED_POWER_LEVEL_CHANGE_Num:
				      if(TRTS5455_CONNECT_STATUS(TypeCSMbusDataTemp[0]) == TRTS5455_CONNECT_STATUS_ATTACHED)
				      {
				        /*
				        *   Connection is present
				        */
				        Usb_Pdc_Status[0].device_connected = TRUE;
				        //if(TRTS5455_POWER_OPERATION_MODE(TypeCSMbusDataTemp[0]) == TRTS5455_POWER_OPERATION_MODE_PD)
				        {
						      if(TRTS5455_POWER_DIRECTION(TypeCSMbusDataTemp[0]) == TRTS5455_POWER_DIRECTION_CONSUMER)
						      {
					          /*
					          *    USB PD act as Sink
					          */
	                  if(Usb_Pdc_Power_Status[0].power_source)
	                  {
	                    Usb_Pdc_Power_Status[0].power_source = 0;
	                  }
						      	SET_MASK(TypeCStatus,TypeCAdpExist);
						  			TypeCAdpPdStableCheck = 5;
						  			//TypeCAdpDetectDelay = 50;
						  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
						  			{
											CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
											SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
											CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
										}
					        }
					        else
					        {
					          /*
					          *    USB PD act as Source
					          */
	                  if(!Usb_Pdc_Power_Status[0].power_source)
	                  {
	                    Usb_Pdc_Power_Status[0].power_source = 1;
	                  }
						  			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Attached))
						  			{
											SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type);
											CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
											CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
								      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
								      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
								      TypeCAdpID = 0;
								      CurrentTypeCWatt = 0;
								      CurrentTypeCV = 0;
								      CurrentTypeCI = 0;
										}
									}
								}
				      }
				      SET_MASK(TypeCStatus,TypeCProcessOK);
	      			TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						RTS5455WaitingPingID = 0;
			        break;
			      default:
		  				{
			      	 #if DEBUG_UCSI_RTK
                        RamDebug(0xCE);
                    #endif
								/*
								*    Clear other events
								*/
								ClearTemp[0] = RTS5455_INT_CLEAR_EVENT_MASK_LL;
								ClearTemp[1] = RTS5455_INT_CLEAR_EVENT_MASK_LH;
								ClearTemp[2] = RTS5455_INT_CLEAR_EVENT_MASK_HL;
								ClearTemp[3] = RTS5455_INT_CLEAR_EVENT_MASK_HH;
				      	SET_MASK(TypeCStatus,TypeCProcessOK);
	      				TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
						RTS5455WaitingPingID = 0;
				    	}
			        break;
			    }
	      break;
	    case RTS5455_INT_CLEAR_EVENT:
            #if DEBUG_UCSI_RTK
                        RamDebug(0xCC);
                    #endif
	    		i = ClearPdEvents(RTS5455_INT_CLEAR_EVENT);
	    		if(i != 0xFE)
	    		{
	    			TypeCProcessStep = i;
	    		}
//		      if(IS_MASK_SET(TypeCStatus,TypeCProcessOK))
//		      {
//		      	if(IS_MASK_CLEAR(TypeCStatus3,AllEventsNotClear))
//		      	{
//			      	//De-assert INT gpio
//							if(!bSMBusReceiveByte(chSMB_TYPEC, RTS5455_DE_ASSERT_I2C_ADDR, &SMB3_DATA[0]))
//			  			{
//			  				TypeCCommFailCnt++;
//								if(TypeCCommFailCnt == 3)
//								{
//						    	TypeCIniDelay = 200;
//	//						    	TypeCProcessStep = RTS5455_ERROR_STATE;
//						    	TypeCCommFailCnt = 0;
//								}
//			  			}
//			  			else
//			  			{
//							  TypeCCommFailCnt = 0;
//							}
//						}
//		      	//Clear events
//						SMB3_DATA[0] = RTS5455_ACK_CC_CI & 0x00FF;
//						SMB3_DATA[1] = RTS5455_PORT_NUM;
//						SMB3_DATA[2] = ClearTemp[0];
//						SMB3_DATA[3] = ClearTemp[1];
//						SMB3_DATA[4] = ClearTemp[2];
//						SMB3_DATA[5] = ClearTemp[3];
//						SMB3_DATA[6] = 0x01;	//Set bit0 for command complete ack
//						if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_ACK_CC_CI>>8), &SMB3_DATA[0], RTS5455_ACK_CC_CI_WR_DATA_LEN, SMBus_NoPEC))
//		  			{
//		  				TypeCCommFailCnt++;
//							if(TypeCCommFailCnt == 3)
//							{
//					    	TypeCIniDelay = 200;
////						    	TypeCProcessStep = RTS5455_ERROR_STATE;
//					    	TypeCCommFailCnt = 0;
//							}
//		  			}
//		  			else
//		  			{
//						  TypeCCommFailCnt = 0;
//		  				//Process finish
//		      		TypeCProcessStep = 0;	//Clear for new process flow step restart.
//				      CLEAR_MASK(TypeCStatus,TypeCProcessOK);
//						}
//		      }
	      break;
	    case RTS5455_INT_PROCESS_EVENT_WAIT_PING:
  		//	if(RTS5455WaitingPingID == 6)
  			{
				if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
			      	//RamDebug(0xD5);
			      	//RamDebug(SMB3_DATA[0]);
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
					{
						//	RTS5455WaitingPingID = 0;
	      		TypeCProcessStep = RTS5455_INT_PROCESS_EVENT_PROCESS_DATA;
					}
					else
					{
						if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
						{
								CLEAR_MASK(TypeCStatus3,RTS5455WaitingPing);
							//	RTS5455WaitingPingID = 0;
							TypeCProcessStep = RTS5455_INT_PROCESS_EVENT;	//Retry command
						}
						//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
						}
					}
				}
	      break;
	    case RTS5455_INT_PROCESS_EVENT_PROCESS_DATA:
	//RamDebug(0xD6);
	//RamDebug(TypeCEventNum);
				switch(TypeCEventNum)
				{
		      case TRTS5455_INT_VDM_RECEIVED_Num:
						if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
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
      	/*RamDebug(0xCF);
      	RamDebug(SMB3_DATA[0]);
      	RamDebug(SMB3_DATA[1]);
      	RamDebug(SMB3_DATA[2]);
      	RamDebug(SMB3_DATA[3]);
      	RamDebug(SMB3_DATA[4]);
      	RamDebug(SMB3_DATA[5]);
      	RamDebug(SMB3_DATA[6]);
      	RamDebug(SMB3_DATA[7]);
      	RamDebug(SMB3_DATA[8]);
      	RamDebug(SMB3_DATA[9]);
      	RamDebug(SMB3_DATA[10]);
      	RamDebug(SMB3_DATA[11]);*/
		  				//Device's VDM infromation beging from Byte1,Byte0 is PDO length
		  				if((SMB3_DATA[4] == 0x17) && (SMB3_DATA[3] == 0xEF))	//Lenovo's USB-IF ID is "17EFh"
					  				{
					  					if((SMB3_DATA[1] == 0x12)&&(SMB3_DATA[2] == 0xA0))
					  					{	//Get anouther unit information for P2P
		  									SET_MASK(TypeCStatus2,TypeC_Laptop_Attached);	//For EC wake from sleep mode.
					  						AnoutherUnitInformation[0] = SMB3_DATA[5];
					  						AnoutherUnitInformation[1] = SMB3_DATA[6];
						  					SET_MASK(TypeCStatus3,GotPartnerInformation);
//					  						if(IS_MASK_SET(TypeCStatus3,NeedUpdateToPartner))
//					  						{
//					  							CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
//							      			SendUnitInformationToPdc();
//							      		}
					  		}
					  		else
					  		{
		  						OldDockingACKStatus = SMB3_DATA[5];
		  						OldDockingEvent = SMB3_DATA[8];
		  						OldVDO2[0] = SMB3_DATA[9];
		  						OldVDO2[1] = SMB3_DATA[10];
		  						OldVDO2[2] = SMB3_DATA[11];
		  						OldVDO2[3] = SMB3_DATA[12];
		  						if((SMB3_DATA[1]& 0x1F) == 0x10)
		  						{
		  							if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT)
		  							{	// Is WOL/Button action?
		  								if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_WOL)
		  								{	// Is WOL action?
		  									if(IS_MASK_SET(u4Cfgspace2,DockingWOLEn))
		  									{

		  										SET_MASK(TypeCStatus2,DockingEvent);
												SET_MASK(u4ReserveC3,DOCKONLAN);
		  										LanWakeLow();
												RamDebug(0x34);


		  									}
		  								}
		  								else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_MAKE))
		  								{	// Is Button Make action?
		  									PSWPressed();
		  									if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
		  									{ // Is Button Break at the same time?
		  										DockingPSW_ActiveCnt = 11;
		  									}
		  								}
		  								else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
		  								{	// Is Button Break action?
		  									PSWReleased();
											RamDebug(0x36);
		  								}
		  							SMB3_DATA[7] = SMB3_DATA[8];
	  								SMB3_DATA[8] = 0x00;
	  								SendPowerModeChangeToPdc();
									}
		  						}
		  					}

		  					if((SMB3_DATA[1] == 0x44) || (SMB3_DATA[1] == 0x06))	//0x44 for User VID Alt mode enter;0x06 for Lenovo Attention VDM
		  					{	//Send "Get Lenovo status" package
		  						SET_MASK(TypeCStatus2,TypeC_Docking_Attached);
		  						//SendLenovoAttentionVdmToPdc();
		  						SendPowerModeChangeToPdc();
		  					}
		  				}
						}
				    SET_MASK(TypeCStatus,TypeCProcessOK);
	      		TypeCProcessStep = 0;//RTS5455_INT_CLEAR_EVENT;
					//	RTS5455WaitingPingID = 0;
		        break;
		    }
	      break;
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
	if(Read_EC_ON())
	{
//	if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, TPS65982_SYSTEM_CONFIG, &SMB3_DATA[0]))
//	{
//		TypeCCommFailCnt++;
//	}
//	else
//	{
//	  TypeCCommFailCnt = 0;
//	}
RamDebug(0x66);
		TypeCIniDelay = TypeC_Ini_Delay_time;	//Delay for type C stable
	  TypeCStatus = 0;
	  TypeCCommFailCnt = 0;
	  TypeCAdpID = 0;
	  CurrentTypeCV = 0;
	  CurrentTypeCI = 0;
	  CurrentTypeCWatt = 0;
	  CLEAR_MASK(TypeCStatus2,TypeC_ChipOff);
	}
	else
	{
		SET_MASK(TypeCStatus2,TypeC_ChipOff);
	}
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
	if(!Read_PM_PWRBTN())
	{	//Work-around for quick press and release docking power button
		if(DockingPSW_ActiveCnt)
		{
			if(DockingPSW_ActiveCnt == 1)
			{
				PSWReleased();
			}
			DockingPSW_ActiveCnt--;
		}
	}
	else
	{
		DockingPSW_ActiveCnt = 0;
	}
	if(IS_MASK_CLEAR(PDStatus,TypeCFwUpdating))
	{
		if(IS_MASK_SET(TypeCStatus2,TypeC_ChipOff))
		{
			OEM_TYPE_C_init_hook();
			RamDebug(0x41);
		}
		if(IS_MASK_SET(TypeCStatus,TypeCIniOK))
		{//RamDebug(0xD6);
		if(IS_MASK_SET(PDStatus,Get_Fw_Version))
			{
				GetTypecPDVersion();
			}
		else{
		    if(P2P_Charge_Disable)
            {RamDebug(0xDD);
                P2P_Charge_Condition();
            }

			TypecPDProcess();
			DetectAdaptorWatt();
			P2P_Process();
			}
		}
		else
		{
			InitialTypecPD();
			//SendPdoSet(ObjectPosition20V);
			//RamDebug(0XD1);
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
	if(IS_MASK_SET(TypeCStatus,TypeCAdpExist) && IS_MASK_SET(TypeCStatus,TypeCAdpReady))
	{
	if(PD_Ini_Stable_Delay)
		{
			PD_Ini_Stable_Delay--;
			return;
		}
		if((RTS5455_GET_RDO_OBJECT_POSITION_MASK(CurrentRDO[0])!= ObjectPosition09V) && IS_MASK_SET(BattStatusL,FullyChg))
		{
			SendPdoSet(ObjectPosition09V);	// Switch PDO to 9V
			RamDebug(0x33);
	  	}
	  	else if((RTS5455_GET_RDO_OBJECT_POSITION_MASK(CurrentRDO[0])!= ObjectPosition20V) && IS_MASK_CLEAR(BattStatusL,FullyChg))
	  	{
			SendPdoSet(ObjectPosition20V);	// Switch PDO to 20V
	  	}
	}
}
/*
*******************************************************************************
* Function name: OEM_TYPE_C_S5S0_HOOK
*
* Descriptoin: TYPE-C hook for S5 to S0
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S5S0_HOOK(void)
{
	CLEAR_MASK(PDStatus,TypeCFwUpdating);
	//SendPdoSet(ObjectPosition20V);
	if( Read_AC_IN())
		{
	if((RTS5455_GET_RDO_OBJECT_POSITION_MASK(CurrentRDO[0])!= ObjectPosition20V))
	{
			SendPdoSet(ObjectPosition20V);	// Switch PDO to 20V
	}
	}
}
/*
*******************************************************************************
* Function name: Oem_Hook_Type_C_BeforeSleep
*
* Descriptoin:
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void Oem_Hook_Type_C_BeforeSleep(void){

  //SET_MASK(TypeCStatus2,TypeC_ChipOff);
}

/*
*******************************************************************************
* Function name: GetTypecPDVersion
*
* Descriptoin: Read PD FW version
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void GetTypecPDVersion(void) {

	switch(PDStatus&0x3F)
	{
    case RTS5455_GET_VERSION_SEND_CMD:
			SMB3_DATA[0] = 0x03;	//OFFSET, FW version data is from bit32~55
			SMB3_DATA[1] = 0x00;
			SMB3_DATA[2] = 0x03;	//DATA_LEN, FW version are 3bytes(bit32~55)
			if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, RTS5455_GET_IC_STATUS, &SMB3_DATA[0], RTS5455_GET_IC_STATUS_LEN, SMBus_NoPEC))
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
				PDStatus++;
			}
	    break;
	    case RTS5455_GET_VERSION_SEND_CMD_WAIT_PING:
			if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
				if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
				{
      		PDStatus++;
				}
				else
				{
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
					{
						PDStatus--;	//Retry command
					}
					//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
				}
			}
      break;
	    case RTS5455_GET_VERSION_PROCESS:
			if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
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
				PDFwVersion[0] = SMB3_DATA[0];
				PDFwVersion[1] = SMB3_DATA[1];
				CLEAR_MASK(PDStatus,Get_Fw_Version);
				PDStatus &= 0xC0;
			}
      break;
  }
}
/*
*******************************************************************************
* Function name: RTS5455_WaitPing
*
* Descriptoin: Wait the RTS5455 ping status
*
* Arguments:
*       none
* Return Values:
*       BYTE
*******************************************************************************
*/
/*BYTE RTS5455_WaitPing(void)
{
	BYTE CheckTemp;
	CheckTemp = 0;
	if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
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
		//RamDebug(0xD4);
		//RamDebug(SMB3_DATA[0]);
		if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
		{
			//CheckTemp = 1;
			CheckTemp = SMB3_DATA[0];
		}
	}
	return CheckTemp;
}*/

/*
*******************************************************************************
* Function name: ClearPdEvents
*
* Descriptoin: Clear PD's events
*
* Arguments:
*       none
* Return Values:
*       BYTE: 0x00~0x7F:Next Step, 0xFE:No process, 0xFF:ERROR
*******************************************************************************
*/
BYTE ClearPdEvents(BYTE CurrentStep)
{
	BYTE NextStep;
	if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
	|| IS_MASK_SET(PDStatus,TypeCFwUpdating)
	)
	{
		return 0xFE;
	}
	NextStep = CurrentStep + 1;
	//RamDebug(0xD7);
	//RamDebug(ClearTemp[0]);
	//RamDebug(ClearTemp[1]);
	//RamDebug(ClearTemp[2]);
	//RamDebug(ClearTemp[3]);
  //if(IS_MASK_SET(TypeCStatus,TypeCProcessOK))
  {
  	if(IS_MASK_CLEAR(TypeCStatus3,AllEventsNotClear))
  	{
	//RamDebug(0xDD);
	#if DEBUG_UCSI_RTK
    RamDebug(0xCD);
    RamDebug(ClearTemp[0]);
    RamDebug(ClearTemp[1]);
#endif
    	//De-assert INT gpio
			/*if(!bSMBusReceiveByte(chSMB_TYPEC, RTS5455_DE_ASSERT_I2C_ADDR, &SMB3_DATA[0]))
			{
				TypeCCommFailCnt++;
				if(TypeCCommFailCnt == 3)
				{
		    	TypeCIniDelay = 200;
//		    	NextStep = RTS5455_ERROR_STATE;
		    	TypeCCommFailCnt = 0;
				}
			}
			else
			{
			  TypeCCommFailCnt = 0;
			  CLEAR_MASK(TypeCStatus,TypeCProcessOK);
			}*/
		}
  	//Clear events
		SMB3_DATA[0] = RTS5455_ACK_CC_CI & 0x00FF;
		SMB3_DATA[1] = RTS5455_PORT_NUM;
		SMB3_DATA[2] = ClearTemp[0];
		SMB3_DATA[3] = ClearTemp[1];
		SMB3_DATA[4] = ClearTemp[2];
		SMB3_DATA[5] = ClearTemp[3];
		SMB3_DATA[6] = 0x01;	//Set bit0 for command complete ack
		if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_ACK_CC_CI>>8), &SMB3_DATA[0], RTS5455_ACK_CC_CI_WR_DATA_LEN, SMBus_NoPEC))
		{
			TypeCCommFailCnt++;
			if(TypeCCommFailCnt == 3)
			{
	    	TypeCIniDelay = 200;
//	    	NextStep = RTS5455_ERROR_STATE;
	    	TypeCCommFailCnt = 0;
			}
		}
		else
		{
		  TypeCCommFailCnt = 0;
			//Process finish
  		NextStep = 0;	//Clear for new process flow step restart.
      CLEAR_MASK(TypeCStatus,TypeCProcessOK);
		}
  }
  return NextStep;
}

/*
*******************************************************************************
* Function name: SendLenovoAttentionVdmToPdc
*
* Descriptoin: Send Lenovo Attention VDM to PDC
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
/*void SendLenovoAttentionVdmToPdc(void)
{
	BYTE i;
	if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
	|| IS_MASK_CLEAR(TypeCStatus2,TypeC_Docking_Attached)
	|| IS_MASK_SET(PDStatus,TypeCFwUpdating)
	)
	{
		return;
	}
	//Header
	SMB3_DATA[2] = 0x06;
	SMB3_DATA[3] = 0xA0;
	SMB3_DATA[4] = 0xEF;
	SMB3_DATA[5] = 0x17;

	SMB3_DATA[0] = RTS5455_SEND_VDM & 0x00FF;
	SMB3_DATA[1] = RTS5455_PORT_NUM;
			      	//RamDebug(0xD8);
	if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SEND_VDM>>8), &SMB3_DATA[0], 14, SMBus_NoPEC))
	{
		TypeCCommFailCnt++;
	}
	else
	{
		TypeCCommFailCnt = 0;
	}
}*/

/*
*******************************************************************************
* Function name: SendGetLenovoStatusVdmToPdc
*
* Descriptoin: Send Get Lenovo Status VDM to PDC
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
/*void SendGetLenovoStatusVdmToPdc(void)
{
	BYTE i;
	if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
	|| IS_MASK_CLEAR(TypeCStatus2,TypeC_Docking_Attached)
	|| IS_MASK_SET(PDStatus,TypeCFwUpdating)
	)
	{
		return;
	}
	//Header
	SMB3_DATA[2] = 0x10;
	SMB3_DATA[3] = 0xA0;
	SMB3_DATA[4] = 0xEF;
	SMB3_DATA[5] = 0x17;

	SMB3_DATA[0] = RTS5455_SEND_VDM & 0x00FF;
	SMB3_DATA[1] = RTS5455_PORT_NUM;
			      	//RamDebug(0xD9);
	if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SEND_VDM>>8), &SMB3_DATA[0], 14, SMBus_NoPEC))
	{
		TypeCCommFailCnt++;
	}
	else
	{
		TypeCCommFailCnt = 0;
	}
}*/

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
	RTS5455WaitingPingID=12;
	for(i=0;i<14;i++)//14
	{
		SMB3_DATA[i+1] = 0;
	}
	SMB3_DATA[6] = OldDockingACKStatus;
	SMB3_DATA[6] &= 0xFE;	//Clear Docking ACK flag
	SMB3_DATA[9] = OldDockingEvent;
	SMB3_DATA[8] = OldDockingEvent;
	SMB3_DATA[8] |= 0x01;	//Set System ACK flag
	SMB3_DATA[10] = OldVDO2[0];
	SMB3_DATA[11] = OldVDO2[1];
	SMB3_DATA[12] = OldVDO2[2];
	SMB3_DATA[13] = OldVDO2[3];

	//Header
	SMB3_DATA[2] = 0x10;
	SMB3_DATA[3] = 0xA0;
	SMB3_DATA[4] = 0xEF;
	SMB3_DATA[5] = 0x17;

  switch (SysPowState)
  {	//Set power status
    case SYSTEM_S4 :
    case SYSTEM_S5 :
			SMB3_DATA[7] |= (0x05 << 1);
      break;
    case SYSTEM_S3 :
			SMB3_DATA[7] |= (0x03 << 1);
      break;
	 case  SYSTEM_S3_S0:
	case SYSTEM_S5_S0: 
    case SYSTEM_S0 :
			SMB3_DATA[7] |= (0x01 << 1);
      break;
    default :
      break;
  }
  if(ACPOWER_ON)
  {	//Set power mode
		SMB3_DATA[7] |= (0x01 << 4);
  }
	SMB3_DATA[7] |= 0x01;	//Set power status change
	//SMB3_DATA[1] = 3;	//PDO number is 3

	SMB3_DATA[0] = RTS5455_SEND_VDM & 0x00FF;
	SMB3_DATA[1] = RTS5455_PORT_NUM;
			      	//RamDebug(0xD7);
	if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SEND_VDM>>8), &SMB3_DATA[0], 14, SMBus_NoPEC))
	{
		TypeCCommFailCnt++;
	}
	else
	{
		TypeCCommFailCnt = 0;
		RTS5455WaitingPingID=12;
		SENT_PD_RETRY_ReceiveByte();
		RamDebug(0xA4);
	}
}

/*
*******************************************************************************
* Function name: SendPdcVdm
*
* Descriptoin: Send PDO select to PDC
*
* Arguments:
*       PDOSetNum: 01:5V,02:9V,03:15V,04:20V
* Return Values:
*       none
*******************************************************************************
*/
void SendPdoSet(BYTE PDOSetNum)
{
	if(IS_MASK_SET(PDStatus,TypeCFwUpdating))
	{
		return;
	}
	if((PDOSetNum == 0)||(PDOSetNum > 4))
	{
		return;
	}
	SMB3_DATA[0] = RTS5455_SET_RDO & 0x00FF;
	SMB3_DATA[1] = RTS5455_PORT_NUM;
	SMB3_DATA[2] = CurrentRDO[0];
	SMB3_DATA[3] = CurrentRDO[1];
	SMB3_DATA[4] = CurrentRDO[2];
	SMB3_DATA[5] = CurrentRDO[3];
	switch( PDOSetNum )
	{
    case 0x02:	//9V
			SMB3_DATA[5] &= 0x8F;	//Clear bit28~bit30
			SMB3_DATA[5] |= (ObjectPosition09V<<4);	//Set bit28~bit30
			SMB3_DATA[4] &= 0xF0;	//Clear bit16~bit19
			SMB3_DATA[4] |= (PDO09I>>6);	//Set bit16~bit19
			SMB3_DATA[3] = 0x00;	//Clear bit8~bit15
			SMB3_DATA[3] = ((PDO09I<<2)&0xFC)+(PDO09I>>8);	//Clear bit8~bit15
			SMB3_DATA[2] = 0x00;	//Clear bit0~bit7
			SMB3_DATA[2] = PDO09I&0xFF;	//Set bit0~bit7
      break;
    case 0x04:	//20V
			SMB3_DATA[5] &= 0x8F;	//Clear bit28~bit30
			SMB3_DATA[5] |= (ObjectPosition20V<<4);	//Set bit28~bit30
			SMB3_DATA[4] &= 0xF0;	//Clear bit16~bit19
			SMB3_DATA[4] |= (PDO20I>>6);	//Set bit16~bit19
			SMB3_DATA[3] = 0x00;	//Clear bit8~bit15
			SMB3_DATA[3] = ((PDO20I<<2)&0xFC)+(PDO20I>>8);	//Clear bit8~bit15
			SMB3_DATA[2] = 0x00;	//Clear bit0~bit7
			SMB3_DATA[2] = PDO20I&0xFF;	//Set bit0~bit7
      break;
	}
	if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SET_RDO>>8), &SMB3_DATA[0], RTS5455_SET_RDO_WR_DATA_LEN, SMBus_NoPEC))
	{
		TypeCCommFailCnt++;
	}
	else
	{
		TypeCCommFailCnt = 0;
		CurrentRDO[0] = SMB3_DATA[2];
		CurrentRDO[1] = SMB3_DATA[3];
		CurrentRDO[2] = SMB3_DATA[4];
		CurrentRDO[3] = SMB3_DATA[5];
		RTS5455WaitingPingID = 13;
	}
}
/*
*******************************************************************************
* Function name: P2P_Process
*
* Descriptoin: P to P function process
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void P2P_Process(void)
{
	BYTE i,UnitARSOCTemp,UnitASxTemp;
	if(Detect_Adaptor_Watt_SKIP==1)
		{return;}
	if(IS_MASK_SET(TypeCStatus3,NeedUpdateToPartner)&&(TypeCProcessStep==0)&&(PDStatus==0))//&&(UCSI_RTS5455_CMD_WAIT==0))
	{
		SendUnitInformationToPdc();//qizhuang
		RamDebug(0xb0);
		return;
	}
	else
	{
		if(RTS5455WaitingPingID == 8)
		{
			RTS5455WaitingPingID = 0;
		}
	}
	if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
	|| IS_MASK_CLEAR(TypeCStatus2,TypeC_Laptop_Attached)
	|| IS_MASK_SET(PDStatus,TypeCFwUpdating)
	|| IS_MASK_SET(TypeCStatus3,AllEventsNotClear)
	)
	{
		if(RTS5455WaitingPingID == 9)
		{
			RTS5455WaitingPingID = 0;
			CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
			RTS5455WaitPingDelay = 10;
                    RamDebug(0xDB);
		}
		return;
	}
	if((SysPowState == SYSTEM_S0)||(SysPowState == SYSTEM_S5_S0))
	{
		i = 1;
	}
	else
	{
		i = 0;
	}
	if(IS_MASK_CLEAR(TypeCStatus2,TypeC_Laptop_Power_Type))
	{	//This unit is Sink side
		if(IS_MASK_SET(TypeCStatus3,GotPartnerInformation))
		{
			UnitARSOCTemp = AnoutherUnitInformation[0] & 0x7F;
			UnitASxTemp = ((AnoutherUnitInformation[1]&0x03) << 1) + (AnoutherUnitInformation[0] >> 7);
			if(((i == 1) && (UnitASxTemp == 0))
				|| ((i != 1) && (UnitASxTemp != 0))
				)
			{	//Two units both are in Non-S0 or S0.
		//						      	RamDebug(0xE0);
				if(UnitARSOCTemp >= RelativeStateOfChgL)
				{RamDebug(0xA0);
		            if(UnitARSOCTemp > 30)
                    {RamDebug(0xA1);
                        if((UnitARSOCTemp - RelativeStateOfChgL) < 20)
                        {
                            P2P_Charge_Disable = 1;
                            TypeCAdpPdStableCheck = 0;
                            RTS5455WaitingPingID = 16;
                        }
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        //CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                    }
                    else
                    {RamDebug(0xA2);
                        //if((UnitARSOCTemp - RelativeStateOfChgL) > 20)
                       // {
                            P2P_Charge_Disable = 1;
                            TypeCAdpPdStableCheck = 0;
                            RTS5455WaitingPingID = 16;
                        //}
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        //CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                    }

				}
				else
				{RamDebug(0xA3);
                    if(RelativeStateOfChgL > 30)
                    {
                        if((RelativeStateOfChgL - UnitARSOCTemp) < 20)
                        {
                            P2P_Charge_Disable = 1;
                            TypeCAdpPdStableCheck = 0;
                            RTS5455WaitingPingID = 16;
                            CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                           // CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                        }
                        else
                        {
                            SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        }
                    }
                    else
                    {RamDebug(0xA5);
                        //if((RelativeStateOfChgL - UnitARSOCTemp) > 20)
                       // {
                            P2P_Charge_Disable = 1;
                            TypeCAdpPdStableCheck = 0;
                            RTS5455WaitingPingID = 16;
                       // }
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                       // CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                    }

				}
			}
	else
			{	//Two units' system status is different
		if(UnitASxTemp != 0)
				{	//Partner unit in non-S0
                    if((RelativeStateOfChgL > 30) && (UnitARSOCTemp < 10))
                    {
                        SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                    }
                    else
                    {
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        //CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                    }
        		}
        		else
				{	//This unit in non-S0

					if(RelativeStateOfChgL > 30)
					{
                        SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                    }
                    else
                    {
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        //CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                    }
		        }
	        }
			if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type_Target) != IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type))
			{	//Send PD command to swap the power type.
						SinkEnDisDelay = 101;
				if(PdPowerStableDelay > 1)
				{
					PdPowerStableDelay--;
				}
				else if(PdPowerStableDelay == 0)
				{
					PdPowerStableDelay = 101;
				}
				if(PdPowerStableDelay == 1)
				{
					if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type_Target))
					{
						if(IS_MASK_CLEAR(TypeCStatus3,P2PPowerSwapWait)&&(RTS5455WaitingPingID == 0))
						{
							SMB3_DATA[0] = RTS5455_INIT_PD_AMS & 0x00FF;
							SMB3_DATA[1] = RTS5455_PORT_NUM;
							SMB3_DATA[2] = RTS5455_INIT_PD_AMS_PR_SWAP;
							if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_INIT_PD_AMS>>8), &SMB3_DATA[0], RTS5455_INIT_PD_AMS_WR_DATA_LEN, SMBus_NoPEC))
							{
								TypeCCommFailCnt++;
							}
							else
							{
								TypeCCommFailCnt = 0;
								RTS5455WaitingPingID = 9;
								SET_MASK(TypeCStatus3,P2PPowerSwapWait);
								//SendUnitInformationToPdc();
							}
						}
						else if(RTS5455WaitingPingID == 9)
						{
							if(RTS5455WaitPingDelay == 0)
							{
								if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
								{
									TypeCCommFailCnt++;
									if(TypeCCommFailCnt == 3)
									{
							    	TypeCCommFailCnt = 0;
									}
								}
								else
								{
									TypeCCommFailCnt = 0;
									if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
									{
										RTS5455WaitingPingID = 0;
                                        ReTry_P2P = 0;
										//RTS5455WaitPingDelay = 10;
									}
									else
									{
										if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
										{	//Retry command
											RTS5455WaitingPingID = 0;
                                            if(ReTry_P2P < 3)
                                            {
											CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
											RTS5455WaitPingDelay = 10;
                                            }
										}
										//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
									}
								}
							}
							else if(RTS5455WaitPingDelay)
							{
								RTS5455WaitPingDelay--;
								if(RTS5455WaitPingDelay == 0)
								{
									//CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
									ReTry_P2P ++;
								}
							}
						}
					}
				}
			}
			else
			{
				RTS5455WaitPingDelay=10;
				PdPowerStableDelay=101;
				CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
				if(IS_MASK_CLEAR(TypeCStatus2,TypeC_Laptop_Power_Charge_Status) != IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Charge_Target))
				{
					if(SinkEnDisDelay > 1)
					{
						SinkEnDisDelay--;
					}
					else if(SinkEnDisDelay == 0)
					{
						SinkEnDisDelay = 101;
					}
					if(SinkEnDisDelay == 1)
					{
					if(IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Charge_Target))
					{	//Disable changing
							//SinkPowerPathControl(0);
				CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
					}
					else
					{	//Enable changing
							//SinkPowerPathControl(1);
				SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
						}
					}
				}
				else
				{
					SinkEnDisDelay = 0;
				}
			}
		}
		if((RelativeStateOfChgL != (OldInformation&0x7F))||((OldInformation>>7) != i))
		{
			SendUnitInformationToPdc();//qizhuang
		}
	}
	else
	{	//This unit is Source side
		RTS5455WaitPingDelay=10;
		PdPowerStableDelay=101;
		CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
		SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
		CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
		if((RelativeStateOfChgL != (OldInformation&0x7F))||((OldInformation>>7) != i))
		{
			SendUnitInformationToPdc();//qizhuang
		}
	}
}
/*
*******************************************************************************
* Function name: SendUnitInformationToPdc
*
* Descriptoin: Send unit information to another unit for P2P function
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void SendUnitInformationToPdc(void)
{
	BYTE i;
	if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
	|| IS_MASK_CLEAR(TypeCStatus2,TypeC_Laptop_Attached)
	|| IS_MASK_SET(PDStatus,TypeCFwUpdating)
	)
	{
		return;
	}
	if(RTS5455WaitingPingID == 0)
	{
		for(i=0;i<9;i++)//8
		{
			SMB3_DATA[i+1] = 0;
		}

	//Header
	SMB3_DATA[2] = 0x12;
	SMB3_DATA[3] = 0xA0;
	SMB3_DATA[4] = 0xEF;
	SMB3_DATA[5] = 0x17;

	//Set RSOC
	OldInformation = RelativeStateOfChgL;
	if((SysPowState == SYSTEM_S0)||(SysPowState == SYSTEM_S5_S0))
	{
		OldInformation |= 0x80;
	}
	SMB3_DATA[6] = RelativeStateOfChgL;

  switch (SysPowState)
  {	//Set power status
    case SYSTEM_S4 :
	    case SYSTEM_S0_S4 :
    case SYSTEM_S5 :
	    case SYSTEM_S0_S5 :
			SMB3_DATA[6] |= (0x05 << 7);
			SMB3_DATA[7] |= (0x05 >> 1);
      break;
    case SYSTEM_S3 :
	    case SYSTEM_S0_S3 :
			SMB3_DATA[6] |= (0x03 << 7);
				SMB3_DATA[7] |= (0x03 >> 1);
      break;
    case SYSTEM_S0 :
	    case SYSTEM_S3_S0 :
	    case SYSTEM_S4_S0 :
	    case SYSTEM_S5_S0 :
//			SMB3_DATA[6] |= (0x00 << 7);
//			SMB3_DATA[7] |= (0x00 >> 1);
      break;
    default :
      break;
  }
  if(ACPOWER_ON)
  {	//Set power mode
		SMB3_DATA[7] |= (0x01 << 2);
  }

	SMB3_DATA[0] = RTS5455_SEND_VDM & 0x00FF;
	SMB3_DATA[1] = RTS5455_PORT_NUM;
			      	//RamDebug(0xD1);
	if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SEND_VDM>>8), &SMB3_DATA[0], 10, SMBus_NoPEC))//8
	{
		TypeCCommFailCnt++;
	}
	else
		{
			RTS5455WaitingPingID = 8;
			TypeCCommFailCnt = 0;
			SENT_UNIT_RETRY_ReceiveByte();
		}
	}
/*	else if(RTS5455WaitingPingID == 8)
	{
		if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
		{
			TypeCCommFailCnt++;
			if(TypeCCommFailCnt == 3)
			{
	    	TypeCCommFailCnt = 0;
			}
		}
		else
		{
			//TypeCCommFailCnt = 0;
			if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
			{
			    TypeCCommFailCnt = 0;
				RTS5455WaitingPingID = 0;
				CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
			}
			else
			{
				if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
				{	//Retry command
					RTS5455WaitingPingID = 0;
                    TypeCCommFailCnt++;
                    if(TypeCCommFailCnt >= 5)
                    {
                        TypeCCommFailCnt = 0;
					    CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
                    }
				}
                else
                    {
                    TypeCCommFailCnt = 0;
				//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
				SET_MASK(TypeCStatus3,NeedUpdateToPartner);
                    }
			}
		}
	}*/




	/*else if(RTS5455WaitingPingID == 8)
		{		if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
		{			TypeCCommFailCnt++;
	if(TypeCCommFailCnt == 3)
		{	    	TypeCCommFailCnt = 0;
	}
	}
	else
		{			TypeCCommFailCnt = 0;
	if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
		{				RTS5455WaitingPingID = 0;
	CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
	}
	else
		{
		if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
			{	//Retry command
			RTS5455WaitingPingID = 0;
			//SET_MASK(TypeCStatus3,NeedUpdateToPartner);
			}
		//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
		SET_MASK(TypeCStatus3,NeedUpdateToPartner);
		}
	}
	}*/
}
/*
*******************************************************************************
* Function name: SinkPowerPathControl
*
* Descriptoin: Enable/Disable sink side power path from source side or sink side
*
* Arguments:
*       OnOff:1:Enable,0:Disable
* Return Values:
*       none
*******************************************************************************
*/
void SinkPowerPathControl(BYTE OnOff)
{
	BYTE i;
	if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
	|| IS_MASK_CLEAR(TypeCStatus2,TypeC_Laptop_Attached)
	|| IS_MASK_SET(PDStatus,TypeCFwUpdating)
	)
	{
		return;
	}
	if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type))
	{ //Send VDM command to enable/disable from source side to sink
		for(i=0;i<10;i++)//7
		{
			SMB3_DATA[i+1] = 0;
		}

		//Header
		SMB3_DATA[2] = 0x13;
		SMB3_DATA[3] = 0xA0;
		SMB3_DATA[4] = 0xEF;
		SMB3_DATA[5] = 0x17;

	  if(OnOff)
	  {
			SMB3_DATA[6] |= 0x01;
	  }

		SMB3_DATA[0] = RTS5455_SEND_VDM & 0x00FF;
		SMB3_DATA[1] = RTS5455_PORT_NUM;
				      	//RamDebug(0xD1);
		if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SEND_VDM>>8), &SMB3_DATA[0], 10, SMBus_NoPEC))//7
		{
			TypeCCommFailCnt++;
		}
		else
		{
			TypeCCommFailCnt = 0;
			RTS5455WaitingPingID=10;
			//bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]);
		 // if(OnOff)
		  //{
				//SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
		 // }
		 // else
		 // {
			//	CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
		  //}
		}
	}
	else
	{ //Send PD command to enable/disable from sink side
		if(RTS5455WaitingPingID == 0)
		{
		SMB3_DATA[0] = RTS5455_ENABLE_DISABLE_SINK & 0x00FF;
		SMB3_DATA[1] = RTS5455_PORT_NUM;
			SMB3_DATA[2] = 0;
	  if(OnOff)
	  {
			SMB3_DATA[2] |= 0x01;
	  }
		if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_ENABLE_DISABLE_SINK>>8), &SMB3_DATA[0], RTS5455_ENABLE_DISABLE_SINK_LEN, SMBus_NoPEC))
		{
			TypeCCommFailCnt++;
		}
		else
		{
				RTS5455WaitingPingID = 10;
				//bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]);
			TypeCCommFailCnt = 0;
		}
	}
		else if(RTS5455WaitingPingID == 10)
		{
			if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
			{
				TypeCCommFailCnt++;
				if(TypeCCommFailCnt == 3)
				{
		    	TypeCCommFailCnt = 0;
				}
			}
			else
			{
				TypeCCommFailCnt = 0;
				RTS5455WaitingPingID = 0;
				if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
				{
				 // if(OnOff)
				 // {
					//	SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
				 // }
				//  else
				// {
						//CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
				 // }
					RTS5455WaitingPingID = 0;
				}
				else
				{
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
					{	//Retry command
						RTS5455WaitingPingID = 0;
					}
					//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
				}
			}
		}
	}
}

BYTE UsbPdcRequest(BYTE usb_pdc_id, USB_PDC_REQ*request_packet, BYTE cmd, WORD parm1, WORD parm2, void *address) {

  BYTE        rc = USB_PDC_RC_OK;
  BYTE i;

  #if DEBUG_UCSI
  RamDebug(0xA1);
  #endif

  if ((0 < usb_pdc_id) && (NUMBER_OF_USB_PDC >= usb_pdc_id)) {
    /*
    *     valid USB PD ID
    */
//    int_mask = CmnGetIntMask();
//    CmnDisableInt();
#if DEBUG_UCSI
    RamDebug(0xA2);
#endif
    if ((TRUE == request_packet->status.busy)
        && (request_packet->status.done == FALSE)) {
      /*
      *   PDC is busy
      */
      rc = USB_PDC_RC_BUSY;
      #if DEBUG_UCSI_CMD
      RamDebug(0xA3);
      #endif
    }
    if (USB_PDC_RC_OK == rc) {
        #if DEBUG_UCSI
  	  RamDebug(0xA4);
      RamDebug(cmd);
      #endif
      /*
      *   request packet is enqueued then set up the packet
      */
      //request_packet->status.done  = FALSE;
      CLEAR_MASK(TypeCStatus3,PdcRequestDone);
      request_packet->status.busy  = TRUE;
      switch (cmd) {
        case USB_PDC_REQ_HARD_RESET:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xF0);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_TCPM_RESET>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_TCPM_RESET_WR_DATA_LEN;
                SMB3_DATA[0] = UCSI_RTS5455_CMD;
    			SMB3_DATA[1] = 0;
                SMB3_DATA[2] = RTS5455_TCPM_RESET & 0x00FF;
    			SMB3_DATA[3] = RTS5455_PORT_NUM;
    			SMB3_DATA[4] = RTS5455_TCPM_RESET_RESET_TYPE_PD_HARD_RESET;
            }
          break;
        case USB_PDC_REQ_SOFT_RESET:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xF1);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_TCPM_RESET>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_TCPM_RESET_WR_DATA_LEN;
                SMB3_DATA[0] = UCSI_RTS5455_CMD;
    			SMB3_DATA[1] = 0;
                SMB3_DATA[2] = RTS5455_TCPM_RESET & 0x00FF;
    			SMB3_DATA[3] = RTS5455_PORT_NUM;
    			SMB3_DATA[4] = RTS5455_TCPM_RESET_RESET_TYPE_PD_SOFT_RESET;
            }
          break;
        case USB_PDC_REQ_PPM_RESET:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xF1);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_PPM_RESET>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_PPM_RESET_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_PPM_RESET & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
            }
            break;
        case USB_PDC_REQ_SET_NOTIFICATION_ENABLE:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if 1
                        RamDebug(0xF2);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_SET_NOTIFICATION_ENABLE>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_NOTIFICATION_ENABLE_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_NOTIFICATION_ENABLE & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
				SMB3_DATA[2] = RTS5455_INT_EVENT_MASK_LL;
				SMB3_DATA[3] = RTS5455_INT_EVENT_MASK_LH;
				SMB3_DATA[4] = RTS5455_INT_EVENT_MASK_HL;
				SMB3_DATA[5] = RTS5455_INT_EVENT_MASK_HH;
                SMB3_DATA[2] |= UCSI_NOTIFICATION_ENABLE[0];
    			SMB3_DATA[3] |= UCSI_NOTIFICATION_ENABLE[1];
                //SMB3_DATA[4] = 0x00;
                //SMB3_DATA[5] = 0x00;
            }
            break;
        case USB_PDC_REQ_GET_CAPABILITY:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if 1
                        RamDebug(0xF3);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_GET_CAPABILITY>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_GET_CAPABILITY_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_GET_CAPABILITY & 0x00FF;
                SMB3_DATA[1] = RTS5455_PORT_NUM;
            }
            break;
        case USB_PDC_REQ_GET_CONNECTOR_CAPABILITY:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if 1
                        RamDebug(0xF4);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_GET_PORT_CAPABILITY>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_GET_PORT_CAPABILITY_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_GET_PORT_CAPABILITY & 0x00FF;
                SMB3_DATA[1] = RTS5455_PORT_NUM;
            }
            break;
        case USB_PDC_REQ_SWAP_TO_SOURCE:

           /* if(Usb_Pdc_Power_Status[0].power_source)
            {
                UCSI_POWER_SOURCE = 1;
                SET_MASK(TypeCStatus3,PdcRequestDone);
            }
            else
            {*/
                if(UCSI_RTS5455_CMD_WAIT == 1)
                {
                    request_packet->status.done  = TRUE;
                }
                else
                {
                #if 1
                        RamDebug(0xF5);
                    #endif
                    UCSI_RTS5455_CMD = (RTS5455_SET_PDR>>8);
                    UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_PDR_WR_DATA_LEN;
                    SMB3_DATA[0] = RTS5455_SET_PDR & 0x00FF;
        			SMB3_DATA[1] = RTS5455_PORT_NUM;
        			SMB3_DATA[2] = 1;
                    //TypeCAdpDetectDelay = 50;
                }
           // }
          break;
        case USB_PDC_REQ_SWAP_TO_SINK:

           /* if(Usb_Pdc_Power_Status[0].power_source)
            {*/
                if(UCSI_RTS5455_CMD_WAIT == 1)
                {
                    request_packet->status.done  = TRUE;
                }
                else
                {
                #if 1
                        RamDebug(0xF6);
                    #endif
                    UCSI_RTS5455_CMD = (RTS5455_SET_PDR>>8);
                    UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_PDR_WR_DATA_LEN;
                    SMB3_DATA[0] = RTS5455_SET_PDR & 0x00FF;
        			SMB3_DATA[1] = RTS5455_PORT_NUM;
        			SMB3_DATA[2] = 2;
                    //TypeCAdpDetectDelay = 50;
                }
           /* }
            else
            {
                UCSI_POWER_SOURCE = 0;
                SET_MASK(TypeCStatus3,PdcRequestDone);
            }*/
          break;
          case USB_PDC_REQ_SWAP_TO_ACCEPT_SOURCE_SINK:
            //RamDebug(0xF8);
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if 1
                        RamDebug(0xF7);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_SET_PDR>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_PDR_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_PDR & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 4;
            }
            break;
        case USB_PDC_REQ_SWAP_TO_DFP:
            //RamDebug(0xF9);
            //RamDebug(Usb_Pdc_Status[0].dfp);
            //RamDebug(UCSI_DATA_ROLE);
            /*if(Usb_Pdc_Status[0].dfp)
            {
                UCSI_DATA_ROLE = 1;
                SET_MASK(TypeCStatus3,PdcRequestDone);
            }
            else
            {*/
                if(UCSI_RTS5455_CMD_WAIT == 1)
                {
                    request_packet->status.done  = TRUE;
                }
                else
                {
                #if DEBUG_UCSI_CMD
                        RamDebug(0xF8);
                    #endif
                    UCSI_RTS5455_CMD = (RTS5455_SET_UOR>>8);
                    UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_UOR_WR_DATA_LEN;
                    SMB3_DATA[0] = RTS5455_SET_UOR & 0x00FF;
        			SMB3_DATA[1] = RTS5455_PORT_NUM;
        			SMB3_DATA[2] = 1;
                    //TypeCAdpDetectDelay = 50;
                }
            //}
          break;
        case USB_PDC_REQ_SWAP_TO_UFP:
            //RamDebug(0xFA);
            //RamDebug(Usb_Pdc_Status[0].dfp);
            //RamDebug(UCSI_DATA_ROLE);
            /*if(Usb_Pdc_Status[0].dfp)
            {*/
                if(UCSI_RTS5455_CMD_WAIT == 1)
                {
                    request_packet->status.done  = TRUE;
                }
                else
                {
                #if DEBUG_UCSI_CMD
                        RamDebug(0xF9);
                    #endif
                    UCSI_RTS5455_CMD = (RTS5455_SET_UOR>>8);
                    UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_UOR_WR_DATA_LEN;
                    SMB3_DATA[0] = RTS5455_SET_UOR & 0x00FF;
        			SMB3_DATA[1] = RTS5455_PORT_NUM;
        			SMB3_DATA[2] = 2;
                   // TypeCAdpDetectDelay = 50;
                }
           /* }
            else
            {
                UCSI_DATA_ROLE = 0;
                SET_MASK(TypeCStatus3,PdcRequestDone);
            }*/
          break;
        case USB_PDC_REQ_SWAP_TO_DRP:
            //RamDebug(0xFB);
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xFA);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_SET_UOR>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_UOR_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_UOR & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 4;
            }
          break;
       /* case USB_PDC_REQ_GET_RX_SOURCE_PDO:
          break;
        case USB_PDC_REQ_GET_RX_SINK_PDO:
         break;
        case USB_PDC_REQ_GET_TX_SOURCE_PDO:
          break;
        case USB_PDC_REQ_GET_TX_SINK_PDO:
          break;*/
          case USB_PDC_REQ_GET_PDO:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xFB);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_GET_PDO>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_GET_PDO_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_GET_PDO & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
                SMB3_DATA[2] = 0x00;
    			SMB3_DATA[2] = (UCSI_GET_PDOS_COMMAND_SOURCE_OR_SINK_PDOS |
                                (UCSI_GET_PDOS_COMMAND_PARTNER_PDOS << 1) |
                                ((UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07) << 2) |
                                ((UCSI_GET_PDOS_COMMAND_NUMBER_OF_PDOS & 0x07) << 5));
            }
            break;
          case USB_PDC_REQ_OPERATE_AS_DFP:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
                UCSI_RTS5455_CMD = (RTS5455_SET_UOM>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_UOM_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_UOM & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 1;
            }
          break;
          case USB_PDC_REQ_OPERATE_AS_UFP:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
                UCSI_RTS5455_CMD = (RTS5455_SET_UOM>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_UOM_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_UOM & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 2;
            }
          break;
          case USB_PDC_REQ_OPERATE_AS_DRP:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
                UCSI_RTS5455_CMD = (RTS5455_SET_UOM>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_UOM_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_UOM & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 4;
            }
          break;
          case USB_PDC_REQ_OPERATE_AS_SOURCE:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
                UCSI_RTS5455_CMD = (RTS5455_SET_PDM>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_PDM_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_PDM & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 1;
            }
          break;
          case USB_PDC_REQ_OPERATE_AS_SINK:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
                UCSI_RTS5455_CMD = (RTS5455_SET_PDM>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_PDM_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_PDM & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 2;
            }
          break;
          case USB_PDC_REQ_OPERATE_AS_ACCEPT_SOURCE_SINK:
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
                UCSI_RTS5455_CMD = (RTS5455_SET_PDM>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_PDM_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_PDM & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			SMB3_DATA[2] = 4;
            }
          break;
          case USB_PDC_REQ_GET_ALTERNATE_MODES:
            //RamDebug(0xFB);
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xFC);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_SET_ALTERNATE_MODE>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_SET_ALTERNATE_MODE_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_SET_ALTERNATE_MODE & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
                SMB3_DATA[2] = UCSI_GET_ALTERNATE_MODES_COMMAND_RECIPIENT;
    			SMB3_DATA[3] = UCSI_GET_ALTERNATE_MODES_COMMAND_ALTERNATE_MODE_OFFSET;
                SMB3_DATA[4] = (UCSI_GET_ALTERNATE_MODES_COMMAND_NUMBER_OF_ALTERNATE_MODES);
            }
          break;
          case USB_PDC_REQ_GET_CAM_SUPPORTED:
            //RamDebug(0xFB);
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xFD);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_GET_PAM_SUPPORTED>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_GET_PAM_SUPPORTED_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_GET_PAM_SUPPORTED & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
            }
          break;
          case USB_PDC_REQ_GET_CURRENT_CAM:
            //RamDebug(0xFB);
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xFE);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_GET_CURRENT_PAM>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_GET_CURRENT_PAM_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_GET_CURRENT_PAM & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
            }
          break;
          case USB_PDC_REQ_GET_CABLE_PROPERTY:
            //RamDebug(0xFB);
            if(UCSI_RTS5455_CMD_WAIT == 1)
            {
                request_packet->status.done  = TRUE;
            }
            else
            {
            #if DEBUG_UCSI_CMD
                        RamDebug(0xFF);
                    #endif
                UCSI_RTS5455_CMD = (RTS5455_GET_CABLE_PROPERTY>>8);
                UCSI_RTS5455_WR_DATA_LEN = RTS5455_GET_CABLE_PROPERTY_WR_DATA_LEN;
                SMB3_DATA[0] = RTS5455_GET_CABLE_PROPERTY & 0x00FF;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
            }
          break;
        }
        //if(request_packet->status.done == FALSE)
        if(IS_MASK_CLEAR(TypeCStatus3,PdcRequestDone))
        {
        #if DEBUG_UCSI
          RamDebug(0xA5);
          //RamDebug(request_packet->status.done);
          //RamDebug(request_packet->request);
          #endif
          if(request_packet->status.done == FALSE)
          {
          #if DEBUG_UCSI_CMD
          RamDebug(0xA6);
          //RamDebug(UCSI_RTS5455_CMD);
          RamDebug(SMB3_DATA[0]);
          RamDebug(SMB3_DATA[1]);
          RamDebug(SMB3_DATA[2]);
          #endif
              if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, UCSI_RTS5455_CMD, &SMB3_DATA[0], UCSI_RTS5455_WR_DATA_LEN, SMBus_NoPEC))
              {
              #if DEBUG_UCSI_CMD
                        RamDebug(0xEC);
                    #endif
                    TypeCCommFailCnt++;
                    if(TypeCCommFailCnt == 3)
					{
			    	    TypeCIniDelay = 200;
			    	    TypeCCommFailCnt = 0;
                        UCSI_RTS5455_CMD_ERROR = 1;
                        UCSI_RTS5455_CMD_WAIT = 0;
                        UCSI_RTS5455_CMD_ERROR_CNT = 0;
                        SET_MASK(TypeCStatus3,PdcRequestDone);
					}
              }
              else
              {
              #if 1
                        RamDebug(0xED);
                    #endif
                UCSI_RTS5455_CMD_WAIT = 1;
                UCSI_RTS5455_CMD_WAIT_CNT = 200;
                TypeCCommFailCnt = 0;
                /*if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                {RamDebug(SMB3_DATA[0]);

                    if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                    {
                        UCSI_RTS5455_CMD_WAIT = 0;
                        UCSI_RTS5455_CMD_ERROR = 0;
                        SET_MASK(TypeCStatus3,PdcRequestDone);
                        if(request_packet->request == USB_PDC_REQ_HARD_RESET)
                            CLEAR_MASK(TypeCStatus,TypeCIniOK);
                    }
                    else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                    {
                        UCSI_RTS5455_CMD_ERROR = 1;
                        SET_MASK(TypeCStatus3,PdcRequestDone);
                    }
                    else
                    {//SET_MASK(TypeCStatus3,PdcRequestDone);
                        UCSI_RTS5455_CMD_WAIT = 1;
                        UCSI_RTS5455_CMD_WAIT_CNT = 3;
                    }
                }*/
              }
          }
          else
          {
          #if DEBUG_UCSI_CMD
            RamDebug(0xA7);
            //RamDebug(request_packet->status.done);
            //RamDebug(request_packet->request);
            #endif
            TypeCCommFailCnt = 0;
            switch (request_packet->request) {
              case USB_PDC_REQ_HARD_RESET:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xA0);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;

                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                    #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                            //CLEAR_MASK(TypeCStatus,TypeCIniOK);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
              case USB_PDC_REQ_GET_CAPABILITY:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xA1);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                    #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {
                                for(i = 0;i < 16;i++)
                                {
                                    if(i == 5)
                                    {
                                    #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                                        Ucsi_message_in[i] = (SMB3_DATA[i]
                                        #if !_SUPPORT_SET_UOM_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_SET_UOM_SUPPORTED
                                        #endif // _SUPPORT_SET_UOM_
                                        #if !_SUPPORT_SET_PDM_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_SET_PDM_SUPPORTED
                                        #endif // _SUPPORT_SET_PDM_
                                        #if !_SUPPORT_ALT_MODE_DETAILS_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_ALTERNATE_MODE_DETAILS_SUPPORTED
                                        #endif // _SUPPORT_ALT_MODE_DETAILS_
                                        #if !_SUPPORT_ALT_MODE_OVERRIDE_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_ALTERNATE_MODE_OVERRIDE_SUPPORTED
                                        #endif // _SUPPORT_ALT_MODE_OVERRIDE_
                                        #if !_SUPPORT_PDO_DETAIL_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_PDO_DETAILS_SUPPORTED
                                        #endif // _SUPPORT_PDO_DETAIL_
                                        #if !_SUPPORT_CABLE_DETAILS_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_CABLE_DETAILS_SUPPORTED
                                        #endif // _SUPPORT_CABLE_DETAILS_
                                        #if !_SUPPORT_EXT_SUPPLY_NOTIFICATION_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_EXTERNAL_SUPPLY_NOTIFICATION_SUPPORTED
                                        #endif // _SUPPORT_EXT_SUPPLY_NOTIFICATION_
                                        #if !_SUPPORT_PD_RESET_NOTIFICATION_
                                        & ~UCSI_BM_OPTIONAL_FEATURES_PD_RESET_NOTIFICATION_SUPPORTED
                                        #endif // _SUPPORT_RESET_NOTIFICATION_
                                        );

                                        UCSI_OPTIONAL_FEATURE = Ucsi_message_in[i];
                                        #if DEBUG_UCSI_CMD
                                        RamDebug(Ucsi_message_in[i]);
                                        RamDebug(UCSI_OPTIONAL_FEATURE);
                                        #endif
                                    }
                                    else
                                    {
                                        Ucsi_message_in[i] = SMB3_DATA[i];
                                        #if DEBUG_UCSI_CMD
                                        RamDebug(Ucsi_message_in[i]);
                                        #endif
                                    }
                                }
                            }
                            Ucsi_cci[1] = 0x10;
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {RamDebug(0xA1);
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
              case USB_PDC_REQ_GET_CONNECTOR_CAPABILITY:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xA2);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                    #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {

                                Ucsi_message_in[0] = (SMB3_DATA[0]);
                                    /*#if !_SUPPORT_ALT_MODE_DETAILS_
                                    & ~UCSI_CONNECTOR_SUPPORTS_ALTERNATE_MODE
                                    #endif // _SUPPORT_ALT_MODE_DETAILS_
                                    );*/
                                Ucsi_message_in[1] = SMB3_DATA[1];
                             }
                            #if DEBUG_UCSI_CMD
                 		     RamDebug(Ucsi_message_in[0]);
                             RamDebug(Ucsi_message_in[1]);
                             #endif
                            Ucsi_cci[1] = 2;
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {RamDebug(0xA2);
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
              case USB_PDC_REQ_SOFT_RESET:
              case USB_PDC_REQ_PPM_RESET:
              case USB_PDC_REQ_SET_NOTIFICATION_ENABLE:
              case USB_PDC_REQ_SWAP_TO_ACCEPT_SOURCE_SINK:
              case USB_PDC_REQ_SWAP_TO_DRP:
              case USB_PDC_REQ_OPERATE_AS_DFP:
              case USB_PDC_REQ_OPERATE_AS_UFP:
              case USB_PDC_REQ_OPERATE_AS_SOURCE:
              case USB_PDC_REQ_OPERATE_AS_SINK:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xA3);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    //Detect_Adaptor_Watt_SKIP = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                     #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            //Detect_Adaptor_Watt_SKIP = 0;
                            if(cmd == USB_PDC_REQ_PPM_RESET)
                            {
                                Ucsi_Connector_Change_Status[0] = 0;
                                 Ucsi_Connector_Number_Changed = 0;
                                 UCSI_RTS5455_CMD_ERROR = 0;
                            }
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {RamDebug(0xA3);
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                               // Detect_Adaptor_Watt_SKIP = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
              case USB_PDC_REQ_SWAP_TO_SINK:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xA4);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    //Detect_Adaptor_Watt_SKIP = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                     #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            //Detect_Adaptor_Watt_SKIP = 0;
                            Usb_Pdc_Power_Status[0].power_source = 0;
                            UCSI_POWER_SOURCE = 0;
                            //UCSI_POWER_DIRECTION_CHANGE = 1;
                            //TypeCAdpDetectDelay = 0;
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {RamDebug(0xA4);
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                              //  Detect_Adaptor_Watt_SKIP = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
              case USB_PDC_REQ_SWAP_TO_SOURCE:
                 #if DEBUG_UCSI_CMD
                        RamDebug(0xA5);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                  //  Detect_Adaptor_Watt_SKIP = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                     #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                           // Detect_Adaptor_Watt_SKIP = 0;
                            Usb_Pdc_Power_Status[0].power_source = 1;
                            UCSI_POWER_SOURCE = 1;
                            //TypeCAdpDetectDelay = 0;
                            //UCSI_POWER_DIRECTION_CHANGE = 1;
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {RamDebug(0xA5);
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                              //  Detect_Adaptor_Watt_SKIP = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
              case USB_PDC_REQ_SWAP_TO_UFP:
                 #if DEBUG_UCSI_CMD
                        RamDebug(0xA6);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                     #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            Usb_Pdc_Status[0].dfp = 0;
                            UCSI_DATA_ROLE = 0;
                            //TypeCAdpDetectDelay = 0;
                            //UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
              case USB_PDC_REQ_SWAP_TO_DFP:
                 #if DEBUG_UCSI_CMD
                        RamDebug(0xA7);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                     #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            Usb_Pdc_Status[0].dfp = 1;
                            UCSI_DATA_ROLE = 1;
                            //TypeCAdpDetectDelay = 0;
                            //UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
               case USB_PDC_REQ_GET_ALTERNATE_MODES:
                 #if DEBUG_UCSI_CMD
                        RamDebug(0xA8);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                     #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            Ucsi_cci[1] = (SMB3_DATA[0] >> 2);
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            if(Ucsi_cci[1] > 1)
                                Ucsi_cci[1] = Ucsi_cci[1]-1;
                            //Ucsi_cci[1] = 0x10;
                            #if DEBUG_UCSI_CMD
                            RamDebug(Ucsi_cci[1]);
                            #endif
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {
                                for(i = 0;i < Ucsi_cci[1];i++)
                                {
                                    Ucsi_message_in[i] = SMB3_DATA[i];
                                    #if DEBUG_UCSI_CMD
                                    RamDebug(Ucsi_message_in[i]);
                                    #endif
                                }
                            }
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
                case USB_PDC_REQ_GET_CAM_SUPPORTED:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xA9);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                    #if DEBUG_UCSI_CMD
                    RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            Ucsi_cci[1] = (SMB3_DATA[0] >> 2);
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            if(Ucsi_cci[1] > 1)
                                Ucsi_cci[1] = Ucsi_cci[1]-1;
                            #if DEBUG_UCSI_CMD
                            RamDebug(Ucsi_cci[1]);
                            #endif
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {
                                for(i = 0;i < Ucsi_cci[1];i++)
                                {
                                    Ucsi_message_in[i] = SMB3_DATA[i];
                                    #if DEBUG_UCSI_CMD
                                    RamDebug(Ucsi_message_in[i]);
                                    #endif
                                }
                            }
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
                case USB_PDC_REQ_GET_CURRENT_CAM:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xAA);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                     #if DEBUG_UCSI_CMD
                RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            Ucsi_cci[1] = (SMB3_DATA[0] >> 2);
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                            if(Ucsi_cci[1] > 1)
                                Ucsi_cci[1] = Ucsi_cci[1]-1;
                            #if DEBUG_UCSI_CMD
                            RamDebug(Ucsi_cci[1]);
                            #endif
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {
                                for(i = 0;i < Ucsi_cci[1];i++)
                                {
                                    Ucsi_message_in[i] = SMB3_DATA[i];
                                   #if DEBUG_UCSI_CMD
                                    RamDebug(Ucsi_message_in[i]);
                                    #endif
                                }
                            }
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
             /* case USB_PDC_REQ_GET_RX_SOURCE_PDO:	//RX:Device;TX:PDC
                SET_MASK(TypeCStatus3,PdcRequestDone);
                break;
              case USB_PDC_REQ_GET_RX_SINK_PDO:
                SET_MASK(TypeCStatus3,PdcRequestDone);
                break;
              case USB_PDC_REQ_GET_TX_SOURCE_PDO:

                break;
              case USB_PDC_REQ_GET_TX_SINK_PDO:

              break;*/
              case USB_PDC_REQ_GET_PDO:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xAB);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    UCSI_RTS5455_CMD_ERROR_CNT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                    #if DEBUG_UCSI_CMD
                RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            UCSI_RTS5455_CMD_ERROR_CNT = 0;
                             if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {
                            #if DEBUG_UCSI_CMD
                                RamDebug(SMB3_DATA[0 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)]);
                                RamDebug(SMB3_DATA[1 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)]);
                                RamDebug(SMB3_DATA[2 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)]);
                                RamDebug(SMB3_DATA[3 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)]);
                                 #endif
                                if((SMB3_DATA[0 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)] == 0) && (SMB3_DATA[1 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)] == 0)
                                        && (SMB3_DATA[2 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)] == 0) && (SMB3_DATA[3 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)] == 0))
                                {
                                    Ucsi_message_in[0] = 0;
                                    Ucsi_message_in[1] = 0;
                                    Ucsi_message_in[2] = 0;
                                    Ucsi_message_in[3] = 0;
                                    Ucsi_cci[1] = 0x00;
                                }
                                else
                                {
                                    Ucsi_message_in[0] = SMB3_DATA[0 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)];
                                    Ucsi_message_in[1] = SMB3_DATA[1 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)];
                                    Ucsi_message_in[2] = SMB3_DATA[2 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)];
                                    Ucsi_message_in[3] = SMB3_DATA[3 + 4*(UCSI_GET_PDOS_COMMAND_PDO_OFFSET & 0x07)];
                                    Ucsi_cci[1] = 4;
                                }
                            }
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            UCSI_RTS5455_CMD_ERROR_CNT++;
                            UCSI_RTS5455_CMD_WAIT = 2;
                            if(UCSI_RTS5455_CMD_ERROR_CNT == 10)
                            {
                                UCSI_RTS5455_CMD_ERROR = 1;
                                UCSI_RTS5455_CMD_WAIT = 0;
                                UCSI_RTS5455_CMD_ERROR_CNT = 0;
                                SET_MASK(TypeCStatus3,PdcRequestDone);
                            }
                        }
                    }
                }
                break;
                case USB_PDC_REQ_GET_CABLE_PROPERTY:
                #if DEBUG_UCSI_CMD
                        RamDebug(0xAC);
                    #endif
                if(UCSI_RTS5455_CMD_WAIT_CNT == 0)
                {
                    UCSI_RTS5455_CMD_ERROR = 1;
                    UCSI_RTS5455_CMD_WAIT = 0;
                    SET_MASK(TypeCStatus3,PdcRequestDone);
                }
                else
                {
                    UCSI_RTS5455_CMD_WAIT_CNT--;
                    if(bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
                    {
                    #if DEBUG_UCSI_CMD
                RamDebug(SMB3_DATA[0]);
                    #endif
                        if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
                        {
                            Ucsi_cci[1] = 0x05;
                            UCSI_RTS5455_CMD_WAIT = 0;
							UCSI_RTS5455_CMD_WAIT_CNT=0;
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {
                                for(i = 0;i < Ucsi_cci[1];i++)
                                {
                                    Ucsi_message_in[i] = SMB3_DATA[i];
                                    #if DEBUG_UCSI_CMD
                                    RamDebug(Ucsi_message_in[i]);
                                    #endif
                                }
                            }
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                        else if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                        {
                            //UCSI_RTS5455_CMD_ERROR = 1;
                            Ucsi_cci[1] = 0x05;
                            UCSI_RTS5455_CMD_WAIT = 0;

                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                            {
                                for(i = 0;i < Ucsi_cci[1];i++)
                                {
                                    Ucsi_message_in[i] = SMB3_DATA[i];
                                    #if DEBUG_UCSI_CMD
                                    RamDebug(Ucsi_message_in[i]);
                                    #endif
                                }
                            }
                            SET_MASK(TypeCStatus3,PdcRequestDone);
                        }
                    }
                }
                break;
            }
          }
        }
//      request_packet->next         = NULL;
        request_packet->request      = cmd;
        request_packet->parameter[0] = parm1;
        request_packet->parameter[1] = parm2;
        request_packet->address      = address;
//      request_packet->call_back    = call_back;
      request_packet->status.done  = FALSE;
      request_packet->status.busy  = FALSE;
      }
//    CmnSetIntMask(int_mask);
  } else {
    /*
    *    invalid USB PD ID
    */
    #if DEBUG_UCSI
    RamDebug(0xAF);
    #endif
    request_packet->status.done = TRUE;
    request_packet->status.busy = FALSE;
    rc                          = USB_PDC_RC_NOT_SUPPORTED;
  }
  if (USB_PDC_RC_OK == rc) {
//    TskActivateTask(TASK_USB_PDC_0 + usb_pdc_id);
  }
  return rc;
}
void GetConnectorStatus_RTS5455(BYTE state)
{
    BYTE i;

    //RamDebug(0xCC);

    SMB3_DATA[0] = RTS5455_GET_CONNECTOR_STATUS & 0x00FF;
        SMB3_DATA[1] = RTS5455_PORT_NUM;
         if(bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_GET_CONNECTOR_STATUS>>8), &SMB3_DATA[0], RTS5455_GET_CONNECTOR_STATUS_WR_DATA_LEN, SMBus_NoPEC))
    {
    Delay1MS(50);
            bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]);
            //RamDebug(0xFF);
            //RamDebug(SMB3_DATA[0]);
            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_Addr, RTS5455_READ_TRANSFER_DATA, &SMB3_DATA[0]))
                {

            /*RamDebug(SMB3_DATA[0]);
            RamDebug(SMB3_DATA[1]);
            RamDebug(SMB3_DATA[2]);
            RamDebug(SMB3_DATA[3]);
            RamDebug(SMB3_DATA[4]);
            RamDebug(SMB3_DATA[5]);
            RamDebug(SMB3_DATA[6]);
            RamDebug(SMB3_DATA[7]);
            RamDebug(SMB3_DATA[8]);*/
            for(i = 0;i < 9;i++)
                {
                UCSI_GET_CONNECTOR_STATUS_RTS5455[i] = SMB3_DATA[i];
                }
            RamDebug(UCSI_GET_CONNECTOR_STATUS_RTS5455[2]);
                }
             }
}

void SET_TPC_RECONNECT_RTS5455(BYTE state)
{
					SMB3_DATA[0] = RTS5455_SET_TPC_RECONNECT&0x00ff;
					SMB3_DATA[1] = RTS5455_PORT_NUM;
					SMB3_DATA[2] = 0x01;
					if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SET_TPC_RECONNECT>>8), &SMB3_DATA[0], RTS5455_GET_STATUS_WR_DATA_LEN, SMBus_NoPEC))
					{
						TypeCCommFailCnt++;
						if(TypeCCommFailCnt == 3)
						{

				    	TypeCCommFailCnt = 0;
						}
					}
					else
					{
						TypeCCommFailCnt = 0;


					}
}



void SENT_PD_RETRY_ReceiveByte(void)
{

if(RTS5455WaitingPingID==12)
{
RamDebug(0xA3);
if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
		{	    TypeCCommFailCnt++;
				RamDebug(0x94);
	if(TypeCCommFailCnt == 3)
		{	    	
		TypeCCommFailCnt = 0;
		}
	    }
		
	else
		{			

	if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
		{				
		RTS5455WaitingPingID = 0;
		RamDebug(0xA5);
		}
	else
		{
		RamDebug(0xA6);
		RTS5455WaitingPingID = 12;
		TypeCAdpDetectDelay=50;
		if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
			{	//Retry command
			RTS5455WaitingPingID = 12;
			RamDebug(0x96);
			
			}
		}
		}
}
}


void SENT_UNIT_RETRY_ReceiveByte(void)
{

if(RTS5455WaitingPingID == 8)
{
if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
		{			TypeCCommFailCnt++;
		
	if(TypeCCommFailCnt == 3)
		{	    	TypeCCommFailCnt = 0;
		}
	  }
	else
		{			
	
	
	if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
		{				RTS5455WaitingPingID = 0;
	CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
	}
	else
		{
		RTS5455WaitingPingID = 8;
	//	SET_MASK(TypeCStatus3,NeedUpdateToPartner);
		//CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
		if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
			{	//Retry command
			RTS5455WaitingPingID = 0;
			//SET_MASK(TypeCStatus3,NeedUpdateToPartner);
			//CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
			}
		}
		}
}
}

void SendPdoSet_ReceiveByte(void)
{
if(RTS5455WaitingPingID==13)
{
if( !Read_AC_IN())
{RTS5455WaitingPingID = 0;}
if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
		{	    TypeCCommFailCnt++;
				
	if(TypeCCommFailCnt == 3)
		{	    	
		TypeCCommFailCnt = 0;
		}
	    }
		
	else
		{			

	if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
		{				
		RTS5455WaitingPingID = 0;
		}
	else
		{
		RTS5455WaitingPingID = 13;
		//TypeCAdpDetectDelay=50;
		if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
			{	//Retry command
			RTS5455WaitingPingID = 0;
		
			
			}
		}
		}
}

}

void P2P_Charge_Condition(void)
{

    if(RTS5455WaitingPingID == 16)
    {
        SMB3_DATA[0] = 0;
        SMB3_DATA[1] = 0;
        SMB3_DATA[2] = Disable_Sink;
        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, 0x15, &SMB3_DATA[0], 3, SMBus_NoPEC))
        {
        	TypeCCommFailCnt++;
            if(TypeCCommFailCnt == 3)
            {RamDebug(0xFF);
                TypeCCommFailCnt = 0;
                //P2P_Charge_Disable = 0;
            }
        }
        else
        {
            RamDebug(0xD0);
        	TypeCCommFailCnt = 0;
        	RTS5455WaitingPingID = 11;
			CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
        }
    }
    else if(RTS5455WaitingPingID == 11)
    {
        if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
        {
            TypeCCommFailCnt++;
            if(TypeCCommFailCnt == 3)
            {RamDebug(0xEE);
                TypeCCommFailCnt = 0;
                //P2P_Charge_Disable = 0;
            }
        }
        else
        {
            if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
            {RamDebug(0xD1);
                RTS5455WaitingPingID = 0;
                P2P_Charge_Disable = 0;
				CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
            }
            else
            {
                if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
                {	//Retry command
                RamDebug(0xD2);
                    RTS5455WaitingPingID = 0;
					P2P_Charge_Disable = 0;
					CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                }
            }
        }
    }
}

void Sinkpower_ReceiveByte(void)
{ if(RTS5455WaitingPingID == 10)
		{
			if(!bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]))
			{
				TypeCCommFailCnt++;
				if(TypeCCommFailCnt == 3)
				{
		    	TypeCCommFailCnt = 0;
				}
			}
			else
			{
				TypeCCommFailCnt = 0;
				RTS5455WaitingPingID = 0;
				if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE)
				{
				 // if(OnOff)
				 // {
					//	SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
				 // }
				//  else
				// {
						//CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
				 // }
					RTS5455WaitingPingID = 0;
				}
				else
				{
					if(RTS5455_GET_PING_STATUS_CMD_STS_MASK(SMB3_DATA[0]) == RTS5455_GET_PING_STATUS_CMD_STS_ERROR)
					{	//Retry command
						RTS5455WaitingPingID = 0;
					}
					//If the reply is 0x00 or 0x02, that means busy, need waiting and do not re-send command.
				}
			}
}
}
#endif //Support_TYPE_C