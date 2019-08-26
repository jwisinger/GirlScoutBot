#include <stdbool.h>

typedef enum
{
	upDir = 0,
	downDir = 1,
	leftDir = 2,
	rightDir = 3,
	enterKey = 4
} DIRECTION;

bool motorCommand(DIRECTION dir, uint16_t delay);
void motorInit(void);
void motorBleSignal(void);
