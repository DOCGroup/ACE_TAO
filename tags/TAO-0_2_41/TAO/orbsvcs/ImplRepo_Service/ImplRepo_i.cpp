/* -*- C++ -*- */
// $Id$

#include "ImplRepo_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/Process.h"

ACE_RCSID(ImplRepo_Service, ImplRepo_i, "$Id$")

ImplRepo_i::ImplRepo_i (void)
  : ior_output_file_ (0),
    server_key_ (0),
    forwarder_impl_ (0),
    activator_ (0)
{
}


// Starts the server <server> if it is not already started
void ImplRepo_i::start (const char *server)
{
  int status;
  // Find out if it is already running
  char *ping_ior;
  status = this->repository_.get_ping_ior (server, ping_ior);

  if (status != 0)
  {
    ACE_ERROR ((LM_ERROR, "ERROR starting %s: No ping object\n", server));
    return;
  }

  TAO_TRY
  {
    CORBA::Object_var object =
      this->orb_manager_.orb ()->string_to_object (ping_ior, TAO_TRY_ENV);
    TAO_CHECK_ENV;

    Ping_Object_var ping_object = Ping_Object::_narrow (object.in(), TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (CORBA::is_nil (ping_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                 "invalid Ping Object ior: <%s>\n",
                 ping_ior));
      return;
    }
    
    ping_object->ping (TAO_TRY_ENV);
    TAO_CHECK_ENV;
  }
  TAO_CATCHANY
  {
    // Start it up
    char *cl;

    status = this->repository_.get_comm_line (server, cl);

    if (status == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Starting %s\n", server));

      ACE_Process_Options proc_opts;

      proc_opts.command_line (cl);

      ACE_Process proc;

      proc.spawn (proc_opts);

      ACE_OS::sleep (2);
      delete [] cl;
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, "ERROR starting %s\n", server));
    }
  }
  TAO_ENDTRY;
}


// Register the current location of the server
CORBA::String ImplRepo_i::server_is_running (const char *server,
                                             //const Implementation_Repository::INET_Addr &addr,
                                             CORBA::Object_ptr ior,
                                             CORBA::Object_ptr ping,
                                             CORBA::Environment &_env)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Server is running!\n"));

  // Update the record in the repository
  Repository::Record rec;
  this->repository_.resolve (server, rec);
  delete [] rec.ior;
  delete [] rec.ping_ior;
  rec.ior = ACE::strnew (this->orb_manager_.orb ()->object_to_string (ior, _env));
  rec.ping_ior = ACE::strnew (this->orb_manager_.orb ()->object_to_string (ping, _env));
  this->repository_.update (server, rec);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "The old IOR is: <%s>\n", this->orb_manager_.orb ()->object_to_string (ior, _env)));

  // Change the ior to ping to the IR instead, and return that 
  IIOP_Object *iiop_obj = ACE_dynamic_cast (IIOP_Object *, ior->_stubobj ());

  ACE_INET_Addr my_addr = TAO_ORB_Core_instance ()->addr ();

  // We are assuming that we are on the same machine right now
  iiop_obj->profile.port = my_addr.get_port_number ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "The new IOR is: <%s>\n", this->orb_manager_.orb ()->object_to_string (ior, _env)));

  return this->orb_manager_.orb ()->object_to_string (ior, _env);
}


// Reads the Server factory ior from a file
int
ImplRepo_i::read_ior (char *filename)
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
ImplRepo_i::parse_args (void)
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
        this->server_input_file_ = ACE::strnew (get_opts.optarg);
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
ImplRepo_i::init (int argc, char **argv, CORBA::Environment &_env)
{
  TAO_TRY
  {
    // Call the init of <TAO_ORB_Manager> to initialize the ORB and
    // create a child POA under the root POA.
    if (this->orb_manager_.init_child_poa (argc, argv, "ir_poa", TAO_TRY_ENV) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init_child_poa"), -1);
    TAO_CHECK_ENV;

    this->argc_ = argc;
    this->argv_ = argv;

    int retval = this->parse_args ();

    if (retval != 0)
      return retval;

    ACE_NEW_RETURN (this->forwarder_impl_,
                    IR_Forwarder (this->orb_manager_.orb (),
                                  this->orb_manager_.child_poa (),
                                  this),
                    -1);

    CORBA::String_var str  =
      this->orb_manager_.activate (this->forwarder_impl_);
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
                                                   TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "The IR IOR is: <%s>\n", ir_var.in ()));

    FILE *ir_file = ACE_OS::fopen ("implrepo.ior", "w");
    ACE_OS::fprintf (ir_file, "%s", ir_var.in ());
    ACE_OS::fclose (ir_file);

    ACE_NEW_RETURN (this->activator_,
                    IR_Adapter_Activator(this->forwarder_impl_),
                    -1);

    PortableServer::AdapterActivator_var activator =
      this->activator_->_this (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Register the TAO_Adapter_Activator reference to be the RootPOA's
    // Adapter Activator.

    this->orb_manager_.root_poa ()->the_activator (activator.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;


  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Server_i::init");
    TAO_RETHROW_RETURN (-1);
  }
  TAO_ENDTRY;
  return 0;
}

int
ImplRepo_i::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "IR_Server_i::run"), -1);
  return 0;
}

