/*
********************************************************************************
* Lenovo Confidential
*
* Microcode Source Materials
*
* ThinkPad Embedded Controller Microcode
*
* (C) COPYRIGHT LENOVO 2016 ALL RIGHTS RESERVED
*
* Filename: OEM_UCSI.c
*
* Description:
*
*      USB Type-C System Interface
*
* History:
*    10/27/2016    Tetsuji Nakamura
*       created as new
********************************************************************************
*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
#if Support_UCSI
/*
*******************************************************************************
*       Local Macros
*******************************************************************************
*/

//#define UCSI_OPTIONAL_FEATURE (UCSI_BM_OPTIONAL_FEATURES_PDO_DETAILS_SUPPORTED) //(1 << 0)  // Isaac check
#define UCSI_ATTRIBUTE (UCSI_BM_ATTRIBUTES_USB_POWER_DELIVERY | UCSI_BM_ATTRIBUTES_USB_TYPE_C_CURRENT | \
                        UCSI_BM_ATTRIBUTES_POWER_SOURCE_USES_VBUS)



/*
*******************************************************************************
*       Local Data Type
*******************************************************************************
*/

enum {
    UCSI_REQUEST_NONE,
    UCSI_REQUEST_HARD_RESET,
    UCSI_REQUEST_SOFT_RESET,
    UCSI_REQUEST_PPM_RESET,
    UCSI_REQUEST_SET_NOTIFICATION_ENABLE,
    UCSI_REQUEST_GET_CAPABILITY,
    UCSI_REQUEST_GET_CONNECTOR_CAPABILITY,
    UCSI_REQUEST_GET_PDOS,
    UCSI_REQUEST_SWAP_TO_SINK,
    UCSI_REQUEST_SWAP_TO_SOURCE,
    UCSI_REQUEST_SWAP_TO_ACCEPT_SOURCE_SINK,
    UCSI_REQUEST_SWAP_TO_UFP,
    UCSI_REQUEST_SWAP_TO_DFP,
    UCSI_REQUEST_SWAP_TO_DRP,
    UCSI_REQUEST_OPERATE_AS_DFP,
    UCSI_REQUEST_OPERATE_AS_UFP,
    UCSI_REQUEST_OPERATE_AS_SOURCE,
    UCSI_REQUEST_OPERATE_AS_SINK,
    UCSI_REQUEST_GET_ALTERNATE_MODES,
    UCSI_REQUEST_GET_CAM_SUPPORTED,
    UCSI_REQUEST_GET_CURRENT_CAM,
    UCSI_REQUEST_GET_CABLE_PROPERTY,
};

enum {
    UCSI_INIT,
    UCSI_WAIT_REQ,
    UCSI_SEND_REQUEST,
    UCSI_WAIT_REQUEST_DONE,
};

struct _UCSI_FLAG {
    BYTE busy:        1;
    BYTE cancel:      1;
};

typedef struct _UCSI_FLAG UCSI_FLAG;

struct _UCSI_REQUEST {
    BYTE request:          8;
    BYTE connector:        8;
    BYTE offset:           8;
    BYTE number_of_pdo:    8;
};

typedef struct _UCSI_REQUEST UCSI_REQUEST;

struct _USB_PDC_CHARGE_STATUS {
    BOOL quick_charge:   1;
    BOOL trickle_charge: 1;
	BOOL slow_charge: 1;
};

typedef struct _USB_PDC_CHARGE_STATUS USB_PDC_CHARGE_STATUS;

typedef enum {POWER_SOURCE_TYPE_NONE,
              POWER_SOURCE_TYPE_FIXED_SUPPLY,
              POWER_SOURCE_TYPE_BATTERY,
              POWER_SOURCE_TYPE_VARIABLE_SUPPLY} POWER_SOURCE_TYPE;

/*
*******************************************************************************
*       Local Variable
*******************************************************************************
*/

//static UCSI_DATA_STRUCTURE   Ucsi_Data;
static UCSI_FLAG             Ucsi_Flag;
static UCSI_REQUEST          Ucsi_Request, Ucsi_Request_Executing;
static WORD                  Ucsi_Error_Status;
//static BYTE                 Ucsi_Connector_Number_Changed;
static WORD                Ucsi_Notification_Enable;
//static WORD                Ucsi_Connector_Change_Status[NUMBER_OF_USB_PDC];
static BYTE                 Fsm;
//static USB_POWER_STATUS      Usb_Pdc_Power_Status[NUMBER_OF_USB_PDC];
//static USB_STATUS            Usb_Pdc_Status[NUMBER_OF_USB_PDC];
static USB_PDC_CHARGE_STATUS Usb_Pdc_Charge_Status[NUMBER_OF_USB_PDC];
//static BYTE                  UCSI_OPTIONAL_FEATURE;
//static BYTE                  Ucsi_Device_Connected;
static BYTE                  Ucsi_Error_Set;

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

static void Ucsi_Usb_Pdc_Req_Done(BYTE result);

/*
*******************************************************************************
*       Global Function Declarations
*******************************************************************************
*/

//extern void UcsiInit(void);
//extern BOOL UcsiHousekeeper(BYTE task_id);

/*
*******************************************************************************
* Function name: UcsiInit
*
* Descriptoin:
*          This is invoked when the system is powered on
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void UcsiInit(void) {
    Ucsi_version        = 0x0100;
    Fsm                 = 0;
}

/*
*******************************************************************************
* Function name: UcsiHousekeeper
*
* Descriptoin:
*          This is used to control USB-C PD controllers.
*
* Arguments:
*       int task_id
* Return Values:
*       BOOL : TRUE if allow to enter deep sleep
*******************************************************************************
*/

