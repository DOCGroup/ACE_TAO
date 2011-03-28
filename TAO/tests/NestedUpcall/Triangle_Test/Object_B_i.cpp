
//=============================================================================
/**
 *  @file    Object_B_i.cpp
 *
 *  $Id$
 *
 *  This class implements the Object B  of the
 *  Nested Upcalls - Triangle test.
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================


#include "tao/Exception.h"
#include "Object_B_i.h"
#include "ace/OS_NS_unistd.h"

// CTOR
Object_B_i::Object_B_i (void)
{
}

// DTOR
Object_B_i::~Object_B_i (void)
{
}



void
Object_B_i::foo (Object_A_ptr theObject_A_ptr)
{
  try
    {

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) BEGIN Object_B_i::foo: Trying to call Object A\n"));

      theObject_A_ptr->finish ();

      // Start to wait on this variable, it is set to true
      // by the method finish ()
      long int usecs = 500000;
      ACE_Time_Value pause(0, usecs);

      ACE_OS::sleep(pause);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("calling the initiator");
    }

}

void
Object_B_i::shutdown (void)
{
  int argc = 0;
  ACE_TCHAR **argv = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  orb->shutdown ();
}
