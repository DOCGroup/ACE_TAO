// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_A_Impl.cpp
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
#include "Object_A_Impl.h"

// CTOR
Object_A_Impl::Object_A_Impl (void)
: finish_two_way_call_ (0)
{
}

// DTOR
Object_A_Impl::~Object_A_Impl (void)
{
}



void
Object_A_Impl::foo (Initiator_ptr initiator_ptr,
                    CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Object_A_Impl::foo ()\n"));

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
              "(%P|%t) END Object_A_Impl::foo ()\n"));
}

void 
Object_A_Impl::finish (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Object_A_Impl::finish ()\n"));

  this->finish_two_way_call_ = 1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Object_A_Impl::finish ()\n"));

}