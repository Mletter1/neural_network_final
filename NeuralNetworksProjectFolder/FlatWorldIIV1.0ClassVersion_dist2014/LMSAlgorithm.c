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

int initialized = 0;
perceptron neuron_brain;
double accumulated_rms = 0;

void v_function(double* inputs, struct perceptron* p)
{
	int it = 0;
	p->v = 0;
	for(int it = 0; it < p->input_num; it++)
	{
		p->v += (inputs[it] * p->weights[it]);
	}
}
void y_function(struct perceptron* p)
{
	p->output = p->v;
}

void adjust_function(double* inputs, struct perceptron* p, void *params)
{
	double target =*((double *) params);
	int idx = 0;

	p->error = target - p->output;

	for(idx = 0; idx < p->input_num; idx++)
	{
		p->weights[idx] += (p->error * inputs[idx] * LEARNING_RATE);
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
int calculate(double *inputs, int input_num, int isCal, int type)
{
	int ret = 0;

	if(!initialized)
	{
		init(input_num);
		initialized = 1;
	}

	v_function(inputs, &neuron_brain);
	y_function(&neuron_brain);

	if(neuron_brain->output > 0.5)
		ret = 1;

	if(isCal)
	{
		adjust_function(inputs, &neuron_brain, NULL);
		
		/*Log rms*/
		accumulated_rms += (pow(neuron_brain->error, 2));
	}

	return ret;
}
