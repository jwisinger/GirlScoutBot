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
#include "app_x-cube-ble1.h"

/* USER CODE BEGIN Includes */
#include "MX_sensor_service.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
/* USER CODE END Includes */

/* Private Variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
 extern volatile uint8_t set_connectable;
/* USER CODE END PV */

/* Private Function Prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

#if PRINT_CSV_FORMAT
extern volatile uint32_t ms_counter;
/**
 * @brief  This function is a utility to print the log time
 *         in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
void print_csv_time(void){
  uint32_t ms = HAL_GetTick();
  PRINT_CSV("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
}
#endif

void MX_BlueNRG_MS_Init(void)
{
  /* USER CODE BEGIN SV */ 
	const char *name = "BlueNRG";
	uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x02};
	uint8_t bdaddr[sizeof(SERVER_BDADDR)];
	uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

	hci_init(user_notify, NULL);
	hci_reset();
	HAL_Delay(100);
	BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));
  	if(aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, bdaddr)) return;
	if(aci_gatt_init()) return;
	if(aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle)) return ;
	if(aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, strlen(name), (uint8_t *)name)) return;
	if(aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED, OOB_AUTH_DATA_ABSENT, NULL, 7, 16, USE_FIXED_PIN_FOR_PAIRING, 123456, BONDING)) return;
	if(Add_Acc_Service()) return;
	if(Add_Environmental_Sensor_Service()) return;
	if(aci_hal_set_tx_power_level(1,4)) return;
  /* USER CODE END SV */
  
  /* USER CODE BEGIN BlueNRG_MS_Init_PreTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PreTreatment */

  /* USER CODE BEGIN BlueNRG_MS_Init_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PostTreatment */
}

/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void)
{
  /* USER CODE BEGIN BlueNRG_MS_Process_PreTreatment */
	if (set_connectable)
	{
		setConnectable();
		set_connectable = FALSE;
	}
	hci_user_evt_proc();
  /* USER CODE END BlueNRG_MS_Process_PreTreatment */
  

  /* USER CODE BEGIN BlueNRG_MS_Process_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Process_PostTreatment */
}

/* USER CODE BEGIN Function Implementation */

/* USER CODE END Function Implementation */

#ifdef __cplusplus
}
#endif
#endif /* __APP_X_CUBE_BLE1_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
