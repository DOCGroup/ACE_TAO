// $Id$

//===========================================================================
//
// = LIBRARY
//     TAO/tests/POA/FindPOA
//
// = FILENAME
//     FindPOA.cpp
//
// = DESCRIPTION
//     This Program tests the find_POA method of a POA.
//
// = AUTHOR
//     Irfan Pyarali
//
//===========================================================================

#include "tao/corba.h"

ACE_RCSID(FindPOA, FindPOA, "$Id$")

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Initialize the ORB

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Get Object reference to RootPOA.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA");

  // Narrow Object reference to RootPOA to a POA reference.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in(), env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  // Get a TAO_Adapter_Activator reference
  TAO_Adapter_Activator activator_impl;

  PortableServer::AdapterActivator_var activator =
    activator_impl._this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("TAO_Adapter_Activator::_this");
      return -1;
    }

  // Register the TAO_Adapter_Activator reference to be the RootPOA's
  // Adapter Activator.

  root_poa->the_activator (activator.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::the_activator");
      return -1;
    }

  // Try to find a childPOA of RootPOA named firstPOA
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa =
    root_poa->find_POA (name.c_str (),
                        1,
                        env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  // Use the TAO_POA name_separator (which is '/') to find a childPOA
  // of firstPOA named secondPOA.

  name += TAO_POA::name_separator ();
  name += "secondPOA";
  PortableServer::POA_var second_poa =
    root_poa->find_POA (name.c_str (),
                        1,
                        env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  // Create a hierarchical string of POA names
  // eg. thirdPOA/forthPOA/fifthPOA thirdPOA being the root of the
  // hierarchy with forthPOA as its child and fifthPOA as its
  // grandchild.

  name = "thirdPOA";
  name += TAO_POA::name_separator ();
  name += "forthPOA";
  name += TAO_POA::name_separator ();
  name += "fifthPOA";

  // Try to find the fifth_poa by passing the hierarchy of POA names
  // resulting in the creation of third and forth POAs as well as the
  // fifth POA.

  PortableServer::POA_var fifth_poa =
    root_poa->find_POA (name.c_str (),
                        1,
                        env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  // Get the names of all the POAs

  CORBA::String_var root_poa_name =
    root_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var first_poa_name =
    first_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var second_poa_name =
    second_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var fifth_poa_name =
    fifth_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }


  ACE_DEBUG((LM_DEBUG,"%s\n%s\n%s\n%s\n",
	     root_poa_name.in (),
	     first_poa_name.in (),
	     second_poa_name.in (),
	     fifth_poa_name.in ()));

  // This should destroy all its children

  root_poa->destroy (1,
                     1,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  return 0;
}
