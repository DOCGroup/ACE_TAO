
//=============================================================================
/**
 *  @file     Current.cpp
 *
 *  $Id$
 *
 *   This program tests if accessing the POA current before the
 *   RootPOA can cause any problems.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/Log_Msg.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
