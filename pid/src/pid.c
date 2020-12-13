/*=====[pid]===================================================================

/*=====[Inclusions of private function dependencies]=========================*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "sapi.h"
#include "pid.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

float pid_control(pid_config_t *config, pid_state_t *state, float y, float r);

/*=====[Implementations of public functions]=================================*/

void pid_init(pid_control_t *pid, uint32_t ts_ms, void (*pfr)(float *, float *), void (*pft)(float *)) {
	float Kp, Ki, Kd;

	pid->ts_ms = ts_ms;

	pid->p_receive = pfr;
	pid->p_transmit = pft;

	pid->config.h = (float)ts_ms / 1000;

	// sobrepico
	Kp = 2;
	Ki = 500;
	Kd = 0.0015;

	// Segunda opcion
	Kp = 1.3;
	Ki = 300;
	Kd = 0.0001;

	pid->config.Kp = Kp;
	pid->config.Ti = Kp / Ki;
	pid->config.Td = Kd / Kp;

	pid->config.b = 1;
	pid->config.N = 5;

	pid->state.futureI = 0;
	pid->state.pastY = 0;
	pid->state.pastD = 0;
}

void pid_run(pid_control_t *pid) {
	float r, y, u;
	static float y_1 = 0;

	// receive reference and output
	pid->p_receive(&r, &y);

	// should be y[n-1]?
	u = pid_control(&(pid->config), &(pid->state), y_1, r);

	// save previous y
	y_1 = y;

	// pid output
	pid->p_transmit(&u);
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

float pid_control(pid_config_t *config, pid_state_t *state, float y, float r) {
	float P = 0;
	float D = 0;
	float I = 0;
	float U = 0;

	P = config->Kp * (config->b * r - y);
	D = (config->Td * state->pastD - config->N * config->Kp * config->Td * (y-state->pastY)) / (config->Td + config->N * config->h);
	I = state->futureI;

	U = P + I + D;

	state->pastD = D;
	state->pastY = y;
	state->futureI = I + config->Kp * config->h / config->Ti * (r - y);

	return U;
}
