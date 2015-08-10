// freeman.justin@gmail.com


#include "soundspeed.h"

int main(int argc,char **argv)
{
	e	*E;

        int	t,i,j,k;   
	double	P0, Tpot;

 
	// malloc the work struct
	E = malloc(sizeof(e));
	if(E==NULL) fail("Malloc failed\n");
	
	// parse command line arguments
	if(argc < 3){
		fail("ss temperature_file.nc salt_file.nc output[sound_speed]_file.nc\n");
	}
	else{
		get_command_line_arg_as_string(&E->temperature_file_name, argv[1]);
		get_command_line_arg_as_string(&E->salinity_file_name, argv[2]);
		get_command_line_arg_as_string(&E->output_file_name, argv[3]);
	}
	

    // read the input temperature file to get the grid parameters
    get_grid_params(E);   

    // calculate sound speed for all time and spatial levels
   
     // malloc room for the sound speed array
     E->c = malloc4d_double(E->time+1, E->st_ocean, E->yt_ocean, E->xt_ocean);
    
     for(t=0;t<E->time;t++){
	for(k=0;k<E->st_ocean;k++){
	    for(i=0;i<E->yt_ocean;i++){
		for(j=0;j<E->xt_ocean;j++){
			// convert depth to pressure
			// we need the depth in km and the latitude of the location
			// the pressure is returned in MPa
			P0 = pressure(E->depth[k], E->lat[j]);
			// convert potential temperature to in-situ temperature
			// this function wants the reference pressure (P0) in decibars
			// 1 Mpa = 100 deci bar
		        Tpot = convert_temperature(E->T[t][k][i][j], E->S[t][k][i][j], 0.0, P0*100.0);	
			// calculate soud speed
			E->c[t][k][i][j] = soundspeed(Tpot, E->S[t][k][i][j], P0);

		}
	    }
   	}
    }

    #ifdef CHECK 
        printf("check val sound speed is = %f m/s\n", soundspeed(40.0, 40.0, 10000.0) );
        printf("check depth to pressure: (d = 1 km at 0 degrees latitude) = %f MPa\n", pressure(11.0*1000.0, -11.0) );
        printf("convert potential temp to insitu: %f C (should be 20.0)\n", convert_temperature(19.0, 35.0, 0.0,5000.0) );
        printf("convert insitu to potential: %f C (should be 19.0)\n", convert_temperature(20.0, 35.0, 5000.0,0.0) );
    #endif
    // convert depth (m) to pressure ( units??? )
    // convert T,S to sound speed

    // write the sound speed output file
    write_netcdf(E);
    
    return 0;
}
    
