// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Current
//
// = FILENAME
//     Current.cpp
//
// = DESCRIPTION
//     This program tests if accessing the POA current before the
//     RootPOA can cause any problems.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object;

      object =
        orb->resolve_initial_references ("POACurrent"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::Current_var current =
        PortableServer::Current::_narrow (object.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
