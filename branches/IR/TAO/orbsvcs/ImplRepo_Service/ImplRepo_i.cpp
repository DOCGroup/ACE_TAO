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

// Starts the server defined by the POA name <server> if it is 
// not already started and if it can be started.

void
ImplRepo_i::activate_server (const char *server,
                             CORBA::Environment &ACE_TRY_ENV)
{
  int start = 0;
  ACE_TString ping_ior, host;
  unsigned short port;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "Activating Server: %s\n", server));

  // Find out if it is already running
  if (this->repository_.get_running_info (server, host, port, ping_ior) != 0)
    {
      // If we had problems getting the ping_ior, probably meant that
      // there is no <server> registered
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find ping IOR for server <%s>\n",
                  server));
      ACE_THROW (ImplementationRepository::Administration::NotFound ());
    }

  // Check to see if there is one running (if there is a ping_ior)
  if (ping_ior.length () != 0)
    {
      // It is running
      ACE_TRY
        {
          CORBA::Object_var object =
            this->orb_manager_.orb ()->string_to_object (ping_ior.c_str (),
                                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // We shouldn't have to narrow.

          ImplementationRepository::ServerObject_var ping_object = 
            ImplementationRepository::ServerObject::_narrow (object.in (), ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (ping_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Invalid Ping Object ior: <%s>\n",
                          ping_ior));
              ACE_THROW (ImplementationRepository::Administration::NotFound ());
            }

          // Check to see if we can ping it
          ping_object->ping (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // If we got an exception, then we have to restart it.
          start = 1;
        }
      ACE_ENDTRY;
    }
  else
    // We need to restart
    start = 1;

  // Start it up...
  if (start == 1)
    {
      // Check to see if it is already starting up
      int startup_val = this->repository_.starting_up (server, 1);

      if (startup_val == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error: Cannot find startup info for server <%s>\n",
                      server));
          ACE_THROW(ImplementationRepository::Administration::NotFound ());
        }
      
      if (startup_val == 0)
        {
          ACE_TString logical, startup, working;
          if (this->repository_.get_startup_info (server, logical, startup, working) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Cannot find startup info for server <%s>\n",
                          server));
              ACE_THROW(ImplementationRepository::Administration::NotFound ());
            }

          if (startup.length () == 0)
            {
              // If there is no startup information, throw a transient exception
              ACE_ERROR ((LM_ERROR,
                          "Error: No startup information for server <%s>\n",
                          server));
              ACE_THROW (CORBA::TRANSIENT (CORBA::COMPLETED_NO));
            }


          if (OPTIONS::instance()->debug () >= 1)
            ACE_DEBUG ((LM_DEBUG, "Starting %s\n", server));
      
          ACE_Process_Options proc_opts;

          proc_opts.command_line (startup.c_str ());
          proc_opts.working_directory (working.c_str ());

          ACE_Process proc;

          if (proc.spawn (proc_opts) == -1)
            {
              ACE_ERROR ((LM_ERROR, 
                         "Error: Cannot activate server <%s> using <%s>\n",
                          server,
                          startup.c_str ()));
              ACE_THROW (ImplementationRepository::Administration::CannotActivate (CORBA::string_dup ("N/A")));
            }
        }
      
      // Now that the server has been started up, we need to go back into the event
      // loop so we can get the reponse or handle other requests
      TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

      int starting_up;
      
      while ((starting_up = this->repository_.starting_up (server)) == 1)
        {
          if (OPTIONS::instance()->debug () >= 2)
            ACE_DEBUG ((LM_DEBUG, "activate_server: Going into handle_events\n"));
          orb_core->reactor ()->handle_events ();
        }
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "activate_server: Got out of handle_events loop\n"));
      
      // Check to see if it disappeared on us
      if (starting_up == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error: Cannot find startup info for server <%s>\n",
                      server));
          ACE_THROW (ImplementationRepository::Administration::NotFound ());
        }

      // Now it should be started.
    }

  if (this->repository_.get_running_info (server, host, port, ping_ior) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ImplRepo_i::activate_server: "
                  "cannot resolve server <%s>\n",
                  server));
    }
}

// Adds an entry to the Repository about this <server>

