/* -*- C++ -*- */
// $Id$

#include "ImplRepo_i.h"
#include "Options.h"
#include "ace/Read_Buffer.h"
#include "ace/Process.h"
#include "tao/IIOP_Profile.h"

// Constructor

ImplRepo_i::ImplRepo_i (void)
  : forwarder_impl_ (0),
    activator_ (0)
{
  // Nothing
}

// Starts up the server associated with the object pointer and returns
// an updated pointer.

CORBA::Object_ptr
ImplRepo_i::activate_object (CORBA::Object_ptr obj,
                             CORBA::Environment &ACE_TRY_ENV)
{
  Implementation_Repository::INET_Addr *new_addr;
  TAO_Stub *new_stub_obj = 0;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Activating Object: %s\n",
                this->orb_manager_.orb ()->object_to_string (obj)));

  ACE_TRY
    {
      // @@ Where to get the poa name from?
      new_addr = this->activate_server (0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ Use auto_ptr<> to avoid memory leaks!
      TAO_Stub *stub_obj = obj->_stubobj ();
      TAO_IIOP_Profile *iiop_pfile =
                ACE_dynamic_cast (TAO_IIOP_Profile *,
                                  stub_obj->profile_in_use ());

      TAO_MProfile mp(1);

      TAO_Profile *new_pfile;
      // @@ Would new_addr->host_ be different from object_addr()?
      // if so I will add another ctor  fred
      ACE_NEW_RETURN (new_pfile,
                      TAO_IIOP_Profile (iiop_pfile->object_addr (),
                                        iiop_pfile->object_key ()),
                      CORBA::Object::_nil ());

      mp.give_profile (new_pfile);

      // create new obj, pfile will be copied!
      new_stub_obj = new TAO_Stub (stub_obj->type_id,
                                   mp,
                                   stub_obj->orb_core ());

      if (new_stub_obj == 0)
        return CORBA::Object::_nil ();
    }
  ACE_CATCHANY
    {
      ACE_RETHROW;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return new CORBA_Object (new_stub_obj,
                           obj->_servant ());
}

// Starts the server <server> if it is not already started

Implementation_Repository::INET_Addr *
ImplRepo_i::activate_server (const char *server,
                             CORBA::Environment &ACE_TRY_ENV)
{
  int start = 0;
  ASYS_TCHAR *ping_ior;
  Implementation_Repository::INET_Addr *address =
    new Implementation_Repository::INET_Addr;
  address->port_ = 0;
  address->host_ = CORBA::string_dup ("");

  if (OPTIONS::instance()->debug () >= 1)
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
      ACE_THROW_RETURN (Implementation_Repository::Not_Found (),
                        address);
    }

  // if length is 0, then none is running yet.
  if (ACE_OS::strlen (ping_ior) != 0)
    {
      ACE_TRY
        {
          CORBA::Object_var object =
            this->orb_manager_.orb ()->string_to_object (ping_ior,
                                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          Ping_Object_var ping_object = Ping_Object::_narrow (object.in (),
                                                              ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (ping_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Invalid Ping Object ior: <%s>\n",
                          ping_ior));
              ACE_THROW_RETURN (Implementation_Repository::Not_Found (),
                                address);
            }

          ping_object->ping (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          start = 1;
        }
      ACE_ENDTRY;
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
          if (OPTIONS::instance()->debug () >= 1)
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
              ACE_THROW_RETURN (Implementation_Repository::Cannot_Activate (CORBA::string_dup ("N/A")),
                                address);
            }

          delete [] cl;
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "Error: Cannot find commandline for server <%s>\n",
                      server));
          ACE_THROW_RETURN (Implementation_Repository::Not_Found (),
                            address);
        }

      // @@ Here is where we need to wait for the response so we can
      // find out where (host/port) the server started
      ACE_OS::sleep (3);
    }

  char *host;
  u_short port;

  if (this->repository_.get_hostport (server, host, port) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ImplRepo_i::activate_server: "
                  "cannot resolve server <%s>\n",
                  server));
      return address;
    }

  address->host_ = CORBA::string_dup (host);
  address->port_ = port;

  return address;
}

// Adds an entry to the Repository about this <server>

