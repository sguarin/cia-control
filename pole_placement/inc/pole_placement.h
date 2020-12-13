/*=====[identification_rls]====================================================
 * Copyright 2019 Diego Fernández <dfernandez202@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2019/09/23
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _POLE_PLACEMENT_H_
#define _POLE_PLACEMENT_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "arm_math.h"
#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

// Order of process to identify
#define Q_ORDER         2

// Order of matrix to calculate identify
#define M_SIZE          (( 2 * Q_ORDER ) + 1)

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

typedef struct {
	float K0;
	float K1;
	float K2;
} pole_placement_config_t;

typedef struct {
	uint32_t ts_ms;

	void (*p_receive) (float *, float *, float *);   // Recibe señal de la planta
	void (*p_transmit) (float *);	        // Transmite señal a la planta

	pole_placement_config_t config;
} pole_placement_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

void pole_placement_init(pole_placement_t *data, uint32_t ts_ms, void (*pfr)(float *, float *, float *), void (*pft)(float *));
void pole_placement_run(pole_placement_t *data);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _POLE_PLACEMENT_H_ */