void
ImplRepo_i::register_server (const char *server,
                             const ImplementationRepository::StartupOptions &options,
                             CORBA::Environment &ACE_TRY_ENV)
{
  if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                              "Command Line: %s\n"
                              "Working Directory: %s\n\n",
                              server,
                              options.command_line.in (),
                              options.working_directory.in ()));
  
  // Add the server
  int status = this->repository_.add (server, 
                                      "",
                                      options.command_line.in (), 
                                      options.working_directory.in ());

  if (status == 1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Server %s Already Registered!\n",
                  server));
      ACE_THROW (ImplementationRepository::Administration::AlreadyRegistered ());
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
                              "Working Directory: %s\n\n",
                              server,
                              options.command_line.in (),
                              options.working_directory.in ()));
    }
}

// Updates the entry in the Repository about this <server> or adds it
// if necessary.

void
ImplRepo_i::reregister_server (const char *server,
                               const ImplementationRepository::StartupOptions &options,
                               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  this->repository_.remove (server);

  this->repository_.add (server, 
                         "",
                         options.command_line.in (),
                         options.working_directory.in ());

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server %s Successfully Registered\n",
                server));
  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                          "Command Line: %s\n"
                          "Working Directory: %s\n\n",
                          server,
                          options.command_line.in (),
                          options.working_directory.in ()));
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
      ACE_THROW (ImplementationRepository::Administration::NotFound ());
    }
}

// Register the current location of the server

ImplementationRepository::Address * 
ImplRepo_i::server_is_running (const char * server,
                               const ImplementationRepository::Address &addr,
                               ImplementationRepository::ServerObject_ptr server_object,
                               CORBA::Environment &ACE_TRY_ENV)
{
  ImplementationRepository::Address *new_addr =
    new ImplementationRepository::Address;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server <%s> is running\n",
                server));

  // Update the record in the repository

  // Get the stringified ping_ior
  ASYS_TCHAR *ping_ior = this->orb_manager_.orb ()->object_to_string (server_object, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (this->repository_.update (server, addr.host.in (), addr.port, ping_ior) == 0)
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Successful server_is_running () of <%s>\n",
                    server));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                 "Error: Could not update running information for server <%s>\n",
                 server));
      ACE_THROW_RETURN (ImplementationRepository::Administration::NotFound (), new_addr);
    }

  ACE_INET_Addr my_addr = TAO_ORB_Core_instance ()->orb_params ()->addr ();

  new_addr->host = CORBA::string_dup (my_addr.get_host_name ());
  new_addr->port = my_addr.get_port_number ();

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG,
                "The new host/port is: %Lu:%hu\n",
                new_addr->host.in (),
                new_addr->port));

  this->repository_.starting_up (server, 0);

  return new_addr;
}

// Remove the state information for the current server

void
ImplRepo_i::server_is_shutting_down (const char *server,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  if (this->repository_.update (server, "", 0, "") == 0)
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Successful server_is_shutting_down () of <%s>\n",
                    server));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                 "Error: Could not update information for unknown server <%s>\n",
                 server));
      ACE_THROW (ImplementationRepository::Administration::NotFound ());
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
  ACE_TString host, ping_ior;
  unsigned short port;

  if (this->repository_.get_running_info (server, host, port, ping_ior) != 0)
    return 0;

  return CORBA::string_dup (host.c_str ());
}

CORBA::UShort
ImplRepo_i::get_forward_port (const char *server)
{
  ACE_TString host, ping_ior;
  unsigned short port;

  if (this->repository_.get_running_info (server, host, port, ping_ior) != 0)
    return 0;

  return port;
}

ImplRepo_i::~ImplRepo_i (void)
{
  if (this->forwarder_impl_ != 0)
    delete this->forwarder_impl_;

  if (this->activator_ != 0)
    delete this->activator_;
}


// Used to access the list of servers registered.  May also return an
// iterator which can be used to access more than <how_many> of them.

