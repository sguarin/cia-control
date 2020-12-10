/*=====[pid]===================================================================

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _PID_H_
#define _PID_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

typedef struct {
	float Kp;
	float Ti;
	float Td;
	float h;
	float N;
	float b;
} pid_config_t;

typedef struct {
	float pastD;
	float pastY;
	float futureI;
} pid_state_t;

typedef struct {
	uint32_t ts_ms;

	void (*p_receive) (float *, float *);   // Recibe señal de la planta
	void (*p_transmit) (float *);	        // Transmite señal a la planta

	pid_config_t config;
	pid_state_t state;
} pid_control_t;


/*=====[Prototypes (declarations) of public functions]=======================*/

// Initialize pid controller
void pid_init(pid_control_t* pid, uint32_t ts_ms, void (*pfr)(float *, float *), void (*pft)(float *));

// Recurrent execution of pid controller
void pid_run(pid_control_t* pid);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _PID_H_ */
