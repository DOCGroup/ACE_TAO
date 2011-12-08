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

#include "HomeT_HomeCC.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Resolve HomeFinder interface

      CORBA::Object_var obj
        = orb->string_to_object ("file://Receiver.ior");

      ConnHome_var home
        = ConnHome::_narrow (obj.in ());

      if (CORBA::is_nil (home.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire ConnHome objref\n"), -1);

      home->new_ConnComp ();

      // Place to plug in the rate

      ACE_OS::sleep (15);

  //    home->remove_component ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Who is the culprit\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Uncaught CORBA exception\n"),
                        1);
    }

  return 0;
}
