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
                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Object_A_i::foo ()\n"));

  ACE_TRY
    {
      initiator_ptr->foo_object_B (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      while (!this->finish_two_way_call_)
        TAO_ORB_Core_instance ()->reactor ()->handle_events ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "calling the initiator");
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Object_A_i::foo ()\n"));
}

void
Object_A_i::finish (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Object_A_i::finish ()\n"));

  this->finish_two_way_call_ = 1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Object_A_i::finish ()\n"));

}
