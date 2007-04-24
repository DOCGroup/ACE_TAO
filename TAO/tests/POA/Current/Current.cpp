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

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var object;

      object = orb->resolve_initial_references ("POACurrent");

      PortableServer::Current_var current =
        PortableServer::Current::_narrow (object.in ());

      object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
