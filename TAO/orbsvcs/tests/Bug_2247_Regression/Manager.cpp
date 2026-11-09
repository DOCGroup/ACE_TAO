#include "Manager.h"
#include "Client_i.h"
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/Malloc_Base.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/PortableServer/PortableServer.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "ace/OS_NS_stdio.h"

#include <memory>
#include <string>

struct Read_Buffer_Deleter
{
  ACE_Allocator *allocator {};

  void operator() (char *buffer) const
  {
    if (buffer != nullptr)
      this->allocator->free (buffer);
  }
};

int
Manager::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:k:b:l:c:sm"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        this->options_.first_ior = get_opts.opt_arg ();
        break;
      case 'k':
        this->options_.first_key = get_opts.opt_arg ();
        break;
      case 'b':
        this->options_.second_ior = get_opts.opt_arg ();
        break;
      case 'l':
        this->options_.second_key = get_opts.opt_arg ();
        break;
      case 'c':
        this->options_.ior_output_file = get_opts.opt_arg ();
        break;
      case 's':
        this->options_.shutdown_test = true;
        break;
      case 'm':
        this->options_.merged_test = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <iorfile1> -k <key1> "
                           "-b <iorfile2> -l <key2> "
                           "-c <output ior file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (this->options_.first_ior == nullptr
      || this->options_.first_key == nullptr
      || this->options_.second_ior == nullptr
      || this->options_.second_key == nullptr
      || this->options_.ior_output_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "All IOR files, keys, and output file are required\n"), -1);

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Manager manager;

  try
    {
      // Initialize the ORB, POA etc.
      manager.init (argc, argv);

      // the command line arguments
      if (manager.parse_args (argc, argv) == -1)
        return -1;

      // Merge the different IORS
      manager.make_merged_iors ();

      if (manager.options ().merged_test)
        {
          // Set properties. This is the most important portion of the
          // test
          manager.set_properties ();
        }

      // Write IOR to file
      manager.write_to_file ();

      // Client, who is going to use the merged IOR
      // Construct that with the managers ORB
      Client_i client_imp (manager.orb (), manager.options ());
      return client_imp.init ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught");
      return -1;
    }

}

void
Manager::init (int argc,
               ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  // Obtain the RootPOA.
  CORBA::Object_var const obj_var =
    this->orb_->resolve_initial_references ("RootPOA");

  // Get the POA_var object from Object_var.
  PortableServer::POA_var const root_poa_var =
    PortableServer::POA::_narrow (obj_var.in ());

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var const poa_manager_var =
    root_poa_var->the_POAManager ();

  poa_manager_var->activate ();
}

Test_Options const &
Manager::options () const
{
  return this->options_;
}

int
Manager::make_merged_iors ()
{
  // First  server
  this->object_primary_ =
    this->orb_->string_to_object (this->options_.first_ior);

  if (this->options_.merged_test)
    {
      //Second server
      this->object_secondary_ =
        this->orb_->string_to_object (this->options_.second_ior);

      // Get an object reference for the ORBs IORManipultion object!
      CORBA::Object_var const iorm_object =
        this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                                0);

      this->iorm_ =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

      // Create the list
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length (2);
      iors[0] = CORBA::Object::_duplicate (this->object_primary_.in ());
      iors[1] = CORBA::Object::_duplicate (this->object_secondary_.in ());

      // Create a merged set 1;
      merged_set_ =
        this->iorm_->merge_iors (iors);
    }
  else
    {
      merged_set_ = this->object_primary_;
    }

  return 0;
}

int
Manager::set_properties ()
{
  FT::TagFTGroupTaggedComponent ft_tag_component;

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = static_cast<CORBA::Octet> (1);
  ft_tag_component.component_version.minor = static_cast<CORBA::Octet> (0);

  // Domain id
  char const * const id = "iogr_testing";
  ft_tag_component.group_domain_id = id;

  // Object group id
  ft_tag_component.object_group_id = static_cast<CORBA::ULongLong> (10);

  // Version
  ft_tag_component.object_group_ref_version = static_cast<CORBA::ULong> (5);

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  CORBA::Boolean const retval = this->iorm_->set_primary (&iogr_prop,
                                                           this->object_secondary_.in (),
                                                           this->merged_set_.in ());

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval)
    {
      this->iorm_->set_property (&iogr_prop, this->merged_set_.in ());
    }

  return 0;
}

int
Manager::write_to_file ()
{
  CORBA::String_var const iorref =
    this->orb_->object_to_string (this->merged_set_.in ());

  if (this->options_.ior_output_file != nullptr)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
        ACE_OS::fopen (this->options_.ior_output_file, "w"), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ACE_TEXT_ALWAYS_CHAR (this->options_.ior_output_file)),
                          1);
      ACE_OS::fprintf (output_file.get (), "%s", iorref.in ());
    }

  return 0;
}

CORBA::ORB_ptr
Manager::orb () const
{
  return this->orb_.in ();
}

Client_i::Client_i (CORBA::ORB_ptr orb, Test_Options const &options)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    options_ (options)
{
}

