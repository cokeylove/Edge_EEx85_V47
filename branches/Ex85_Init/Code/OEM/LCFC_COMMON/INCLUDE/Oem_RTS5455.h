/*
********************************************************************************
* Lenovo Confidential
*
* Microcode Source Materials
*
* ThinkPad Embedded Controller Microcode
*
* COPYRIGHT LENOVO 2016 ALL RIGHTS RESERVED
*
* Filename: Oem_RTS5455.h
*
* Description:
*    definition of the unique address interface registers of TI TPS65982 USB-PD controller.
*
* History:
*  04/28/2017    Isaac Lin
*      created as new
********************************************************************************
*/

#ifndef _OEM_RTS5455_H
   #define _OEM_RTS5455_H
#if (Support_TYPE_C == RTS5455)

#define 	TypeC_Ini_Delay_time  10 	//Delay 100ms for type C stable     10 * 10mS
#define 	TypeCLimitedSupport_BatteryRSOC  10 	//In AC+DC mode, when battery capacity < 5%, behavior is the same with AC only mode
#define 	TypeC_IntSetting_Delay_time  50 	//Wait for 500ms if power is supplied from USB PDC B port(Dead battery)     50 * 10mS

#define NUMBER_OF_USB_PDC 1
#define USB_PDC_RC_OK              0
#define USB_PDC_RC_NOT_SUPPORTED   1
#define USB_PDC_RC_BUSY            2
#define USB_PDC_RC_ERROR           3
#define USB_PDC_RC_REJECT          4

#define PDO_TYPE_MASK                                  //bit30~31
    #define PDO_TYPE_FIXED_SUPPLY    0
    #define PDO_TYPE_BATTERY         1
    #define PDO_TYPE_VARIABLE_SUPPLY 2


/*
*********************************************************
*     #EC ram define
*********************************************************
*/
#define  TypeCStatus          TYPE_C_RAM0             //_at_ OEMRAM4+0x70;	//Bit2~bit0:Step
 #define     TypeCAdpAuthing   	BIT0
 #define     TypeCAdpAuthDone  BIT1
 #define     TypeCAdpAuthPass  	BIT2
 #define     TypeCAdpExist   	BIT3
 #define     OldDcinAttached2  BIT4
 #define     TypeCAdpReady     BIT5
 #define     TypeCProcessOK    BIT6
 #define     TypeCIniOK        BIT7
#define  TypeCIniDelay         TYPE_C_RAM1             //_at_ OEMRAM4+0x71;
#define  TypeCEventNum         TYPE_C_RAM2             //_at_ OEMRAM4+0x72;
#define  TypeCCommFailCnt      TYPE_C_RAM3             //_at_ OEMRAM4+0x73;
#define  TypeCAdpID            TYPE_C_RAM4             //_at_ OEMRAM4+0x74;
#define  CurrentTypeCWatt      TYPE_C_RAM5             //_at_ OEMRAM4+0x75;
#define  TypeCAdpPdStableCheck TYPE_C_RAM6             //_at_ OEMRAM4+0x76;
#define  TypeCProcessStep      TYPE_C_RAM7             //_at_ OEMRAM4+0x77;
#define  PDStatus	             TYPE_C_RAM8             //_at_ OEMRAM4+0x78;
 #define		Get_Fw_Version		BIT6
 #define		TypeCFwUpdating   BIT7
#define  LimitedSupportAdpWattUpper		TYPE_C_RAM9              //_at_ OEMRAM4+0x79;
#define  CurrentTypeCV         TYPE_C_RAM10             //_at_ OEMRAM4+0x7A;  word
#define  CurrentTypeCI         TYPE_C_RAM11             //_at_ OEMRAM4+0x7C;  word
#define  TypeCStatus2					TYPE_C_RAM12             //_at_ OEMRAM4+0x7E;
 #define    ClassicAdpDetachShutdown   	BIT0
 #define    DockingEvent				BIT1
 #define    TypeC_Docking_Attached	   	BIT2
 #define    TypeC_ChipOff	   	BIT3
 #define    TypeC_Laptop_Attached	   	BIT4
 #define    TypeC_Laptop_Power_Type	   	BIT5	//1:Source,0:Sink
 #define    TypeC_Laptop_Power_Type_Target	   	BIT6	//1:Source,0:Sink
 #define    TypeC_Laptop_Power_Charge_Status	   	BIT7	//1:Charge,0:Don't charge
#define  UnknowAdpWatt					TYPE_C_RAM13             //_at_ OEMRAM4+0x7F;
#define  TypeCStatus3					TYPE_C_RAM14             //_at_ OEMRAM4+0x80;
 #define    NeedUpdateToPartner   	BIT0
 #define    AllEventsNotClear	   	BIT1
 #define    GotPartnerInformation   	BIT2
 #define    P2PPowerSwapWait	   	BIT3
 #define    TypeC_Laptop_Power_Charge_Target	   	BIT4	//1:Charge,0:Don't charge
 #define    PdcRequestDone             BIT5
 #define    RTS5455WaitingPing	   	BIT6
