//
// freeman.justin@gmail.com


// standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>

// netCDF header
#include <netcdf.h>

// libxml2 headers
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "jutil.h"


// macros
#define	TRUE 1
#define FALSE 0


#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))


#define fail(...) _fail(__LINE__,__func__,__FILE__,__VA_ARGS__)

#define pi  (M_PI)
#define earthradius (6356750.52)
#define deg2rad (M_PI/180.0)
#define rad2deg (180.0/M_PI)



// grid parameters
typedef struct{
    double  lat;        // Latitude  (degrees) of the bottom-left corner of the grid.
    double  lon;        // Longitude (degrees) of the bottom-left corner of the grid. 
    
    int     N;          // Number of vertical levels
    
    int     nX;         // number of X
    int     nY;         // number of Y
    
}grid;

typedef struct{
	
    char    *output_file_name;
    char    *temperature_file_name;
    char    *salinity_file_name;
    grid    g;
     
    double  ****T;  // temperature
    double  ****S;  // salinity
    double  ****c;  // sound speed
    float  ***c_max_depth; // depth to max sound speed

    double *lat;
    double *lon;
    double *depth;
    double *times;

    float fillValue;
    float fillValue_temp;
    float fillValue_salt;
    double fillValue_soundspeed;
    float fillValue_maxDepth;   
 
    // netcdf params
    int ncid;
    int varid;
    int retval;
    int dimIds[NC_MAX_VAR_DIMS];
    int dimIds_max_depth[NC_MAX_VAR_DIMS];
    
    // dimensions
    size_t xt_ocean;
    size_t yt_ocean;
    size_t st_ocean;
    size_t time;
   
    // dimids for output
    int xt_ocean_dimid;
    int yt_ocean_dimid;
    int st_ocean_dimid;
    int time_dimid;
 
    // variable ids
    int vid_sound_speed;
    int vid_max_depth;
    int vid_xt_ocean;
    int vid_yt_ocean;
    int vid_st_ocean;
    int vid_time;
    
}e;




// prototypes
// fail.c
void _fail( const int, const char*, const char*, const char*, ... );

void malloc_arrays( e* );

// netcdf functions
void get_field(e*, char*, void*);
void get_attribute(e*, char*, char*, double*);
void get_dimension(e*, char*, size_t*);
void write_netcdf( e* );
void create_netcdf( e*, char*, int* );
void defdims_netcdf( e* );
void defvars( e* );
void defvar_netcdf(e *E, int, char*, nc_type, int, int*, int*);
void add_txt_attribute_netcdf(e *E, int, int, char*, char* );
void add_double_attribute_netcdf(e *E, int, int, char*, int, double*);
void add_float_attribute_netcdf(e *E, int, int, char*, int, float*);

void add_global_metadata( e*, int );
void write_data( e* );

double soundspeed(double, double, double);
double pressure(double, double);
double convert_temperature(double, double, double, double);

void get_grid_params(e*);