char *
ImplRepo_i::get_forward (const char *server)
{

  char *forward;
  if (this->repository_.get_ior (server, forward) != 0)
    return 0;
  return forward;
}

ImplRepo_i::~ImplRepo_i (void)
{
  if (this->forwarder_impl_ != 0)
    delete this->forwarder_impl_;
  if (this->activator_ != 0)
    delete this->activator_;
}

void
ImplRepo_i::register_server (const char *server,
                             const Implementation_Repository::Process_Options &options,
                             CORBA::Environment &env)
{
  Repository::Record rec;
  rec.comm_line = CORBA::string_dup (options.command_line_);
  rec.env = CORBA::string_dup (options.environment_);
  rec.wdir = CORBA::string_dup (options.working_directory_);
  rec.ior = "";
  rec.ping_ior = "";

  int status = this->repository_.add (server, rec);

  if (status == 1)
  {
    ACE_DEBUG ((LM_DEBUG, "Server %s Already Registered\n", server));
    TAO_THROW_ENV (Implementation_Repository::Already_Registered (), env);
  }
  else
    ACE_DEBUG ((LM_DEBUG, "Server %s Successfully Registered\n", server));
  this->repository_.dump ();
}


IR_Adapter_Activator::IR_Adapter_Activator (IR_Forwarder *servant)
: servant_ (servant)
{
  // Nothing
}


CORBA::Boolean
IR_Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                       const char *name,
                                       CORBA_Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "Unknown Adapter called\n"));

  CORBA::PolicyList policies (4);
  policies.length (4);

  // ID Assignment Policy
  policies[0] =
    parent->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return -1;
    }

  // Lifespan Policy
  policies[1] =
    parent->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }

  // Request Processing Policy
  policies[2] =
    parent->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_request_processing_policy");
      return -1;
    }

  // Id Uniqueness Policy
  policies[3] =
    parent->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_uniqueness_policy");
      return -1;
    }

  PortableServer::POA_var child = parent->create_POA (name,
                                                      PortableServer::POAManager::_nil (),
                                                      policies,
                                                      env);
  if (env.exception () != 0)
    return 0;

  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }

  if (env.exception () != 0)
    {
      env.print_exception ("unknown_adapter::policy->destroy");
      return -1;
    }

    PortableServer::AdapterActivator_var activator = this->_this (env);
    if (env.exception () != 0)
      {
        child->destroy (0, 0, env);
        return 0;
      }

    child->the_activator (activator.in (), env);

    if (env.exception () != 0)
      {
        env.print_exception ("unknown_adapter, the_activator");
        child->destroy (0, 0, env);
        return 0;
      }

    child->set_servant (this->servant_, env);

    if (env.exception () != 0)
      {
        env.print_exception ("unknown_adapter, set_servant");
        child->destroy (0, 0, env);
        return 0;
      }

    // Finally everything is fine
    return 1;
}


int IR_Forwarder::forward (char *name, CORBA::Environment &env)
{
  this->ir_impl_->start (name);
  char *forward = this->ir_impl_->get_forward (name);

  ACE_DEBUG ((LM_DEBUG, "Forwarding to %s\n", forward));

  CORBA::Object_ptr forward_object = this->orb_var_->string_to_object (forward);

  if (!CORBA::is_nil (forward_object))
    {
      env.exception (new PortableServer::ForwardRequest (forward_object));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Forward_to reference is nil.\n"));
      return -1;
    }
}


// Constructor
IR_Forwarder::IR_Forwarder (CORBA::ORB_ptr orb_ptr,
                            PortableServer::POA_ptr poa_ptr,
                            ImplRepo_i *ir_impl)
  : ir_impl_ (ir_impl),
    orb_var_ (CORBA::ORB::_duplicate (orb_ptr)),
    poa_var_ (PortableServer::POA::_duplicate (poa_ptr))
{
}

CORBA::RepositoryId 
IR_Forwarder::_primary_interface (const PortableServer::ObjectId &oid,
                                  PortableServer::POA_ptr poa,
                                  CORBA::Environment &env)
{
  return 0;
}

void 
IR_Forwarder::invoke (CORBA::ServerRequest_ptr request,
                      CORBA::Environment &env)
{
  // Get the POA Current object reference
  CORBA::Object_var obj = this->orb_var_->resolve_initial_references ("POACurrent");

  // Narrow the object reference to a POA Current reference
  PortableServer::Current_var poa_current = PortableServer::Current::_narrow (obj.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::Current::_narrow");
      return;
    }
  
  // The servant determines the key associated with the database entry
  // represented by self
  PortableServer::ObjectId_var oid = poa_current->get_object_id (env);
  if (env.exception () != 0)
    return;

  // Now convert the id into a string
  CORBA::String_var key = PortableServer::ObjectId_to_string (oid.in ());
  
  ACE_DEBUG ((LM_DEBUG, "Object is: %s\n", key));

  PortableServer::POA_ptr poa = poa_current->get_POA (env);
  if (env.exception () != 0)
    return;

  ACE_DEBUG ((LM_DEBUG, "POA is: %s\n", poa->the_name ()));

  this->forward (poa->the_name(), env);
}