#define  OldDockingACKStatus   TYPE_C_DOCKING_RAM0      //_at_ OEMRAM4+0x11;
#define  OldDockingEvent	   TYPE_C_DOCKING_RAM1      //_at_ OEMRAM4+0x12;
#define  OldVDO2			   TYPE_C_DOCKING_RAM2      //_at_ OEMRAM4+0x13;0x13~0x16
#define  PDFwVersion		   TYPE_C_RAM15             //_at_ OEMRAM4+0x17;
#define  AnoutherUnitInformation	    TYPE_C_RAM16    //_at_ OEMRAM4+0x19;0x19~0x1A
#define  RTS5455WaitPingDelay					TYPE_C_RAM17             //_at_ OEMRAM4+0x60
#define  OldInformation					TYPE_C_RAM18             //_at_ OEMRAM4+0x61
																			//bit0~bit6:Battery capacity
 #define    OldSystemStatus	   	BIT7	//1:S0,0:non-S0
#define  RTS5455WaitingPingID					TYPE_C_RAM19             //_at_ OEMRAM4+0x62
#define  DockingPSW_ActiveCnt					TYPE_C_RAM22             //_at_ OEMRAM4+0x80
#define  SinkEnDisDelay					TYPE_C_RAM20             //_at_ OEMRAM4+0x63
#define  PdPowerStableDelay					TYPE_C_RAM21             //_at_ OEMRAM4+0x63
/*
*********************************************************
*     #define Constants
*********************************************************
*/

#ifndef RTS5455_I2C_ADDR
    #define RTS5455_I2C_ADDR   0xBC
#endif
#ifndef RTS5455_DE_ASSERT_I2C_ADDR
    #define RTS5455_DE_ASSERT_I2C_ADDR   0x18
#endif
#ifndef RTS5455_PORT_NUM
    #define RTS5455_PORT_NUM   0x00
#endif
#ifndef RTS5455_INT_DATA_LENGTH
    #define RTS5455_INT_DATA_LENGTH   4
#endif

//#define RTS5455_INT_EVENT_MASK_LL   0x82	//Event mask BIT7+BIT2+BIT1
//#define RTS5455_INT_EVENT_MASK_LH   0x59	//Event mask BIT14+BIT12+BIT11+BIT8
//#define RTS5455_INT_EVENT_MASK_HL   0x00	//Event mask
//#define RTS5455_INT_EVENT_MASK_HH   0x02	//Event mask BIT25

//#define RTS5455_INT_CLEAR_EVENT_MASK_LL   0x7d	//Event mask BIT7+BIT2+BIT1
//#define RTS5455_INT_CLEAR_EVENT_MASK_LH   0xA6	//Event mask BIT14+BIT12+BIT11+BIT8
//#define RTS5455_INT_CLEAR_EVENT_MASK_HL   0xFF	//Event mask
//#define RTS5455_INT_CLEAR_EVENT_MASK_HH   0xFD	//Event mask BIT25

#define RTS5455_INT_EVENT_MASK_LL   0x42	//Event mask BIT7+BIT6+BIT2+BIT1
#define RTS5455_INT_EVENT_MASK_LH   0x59	//Event mask BIT14+BIT12+BIT11+BIT8
#define RTS5455_INT_EVENT_MASK_HL   0x00	//Event mask
#define RTS5455_INT_EVENT_MASK_HH   0x02	//Event mask BIT25

#define RTS5455_INT_CLEAR_EVENT_MASK_LL   0xBD	//Event mask BIT7+BIT6+BIT2+BIT1
#define RTS5455_INT_CLEAR_EVENT_MASK_LH   0xA6	//Event mask BIT14+BIT12+BIT11+BIT8
#define RTS5455_INT_CLEAR_EVENT_MASK_HL   0xFF	//Event mask
#define RTS5455_INT_CLEAR_EVENT_MASK_HH   0xFD	//Event mask BIT25

