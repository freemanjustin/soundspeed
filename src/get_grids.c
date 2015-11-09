// freeman.justin@gmail/.com

#include "soundspeed.h"


void get_grid_params(e *E){

    int      t,i,j,k;

    int	     temp_id;
    int      salt_id;
    int      status = 0;
    float    scale = 1.0;
    float    offset = 0.0;

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
    nc_inq_varid (E->ncid, "temp", &temp_id);
    nc_get_att_float(E->ncid, temp_id, "_FillValue", &E->fillValue_temp);
    //printf("fillValue = %f\n", E->fillValue);
    // read in the scale and offset
    status = nc_get_att_float(E->ncid, temp_id, "scale_factor", &scale);
    if(status != NC_NOERR) scale = 1.0;
    status = nc_get_att_float(E->ncid, temp_id, "add_offset", &offset);
    if(status != NC_NOERR) offset = 0.0;

    //printf("scale = %f\noffset = %f\n", scale, offset);	
    // apply scale and offset to fillvalue
    if(status == NC_NOERR) E->fillValue_temp = E->fillValue_temp*scale + offset;
 
    // unpack the data
    for(t=0;t<E->time;t++){
        for(i=0;i<E->st_ocean;i++){
            for(j=0;j<E->yt_ocean;j++){
                for(k=0;k<E->xt_ocean;k++){
                    E->T[t][i][j][k] = E->T[t][i][j][k]*scale + offset;
                }
            }
         }
     }

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


    // read in the _FillValue
    nc_inq_varid (E->ncid, "salt", &salt_id);
    nc_get_att_float(E->ncid, salt_id, "_FillValue", &E->fillValue_salt);
    //printf("fillValue = %f\n", E->fillValue);
    // read in the scale and offset
    status = nc_get_att_float(E->ncid, salt_id, "scale_factor", &scale);
    if(status != NC_NOERR) scale = 1.0;
    status = nc_get_att_float(E->ncid, salt_id, "add_offset", &offset);
    if(status != NC_NOERR) offset = 0.0;
    //printf("scale = %f\noffset = %f\n", scale, offset);
    
    // apply scale and offset to fillvalue
    if(status == NC_NOERR) E->fillValue_salt = E->fillValue_salt*scale + offset;
    nc_close(E->ncid);
    
    
    // unpack the data
    for(t=0;t<E->time;t++){
        for(i=0;i<E->st_ocean;i++){
            for(j=0;j<E->yt_ocean;j++){
                for(k=0;k<E->xt_ocean;k++){
                    E->S[t][i][j][k] = E->S[t][i][j][k]*scale + offset;
                }
            }
         }
     }

    nc_close(E->ncid);

}
