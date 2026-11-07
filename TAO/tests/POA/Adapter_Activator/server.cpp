//=============================================================================
/**
 *  @file    server.cpp
 *
 *  This example is very similar to the Explicit_Activation example
 *  except that the POAs are deleted once the object references have
 *  been created. After this, an adapter activator is install in the
 *  RootPOA to reactivate the POAs on demand.
 *
 *  Similar to the Explicit_Activation, the client for this example
 *  is Generic_Servant.
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "tao/PortableServer/AdapterActivatorC.h"

#include <cstdio>
#include <cstring>
#include <memory>
#include <string>

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

  reference_counted_test_i (reference_counted_test_i const &) = delete;
  reference_counted_test_i &operator= (reference_counted_test_i const &) = delete;
  reference_counted_test_i (reference_counted_test_i &&) = delete;
  reference_counted_test_i &operator= (reference_counted_test_i &&) = delete;
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

  Adapter_Activator (Adapter_Activator const &) = delete;
  Adapter_Activator &operator= (Adapter_Activator const &) = delete;
  Adapter_Activator (Adapter_Activator &&) = delete;
  Adapter_Activator &operator= (Adapter_Activator &&) = delete;

  CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                 char const *name) override;

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
                                    char const *name)
{
  if (std::strcmp (name, "firstPOA") == 0)
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

      PortableServer::ServantBase_var servant (
        new reference_counted_test_i (this->orb_.in (), child.in ()));

      child->set_servant (servant.in ());

      // Finally everything is fine
      return 1;
    }
  else if (std::strcmp (name, "secondPOA") == 0)
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

      PortableServer::ServantBase_var servant (
        new reference_counted_test_i (this->orb_.in (), child.in ()));

      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("third test");

      child->activate_object_with_id (oid.in (), servant.in ());

      // Finally everything is fine
      return 1;
    }
  // Unknown POA.
  return false;
}

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
write_ior_to_file (std::string const &filename,
                   char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (std::fopen (filename.c_str (), "w"), std::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output file for writing IOR: %C\n", filename.c_str ()), -1);

  int const result = std::fprintf (output_file.get (), "%s", ior);
  if (result < 0 || static_cast<size_t> (result) != std::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR, "Failed writing IOR to %C\n", filename.c_str ()), -1);
  return 0;
}

static int
write_iors_to_file (ACE_TCHAR const *ior_output_file,
                    char const *first_ior,
                    char const *second_ior,
                    char const *third_ior)
{
  std::string const base (ACE_TEXT_ALWAYS_CHAR (ior_output_file));
  std::string const first_filename = base + "_1";
  std::string const second_filename = base + "_2";
  std::string const third_filename = base + "_3";

  if (write_ior_to_file (first_filename, first_ior) != 0 ||
      write_ior_to_file (second_filename, second_ior) != 0 ||
      write_ior_to_file (third_filename, third_ior) != 0)
    return -1;
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      ACE_TCHAR const *ior_output_file = ACE_TEXT ("ior");
      int const result = parse_args (argc, argv, ior_output_file);
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

      int const write_result = write_iors_to_file (ior_output_file,
                                                   first_ior.in (),
                                                   second_ior.in (),
                                                   third_ior.in ());
      if (write_result != 0)
        return write_result;

      first_poa->destroy (true, true);

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