//   SYMBOL definition
#define	SET_SINGLE_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	SET_MASK(*(&_reg_base_+_reg_offset_),_bit_number_)
#define	CLEAR_SINGLE_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	CLEAR_MASK(*(&_reg_base_+_reg_offset_),_bit_number_)
#define	READ_SINGLE_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	IS_MASK_SET(*(&_reg_base_+_reg_offset_),_bit_number_)
#define	READ_MULTI_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	((*(&_reg_base_+_reg_offset_)) & _bit_number_ )
//#define	SET_SINGLE_BIT_IN_BYTES(x,y,z)	SET_MASK(*(&x+y),z)
//#define	CLEAR_SINGLE_BIT_IN_BYTES(x,y,z)	CLEAR_MASK(*(&x+y),z)
//#define	READ_SINGLE_BIT_IN_BYTES(x,y,z)	IS_MASK_SET(*(&x+y),z)
//define	Fu_En(x)	Fs(x,1,BIT2)	//Set bit10
//define	Fu_Dis(x)	Fc(x,1,BIT2)	//Clear bit10

//     b29: Littel/Big endian, b28: Multiple bit/ Single bit, b24-b27: Type
//     b16-23: number of bit
//     b8-15:  offset or size
//     b0-7:   bit number or start bit number for multiple bit definition

#define RTS5455_VENDOR_COMMAND_ENABLE			              0x01          //
		#define RTS5455_VENDOR_COMMAND_ENABLE_WR_DATA_LEN		0x03	          //
#define RTS5455_VENDOR_COMMAND_ENABLE_ID_LL             0xDA          // Vendor ID for Lenovo:0x010BDA
#define RTS5455_VENDOR_COMMAND_ENABLE_ID_LH             0x0B          //
#define RTS5455_VENDOR_COMMAND_ENABLE_ID_HL             0x01          //

#define RTS5455_GET_PING_STATUS	                        //
                                                              // Bits: 1..0: Command status of RTS5455's ping status
		#define	RTS5455_GET_PING_STATUS_CMD_STS_MASK(x)	READ_MULTI_BIT_IN_BYTES(x,0,0x03)	//Set bit1..0
        #define RTS5455_GET_PING_STATUS_CMD_STS_NOTPROCESSED_OR_INPROCESS         0     // RTS5455 is not processed or in process
        #define RTS5455_GET_PING_STATUS_CMD_STS_COMPLETE     1     // RTS5455 is process complete
        #define RTS5455_GET_PING_STATUS_CMD_STS_DEFERRED   2     // RTS5455 is busy and need to cost some time
        #define RTS5455_GET_PING_STATUS_CMD_STS_ERROR   3     // RTS5455 is process complete and has error.
                                                              // Bits: 7..2: Data length of RTS5455's ping status
		#define	RTS5455_GET_PING_STATUS_DATA_LENGTH_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,0,0xFC)>>2)	//Set bit7..2

