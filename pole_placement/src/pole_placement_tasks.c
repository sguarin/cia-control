/*=====[plole_placement_tasks]=================================================
 
/*=====[Inclusions of private function dependencies]=========================*/

#include "sapi.h"

#include "arm_math.h"
#include <math.h>

#include "pole_placement_tasks.h"
#include "pole_placement.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

void console_print (float* buffer);

/*=====[Implementations of public functions]=================================*/


void pole_placement_task( void* taskParmPtr )
{
    pole_placement_t *pole_placement;
	portTickType xLastWakeTime;

	pole_placement = (pole_placement_t *) taskParmPtr;

	// Para implementar un delay relativo
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
        pole_placement_run(pole_placement);

        // Imprimo los parámetros calculados
        //console_print(tIRLS->buffer_T);

		vTaskDelayUntil( &xLastWakeTime, ( pole_placement->ts_ms / portTICK_RATE_MS ) ); // dividi por dos para probar en 500ms de sample time
	}
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

// Función para imprimir los parámetros la planta (2do orden)
void console_print (float* buffer)
{
	uint32_t i;
	int32_t integer, fraction;

	// Imprimo los parámetros calculados
	//printf ("T=[");
	for (i = 0; i<5; i++)
	{
		// Casteo de float a int

		integer = (int)buffer[i];
		fraction = (int)(((buffer[i] - (float)integer)) * 1000);
        if (fraction<0)
        {
            fraction = (-1)*fraction;
            if (integer==0)	
            {
                printf("-%d.%03d;", integer, fraction);
            }
            else 
            {
                printf("%d.%03d;", integer, fraction);
            }
        }
        else 
        {
            printf("%d.%03d;", integer, fraction);
        }
	}
	printf("\r\n");
}
