#include "Server_i.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Echo, Server_i, "$Id$")

// Constructor.

Server_i::Server_i (void)
  : ior_output_file_ (0),
    argc_ (0),
    argv_ (0),
    using_naming_service_ (1)
{
  // no-op.
}

// Destructor.

Server_i::~Server_i (void)
{
  // no-op.
}

// The naming service is initialized and the naming context as well as
// the object name is bound to the naming server.

int
Server_i::init_naming_service (CORBA::Environment& env)
{
  TAO_TRY
    {
      CORBA::ORB_var orb = this->orb_manager_.orb ();

      PortableServer::POA_var child_poa
	= this->orb_manager_.child_poa ();

      TAO_debug_level = 1;
      int return_val =
	this->naming_server_.init (orb.in (),
				   child_poa.in ());
      if (return_val == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "Failed to initialize TAO_Naming_Server\n"),
			  -1);

      // Register the object implementation with the POA.
      Echo_var echo_obj = this->servant_._this (env);
      TAO_CHECK_ENV_RETURN (env, -1);

      // Name the object.
      CosNaming::Name echo_obj_name (1);
      echo_obj_name.length (1);
      echo_obj_name[0].id = CORBA::string_dup ("Echo");

      TAO_CHECK_ENV_RETURN (env, -1);

      // Now, attach the object name to the context.
      this->naming_server_->bind (echo_obj_name,
				  echo_obj.in (),
				  env);
      TAO_CHECK_ENV_RETURN (env, -1);
    }
  TAO_CATCHANY
    {
      //  TAO_TRY_ENV.print_execption ("Server_i::init_naming_service\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Parse the command-line arguments and set options.
int
Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do: ");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
	 TAO_debug_level++;  ///*****
        break;
      case 'o':  // output the IOR toi a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                              get_opts.optarg),
			     -1);
        break;
      case 's': // don't use the naming service
	this->using_naming_service_ = 0;
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
			   " [-s]"
                           "\n",
                           argv_ [0]),
                          1);
      }
  // Indicates successful parsing of command line.
  return 0;
}

// Initialize the server.

int
Server_i::init (int argc,
		char *argv[],
		CORBA::Environment &TAO_TRY_ENV)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create the child poa under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
					 argv,
					 "child_poa",
					 TAO_TRY_ENV) == -1)
   ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_.orb (orb.in ());

  // Activate the servant in the POA.
  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("echo",
						 &this->servant_,
						 TAO_TRY_ENV);
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));
  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->using_naming_service_)
    return this->init_naming_service (TAO_TRY_ENV);
  return 0;
}

int
Server_i::run (CORBA::Environment &TAO_TRY_ENV)
{
  // Run the main event loop for the ORB.
  if (this->orb_manager_.run (TAO_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);
  return 0;
}