#define RTS5455_GET_STATUS			                        0x0981          //
		#define RTS5455_GET_STATUS_WR_DATA_LEN 							0x03	          //
                                                              // Bits: 1:External supply change
		#define	TRTS5455_INT_EXTERNAL_SUPPLY_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT1)	//Set bit1
		#define	TRTS5455_INT_EXTERNAL_SUPPLY_CHANGE_Num	01
                                                              // Bits: 2:Power operation mode change
		#define	TRTS5455_INT_POWER_OPERATION_MODE_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT2)	//Set bit2
		#define	TRTS5455_INT_POWER_OPERATION_MODE_CHANGE_Num	02
                                                              // Bits: 3:Port operation mode change
		#define	TRTS5455_INT_PORT_OPERATION_MODE_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT3)	//Set bit3
		#define	TRTS5455_INT_PORT_OPERATION_MODE_CHANGE_Num	03
                                                              // Bits: 5:Supported provider capabilities change
		#define	TRTS5455_INT_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT5)	//Set bit5
		#define	TRTS5455_INT_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE_Num	05
                                                              // Bits: 6:Negotiated power level change
		#define	TRTS5455_INT_NEGOTIATED_POWER_LEVEL_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT6)	//Set bit6
		#define	TRTS5455_INT_NEGOTIATED_POWER_LEVEL_CHANGE_Num	06
                                                              // Bits: 7:PD reset complete
		#define	TRTS5455_INT_PD_RESET_COMPLETE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT7)	//Set bit7
		#define	TRTS5455_INT_PD_RESET_COMPLETE_Num	07

        #define TRTS5455_INT_SUPPORTED_CAM_CHANGE(x)   READ_SINGLE_BIT_IN_BYTES(x,1,BIT0)  //Set bit8
        #define	TRTS5455_INT_SUPPORTED_CAM_CHANGE_Num	8
                                                              // Bits: 10:Power control request
		#define	TRTS5455_INT_POWER_CONTROL_REQUEST(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT2)	//Set bit10
		#define	TRTS5455_INT_POWER_CONTROL_REQUEST_Num	10
                                                              // Bits: 11:Port partner changed
		#define	TRTS5455_INT_PORT_PARTNER_CHANGED(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT3)	//Set bit11
		#define	TRTS5455_INT_PORT_PARTNER_CHANGED_Num	11
                                                              // Bits: 12:Power direction changed
		#define	TRTS5455_INT_POWER_DIRECTION_CHANGED(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT4)	//Set bit12
		#define	TRTS5455_INT_POWER_DIRECTION_CHANGED_Num	12
                                                              // Bits: 14:Connect change
		#define	TRTS5455_INT_CONNECT_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT6)	//Set bit14
		#define	TRTS5455_INT_CONNECT_CHANGE_Num	14
                                                              // Bits: 20:Alternate flow change
		#define	TRTS5455_INT_ALTERNATE_FLOW_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT4)	//Set bit20
		#define	TRTS5455_INT_ALTERNATE_FLOW_CHANGE_Num	20
                                                              // Bits: 21:DP status change
		#define	TRTS5455_INT_DP_STATUS_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT5)	//Set bit21
		#define	TRTS5455_INT_DP_STATUS_CHANGE_Num	21
                                                              // Bits: 22:Power switch status change
		#define	TRTS5455_INT_POWER_SWITCH_STATUS_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT6)	//Set bit22
		#define	TRTS5455_INT_POWER_SWITCH_STATUS_CHANGE_Num	22
                                                              // Bits: 24:DP lane SWAP request
		#define	TRTS5455_INT_DP_LANE_SWAP_REQUEST(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT0)	//Set bit24
		#define	TRTS5455_INT_DP_LANE_SWAP_REQUEST_Num	24
                                                              // Bits: 25:VDM received
		#define	TRTS5455_INT_VDM_RECEIVED(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT1)	//Set bit25
		#define	TRTS5455_INT_VDM_RECEIVED_Num	25
                                                              // Bits: 29:System Miscellaneous change
		#define	TRTS5455_INT_SYSTEM_MISCELLANEOUS_CAHNGE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT5)	//Set bit29
		#define	TRTS5455_INT_SYSTEM_MISCELLANEOUS_CAHNGE_Num	29
                                                              // Bits: 31:PD AMS change
		#define	TRTS5455_INT_PD_AMS_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT7)	//Set bit31
		#define	TRTS5455_INT_PD_AMS_CHANGE_Num	31

        #define	TRTS5455_POWER_OPERATION_MODE(x)	(READ_MULTI_BIT_IN_BYTES(x,4,0x0E)>>1)	//Set bit33..35
        #define TRTS5455_POWER_OPERATION_MODE_NO_CONSUMER  0
        #define TRTS5455_POWER_OPERATION_MODE_USB_DEFAULT  1
        #define TRTS5455_POWER_OPERATION_MODE_BC           2
        #define TRTS5455_POWER_OPERATION_MODE_PD           3
        #define TRTS5455_POWER_OPERATION_MODE_TYPE_C_1_5_A 4
        #define TRTS5455_POWER_OPERATION_MODE_TYPE_C_3_0_A 5
                                                              // Bits: 38:Power direction
		#define	TRTS5455_POWER_DIRECTION(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT6)	//Set bit38
        #define TRTS5455_POWER_DIRECTION_CONSUMER         0     //
        #define TRTS5455_POWER_DIRECTION_PROVIDER		      1     //
                                                              // Bits: 39:Connect status
		#define	TRTS5455_CONNECT_STATUS(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT7)	//Set bit39
        #define TRTS5455_CONNECT_STATUS_UNATTACHED         0     //
        #define TRTS5455_CONNECT_STATUS_ATTACHED			     1     //

        #define	TRTS5455_PORT_PARTNER_FLAG_USB(x)	READ_SINGLE_BIT_IN_BYTES(x,5,BIT0)	//Set bit40
        #define TRTS5455_PORT_PARTNER_FLAG_ALT_MODE(x)   READ_SINGLE_BIT_IN_BYTES(x,5,BIT1)  //Set bit41
                                                              // Bits: 46:Lenovo Dock check
		#define	TRTS5455_LENOVO_DOCK_CHECK(x)	READ_SINGLE_BIT_IN_BYTES(x,5,BIT6)	//Set bit46
        #define TRTS5455_LENOVO_DOCK_CHECK_THIRD_PARTY_DOCK         0     //
        #define TRTS5455_LENOVO_DOCK_CHECK_LENOVO_DOCK			     1     //
                                                              // Bits: 47:Lenovo Laptop check
		#define	TRTS5455_LENOVO_LAPTOP_CHECK(x)	READ_SINGLE_BIT_IN_BYTES(x,5,BIT7)	//Set bit47
        #define TRTS5455_LENOVO_LAPTOP_CHECK_LENOVO_LAPTOP_UNATTACHED         0     //
        #define TRTS5455_LENOVO_LAPTOP_CHECK_LENOVO_LAPTOP_ATTACHED			     1     //

        #define	TRTS5455_REQUEST_DATA_OBJECT1(x)	READ_MULTI_BIT_IN_BYTES(x,6,0xFF)	//Set bit48..55
        #define	TRTS5455_REQUEST_DATA_OBJECT2(x)	READ_MULTI_BIT_IN_BYTES(x,7,0xFF)	//Set bit56..63
        #define	TRTS5455_REQUEST_DATA_OBJECT3(x)	READ_MULTI_BIT_IN_BYTES(x,8,0xFF)	//Set bit64..71
        #define	TRTS5455_REQUEST_DATA_OBJECT4(x)	READ_MULTI_BIT_IN_BYTES(x,9,0xFF)	//Set bit72..79

        #define	TRTS5455_PORT_PARTNER_TYPE(x)	READ_MULTI_BIT_IN_BYTES(x,10,0x07)	//Set bit80..82
        #define TRTS5455_PORT_PARTNER_DFP         1     //
        #define TRTS5455_PORT_PARTNER_UFP		      2     //
                                                              // Bits: 106..104:Alternate mode relate status
		#define	RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS(x)	READ_MULTI_BIT_IN_BYTES(x,13,0x07)	//Set bit106..104
        #define RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS_DISCOVER_IDENTITY_NOT         0     //
        #define RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS_DISCOVER_IDENTITY_DONE         1     //
        #define RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS_DISCOVER_SVID_DONE         2     //
        #define RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS_DISCOVER_MODE_DONE         3     //
        #define RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS_ENTER_MODE_DONE         4     //
        #define RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS_PD_STATUS_UPDATE_COMMAND_DONE         5     //
        #define RTS5455_GET_ALTERNATE_MODE_RELAT_STATUS_PD_CONFIGURE_COMMAND_DONE         6     //
