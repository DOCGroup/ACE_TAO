//=============================================================================
/**
 *  @file    Object_B_i.cpp
 *
 *  This class implements the Object B  of the
 *  Nested Upcalls - Triangle test.
 *
 *  @author Michael Kircher
 */
//=============================================================================
#include "tao/Exception.h"
#include "Object_B_i.h"
#include "ace/OS_NS_unistd.h"

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
      long int const usecs = 500000;
      ACE_Time_Value const pause (0, usecs);

      ACE_OS::sleep (pause);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("calling the initiator");
    }

}

void
Object_B_i::shutdown ()
{
  int argc {};
  ACE_TCHAR **argv {};
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  orb->shutdown ();
}
