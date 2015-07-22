// freeman.justin@gmail.com


#include "soundspeed.h"

int main(int argc,char **argv)
{
	e	*E;
    
	// malloc the work struct
	E = malloc(sizeof(e));
	if(E==NULL) fail("Malloc failed\n");
	
	// parse command line arguments
	if(argc < 3){
		fail("need an input xml file and an output filename\n");
	}
	else{
		get_command_line_arg_as_string(&E->temperature_file_name, argv[1]);
		get_command_line_arg_as_string(&E->salinity_file_name, argv[2]);
		get_command_line_arg_as_string(&E->output_file_name, argv[3]);
	}
	

    // read the input temperature file to get the grid parameters
    //get_grid_params(E);   

    // calculate sound speed for all time and spatial levels
    //soundspeed(E); 
    printf("check val sound speed is = %f m/s\n", soundspeed(40.0, 40.0, 10000.0) );
    printf("check depth to pressure: (d = 1 km at 0 degrees latitude) = %f MPa\n", pressure(11.0, -11.0) );
    printf("convert potential temp to insitu: %f C (should be 20.0)\n", convert_temperature(19.0, 35.0, 0.0,5000.0) );
     printf("convert insitu to potential: %f C (should be 19.0)\n", convert_temperature(20.0, 35.0, 5000.0,0.0) );


    // write the sound speed output file
    //write_netcdf(E);
    
    return 0;
}
    