#define RTS5455_ACK_CC_CI				                        0x0A00          //
		#define RTS5455_ACK_CC_CI_WR_DATA_LEN 							0x07	          //
		#define RTS5455_ACK_CC_CI_COMMAND_COMPLETE_ACK				          //bit64
#define RTS5455_GET_DEFERRED_CMD_STATUS                 0x0B00          //
		#define RTS5455_GET_DEFERRED_CMD_STATUS_WR_DATA_LEN 0x03	          //
#define RTS5455_GET_DEFERRED_CMD_DATA                   0x0C00          //
		#define RTS5455_GET_DEFERRED_CMD_DATA_WR_DATA_LEN		0x03	          //
#define RTS5455_GET_ERROR_STATUS                       0x0D00          //
		#define RTS5455_GET_ERROR_STATUS_WR_DATA_LEN 			0x03	          //
		#define RTS5455_GET_ERROR_STATUS_PORT_NUM		 			0xFF	          //
//#define RTS5455_CANCEL					                        0x0E00          //
//		#define RTS5455_CANCEL_WR_DATA_LEN 									0x03	          //
#define RTS5455_TCPM_RESET			                        0x0E03          //
		#define RTS5455_TCPM_RESET_WR_DATA_LEN 							0x05	          //
		#define RTS5455_TCPM_RESET_RESET_TYPE				          //bit33..32
        #define RTS5455_TCPM_RESET_RESET_TYPE_PD_HARD_RESET         1     //
        #define RTS5455_TCPM_RESET_RESET_TYPE_PD_SOFT_RESET		      0     //
#define RTS5455_PPM_RESET			                        0x0E01          //
#define RTS5455_PPM_RESET_WR_DATA_LEN                          0x02             //

#define RTS5455_SET_NOTIFICATION_ENABLE                 0x0801          //
		#define RTS5455_SET_NOTIFICATION_ENABLE_WR_DATA_LEN 0x06	          //
#define RTS5455_GET_CAPABILITY                 0x0E06          //
		#define RTS5455_GET_CAPABILITY_WR_DATA_LEN 0x02
#define RTS5455_GET_PORT_CAPABILITY                     0x0E07          //
		#define RTS5455_GET_PORT_CAPABILITY_WR_DATA_LEN 		0x02	          //
#define RTS5455_GET_CONNECTOR_STATUS                     0x0E12          //
		#define RTS5455_GET_CONNECTOR_STATUS_WR_DATA_LEN 		0x02	          //
//#define RTS5455_SET_PORT_CAPABILITY                     0x0802          //
//		#define RTS5455_SET_PORT_CAPABILITY_WR_DATA_LEN 		0x03	          //
#define RTS5455_SET_PDO			                      		  0x0803          //
//		#define RTS5455_SET_PDO_WR_DATA_LEN 								0x03	          //
//#define RTS5455_GET_PDO			                     		    0x0883          //
//		#define RTS5455_GET_PDO_WR_DATA_LEN 								0x03	          //
#define RTS5455_SET_RDO			                     		    0x0804          //
		#define RTS5455_SET_RDO_WR_DATA_LEN 								0x06	          //
