/**
  ******************************************************************************
  * File Name          : app_x-cube-ble1.c
  * Description        : Implementation file
  *             
  ******************************************************************************
  *
  * COPYRIGHT 2019 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_X_CUBE_BLE1_C
#define __APP_X_CUBE_BLE1_C

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <MX_app_x-cube-ble1.h>

#include "hci_tl.h"
#include <MX_sensor_service.h>
#include "bluenrg_utils.h"
#include "custom.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private defines -----------------------------------------------------------*/
#define BDADDR_SIZE 6
 
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern volatile uint8_t set_connectable;
extern volatile int     connected;
uint8_t bnrg_expansion_board = IDB04A1; /* at startup, suppose the X-NUCLEO-IDB04A1 is used */

void MX_BlueNRG_MS_Init(void)
{
  /* USER CODE BEGIN SV */ 

  /* USER CODE END SV */
  
  /* USER CODE BEGIN BlueNRG_MS_Init_PreTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PreTreatment */

  /* Initialize the peripherals and the BLE Stack */
  const char *name = "BlueNRG";
  uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03};
  uint8_t bdaddr[BDADDR_SIZE];
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  
  uint8_t  hwVersion;
  uint16_t fwVersion;
  int ret;  
  
  hci_init(user_notify, NULL);
  
  /* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);
  	
  /* 
   * Reset BlueNRG again otherwise we won't
   * be able to change its MAC address.
   * aci_hal_write_config_data() must be the first
   * command after reset otherwise it will fail.
   */
  hci_reset();
  
  HAL_Delay(100);
 
  PRINTF("HWver %d\nFWver %d\n", hwVersion, fwVersion);
  
  if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
    bnrg_expansion_board = IDB05A1; 
    /*
     * Change the MAC address to avoid issues with Android cache:
     * if different boards have the same MAC address, Android
     * applications unless you restart Bluetooth on tablet/phone
     */
    SERVER_BDADDR[5] = 0x02;
  }

  /* The Nucleo board must be configured as SERVER */
  BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));
  
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);
  if (ret) {
    PRINTF("Setting BD_ADDR failed.\n");
  }
  
  ret = aci_gatt_init();    
  if(ret){
    PRINTF("GATT_Init failed.\n");
  }

  if (bnrg_expansion_board == IDB05A1) {
    ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }
  else {
    ret = aci_gap_init_IDB04A1(GAP_PERIPHERAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }

  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("GAP_Init failed.\n");
  }

  ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
                                   strlen(name), (uint8_t *)name);

  if (ret) {
    PRINTF("aci_gatt_update_char_value failed.\n");            
    while(1);
  }
  
  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL,
                                     7,
                                     16,
                                     USE_FIXED_PIN_FOR_PAIRING,
                                     123456,
                                     BONDING);
  if (ret == BLE_STATUS_SUCCESS) {
    PRINTF("BLE Stack Initialized.\n");
  }
  
  PRINTF("SERVER: BLE Stack Initialized\n");
  
  ret = Add_Acc_Service();
  
  if (ret == BLE_STATUS_SUCCESS)
    PRINTF("Acc service added successfully.\n");
  else
    PRINTF("Error while adding Acc service.\n");
  
  ret = Add_Environmental_Sensor_Service();
  
  if (ret == BLE_STATUS_SUCCESS)
    PRINTF("Environmental Sensor service added successfully.\n");
  else
    PRINTF("Error while adding Environmental Sensor service.\n");

  /* Set output power level */
  ret = aci_hal_set_tx_power_level(1,4);
  
  /* USER CODE BEGIN BlueNRG_MS_Init_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PostTreatment */
}

/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void)
{
	if (set_connectable)
	{
		setConnectable();
		set_connectable = FALSE;
	}
	hci_user_evt_proc();
}

#ifdef __cplusplus
}
#endif
#endif /* __APP_X_CUBE_BLE1_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