void
ImplRepo_i::register_server (const char *server,
                             const Implementation_Repository::Process_Options &options,
                             CORBA::Environment &ACE_TRY_ENV)
{
  if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                              "Command Line: %s\n"
                              "Environment: %s\n"
                              "Working Directory: %s\n\n",
                              server,
                              options.command_line_.in (),
                              options.environment_.in (),
                              options.working_directory_.in ()));

  Repository_Record rec (options.command_line_,
                         options.environment_,
                         options.working_directory_,
                         "",
                         0,
                         "");

  int status = this->repository_.add (server, rec);

  if (status == 1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Server %s Already Registered!\n",
                  server));
      ACE_THROW (Implementation_Repository::Already_Registered ());
    }
  else
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "register_server: Server %s Successfully Registered\n",
                    server));
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                              "Command Line: %s\n"
                              "Environment: %s\n"
                              "Working Directory: %s\n\n",
                              server,
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
                               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  Repository_Record rec (options.command_line_,
                         options.environment_,
                         options.working_directory_,
                         "",
                         0,
                         "");

  this->repository_.update (server, rec);

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server %s Successfully Registered\n",
                server));
  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                          "Command Line: %s\n"
                          "Environment: %s\n"
                          "Working Directory: %s\n\n",
                          server,
                          rec.comm_line,
                          rec.env,
                          rec.wdir));
}

// Remove the server entry from the Repository

void
ImplRepo_i::remove_server (const char *server,
                           CORBA::Environment &ACE_TRY_ENV)
{
  if (this->repository_.remove (server) == 0)
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Successfully Removed Server\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Trying to remove unknown server <%s>\n",
                  server));
      ACE_THROW (Implementation_Repository::Not_Found ());
    }
}

// Register the current location of the server

Implementation_Repository::INET_Addr *
ImplRepo_i::server_is_running (const char *server,
                               const Implementation_Repository::INET_Addr &addr,
                               CORBA::Object_ptr ping,
                               CORBA::Environment &ACE_TRY_ENV)
{
  Implementation_Repository::INET_Addr *new_addr =
    new Implementation_Repository::INET_Addr;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server <%s> is running\n",
                server));

  // Update the record in the repository
  Repository_Record rec;

  if (this->repository_.resolve (server, rec) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Unknown Server <%s>\n",
                  server));
      ACE_THROW_RETURN (Implementation_Repository::Not_Found (),
                        new_addr);
    }

  // Delete the stuff that we will update
  delete [] rec.ping_ior;
  delete [] rec.host;

  ACE_NEW_RETURN (rec.host, ASYS_TCHAR[ACE_OS::strlen (addr.host_.in ()) + 1], 0);
  ACE_OS::strcpy (rec.host, addr.host_.in ());
  rec.port = addr.port_;

  ASYS_TCHAR *ping_ior = this->orb_manager_.orb ()->object_to_string (ping, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_NEW_RETURN (rec.ping_ior, ASYS_TCHAR[ACE_OS::strlen (ping_ior) + 1], 0);
  ACE_OS::strcpy (rec.ping_ior, ping_ior);

  if (this->repository_.update (server, rec) == 0)
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Successful server_is_running () of <%s>\n",
                    server));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                 "Error: While updating Repository while server_is_running () %s\n",
                 server));
      return new_addr;
    }

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG,
                "The old host/port was: %Lu:%hu\n",
                rec.host,
                rec.port));

  ACE_INET_Addr my_addr = TAO_ORB_Core_instance ()->orb_params ()->addr ();

  // @@ We are assuming that we are on the same machine right now
  new_addr->host_ = CORBA::string_dup (my_addr.get_host_name ());
  new_addr->port_ = my_addr.get_port_number ();

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG,
                "The new host/port is: %Lu:%hu\n",
                new_addr->host_.inout (),
                new_addr->port_));

  return new_addr;
}

// Remove the state information for the current server

