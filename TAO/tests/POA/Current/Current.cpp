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

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/Log_Msg.h"

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

      CORBA::Object_var object;

      object =
        orb->resolve_initial_references ("POACurrent"
                                         ACE_ENV_ARG_PARAMETER);

      PortableServer::Current_var current =
        PortableServer::Current::_narrow (object.in ()
                                          ACE_ENV_ARG_PARAMETER);

      object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);

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

  return 0;
}
