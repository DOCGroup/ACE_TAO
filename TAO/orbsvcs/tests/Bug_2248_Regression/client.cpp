#include "client.h"
#include "Client_i.h"
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/PortableServer/PortableServer.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#include <cstdio>
#include <memory>
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:k:b:l:g:h:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.first_ior = get_opts.opt_arg ();
        break;
      case 'k':
        options.first_key = get_opts.opt_arg ();
        break;
      case 'b':
        options.second_ior = get_opts.opt_arg ();
        break;
      case 'l':
        options.second_key = get_opts.opt_arg ();
        break;
      case 'g':
        options.first_iogr_file = get_opts.opt_arg ();
        break;
      case 'h':
        options.second_iogr_file = get_opts.opt_arg ();
        break;
      case 't':
        options.sleep_delay = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -a <iorfile1> -k <key1> "
                                     "-b <iorfile2> -l <key2> "
                                     "-g <iogr v10 output file> "
                                     "-h <iogr v11 output file>\n"),
                           argv[0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Manager manager {};

  try
    {
      manager.init (argc, argv);

      Client_Options options {};
      if (parse_args (argc, argv, options) == -1)
        return -1;

      if (manager.make_merged_iors (options) != 0)
        return -1;

      if (manager.write_to_files (options) != 0)
        return -1;

      ACE_OS::sleep (options.sleep_delay);

      Client_i client_imp (manager.orb ());
      return client_imp.init (options);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught");
      return -1;
    }

  return 0;
}

void
Manager::init (int& argc,
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

int
Manager::make_merged_iors (Client_Options const &options)
{
  CORBA::Object_var const iorm_object =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION, nullptr);

  if (CORBA::is_nil (iorm_object.in ()))
    return -1;

  TAO_IOP::TAO_IOR_Manipulation_var const iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

  if (CORBA::is_nil (iorm.in ()))
    return -1;

  CORBA::Object_var first {this->orb_->string_to_object (
    ACE_TEXT_ALWAYS_CHAR (options.first_ior))};

  if (CORBA::is_nil (first.in ()))
    return -1;

  TAO_IOP::TAO_IOR_Manipulation::IORList iors {};
  iors.length (1);
  iors[0] = first;

  merged_set_10_ = iorm->merge_iors (iors);

  if (CORBA::is_nil (merged_set_10_.in ()))
    {
      return -1;
    }

  if (this->set_properties (iorm.in (), merged_set_10_.in (),
                            first.in (), 10) == -1)
    {
      return -1;
    }

  first = this->orb_->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.first_ior));
  if (CORBA::is_nil (first.in ()))
    {
      return -1;
    }

  CORBA::Object_var const second = this->orb_->string_to_object (
    ACE_TEXT_ALWAYS_CHAR (options.second_ior));

  if (CORBA::is_nil (second.in ()))
    {
      return -1;
    }

  iors.length (2);
  iors[0] = first;
  iors[1] = second;

  merged_set_11_ = iorm->merge_iors (iors);

  if (CORBA::is_nil (merged_set_11_.in ()))
    {
      return -1;
    }

  if (this->set_properties (iorm.in (), merged_set_11_.in (),
                            first.in (), 11) == -1)
    {
      return -1;
    }

  return 0;
}

int
Manager::set_properties (TAO_IOP::TAO_IOR_Manipulation_ptr iorm,
                         CORBA::Object_ptr merged_set,
                         CORBA::Object_ptr primary,
                         CORBA::ULong const version)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = static_cast<CORBA::Octet> (1);
  ft_tag_component.component_version.minor = static_cast<CORBA::Octet> (0);

  // Domain id
  constexpr char id[] {"iogr_testing"};
  ft_tag_component.group_domain_id = id;

  // Object group id
  ft_tag_component.object_group_id = 10;

  // Version
  ft_tag_component.object_group_ref_version = version;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean retval = iorm->set_primary (&iogr_prop,
                                             primary,
                                             merged_set);

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval)
    {
      retval = iorm->set_property (&iogr_prop,
                                    merged_set);
    }

  return 0;
}

int
Manager::write_to_files (Client_Options const &options)
{
  if (this->write_to_file (merged_set_10_.in (), options.first_iogr_file) != 0 ||
      this->write_to_file (merged_set_11_.in (), options.second_iogr_file) != 0)
    {
      return -1;
    }
  return 0;
}

int
Manager::write_to_file (CORBA::Object_ptr ior, ACE_TCHAR const *ior_output_file)
{
  CORBA::String_var const iorref =
    this->orb_->object_to_string (ior);

  if (ior_output_file != nullptr)
    {
      std::unique_ptr<FILE, decltype (&ACE_OS::fclose)> const output_file (
        ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           ior_output_file),
                          1);
      if (std::fprintf (output_file.get (), "%s", iorref.in ()) < 0)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), 1);
    }

  return 0;
}

CORBA::ORB_ptr
Manager::orb () const
{
  return this->orb_.in ();
}

Client_i::Client_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
run_remote_test (Simple_Server_ptr server, ACE_TCHAR const *execute_key)
{
  std::string expected {};
  if (execute_key != nullptr)
    {
      expected = "remote_call() completed by ";
      expected += ACE_TEXT_ALWAYS_CHAR (execute_key);
    }
  else
    {
      expected = "CORBA::TRANSIENT";
    }

  std::string received {};
  try
    {
      // Make a remote call
      CORBA::String_var const s =
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

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("... OK\n")));
  return 0;
}

int run_abort_test (Simple_Server_ptr server,
                    ACE_TCHAR const *request_key,
                    ACE_TCHAR const *execute_key)
{
  std::string expected {};
  if (execute_key != nullptr)
    {
      expected = "abort() completed by ";
      expected += ACE_TEXT_ALWAYS_CHAR (execute_key);
      expected += ", still_alive=1";
    }
  else
    {
      expected = "CORBA::TRANSIENT";
    }

  std::string received {};
  try
    {
      CORBA::String_var const s = server->abort (ACE_TEXT_ALWAYS_CHAR (request_key));

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

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("... OK\n")));
  return 0;
}

int
run_test (Simple_Server_ptr server, Client_Options const &options)
{
  int rv = run_abort_test (server, options.first_key, options.second_key);

  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, options.second_key);

  if (rv)
    {
      return rv;
    }

  rv = run_abort_test (server, options.second_key, nullptr);

  return rv;
}

int
Client_i::init (Client_Options const &options)
{
  std::string iogrstr {"file://"};
  iogrstr += ACE_TEXT_ALWAYS_CHAR (options.first_iogr_file);

  CORBA::Object_var const object =
    this->orb_->string_to_object (iogrstr.c_str ());

  Simple_Server_var const server =
    Simple_Server::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Object reference <%C> is nil\n", iogrstr.c_str ()),
                        -1);
    }

  return run_test (server.in (), options);
}
