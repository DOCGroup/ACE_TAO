// $Id$

#include "ir_implrepo_impl.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/Process.h"

ACE_RCSID(ImplRepo, ir_implrepo_impl, "$Id$")

IR_iRepo_i::IR_iRepo_i (void)
  : ior_output_file_ (0),
    server_key_ (0),
    server_impl_ (0) 
{
}

void IR_iRepo_i::start (const char *server)
{
  if (ACE_OS::strcmp (server, "Simple_Server") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Starting Simple_Server!\n"));
      ACE_Process_Options proc_opts;
      
      proc_opts.command_line ("server -o server.ior");

      ACE_Process proc;

      proc.spawn (proc_opts);

      ACE_OS::sleep (2);
    }
}

void IR_iRepo_i::server_is_running (const char *server,
                                          const Implementation_Repository::INET_Addr &addr,
                                          CORBA::Environment &_tao_environment)
{
  ACE_DEBUG ((LM_DEBUG, "Server is running!\n"));
  int result = this->read_ior (this->server_input_file_);
  if (result < 0)
    { 
      ACE_ERROR ((LM_ERROR,
                 "Unable to read ior from %s : %p\n",
                 this->server_input_file_));
      return;
    }
  CORBA::Object_var server_object =
    this->orb_manager_.orb ()->string_to_object (this->server_key_, _tao_environment);
  this->server_impl_->forward_to (server_object.in ());
}


// Reads the Server factory ior from a file

int
IR_iRepo_i::read_ior (char *filename)
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

  ACE_OS::close (f_handle_);
  return 0;
}

int
IR_iRepo_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "df:o:");
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
      case 'f': // read the IOR from the file.
        this->server_input_file_ = ACE_OS::strnew (get_opts.optarg);
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
IR_iRepo_i::init (int argc, char** argv, CORBA::Environment& env)
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

  this->server_impl_ = new IR_Simple_i (this->orb_manager_.orb (),
                                           this->orb_manager_.child_poa (),
                                           this);

  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("server",
                                                 this->server_impl_,
                                                 env);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "The server IOR is: <%s>\n", str.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  CORBA::String_var ir_var  =
    this->orb_manager_.activate_under_child_poa ("implrepo",
                                                 this,
                                                 env);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "The IR IOR is: <%s>\n", ir_var.in ()));
  
  FILE *ir_file = ACE_OS::fopen ("implrepo.ior", "w");
  ACE_OS::fprintf (ir_file, "%s", ir_var.in ());
  ACE_OS::fclose (ir_file);

  return 0;
}

int
IR_iRepo_i::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "IR_Server_i::run"), -1);
  return 0;
}

IR_iRepo_i::~IR_iRepo_i (void)
{
  if (this->server_impl_ != 0)
    delete server_impl_;
}


// Constructor
IR_Simple_i::IR_Simple_i (CORBA::ORB_ptr orb_ptr,
                                PortableServer::POA_ptr poa_ptr,
                                IR_iRepo_i *ir_impl)
  : ir_impl_ (ir_impl),
    orb_var_ (CORBA::ORB::_duplicate (orb_ptr)),
    poa_var_ (PortableServer::POA::_duplicate (poa_ptr)),
    forward_to_var_ (CORBA::Object::_nil ())
{
}

// Destructor

IR_Simple_i::~IR_Simple_i (void)
{
}

// Cube a long

CORBA::Long
IR_Simple_i::simple_method (CORBA::Long l, CORBA::Environment &env)
{
  return this->forward (env);
}

// Shutdown.

void IR_Simple_i::shutdown (CORBA::Environment &env)
{
  this->forward (env);
}

int IR_Simple_i::forward (CORBA::Environment &env)
{
  this->ir_impl_->start ("Simple_Server");

  if (!CORBA::is_nil (this->forward_to_var_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "Forwarding...\n"));
    
      PortableServer::ObjectId_var oid =
        this->poa_var_->servant_to_id (this, env);
      
      if (env.exception () != 0)
        return -1;

      PortableServer::Servant servant = this->poa_var_->_servant ();
      if (servant == 0)
        {
//          CORBA::Exception *exception = new Foo::Cannot_Forward;
//          env.exception (exception);
          return -1;
        }
      
      void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
      POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
      TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);
      
      tao_poa->forward_object (oid.in (),
                               this->forward_to_var_.in (),
                               env);
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Forward_to reference is nil.\n"));
//      CORBA::Exception *exception = new Foo::Cannot_Forward;
//      env.exception (exception);
      return -1;
    }
}

void IR_Simple_i::forward_to (CORBA::Object_ptr forward_to_ptr)
{
  this->forward_to_var_ = CORBA::Object::_duplicate (forward_to_ptr);
}
