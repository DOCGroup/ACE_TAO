// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_A_i.cpp
//
// = DESCRIPTION
//    This class implements the Object A  of the 
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "tao/corba.h"
#include "Initiator_i.h"

ACE_RCSID(Triangle_Test, Initiator_i, "$Id$")

// CTOR
Initiator_i::Initiator_i (Object_A_ptr object_A_ptr,
                                Object_B_ptr object_B_ptr)
: object_A_var_ (Object_A::_duplicate (object_A_ptr)),
  object_B_var_ (Object_B::_duplicate (object_B_ptr))
{
}

// DTOR
Initiator_i::~Initiator_i (void)
{
}



void
Initiator_i::foo_object_B (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Initiator_i::foo_object_B ()\n"));

  TAO_TRY
    {
      this->object_B_var_->foo (this->object_A_var_.in (), 
                                TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Initiator_i::foo_object_B: Returned from call.\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("calling the server_");
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Initiator_i::foo_object_B ()\n"));
}

