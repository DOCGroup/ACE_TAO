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

#include "tao/corba.h"

ACE_RCSID(RootPOA, RootPOA, "$Id$")

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Initilize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);

  TAO_CHECK_ENV_PRINT_RETURN (env,
                              "CORBA::ORB_init",
                              -1);


  // Resolve the initial references for the name RootPOA thus getting
  // an object of type CORBA::Object.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA");

  // apply _narrow on the object of type CORBA::Object, to make it a
  // POA class Object.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in (), env);

  TAO_CHECK_ENV_PRINT_RETURN (env,
                              "PortableServer::POA::_narrow",
                              -1);

  // Get the name of the root POA.
  CORBA::String_var poa_name =
    root_poa->the_name (env);

  TAO_CHECK_ENV_PRINT_RETURN (env,
                              "PortableServer::POA::_narrow",
                              -1);

  ACE_DEBUG ((LM_DEBUG,
              "The RootPOA is : %s\n",
              poa_name.in ()));

  // Destroy the POA object,also destroys the child POAs if any.
  root_poa->destroy (1,
                     1,
                     env);

  TAO_CHECK_ENV_PRINT_RETURN (env,
                              "PortableServer::POA::destroy",
                              -1);

  return 0;
}
