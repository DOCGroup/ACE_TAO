// $Id$

// This version of Simple_Util doesn't need to link against orbsvcs
// On the other hand it has no naming service.

#ifndef SIMPLE_UTIL_C
#define SIMPLE_UTIL_C
# include "Simple_util.h"
# include "tao/debug.h"

// Constructor.

template <class Servant>
Server<Servant>::Server (void)
    : ior_output_file_ (0)
{
  // no-op.
}

// Destructor.

template <class Servant>
Server<Servant>::~Server (void)
{
}

// Parse the command-line arguments and set options.

template <class Servant> int
Server<Servant>::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:ni:");
  int c = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Initialize the server.
template <class Servant> int
Server<Servant>::init (const char *servant_name,
                       int argc,
                       char *argv[]
                       ACE_ENV_ARG_DECL)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa"
                                         ACE_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);

  ACE_CHECK_RETURN (-1);

  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_.orb (orb.in ());

  // Activate the servant in its own child POA.

  // Make sure that you check for failures here via the ACE_TRY
  // macros?!
  ACE_TRY
    {
      CORBA::String_var str  =
        this->orb_manager_.activate_under_child_poa (servant_name,
                                                     &this->servant_
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in activation of POA");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

template <class Servant>int
Server<Servant>::run (ACE_ENV_SINGLE_ARG_DECL)
{
    // Run the main event loop for the ORB.
  if (this->orb_manager_.run (ACE_ENV_SINGLE_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);

  return 0;
}

template <class Servant> int
Server<Servant>::register_name (void)
{
  return -1;
}

// Constructor.

template <class InterfaceObj, class Var>
Client<InterfaceObj, Var>::Client (void)
  : ior_ (0)
{
  //no-op
}

// Reads the Server ior from a file

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n"),
                      -1);

  this->ior_ = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "df:nk:x");
  int c = 0;
  int result = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
        break;
      case 'x': // read the flag for shutting down
        this->shutdown_ = 1;
        break;
      }

  // Indicates successful parsing of command line.
  return 0;
}

template <class InterfaceObj, class Var>
Client<InterfaceObj, Var>::~Client (void)
{
  ACE_OS::free (this->ior_);
}

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::init (const char *name,
                                 int argc,
                                 char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;


  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    name
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;



      if(this->ior_ != 0)
        {
          CORBA::Object_var server_object =
            this->orb_->string_to_object (this->ior_ ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;


          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%s>\n",
                               this->ior_),
                              -1);
          this->server_ = InterfaceObj::_narrow (server_object.in ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "no ior or naming options  specified\n"),
                          -1);


    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Client_i::init");
      return -1;
    }
  ACE_ENDTRY;


  return 0;
}


template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::obtain_initial_references (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{

  return 0;
}

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::shutdown (void )
{
  // Returns the shutdwon flag
  return shutdown_;
}

template <class InterfaceObj, class Var> void
Client<InterfaceObj, Var>::shutdown (int flag)
{
  // Fills the flag
  shutdown_ = flag;
}

#endif
