/* -*- C++ -*- */
// $Id$

#include "ImplRepo_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/Process.h"

ACE_RCSID(ImplRepo_Service, ImplRepo_i, "$Id$")

ImplRepo_i::ImplRepo_i (void)
  : forwarder_impl_ (0),
    activator_ (0),
    ior_output_file_ (0),
    server_key_ (0),
    debug_level_ (1)
{
}

// Starts up the server associated with the object pointer and returns
// an updated pointer.

CORBA::Object_ptr
ImplRepo_i::activate_object (CORBA::Object_ptr obj,
                             CORBA::Environment &TAO_IN_ENV)
{
  Implementation_Repository::INET_Addr *new_addr;
  IIOP_Object *new_iiop_obj = 0;

  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Activating Object: %s\n",
                this->orb_manager_.orb ()->object_to_string (obj)));

  TAO_TRY
    {
      // @@ Where to get the poa name from?
      new_addr = this->activate_server (0, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      IIOP_Object *iiop_obj = ACE_dynamic_cast (IIOP_Object *,
                                                obj->_stubobj ());

      ACE_NEW_RETURN (new_iiop_obj,
                      IIOP_Object (iiop_obj->type_id,
                                   ACE::strnew (new_addr->host_),
                                   new_addr->port_,
                                   iiop_obj->profile_in_use ()->object_key (),
                                   iiop_obj->profile_in_use ()->object_addr ()),
                      0);
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN (0);
    }
  TAO_ENDTRY;

  return new CORBA_Object (new_iiop_obj,
                           obj->_servant ());
}

// Starts the server <server> if it is not already started

Implementation_Repository::INET_Addr *
ImplRepo_i::activate_server (const char *server,
                             CORBA::Environment &TAO_IN_ENV)
{
  int start = 0;
  char *ping_ior;
  Implementation_Repository::INET_Addr *address =
    new Implementation_Repository::INET_Addr;
  address->port_ = 0;
  address->host_ = CORBA::string_dup ("");

  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Activating Server: %s\n",
                server));

  // Find out if it is already running
  if (this->repository_.get_ping_ior (server, ping_ior) != 0)
    {
      // If we had problems getting the ping_ior, probably meant that
      // there is no <server> registered
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find ping IOR for server <%s>\n",
                  server));
      TAO_THROW_RETURN (Implementation_Repository::Not_Found (),
                        address);
    }

  // if length is 0, then none is running yet.
  if (ACE_OS::strlen (ping_ior) != 0)
    {
      TAO_TRY
        {
          CORBA::Object_var object =
            this->orb_manager_.orb ()->string_to_object (ping_ior,
                                                         TAO_TRY_ENV);
          TAO_CHECK_ENV;

          Ping_Object_var ping_object = Ping_Object::_narrow (object.in (),
                                                              TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (ping_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Invalid Ping Object ior: <%s>\n",
                          ping_ior));
              TAO_THROW_RETURN (Implementation_Repository::Not_Found (),
                                address);
            }

          ping_object->ping (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
      TAO_CATCHANY
        {
          start = 1;
        }
      TAO_ENDTRY;
    }
  else
    start = 1;

  // Start it up...
  if (start == 1)
    {
      // Start it up
      char *cl;

      int status = this->repository_.get_comm_line (server,
                                                    cl);

      if (status == 0)
        {
          if (this->debug_level_ >= 1)
            ACE_DEBUG ((LM_DEBUG,
                        "Starting %s\n",
                        server));
          ACE_Process_Options proc_opts;

          proc_opts.command_line (cl);

          ACE_Process proc;

          if (proc.spawn (proc_opts) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Cannot activate server <%s> using <%s>\n",
                          server,
                          cl));
              delete [] cl;
              TAO_THROW_RETURN (Implementation_Repository::Cannot_Activate (CORBA::string_dup ("N/A")),
                                address);
            }

          delete [] cl;
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "Error: Cannot find commandline for server <%s>\n",
                      server));
          TAO_THROW_RETURN (Implementation_Repository::Not_Found (),
                            address);
        }

      // @@ Here is where we need to wait for the response so we can
      // find out where (host/port) the server started
      ACE_OS::sleep (3);
    }

  char *host;
  u_short port;

  this->repository_.get_hostport (server, host, port);

  address->host_ = CORBA::string_dup (host);
  address->port_ = port;

  return address;
}

// Adds an entry to the Repository about this <server>

