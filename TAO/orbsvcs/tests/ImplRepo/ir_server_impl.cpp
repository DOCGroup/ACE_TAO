// $Id$

#include "ir_server_impl.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"


IR_Server_Impl::IR_Server_Impl (void)
  : ior_output_file_ (0),
    server_key_ (0),
    server_impl_ (0) 
{
}

// Reads the Server factory ior from a file

int
IR_Server_Impl::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle_ = ACE_OS::open (filename, 0);

  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle_);
  this->server_key_ = ior_buffer.read ();

  if (this->server_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);

  ACE_OS::close (filename);
  return 0;
}

int
IR_Server_Impl::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "df:o:");
  int c;
  int result;

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
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
            break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f] <server_ior_file>"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
IR_Server_Impl::init (int argc, char** argv, CORBA::Environment& env)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc, argv, "child_poa", env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init_child_poa"), -1);

  TAO_CHECK_ENV_RETURN (env, -1);
  
  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::Object_var server_object =
    this->orb_manager_.orb ()->string_to_object (this->server_key_, env);
/*
  this->server_ = simple_object::_narrow (server_object.in(), env);

  if (CORBA::is_nil (server_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "invalid server key <%s>\n",
                       this->server_key_),
                      -1);
*/

  this->server_impl_ = new IR_Simple_Impl (this->orb_manager_.orb (),
                                           this->orb_manager_.child_poa (),
                                           server_object.in ());

  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("server",
                                                 this->server_impl_,
                                                 env);
  ACE_DEBUG ((LM_DEBUG, "The IOR is: <%s>\n", str.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  return 0;
}

int
IR_Server_Impl::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "IR_Server_Impl::run"), -1);
  return 0;
}

IR_Server_Impl::~IR_Server_Impl (void)
{
  if (this->server_impl_ != 0)
    delete server_impl_;
}
