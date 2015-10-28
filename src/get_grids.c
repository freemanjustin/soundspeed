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

    E->lon = malloc(E->xt_ocean*sizeof(double));
    E->lat = malloc(E->yt_ocean*sizeof(double));
    E->depth = malloc(E->st_ocean*sizeof(double));
    E->times = malloc(E->time*sizeof(double));

    get_field(E, "xt_ocean", &E->lon[0]);
    get_field(E, "yt_ocean", &E->lat[0]);
    get_field(E, "st_ocean", &E->depth[0]);
    get_field(E, "Time", &E->times[0]);   

 
    // read in the _FillValue
    //
    // read in the scale and offset
	

    nc_close(E->ncid);
    // get salinity
    if ((E->retval = nc_open(E->salinity_file_name, NC_NOWRITE, &E->ncid)))
		fail("couldn't open salinity file\n");	
    
    E->S = malloc4d_double(E->time+1, E->st_ocean, E->yt_ocean, E->xt_ocean); 
    get_field(E,"salt", &E->S[0][0][0][0]);

    // malloc room for the sound speed variable
    E->c = malloc4d_double(E->time+1, E->st_ocean, E->yt_ocean, E->xt_ocean);

    // read in scale factor and offsets for temp and salinity
    // read in _FillValue for temp and salinity
    // read in st_ocean (metres)

    nc_close(E->ncid);

}