void
ImplRepo_i::register_server (const char *server,
                             const Implementation_Repository::Process_Options &options,
                             CORBA::Environment &TAO_IN_ENV)
{
  Repository::Record rec;
  rec.comm_line = CORBA::string_dup (options.command_line_);
  rec.env = CORBA::string_dup (options.environment_);
  rec.wdir = CORBA::string_dup (options.working_directory_);
  rec.host = "";
  rec.port = 0;
  rec.ping_ior = "";

  int status = this->repository_.add (server, rec);

  if (status == 1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Server %s Already Registered!\n",
                  server));
      TAO_THROW (Implementation_Repository::Already_Registered ());
    }
  else
    {
      if (this->debug_level_ >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "register_server: Server %s Successfully Registered\n",
                    server));
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                              "Command Line: %s\n"
                              "Environment: %s\n"
                              "Working Directory: %s\n\n",
                              rec.comm_line,
                              rec.env,
                              rec.wdir));
    }
}

// Updates the entry in the Repository about this <server> or adds it
// if necessary.

void
ImplRepo_i::reregister_server (const char *server,
                               const Implementation_Repository::Process_Options &options,
                               CORBA::Environment &TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);
  Repository::Record rec;
  // @@ Darrell, please make sure to use the ACE_ALLOCATOR and
  // ACE_ALLOCATOR_RETURN macros for all of these string_dup() calls
  // in your code.
  rec.comm_line = CORBA::string_dup (options.command_line_);
  rec.env = CORBA::string_dup (options.environment_);
  rec.wdir = CORBA::string_dup (options.working_directory_);
  rec.host = "";
  rec.port = 0;
  rec.ping_ior = "";

  this->repository_.update (server, rec);

  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server %s Successfully Registered\n",
                server));
  if (this->debug_level_ >= 2)
    ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                          "Command Line: %s\n"
                          "Environment: %s\n"
                          "Working Directory: %s\n\n",
                          rec.comm_line,
                          rec.env,
                          rec.wdir));
}

// Remove the server entry from the Repository

void
ImplRepo_i::remove_server (const char *server,
                           CORBA::Environment &TAO_IN_ENV)
{
  if (this->repository_.remove (server) == 0)
    {
      if (this->debug_level_ >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Successfully Removed Server\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Trying to remove unknown server <%s>\n",
                  server));
      TAO_THROW (Implementation_Repository::Not_Found ());
    }
}

// Register the current location of the server

Implementation_Repository::INET_Addr *
ImplRepo_i::server_is_running (const char *server,
                               const Implementation_Repository::INET_Addr &addr,
                               CORBA::Object_ptr ping,
                               CORBA::Environment &TAO_IN_ENV)
{
  Implementation_Repository::INET_Addr *new_addr =
    new Implementation_Repository::INET_Addr;

  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server <%s> is running\n",
                server));

  // Update the record in the repository
  Repository::Record rec;
  if (this->repository_.resolve (server, rec) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Unknown Server <%s>\n",
                  server));
      TAO_THROW_RETURN (Implementation_Repository::Not_Found (),
                        new_addr);
    }

  // Delete the stuff that we will update
  delete [] rec.ping_ior;
  delete [] rec.host;

  rec.host = ACE::strnew (addr.host_.in ());
  rec.port = addr.port_;
  rec.ping_ior = ACE::strnew (this->orb_manager_.orb ()->object_to_string (ping,
                                                                           TAO_IN_ENV));
  this->repository_.update (server, rec);

  if (this->repository_.update (server, rec) == 0)
    {
      if (this->debug_level_ >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Successful server_is_running () of <%s>\n",
                    server));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                 "Error: While updating Repository while server_is_running () %s\n",
                 server));
      // @@ Return here?
    }

  if (this->debug_level_ >= 2)
    ACE_DEBUG ((LM_DEBUG,
                "The old host/port was: %Lu:%hu\n",
                rec.host,
                rec.port));

  ACE_INET_Addr my_addr = TAO_ORB_Core_instance ()->orb_params ()->addr ();

  // @@ We are assuming that we are on the same machine right now
  new_addr->host_ = CORBA::string_dup (my_addr.get_host_name ());
  new_addr->port_ = my_addr.get_port_number ();

  if (this->debug_level_ >= 2)
    ACE_DEBUG ((LM_DEBUG,
                "The new host/port is: %Lu:%hu\n",
                new_addr->host_.inout (),
                new_addr->port_));

  return new_addr;
}

// Remove the state information for the current server

void
ImplRepo_i::server_is_shutting_down (const char *server,
                                     CORBA::Environment &TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);
  Repository::Record rec;

  if (this->repository_.resolve (server, rec) == 0)
    {
      rec.host = "";
      rec.port = 0;
      rec.ping_ior = "";

      if (this->repository_.update (server, rec) == 0)
        {
          if (this->debug_level_ >= 1)
            ACE_DEBUG ((LM_DEBUG,
                        "Successful server_is_shutting_down () of <%s>\n",
                        server));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                     "Error: While updating Repository while shutting down <%s>\n",
                     server));
        }
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Unknown Server <%s>\n",
                  server));
      TAO_THROW (Implementation_Repository::Not_Found ());
    }
}