void 
ImplRepo_i::list (CORBA::ULong how_many,
                  ImplementationRepository::ServerInformationList_out server_list,
                  ImplementationRepository::ServerInformationIterator_out server_iterator,
                  CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize the out varaibles, so if we return early, they will
  // not be dangling.
  server_iterator = 
    ImplementationRepository::ServerInformationIterator::_nil ();

  ACE_NEW_THROW_EX (server_list,
                    ImplementationRepository::ServerInformationList (0),
                    CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_CHECK;

  // Get a new iterator
  Server_Repository::HASH_IR_ITER *hash_iter = this->repository_.new_iterator ();

  // Check for a memory error.
  if (hash_iter == 0)
  {
    ACE_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
    ACE_CHECK;
  }
  
  // Number of servers that will go into the server_list.
  CORBA::ULong n;

  if (this->repository_.get_repository_size () > how_many)
    n = how_many;
  else
    n = this->repository_.get_repository_size ();


  
  // A pointer to BindingIterator servant.
  TAO_Hash_Binding_Iterator *bind_iter = 0;

  // Use hash iterator to populate a BindingList with bindings.
  bl->length (n);

  Server_Repository::HASH_IR_ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (populate_binding (hash_entry, bl[i]) == 0)
        {
          delete hash_iter;
          ACE_THROW (CORBA::NO_MEMORY(CORBA::COMPLETED_NO));
        }
    }

  // Now we are done with the BindingsList, and we can follow up on
  // the iterator business.

  // If we do not need to pass back BindingIterator.
  if (this->context_.current_size () <= how_many)
    delete hash_iter;
  else
    {
      // Create a BindingIterator for return.

      bind_iter = new TAO_Hash_Binding_Iterator (hash_iter, this->poa_.in (), this->lock_);
      if (bind_iter == 0)
        {
          delete hash_iter;
          ACE_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
        }

      ACE_TRY
        {
          char poa_id[BUFSIZ];
          ACE_OS::sprintf (poa_id,
                           "%s_%ld",
                           this->poa_id_.c_str (),
                           this->counter_++);

          PortableServer::ObjectId_var id =
            PortableServer::string_to_ObjectId (poa_id);

          this->poa_->activate_object_with_id (id.in (),
                                               bind_iter,
                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          bi = bind_iter->_this (ACE_TRY_ENV);
          ACE_TRY_CHECK;

        }
      ACE_CATCHANY
        {
          delete bind_iter;
          ACE_RETHROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}


// Attempts to gracefully shut down the server, if that fails, will try
// to do it ungracefully.

void 
ImplRepo_i::shutdown_server (const char *server,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Nothing yet
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

  ACE_TRY
    {
      this->ir_impl_->activate_server (poa->the_name (),
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

  iiop_pfile->port (this->ir_impl_->get_forward_port (poa->the_name ()));
  iiop_pfile->host (this->ir_impl_->get_forward_host (poa->the_name ()));

  if (!CORBA::is_nil (forward_object))
    ACE_TRY_ENV.exception (new PortableServer::ForwardRequest (forward_object));
  else
    ACE_ERROR ((LM_ERROR,
                "Error: Forward_to reference is nil.\n"));
}


// Plain old constructor

IR_Iterator::IR_Iterator (Server_Repository::HASH_IR_ITER *iterator,
                          PortableServer::POA_ptr poa)
  : iterator_ (iterator),
    poa_ (poa)
{
  // Nothing
}


// Destructor

IR_Iterator::~IR_Iterator ()
{
  delete iterator_;
}


// Returns the next list of up to <how_many> servers.  If empty, will return
// false.

CORBA::Boolean 
IR_Iterator::next_n (CORBA::ULong how_many,
                     ImplementationRepository::ServerInformationList_out server_list,
                     CORBA::Environment &ACE_TRY_ENV)
{
  // We perform an allocation before obtaining the lock so that an out
  // parameter is allocated in case we fail to obtain the lock.
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);
  // Obtain a lock.
  ACE_GUARD_THROW_EX (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL (CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  // If there are no more bindings...
  if (hash_iter_->done ())
      return 0;
  else
    {
      // Initially assume that iterator has the requested number of
      // bindings.
      bl->length (how_many);

      TAO_Hash_Naming_Context::HASH_MAP::ENTRY *hash_entry;

      // Iterate and populate the BindingList.

      for (CORBA::ULong i = 0; i < how_many; i++)
        {
          hash_iter_->next (hash_entry);

          if (TAO_Hash_Naming_Context::populate_binding (hash_entry, bl[i]) == 0)
            ACE_THROW_RETURN (CORBA::NO_MEMORY (CORBA::COMPLETED_NO), 0);

          if (hash_iter_->advance () == 0)
            {
              // If no more bindings left, reset length to the actual
              // number of bindings populated and get out of the loop.
              bl->length (i + 1);
              break;
            }
        }
      return 1;
    }
}

void IR_Iterator::destroy (CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ());
  // Destroys the iterator.

