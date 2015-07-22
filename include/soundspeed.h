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
    
    // netcdf params
    int ncid;
    int varid;
    int retval;
    int dimIdsSoundSpeed[NC_MAX_VAR_DIMS];
    
    // dimensions
    size_t xt_ocean;
    size_t yt_ocean;
    size_t st_ocean;
    size_t time;
   

 
    // variable ids
    int vid_angle;
    int vid_dmde;
    int vid_dndx;
    int vid_el;
    int vid_f;
    int vid_h;
    int vid_lat_rho;
    int vid_lat_psi;
    int vid_lat_u;
    int vid_lat_v;
    
    int vid_lon_rho;
    int vid_lon_psi;
    int vid_lon_u;
    int vid_lon_v;
    
    int vid_mask_rho;
    int vid_mask_psi;
    int vid_mask_u;
    int vid_mask_v;
    
    int vid_pm;
    int vid_pn;
    int vid_spherical;
    int vid_xl;
    int vid_X;
    int vid_Y;
    int vid_dx;
    int vid_dy;
    
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

void add_global_metadata( e*, int );
void write_data( e* );

double soundspeed(double, double, double);
double pressure(double, double);
double convert_temperature(double, double, double, double);

