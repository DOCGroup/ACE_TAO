// $Id$

// ==========================================================================
// = LIBRARY
//    TAO/tests/POA/NewPOA
//
// = FILENAME
//    NewPOA.cpp
//
// = DESCRIPTION
//    This program demonstrates creation of new POAs, as children of the root POA or the
//    existing POA.
//    There are three new POA created in this example.
//    The hierarchy of POAs looks like this.
//
//             /-->first_poa-->first_poa/second_poa
//    RootPOA--
//             \-->third_poa
//
// = AUTHOR
//    Irfan Pyarali
// ========================================================================

#include "tao/corba.h"

ACE_RCSID(NewPOA, NewPOA, "$Id$")

int
main (int argc, char **argv)
{
  //  CORBA::Environment env;

  ACE_DECLARE_NEW_CORBA_ENV;

  // The first step Initialize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, 
                                        argv, 
                                        0, 
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  // Obtain the object reference to the RootPOA.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA");

  // _narrow() the Object to get the POA object, i.e., the root_poa.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Policies for the new POAs
  CORBA::PolicyList policies (2);
  policies.length (2);

  // Threading policy
  policies[0] =
    root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Lifespan policy
  policies[1] =
    root_poa->create_lifespan_policy (PortableServer::TRANSIENT, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Creation of the firstPOA
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa =
    root_poa->create_POA (name.c_str (),
                          PortableServer::POAManager::_nil (),
                          policies,
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Creation of the new POA, i.e. firstPOA/secondPOA
  name = "secondPOA";
  PortableServer::POA_var second_poa =
    first_poa->create_POA (name.c_str (),
                           PortableServer::POAManager::_nil (),
                           policies,
                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Creating thirdPOA.
  name = "thirdPOA";

  PortableServer::POA_var third_poa =
    root_poa->create_POA (name.c_str (),
                          PortableServer::POAManager::_nil (),
                          policies,
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Creation of the new POAs over, so destroy the Policy_ptr's.
  for (CORBA::ULong i = 0;
       i < policies.length () && ACE_TRY_ENV.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_TRY_ENV);
    }

  ACE_CHECK_RETURN (-1);

  // Get the names of all the POAs and print them out.

  CORBA::String_var root_poa_name =
    root_poa->the_name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::String_var first_poa_name =
    first_poa->the_name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::String_var second_poa_name =
    second_poa->the_name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);


  CORBA::String_var third_poa_name =
    third_poa->the_name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "%s\n%s\n%s\n%s\n",
              root_poa_name.in (),
              first_poa_name.in (),
              second_poa_name.in (),
              third_poa_name.in ()));

  // This should destroy all its children
  root_poa->destroy (1,
                     1,
                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}
