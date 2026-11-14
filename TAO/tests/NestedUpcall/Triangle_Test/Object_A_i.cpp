//=============================================================================
/**
 *  @file    Object_A_i.cpp
 *
 *  This class implements the Object A  of the
 *  Nested Upcalls - Triangle test.
 *
 *  @author Michael Kircher
 */
//=============================================================================
#include "Object_A_i.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

void
Object_A_i::foo (Initiator_ptr theInitiator_ptr)
{
  try
    {
      theInitiator_ptr->foo_object_B ();

      while (!this->finish_two_way_call_)
        TAO_ORB_Core_instance ()->reactor ()->handle_events ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("calling the initiator");
    }

}

void
Object_A_i::finish ()
{
  this->finish_two_way_call_ = 1;
}

void
Object_A_i::shutdown ()
{
  int argc {};
  ACE_TCHAR **argv {};
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  orb->shutdown ();
}
