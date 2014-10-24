/***********************************************************/
/*  Perceptron header file				   */
/***********************************************************/

#ifndef PERCEPTRON
#define PERCEPTRON

typedef struct perceptron
{
	int input_num;
	int v;
	int output;
	int error;
	int sigma;	/*Reserved variable for multi-layer back propagation*/
	void *param;	/*Reserved variable for future use*/
}perceptron;

/***********************************************************/
/*  Global variable					   */
/***********************************************************/


/***********************************************************/
/*  Exportable functions				   */
/***********************************************************/

#endif
