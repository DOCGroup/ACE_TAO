// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/RootPOA
//
//
// = FILENAME
//     RootPOA.cpp
//
// = DESCRIPTION
//     This program gets the name of the Root POA and prints it out on
//     the standard output.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "tao/PortableServer/PortableServer.h"

#include "tao/ORB.h"

#include "ace/Log_Msg.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initilize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Resolve the initial references for the name RootPOA thus getting
      // an object of type CORBA::Object.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // apply _narrow on the object of type CORBA::Object, to make it
      // a POA class Object.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the name of the root POA.
      CORBA::String_var poa_name =
        root_poa->the_name ();

      ACE_DEBUG ((LM_DEBUG,
                  "The RootPOA is : %C\n",
                  poa_name.in ()));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised");
    }

  return 0;
}
