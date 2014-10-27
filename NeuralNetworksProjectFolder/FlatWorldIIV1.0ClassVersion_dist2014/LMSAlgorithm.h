/*
 * LMSAlgorithm.h
 *
 *  Created on: Oct 24, 2014
 *      Author: lin
 */
#include "Perceptron.h"
#include <math.h>

#ifndef LMSALGORITHM_H_
#define LMSALGORITHM_H_

extern double accumulated_rms;
/***********************************************************/
/*  Exportable functions				   */
/***********************************************************/
int LMScalculate(float *inputs, int input_num, int isCal, float expected);

#endif /* LMSALGORITHM_H_ */
