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

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);

      // Resolve HomeFinder interface

      CORBA::Object_var obj
        = orb->string_to_object ("file://ECHome.ior" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      BasicSP::ECHome_var home
        = BasicSP::ECHome::_narrow (obj.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (home.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire ECHome objref\n"), -1);

      BasicSP::EC_var pulser
        = home->new_EC (8
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::sleep (45);
      home->remove_component (pulser.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
