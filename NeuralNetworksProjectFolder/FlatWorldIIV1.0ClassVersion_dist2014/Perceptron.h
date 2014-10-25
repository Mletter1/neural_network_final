/***********************************************************/
/*  Perceptron header file				   */
/***********************************************************/

#ifndef PERCEPTRON
#define PERCEPTRON

typedef struct perceptron
{
	int input_num;
	double *weights;
	double v;
	double output;
	double error;
	int is_inner_neuron;	/*inner neuron has no error, but sigma instead*/
	void *param;	/*Reserved variable for future use*/
}perceptron;

/***********************************************************/
/*  Global variable					   */
/***********************************************************/


/***********************************************************/
/*  Exportable functions				   */
/***********************************************************/
void perceptron_default(perceptron *neuron);

void perceptron_clear(perceptron *neuron);

#endif
