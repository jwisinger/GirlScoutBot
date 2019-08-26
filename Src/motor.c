#include <stdbool.h>
#include "main.h"
#include "cmsis_os.h"
#include "app_x-cube-ble1.h"
#include "motor.h"

#define DIRECTION_ARRAY_LEN		(50)

#define UP_SIG					(1 << 0)
#define DOWN_SIG				(1 << 1)
#define LEFT_SIG				(1 << 2)
#define RIGHT_SIG				(1 << 3)
#define ENTER_SIG				(1 << 4)
#define BLE_SIG					(1 << 5)
#define	ANY_SIG					(0x003F)

static osThreadId sMotorTaskHandle;
static osTimerId sMotorTimerHandle;
static DIRECTION sDirectionArray[DIRECTION_ARRAY_LEN];
static uint8_t sDirectionMax = 0;
static uint8_t sDirectionPos = 0;
static bool sCommandEnable = true;
static bool sDriveMode = false;
static uint16_t sDebounceDelay = 1000;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case UP_Pin:
			motorCommand(upDir, 1000);
			break;
		case DOWN_Pin:
			motorCommand(downDir, 1000);
			break;
		case LEFT_Pin:
			motorCommand(leftDir, 1000);
			break;
		case RIGHT_Pin:
			motorCommand(rightDir, 1000);
			break;
		case ENTER_Pin:
			motorCommand(enterKey, 1000);
			break;
	}
}

bool motorCommand(DIRECTION dir, uint16_t delay)
{
	bool retVal = false;

	if(sCommandEnable)
	{
		switch(dir)
		{
			case upDir:
				sCommandEnable = false;
				retVal = true;
				osSignalSet (sMotorTaskHandle, UP_SIG);
				break;
			case downDir:
				sCommandEnable = false;
				retVal = true;
				osSignalSet (sMotorTaskHandle, DOWN_SIG);
				break;
			case leftDir:
				sCommandEnable = false;
				retVal = true;
				osSignalSet (sMotorTaskHandle, LEFT_SIG);
				break;
			case rightDir:
				sCommandEnable = false;
				retVal = true;
				osSignalSet (sMotorTaskHandle, RIGHT_SIG);
				break;
			case enterKey:
				sCommandEnable = false;
				retVal = true;
				osSignalSet (sMotorTaskHandle, ENTER_SIG);
				break;
		}
		sDebounceDelay = delay;
	}

	return retVal;
}

void motorBleSignal(void)
{
	if (sMotorTaskHandle != 0)
	{
		osSignalSet (sMotorTaskHandle, BLE_SIG);
	}
}

static void motorDrive(DIRECTION d)
{
	switch(d)
	{
		case upDir:
			HAL_GPIO_WritePin(RIGHT_MOTOR_GPIO_Port, RIGHT_MOTOR_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LEFT_MOTOR_GPIO_Port, LEFT_MOTOR_Pin, GPIO_PIN_SET);
			break;
		case downDir:
			break;
		case leftDir:
			HAL_GPIO_WritePin(RIGHT_MOTOR_GPIO_Port, RIGHT_MOTOR_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LEFT_MOTOR_GPIO_Port, LEFT_MOTOR_Pin, GPIO_PIN_RESET);
			break;
		case rightDir:
			HAL_GPIO_WritePin(RIGHT_MOTOR_GPIO_Port, RIGHT_MOTOR_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LEFT_MOTOR_GPIO_Port, LEFT_MOTOR_Pin, GPIO_PIN_SET);
			break;
		default:
			break;
	}
}

static void timerCallback(void const * argument)
{
	if(sDriveMode)
	{
		if(sDirectionPos < sDirectionMax)
		{
			motorDrive(sDirectionArray[sDirectionPos++]);
			osTimerStart(sMotorTimerHandle, 1000);
			return;
		}
		else
		{
			sDriveMode = false;
			sDirectionMax = 0;
		}
	}

	HAL_GPIO_WritePin(RIGHT_MOTOR_GPIO_Port, RIGHT_MOTOR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEFT_MOTOR_GPIO_Port, LEFT_MOTOR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	sCommandEnable = true;
}

static void motorTask(void const * argument)
{
	osEvent event;

	while(1)
	{
		MX_BlueNRG_MS_Process();
		event = osSignalWait(ANY_SIG, osWaitForever);
		if(event.value.signals & ENTER_SIG)
		{
			sDirectionPos = 0;
			if(sDirectionPos < sDirectionMax)
			{
				sDriveMode = true;
				motorDrive(sDirectionArray[sDirectionPos++]);
			}
			osTimerStart(sMotorTimerHandle, sDebounceDelay);
		}
		else if(event.value.signals & (UP_SIG | DOWN_SIG | LEFT_SIG | RIGHT_SIG))
		{
			if(event.value.signals & UP_SIG) sDirectionArray[sDirectionMax] = upDir;
			else if(event.value.signals & DOWN_SIG) sDirectionArray[sDirectionMax] = downDir;
			else if(event.value.signals & LEFT_SIG) sDirectionArray[sDirectionMax] = leftDir;
			else if(event.value.signals & RIGHT_SIG) sDirectionArray[sDirectionMax] = rightDir;
			if(sDirectionMax < DIRECTION_ARRAY_LEN) sDirectionMax++;
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			osTimerStart(sMotorTimerHandle, sDebounceDelay);
		}
	}
}

void motorInit(void)
{
	osThreadDef(motorTask, motorTask, osPriorityHigh, 0, 512);
	sMotorTaskHandle = osThreadCreate(osThread(motorTask), NULL);
	osTimerDef(motorTimer, timerCallback);
	sMotorTimerHandle = osTimerCreate(osTimer(motorTimer), osTimerPeriodic, NULL);
}