void
ImplRepo_i::server_is_shutting_down (const char *server,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  Repository_Record rec;

  if (this->repository_.resolve (server, rec) == 0)
    {
      ACE_OS::strcpy (rec.host, ASYS_TEXT (""));
      rec.port = 0;
      ACE_OS::strcpy (rec.ping_ior, ASYS_TEXT (""));

      if (this->repository_.update (server, rec) == 0)
        {
          if (OPTIONS::instance()->debug () >= 1)
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
      ACE_THROW (Implementation_Repository::Not_Found ());
    }
}

int
ImplRepo_i::init (int argc, char **argv, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.
      if (this->orb_manager_.init_child_poa (argc,
                                             argv,
                                             "ir_poa",
                                             ACE_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error: With %p\n",
                           "init_child_poa"),
                          -1);
      ACE_TRY_CHECK;

      int retval = OPTIONS::instance()->parse_args (argc, argv);

      if (retval != 0)
        return retval;

      ACE_NEW_RETURN (this->forwarder_impl_,
                      IR_Forwarder (this->orb_manager_.orb (),
                                    this->orb_manager_.child_poa (),
                                    this),
                      -1);

      CORBA::String_var str  =
        this->orb_manager_.activate (this->forwarder_impl_);
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "The server IOR is: <%s>\n",
                    str.in ()));

      if (OPTIONS::instance()->output_file ())
        {
          ACE_OS::fprintf (OPTIONS::instance()->output_file (),
                           "%s",
                           str.in ());
          ACE_OS::fclose (OPTIONS::instance()->output_file ());
        }

      CORBA::String_var ir_var  =
        this->orb_manager_.activate_under_child_poa ("implrepo",
                                                     this,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (OPTIONS::instance()->debug () >= 2)
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
        this->activator_->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Register the TAO_Adapter_Activator reference to be the RootPOA's
      // Adapter Activator.

      this->orb_manager_.root_poa ()->the_activator (activator.in (),
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Server_i::init");
      ACE_RETHROW;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);
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
                                       CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::PolicyList policies (4);
  policies.length (4);

  const char *exception_message = "Null Message";

  ACE_TRY
    {
      // ID Assignment Policy
      exception_message = "While PortableServer::POA::create_id_assignment_policy";
      policies[0] =
        parent->create_id_assignment_policy (PortableServer::USER_ID, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Lifespan Policy
      exception_message = "While PortableServer::POA::create_lifespan_policy";
      policies[1] =
        parent->create_lifespan_policy (PortableServer::PERSISTENT, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Request Processing Policy
      exception_message = "While PortableServer::POA::create_request_processing_policy";
      policies[2] =
        parent->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Id Uniqueness Policy
      exception_message = "While PortableServer::POA::create_id_uniqueness_policy";
      policies[3] =
        parent->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      exception_message = "While create_POA";
      PortableServer::POA_var child = parent->create_POA (name,
                                                          PortableServer::POAManager::_nil (),
                                                          policies,
                                                          ACE_TRY_ENV);

      exception_message = "While unknown_adapter::policy->destroy";
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      exception_message = "While _this";
      PortableServer::AdapterActivator_var activator = this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      exception_message = "While child->the_activator";
      child->the_activator (activator.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      exception_message = "While unknown_adapter, set_servant";
      child->set_servant (this->servant_, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "IR_Adapter_Activator::unknown_adapter - %s\n", exception_message));
      ACE_TRY_ENV.print_exception ("SYS_EX");
      return 0;
    }
  ACE_ENDTRY;

  // Finally, now everything is fine
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
IR_Forwarder::_primary_interface (const PortableServer::ObjectId & /* oid */,
                                  PortableServer::POA_ptr /* poa */,
                                  CORBA::Environment &)
{
  return 0;
}

void
IR_Forwarder::invoke (CORBA::ServerRequest_ptr /* request */,
                      CORBA::Environment &ACE_TRY_ENV)
{
  // Get the POA Current object reference
  CORBA::Object_var obj = this->orb_var_->resolve_initial_references ("POACurrent");

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  TAO_POA_Current *poa_current = orb_core->poa_current ();

  if (ACE_TRY_ENV.exception () != 0)
    {
      ACE_TRY_ENV.print_exception ("PortableServer::Current::_narrow");
      return;
    }

  // The servant determines the key associated with the database entry
  // represented by self
  PortableServer::ObjectId_var oid = poa_current->get_object_id (ACE_TRY_ENV);
  if (ACE_TRY_ENV.exception () != 0)
    return;

  // Now convert the id into a string
  CORBA::String_var key = PortableServer::ObjectId_to_string (oid.in ());

  PortableServer::POA_ptr poa = poa_current->get_POA (ACE_TRY_ENV);
  if (ACE_TRY_ENV.exception () != 0)
    return;

  // Now FORWARD!!!

  Implementation_Repository::INET_Addr *new_addr = 0;

  ACE_TRY
    {
      new_addr = this->ir_impl_->activate_server (poa->the_name (),
                                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_RETHROW;
    }
  ACE_ENDTRY;

  CORBA_Object_ptr forward_object =
    this->orb_var_->key_to_object (poa_current->object_key (),
                                   0,
                                   ACE_TRY_ENV);

  TAO_Stub *stub_obj = ACE_dynamic_cast (TAO_Stub *,
                                         forward_object->_stubobj ());

  TAO_IIOP_Profile *iiop_pfile =
            ACE_dynamic_cast (TAO_IIOP_Profile *,
                              stub_obj->profile_in_use ());

  iiop_pfile->port (new_addr->port_);
  iiop_pfile->host (new_addr->host_);

//  if (TAO_debug_level > 0)
//    ACE_DEBUG ((LM_DEBUG,
//                "The forward_to is <%s>\n",
//                this->orb_var_->object_to_string (forward_object, ACE_TRY_ENV)));

  if (!CORBA::is_nil (forward_object))
    ACE_TRY_ENV.exception (new PortableServer::ForwardRequest (forward_object));
  else
    ACE_ERROR ((LM_ERROR,
                "Error: Forward_to reference is nil.\n"));
}
