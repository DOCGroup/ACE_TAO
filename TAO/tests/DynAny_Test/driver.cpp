// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    driver.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================



#include "DynAny_i.h"
#include "DynStruct_i.h"
#include "DynSequence_i.h"
#include "DynArray_i.h"
#include "DynEnum_i.h"
#include "da_testsC.cpp"

int main (int argc, char* argv[])
{
  CORBA::Environment env;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);

 
  return 0;
}