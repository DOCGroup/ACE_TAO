// $Id$

#include "server_impl.h"
#include "Impl_RepoC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, server_impl, "$Id$")

Server_i::Server_i (void)
  : ior_output_file_ (0)
{
}

int
Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:f:");
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

int
Server_i::init (int argc, char** argv, CORBA::Environment& env)
{
  TAO_TRY 
    {
      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.
      if (this->orb_manager_.init_child_poa (argc, argv, "child_poa", TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init_child_poa"), -1);

      TAO_CHECK_ENV;
    
      this->argc_ = argc;
      this->argv_ = argv;

      int retval = this->parse_args ();

      if (retval != 0)
        return retval;
   
      CORBA::String_var str  =
        this->orb_manager_.activate_under_child_poa ("server",
                                                 &this->server_impl,
                                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "The IOR is: <%s>\n", str.in ()));

      // Talk to the Implementation Repository
      
      this->read_ir_ior ();

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }


      CORBA::Object_var implrepo_object =
        this->orb_manager_.orb ()->string_to_object (this->ir_server_key_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      Implementation_Repository *ImplRepo = 
        Implementation_Repository::_narrow (implrepo_object.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (implrepo_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid implrepo key <%s>\n",
                           this->ir_server_key_),
                          -1);

      Implementation_Repository::INET_Addr addr;
      ImplRepo->server_is_running ("Simple_Server", addr, TAO_TRY_ENV);

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Server_i::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
Server_i::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Server_i::run"), -1);
  return 0;
}

Server_i::~Server_i (void)
{
}

int
Server_i::read_ir_ior (void)
{
  // Open the file for reading.
  ACE_HANDLE f_handle_ = ACE_OS::open ("implrepo.ior", 0);

  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: implrepo.ior\n"),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle_);
  this->ir_server_key_ = ior_buffer.read ();

  if (this->ir_server_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);

  ACE_OS::close (f_handle_);
  return 0;
}
