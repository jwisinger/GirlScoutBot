#include <MX_sensor_service.h>
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "motor.h"

#warning TODO: Clean up this file in general
__IO uint8_t set_connectable = 1;
uint16_t accServHandle, freeFallCharHandle;

/* Private macros ------------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

#define COPY_ACC_SERVICE_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_FREE_FALL_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0xe2,0x3e,0x78,0xa0, 0xcf,0x4a, 0x11,0xe1, 0x8f,0xfc, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)

tBleStatus Add_Acc_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_ACC_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 7,
                          &accServHandle);
  if (ret != BLE_STATUS_SUCCESS) return BLE_STATUS_ERROR;
  
  COPY_FREE_FALL_UUID(uuid);
  ret =  aci_gatt_add_char(accServHandle, UUID_TYPE_128, uuid, 1,
		  	  	  	  	   CHAR_PROP_READ|CHAR_PROP_WRITE, ATTR_PERMISSION_NONE, GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &freeFallCharHandle);
  if (ret != BLE_STATUS_SUCCESS) return BLE_STATUS_ERROR;

  return BLE_STATUS_SUCCESS; 
}

void setConnectable(void)
{  
	const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'G','S','_','B','o','t'};
	hci_le_set_scan_resp_data(0,NULL);
	aci_gap_set_discoverable(ADV_DATA_TYPE, ADV_INTERV_MIN, ADV_INTERV_MAX, PUBLIC_ADDR,
                                 NO_WHITE_LIST_USE, sizeof(local_name), local_name, 0, NULL, 0, 0);
}

void user_notify(void * pData)
{
	bool retVal;
	hci_uart_pckt *hci_pckt = pData;
	hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
	if(hci_pckt->type != HCI_EVENT_PKT) return;
  
	switch(event_pckt->evt)
	{
		case EVT_DISCONN_COMPLETE:
			set_connectable = TRUE;
			break;
		case EVT_VENDOR:
		{
			evt_blue_aci *blue_evt = (void*)event_pckt->data;
			if(blue_evt->ecode == EVT_BLUE_GATT_WRITE_PERMIT_REQ)
			{
				evt_gatt_write_permit_req *pr = (void*)blue_evt->data;
#warning TODO: Verify data is valid and length is good (allow for multiple steps in one command)
#warning TODO: Write value as number of items in list
				retVal = motorCommand(pr->data[0], 1000);
				if(pr->conn_handle != 0) aci_gatt_write_response(pr->conn_handle, pr->attr_handle, retVal, 0, pr->data_length, pr->data);
			}
			break;
		}
	}
}
