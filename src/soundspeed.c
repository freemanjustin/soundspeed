// freeman.justin@gmail.com

#include "soundspeed.h"

double soundspeed(double T, double S, double P0){

  // SOUND SPEED SEAWATER CHEN AND MILLERO 1977, JASA, 62, 1129-1135
  // ----------------------------------------------------------
  // UNITS:
  //   PRESSURE     P0    DECIBARS
  //   TEMPERATURE  T     DEG CELSIUS (IPTS-68)
  //   SALINITY     S     (PSS-78)
  //   SOUND SPEED  SVEL  METERS/SECOND
  // ----------------------------------------------------------
  // CHECKVALUE:
  //   SVEL=1731.995 M/S for S=40 (PSS-78), T=40 DEG C, P=10000 DBAR
  // ----------------------------------------------------------
  // Original fortran code is found in:
  //   UNESCO technical papers in marine science 44 (1983) -
  //   'Algorithms for computation of fundamental properties of seawater'
  // ----------------------------------------------------------

    double  P, SR, D;
    double  B, B0, B1;
    double  A, A0, A1, A2, A3;
    double  C, C0, C1, C2, C3;

    // convert pressur to bars
    P  = P0 / 10;
    SR = sqrt(fabs(S)); // why is this fabs on salinity???

    // S**2 TERM
    D = 1.727e-3 - 7.9836e-6 * P;

    // S**3/2 TERM
    B1 =  7.3637e-5 + 1.7945e-7 * T;
    B0 = -1.922e-2  - 4.42e-5   * T;
    B  = B0 + B1 * P;

    // S**1 TERM
    A3 = (-3.389e-13    * T + 6.649e-12)  * T + 1.100e-10;
    A2 = ((7.988e-12    * T - 1.6002e-10) * T + 9.1041e-9) * T - 3.9064e-7;
    A1 = (((-2.0122e-10 * T + 1.0507e-8)  * T - 6.4885e-8) * T - 1.2580e-5) * T + 9.4742e-5;
    A0 = (((-3.21e-8    * T + 2.006e-6)   * T + 7.164e-5)  * T - 1.262e-2)  * T + 1.389;
    A  = ((A3 * P + A2) * P + A1) * P + A0;

    // S**0 TERM
    C3 = (-2.3643e-12   * T + 3.8504e-10) * T - 9.7729e-9;
    C2 = (((1.0405e-12  * T - 2.5335e-10) * T + 2.5974e-8) * T - 1.7107e-6)  * T + 3.1260e-5;
    C1 = (((-6.1185e-10 * T + 1.3621e-7)  * T - 8.1788e-6) * T + 6.8982e-4)  * T + 0.153563;
    C0 = ((((3.1464e-9  * T - 1.47800e-6) * T + 3.3420e-4) * T - 5.80852e-2) * T + 5.03711) * T + 1402.388;
    C  = ((C3 * P + C2) * P + C1) * P + C0;

    // calculate spund speed
    return ( C + (A + B * SR + D * S) * S);
}



double pressure(double z, double lat){

    // return pressure from depth assuming the ocean is an ideal medium with 
    // temperature 0 and salinity of 35 parts per thousand

    // double z is the depth in metres
    // double lat is the latitude in degrees
    
    double  depth, latitude;
    double  g, k, h45, h;

    depth = z ; 
    latitude = lat*M_PI/180.0; // convert degrees to radians

    g = 9.7803*(1.0 + 5.3e-3 * pow(sin(latitude),2.0));
    k = (g - 2e-5 * depth)/(9.80612 - 2e-5 * depth);
    h45 = 1.00818e-2 * depth + 2.465e-8 * pow(depth,2.0) - 1.25e-13 * pow(depth,3.0) + 2.8e-19*pow(depth,4.0);
    h = h45 * k;

    // h is the pressure in MPa
    return (h);
}

// calculate in-situ temperature from potential temperature
// reference:
// http://www.ncl.ucar.edu/Support/talk_archives/2011/att-1609/potmp.f
// https://bb.cgd.ucar.edu/node/1002100
// http://www.pmel.noaa.gov/maillists/tmap/ferret_users/fu_2009/msg00251.html

double convert_temperature(double TEMP, double SALT, double PRESS, double RP){
//       POTMP  -- CALCULATE POTENTIAL TEMPERATURE FOR AN ARBITRARY
//                 REFERENCE PRESSURE
//     PURPOSE:
//    *******
//
//       TO CALCULATE POTENTIAL TEMPERATURE
//
//       REF: N.P. FOFONOFF
//            DEEP SEA RESEARCH
//            IN PRESS NOV 1976
//
//     PARAMETERS:
//     **********
//
//      PRESS  -> PRESSURE IN DECIBARS
//       TEMP   -> TEMPERATURE IN CELSIUS DEGREES
//       S      -> SALINITY PSS 78
//       RP     -> REFERENCE PRESSURE IN DECIBARS
//                 (0.0 FOR THE QUANTITY THETA)
//       POTEMP <- POTENTIAL TEMPERATURE (DEG C)

    int i,j,N;
    double DP,P,Q,R1,R2,R3,R4,R5,S1,T,X;

    S1 = SALT-35.0;
    P  = PRESS;
    T  = TEMP;

    DP = RP - P;
    N  = (fabs(DP)/1000.0) + 1;
    DP = DP/N;

    for(i=0;i<N;i++){ 
        for(j=0;j<4;j++){ 
            R1 = ((-2.1687E-16*T+1.8676E-14)*T-4.6206E-13)*P;
            R2 = (2.7759E-12*T-1.1351E-10)*S1;
            R3 = ((-5.4481E-14*T+8.733E-12)*T-6.7795E-10)*T;
            R4 = (R1+(R2+R3+1.8741E-8))*P+(-4.2393E-8*T+1.8932E-6)*S1;
            R5 = R4+((6.6228E-10*T-6.836E-8)*T+8.5258E-6)*T+3.5803E-5;

            X  = DP*R5;

            if(j==0){ 
                T = T + 0.5*X;
                Q = X;
                P = P + 0.5*DP;
            }
            else if(j==1){ 
                T = T + 0.29298322*(X-Q);
                Q = 0.58578644*X + 0.121320344*Q;
            }
            else if(j==2){ 
                T = T + 1.707106781*(X-Q);
                Q = 3.414213562*X - 4.121320344*Q;
                P = P + 0.5*DP;
            }
            else
                T = T + (X-2.0*Q)/6.0;
        }
    }

    return (T);

}

