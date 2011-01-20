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

#include "tao/PortableServer/PortableServer.h"

#include "tao/ORB.h"

#include "ace/SString.h"
#include "ace/Log_Msg.h"

void
print_poa (PortableServer::POA_ptr poa)
{
  CORBA::String_var poa_name =
    poa->the_name ();

  CORBA::OctetSeq_var poa_id =
    poa->id ();

  ACE_DEBUG ((LM_DEBUG,
              "POA name = %C\n",
              poa_name.in ()));

  ACE_DEBUG ((LM_DEBUG,
              "POA id = "));

  for (CORBA::ULong i = 0;
       i != poa_id->length ();
       ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%o",
                  poa_id[i]));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  PortableServer::POAList_var children =
    poa->the_children ();

  for (CORBA::ULong index = 0;
       index != children->length ();
       ++index)
    {
      print_poa (children[index]);
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // The first step Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // _narrow() the Object to get the POA object, i.e., the root_poa.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Policies for the new POAs
      CORBA::PolicyList policies (2);
      policies.length (2);

      // Threading policy
      policies[0] =
        root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL);

      // Lifespan policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::TRANSIENT);

      // Creation of the firstPOA
      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->create_POA (name.c_str (),
                              PortableServer::POAManager::_nil (),
                              policies);

      // Creation of the new POA, i.e. firstPOA/secondPOA
      name = "secondPOA";
      PortableServer::POA_var second_poa =
        first_poa->create_POA (name.c_str (),
                               PortableServer::POAManager::_nil (),
                               policies);

      // Creating thirdPOA.
      name = "thirdPOA";

      PortableServer::POA_var third_poa =
        root_poa->create_POA (name.c_str (),
                              PortableServer::POAManager::_nil (),
                              policies);

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      // Get the names of all the POAs and print them out.

      CORBA::String_var root_poa_name =
        root_poa->the_name ();

      CORBA::String_var first_poa_name =
        first_poa->the_name ();

      CORBA::String_var second_poa_name =
        second_poa->the_name ();

      CORBA::String_var third_poa_name =
        third_poa->the_name ();

      ACE_DEBUG ((LM_DEBUG,
                  "%C\n%C\n%C\n%C\n",
                  root_poa_name.in (),
                  first_poa_name.in (),
                  second_poa_name.in (),
                  third_poa_name.in ()));

      print_poa (root_poa.in ());

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in main ");
      return -1;
    }

  return 0;
}
