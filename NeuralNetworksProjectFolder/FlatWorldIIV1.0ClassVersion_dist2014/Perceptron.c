
#include "Perceptron.h"

/***********************************************************/
/*  Global variable					   */
/***********************************************************/

/***********************************************************/
/*  Internal variable										   */
/***********************************************************/


/***********************************************************/
/*  Exportable functions				   */
/***********************************************************/
void perceptron_default(perceptron *neuron)
{
	neuron->weights = (double *)malloc(sizeof(double) * (neuron->input_num + 1));
}

void perceptron_clear(perceptron *neuron)
{
	free(neuron->weights);
}

