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
#include "ace/OS_NS_unistd.h"
#include <memory>

struct IOGR_IOR_Buffer_Deleter
{
  ACE_Allocator *allocator;

  void operator() (char *data) const
  {
    this->allocator->free (data);
  }
};

static int
parse_args (int argc, ACE_TCHAR *argv[], IOGR_Options& options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:c:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.first_ior = get_opts.opt_arg ();
        break;
      case 'b':
        options.second_ior = get_opts.opt_arg ();
        break;
      case 'c':
        options.output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                           "-a <iorfile>"
                           "-b <iorfile>"
                           "-c <output ior file>"
                           "\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Manager manager;
  IOGR_Options options;

  try
    {
      // Initialize the ORB, POA etc.
      manager.init (argc, argv);

      // the command line arguments
      if (parse_args (argc, argv, options) == -1)
        return -1;

      if (options.first_ior == nullptr || options.second_ior == nullptr
          || options.output_file == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Options -a, -b, and -c are required\n")),
                          -1);

      // Merge the different IORS
      manager.make_merged_iors (options);

      // Set properties. This is the most important portion of the
      // test
      manager.set_properties ();

      // Write IOR to file
      manager.write_to_file (options.output_file);

      // Client, who is going to use the merged IOR
      // Construct that with the managers ORB
      Client_i client_imp (manager.orb ());
      if (client_imp.init (options.output_file) != 0)
        return -1;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Caught");
      return -1;
    }

  return 0;
}

void
Manager::init (int argc, ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  // Obtain the RootPOA.
  CORBA::Object_var obj_var =
    this->orb_->resolve_initial_references ("RootPOA");

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa_var =
    PortableServer::POA::_narrow (obj_var.in ());

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager_var =
    root_poa_var->the_POAManager ();

  poa_manager_var->activate ();
}

int
Manager::make_merged_iors (IOGR_Options const& options)
{
  // First  server
  this->object_primary_ =
    this->orb_->string_to_object (options.first_ior);

  // Second server
  this->object_secondary_ =
    this->orb_->string_to_object (options.second_ior);

  // Get an object reference for the ORB's IORManipulation object!
  CORBA::Object_var iorm_object =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                            nullptr);

  TAO_IOP::TAO_IOR_Manipulation_var iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());
  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
  iors.length (2);
  iors [0] = CORBA::Object::_duplicate (this->object_primary_.in ());
  iors [1] = CORBA::Object::_duplicate (this->object_secondary_.in ());

  // Create a merged set.
  this->merged_set_ =
    iorm->merge_iors (iors);

  return 0;
}

int
Manager::set_properties ()
{
  CORBA::Object_var iorm_object =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                            nullptr);
  TAO_IOP::TAO_IOR_Manipulation_var iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

  FT::TagFTGroupTaggedComponent ft_tag_component {};

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = static_cast<CORBA::Octet> (1);
  ft_tag_component.component_version.minor = static_cast<CORBA::Octet> (0);

  // Domain id
  char const* id = "iogr_testing";
  ft_tag_component.group_domain_id = id;

  // Object group id
  ft_tag_component.object_group_id = static_cast<CORBA::ULongLong> (10);

  // Version
  ft_tag_component.object_group_ref_version = static_cast<CORBA::ULong> (5);

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean retval = iorm->set_property (&iogr_prop,
                                              this->merged_set_.in ());

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval)
    {
      retval = iorm->set_primary (&iogr_prop,
                                  this->object_secondary_.in (),
                                  this->merged_set_.in ());
    }

  return 0;
}

int
Manager::write_to_file (ACE_TCHAR const* output_file_name)
{
  CORBA::String_var iorref =
    this->orb_->object_to_string (this->merged_set_.in ());

  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (output_file_name, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                       output_file_name),
                      1);
  ACE_OS::fprintf (output_file.get (), "%s", iorref.in ());

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

static void
run_test (Simple_Server_ptr server);

int
Client_i::init (ACE_TCHAR const* ior_file)
{
  FILE *input_file = ACE_OS::fopen (ior_file, "r");
  if (input_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to open %s for reading: %p\n"),
                       ior_file),
                      -1);

  ACE_Read_Buffer ior_buffer (input_file, true);
  std::unique_ptr<char, IOGR_IOR_Buffer_Deleter> const data (
    ior_buffer.read (), IOGR_IOR_Buffer_Deleter {ior_buffer.alloc ()});
  if (!data)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to read IOR from %s\n"), ior_file), -1);

  CORBA::Object_var object =
    this->orb_->string_to_object (data.get ());

  // Combined IOR stuff
  Simple_Server_var server =
    Simple_Server::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Object reference <%C> is nil\n"),
                       data.get ()),
                      -1);

  run_test (server.in ());

  return 0;
}

static void
run_test (Simple_Server_ptr server)
{
  // We do this twice as we know that there are only two servers.
  for (CORBA::ULong i {}; i < 2; ++i)
    {
      try
        {
          for (CORBA::ULong j {}; j < 10; ++j)
            {
              // Make a remote call
              server->remote_call ();
            }

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("*********************************\n")));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("I am going to shutdown the server\n")));
          server->shutdown ();
          ACE_OS::sleep (2);
        }
      catch (CORBA::TRANSIENT const& t)
        {
          if (t.completed () != CORBA::COMPLETED_NO)
            {
              t._tao_print_exception ("Unexpected kind of TRANSIENT");
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("The completed status %d\n"), t.completed ()));
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Automagically re-issuing request on TRANSIENT\n")));
              ACE_OS::sleep (1);
            }
        }
      catch (CORBA::COMM_FAILURE const& f)
        {
          f._tao_print_exception ("A (sort of) expected COMM_FAILURE");
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Automagically re-issuing request on COMM_FAILURE\n")));
        }
      catch (CORBA::Exception const& ex)
        {
          ex._tao_print_exception ("Unexpected exception");
          throw;
        }
    }
}
