#include "LMSAlgorithm.h"
#include "Perceptron.h"

typedef struct object
{
	float *inputs;
	int input_num;
	float output;
	struct object *next;
}object;

typedef struct object_list
{
	object *header;
	object *cur;
	int object_num;
}object_list;

static object_list list;
int epoch_num = 0;
double rmss[10000];
int object_num = 0;
float forwardspeed = 0.1;
float init_x;
float init_y;
float init_head_position = 0;

/*Unexported functions*/
object *addObject(float *input, int input_num, float output);
void clearObjects();

void agents_controller( WORLD_TYPE *w )
{ /* Adhoc function to test agents, to be replaced with NN controller. tpc */
	
	AGENT_TYPE *a ;
	int collision_flag=0 ;
	int i,k ;
	int maxvisualreceptor = -1 ;
	int nsomareceptors ;
	int nacousticfrequencies ;
	float delta_energy, old_delta_energy;
	float dfb , drl, dth, dh ;
	float headth ;
	//float forwardspeed ;
	float maxvisualreceptordirection ;
	float bodyx, bodyy, bodyth ;
	float **eyevalues, **ear0values, **ear1values, **skinvalues ;
	float ear0mag=0.0, ear1mag=0.0 ;
	time_t now ;
	struct tm *date ;
	char timestamp[30] ;
	int is_poisonous = 0;
	//temporary file
	char eye_data_file_name[20] = "./dat/eye_data_";
	char eye_data_file_name_str[50] = "";
	int idx = 0;
    float stopping_criteria = 0.00000001;
	FILE *fp = 0x0;
    int ret = 0;
 
 	a = w->agents[0] ; /* get agent pointer */
	
	/* test if agent is alive. if so, process sensors and actuators.  if not, report death and 
		reset agent & world */
	if( a->instate->metabolic_charge > 0.0)
	{
		/* get current motor rates and body/head angles */
		read_actuators_agent(a, &dfb, &drl, &dth, &dh ) ;
		read_agent_body_position( a, &bodyx, &bodyy, &bodyth ) ;
		read_agent_head_angle( a, &headth );
				
		/* read somatic(touch) sensor for collision */  
		collision_flag = read_soma_sensor(w, a) ; 	
    		skinvalues = extract_soma_receptor_values_pointer( a ) ;
    		nsomareceptors = get_number_of_soma_receptors( a ) ;
    	
		/* read visual sensor to get R, G, B intensity values to collect x values*/ 
		read_visual_sensor( w, a) ;
		eyevalues = extract_visual_receptor_values_pointer( a, 0 ) ;

		for(k = 0 ; k < nsomareceptors ; k++)
        {
            if((k == 0 || k == 1 || k ==7 ) && skinvalues[k][0] > 0.0)
            {
                delta_energy = eat_colliding_object(w, a, k) ;

                printf("Training the neuron with delta value is %f\n", delta_energy);
                object_num++;
                
                /*Train the neuron*/
                LMScalculate(eyevalues[a->instate->eyes[0]->nreceptors/2], a->instate->eyes[0]->nbands, 1, (delta_energy > 0 ? 1.0 : 0.0));
			}
        }
        
        /*Move robot*/
        ret = set_direction(w, a, 0);
		 
		/* find brights object in visual field using other method */
		maxvisualreceptor = intensity_winner_takes_all( a ) ;

        printf("Compare result %d with %d.... \n", ret, maxvisualreceptor);
        
		/* move the agents body */
		set_forward_speed_agent(a, forwardspeed) ;
		move_body_agent(a) ;

		/* decrement metabolic charge by basil metabolism rate.  DO NOT REMOVE THIS CALL */
		basal_metabolism_agent(a) ;
		simtime++ ;
	} /* end agent alive condition */
	else
	{
		/* Example of agent is dead condition */
		printf("agent_controller- Agent has died, eating %d objects. simtime: %d\n",a->instate->itemp[0], simtime ) ;
		now = time(NULL) ;
		date = localtime( &now ) ;
		strftime(timestamp, 30, "%y/%m/%d H: %H M: %M S: %S",date) ;
		printf("Death time: %s\n",timestamp) ;
		sprintf(eye_data_file_name_str, "./dat/timevsspeed.csv");
		if((fp = fopen(eye_data_file_name_str, "a+")) != 0x0)
		{
		
			fprintf(fp, "%f,%f,%d\n", forwardspeed, init_head_position, simtime);
			fclose(fp);
		}

		/* Example as to how to restore the world and agent after it dies. */
		restore_objects_to_world( Flatworld ) ;  /* restore all of the objects back into the world */
		reset_agent_charge( a ) ;               /* recharge the agent's battery to full */
		a->instate->itemp[0] = 0 ;              /* zero the number of object's eaten accumulator */
		
        /* keep starting position the same and change head angle */
        
        init_head_position += 5;
        
        init_x = (Flatworld->xmax + Flatworld->xmin)/2;
        init_y = (Flatworld->ymax + Flatworld->ymin)/2;
		
		printf("\nagent_controller- new coordinates after restoration:  x: %f y: %f h: %f\n", init_x, init_y, init_head_position) ;
		set_agent_body_position(a, init_x, init_y, init_head_position/360) ;
        
		/* Accumulate lifetime statistices */
		avelifetime += (float)simtime ;
		simtime = 0 ;
		nlifetimes++ ;
				
		if(nlifetimes >= maxnlifetimes || (epoch_num > 10 && fabs(rmss[epoch_num - 1] - rmss[epoch_num - 2]) < stopping_criteria))   /*Add stopping condition for the neuron training to stop*/
		{
            /*plot data and clean up data*/
            sprintf(eye_data_file_name_str, "%sdata.csv", eye_data_file_name);
            printf("store the data and exit with epoch %d\n", epoch_num);
            
            if((fp = fopen(eye_data_file_name_str, "w+")) != 0x0)
            {
                /*Log rms*/
                for(idx = 0; idx < epoch_num; idx++)
                {
                    fprintf(fp, "%d,%lg\n", idx+1, rmss[idx]);
                }
                
                fprintf(fp, "\nThe final weights are:\n");
                
                /*Log weights*/
                for(idx = 0; idx <= neuron_brain.input_num; idx++)
                {
                    fprintf(fp, "%lg,", neuron_brain.weights[idx]);
                }
                
                fclose(fp);
            }
            epoch_num = 0;
            
			avelifetime /= (float)maxnlifetimes ;
			printf("\nAverage lifetime: %f\n",avelifetime);
			exit(0) ;
		}
		else
		{
			rmss[epoch_num] = pow(accumulated_rms/object_num, 0.5);
            printf("This is the %dth epoch with %f random speed with %f rms\n", epoch_num, forwardspeed, rmss[epoch_num]);
			epoch_num++;
			accumulated_rms = 0;
			object_num = 0;
		}
	} /* end agent dead condition */
}/*end agents_controller()*/

object *addObject(float *inputs, int input_num, float output)
{
	object *new_object = (object *)malloc(sizeof(object));
	new_object->inputs = inputs;
	new_object->input_num = input_num;
	new_object->output = output;
	
	list.object_num++;
	list.cur->next = new_object;
	list.cur = list.cur->next;
	list.cur->next = NULL;

	return new_object;
}

void clearObjects()
{
	object *o1;
	object *o2 = list.header->next;
	list.header->next = NULL;
	list.object_num = 0;

	while(o2)
	{
		o1 = o2;
		o2 = o2->next;
		free(o1);
	}
}