#define RTS5455_GET_RDO			                     		    0x0884          //
		#define RTS5455_GET_RDO_WR_DATA_LEN 								0x02	          //
                                                              // Bits: 30..28: Current Object Position
		#define	RTS5455_GET_RDO_OBJECT_POSITION_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,3,0x70)>>4)	//Set bit30..28
#define RTS5455_SET_TPC_CURRENT			                    0x0805          //
		#define RTS5455_SET_TPC_CURRENT_WR_DATA_LEN 				0x03	          //
#define RTS5455_GET_TPC_CURRENT			                    0x0885          //
		#define RTS5455_GET_TPC_CURRENT_WR_DATA_LEN 				0x02	          //
//#define RTS5455_GET_CABLE_VDO			                      0x0886          //
//		#define RTS5455_GET_CABLE_VDO_WR_DATA_LEN 					0x0	          //
//#define RTS5455_GET_BATTERY_STS			                    0x0887          //
//		#define RTS5455_GET_BATTERY_STS_WR_DATA_LEN 				0x03	          //
#define RTS5455_SET_FEATURE			                        0x0808          //
		#define RTS5455_SET_FEATURE_WR_DATA_LEN 						0x0	          //
#define RTS5455_SET_UOM			                        		0x0E08          //
		#define RTS5455_SET_UOM_WR_DATA_LEN 								0x03	          //
#define RTS5455_SET_UOR					                        0x0E09          //
		#define RTS5455_SET_UOR_WR_DATA_LEN 								0x03	          //
#define RTS5455_SET_PDM			                     		    0x0E0A          //
		#define RTS5455_SET_PDM_WR_DATA_LEN 								0x03	          //
#define RTS5455_SET_PDR			                    		    0x0E0B          //
		#define RTS5455_SET_PDR_WR_DATA_LEN 								0x03	          //
#define RTS5455_GET_PDO			                    		    0x0E10          //
		#define RTS5455_GET_PDO_WR_DATA_LEN 								0x03
#define	TRTS5455_Dual_Role_Power(x)	        READ_SINGLE_BIT_IN_BYTES(x,3,BIT5)	
 #define Dule_role_power_Dock		     1
#define RTS5455_SET_ALTERNATE_MODE			                0x0E0C          //
		#define RTS5455_SET_ALTERNATE_MODE_WR_DATA_LEN 			0x05	          //
#define RTS5455_GET_ALTERNATE_MODE                      0x088D          //
		#define RTS5455_GET_ALTERNATE_MODE_WR_DATA_LEN 			0x0	          //
#define RTS5455_GET_PAM_SUPPORTED                       0x0E0D          //
		#define RTS5455_GET_PAM_SUPPORTED_WR_DATA_LEN 			0x02	          //
#define RTS5455_GET_CURRENT_PAM	                        0x0E0E          //
		#define RTS5455_GET_CURRENT_PAM_WR_DATA_LEN 				0x02	          //
#define RTS5455_SET_NEW_PAM			                        0x0810          //
		#define RTS5455_SET_NEW_PAM_WR_DATA_LEN 						0x0	          //
#define RTS5455_GET_CABLE_PROPERTY                      0x0E11          //
		#define RTS5455_GET_CABLE_PROPERTY_WR_DATA_LEN 			0x02	          //
#define RTS5455_SEND_VDM				                        0x0819          //
		#define RTS5455_SEND_VDM_WR_DATA_LEN 								0x0	          //
#define RTS5455_GET_VDM					                        0x0899          //
		#define RTS5455_GET_VDM_WR_DATA_LEN 								0x02	          //
#define RTS5455_SET_VDO					                        0x081A          //
		#define RTS5455_SET_VDO_WR_DATA_LEN 								0x0	          //
#define RTS5455_GET_VDO					                        0x089A          //
		#define RTS5455_GET_VDO_WR_DATA_LEN 								0x0	          //
#define RTS5455_GET_PORT_OPERATION_MODE                 0x089C          //
		#define RTS5455_GET_PORT_OPERATION_MODE_WR_DATA_LEN 0x0	          //
#define RTS5455_SET_TPC_CSD_OPERATION_MODE		          0x081D          //
		#define RTS5455_SET_TPC_CSD_OPERATION_MODE_WR_DATA_LEN 0x0	          //
#define RTS5455_GET_TPC_CSD_OPERATION_MODE			        0x089D          //
		#define RTS5455_GET_TPC_CSD_OPERATION_MODE_WR_DATA_LEN 0x0	          //
#define RTS5455_SET_TPC_RECONNECT                       0x081F          //
		#define RTS5455_SET_TPC_RECONNECT_WR_DATA_LEN 			0x0	          //
