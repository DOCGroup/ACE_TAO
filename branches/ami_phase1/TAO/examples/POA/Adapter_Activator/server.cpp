// $Id$

// ================================================================
//
// = LIBRARY
//    TAO/tests/POA/Adapter_Activator
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    This example is very similar to the Explicit_Activation example
//    except that the POAs are deleted once the object references have
//    been created. After this, an adapter activator is install in the
//    RootPOA to reactivate the POAs on demand.
//
//    Similar to the Explicit_Activation, the client for this example
//    is Generic_Servant.
//
// = AUTHOR
//    Irfan Pyarali
//
// ================================================================

#include "ace/Get_Opt.h"
#include "MyFooServant.h"

ACE_RCSID(Adapter_Activator, server, "$Id$")

class Adapter_Activator : public POA_PortableServer::AdapterActivator
{
public:

  Adapter_Activator (PortableServer::POAManager_ptr poa_manager,
                     CORBA::ORB_ptr orb);

  CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                  const char *name,
                                  CORBA_Environment &ACE_TRY_ENV);

  CORBA::PolicyList first_poa_policies_;
  CORBA::PolicyList second_poa_policies_;

private:

  PortableServer::POAManager_var poa_manager_;
  CORBA::ORB_var orb_;
};

Adapter_Activator::Adapter_Activator (PortableServer::POAManager_ptr poa_manager,
                                      CORBA::ORB_ptr orb)
  : poa_manager_ (PortableServer::POAManager::_duplicate (poa_manager)),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Boolean
Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                    const char *name,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  if (ACE_OS::strcmp (name, "firstPOA") == 0)
    {
      PortableServer::POA_var child = parent->create_POA (name,
                                                          this->poa_manager_.in (),
                                                          this->first_poa_policies_,
                                                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Creation of firstPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < this->first_poa_policies_.length ();
           ++i)
        {
          this->first_poa_policies_[i]->destroy (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }

      PortableServer::AdapterActivator_var activator = this->_this (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      child->the_activator (activator.in (),
                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      MyFooServant *foo_impl = new MyFooServant (this->orb_.in (),
                                                 child.in (),
                                                 28);

      child->set_servant (foo_impl,
                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Finally everything is fine
      return 1;
    }
  else if (ACE_OS::strcmp (name, "secondPOA") == 0)
    {
      PortableServer::POA_var child = parent->create_POA (name,
                                                          this->poa_manager_.in (),
                                                          this->second_poa_policies_,
                                                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Creation of secondPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < this->second_poa_policies_.length ();
           ++i)
        {
          this->second_poa_policies_[i]->destroy (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }

      MyFooServant *foo_impl = new MyFooServant (this->orb_.in (),
                                                 child.in (),
                                                 29);

      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("third Foo");

      child->activate_object_with_id (oid.in (),
                                      foo_impl,
                                      ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Finally everything is fine
      return 1;
    }
  else
    {
      // Unknown POA.
      return 0;
    }
}

static char *ior_output_file = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f ior_output_file]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

static int
write_iors_to_file (const char *first_ior,
                    const char *second_ior,
                    const char *third_ior)
{
  if (ior_output_file == 0)
    // No filename was specified; simply return
    return 0;

  char ior_output_file_1[BUFSIZ];
  char ior_output_file_2[BUFSIZ];
  char ior_output_file_3[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1, "%s_1", ior_output_file);
  ACE_OS::sprintf (ior_output_file_2, "%s_2", ior_output_file);
  ACE_OS::sprintf (ior_output_file_3, "%s_3", ior_output_file);

  FILE *output_file_1 = ACE_OS::fopen (ior_output_file_1, "w");
  FILE *output_file_2 = ACE_OS::fopen (ior_output_file_2, "w");
  FILE *output_file_3 = ACE_OS::fopen (ior_output_file_3, "w");

  if (output_file_1 == 0 ||
      output_file_2 == 0 ||
      output_file_3 == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %s, %s %s\n",
                       ior_output_file_1,
                       ior_output_file_2,
                       ior_output_file_3),
                      -1);

  u_int result = 0;

  result = ACE_OS::fprintf (output_file_1,
                            "%s",
                            first_ior);
  if (result != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       first_ior,
                       ior_output_file_1),
                      -1);

  result = ACE_OS::fprintf (output_file_2,
                            "%s",
                            second_ior);
  if (result != ACE_OS::strlen (second_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       second_ior,
                       ior_output_file_2),
                      -1);

  result = ACE_OS::fprintf (output_file_3,
                            "%s",
                            third_ior);
  if (result != ACE_OS::strlen (third_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       third_ior,
                       ior_output_file_3),
                      -1);

  ACE_OS::fclose (output_file_1);
  ACE_OS::fclose (output_file_2);
  ACE_OS::fclose (output_file_3);

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Adapter_Activator adapter_activator (poa_manager.in (),
                                           orb.in ());

      PortableServer::AdapterActivator_var activator = adapter_activator._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      root_poa->the_activator (activator.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var first_poa;
      PortableServer::POA_var second_poa;

      {
        // Policies for the firstPOA to be created.
        CORBA::PolicyList &policies = adapter_activator.first_poa_policies_;
        policies.length (4);

        // Id Assignment Policy
        policies[0] =
          root_poa->create_id_assignment_policy (PortableServer::SYSTEM_ID,
                                                 ACE_TRY_ENV);
        ACE_TRY_CHECK;

        // Lifespan policy
        policies[1] =
          root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
                                            ACE_TRY_ENV);
        ACE_TRY_CHECK;

        // Request Processing policy
        policies[2] =
          root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT,
                                                      ACE_TRY_ENV);
        ACE_TRY_CHECK;

        // Id Uniqueness
        policies[3] =
          root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID,
                                                 ACE_TRY_ENV);
        ACE_TRY_CHECK;

        // Create the firstPOA under the RootPOA.
        first_poa = root_poa->create_POA ("firstPOA",
                                          poa_manager.in (),
                                          policies,
                                          ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      {
        // Policies for the secondPOA to be created.
        CORBA::PolicyList &policies = adapter_activator.second_poa_policies_;
        policies.length (2);

        // Id Assignment Policy
        policies[0] =
          root_poa->create_id_assignment_policy (PortableServer::USER_ID,
                                                 ACE_TRY_ENV);
        ACE_TRY_CHECK;

        // Lifespan policy
        policies[1] =
          root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
                                            ACE_TRY_ENV);
        ACE_TRY_CHECK;

        // Create the secondPOA under the firstPOA.
        second_poa = first_poa->create_POA ("secondPOA",
                                            poa_manager.in (),
                                            policies,
                                            ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      // Create a servant.
      MyFooServant first_foo_impl (orb.in (),
                                   root_poa.in (),
                                   27);

      PortableServer::ObjectId_var first_oid =
        root_poa->activate_object (&first_foo_impl,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get Object Reference for the first_foo_impl object.
      Foo_var first_foo = first_foo_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var second_foo =
        first_poa->create_reference ("IDL:Foo:1.0",
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var third_oid =
        PortableServer::string_to_ObjectId ("third Foo");

      CORBA::Object_var third_foo =
        second_poa->create_reference_with_id (third_oid.in (),
                                              "IDL:Foo:1.0",
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Stringyfy all the object references and print them out.
      CORBA::String_var first_ior =
        orb->object_to_string (first_foo.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var second_ior =
        orb->object_to_string (second_foo.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var third_ior =
        orb->object_to_string (third_foo.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n%s\n%s\n",
                  first_ior.in (),
                  second_ior.in (),
                  third_ior.in ()));

      int write_result = write_iors_to_file (first_ior.in (),
                                             second_ior.in (),
                                             third_ior.in ());
      if (write_result != 0)
        return write_result;

      first_poa->destroy (1,
                          1,
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

      // Destroy RootPOA (also destroys all child POAs).
      root_poa->destroy (1,
                         1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
