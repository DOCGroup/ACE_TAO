// $Id$
#include "client.h"
#include "Client_i.h"
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "ace/OS_NS_stdio.h"

// Files which have the IOR
const char *first_ior = 0;
const char *first_key = 0;
const char *second_ior = 0;
const char *second_key = 0;
const char *first_iogr_file = 0;
const char *second_iogr_file = 0;

// Objects
CORBA::Object_var object_primary = 0;
CORBA::Object_var object_secondary = 0;

// Reference to the IOR manipulator

int
parse_args (int argc, char* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "a:k:b:l:g:h:");
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
      case 'g':
        first_iogr_file = get_opts.opt_arg ();
        break;
      case 'h':
        second_iogr_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <iorfile1> -k <key1> "
                           "-b <iorfile2> -l <key2> "
                           "-g <iogr v10 output file> "
                           "-h <iogr v11 output file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


int
main (int argc,
      char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  Manager manager;

  ACE_TRY
    {
      // Initilaize the ORB, POA etc.
      manager.init (argc,
                    argv
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // the command line arguments
      if (parse_args (argc, argv) == -1)
        return -1;

      // Merge the different IORS
      manager.make_merged_iors (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Write IOR to file
      if (manager.write_to_files ())
	      return -1;

      // Client, who is going to use the merged IOR
      // Construct that with the managers ORB
      Client_i client_imp (manager.orb ());
      return client_imp.init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

Manager::Manager (void)
{
  //no-op
}

void
Manager::init (int& argc,
               char**& argv
               ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                0
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Obtain the RootPOA.
  CORBA::Object_var obj_var =
    this->orb_->resolve_initial_references ("RootPOA"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa_var =
    PortableServer::POA::_narrow (obj_var.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager_var =
    root_poa_var->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager_var->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
Manager::make_merged_iors (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get an object reference for the ORBs IORManipultion object!
  CORBA::Object_var IORM =
	  this->orb_->resolve_initial_references (
		  TAO_OBJID_IORMANIPULATION, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  if (CORBA::is_nil (IORM.in()))
	  return -1;

  TAO_IOP::TAO_IOR_Manipulation_var iorm =
	  TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (iorm.in()))
	  return -1;

  CORBA::Object_var first = orb_->string_to_object (first_ior ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (first.in()))
	  return -1;

  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (1);
  iors.length(1);
  iors [0] = first;

  merged_set_10_ = iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (merged_set_10_.in()))
    {
      return -1;
    }

  int result = set_properties (iorm.in(), merged_set_10_.in(),
			    first.in(), 10 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (-1 == result)
    {
      return -1;
    }
  
  first = orb_->string_to_object (first_ior);
  if (CORBA::is_nil (first.in()))
    {
      return -1;
    }

  CORBA::Object_var second = orb_->string_to_object (second_ior);
  
  if (CORBA::is_nil (second.in()))
    {
      return -1;
    }

  // Create the list
  
  iors.length(2);
  iors [0] = first;
  iors [1] = second;

  merged_set_11_ = iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (merged_set_11_.in()))
    {
      return -1;
    }

  result = set_properties (iorm.in(), merged_set_11_.in(),
			    first.in(), 11 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (-1 == result)
    {
      return -1;
    }

  return 0;
}

int
Manager::set_properties (TAO_IOP::TAO_IOR_Manipulation_ptr iorm,
			 CORBA::Object_ptr merged_set,
			 CORBA::Object_ptr primary,
			 CORBA::ULong version
                         ACE_ENV_ARG_DECL)
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
  ft_tag_component.object_group_id = 10;

  // Version
  ft_tag_component.object_group_ref_version = version;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean retval = iorm->set_primary (&iogr_prop,
                                             primary,
					     merged_set
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval != 0)
    {
      retval = iorm->set_property (&iogr_prop,
                                    merged_set
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

int
Manager::run (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Error in run \n"),
                        -1);
    }
  ACE_ENDTRY;

  return 0;
}

int Manager::write_to_files ()
{
  if (write_to_file (merged_set_10_.in(), first_iogr_file) ||
      write_to_file (merged_set_11_.in(), second_iogr_file))
    {
      return -1;
    }
  return 0;
}

int
Manager::write_to_file (CORBA::Object_ptr ior, const char* ior_output_file)
{
  CORBA::String_var iorref =
    this->orb_->object_to_string (ior);

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

int run_remote_test (Simple_Server_ptr server, const char* execute_key ACE_ENV_ARG_DECL)
{
  char expected[1024], received[1024];
  if (execute_key)
    {
      ACE_OS::sprintf (expected,
		      "remote_call() completed by %s", execute_key);
    }
  else
    {
      ACE_OS::strcpy (expected, "CORBA::TRANSIENT");
    }

  ACE_TRY
    {
      // Make a remote call
      CORBA::String_var s =
	      server->remote_call (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!s.in())
        {
	  ACE_OS::strcpy (received, "(null)");
        }
      else
        {
	  ACE_OS::strcpy (received, s.in());
        }
    }
  ACE_CATCH (CORBA::TRANSIENT, te)
    {
      ACE_OS::strcpy (received, "CORBA::TRANSIENT");
    }
  ACE_CATCH (CORBA::COMM_FAILURE, cf)
    {
      ACE_OS::strcpy (received, "CORBA::COMM_FAILURE");
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
	      ACE_TEXT("CLIENT> REMOTE   expected: '%s'"),
		       expected));
  if (ACE_OS::strcmp (expected, received))
    {
      ACE_DEBUG ((LM_DEBUG,
		  ACE_TEXT ("... FAIL\n"
			    "CLIENT>          received: '%s'\n"),
		  received));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int run_abort_test (Simple_Server_ptr server,
		    const char* request_key, const char* execute_key ACE_ENV_ARG_DECL)
{
  char expected[1024], received[1024];
  if (execute_key)
    {
      ACE_OS::sprintf (expected,
		       "abort() completed by %s, still_alive=1",
		        execute_key);
    }
  else
    {
      ACE_OS::strcpy (expected, "CORBA::TRANSIENT");
    }

  ACE_TRY
    {
      CORBA::String_var s = server->abort (request_key);
      ACE_TRY_CHECK;

      if (!s.in())
        {
	  ACE_OS::strcpy (received, "(null)");
        }
      else
        {
	  ACE_OS::strcpy (received, s.in());
        }
    }
  ACE_CATCH (CORBA::TRANSIENT, te)
    {
      ACE_OS::strcpy (received, "CORBA::TRANSIENT");
    }
  ACE_CATCH (CORBA::COMM_FAILURE, cf)
    {
      ACE_OS::strcpy (received, "CORBA::COMM_FAILURE");
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
	      ACE_TEXT("CLIENT> ABORT    expected: '%s'"),
		       expected));

  if (ACE_OS::strcmp (expected, received))
  {
    ACE_DEBUG ((LM_DEBUG,
	        ACE_TEXT ("... FAIL\n"
			  "CLIENT>          received: '%s'\n"),
	        received));
    return -1;
  }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("... OK\n")));
  return 0;
}

int run_test (Simple_Server_ptr server ACE_ENV_ARG_DECL)
{
  int rv = run_abort_test (server, first_key, second_key ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  if (rv)
    {
      return rv;
    }
  
  rv = run_remote_test (server, second_key ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (rv)
    {
      return rv;
    }
  
  rv = run_abort_test (server, second_key, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return rv;
}

int
Client_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  char iogrstr[1024];
  ACE_OS::sprintf (iogrstr, "file://%s", first_iogr_file);

  int argc = 0;
  char **argv = 0;
  this->orb_ = CORBA::ORB_init (argc,
			        argv,
			        0
			        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var object =
	  this->orb_->string_to_object (iogrstr
				        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  // Combined IOR stuff
  Simple_Server_var server =
	  Simple_Server::_narrow (object.in ()
				  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (server.in ()))
  {
	  ACE_ERROR ((LM_ERROR,
		      "Object reference <%s> is nil\n",
		      iogrstr));
  }

  // run tests
  int result = run_test (server.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return result;
}
