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
#include "Object_A_i.h"

ACE_RCSID(Triangle_Test, Object_A_i, "$Id$")

// CTOR
Object_A_i::Object_A_i (void)
: finish_two_way_call_ (0)
{
}

// DTOR
Object_A_i::~Object_A_i (void)
{
}



void
Object_A_i::foo (Initiator_ptr initiator_ptr,
                    CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Object_A_i::foo ()\n"));

  TAO_TRY
    {
      initiator_ptr->foo_object_B (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      while (!this->finish_two_way_call_)
        TAO_ORB_Core_instance ()->reactor ()->handle_events ();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("calling the initiator");
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Object_A_i::foo ()\n"));
}

void
Object_A_i::finish (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Object_A_i::finish ()\n"));

  this->finish_two_way_call_ = 1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Object_A_i::finish ()\n"));

}
