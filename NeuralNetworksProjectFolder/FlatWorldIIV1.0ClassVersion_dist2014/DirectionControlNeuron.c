//
//  DirectionControlNeuron.c
//  FlatWorldIIV1.0ClassVersion_dist2014
//
//  Created by lin sun on 11/22/14.
//  Copyright (c) 2014 lin sun. All rights reserved.
//

int set_direction(WORLD_TYPE *world, AGENT_TYPE *agent, int eye_idx)
{
    VISUAL_SENSOR_TYPE **eyes = agent->instate->eyes;
    int num_receptors;
    int num_bands;
    int receptor_idx = 0;
    int band_idx = 0;
    int max_receptor = -1 ;
    float intensity = 0;
    float maxintensity = 0;
    float bodyx = 0;
    float bodyy = 0;
    float bodyh = 0;
    int ret = 0;
    
    num_receptors = eyes[0]->nreceptors ;
    num_bands = eyes[0]->nbands ;
    
    read_visual_sensor(world, agent) ;
    extract_visual_receptor_values_pointer(agent, 0) ;
    
    for(receptor_idx = 0; receptor_idx < num_receptors; receptor_idx++)
    {
        intensity = 0 ;
        
        for(band_idx = 0; band_idx < num_bands; band_idx++)
            intensity += eyes[0]->values[receptor_idx][band_idx] ;
        //ret = LMScalculate(eyes[0]->values[receptor_idx], a->instate->eyes[0]->nbands, 0, 0);
        if(intensity > maxintensity)// && ret == 1)
        {
            max_receptor = receptor_idx;
            maxintensity = intensity ;
        }
    }
    
    return max_receptor;
}