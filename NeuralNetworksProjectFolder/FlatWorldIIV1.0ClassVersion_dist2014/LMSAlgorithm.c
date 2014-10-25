/*
 * LMSAlgorithm.c
 *
 *  Created on: Oct 24, 2014
 *      Author: lin
 */
#include "LMSAlgorithm.h"

#define OUTPUT_NUM 1
#define LEARNING_RATE 0.01;
#define NEURON_NUM 1

volatile int initialized = 0;
perceptron neuron_brain;

void v_function(double* inputs, perceptron* p)
{
	int it = 0;
	p->v = 0;
	for(int it = 0; it < p->input_num; it++)
	{
		p->v += (inputs[it] * p->weights[it]);
	}
}
void y_function(perceptron* p)
{
	p->output = p->v;
}

void adjust_function(double* inputs, perceptron* p, void *params)
{
	double error = 0;
	double target =*((double *) params);
	int idx = 0;

	error = target - p->output;

	for(idx = 0; idx < p->input_num; idx++)
	{
		p->weights[idx] += (error * inputs[idx] * LEARNING_RATE);
	}
}

void init(int input_num)
{
	int idx = 0;
	neuron_brain->input_num = input_num;
	perceptron_default(&neuron_brain);

	for(idx = 0; idx < input_num; idx++)
	{
		neuron_brain->weights[idx] = 0.1 * (rand()%10);
	}
}


/***********************************************************/
/*  Exportable functions				   */
/***********************************************************/