#define RTS5455_INIT_PD_AMS			                        0x0820          //
		#define RTS5455_INIT_PD_AMS_WR_DATA_LEN 						0x03	          //
		#define RTS5455_INIT_PD_AMS_PR_SWAP 								0x01	          //
		#define RTS5455_INIT_PD_AMS_DR_SWAP 								0x02	          //
		#define RTS5455_INIT_PD_AMS_VCONN_SWAP 								0x03	          //
		#define RTS5455_INIT_PD_AMS_SOURCE_CAP 								0x04	          //
		#define RTS5455_INIT_PD_AMS_REQUSET 								0x05	          //
		#define RTS5455_INIT_PD_AMS_SOFT_RESET 								0x06	          //
		#define RTS5455_INIT_PD_AMS_HARD_RESET 								0x07	          //
		#define RTS5455_INIT_PD_AMS_GOTOMIN 								0x08	          //
		#define RTS5455_INIT_PD_AMS_GET_SNK_CAP 								0x09	          //
		#define RTS5455_INIT_PD_AMS_GET_SRC_CAP 								0x0A	          //
		#define RTS5455_INIT_PD_AMS_BIST1 								0x0B	          //
		#define RTS5455_INIT_PD_AMS_BIST2 								0x0C	          //
		#define RTS5455_INIT_PD_AMS_ALT_PROCESS 								0x0D	          //
		#define RTS5455_INIT_PD_AMS_EXIT_MODE 								0x0E	          //
		#define RTS5455_INIT_PD_AMS_ATTENTION 								0x0F	          //
		#define RTS5455_INIT_PD_AMS_CABLE_DETECTION 								0x10	          //
		#define RTS5455_INIT_PD_AMS_CABLE_RESET 								0x11	          //
		#define RTS5455_INIT_PD_AMS_GET_SOURCE_CAP_EXTEND 								0x30	          //
		#define RTS5455_INIT_PD_AMS_GET_STATUS 								0x31	          //
		#define RTS5455_INIT_PD_AMS_GET_BATTERY_CAP 								0x32	          //
		#define RTS5455_INIT_PD_AMS_GET_BATTERY_STATUS 								0x33	          //
		#define RTS5455_INIT_PD_AMS_GET_MF_INFO 								0x34	          //
		#define RTS5455_INIT_PD_AMS_SECURITY_REQUEST 								0x35	          //
		#define RTS5455_INIT_PD_AMS_FIRMWARE_UPDATE 								0x36	          //
#define RTS5455_FORCE_SET_POWER_SWITCH                  0x0821          //
		#define RTS5455_FORCE_SET_POWER_SWITCH_WR_DATA_LEN 	0x0	          //
#define RTS5455_GET_AMS_STATUS			                    0x0822          //
		#define RTS5455_GET_AMS_STATUS_WR_DATA_LEN 					0x02	          //
#define RTS5455_SET_TPC_DISCONNECT			                0x0823          //
		#define RTS5455_SET_TPC_DISCONNECT_WR_DATA_LEN 			0x0	          //
#define RTS5455_ACK_POWER_CTRL_REQ			                0x0824          //
		#define RTS5455_ACK_POWER_CTRL_REQ_WR_DATA_LEN			0x0	          //
#define RTS5455_GET_POWER_CTRL_REQ			                0x08A4          //
		#define RTS5455_GET_POWER_CTRL_REQ_WR_DATA_LEN 			0x0	          //
#define RTS5455_SET_INTRUSIVE_MODE                      0x0825          //
		#define RTS5455_SET_INTRUSIVE_MODE_WR_DATA_LEN			0x0	          //
#define RTS5455_GET_INTRUSIVE_MODE                      0x08A5          //
		#define RTS5455_GET_INTRUSIVE_MODE_WR_DATA_LEN 			0x0	          //
#define RTS5455_GET_CSD			                  		      0x08F0          //
		#define RTS5455_GET_CSD_WR_DATA_LEN 								0x0	          //
#define RTS5455_ENABLE_DISABLE_SINK			                  		      0x1500          //
		#define RTS5455_ENABLE_DISABLE_SINK_LEN 								0x03	          //
#define RTS5455_GET_IC_STATUS			                  		      0x3A          //
		#define RTS5455_GET_IC_STATUS_LEN 								0x03	          //
#define RTS5455_READ_TRANSFER_DATA                  		      0x80          //

#define        Enable_Sink                            0x01
#define        Disable_Sink                            0x00

