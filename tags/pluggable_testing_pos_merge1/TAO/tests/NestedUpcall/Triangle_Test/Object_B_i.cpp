// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_B_i.cpp
//
// = DESCRIPTION
//    This class implements the Object B  of the 
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "tao/corba.h"
#include "Object_B_i.h"

ACE_RCSID(Triangle_Test, Object_B_i, "$Id$")

// CTOR
Object_B_i::Object_B_i (void)
{
}

// DTOR
Object_B_i::~Object_B_i (void)
{
}



void
Object_B_i::foo (Object_A_ptr object_A_ptr,
                    CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Object_B_i::foo ()\n"));

  TAO_TRY
    {
      
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) BEGIN Object_B_i::foo: Trying to call Object A\n"));

      object_A_ptr->finish (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Start to wait on this variable, it is set to true
      // by the method finish ()
      long int usecs = 500000;
      ACE_Time_Value pause(0, usecs);

      ACE_OS::sleep(pause);

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("calling the initiator");
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Object_B_i::foo ()\n"));
}

