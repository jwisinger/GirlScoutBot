#ifndef _SENSOR_SERVICE_H_
#define _SENSOR_SERVICE_H_

#include "bluenrg_gap.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"
#include "hci.h"
#include "hci_le.h"

tBleStatus Add_Acc_Service(void);
void setConnectable(void);
void user_notify(void * pData);

#endif
