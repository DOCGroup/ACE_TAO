// $Id$
// ===========================================================================================
// = LIBRARY
//    TAO/tests/POA/NewPOA
//
// = FILENAME
//    NewPOA.cpp
//
// = DESCRIPTION
//    This program demonstrates creation of new POAs, as children of the root POA or the
//    existing POA.
//    There are five new POA created in this example.
//    The hierarchy of POAs looks like this.
//
//             /-->first_poa-->first_poa/second_poa
//    RootPOA--
//             \-->third_poa-->third_poa/fourth_poa-->third_poa/fourth_poa/fifth_poa
//
// = AUTHOR
//    Irfan Pyarali
// ===========================================================================================

#include "ace/streams.h"
#include "tao/corba.h"

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // The first step : Initialize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

  // _narrow() the Object to get the POA object, i.e., the root_poa
  PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in(), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  // CORBA::PolicyList policies (2); Policies for the new POAs
  PortableServer::PolicyList policies (2);
  policies.length (2);
  policies[0] = root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL, env);
  policies[1] = root_poa->create_lifespan_policy (PortableServer::TRANSIENT, env);

  // Creation of the firstPOA
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa = root_poa->create_POA (name.c_str (),
                                                            PortableServer::POAManager::_nil(),
                                                            policies,
                                                            env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Creation of the new POA, i.e. firstPOA/secondPOA
  name += TAO_POA::name_separator ();
  name += "secondPOA";
  PortableServer::POA_var second_poa = root_poa->create_POA (name.c_str (),
                                                             PortableServer::POAManager::_nil(),
                                                             policies,
                                                             env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Creating thirdPOA/fourthPOA/fifthPOA. The non-existing thirdPOA and thirdPOA/fourthPOA
  // are created automatically.
  name = "thirdPOA";
  name += TAO_POA::name_separator ();
  name += "forthPOA";
  name += TAO_POA::name_separator ();
  name += "fifthPOA";

  PortableServer::POA_var fifth_poa = root_poa->create_POA (name.c_str (),
                                                            PortableServer::POAManager::_nil(),
                                                            policies,
                                                            env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Creation of the new POAs over, so destroy the Policy_ptr's.
  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       ++i)
    {
      PortableServer::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Get the names of all the POAs and print them out.

  CORBA::String_var root_poa_name = root_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var first_poa_name = first_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var second_poa_name = second_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var fifth_poa_name = fifth_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  cout << root_poa_name.in () << endl;
  cout << first_poa_name.in () << endl;
  cout << second_poa_name.in () << endl;
  cout << fifth_poa_name.in () << endl;

  // This should destroy all its children
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
