// $Id$

#include "Server_i.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Time, Server_i, "$Id$")

// Constructor.

Server_i::Server_i (void)
  : ior_output_file_ (0)
{
  // no-op.
}

// Destructor.

Server_i::~Server_i (void)
{
  // no-op.
}

// Parse the command-line arguments and set options.

int
Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Initialize the server.

int
Server_i::init (int argc, char *argv[], CORBA::Environment &env)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;
   
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_.orb (orb.in ());

  // Activate the servant in its own child POA.
  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("time",
                                                 this->servant_,
                                                 env);
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

  return 0;
}

int
Server_i::run (CORBA::Environment &env)
{
  // Run the main event loop for the ORB.
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);

  return 0;
}

