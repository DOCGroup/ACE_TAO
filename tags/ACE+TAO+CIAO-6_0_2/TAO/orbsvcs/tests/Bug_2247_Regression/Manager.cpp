// $Id$
#include "Manager.h"
#include "Client_i.h"
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/PortableServer/PortableServer.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "ace/OS_NS_stdio.h"

// Files which have the IOR
const ACE_TCHAR *first_ior = 0;
const ACE_TCHAR *first_key = 0;
const ACE_TCHAR *second_ior = 0;
const ACE_TCHAR *second_key = 0;
const ACE_TCHAR *ior_output_file = 0;
int shutdown_test = 0;
int merged_test = 0;

// Objects
CORBA::Object_var object_primary = 0;
CORBA::Object_var object_secondary = 0;

// Reference to the IOR manipulator
TAO_IOP::TAO_IOR_Manipulation_var iorm = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:k:b:l:c:sm"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        first_ior = get_opts.opt_arg ();
        break;
      case 'k':
    first_key = get_opts.opt_arg ();
    break;
      case 'b':
        second_ior = get_opts.opt_arg ();
        break;
      case 'l':
    second_key = get_opts.opt_arg ();
    break;
      case 'c':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 's':
    shutdown_test = 1;
    break;
      case 'm':
    merged_test = 1;
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
  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN (int argc,
      ACE_TCHAR *argv[])
{

  Manager manager;

  try
    {
      // Initilaize the ORB, POA etc.
      manager.init (argc, argv);

      // the command line arguments
      if (parse_args (argc, argv) == -1)
        return -1;

      // Merge the different IORS
      manager.make_merged_iors ();

      if (merged_test)
        {
          // Set properties. This is the most important portion of the
          // test
          manager.set_properties ();
        }

      // Write IOR to file
      manager.write_to_file ();

      // Client, who is going to use the merged IOR
      // Construct that with the managers ORB
      Client_i client_imp (manager.orb ());
      return client_imp.init ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught");
      return -1;
    }

  return 0;
}

Manager::Manager (void)
  :orb_ (0),
   merged_set_ (0)
{
  //no-op
}

void
Manager::init (int argc,
               ACE_TCHAR *argv[])
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
Manager::make_merged_iors (void)
{
  // First  server
  object_primary =
    this->orb_->string_to_object (first_ior);

  if (merged_test)
    {
      //Second server
      object_secondary =
        this->orb_->string_to_object (second_ior);

      // Get an object reference for the ORBs IORManipultion object!
      CORBA::Object_var IORM =
        this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                              0);

      iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in());


      // Create the list
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length(2);
      iors [0] = CORBA::Object::_duplicate (object_primary.in ());
      iors [1] = CORBA::Object::_duplicate (object_secondary.in ());

      // Create a merged set 1;
      merged_set_ =
        iorm->merge_iors (iors);
    }
  else
    {
      merged_set_ = object_primary;
    }

  return 0;
}

int
Manager::set_properties (void)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = (CORBA::Octet) 1;
  ft_tag_component.component_version.minor = (CORBA::Octet) 0;

  // Domain id
  const char *id = "iogr_testing";
  ft_tag_component.group_domain_id = id;

  // Object group id
  ft_tag_component.object_group_id =
    (CORBA::ULongLong) 10;

  // Version
  ft_tag_component.object_group_ref_version =
    (CORBA::ULong) 5;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
      CORBA::Boolean retval = iorm->set_primary (&iogr_prop,
                                  object_secondary.in (),
                                  this->merged_set_.in ());

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval != 0)
    {
      retval = iorm->set_property (&iogr_prop,
                           this->merged_set_.in ());
    }

  return 0;
}

int
Manager::run (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Error in run\n"),
                        -1);
    }

  return 0;
}

