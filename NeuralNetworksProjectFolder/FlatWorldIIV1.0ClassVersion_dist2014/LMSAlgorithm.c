/*
 * LMSAlgorithm.c
 *
 *  Created on: Oct 24, 2014
 *      Author: lin
 */
#define OUTPUT_NUM 1
#define LEARNING_RATE 0.01
#define NEURON_NUM 1

#include "LMSAlgorithm.h"
#include "Perceptron.h"

int initialized = 0;
perceptron neuron_brain;
double accumulated_rms = 0;
float forwardspeed;

void v_function(float* inputs, struct perceptron* p)
{
	int it = 0;
	p->v = 0;

	p->v += p->weights[0];/*Bias*/
	for(it = 0; it < p->input_num; it++)
	{
		p->v += (inputs[it] * p->weights[it+1]);
	}
}

void y_function(struct perceptron* p)
{
	p->output = p->v;
}

void adjust_function(float* inputs, struct perceptron* p, void *params)
{
	double target =*((double *) params);
	int idx = 0;

	p->error = target - p->output;

 	p->weights[0] += (p->error * 1 * LEARNING_RATE);/*Bias*/
	for(idx = 0; idx < p->input_num; idx++)
	{
		p->weights[idx+1] += (p->error * inputs[idx] * LEARNING_RATE);
	}
}

void initialize(int input_num)
{
	int idx = 0;
	neuron_brain.input_num = input_num;
	perceptron_default(&neuron_brain);

    /*This part uses the old data*/
    neuron_brain.weights[0] = 0.0364013;
    neuron_brain.weights[1] = -0.462995;
    neuron_brain.weights[2] = 1.26562;
    neuron_brain.weights[3] = -0.833772;

//    /*This part is used to train the neuron*/
//	for(idx = 0; idx <= input_num; idx++)
//	{
//		neuron_brain.weights[idx] = 0.1 * (rand()%10);
//	}
}


/***********************************************************/
/*  Exportable functions				   */
/***********************************************************/
int LMScalculate(float *original_inputs, int input_num, int isCal, float expected)
{
	int ret = 0;
	double e = (double)expected;
    int it = 0;
    float sum = 0;
    float *inputs = (float*)malloc(input_num * sizeof(float));
    memcpy(inputs, original_inputs, sizeof(float) * ( input_num));
    
    for(it = 0; it < input_num; it++)
    {
        sum += inputs[it];
    }
    
    for(it = 0; it < input_num; it++)
    {
        inputs[it] /= sum;
    }
    
	if(!initialized)
	{
		initialize(input_num);
		initialized = 1;
	}

	v_function(inputs, &neuron_brain);
	y_function(&neuron_brain);

	if(neuron_brain.output > 0)
		ret = 1;

	if(isCal)
	{
		adjust_function(inputs, &neuron_brain, &e);
		
		/*Log rms*/
		accumulated_rms += (pow(neuron_brain.error, 2));
	}
	else
	{

	}

	return ret;
}



void reset()
{
	accumulated_rms = 0;
}
