// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    any_test_i.cpp
//
// = AUTHOR
//    Michael Kircher 
//
// ============================================================================

#include "tao/corba.h"
#include "ace/Containers.h"
#include "any_test_i.h"
#include "NavWeapC.h"
#include "AnyAnalyser.h"


// Constructor
Any_Test_i::Any_Test_i (const char *obj_name)
{
}

// Destructor
Any_Test_i::~Any_Test_i (void) 
{ 
}


void 
Any_Test_i::try_an_any (const CORBA::Any &a,  CORBA::Environment &env)
{
  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "Call to try_an_any succeeded!\n"));
      
      AnyAnalyser anyAnalyser_ = AnyAnalyser ("stdout");

      TAO_CHECK_ENV;
      
      anyAnalyser_.printAny (a);      
    }
  TAO_CATCHANY    
    {
      ACE_ERROR ((LM_ERROR, "(%t)Error in extracting the data.\n"));
    }
  TAO_ENDTRY;	    
}