int
run_remote_test (Simple_Server_ptr server,
                 Test_Options const &options,
                 ACE_TCHAR const *execute_key)
{
  std::string expected;
  if (execute_key)
    {
      expected = "remote_call() completed by ";
      expected += ACE_TEXT_ALWAYS_CHAR (execute_key);
    }
  else if (options.shutdown_test && !options.merged_test)
    {
      expected = "CORBA::COMM_FAILURE";
    }
  else
    {
      expected = "CORBA::TRANSIENT";
    }

  std::string received;
  try
    {
      // Make a remote call
      CORBA::String_var s =
        server->remote_call ();

      received = s.in () == nullptr ? "(null)" : s.in ();
    }
  catch (CORBA::TRANSIENT const &)
    {
      received = "CORBA::TRANSIENT";
    }
  catch (CORBA::COMM_FAILURE const &)
    {
      received = "CORBA::COMM_FAILURE";
    }

  ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("CLIENT> REMOTE   expected: '%C'"),
               expected.c_str ()));

  if (expected != received)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("... FAIL\n")
                  ACE_TEXT ("CLIENT>          received: '%C'\n"),
                  received.c_str ()));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int
run_abort_test (Simple_Server_ptr server,
                Test_Options const &options,
                ACE_TCHAR const *request_key,
                ACE_TCHAR const *execute_key)
{
  std::string expected;
  if (execute_key)
    {
      expected = "abort() completed by ";
      expected += ACE_TEXT_ALWAYS_CHAR (execute_key);
      expected += ", still_alive=1";
    }
  else if (options.merged_test)
    {
      expected = "CORBA::TRANSIENT";
    }
  else
    {
      expected = "CORBA::COMM_FAILURE";
    }

  std::string received;
  try
    {
      CORBA::String_var s = server->abort (ACE_TEXT_ALWAYS_CHAR (request_key));

      received = s.in () == nullptr ? "(null)" : s.in ();
    }
  catch (CORBA::TRANSIENT const &)
    {
      received = "CORBA::TRANSIENT";
    }
  catch (CORBA::COMM_FAILURE const &)
    {
      received = "CORBA::COMM_FAILURE";
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CLIENT> ABORT    expected: '%C'"),
              expected.c_str ()));
  if (expected != received)
    {
      ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("... FAIL\n")
              ACE_TEXT ("CLIENT>          received: '%C'\n"),
              received.c_str ()));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int
run_shutdown_test (Simple_Server_ptr server,
                   ACE_TCHAR const *request_key,
                   ACE_TCHAR const *execute_key)
{
  std::string expected;
  if (execute_key)
    {
      expected = "shutdown() completed by ";
      expected += ACE_TEXT_ALWAYS_CHAR (execute_key);
      expected += ", still_alive=0";
    }
  else
    {
      expected = "CORBA::TRANSIENT";
    }

  std::string received;
  try
    {
      CORBA::String_var s = server->shutdown (ACE_TEXT_ALWAYS_CHAR (request_key));

      received = s.in () == nullptr ? "(null)" : s.in ();
    }
  catch (CORBA::TRANSIENT const &)
    {
      received = "CORBA::TRANSIENT";
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("CLIENT> SHUTDOWN expected: '%C'"),
              expected.c_str ()));
  if (expected != received)
    {
      ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("... FAIL\n")
              ACE_TEXT ("CLIENT>          received: '%C'\n"),
              received.c_str ()));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int
run_full_abort_test (Simple_Server_ptr server, Test_Options const &options)
{
  int rv = run_remote_test (server, options, options.second_key);
  if (rv)
    {
      return rv;
    }

  rv = run_abort_test (server, options, options.second_key, options.first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, options, options.first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_abort_test (server, options, options.first_key, nullptr);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, options, nullptr);
  if (rv)
    {
      return rv;
    }

  return 0;
}

int
run_full_shutdown_test (Simple_Server_ptr server, Test_Options const &options)
{
  int rv = run_remote_test (server, options, options.second_key);
  if (rv)
    {
      return rv;
    }

  rv = run_shutdown_test (server, options.second_key, options.second_key);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, options, options.first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_shutdown_test (server, options.first_key, options.first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, options, nullptr);
  if (rv)
    {
      return rv;
    }

  return 0;
}

int
Client_i::init ()
{
  FILE * const ior_file = ACE_OS::fopen (this->options_.ior_output_file, "r");
  if (ior_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to open %s for reading\n"),
                       ACE_TEXT_ALWAYS_CHAR (this->options_.ior_output_file)),
                      -1);

  ACE_Read_Buffer ior_buffer (ior_file, true);
  std::unique_ptr<char, Read_Buffer_Deleter> const data (
    ior_buffer.read (), Read_Buffer_Deleter {ior_buffer.alloc ()});
  if (!data)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to read IOR file\n"), -1);

  CORBA::Object_var const object = this->orb_->string_to_object (data.get ());

  // Combined IOR stuff
  Simple_Server_var const server =
    Simple_Server::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Object reference <%C> is nil\n",
                         data.get ()),
                        -1);
    }

  if (!this->options_.merged_test)
    {
      int const rv = this->options_.shutdown_test
        ? run_shutdown_test (server.in (), this->options_.first_key,
                             this->options_.first_key)
        : run_abort_test (server.in (), this->options_, this->options_.first_key, nullptr);

      if (rv != 0)
        {
          return rv;
        }

      return run_remote_test (server.in (), this->options_, nullptr);
    }

  if (this->options_.shutdown_test)
    {
      return run_full_shutdown_test (server.in (), this->options_);
    }

  return run_full_abort_test (server.in (), this->options_);
}