void UcsiHousekeeper(BYTE task_id) {
    BOOL               rc;
    static USB_PDC_REQ pdc_req;
    BYTE              cmd, i, index;//, partner_type;
    BYTE                counter;
    WORD             current, voltage, Tempdata;
    LWORD	capability;

    rc = FALSE;
    switch (Fsm) {
        case UCSI_INIT:
             /*
             *   Initialize valiables and status of PDC
             */
             if(IS_MASK_SET(TypeCStatus,TypeCIniOK))
             {

              #if DEBUG_UCSI_CMD
            RamDebug(0xE0);
            #endif
	             Ucsi_cci[0]                  = 0;
	             Ucsi_cci[1]                  = 0;
	             Ucsi_cci[2]                  = 0;
	             Ucsi_cci[3]                  = 0;
	             Ucsi_Request.request           = UCSI_REQUEST_NONE;
	             Ucsi_Request_Executing.request = UCSI_REQUEST_NONE;
	             Fsm++;
	           }
             break;
        case UCSI_WAIT_REQ:
		//	UCSIEVENTDEBOUNCE=UCSIEVENTDEBOUNCE+1;
            // // if(UCSIEVENTDEBOUNCE==5)
              {
            //  UCSIEVENTDEBOUNCE=0;
                if(Usb_Pdc_Status[0].device_connected == TRUE)
                {
                    if(!Ucsi_Device_Connected)
                    {
                    #if DEBUG_UCSI_CMD
                        RamDebug(0xEA);
                    #endif
					 if(UCSI_SUPPORTED_CAM_CHANGE)
					 	{RamDebug(0xB0);
						 Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                         UCSI_SUPPORTED_CAM_CHANGE = 0;
					 	}
                        Ucsi_Connector_Change_Status[0] |= UCSI_CONNECTOR_STATUS_CHANGE_CONNECT;
                             // if(UCSI_BATTERY_CHARGE_STATUS_CHANGE)
                       // {

                           // Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_BATTERY_CHARGE_STATUS | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                           // Ucsi_Connector_Number_Changed = 1;
                           // UCSI_BATTERY_CHARGE_STATUS_CHANGE = 0;
                             // 	}
                        Ucsi_Connector_Number_Changed = 1;
                        Ucsi_Device_Connected = 1;
                        UCSI_POWER_SOURCE = Usb_Pdc_Power_Status[0].power_source;
                        UCSI_DATA_ROLE = Usb_Pdc_Status[0].dfp;
                        ECSMI_SCIEvent(AM_UCSI_EVENT);
						 RamDebug(0x91);
                    }
                    if(!Ucsi_Connector_Number_Changed)
                    {
                        if(UCSI_POWER_DIRECTION_CHANGE)
                        {
                             #if DEBUG_UCSI_CMD
                            RamDebug(0xE1);
                            #endif
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            Ucsi_Connector_Number_Changed = 1;
                            UCSI_POWER_DIRECTION_CHANGE = 0;
                            ECSMI_SCIEvent(AM_UCSI_EVENT);
							 RamDebug(0x92);
                        }
                        if(UCSI_CONNECTOR_PARTNER_CHANGE)
                        {
                            #if DEBUG_UCSI_CMD
                            RamDebug(0xE2);
                            #endif
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            Ucsi_Connector_Number_Changed = 1;
                            UCSI_CONNECTOR_PARTNER_CHANGE = 0;
                            ECSMI_SCIEvent(AM_UCSI_EVENT);
							 RamDebug(0x93);
                        }
                        /*if(UCSI_NEGOTIATED_POWER_LEVEL_CHANGE)
                        {
                            RamDebug(0xE3);
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_NEGOTIATED_POWER_LEVEL | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            Ucsi_Connector_Number_Changed = 1;
                            UCSI_NEGOTIATED_POWER_LEVEL_CHANGE = 0;
                            ECSMI_SCIEvent(AM_UCSI_EVENT);
                        }*/
                        if(UCSI_BATTERY_CHARGE_STATUS_CHANGE)
                        {
                             #if DEBUG_UCSI_CMD
                            RamDebug(0xE3);
                            #endif
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_BATTERY_CHARGE_STATUS | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            Ucsi_Connector_Number_Changed = 1;
                            UCSI_BATTERY_CHARGE_STATUS_CHANGE = 0;
                            ECSMI_SCIEvent(AM_UCSI_EVENT);
							 RamDebug(0x94);
                        }
                        /*if(UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE)
                        {
                             RamDebug(0xE5);
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORTED_PROVIDER_CAPABILITIES | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            Ucsi_Connector_Number_Changed = 1;
                            UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE = 0;
                            ECSMI_SCIEvent(AM_UCSI_EVENT);
                        }*/
                        /*if(UCSI_PD_RESET_COMPLETE_CHANGE)
                        {
                             RamDebug(0xE5);
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_PD_RESET_COMPLETE | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            Ucsi_Connector_Number_Changed = 1;
                            UCSI_PD_RESET_COMPLETE_CHANGE = 0;
                            ECSMI_SCIEvent(AM_UCSI_EVENT);
                        }*/
                        if(UCSI_SUPPORTED_CAM_CHANGE)
                        {
                             #if DEBUG_UCSI_CMD
                            RamDebug(0xE4);
                            #endif
                            Ucsi_Connector_Change_Status[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            Ucsi_Connector_Number_Changed = 1;
                            UCSI_SUPPORTED_CAM_CHANGE = 0;
                            ECSMI_SCIEvent(AM_UCSI_EVENT);
							 RamDebug(0x95);
                        }
                    }
                  }
                else
                {
                    if(Ucsi_Device_Connected)
                    {
                    #if DEBUG_UCSI_CMD
                        RamDebug(0xEB);
                    #endif
                        Ucsi_Connector_Change_Status[0] |=UCSI_CONNECTOR_STATUS_CHANGE_CONNECT;
                        Ucsi_Connector_Number_Changed = 1;
                        Ucsi_Device_Connected = 0;
                        ECSMI_SCIEvent(AM_UCSI_EVENT);
						 RamDebug(0x96);
                    }

                }



             if (UCSI_REQUEST_NONE != Ucsi_Request.request) {
             //RamDebug(0xC1);
             //RamDebug(Ucsi_Request.request);
                 /*
                 *   a request is set then start to execute
                 */
                 Ucsi_Request_Executing = Ucsi_Request;
                 Fsm++;
             } else {
                 rc = TRUE;
             }
			 }
             break;
        case UCSI_SEND_REQUEST:
             /*
             *    select a command of USB PD Controller module according to the request
             */
             #if DEBUG_UCSI
             RamDebug(0xE5);
             RamDebug(Ucsi_Request_Executing.request);
             #endif
             switch (Ucsi_Request_Executing.request) {
                 case UCSI_REQUEST_HARD_RESET:
                      cmd   = USB_PDC_REQ_HARD_RESET;
                      break;
                 case UCSI_REQUEST_SOFT_RESET:
                      cmd   = USB_PDC_REQ_SOFT_RESET;
                      break;
                 case UCSI_REQUEST_SET_NOTIFICATION_ENABLE:
                      cmd   = USB_PDC_REQ_SET_NOTIFICATION_ENABLE;
                      break;
                 case UCSI_REQUEST_PPM_RESET:
                      cmd   = USB_PDC_REQ_PPM_RESET;
                      break;
                 case UCSI_REQUEST_GET_CAPABILITY:
                      cmd   = USB_PDC_REQ_GET_CAPABILITY;
                      break;
                 case UCSI_REQUEST_GET_CONNECTOR_CAPABILITY:
                      cmd   = USB_PDC_REQ_GET_CONNECTOR_CAPABILITY;
                      break;
                 case UCSI_REQUEST_GET_PDOS:
                      cmd   = USB_PDC_REQ_GET_PDO;
                      break;
                 case UCSI_REQUEST_SWAP_TO_SINK:
                      cmd   = USB_PDC_REQ_SWAP_TO_SINK;
                      break;
                 case UCSI_REQUEST_SWAP_TO_SOURCE:
                      cmd   = USB_PDC_REQ_SWAP_TO_SOURCE;
                      break;
                 case UCSI_REQUEST_SWAP_TO_ACCEPT_SOURCE_SINK:
                      cmd   = USB_PDC_REQ_SWAP_TO_ACCEPT_SOURCE_SINK;
                      break;
                 case UCSI_REQUEST_SWAP_TO_UFP:
                      cmd   = USB_PDC_REQ_SWAP_TO_UFP;
                      break;
                 case UCSI_REQUEST_SWAP_TO_DFP:
                      cmd   = USB_PDC_REQ_SWAP_TO_DFP;
                      break;
                 case UCSI_REQUEST_SWAP_TO_DRP:
                      cmd   = USB_PDC_REQ_SWAP_TO_DRP;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_DFP:
                      cmd   = USB_PDC_REQ_OPERATE_AS_DFP;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_UFP:
                      cmd   = USB_PDC_REQ_OPERATE_AS_UFP;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_SOURCE:
                      cmd   = USB_PDC_REQ_OPERATE_AS_SOURCE;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_SINK:
                      cmd   = USB_PDC_REQ_OPERATE_AS_SINK;
                      break;
                 case UCSI_REQUEST_GET_ALTERNATE_MODES:
                      cmd   = USB_PDC_REQ_GET_ALTERNATE_MODES;
                      break;
                 case UCSI_REQUEST_GET_CAM_SUPPORTED:
                      cmd   = USB_PDC_REQ_GET_CAM_SUPPORTED;
                      break;
                 case UCSI_REQUEST_GET_CURRENT_CAM:
                      cmd   = USB_PDC_REQ_GET_CURRENT_CAM;
                      break;
                 case UCSI_REQUEST_GET_CABLE_PROPERTY:
                      cmd   = USB_PDC_REQ_GET_CABLE_PROPERTY;
                      break;
                 default:
                      /*
                      *    Request may be canceled
                      */
                      if ((Ucsi_Request.request   == Ucsi_Request_Executing.request)   &&
                          (Ucsi_Request.connector == Ucsi_Request_Executing.connector) &&
                          (Ucsi_Request.offset    == Ucsi_Request_Executing.offset)){
                          /*
                          *     New Request have not come
                          */
                          Ucsi_cci[3]          &= ~UCSI_CCI_BUSY_INDICATOR;
                          Ucsi_cci[3]          |= UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
                          if (0 != (Ucsi_Notification_Enable & UCSI_COMMAND_COMPLETE_NOTIFICATION_ENABLE)) {
                              ECSMI_SCIEvent(AM_UCSI_EVENT);
							   RamDebug(0x97);
                          }
                          Ucsi_Request.request   = UCSI_REQUEST_NONE;
                          Ucsi_Request_Executing = Ucsi_Request;
                      } else {
                          Ucsi_Request_Executing.request = UCSI_REQUEST_NONE;
                      }
             }
             if (UCSI_REQUEST_NONE != Ucsi_Request_Executing.request) {
                 /*
                 *    send a request to a USB-C PDC
                 */
             //RamDebug(0xC3);
             //RamDebug(pdc_req.request);
                 UsbPdcRequest(Ucsi_Request_Executing.connector, &pdc_req, cmd,sizeof(Ucsi_message_in), 0, Ucsi_message_in);
                //RamDebug(0xA9);
                 //RamDebug(UCSI_RTS5455_CMD_WAIT);
                 //RamDebug(IS_MASK_SET(TypeCStatus3,PdcRequestDone));
                  if(UCSI_RTS5455_CMD_WAIT == 0)
                    Fsm++;
                  //RamDebug(Fsm);
             } else {
                 Fsm = UCSI_WAIT_REQ;
             }
             break;
        case UCSI_WAIT_REQUEST_DONE:
             //if (TRUE == pdc_req.status.done) {
             //RamDebug(0xAA);
             //RamDebug(IS_MASK_SET(TypeCStatus3,PdcRequestDone));
             if (IS_MASK_SET(TypeCStatus3,PdcRequestDone)) {
             //RamDebug(0xC4);
                 /*
                 *    the request has been done
                 */
                 if ((Ucsi_Request.request   == Ucsi_Request_Executing.request)   &&
                     (Ucsi_Request.connector == Ucsi_Request_Executing.connector) &&
                     (Ucsi_Request.offset    == Ucsi_Request_Executing.offset)){
                     /*
                     *     New Request have not come
                     */
             //RamDebug(0xC5);
                     if ((USB_PDC_RC_OK == pdc_req.result)&&(UCSI_RTS5455_CMD_ERROR == 0)) {
						             Ucsi_cci[0] = 0;
						             //Ucsi_cci[1] = 0;
						             Ucsi_cci[2] = 0;
						             Ucsi_cci[3] = 0;
             //RamDebug(0xC6);
             //RamDebug(Ucsi_Request_Executing.request);
                     } else {
                         /*
                         *     Done with an error
                         */
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR;
                         Ucsi_cci[0]     = 0;
                         Ucsi_cci[1]     = 0;
                         Ucsi_cci[2]     = 0;
                         Ucsi_cci[3]     = (UCSI_CCI_ERROR_INDICATOR | UCSI_CCI_COMMAND_COMPLETE_INDICATOR);
                         UCSI_RTS5455_CMD_ERROR = 0;
                     }
                     /*
                     *   set completed
                     */
                     Ucsi_cci[3]          &= ~UCSI_CCI_BUSY_INDICATOR;
                     Ucsi_cci[3]          |= UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
                     if (0 != (Ucsi_Notification_Enable & UCSI_COMMAND_COMPLETE_NOTIFICATION_ENABLE)) {
                         ECSMI_SCIEvent(AM_UCSI_EVENT);
						  RamDebug(0x98);
						  RamDebug(SYS_MISC1);
                     }
                     Ucsi_Request.request   = UCSI_REQUEST_NONE;
                     Ucsi_Request_Executing = Ucsi_Request;
                 } else {
                     /*
                     *     New Request have come then ignore the result
                     */
                     Ucsi_Request_Executing.request = UCSI_REQUEST_NONE;
				             Ucsi_cci[0] = 0;
				             Ucsi_cci[1] = 0;
				             Ucsi_cci[2] = 0;
				             Ucsi_cci[3] = 0;
                 }
                 Fsm = UCSI_WAIT_REQ;
             } else {
                 /*
                 *   the request is not done yet
                 */
                 #if DEBUG_UCSI_CMD
                RamDebug(0xE6);
                #endif
                 Fsm--;
                 rc = TRUE;
             }
             break;
            default:
             break;
    }
    if (TRUE == rc) {
        ;//TskWaitTask(task_id);
    }
    //return rc;
}

/*
*******************************************************************************
* Function name: Ucsi_Usb_Pdc_Req_Done
*
* Descriptoin:
*          This is invoked when a request to USB-C PDC is done
*
* Arguments:
*       UCHAR result
* Return Values:
*       none
*******************************************************************************
*/

static void Ucsi_Usb_Pdc_Req_Done(BYTE result) {

    //TskActivateTask(TASK_UCSI);
}

/*
*******************************************************************************
* Function name: UcsiGetVersion
*
* Descriptoin:
*          This is used to get contents of version area.
*
* Arguments:
*       none
* Return Values:
*       USHORT : version
*******************************************************************************
*/

WORD UcsiGetVersion(void) {
    return Ucsi_version;
}

/*
*******************************************************************************
* Function name: UcsiGetCci
*
* Descriptoin:
*          This is used to get contents of CCI area.
*
* Arguments:
*       none
* Return Values:
*       USHORT : version
*******************************************************************************
*/

BYTE  UcsiGetCci(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;
    Ucsi_cci[0] = (Ucsi_cci[0] & ~UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK) |
        ((Ucsi_Connector_Number_Changed << UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_SHIFT) &
         UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK);
    //RamDebug(Ucsi_control[0]);
    if(Ucsi_control[0] == UCSI_COMMAND_PPM_RESET)
     {
        RamDebug(0xEE);
        RamDebug(Ucsi_cci[0]);
        RamDebug(Ucsi_cci[3]);
        Ucsi_cci[0]        = 0;
        Ucsi_cci[1]        = 0;
        Ucsi_cci[2]        = 0;
        Ucsi_cci[3]        = UCSI_CCI_RESET_COMPLETE_INDICATOR;
     }
	// if(Ucsi_control[0] == UCSI_COMMAND_SET_PDM)
	// 	{
	// 	RamDebug(0xEF);
    //    RamDebug(Ucsi_cci[0]);
    //    RamDebug(Ucsi_cci[1]);
	//	 RamDebug(Ucsi_cci[3]);

	 //	}
    for (; (max_length > rc) && (sizeof(Ucsi_cci) > rc); rc++) {
        TempData[rc] = Ucsi_cci[rc];
    }
    return rc;
}

/*
*******************************************************************************
* Function name: UcsiSetControl
*
* Descriptoin:
*          This is used to process data written into CONTROL area.
*
* Arguments:
*       UCHAR *data
*       int   length
* Return Values:
*       none
*******************************************************************************
*/

void UcsiSetControl(BYTE *TempData, BYTE length) {
    BOOL	command_complete = TRUE;
    WORD	connector;
    WORD	value;
    BYTE	i, ope_mode, charging_status;

    if (0 < length) {
        /*
        *   move advertised PDOs to Message-In area
        */
        for (i = 0; (length > i) && (sizeof(Ucsi_control) > i); i++) {
            Ucsi_control[i] = TempData[i];
        }
        Ucsi_cci[0] = ((Ucsi_Connector_Number_Changed << UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_SHIFT) &
                          UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK);
        Ucsi_cci[1] = 0;
        Ucsi_cci[2] = 0;
        Ucsi_cci[3] = 0;
        switch (Ucsi_control[0]) {
            case UCSI_COMMAND_PPM_RESET:
                 /*
                 *     PPM RESET command
                 */
                 RamDebug(0xD0);
              #if DEBUG_UCSI_CMD
                RamDebug(0xD0);
                #endif
                 Ucsi_Flag.busy       = FALSE;
                 //Ucsi_Request.request = UCSI_REQUEST_NONE;
                 //Ucsi_cci[0]        = 0;
                 //Ucsi_cci[1]        = 0;
                 //Ucsi_cci[2]        = 0;
                 //Ucsi_cci[3]        = UCSI_CCI_RESET_COMPLETE_INDICATOR;
                 command_complete     = FALSE;
                 //Ucsi_Connector_Number_Changed = 0;
                 //UCSI_POWER_DIRECTION_CHANGE = 0;
                 //UCSI_CONNECTOR_PARTNER_CHANGE = 0;
                 //UCSI_BATTERY_CHARGE_STATUS_CHANGE = 0;
                 //UCSI_SUPPORTED_CAM_CHANGE = 0;
                 //Ucsi_Error_Set = 0;
                 //UCSI_RTS5455_CMD_WAIT = 0;
                // UCSI_RTS5455_CMD_ERROR = 0;
                 //GetConnectorStatus_RTS5455(0);
                 Ucsi_Request.request = UCSI_REQUEST_PPM_RESET;
                Ucsi_Request.connector = NUMBER_OF_USB_PDC;
                Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                Ucsi_Flag.cancel       = FALSE;

           /*      SMB3_DATA[0] = 0x01;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
    			 if(bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, 0x0E, &SMB3_DATA[0], 4, SMBus_NoPEC))
            {
            Delay1MS(10);
                    bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]);
                    #if 1
                    RamDebug(0xFF);
                    RamDebug(SMB3_DATA[0]);
                    #endif
                     }
                 #if DEBUG_UCSI_CMD
                 RamDebug(Ucsi_cci[0]);
                 RamDebug(Ucsi_Connector_Number_Changed);
                 #endif*/
                 break;
            case UCSI_COMMAND_CANCEL:
                 /*
                 *     CANCEL command
                 */
              #if DEBUG_UCSI_CMD
                RamDebug(0xD1);
                #endif
                 Ucsi_Flag.busy       = FALSE;
                 Ucsi_Request.request = UCSI_REQUEST_NONE;
                 Ucsi_cci[3]        |= UCSI_CCI_CANCEL_COMPLETE_INDICATOR;
                 break;
            case UCSI_COMMAND_CONNECTOR_RESET:
                 /*
                 *     Connector reset command
                 */
             #if DEBUG_UCSI_CMD
                RamDebug(0xD2);
                #endif
                 Ucsi_cci[0] &= ~UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK;
                 connector = UCSI_CONNECTOR_RESET_COMMAND_CONNECTOR_NUMBER;
                 //RamDebug(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector) {
                     if (((TRUE == BattExistGet) && (0 < RelativeStateOfChgL))
                         || (Read_AC_IN() && Read_DCIN_ATTACHED2())) {
                         /*
                         *     not in DeadBattery State
                         */
                         if (1 == UCSI_CONNECTOR_RESET_COMMAND_HARD_RESET) {
                         // unsupported hard reset
                             Ucsi_Request.request = UCSI_REQUEST_HARD_RESET;
                         } else {
                             Ucsi_Request.request = UCSI_REQUEST_SOFT_RESET;
                         }
                         Ucsi_Request.connector = connector;
                         Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                         Ucsi_Flag.busy         = FALSE;
                         Ucsi_Flag.cancel       = FALSE;
                         command_complete       = FALSE;
                         //TskActivateTask(TASK_UCSI);
                     } else {
                         /*
                         *     in DeadBattery State
                         */
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_COMMAND_UNSUCCESSFUL_DUE_TO_DEAD_BATTERY;
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_ACK_CC_CI:
                 /*
                 *     ACK CC CI command
                 */
                 RamDebug(0xD3);
            #if DEBUG_UCSI_CMD
                RamDebug(0xD3);
                #endif
                 if (1 == UCSI_ACK_CC_CI_COMMAND_CONNECTOR_CHANGE_ACKNOWLEDGE) {
                     for (i = 0; NUMBER_OF_USB_PDC > i; i++ ) {
                         Ucsi_Connector_Change_Status[i] = 0;
                     }
                    Ucsi_Connector_Number_Changed = 0;
                    RamDebug(0xB2);
                     SMB3_DATA[0] = 0x00;
    			SMB3_DATA[1] = RTS5455_PORT_NUM;
                SMB3_DATA[2] = Ucsi_Connector_Change_Status[0] & 0xFF;
                SMB3_DATA[3] = Ucsi_Connector_Change_Status[0] >> 8;
                SMB3_DATA[4] = 0x00;
                SMB3_DATA[5] = 0x00;
                SMB3_DATA[6] = 0x01;
    			 if(bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, 0x0A, &SMB3_DATA[0],7, SMBus_NoPEC))
            {
            Delay1MS(20);
                    bSMBusReceiveByte(chSMB_TYPEC, TypeC_Addr, &SMB3_DATA[0]);
                    #if 1
                    RamDebug(0xFF);
                    RamDebug(SMB3_DATA[0]);
                    #endif
                     }
                 }
                 if (1 == UCSI_ACK_CC_CI_COMMAND_COMMAND_COMPLETED_ACKNOWLEDGE) {
                     //Ucsi_cci[3] &= ~UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
                 //}
                 if(Ucsi_Error_Set)
                    Ucsi_Error_Set = 0;
                 RamDebug(0xB3);
                   // }

                  Ucsi_cci[0]    = 0;
                 Ucsi_cci[1]    = 0;
                 Ucsi_cci[2]    = 0;
                 Ucsi_cci[3]    = UCSI_CCI_ACKNOWLEDGE_INDICATOR;
            }
                 command_complete = TRUE;
                 break;
            case UCSI_COMMAND_SET_NOTIFICATION_ENABLE:
                 /*
                 *     SET NOTIFICATION ENABLE command
                 */
             #if DEBUG_UCSI_CMD
                RamDebug(0xD4);
                #endif

                Ucsi_Notification_Enable = (Ucsi_control[2] + (Ucsi_control[3] << 8));
                 //Ucsi_Notification_Enable = UCSI_SET_NOTIFICATION_ENABLE_COMMAND_NOTIFICATION_ENABLE;
                 #if !_SUPPORT_ALT_MODE_DETAILS_
            	Ucsi_Notification_Enable &= ~UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM;
            	#endif // _SUPPORT_ALT_MODE_DETAILS_
            	#if !_SUPPORT_PDO_DETAIL_
            	Ucsi_Notification_Enable &= ~(UCSI_CONNECTOR_STATUS_CHANGE_NEGOTIATED_POWER_LEVEL | UCSI_CONNECTOR_STATUS_CHANGE_SUPPORTED_PROVIDER_CAPABILITIES);
            	#endif // _SUPPORT_PDO_DETAIL_

            	#if !_SUPPORT_EXT_SUPPLY_NOTIFICATION_
            	Ucsi_Notification_Enable &= ~UCSI_CONNECTOR_STATUS_CHANGE_EXTERNAL_SUPPLY;
            	#endif // _SUPPORT_EXT_SUPPLY_NOTIFICATION_

            	#if !_SUPPORT_PD_RESET_NOTIFICATION_
            	Ucsi_Notification_Enable &= ~UCSI_CONNECTOR_STATUS_CHANGE_PD_RESET_COMPLETE;
            	#endif // _SUPPORT_PD_RESET_NOTIFICATION_

                UCSI_NOTIFICATION_ENABLE[0] = (Ucsi_Notification_Enable&0xFF);
                UCSI_NOTIFICATION_ENABLE[1] = ((Ucsi_Notification_Enable&0xFF00)>>8);
#if DEBUG_UCSI_CMD
RamDebug(UCSI_NOTIFICATION_ENABLE[0]);
RamDebug(UCSI_NOTIFICATION_ENABLE[1]);
#endif

                Ucsi_Request.request = UCSI_REQUEST_SET_NOTIFICATION_ENABLE;
                Ucsi_Request.connector = NUMBER_OF_USB_PDC;
                Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                Ucsi_Flag.busy         = FALSE;
                Ucsi_Flag.cancel       = FALSE;
                command_complete       = FALSE;
                //Detect_Adaptor_Watt_SKIP = 1;
                 break;
            case UCSI_COMMAND_GET_CAPABILITY:
                 /*
                 *     GET CAPABILITY command
                 */
             #if DEBUG_UCSI_CMD
                RamDebug(0xD5);
                #endif

                 Ucsi_Request.request = UCSI_REQUEST_GET_CAPABILITY;
                Ucsi_Request.connector = NUMBER_OF_USB_PDC;
                Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                Ucsi_Flag.busy         = FALSE;
                Ucsi_Flag.cancel       = FALSE;
                command_complete       = FALSE;

                 break;
            case UCSI_COMMAND_GET_CONNECTOR_CAPABILITY:
                 /*
                 *     GET COPNNECTOR CAPABILITY command
                 */
             #if DEBUG_UCSI_CMD
                RamDebug(0xD6);
                #endif
                 connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;

                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector){

                Ucsi_Request.request = UCSI_REQUEST_GET_CONNECTOR_CAPABILITY;
                Ucsi_Request.connector = connector;
                Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                Ucsi_Flag.busy         = FALSE;
                Ucsi_Flag.cancel       = FALSE;
                command_complete       = FALSE;
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_SET_UOM:
                #if DEBUG_UCSI_CMD
                RamDebug(0xDF);
                #endif
                #if _SUPPORT_SET_UOM_
                connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;
                //RamDebug(connector);
                if (0 <= connector){
                    if (FALSE == Usb_Pdc_Status[connector-1].device_connected) {
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR;
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {
                        value = UCSI_SET_UOM_COMMAND_USB_OPERATION_MODE;
                         switch (value) {
                             case 1:

                                    // Connector operate as a DFP

                                  Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_DFP;
                                  Ucsi_Request.connector = connector;
                                  Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                  Ucsi_Flag.busy         = FALSE;
                                  Ucsi_Flag.cancel       = FALSE;
                                  command_complete       = FALSE;
                                  //TskActivateTask(TASK_UCSI);
                                  break;
                             case 2:
                               //    Connector operate as a UFP

                                 Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_UFP;
                                 Ucsi_Request.connector = connector;
                                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                 Ucsi_Flag.busy         = FALSE;
                                 Ucsi_Flag.cancel       = FALSE;
                                 command_complete       = FALSE;
                                 break;
                             case 4:
                                 //    Accept operate as a DRP
                                 break;
                             default:
                                 Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                                 Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     }
                }
                     }else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                #else
                Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                #endif
                break;
            case UCSI_COMMAND_SET_PDM:
                #if DEBUG_UCSI_CMD
                RamDebug(0xFD);
                #endif
				RamDebug(0xFD);
                #if _SUPPORT_SET_PDM_
                connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;
                //RamDebug(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector){
                     if (FALSE == Usb_Pdc_Status[connector-1].device_connected) {
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR;
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {
                         value = UCSI_SET_PDM_COMMAND_POWER_DIRECTION_MODE;
                         switch (value) {
                         case 1:
                             /*
                             *   Connector operate as a Source
                             */
                             Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_SOURCE;
                             Ucsi_Request.connector = connector;
                             Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                             Ucsi_Flag.busy         = FALSE;
                             Ucsi_Flag.cancel       = FALSE;
                             command_complete       = FALSE;
                             //TskActivateTask(TASK_UCSI);
                             break;
                        case 2:
                            /*
                            *    Connector operate as a Sink
                            */
                            Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_SINK;
                            Ucsi_Request.connector = connector;
                            Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                            Ucsi_Flag.busy         = FALSE;
                            Ucsi_Flag.cancel       = FALSE;
                            command_complete       = FALSE;
                            break;
                        case 4:
                            /*
                            *    Connector operate as a provider/consumer if it is a DFP
                                or as a consumer/provider if it is a UFP.
                            */
                            break;
                        default:
                            Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                            Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;

                         }
                         Ucsi_Error_Status =  UCSI_ERROR_STATUS_INCOMPATIBLE_CONNECTOR_PARTNER;
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                #else
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                #endif
                 break;
            case UCSI_COMMAND_GET_ALTERNATE_MODES:
                 /*
                 *     unsupported commands, "SET UOM", "SET PDM" and "GET ALTERNATE MODES"
                 */
                 #if DEBUG_UCSI_CMD
             RamDebug(0xD7);
                 #endif
                 #if _SUPPORT_ALT_MODE_DETAILS_

                    connector = UCSI_GET_ALTERNATE_MODES_COMMAND_CONNECTOR_NUMBER;
                 //RamDebug(connector);
                 if (0 <= connector){
                 Ucsi_Request.request = UCSI_REQUEST_GET_ALTERNATE_MODES;
                 Ucsi_Request.connector = connector;
                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                 Ucsi_Flag.busy         = FALSE;
                 Ucsi_Flag.cancel       = FALSE;
                 command_complete       = FALSE;
                  #if DEBUG_UCSI_CMD
                 RamDebug(Ucsi_Request.request);
                  #endif
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 #else
                 //Ucsi_cci[1] = 0x10;
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_SET_UOR:
                 /*
                 *     "SET UOR" (USB Operation Role)
                 */
             #if DEBUG_UCSI_CMD
             RamDebug(0xD8);
             #endif
                 connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;
                 //RamDebug(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector){
                     if (FALSE == Usb_Pdc_Status[connector-1].device_connected) {
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR;
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {
                         value = UCSI_SET_UOR_COMMAND_USB_OPERATION_ROLE;
                         #if DEBUG_UCSI_CMD
                         RamDebug(value);
                         #endif
                         switch (value) {
                             case 1:
                                  /*
                                  *    Swap to DFP
                                  */
                                  Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_DFP;
                                  Ucsi_Request.connector = connector;
                                  Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                  Ucsi_Flag.busy         = FALSE;
                                  Ucsi_Flag.cancel       = FALSE;
                                  command_complete       = FALSE;
                                  //TskActivateTask(TASK_UCSI);
                                  break;
                             case 2:
                                 /*
                                 *    Swap to UFP
                                 *///RamDebug(0xB5);
                                 Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_UFP;
                                 Ucsi_Request.connector = connector;
                                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                 Ucsi_Flag.busy         = FALSE;
                                 Ucsi_Flag.cancel       = FALSE;
                                 command_complete       = FALSE;
                                 break;
                             case 4:
                                 /*
                                 *    Accept role swap
                                 */
                                 Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_DRP;
                                 Ucsi_Request.connector = connector;
                                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                 Ucsi_Flag.busy         = FALSE;
                                 Ucsi_Flag.cancel       = FALSE;
                                 command_complete       = FALSE;
                                 break;
                             default:
                                 Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                                 Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                         }
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_SET_PDR:
                 /*
                 *     "SET PDR" (Power Direction Role)
                 */
            #if 1
             RamDebug(0xD9);
            #endif
                 connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;
                 RamDebug(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector){
                   /*  if (FALSE == Usb_Pdc_Status[connector-1].device_connected) {
					 	 RamDebug(Usb_Pdc_Status[connector-1].device_connected);
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR;
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {*/
                      //RamDebug(0xC0);
                      Detect_Adaptor_Watt_SKIP = 1;
                         value = UCSI_SET_PDR_COMMAND_POWER_DIRECTION_ROLE;
                         #if 1
                         RamDebug(value);
                         #endif
                         switch (value) {
                         case 1:
                             /*
                             *    Swap to Source
                             */
                             Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_SOURCE;
                             Ucsi_Request.connector = connector;
                             Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                             Ucsi_Flag.busy         = FALSE;
                             Ucsi_Flag.cancel       = FALSE;
                             command_complete       = FALSE;
                             //TskActivateTask(TASK_UCSI);
                             CLEAR_MASK(TypeCStatus3,GotPartnerInformation);
                              
                             break;
                        case 2:
                            /*
                            *    Swap to Sink
                            */
                            Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_SINK;
                            Ucsi_Request.connector = connector;
                            Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                            Ucsi_Flag.busy         = FALSE;
                            Ucsi_Flag.cancel       = FALSE;
                            command_complete       = FALSE;
							CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
                            break;
                        case 4:
                            /*
                            *    Accept power role swap
                            */
                            Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_ACCEPT_SOURCE_SINK;
                            Ucsi_Request.connector = connector;
                            Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                            Ucsi_Flag.busy         = FALSE;
                            Ucsi_Flag.cancel       = FALSE;
                            command_complete       = FALSE;

                            break;
                        default:
                            Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                            Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;

                         }
                         //Ucsi_Error_Status =  UCSI_ERROR_STATUS_INCOMPATIBLE_CONNECTOR_PARTNER;
                    // }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_GET_CAM_SUPPORTED:
                 /*
                 *     GET CAM SUPPORT command
                 */
                 #if DEBUG_UCSI_CMD
             RamDebug(0xDA);
                 #endif
                 #if _SUPPORT_ALT_MODE_DETAILS_
                    connector = UCSI_GET_CAM_SUPPORT_COMMAND_CONNECTOR_NUMBER;
                 //RamDebug(connector);
                 if (0 <= connector){
                 Ucsi_Request.request = UCSI_REQUEST_GET_CAM_SUPPORTED;
                 Ucsi_Request.connector = connector;
                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                 Ucsi_Flag.busy         = FALSE;
                 Ucsi_Flag.cancel       = FALSE;
                 command_complete       = FALSE;
                 #if DEBUG_UCSI_CMD
                 RamDebug(Ucsi_Request.request);
                 #endif
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                    }
                 #else
                 Ucsi_cci[3] |= UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_GET_CURRENT_CAM:
                 #if _SUPPORT_ALT_MODE_DETAILS_
                    connector = UCSI_GET_CURRENT_CAM_COMMAND_CONNECTOR_NUMBER;
                    if (0 <= connector){
                        Ucsi_Request.request = UCSI_REQUEST_GET_CURRENT_CAM;
                            Ucsi_Request.connector = connector;
                            Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                            Ucsi_Flag.busy         = FALSE;
                            Ucsi_Flag.cancel       = FALSE;
                            command_complete       = FALSE;
                    } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                    }
                 #else
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_SET_NEW_CAM:
                 #if _SUPPORT_ALT_MODE_OVERRIDE_
                 #else
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_GET_CABLE_PROPERTY:
                 /*
                 *     unsupported commands, "GET CURRENTCAM", "SET NEW CAM" and "GET GET CABLE PROPERTY"
                 */
                 #if DEBUG_UCSI_CMD
             RamDebug(0xDB);
                 #endif
                 #if _SUPPORT_CABLE_DETAILS_
                 connector = UCSI_GET_CABLE_PROPERTY_COMMAND_CONNECTOR_NUMBER;
                 //RamDebug(connector);
                 if (0 <= connector){
                 Ucsi_Request.request = UCSI_REQUEST_GET_CABLE_PROPERTY;
                 Ucsi_Request.connector = connector;
                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                 Ucsi_Flag.busy         = FALSE;
                 Ucsi_Flag.cancel       = FALSE;
                 command_complete       = FALSE;
                 #if DEBUG_UCSI_CMD
                 RamDebug(Ucsi_Request.request);
                 #endif
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 #else
                Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_GET_PDOS:
                 /*
                 *     GET PDOs command
                 */
                 #if DEBUG_UCSI_CMD
             RamDebug(0xDC);
                 #endif
                 Ucsi_Flag.busy    = FALSE;
                 Ucsi_Flag.cancel  = FALSE;
                 connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;
                 //RamDebug(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector) {
                     if (0 != (UCSI_OPTIONAL_FEATURE & UCSI_BM_OPTIONAL_FEATURES_PDO_DETAILS_SUPPORTED)) {
                        Ucsi_Request.request = UCSI_REQUEST_GET_PDOS;

	                     if (UCSI_REQUEST_NONE != Ucsi_Request.request) {
	                         Ucsi_Request.connector     = connector;
	                         Ucsi_Request.offset        = UCSI_GET_PDOS_COMMAND_PDO_OFFSET;
	                         Ucsi_Request.number_of_pdo = UCSI_GET_PDOS_COMMAND_NUMBER_OF_PDOS;
                             Ucsi_cci[1]                = 0x04;
	                         //Ucsi_cci[3]              |= UCSI_CCI_BUSY_INDICATOR;
	                         command_complete           = FALSE;
	                         //TskActivateTask(TASK_UCSI);
	                     }
                     } else {
                          Ucsi_cci[3] |= UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            /*
            *************************************************************
            *     GET Connector Status
            *************************************************************
            */
            case UCSI_COMMAND_GET_CONNECTOR_STATUS:
                #if DEBUG_UCSI_CMD
             RamDebug(0xDD);
                #endif
                 connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;


                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if ((0 < connector) && (connector <= NUMBER_OF_USB_PDC)) {
#if DEBUG_UCSI_CMD
//RamDebug(Ucsi_Notification_Enable);
//RamDebug((Ucsi_Notification_Enable >> 8));
RamDebug(Ucsi_Connector_Change_Status[connector-1]);
RamDebug((Ucsi_Connector_Change_Status[connector-1] >> 8));
#endif
                         if((Ucsi_Connector_Change_Status[connector-1] & UCSI_CONNECTOR_STATUS_CHANGE_ERROR) == UCSI_CONNECTOR_STATUS_CHANGE_ERROR)
                            Ucsi_cci[3]     = UCSI_CCI_ERROR_INDICATOR; ;
                         Ucsi_message_in[0] = Ucsi_Connector_Change_Status[connector-1] & 0xFF;
		                 Ucsi_message_in[1] = Ucsi_Connector_Change_Status[connector-1] >> 8;
#if DEBUG_UCSI
RamDebug(Ucsi_message_in[0]);
RamDebug(Ucsi_message_in[1]);
#endif

                     /*
                     *    Operation Mode
                     */
                    // if (TRUE == Usb_Pdc_Power_Status[connector-1].power_connection) {
                    //     if (USB_BC12_NONE != Usb_Pdc_Power_Status[connector-1].bc12_status) {
                             /*
                             *    BC 1.2 (SDP/CDP/DCP) operation
                             */
                      //       ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_BC;
                      //   } else {
                             /*
                             *    Type-C current
                             */
                             switch (Usb_Pdc_Power_Status[connector-1].type_c_current) {
                                 case USB_TYPE_C_CURRENT_DEFAULT:
                                      ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_USB_DEFAULT;
                                      break;
                                 case USB_TYPE_C_CURRENT_1_5_A:
                                      ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_1_5_A;
                                      break;
                                 case USB_TYPE_C_CURRENT_3_0_A:
                                      ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_3_0_A;
                                      break;
                                 case USB_TYPE_C_CURRENT_PD_CONTRACT:
                                      ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_PD;
                                      break;
                                 case USB_TYPE_C_CURRENT_BC:
                                      ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_BC;
                                      break;
                                 default:
                                     ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_NO_CONSUMER;
                             }
              //           }

                //     }
                  //   else {
                    //     ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_NO_CONSUMER;
                     //}
#if DEBUG_UCSI
RamDebug(ope_mode);
RamDebug((SMB3_DATA[2]&0x07));
RamDebug(Usb_Pdc_Power_Status[connector-1].power_source);
RamDebug(UCSI_POWER_SOURCE);
#endif
                    ope_mode = (SMB3_DATA[2]&0x07);

                     /*   Power direction
                         */
                          if (TRUE == Usb_Pdc_Power_Status[connector-1].power_source)
                          //if(UCSI_POWER_SOURCE)
                         {
                             /*
                             *   Operate as a Source
                             */
                         		 SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_POWER_DIRECTION);
                         } else {
                             /*
                             *   Operate as a Sink
                             */
                         		 CLEAR_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_POWER_DIRECTION);
                         }
                     Ucsi_message_in[2] &= 0xF8;
                     Ucsi_message_in[2] |= ope_mode;

                     if (TRUE == Usb_Pdc_Status[connector-1].device_connected) {
                         SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS);
                         } else {
                         CLEAR_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS);
                     }

                         /*
                         *   Connected partner flags ?
                         */
                         Ucsi_message_in[2] &= 0x1F;
                         Ucsi_message_in[3] &= 0x1F;
                         if(Usb_Pdc_Status[connector-1].connect_status == USB_CONNECT_CONNECTED)
                            SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_USB);
                         else if(Usb_Pdc_Status[connector-1].connect_status == USB_CONNECT_DP)
                            SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_ALT);
                         /*
                         *   Connected partner type
                         */
#if DEBUG_UCSI
RamDebug(0xBF);
RamDebug(Ucsi_message_in[2]);
//RamDebug(Usb_Pdc_Status[connector-1].connect_status);
#endif
                              i = 0;
                             if (Usb_Pdc_Status[connector-1].dfp == FALSE)
                                //if(!UCSI_DATA_ROLE)
                                {
                                  i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DFP;
                                  } else {
                                  i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_UFP;
                                  }
                         Ucsi_message_in[3] = ((UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE | i) << 5);
#if DEBUG_UCSI
RamDebug(0xBB);
RamDebug(Ucsi_message_in[3]);
#endif
                         /*
                         *     RDO ?
                         */
                         Ucsi_message_in[4] = UCSI_RDO1;
                         Ucsi_message_in[5] = UCSI_RDO2;
                         Ucsi_message_in[6] = UCSI_RDO3;
                         Ucsi_message_in[7] = UCSI_RDO4;

                         /*
                         *    Charging status
                         */

                         /*if (TRUE == Usb_Pdc_Charge_Status[connector-1].quick_charge) {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_NOMINAL_CHARGING;
                         } else if (TRUE == Usb_Pdc_Charge_Status[connector-1].trickle_charge) {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_TRICKLE_CHARGING;
							 }else if(TRUE == Usb_Pdc_Charge_Status[connector-1].slow_charge) {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_SLOW_CHARGING;
                         } else {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_NO_CHARGING;
                         }*/
                         charging_status=0;

						  if(TRUE == Usb_Pdc_Charge_Status[connector-1].slow_charge) {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_SLOW_CHARGING;
						  	}
						  else
						  	{ charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_NO_CHARGING;
						  	}
						  if( IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
						  	{
						  	charging_status=0;
						  	}
                         Ucsi_message_in[8] = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS | charging_status;

					  Ucsi_message_in[8] &= 0xC1;
                     Ucsi_message_in[8] &= 0x3D;
					 /*
                     *   fill 0 to unused bits
                     */
                     /*if(CurrentTypeCWatt>=36)
                     	{
                     Ucsi_message_in[8] &= 0xC1;
                     Ucsi_message_in[8] &= 0x3D;
                     	}
					 else
					 	{ Ucsi_message_in[8] = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS|UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_SLOW_CHARGING;
					 	Ucsi_message_in[8] &= 0xC1;
                     Ucsi_message_in[8] &= 0x3f;
					 	}*/

#if DEBUG_UCSI_CMD
RamDebug(Ucsi_message_in[0]);
RamDebug(Ucsi_message_in[1]);
#endif
        GetConnectorStatus_RTS5455(0);


                                          //RamDebug(UCSI_GET_CONNECTOR_STATUS_RTS5455[0]);
                                     //RamDebug(UCSI_GET_CONNECTOR_STATUS_RTS5455[1]);

                                     for(i = 2;i < 9;i++)
                                            {
                                            Ucsi_message_in[i] = UCSI_GET_CONNECTOR_STATUS_RTS5455[i];
                                            }

                                     Ucsi_message_in[8] = ((Ucsi_message_in[8] & 0xFC) | charging_status);
									RamDebug(0x79);
									RamDebug(Ucsi_message_in[2]);

                                     #if DEBUG_UCSI_CMD
                                     RamDebug(Ucsi_message_in[8]);
                                     #endif
                     Ucsi_cci[1]     = 9;
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     //Ucsi_Connector_Change_Status[0] = UCSI_CONNECTOR_STATUS_CHANGE_ERROR;
                     //Ucsi_cci[1]     = 9;
                     //Ucsi_Connector_Number_Changed = 1;
                 }
                 break;
            /*
            ********************************************
            *   Get Error Status
            ********************************************
            */
            case UCSI_COMMAND_GET_ERROR_STATUS:
                #if DEBUG_UCSI_CMD
             RamDebug(0xDE);
                #endif
// Isaac check                 Ucsi_cci[1] =   (Ucsi_cci[1] & ~UCSI_CCI_DATA_LENGTH_MASK) | UCSI_PARM_GET_ERROR_STATUS_DATA_LENGTH;
                 Ucsi_cci[1] =  UCSI_PARM_GET_ERROR_STATUS_DATA_LENGTH;
                 Ucsi_message_in[0] = Ucsi_Error_Status & 0xff;
                 Ucsi_message_in[1] = (Ucsi_Error_Status >> 8) & 0xff;
                 Ucsi_Error_Set = 1;
                 for (i = 2; UCSI_PARM_GET_ERROR_STATUS_DATA_LENGTH > i; i++) {
                     Ucsi_message_in[i] = 0;
                 }
                 break;
            default:
                 Ucsi_cci[0]     = 0;
                 Ucsi_cci[1]     = 0;
                 Ucsi_cci[2]     = 0;
                 Ucsi_cci[3] = (UCSI_CCI_NOT_SUPPORTED_INDICATOR | UCSI_CCI_COMMAND_COMPLETE_INDICATOR);
        }
    } else {
        Ucsi_cci[0] = 0;
        Ucsi_cci[1] = 0;
        Ucsi_cci[2] = 0;
        Ucsi_cci[3] = (UCSI_CCI_NOT_SUPPORTED_INDICATOR
| UCSI_CCI_COMMAND_COMPLETE_INDICATOR);
    }
    if (TRUE == command_complete) {
    		if((Ucsi_cci[3] & UCSI_CCI_ACKNOWLEDGE_INDICATOR) == 0x00 )
    		{
            /*
            *   not ACK_CC_CI
            */
        	Ucsi_cci[3] |= UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
        }
        /*
        *    Issue the command complete notification
        */
        if (0 != (Ucsi_Notification_Enable & UCSI_COMMAND_COMPLETE_NOTIFICATION_ENABLE)) {
            ECSMI_SCIEvent(AM_UCSI_EVENT);
			 RamDebug(0x99);
        }
    }
}

/*
*******************************************************************************
* Function name: UcsiGetControl
*
* Descriptoin:
*          This is used to get data in CONTROL area.
*
* Arguments:
*       UCHAR *data
*       int   max_length
* Return Values:
*       int   number of data written into data area
*******************************************************************************
*/

BYTE UcsiGetControl(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;
    for (; (max_length > rc) && (sizeof(Ucsi_control) > rc); rc++) {
        TempData[rc] = Ucsi_control[rc];
    }
    return rc;
}

/*
*******************************************************************************
* Function name: UcsiGetMessageIn
*
* Descriptoin:
*          This is used to get data in MESSAGE-IN area.
*
* Arguments:
*       UCHAR *data
*       int   max_length
* Return Values:
*       int   number of data written into data area
*******************************************************************************
*/

BYTE UcsiGetMessageIn(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;
    for (; (max_length > rc) && (sizeof(Ucsi_message_in) > rc); rc++) {
        TempData[rc] = Ucsi_message_in[rc];
    }
     if(Ucsi_control[0] == UCSI_COMMAND_GET_CONNECTOR_STATUS)
        {
    RamDebug(0xFF);
    RamDebug(Ucsi_message_in[2]);
        }
    return rc;
}

/*
*******************************************************************************
* Function name: UcsiSetMessageOut
*
* Descriptoin:
*          This is used to set data into MESSAGE-OUT area.
*
* Arguments:
*       UCHAR *data
*       int   length
* Return Values:
*       none
*******************************************************************************
*/

void UcsiSetMessageOut(BYTE *TempData, BYTE length) {
BYTE	i;
    for (i = 0; (length > i) && (sizeof(Ucsi_message_out) > i); i++) {
        Ucsi_message_out[i] = TempData[i];
    }
}

/*
*******************************************************************************
* Function name: UcsiGetMessageOut
*
* Descriptoin:
*          This is used to get data in MESSAGE-OUT area.
*
* Arguments:
*       UCHAR *data
*       int   max_length
* Return Values:
*       int   number of data written into data area
*******************************************************************************
*/

BYTE UcsiGetMessageOut(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;
    for (; (max_length > rc) && (sizeof(Ucsi_message_out) > rc); rc++) {
        TempData[rc] = Ucsi_message_out[rc];
    }
    return rc;
}


/*
*******************************************************************************
* Function name: UcsiUsbPdcStatusChanged
*
* Descriptoin:
*          This is invoked when status of USB-C PDC is changed
*
* Arguments:
*       int   state
* Return Values:
*       none
*******************************************************************************
*/

void UcsiUsbPdcStatusChanged(USB_POWER_STATUS power_status, USB_STATUS status) {
    BOOL             attention = FALSE;
    BOOL             changed;
		BYTE	i;

    for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
        changed = FALSE;
        //power_status = UsbPdcGetPowerStatus(i);
        if ((power_status.bc12_status      != Usb_Pdc_Power_Status[i].bc12_status)    ||
            (power_status.type_c_current   != Usb_Pdc_Power_Status[i].type_c_current) ||
            (power_status.power_connection != Usb_Pdc_Power_Status[i].power_connection)) {
            /*
            *   Operation mode is changed
            */
            Ucsi_Connector_Change_Status[i] |= UCSI_CONNECTOR_STATUS_CHANGE_POWER_OPERATION_MODE;
            //Usb_Pdc_Power_Status[i]         = power_status;
            changed                         = TRUE;
            if (0 != (Ucsi_Notification_Enable & UCSI_POWER_OPERATION_MODE_CHANGE_NOTIFICATION_ENABLE)) {
                attention = TRUE;
            }
        }
        if (power_status.power_source != Usb_Pdc_Power_Status[i].power_source) {
            /*
            *   power direction (Source/ Sink) is changed
            */
            Ucsi_Connector_Change_Status[i] |= UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION;
            //Usb_Pdc_Power_Status[i].power_source =  power_status.power_source;
            changed                         = TRUE;
            if (0 != (Ucsi_Notification_Enable & UCSI_POWER_DIRECTION_CHANGE_NOTIFICATION_ENABLE)) {
                attention = TRUE;
            }
        }
        //status = UsbPdcGetStatus(i);
        if (status.connect_status != Usb_Pdc_Status[i].connect_status) {
            /*
            *   Connected partner is changed
            */
            Ucsi_Connector_Change_Status[i] |= UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER;
            //Usb_Pdc_Status[i]               = status;
            changed                         = TRUE;
            if (0 != (Ucsi_Notification_Enable & UCSI_CONNECTOR_PARTNER_CHANGE_NOTIFICATION_ENABLE)) {
                attention = TRUE;
            }
        }
        if (TRUE == changed) {
            /*
            *    Connector state is changed
            */
            Ucsi_Connector_Number_Changed = 0;//SysConvertUsbPdcIdToUsbConnectorId(i);
        }
    }
    if (TRUE == attention) {
        ECSMI_SCIEvent(AM_UCSI_EVENT);
		 RamDebug(0x9A);
    }
}

/*
*******************************************************************************
* Function name: UcsiFuelStateChanged
*
* Descriptoin:
*          This is invoked when status of battery charging is changed
*
* Arguments:
*       int   state
* Return Values:
*       none
*******************************************************************************
*/

void UcsiFuelStateChanged(BYTE state) {
    BOOL                  attention = FALSE;
    USB_PDC_CHARGE_STATUS status;
		BYTE	i;
		WORD Temp;
if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
	return;
    //if (0 != (UCSI_ATTRIBUTE & UCSI_BM_ATTRIBUTES_BATTERY_CHARGING))
    {
	    for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
	        /*
	        *    get charging status
	        */
	        status.quick_charge   = FALSE;
	        status.trickle_charge = FALSE;
			status.slow_charge = FALSE;

	        if (IS_MASK_SET(TypeCStatus,TypeCAdpReady))
				{
                //RamDebug(0xA0);
	        		Temp = ((WORD)(CurrentH & 0x7F) << 8) + CurrentL;
	            if (IS_MASK_CLEAR(CurrentH, BIT7) && (Temp > 100)) {
	                if (Temp < 0x01F4) {
	                	//Battery Charge Current < 0x01F4  (500mA) :Trickle charge
	                    status.quick_charge   = FALSE;
	                    status.trickle_charge = TRUE;
	                } else {
	                    status.quick_charge   = TRUE;
	                    status.trickle_charge = FALSE;
	                }
	            }
				 if(CurrentTypeCWatt<36)
				 	{
				 	status.slow_charge  = TRUE;
					//RamDebug(0x78);
					//RamDebug(Usb_Pdc_Charge_Status[i].slow_charge);
				 	}
				 else
				 	{
				 	status.slow_charge  = FALSE;
					//RamDebug(0x76);
					//RamDebug(Usb_Pdc_Charge_Status[i].slow_charge);
				 	}

	        }


	        if ((status.quick_charge   != Usb_Pdc_Charge_Status[i].quick_charge) ||
	            (status.trickle_charge != Usb_Pdc_Charge_Status[i].trickle_charge)||
	            (status.slow_charge != Usb_Pdc_Charge_Status[i].slow_charge)){
	            /*
	            *   Charging status is changed
	            */
	            Usb_Pdc_Charge_Status[i]      = status;
                //UCSI_BATTERY_CHARGE_STATUS_CHANGE = 1;
                #if DEBUG_UCSI_CMD
                RamDebug(0xE7);
                #endif
				RamDebug(0x77);
                //Ucsi_Connector_Number_Changed = 0;
	            if (0 != (Ucsi_Notification_Enable & UCSI_BATTERY_CHARGE_STATUS_CHANGE_NOTIFICATION_ENABLE))
	           {
                        #if DEBUG_UCSI_CMD
                RamDebug(0xE8);
                #endif
                UCSI_BATTERY_CHARGE_STATUS_CHANGE = 1;
			            Ucsi_Connector_Number_Changed = 0;//SysConvertUsbPdcIdToUsbConnectorId(i);
			            Ucsi_Connector_Change_Status[i] |= (UCSI_CONNECTOR_STATUS_CHANGE_BATTERY_CHARGE_STATUS | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
	               // attention = TRUE;
	            }
	        }
	    }
	    if (TRUE == attention) {
	        ECSMI_SCIEvent(AM_UCSI_EVENT);
			 RamDebug(0x9B);
	    }
	  }
}
#endif