// Reads the Server factory ior from a file

int
ImplRepo_i::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle_ = ACE_OS::open (filename, 0);

  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error: Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle_);
  this->server_key_ = ior_buffer.read ();

  if (this->server_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error: Unable to allocate memory to read ior: %p\n"),
                      -1);

  ACE_OS::close (f_handle_);
  return 0;
}

int
ImplRepo_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "d:f:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        this->debug_level_ = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 'f': // read the IOR from the file.
        this->server_input_file_ = ACE::strnew (get_opts.optarg);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s"
                           " [-d] <debug_level>"
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
ImplRepo_i::init (int argc, char **argv, CORBA::Environment &TAO_IN_ENV)
{
  TAO_TRY
    {
      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.
      if (this->orb_manager_.init_child_poa (argc,
                                             argv,
                                             "ir_poa",
                                             TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error: With %p\n",
                           "init_child_poa"),
                          -1);
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
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "The server IOR is: <%s>\n",
                    str.in ()));

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_,
                           "%s",
                           str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }

      CORBA::String_var ir_var  =
        this->orb_manager_.activate_under_child_poa ("implrepo",
                                                     this,
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "The IR IOR is: <%s>\n",
                    ir_var.in ()));

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

      this->orb_manager_.root_poa ()->the_activator (activator.in (),
                                                     TAO_TRY_ENV);
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
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error: In IR_Server_i::run"),
                      -1);
  return 0;
}

CORBA::String
ImplRepo_i::get_forward_host (const char *server)
{
  char *host;
  u_short port;

  if (this->repository_.get_hostport (server, host, port) != 0)
    return 0;

  return host;
}

CORBA::UShort
ImplRepo_i::get_forward_port (const char *server)
{
  char *host;
  u_short port;

  if (this->repository_.get_hostport (server, host, port) != 0)
    return 0;

  delete host;
  return port;
}

ImplRepo_i::~ImplRepo_i (void)
{
  if (this->forwarder_impl_ != 0)
    delete this->forwarder_impl_;

  if (this->activator_ != 0)
    delete this->activator_;
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
  CORBA::PolicyList policies (4);
  policies.length (4);

  // ID Assignment Policy
  policies[0] =
    parent->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return 0;
    }

  // Lifespan Policy
  policies[1] =
    parent->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return 0;
    }

  // Request Processing Policy
  policies[2] =
    parent->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_request_processing_policy");
      return 0;
    }

  // @@ Darrell, can you please make all this code work correctly with
  // native C++ exceptions?!

  // Id Uniqueness Policy
  policies[3] =
    parent->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_uniqueness_policy");
      return 0;
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
      return 0;
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
                      CORBA::Environment &TAO_IN_ENV)
{
  // Get the POA Current object reference
  CORBA::Object_var obj = this->orb_var_->resolve_initial_references ("POACurrent");

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  TAO_POA_Current *poa_current = orb_core->poa_current ();

  if (TAO_IN_ENV.exception () != 0)
    {
      TAO_IN_ENV.print_exception ("PortableServer::Current::_narrow");
      return;
    }

  // The servant determines the key associated with the database entry
  // represented by self
  PortableServer::ObjectId_var oid = poa_current->get_object_id (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0)
    return;

  // Now convert the id into a string
  CORBA::String_var key = PortableServer::ObjectId_to_string (oid.in ());

  PortableServer::POA_ptr poa = poa_current->get_POA (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0)
    return;

  // Now FORWARD!!!

  Implementation_Repository::INET_Addr *new_addr = 0;

  TAO_TRY
    {
      new_addr = this->ir_impl_->activate_server (poa->the_name (),
                                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;

  CORBA_Object_ptr forward_object =
    this->orb_var_->key_to_object (poa_current->object_key (),
                                   0,
                                   TAO_IN_ENV);

  IIOP_Object *iiop_obj = ACE_dynamic_cast (IIOP_Object *,
                                            forward_object->_stubobj ());

  iiop_obj->profile_in_use ()->port (new_addr->port_);
  iiop_obj->profile_in_use ()->host (new_addr->host_);

//  if (TAO_debug_level > 0)
//    ACE_DEBUG ((LM_DEBUG,
//                "The forward_to is <%s>\n",
//                this->orb_var_->object_to_string (forward_object, TAO_IN_ENV)));

  if (!CORBA::is_nil (forward_object))
    TAO_IN_ENV.exception (new PortableServer::ForwardRequest (forward_object));
  else
    ACE_ERROR ((LM_ERROR,
                "Error: Forward_to reference is nil.\n"));
}
