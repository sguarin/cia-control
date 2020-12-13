/*
*  Control Digital en Sistemas Embebidos - MSE
*/

/*=====[Inclusions of private function dependencies]=========================*/

#include <stdio.h>
#include <stdint.h>

#include "pole_placement.h"

/*=====[Definition macros of private constants]==============================*/

#define N_SAMPLES 1000



/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static float R[N_SAMPLES];
static float Y_OPEN[N_SAMPLES];
static float Y_POLE_PLACEMENT[N_SAMPLES];
static float U[N_SAMPLES];

static float NumZ[N_SAMPLES] = {0, 0.009};
static float DenZ[N_SAMPLES] = {1, -0.999};

/*=====[Prototypes (declarations) of private functions]======================*/

float pole_placement_control(pole_placement_config_t *config, float y, float x1, float r);

/*=====[Implementations of public functions]=================================*/

void pole_placement_init(pole_placement_t *data, uint32_t ts_ms, void (*pfr)(float *, float *, float *), void (*pft)(float *)) {

	float desired_pole = 0.9;

	data->ts_ms = ts_ms;

	data->p_receive = pfr;
	data->p_transmit = pft;

	data->config.K0 = 5.6;
	data->config.K1 = 3.7;
	data->config.K2 = 1.1;
}


void pole_placement_run(pole_placement_t *data) {
	float r, y, u, x1;

	// receive reference and output
	data->p_receive(&r, &y, &x1);

	// should be y[n-1]?
	u = pole_placement_control(&(data->config), y, x1, r);

	// pid output
	data->p_transmit(&u);
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

float pole_placement_control(pole_placement_config_t *config, float y, float x1, float r) {
	return (config->K0 * r) - (config->K1 * y) - (config->K2 * x1);
}

int mainsarasa() {
	pole_placement_config_t pole_placement_config;

//	pole_placement_config.Ad = 0.999;
//	pole_placement_config.Bd = 0.099;
//	pole_placement_config.Cd = 0.1;

	//pole_placement_init(&pole_placement_config, 0.9);

//	for (int i = 0; i < N_SAMPLES; i++){
//		R [i] = i < N_SAMPLES/2 ? 0.0 : 1.0;
//		Y_OPEN[i] = 0.0;
//		Y_POLE_PLACEMENT[i] = 0.0;
//		U[i] = 0.0;
//	}

	printf("\n\ry_open_c = [0;" );
	for (int n = 1; n < N_SAMPLES; n++){
		Y_OPEN[n] = NumZ[1] * R[n-1] - DenZ[1] * Y_OPEN[n-1];
		printf("%f;", Y_OPEN[n]);
	}
	printf ("];\n\r");

	printf("\n\ry_pole_placement_c = [0;" );
	for (int n = 1; n < N_SAMPLES; n++){
        // X[n] = ADC
		//U[n] = pole_placement_control(&pole_placement_config, Y_POLE_PLACEMENT[n-1], R[n]);
        // DAC = U[n] 
		Y_POLE_PLACEMENT[n] = NumZ[1] * U[n-1] - DenZ[1] * Y_POLE_PLACEMENT[n-1];
		printf("%f;", Y_POLE_PLACEMENT[n]);
	}
	printf("];\n\r");

	return (0);
}
