// freeman.justin@gmail/.com

#include "soundspeed.h"


void get_grid_params(e *E){

    // open the mom4 temperature file and read the dimensions info
   if ((E->retval = nc_open(E->temperature_file_name, NC_NOWRITE, &E->ncid)))
		fail("couldn't open temperature file\n");	

    get_dimension(E, "xt_ocean", &E->xt_ocean);
    get_dimension(E, "yt_ocean", &E->yt_ocean);
    get_dimension(E, "st_ocean", &E->st_ocean);
    get_dimension(E, "Time", &E->time);

    E->T = malloc4d_double(E->time+1, E->st_ocean, E->yt_ocean, E->xt_ocean); 
    get_field(E,"temp", &E->T[0][0][0][0]);

    nc_close(E->ncid);
    // get salinity
    if ((E->retval = nc_open(E->salinity_file_name, NC_NOWRITE, &E->ncid)))
		fail("couldn't open salinity file\n");	
    
    E->S = malloc4d_double(E->time+1, E->st_ocean, E->yt_ocean, E->xt_ocean); 
    get_field(E,"salt", &E->S[0][0][0][0]);

    // malloc room for the sound speed variable
    E->c = malloc4d_double(E->time+1, E->st_ocean, E->yt_ocean, E->xt_ocean);

}
