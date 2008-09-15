// $Id$

#ifndef common_h
#define common_h

#include "tao/ORB.h"

CORBA::ORB_ptr initialize_orb_and_poa(int & argc, char * argv[]);
void report_exception();

#endif // common_h

