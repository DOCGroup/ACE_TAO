// $Id$

//==============================================================
/**
 * @file client.cpp
 *
 * This is a simple client test program that interact with the EC
 * component implementation.  This test uses the explicit factory
 * operation in the home interface to create a EC component
 * instance, run it for a while, and destroy the component instance.
 */
//==============================================================

#include "ECC.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

      // Resolve HomeFinder interface

      CORBA::Object_var obj
        = orb->string_to_object ("file://ec.ior");

      BasicSP::ECHome_var home
        = BasicSP::ECHome::_narrow (obj.in ());

      if (CORBA::is_nil (home.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire ECHome objref\n"), -1);

      BasicSP::EC_var pulser
        = home->create ();

      // Place to plug in the rate
      pulser->hertz (5);

      pulser->start ();

      ACE_OS::sleep (45);

      pulser->stop ();

      home->remove_component (pulser.in ());

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Who is the culprit \n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Uncaught CORBA exception\n"),
                        1);
    }

  return 0;
}
