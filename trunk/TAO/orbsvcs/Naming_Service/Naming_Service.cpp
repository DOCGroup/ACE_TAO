// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs/Naming_Service/Naming_Service
//
// = FILENAME
//    Naming_Service.cpp
//
// = DESCRIPTION
//      This class implements a Naming_Service object.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#include "Naming_Service.h"

// Default Constructor.

Naming_Service::Naming_Service (void)
  : ior_output_file_ (0),
    pid_file_name_ (0)
{
}

// Constructor taking command-line arguments.

Naming_Service::Naming_Service (int argc,
                                char* argv[])
  : ior_output_file_ (0),
    pid_file_name_ (0)
{
  this->init (argc, argv);
}

int
Naming_Service::parse_args (int argc,
                            char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o': // outputs the naming service ior to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 'p':
	this->pid_file_name_ = get_opts.optarg;
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-NScontextname <contextname> "
                           "-o <ior_output_file> "
			   "-p <pid_file_name> "
			   "\n",
                           argv [0]),
                          -1);
        break;
      }
  return 0;
}

// Initialize the state of the Naming_Service object
int
Naming_Service::init (int argc,
                      char* argv[])
{
  int result;
  CORBA::ORB_var orb;
  PortableServer::POA_var child_poa;

  TAO_TRY
    {
      this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      orb = this->orb_manager_.orb ();
      child_poa = this->orb_manager_.child_poa ();

      result = this->my_naming_server_.init (orb.in (),
                                             child_poa,
					     argc,
					     argv);
      TAO_CHECK_ENV;
      if (result < 0)
        return result;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Naming_Service::init");
      return -1;
    }
  TAO_ENDTRY;

  // Check the non-ORB arguments.
  result = this->parse_args (argc,
                             argv);

  if (result < 0)
    return result;
  if (this->ior_output_file_ != 0)
    {
      CORBA::String_var str =
        this->my_naming_server_.naming_service_ior ();
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->pid_file_name_ != 0)
    {
      FILE* pidf = fopen (this->pid_file_name_, "w");
      if (pidf != 0)
	{
	  ACE_OS::fprintf (pidf, "%d\n", ACE_OS::getpid ());
	  ACE_OS::fclose (pidf);
	}
    }
  return 0;
}

// Run the ORB event loop

int
Naming_Service::run (CORBA_Environment& env)
{
  return this->orb_manager_.run (env);
}

// Destructor.

Naming_Service::~Naming_Service (void)
{
}

int
main (int argc, char* argv[])
{
  int init_result;

  Naming_Service naming_service;

  init_result = naming_service.init (argc,argv);
  if (init_result < 0)
    return init_result;

  TAO_TRY
    {
      naming_service.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NamingService");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
