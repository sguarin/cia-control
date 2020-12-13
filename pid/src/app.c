/*=====[app]===================================================================


/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
#include "task.h"

#include "sapi.h"
#include "pid.h"
#include "pid_tasks.h"


/*=====[Definition macros of private constants]==============================*/

// ADC0 Channels for u and y
#define ADC0_CH_Y       CH3

// adcRead() returns 10 bits integer sample (uint16_t)
// sampleInVolts = (3.3 / 1023.0) * adcSample
#define getVoltsSampleFrom(adc0Channel) 3.3*(float)adcRead((adc0Channel))/1023.0

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static StackType_t taskPIDStack[configMINIMAL_STACK_SIZE*20];
static StaticTask_t taskPIDTCB;

pid_control_t *pid;

/*=====[Prototypes (declarations) of private functions]======================*/

void receive_data (float *u, float *y);
void transmit_data (float *u);

/*=====[Main function, program entry point after power on or reset]==========*/

int main (void)
{
    // ----- Configurations --------------------------
    boardConfig();
    delayInaccurateMs(1000);

    // ----- Initialize ADC and DAC ------------------
    adcInit( ADC_ENABLE );
    dacInit( DAC_ENABLE );

    pid = (pid_control_t *) pvPortMalloc (sizeof(pid_control_t));

	pid_init(pid, 1, receive_data, transmit_data);

    xTaskCreateStatic(
         PID_Task,                  	// task function
         "PID Task",      				// human-readable neame of task
         configMINIMAL_STACK_SIZE*20,   // task stack size
         (void*)pid,              		// task parameter (cast to void*)
         tskIDLE_PRIORITY+1,         	// task priority
         taskPIDStack,    				// task stack (StackType_t)
         &taskPIDTCB      				// pointer to Task TCB (StaticTask_t)
     );

    vTaskStartScheduler();

    // ----- Repeat for ever -------------------------
    while(TRUE);

    // YOU NEVER REACH HERE, because this program runs directly or on a
    // microcontroller and is not called by any Operating System, as in the 
    // case of a PC program.
    return 0;
}

/*=====[Implementations of private functions]================================*/

// receive hook for reference and output of system
void receive_data(float *r, float *y)
{
    static uint32_t count = 0;
    static float rvalue = 0;

    // synth reference
    if (count % 50 == 0)
    	rvalue = (rvalue == (float)0) ? (float)2 : (float)0;
	count++;

	*r = rvalue;
	*y = (float) getVoltsSampleFrom( ADC0_CH_Y );
}

// transmit hook of pid output
void transmit_data(float *u) {
	uint16_t dacValue;

	dacValue = (uint16_t)(*u * 1023.0 / 3.3);
	dacWrite( DAC, dacValue );

    // Need at least 2.5 us to uptate DAC.
    delayInaccurateUs(5);
}
