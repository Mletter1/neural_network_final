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
	int is_inner_neuron;	/*inner neuron has no error, but sigma instead*/
	void (*v_function)(double*, perceptron*);
	void (*y_function)(perceptron*);
	void (*adjust_function)(double*, perceptron*, void*);
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
