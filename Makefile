###################################################################
#
# j.freeman@bom.gov.au 
#
##################################################################

CC=	gcc

CSRC=	./src/

CFLAGS=	-O3 -g -Wall `xml2-config --cflags`
#CFLAGS=	-O3 -g -fPIC -Wall

INC=	-I./include 

LFLAGS= -lnetcdf

COBJ=	$(CSRC)main.o \
	$(CSRC)jutil.o \
	$(CSRC)fail.o \
	$(CSRC)soundspeed.o \
	$(CSRC)get_grids.o \
	$(CSRC)netcdfIO.o 
	

OBJ=	$(COBJ) 

EXEC=	./bin/ss

$(EXEC):$(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LFLAGS)

$(COBJ) : %.o : %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	rm $(COBJ)
	rm $(EXEC)
