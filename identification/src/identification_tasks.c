/*=====[identification_task]===================================================
 * Copyright 2019 Diego Fernández <dfernandez202@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2019/09/23
 */
 
/*=====[Inclusions of private function dependencies]=========================*/

#include "identification_tasks.h"

#include "sapi.h"

#include "arm_math.h"
#include <math.h>

#include "identification_rls.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

void console_print (float* buffer);

/*=====[Implementations of public functions]=================================*/


void IRLS_Task( void* taskParmPtr )
{
	uint32_t u = 0;
    t_IRLSdata* tIRLS;
	portTickType xLastWakeTime;

	tIRLS = (t_IRLSdata*) taskParmPtr;

	// Para implementar un delay relativo
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		u++;
        // Ejecuto el Identificador
        IRLS_Run(tIRLS);
        if (! (u % 10000))
        	// Imprimo los parámetros calculados
        	console_print(tIRLS->buffer_T);

		vTaskDelayUntil( &xLastWakeTime, ( tIRLS->ts_Ms / portTICK_RATE_MS ) );
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
	//printf("Den: 1, %f, %f\r\n", -buffer[0], -buffer[1]);
	//printf ("Parametros = [\t");
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
                printf("-%d.%03d\t", integer, fraction);
            }
            else
            {
                printf("%d.%03d\t", integer, fraction);
            }
        }
        else
        {
            printf("%d.%03d\t", integer, fraction);
        }
	}
	printf("\r\n");
}
