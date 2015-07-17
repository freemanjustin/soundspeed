// freeman.justin@gmail.com

#include "soundspeed.h"

void get_field(e *E, char *field_name, void* field){

    if(( E->retval = nc_inq_varid(E->ncid, field_name, &E->varid)))
        fail("inq_varid failed: %d\n", E->retval);

    if ((E->retval = nc_get_var_double(E->ncid, E->varid, (double*)field)))
        fail("get var_double failed: %d\n",E->retval);

}

void get_attribute(e *E, char *var, char *att_name, double *att){

    if(( E->retval = nc_inq_varid(E->ncid, var, &E->varid)))
        fail("inq_varid failed: %d\n", E->retval);

    if((E->retval = nc_get_att_double(E->ncid, E->varid, att_name, att)))
        fail("get_att_double failed: %d\n", E->retval);

}

void get_dimension(e *E, char *dim_name, size_t *dim){


    if((E->retval = nc_inq_dimid(E->ncid, dim_name, &E->varid)))
        fail("get_dimid failed: %d\n", E->retval);
    if((E->retval = nc_inq_dimlen(E->ncid,E->varid,dim)))
        fail("inq_dimlen failed: %d\n", E->retval);
}


void write_netcdf(e *E){

	//float	fillValue = -1e34;
	float fillValue = NC_FILL_FLOAT;
	
    
	create_netcdf(E, E->output_file_name, &E->ncid);
    
    defdims_netcdf(E);
	
	// setup variables
	defvars(E);
    
    	
	/*
	// add global metadata
	add_global_metadata(E, E->ncid);
	*/
    
    
    // write the data to the netcdf file
    write_data(E);
    
    nc_close(E->ncid);
}


void create_netcdf(e *E, char *fname, int *ncid){
	
	//int	old_fill_mode;
    
	if ( (E->retval = nc_create(fname, NC_CLOBBER, ncid) ) )
		fail("couldn't create netcdf out file %s\n",fname);
    
	// set the fill mode to be NO_FILL 
	//nc_set_fill(*ncid, NC_NOFILL, &old_fill_mode); 
	
}

void defdims_netcdf(e *E){
	
/*	
	// define the dimensions
	if ((E->retval = nc_def_dim(E->ncid, "xi_rho", E->g.nX, &E->xi_rho_dimid)))
		fail("nc_def_dim failed!\n");
	
	if ((E->retval = nc_def_dim(E->ncid, "xi_u", E->g.nX-1, &E->xi_u_dimid)))
		fail("nc_def_dim failed!\n");
    
    if ((E->retval = nc_def_dim(E->ncid, "xi_v", E->g.nX, &E->xi_v_dimid)))
		fail("nc_def_dim failed!\n");
	
    if ((E->retval = nc_def_dim(E->ncid, "xi_psi", E->g.nX-1, &E->xi_psi_dimid)))
		fail("nc_def_dim failed!\n");
	
    if ((E->retval = nc_def_dim(E->ncid, "eta_rho", E->g.nY, &E->eta_rho_dimid)))
		fail("nc_def_dim failed!\n");
	// end define mode for this file
	if ((E->retval = nc_enddef(E->ncid)))
      	fail("nc_enddef failed\n");
    
*/	
}


void defvars(e *E){
/*	
    // setup dimids
    
    E->dimIdsOne[0] = E->one_dimid;
    
    E->dimIdsRho[0] = E->eta_rho_dimid;
    E->dimIdsRho[1] = E->xi_rho_dimid;
    
    E->dimIdsPsi[0] = E->eta_psi_dimid;
    E->dimIdsPsi[1] = E->xi_psi_dimid;
    
    E->dimIdsU[0] = E->eta_u_dimid;
    E->dimIdsU[1] = E->xi_u_dimid;
    
    E->dimIdsV[0] = E->eta_v_dimid;
    E->dimIdsV[1] = E->xi_v_dimid;
    
    
	//float	fillValue = -1e34;
    defvar_netcdf(E, E->ncid, "angle", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_angle);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "long_name", "angle between xi axis and east");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "units", "radian");
	
	defvar_netcdf(E, E->ncid, "dmde", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_dmde);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "long_name", "eta derivative of inverse metric factor pm");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "units", "m");
    
*/
    
}



void defvar_netcdf(e *E, int ncid, char *var_name, nc_type type, int dims, int *dimIds, int *varid){
	
	
	if((E->retval = nc_redef(ncid)))
		fail("nc_redef failed\n");
    
	if ((E->retval = nc_def_var(ncid, var_name, type, dims, dimIds, varid)))
		fail("nc_def_var failed\n");
	
	if ((E->retval = nc_enddef(ncid)))
		fail("nc_enddef failed\n");
	
}


void add_txt_attribute_netcdf(e *E, int ncid, int varid, char* att_name, char* att_value){
    
	if((E->retval = nc_redef(ncid)))
		fail("nc_redef failed\n");
    
	if(( E->retval = nc_put_att_text(ncid, varid, att_name, strlen(att_value), att_value)))
		fail("nc_put_att_txt failed\n");
	
	if ((E->retval = nc_enddef(ncid)))
		fail("nc_enddef failed\n");
}


void add_global_metadata(e *E, int ncid){
    
    // TODO!
}


void write_data(e *E){
    
   /* 
    // write angle
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_angle, &E->angle[0][0])))
        fail("put_var_ failed for angle. Error code = %d\n",E->retval);
    
    // write dmde
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_dmde, &E->dmde[0][0])))
            fail("put_var_ failed for dmdw. Error code = %d\n",E->retval);   
    // write dndx
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_dndx, &E->dndx[0][0])))
        fail("put_var_ failed for dndx. Error code = %d\n",E->retval);
    */

}
