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
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID(NewPOA, NewPOA, "$Id$")

void
print_poa (PortableServer::POA_ptr poa
           TAO_ENV_ARG_DECL)
{
  CORBA::String_var poa_name =
    poa->the_name (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::OctetSeq_var poa_id =
    poa->id (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "POA name = %s\n",
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
    poa->the_children (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong index = 0;
       index != children->length ();
       ++index)
    {
      print_poa (children[index].in ()
                 TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

int
main (int argc, char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // The first step Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // _narrow() the Object to get the POA object, i.e., the root_poa.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Policies for the new POAs
      CORBA::PolicyList policies (2);
      policies.length (2);

      // Threading policy
      policies[0] =
        root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Lifespan policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::TRANSIENT TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of the firstPOA
      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->create_POA (name.c_str (),
                              PortableServer::POAManager::_nil (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of the new POA, i.e. firstPOA/secondPOA
      name = "secondPOA";
      PortableServer::POA_var second_poa =
        first_poa->create_POA (name.c_str (),
                               PortableServer::POAManager::_nil (),
                               policies
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creating thirdPOA.
      name = "thirdPOA";

      PortableServer::POA_var third_poa =
        root_poa->create_POA (name.c_str (),
                              PortableServer::POAManager::_nil (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Get the names of all the POAs and print them out.

      CORBA::String_var root_poa_name =
        root_poa->the_name (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var first_poa_name =
        first_poa->the_name (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var second_poa_name =
        second_poa->the_name (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var third_poa_name =
        third_poa->the_name (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n%s\n%s\n%s\n",
                  root_poa_name.in (),
                  first_poa_name.in (),
                  second_poa_name.in (),
                  third_poa_name.in ()));

      print_poa (root_poa.in ()
                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught in main ");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
