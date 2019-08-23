#include <stdbool.h>
#include "main.h"
#include "cmsis_os.h"

#define DIRECTION_ARRAY_LEN		(50)

#define UP_SIG					(1 << 0)
#define DOWN_SIG				(1 << 1)
#define LEFT_SIG				(1 << 2)
#define RIGHT_SIG				(1 << 3)
#define ENTER_SIG				(1 << 4)
#define	ANY_SIG					(0x001F)

typedef enum
{
	upDir,
	downDir,
	leftDir,
	rightDir
} DIRECTION;

static osThreadId sMotorTaskHandle;
static osTimerId sMotorTimerHandle;
static DIRECTION sDirectionArray[DIRECTION_ARRAY_LEN];
static uint8_t sDirectionMax = 0;
static uint8_t sDirectionPos = 0;
static bool sCommandEnable = true;
static bool sDriveMode = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(sCommandEnable)
	{
		switch(GPIO_Pin)
		{
			case UP_Pin:
				sCommandEnable = false;
				osSignalSet (sMotorTaskHandle, UP_SIG);
				break;
			case DOWN_Pin:
				sCommandEnable = false;
				osSignalSet (sMotorTaskHandle, DOWN_SIG);
				break;
			case LEFT_Pin:
				sCommandEnable = false;
				osSignalSet (sMotorTaskHandle, LEFT_SIG);
				break;
			case RIGHT_Pin:
				sCommandEnable = false;
				osSignalSet (sMotorTaskHandle, RIGHT_SIG);
				break;
			case ENTER_Pin:
				sCommandEnable = false;
				osSignalSet (sMotorTaskHandle, ENTER_SIG);
				break;
		}
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
		event = osSignalWait(ANY_SIG, osWaitForever);
		if(event.value.signals & ENTER_SIG)
		{
			sDirectionPos = 0;
			if(sDirectionPos < sDirectionMax)
			{
				sDriveMode = true;
				motorDrive(sDirectionArray[sDirectionPos++]);
			}
			osTimerStart(sMotorTimerHandle, 1000);
		}
		else
		{
			if(event.value.signals & UP_SIG) sDirectionArray[sDirectionMax] = upDir;
			else if(event.value.signals & DOWN_SIG) sDirectionArray[sDirectionMax] = downDir;
			else if(event.value.signals & LEFT_SIG) sDirectionArray[sDirectionMax] = leftDir;
			else if(event.value.signals & RIGHT_SIG) sDirectionArray[sDirectionMax] = rightDir;
			if(sDirectionMax < DIRECTION_ARRAY_LEN) sDirectionMax++;
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			osTimerStart(sMotorTimerHandle, 1000);
		}
	}
}

void motorInit(void)
{
	osThreadDef(motorTask, motorTask, osPriorityHigh, 0, 128);
	sMotorTaskHandle = osThreadCreate(osThread(motorTask), NULL);
	osTimerDef(motorTimer, timerCallback);
	sMotorTimerHandle = osTimerCreate(osTimer(motorTimer), osTimerPeriodic, NULL);
}
