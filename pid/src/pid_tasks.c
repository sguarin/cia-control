/*=====[pid_tasks]=============================================================

/*=====[Inclusions of private function dependencies]=========================*/

#include "sapi.h"
#include "pid_tasks.h"
#include "pid.h"

/*=====[Definition macros of private constants]==============================*/

#define DEBUG_PRINT_PROCESSING_TIME 0

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

void console_print (pid_control_t* pid);

/*=====[Implementations of public functions]=================================*/


void PID_Task( void* taskParmPtr )
{
	uint32_t u = 0;
    pid_control_t *pid;
	portTickType xLastWakeTime;
#if DEBUG_PRINT_PROCESSING_TIME
	uint32_t startCycles, cycles;
#endif

	// get task parameter
	pid = (pid_control_t*) taskParmPtr;

	// Relative delay
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		u++;
#if DEBUG_PRINT_PROCESSING_TIME
		startCycles = cyclesCounterRead();
#endif
        // Run PID
        pid_run(pid);
#if DEBUG_PRINT_PROCESSING_TIME
		cycles = cyclesCounterRead() - startCycles;
		printf("Cycles processing %u Time [us] %f\r\n", cycles, cyclesCounterToUs( cycles));
#endif

        //if (! (u % 10000))
        	// print debug
        	//console_print(b-a);

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
