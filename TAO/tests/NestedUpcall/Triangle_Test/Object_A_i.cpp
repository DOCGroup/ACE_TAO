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

#include "Object_A_i.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

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
Object_A_i::foo (Initiator_ptr theInitiator_ptr
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      theInitiator_ptr->foo_object_B ();

      while (!this->finish_two_way_call_)
        TAO_ORB_Core_instance ()->reactor ()->handle_events ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "calling the initiator");
    }
  ACE_ENDTRY;

}

void
Object_A_i::finish (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->finish_two_way_call_ = 1;

}