/*
*     Type-C docking
*/
 #define 		TYPE_C_DOCKING_EVENT							BIT0
 #define    TYPE_C_DOCKING_EVENT_BUTTON_WOL   BIT1
 #define 		TYPE_C_DOCKING_EVENT_BUTTON_MAKE	BIT2
 #define 		TYPE_C_DOCKING_EVENT_BUTTON_BREAK	BIT3

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
struct _USB_REQ {
//    struct _USB_REQ    *next;
    struct {
        BOOL           done: 1;
        BOOL           busy: 1;
    } status;
    WORD              request;
    WORD              result;
    WORD             parameter[2];
    void               *address;
//    USB_PDC_CALL_BACK  *call_back;
};

typedef struct _USB_REQ USB_PDC_REQ;

enum _USB_BC12_STATUS {
    USB_BC12_NONE,
    USB_BC12_SDP,
    USB_BC12_CDP,
    USB_BC12_DCP,
};

typedef enum _USB_BC12_STATUS USB_BC12_STATUS;

enum _USB_TYPE_C_CURRENT {
    USB_TYPE_C_CURRENT_NONE,
    USB_TYPE_C_CURRENT_DEFAULT,
    USB_TYPE_C_CURRENT_BC,
    USB_TYPE_C_CURRENT_PD_CONTRACT,
    USB_TYPE_C_CURRENT_1_5_A,
    USB_TYPE_C_CURRENT_3_0_A,
};

typedef enum _USB_TYPE_C_CURRENT USB_TYPE_C_CURRENT;

struct _USB_POWER_STATUS {
    BOOL               power_connection:  1;
    USB_BC12_STATUS  bc12_status:       3;
    USB_TYPE_C_CURRENT type_c_current:    3;
    BOOL               power_source:      1;   // power role is source
};

typedef struct _USB_POWER_STATUS USB_POWER_STATUS;

enum _USB_CONNECT {
    USB_CONNECT_NONE,
    USB_CONNECT_CONNECTED,
    USB_CONNECT_AUDIO_ACCESSORY,
    USB_CONNECT_DEBUG_ACCESSORY,
    USB_CONNECT_DP,
};

typedef enum _USB_CONNECT USB_CONNECT;

enum _USB_HOST {
    USB_HOST_NONE,                      // VBus is not provided
    USB_HOST_PD,                        // VBus is provided by PD (with USB functions)
    USB_HOST_PD_NO_USB,                 // VBus is provided by PD (no USB function)
    USB_HOST_NONE_PD,                   // VBUs is provided by a device (no PD function)
};

typedef enum _USB_HOST USB_HOST;

struct _USB_STATUS {
    BOOL        device_connected:  1;   // a device is connected
    BOOL        dfp:               1;   // Connector acts as DFP
    USB_CONNECT connect_status:    3;   // type of the device connected
    USB_HOST    usb_host:          3;   // VBus state
};

typedef struct _USB_STATUS USB_STATUS;

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
extern void OEM_TYPE_C_ADPID_HOOK(void);
extern void ECResetTypecPD(void);
extern void InitialTypecPD(void);
extern void TypecPDProcess(void);
extern void OEM_TYPE_C_init_hook(void);
extern void OEM_TYPE_C_HOOK(void);
extern void OEM_TYPE_C_S3_HOOK(void);
extern void OEM_TYPE_C_S5_HOOK(void);
extern void OEM_TYPE_C_S5S0_HOOK(void);
extern void SendPdoSet_ReceiveByte(void);
//extern void Oem_Hook_Type_C_BeforeSleep(void);
extern void GetTypecPDVersion(void);
//extern void SendPdcVdm(BYTE DataCnt);
//extern BYTE RTS5455_WaitPing(void);
extern BYTE ClearPdEvents(BYTE CurrentStep);
//extern void SendLenovoAttentionVdmToPdc(void);
//extern void SendGetLenovoStatusVdmToPdc(void);
extern void SendPowerModeChangeToPdc(void);
extern void SendPdoSet(BYTE PDOSetNum);
//extern void UsbPdcGetPowerStatus(BYTE usb_pdc_id);
//extern void UsbPdcGetStatus(BYTE usb_pdc_id);
extern BYTE UsbPdcRequest(BYTE usb_pdc_id, USB_PDC_REQ *request_packet, BYTE cmd, WORD parm1, WORD parm2,void *address);
extern void P2P_Process(void);
extern void SendUnitInformationToPdc(void);
extern void SinkPowerPathControl(BYTE OnOff);
extern void GetConnectorStatus_RTS5455(BYTE state);
extern void SET_TPC_RECONNECT_RTS5455(BYTE state);
extern void SENT_PD_RETRY_ReceiveByte(void);
extern void SENT_UNIT_RETRY_ReceiveByte(void);
extern void USB_PDC_ReceiveByte(void);
extern void P2P_Charge_Condition(void);
extern void Sinkpower_ReceiveByte(void);
#endif  //Support_TYPE_C
#endif
