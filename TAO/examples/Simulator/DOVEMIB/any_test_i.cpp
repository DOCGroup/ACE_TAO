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

ACE_RCSID(DOVEMIB, any_test_i, "$Id$")

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
        Weapons *weapons_ = 0; 
      if (a.type()->equal(_tc_Weapons,TAO_TRY_ENV)) {
      
        CORBA::Any b;
        b = a;
        weapons_ = (Weapons *)b.value();        

        ACE_DEBUG ((LM_DEBUG, 
			        "ID: %d\n" "CompTime: %d\n"
					"Adr number of weapons: %d\n"
					"Adr CompTime: %d\n", 
				    a.type()->id(TAO_TRY_ENV),
                    weapons_->computation_time,
                    (u_long)(void *) &(weapons_->number_of_weapons),
                    (u_long)(void *) &(weapons_->computation_time)));
      }
      
      anyAnalyser_.printAny (a.type(), a.value());      
    }
  TAO_CATCHANY    
    {
      ACE_ERROR ((LM_ERROR, "(%t)Error in extracting the data.\n"));
    }
  TAO_ENDTRY;	    
}


