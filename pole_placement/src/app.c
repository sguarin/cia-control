/*=====[app]===================================================================

/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
#include "task.h"

#include "sapi.h"
#include "pole_placement.h"
#include "pole_placement_tasks.h"

/*=====[Definition macros of private constants]==============================*/

// ADC0 Channels for u and y
#define ADC0_CH_Y       CH3
#define ADC0_CH_X1      CH2

// Noise signal limits
#define DAC_REFERENCE_VALUE_HIGH   800  // 1023 = 3.3V, 666 = 2.15V
#define DAC_REFERENCE_VALUE_LOW    100  // 1023 = 3.3V, 356 = 1.15V

// adcRead() returns 10 bits integer sample (uint16_t)
// sampleInVolts = (3.3 / 1023.0) * adcSample
#define getVoltsSampleFrom(adc0Channel) 3.3*(float)adcRead((adc0Channel))/1023.0

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static StackType_t task_stack_pole_placement[configMINIMAL_STACK_SIZE*15];
static StaticTask_t task_tcp_pole_placement;

pole_placement_t *pole_placement;

/*=====[Prototypes (declarations) of private functions]======================*/

void receive_data (float *u, float *y, float *x1);
void transmit_data (float *u);

/*=====[Main function, program entry point after power on or reset]==========*/

int main (void)
{
    // ----- Configurations --------------------------
    boardConfig();

    // ----- Initialize ADC and DAC ------------------
    adcInit( ADC_ENABLE );
    dacInit( DAC_ENABLE );

    pole_placement = (pole_placement_t*) pvPortMalloc (sizeof(pole_placement_t));

	pole_placement_init(pole_placement, 1, receive_data, transmit_data);

     xTaskCreateStatic(
         pole_placement_task,                  // task function
         "Pole Placement Task",      // human-readable neame of task
         configMINIMAL_STACK_SIZE*10,   // task stack size
         (void*)pole_placement,              // task parameter (cast to void*)
         tskIDLE_PRIORITY+1,         // task priority
		 task_stack_pole_placement,    // task stack (StackType_t)
         &task_tcp_pole_placement      // pointer to Task TCB (StaticTask_t)
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
void receive_data(float *r, float *y, float *x1)
{
    static uint32_t count = 0;
    static float rvalue = 0;

    // synth reference
    if (count % 50 == 0)
    	rvalue = (rvalue == (float)0) ? (float)2 : (float)0;
	count++;

	*r = rvalue;
	*y = (float) getVoltsSampleFrom( ADC0_CH_Y );
	*x1 = (float) getVoltsSampleFrom( ADC0_CH_X1 );
}

// transmit hook of pid output
void transmit_data(float *u) {
	uint16_t dacValue;

	dacValue = (uint16_t)(*u * 1023.0 / 3.3);
	dacWrite( DAC, dacValue );

    // Need at least 2.5 us to uptate DAC.
    delayInaccurateUs(5);
}
