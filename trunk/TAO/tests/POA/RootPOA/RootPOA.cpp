
// $Id$


//==================================================================================
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
//==================================================================================

#include "ace/streams.h"
#include "tao/corba.h"

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Initilize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Resolve the initial references for the name RootPOA thus getting
  // an object of type CORBA::Object.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA");

  // apply _narrow on the object of type CORBA::Object, to make it a
  // POA class Object.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  // Get the name of the root POA.
  CORBA::String_var poa_name =
    root_poa->the_name (env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "The RootPOA is : %s\n",
              poa_name.in ()));
  
  // Destroy the POA object,also destroys the child POAs if any.
  root_poa->destroy (CORBA::B_TRUE,
                     CORBA::B_TRUE,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  return 0;
}