int
Manager::write_to_file (void)
{
  //
  CORBA::String_var iorref =
    this->orb_->object_to_string (this->merged_set_.in ());

  if (ior_output_file != 0)
    {
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", iorref.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

CORBA::ORB_ptr
Manager::orb (void)
{
  return this->orb_.in ();
}

Client_i::Client_i (CORBA::ORB_ptr orb)
  :orb_ (CORBA::ORB::_duplicate (orb))
{
}

int run_remote_test (Simple_Server_ptr server, const ACE_TCHAR* execute_key)
{
  char expected[1024], received[1024];
  if (execute_key)
    {
      ACE_OS::sprintf (expected,
               "remote_call() completed by %s", ACE_TEXT_ALWAYS_CHAR (execute_key));
    }
  else if (shutdown_test && !merged_test)
    {
      ACE_OS::strcpy (expected, "CORBA::COMM_FAILURE");
    }
  else
    {
      ACE_OS::strcpy (expected, "CORBA::TRANSIENT");
    }

  try
    {
      // Make a remote call
      CORBA::String_var s =
          server->remote_call ();

      if (!s.in())
        {
          ACE_OS::strcpy (received, "(null)");
        }
      else
        {
          ACE_OS::strcpy (received, s.in());
        }
    }
  catch (const CORBA::TRANSIENT&)
    {
      ACE_OS::strcpy (received, "CORBA::TRANSIENT");
    }
  catch (const CORBA::COMM_FAILURE&)
    {
      ACE_OS::strcpy (received, "CORBA::COMM_FAILURE");
    }

  ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT("CLIENT> REMOTE   expected: '%C'"),
               expected));

  if (ACE_OS::strcmp (expected, received))
  {
      ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("... FAIL\n")
              ACE_TEXT ("CLIENT>          received: '%C'\n"),
              received));
      return -1;
  }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int run_abort_test (Simple_Server_ptr server,
            const ACE_TCHAR* request_key, const ACE_TCHAR* execute_key)
{
  char expected[1024], received[1024];
  if (execute_key)
    {
      ACE_OS::sprintf (expected,
               "abort() completed by %s, still_alive=1",
               ACE_TEXT_ALWAYS_CHAR (execute_key));
    }
  else if (merged_test)
    {
      ACE_OS::strcpy (expected, "CORBA::TRANSIENT");
    }
  else
    {
      ACE_OS::strcpy (expected, "CORBA::COMM_FAILURE");
    }

  try
    {
      CORBA::String_var s = server->abort (ACE_TEXT_ALWAYS_CHAR (request_key));

      if (!s.in())
        {
          ACE_OS::strcpy (received, "(null)");
        }
      else
        {
          ACE_OS::strcpy (received, s.in());
        }
    }
  catch (const CORBA::TRANSIENT&)
    {
      ACE_OS::strcpy (received, "CORBA::TRANSIENT");
    }
  catch (const CORBA::COMM_FAILURE&)
    {
      ACE_OS::strcpy (received, "CORBA::COMM_FAILURE");
    }

  ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT("CLIENT> ABORT    expected: '%C'"),
               expected));
  if (ACE_OS::strcmp (expected, received))
    {
      ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("... FAIL\n")
              ACE_TEXT ("CLIENT>          received: '%C'\n"),
              received));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int run_shutdown_test (Simple_Server_ptr server,
            const ACE_TCHAR* request_key, const ACE_TCHAR* execute_key)
{
  char expected[1024], received[1024];
  if (execute_key)
    {
      ACE_OS::sprintf (expected,
               "shutdown() completed by %s, still_alive=0",
               ACE_TEXT_ALWAYS_CHAR (execute_key));
    }
  else
    {
      ACE_OS::strcpy (expected, "CORBA::TRANSIENT");
    }

  try
    {
      CORBA::String_var s = server->shutdown (ACE_TEXT_ALWAYS_CHAR (request_key));

      if (!s.in())
        {
          ACE_OS::strcpy (received, "(null)");
        }
      else
        {
          ACE_OS::strcpy (received, s.in());
        }
    }
  catch (const CORBA::TRANSIENT&)
    {
      ACE_OS::strcpy (received, "CORBA::TRANSIENT");
    }

  ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT("CLIENT> SHUTDOWN expected: '%C'"),
               expected));
  if (ACE_OS::strcmp (expected, received))
    {
      ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("... FAIL\n")
              ACE_TEXT ("CLIENT>          received: '%C'\n"),
              received));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int run_full_abort_test (Simple_Server_ptr server)
{
  int rv = run_remote_test (server, second_key);
  if (rv)
    {
      return rv;
    }

  rv = run_abort_test (server, second_key, first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_abort_test (server, first_key, 0);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, 0);
  if (rv)
    {
      return rv;
    }

  return 0;
}

int run_full_shutdown_test (Simple_Server_ptr server)
{
  int rv = run_remote_test (server, second_key);
  if (rv)
    {
      return rv;
    }

  rv = run_shutdown_test (server, second_key, second_key);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_shutdown_test (server, first_key, first_key);
  if (rv)
    {
      return rv;
    }

  rv = run_remote_test (server, 0);
  if (rv)
    {
      return rv;
    }

  return 0;
}

int
Client_i::init (void)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (ior_output_file,
                                      0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT("Unable to open %s for writing (%p)\n"),
                ACE_TEXT("open"),
                ior_output_file));

  ACE_Read_Buffer ior_buffer (f_handle);

  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR ((LM_ERROR,
                "Unable to read ior: %p\n"));


  int argc = 0;
  ACE_TCHAR **argv = 0;
  this->orb_ = CORBA::ORB_init (argc, argv);

  CORBA::Object_var object =
    this->orb_->string_to_object (data);

  // Combined IOR stuff
  Simple_Server_var server =
    Simple_Server::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Object reference <%C> is nil\n",
                  data));
    }

  int rv;
  if (!merged_test)
    {
      if (shutdown_test)
        {
          rv = run_shutdown_test (server.in(), first_key, first_key);
        }
      else
        {
          rv = run_abort_test (server.in(), first_key, 0);
        }

      if (!rv)
        {
          rv = run_remote_test (server.in(), 0);
        }
    }
  else if (shutdown_test)
    {
      rv = run_full_shutdown_test (server.in ());
    }
  else
    {
      rv = run_full_abort_test (server.in ());
    }

  ior_buffer.alloc ()->free (data);
  ACE_OS::close (f_handle);
  return rv;
}
