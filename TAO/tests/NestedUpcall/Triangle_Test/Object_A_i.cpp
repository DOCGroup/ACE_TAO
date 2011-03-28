
//=============================================================================
/**
 *  @file    Object_A_i.cpp
 *
 *  $Id$
 *
 *  This class implements the Object A  of the
 *  Nested Upcalls - Triangle test.
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================


#include "Object_A_i.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

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
Object_A_i::foo (Initiator_ptr theInitiator_ptr)
{
  try
    {
      theInitiator_ptr->foo_object_B ();

      while (!this->finish_two_way_call_)
        TAO_ORB_Core_instance ()->reactor ()->handle_events ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("calling the initiator");
    }

}

void
Object_A_i::finish (void)
{
  this->finish_two_way_call_ = 1;

}

void
Object_A_i::shutdown (void)
{
  int argc = 0;
  ACE_TCHAR **argv = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  orb->shutdown ();
}
