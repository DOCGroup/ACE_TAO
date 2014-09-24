
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  This example is very similar to the Explicit_Activation example
 *  except that the POAs are deleted once the object references have
 *  been created. After this, an adapter activator is install in the
 *  RootPOA to reactivate the POAs on demand.
 *
 *  Similar to the Explicit_Activation, the client for this example
 *  is Generic_Servant.
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "tao/PortableServer/AdapterActivatorC.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

class reference_counted_test_i :
  public virtual test_i
{
public:
  /// Constructor - takes a POA and a value parameter
  reference_counted_test_i (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa);
};

reference_counted_test_i::reference_counted_test_i (CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa)
  : test_i (orb,
            poa)
{
}

class Adapter_Activator : public PortableServer::AdapterActivator
{
public:

  Adapter_Activator (PortableServer::POAManager_ptr poa_manager,
                     CORBA::ORB_ptr orb);

  CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                  const char *name);

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
                                    const char *name)
{
  if (ACE_OS::strcmp (name, "firstPOA") == 0)
    {
      PortableServer::POA_var child = parent->create_POA (name,
                                                          this->poa_manager_.in (),
                                                          this->first_poa_policies_);

      // Creation of firstPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < this->first_poa_policies_.length ();
           ++i)
        {
          this->first_poa_policies_[i]->destroy ();
        }

      child->the_activator (this);

      reference_counted_test_i *servant =
        new reference_counted_test_i (this->orb_.in (),
                                      child.in ());

      child->set_servant (servant);

      // This means that the ownership of <servant> now belongs to the
      // POA.
      servant->_remove_ref ();

      // Finally everything is fine
      return 1;
    }
  else if (ACE_OS::strcmp (name, "secondPOA") == 0)
    {
      PortableServer::POA_var child = parent->create_POA (name,
                                                          this->poa_manager_.in (),
                                                          this->second_poa_policies_);

      // Creation of secondPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < this->second_poa_policies_.length ();
           ++i)
        {
          this->second_poa_policies_[i]->destroy ();
        }

      reference_counted_test_i *servant =
        new reference_counted_test_i (this->orb_.in (),
                                      child.in ());

      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("third test");

      child->activate_object_with_id (oid.in (),
                                      servant);

      // This means that the ownership of <servant> now belongs to the
      // POA.
      servant->_remove_ref ();

      // Finally everything is fine
      return 1;
    }
  else
    {
      // Unknown POA.
      return 0;
    }
}

const ACE_TCHAR *ior_output_file = ACE_TEXT ("ior");

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.opt_arg ();
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
  char ior_output_file_1[BUFSIZ];
  char ior_output_file_2[BUFSIZ];
  char ior_output_file_3[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1, "%s_1", ACE_TEXT_ALWAYS_CHAR(ior_output_file));
  ACE_OS::sprintf (ior_output_file_2, "%s_2", ACE_TEXT_ALWAYS_CHAR(ior_output_file));
  ACE_OS::sprintf (ior_output_file_3, "%s_3", ACE_TEXT_ALWAYS_CHAR(ior_output_file));

  FILE *output_file_1 = ACE_OS::fopen (ior_output_file_1, ACE_TEXT("w"));
  FILE *output_file_2 = ACE_OS::fopen (ior_output_file_2, ACE_TEXT("w"));
  FILE *output_file_3 = ACE_OS::fopen (ior_output_file_3, ACE_TEXT("w"));

  if (output_file_1 == 0 ||
      output_file_2 == 0 ||
      output_file_3 == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %C, %C %C\n",
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
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       first_ior,
                       ior_output_file_1),
                      -1);

  result = ACE_OS::fprintf (output_file_2,
                            "%s",
                            second_ior);
  if (result != ACE_OS::strlen (second_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       second_ior,
                       ior_output_file_2),
                      -1);

  result = ACE_OS::fprintf (output_file_3,
                            "%s",
                            third_ior);
  if (result != ACE_OS::strlen (third_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       third_ior,
                       ior_output_file_3),
                      -1);

  ACE_OS::fclose (output_file_1);
  ACE_OS::fclose (output_file_2);
  ACE_OS::fclose (output_file_3);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Adapter_Activator adapter_activator (poa_manager.in (),
                                           orb.in ());

      root_poa->the_activator (&adapter_activator);

      PortableServer::POA_var first_poa;
      PortableServer::POA_var second_poa;

      {
        // Policies for the firstPOA to be created.
        CORBA::PolicyList &policies = adapter_activator.first_poa_policies_;
        policies.length (4);

        // Id Assignment Policy
        policies[0] =
          root_poa->create_id_assignment_policy (PortableServer::SYSTEM_ID);

        // Lifespan policy
        policies[1] =
          root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

        // Request Processing policy
        policies[2] =
          root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT);

        // Id Uniqueness
        policies[3] =
          root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

        // Create the firstPOA under the RootPOA.
        first_poa = root_poa->create_POA ("firstPOA",
                                          poa_manager.in (),
                                          policies);
      }

      {
        // Policies for the secondPOA to be created.
        CORBA::PolicyList &policies = adapter_activator.second_poa_policies_;
        policies.length (2);

        // Id Assignment Policy
        policies[0] =
          root_poa->create_id_assignment_policy (PortableServer::USER_ID);

        // Lifespan policy
        policies[1] =
          root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

        // Create the secondPOA under the firstPOA.
        second_poa = first_poa->create_POA ("secondPOA",
                                            poa_manager.in (),
                                            policies);
      }

      // Create a servant.
      reference_counted_test_i first_servant (orb.in (),
                                              root_poa.in ());

      PortableServer::ObjectId_var first_oid =
        root_poa->activate_object (&first_servant);

      // Get Object Reference for the first_servant object.
      test_var first_test = first_servant._this ();

      CORBA::Object_var second_test =
        first_poa->create_reference ("IDL:test:1.0");

      PortableServer::ObjectId_var third_oid =
        PortableServer::string_to_ObjectId ("third test");

      CORBA::Object_var third_test =
        second_poa->create_reference_with_id (third_oid.in (),
                                              "IDL:test:1.0");

      // Stringyfy all the object references and print them out.
      CORBA::String_var first_ior =
        orb->object_to_string (first_test.in ());

      CORBA::String_var second_ior =
        orb->object_to_string (second_test.in ());

      CORBA::String_var third_ior =
        orb->object_to_string (third_test.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "%C\n%C\n%C\n",
                  first_ior.in (),
                  second_ior.in (),
                  third_ior.in ()));

      int write_result = write_iors_to_file (first_ior.in (),
                                             second_ior.in (),
                                             third_ior.in ());
      if (write_result != 0)
        return write_result;

      first_poa->destroy (1,
                          1);

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
