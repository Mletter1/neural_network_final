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
	float forwardspeed ;
	float maxvisualreceptordirection ;
	float bodyx, bodyy, bodyth ;
	float x, y, h ;
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
	FILE *fp = 0x0;

	/* Initialize */
	forwardspeed = 0.5;
  
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


		for( k=0 ; k<nsomareceptors ; k++ )
    		{
      			if( (k==0 || k==1 || k==7 ) && skinvalues[k][0]>0.0 )
      			{
        			delta_energy = eat_colliding_object(w, a, k) ;
      				
				if(delta_energy != 0)
				{
					printf("Training the neuron with delta value is %f\n", delta_energy);
					object_num++;	
					/*Train the neuron*/
					LMScalculate(eyevalues[a->instate->eyes[0]->nreceptors/2], a->instate->eyes[0]->nbands, 1, (delta_energy > 0 ? 1.0 : 0.0));
				}
			}
    		}

    						/* read hearing sensors and load spectra for each ear, and compute integrated sound magnitudes */
		read_acoustic_sensor( w, a) ;
		ear0values = extract_sound_receptor_values_pointer( a, 0 ) ;
		ear1values = extract_sound_receptor_values_pointer( a, 1 ) ;
		nacousticfrequencies = get_number_of_acoustic_receptors( a ) ;    
		for( i=0 ; i<nacousticfrequencies ; i++ )
		{
		  ear0mag += ear0values[i][0] ;
		  ear1mag += ear1values[i][0] ;
		}
		//printf("simtime: %d ear0mag: %f ear1mag: %f\n",simtime,ear0mag,ear1mag) ;
    
		/* read visual sensor to get R, G, B intensity values */ 
		read_visual_sensor( w, a) ;
		eyevalues = extract_visual_receptor_values_pointer( a, 0 ) ;
		 
		/* find brights object in visual field */
		maxvisualreceptor = intensity_winner_takes_all( a ) ;

		if( maxvisualreceptor >= 0 ) 
		{
			/* use brightest visual receptor to determine how to turn body to center it in the field of view */
			maxvisualreceptordirection = visual_receptor_position( a->instate->eyes[0], maxvisualreceptor ) ;      
			/* rotate body to face brightes object */
			set_agent_body_angle( a, bodyth + maxvisualreceptordirection ) ;
		}
		else
		{
			printf("agents_controller-  No visible object, simtime: %d, changing direction.\n",simtime) ;
			read_agent_body_position( a, &bodyx, &bodyy, &bodyth ) ;
 			set_agent_body_angle( a, bodyth + 45.0 ) ;
		}

		/* move the agents body */
		set_forward_speed_agent( a, forwardspeed ) ;
		move_body_agent( a ) ;

		/* decrement metabolic charge by basil metabolism rate.  DO NOT REMOVE THIS CALL */
		basal_metabolism_agent( a ) ;
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
		
		/* Example as to how to restore the world and agent after it dies. */
		restore_objects_to_world( Flatworld ) ;  /* restore all of the objects back into the world */
		reset_agent_charge( a ) ;               /* recharge the agent's battery to full */
		a->instate->itemp[0] = 0 ;              /* zero the number of object's eaten accumulator */
		x = distributions_uniform( Flatworld->xmin, Flatworld->xmax ) ; /* pick random starting position and heading */
		y = distributions_uniform( Flatworld->ymin, Flatworld->ymax ) ;
		h = distributions_uniform( -179.0, 179.0) ;
		printf("\nagent_controller- new coordinates after restoration:  x: %f y: %f h: %f\n",x,y,h) ;
		set_agent_body_position( a, x, y, h ) ;    /* set new position and heading of agent */
    
		/* Accumulate lifetime statistices */
		avelifetime += (float)simtime ;
		simtime = 0 ;
		nlifetimes++ ;
		
		if(nlifetimes >= maxnlifetimes )
		{
			/*plot data and clean up data*/
			//Todo: plot data
			sprintf(eye_data_file_name_str, "%s%d_%d_%d.csv", eye_data_file_name, date->tm_hour, date->tm_min, date->tm_sec);
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
			printf("\nAverage lifetime: %f\n",avelifetime) ;
			exit(0) ;
		}
		else
		{
			printf("This is the %dth epoch\n", epoch_num);			
			rmss[epoch_num] = pow(accumulated_rms/object_num, 0.5);
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
