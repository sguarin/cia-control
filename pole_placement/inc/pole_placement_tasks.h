/*=====[pole_placement_tasks]==================================================

 
/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _POLE_PLACEMENT_TASKS_H_
#define _POLE_PLACEMENT_TASKS_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "FreeRTOS.h"
#include "task.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

void pole_placement_task( void* taskParmPtr );

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _POLE_PLACEMENT_TASKS_H_ */
