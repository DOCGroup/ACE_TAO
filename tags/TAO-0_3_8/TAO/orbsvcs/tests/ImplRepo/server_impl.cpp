// $Id$

#include "server_impl.h"
#include "../../ImplRepo_Service/ImplRepoC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, server_impl, "$Id$")

Server_i::Server_i (void)
  : ior_output_file_ (0),
    register_ (0),
    use_ir_ (1)
{
}

int
Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:f:ri");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'r':
        this->register_ = 1;
        break;
      case 'i':
        this->use_ir_ = 0;
        this->server_impl.use_ir_ = 0;
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
        // @@ Update me.
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
      if (this->orb_manager_.init_child_poa (argc, argv, "simpserv", TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init_child_poa"), -1);

      TAO_CHECK_ENV;
    
      this->argc_ = argc;
      this->argv_ = argv;

      int retval = this->parse_args ();

      if (retval != 0)
        return retval;
   
      CORBA::String_var server_str  =
        this->orb_manager_.activate_under_child_poa ("server",
                                                     &this->server_impl,
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var ping_str =
        this->orb_manager_.activate_under_child_poa ("ping",
                                                     &this->ping_impl,
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->use_ir_)
        {
          this->read_ir_ior ();

          if (this->register_ == 1)
            this->register_with_ir ();

          CORBA::Object_var server_object =  
            this->orb_manager_.orb ()->string_to_object (server_str, env);

          Implementation_Repository::INET_Addr server_addr;
          ACE_INET_Addr my_addr = TAO_ORB_Core_instance ()->addr ();
          server_addr.port_ = my_addr.get_port_number ();

          CORBA::Object_var ping_object = 
            this->orb_manager_.orb ()->string_to_object (ping_str, env);

          // Talk to the Implementation Repository if we are already registered
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
          

          Implementation_Repository::INET_Addr new_addr = 
            ImplRepo->server_is_running ("simpserv", 
                                         server_addr,
                                         ping_object, 
                                         TAO_TRY_ENV);
          TAO_CHECK_ENV;

          IIOP_Object *iiop_obj = ACE_dynamic_cast (IIOP_Object *, server_object->_stubobj ());
          // @@ Only same host for now
          iiop_obj->profile.port = new_addr.port_;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "The IOR is: <%s>\n", this->orb_manager_.orb ()->object_to_string (server_object, TAO_TRY_ENV)));

          if (this->ior_output_file_)
            {
              ACE_OS::fprintf (this->ior_output_file_, "%s", this->orb_manager_.orb ()->object_to_string (server_object, TAO_TRY_ENV));
              ACE_OS::fclose (this->ior_output_file_);
            }
        }
      else
      {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "The IOR is: <%s>\n", server_str.in()));

          if (this->ior_output_file_)
            {
              ACE_OS::fprintf (this->ior_output_file_, "%s", server_str.in());
              ACE_OS::fclose (this->ior_output_file_);
            }
      }

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
  if (this->register_ != 1)
    if (this->orb_manager_.run (env) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "Server_i::run"), -1);
  return 0;
}

int 
Server_i::register_with_ir (void)
{
  TAO_TRY 
  {
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

    Implementation_Repository::Process_Options proc_opts;

    proc_opts.command_line_ = CORBA::string_dup ("server -o svr.ior -ORBobjrefstyle url");
    proc_opts.environment_ = CORBA::string_dup ("");
    proc_opts.working_directory_ = CORBA::string_dup ("");

    ImplRepo->register_server ("simpserv", proc_opts, TAO_TRY_ENV);
    TAO_CHECK_ENV;
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Server_i::register_with_ir");
    return -1;
  }
  TAO_ENDTRY;
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
