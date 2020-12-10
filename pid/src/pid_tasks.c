/*=====[pid_tasks]=============================================================

/*=====[Inclusions of private function dependencies]=========================*/

#include "sapi.h"

#include "arm_math.h"
#include <math.h>

#include "pid_tasks.h"
#include "pid.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

void console_print (float* buffer);

/*=====[Implementations of public functions]=================================*/


void PID_Task( void* taskParmPtr )
{
	uint32_t u = 0;
    pid_control_t *pid;
	portTickType xLastWakeTime;

	pid = (pid_control_t*) taskParmPtr;

	// Para implementar un delay relativo
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		u++;
        // Run PID
        pid_run(pid);
        //if (! (u % 10000))
        	// print debug
        	//console_print(pid);

		vTaskDelayUntil( &xLastWakeTime, ( pid->ts_ms / portTICK_RATE_MS ) );
	}
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

// print debug
void console_print (pid_control_t* pid)
{
	//printf("Y: 1, %f, %f\r\n", -buffer[0], -buffer[1]);
}
