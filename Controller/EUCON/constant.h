/*---------------------------------------------------------------
Constants used by EUCON
---------------------------------------------------------------*/

#ifndef __CONSTANT__
#define __CONSTANT__

const int FCS_PERIOD = 1000;

//MPC control parameters
const int PREDICTION_HORIZON = 4;
const int CONTROL_HORIZON = 2;
const int T_REF = 4*FCS_PERIOD;
const int T_SAMPLING = FCS_PERIOD;

#endif
