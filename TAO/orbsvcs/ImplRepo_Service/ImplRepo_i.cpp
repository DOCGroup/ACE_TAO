/* -*- C++ -*- */
// $Id$

#include "ImplRepo_i.h"
#include "Options.h"
#include "tao/ORB.h"
#include "tao/IIOP_Profile.h"
#include "tao/IIOP_Acceptor.h"
#include "tao/Acceptor_Registry.h"
#include "ace/Read_Buffer.h"
#include "ace/Process.h"
#include "ace/Auto_Ptr.h"

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
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound,
                     ImplementationRepository::Administration::CannotActivate))
{
  int start = 0;
  ACE_TString server_object_ior, host;
  unsigned short port;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "Activating Server: %s\n", server));

  // Find out if it is already running
  if (this->repository_.get_running_info (server, host, port, server_object_ior) != 0)
    {
      // If we had problems getting the server_object_ior, probably meant that
      // there is no <server> registered
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find ServerObject IOR for server <%s>\n",
                  server));
      ACE_THROW (ImplementationRepository::Administration::NotFound ());
    }

  // Check to see if there is one running (if there is a server_object_ior)
  if (server_object_ior.length () != 0)
    {
      // It is running
      ACE_TRY
        {
          CORBA::ORB_var orb = this->orb_manager_.orb ();
          CORBA::Object_var object =
            orb->string_to_object (server_object_ior.c_str (),
                                   ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ImplementationRepository::ServerObject_var server_object = 
            ImplementationRepository::ServerObject::_narrow (object.in (), ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (server_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Invalid ServerObject IOR: <%s>\n",
                          server_object_ior.c_str ()));
              ACE_THROW (ImplementationRepository::Administration::NotFound ());
            }

          // Check to see if we can ping it
          server_object->ping (ACE_TRY_ENV);
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
              ACE_THROW (CORBA::TRANSIENT ());
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

  if (this->repository_.get_running_info (server, host, port, server_object_ior) != 0)
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
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::AlreadyRegistered))
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
    ACE_THROW_SPEC ((CORBA::SystemException))
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
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound))
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
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound))
{
  ImplementationRepository::Address *new_addr =
    new ImplementationRepository::Address;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server <%s> is running\n",
                server));

  // Get the stringified server_object_ior
  CORBA::ORB_var orb = this->orb_manager_.orb ();
  ASYS_TCHAR *server_object_ior = orb->object_to_string (server_object, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (this->repository_.update (server, addr.host.in (), addr.port, server_object_ior) == 0)
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

  TAO_Acceptor_Registry* registry =
    orb->orb_core ()->acceptor_registry ();

  TAO_Acceptor *acceptor = 0;
  TAO_AcceptorSetItor end = registry->end ();
  for (TAO_AcceptorSetItor i = registry->begin (); i != end; ++i)
    {
      if ((*i)->tag () == TAO_IOP_TAG_INTERNET_IOP)
        {
          acceptor = (*i);
          break;
        }
    }
  if (acceptor == 0)
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT(), 0);

  TAO_IIOP_Acceptor* iiop_acceptor =
    ACE_dynamic_cast (TAO_IIOP_Acceptor*,acceptor);

  // Get our host and port and convert it to something we can use.
  const ACE_INET_Addr& my_addr  = iiop_acceptor->address ();

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
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound))
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

      CORBA::ORB_var orb = this->orb_manager_.orb ();
      PortableServer::POA_var child_poa = this->orb_manager_.child_poa ();
      ACE_NEW_RETURN (this->forwarder_impl_,
                      IR_Forwarder (orb.in (),
                                    child_poa.in (),
                                    this),
                      -1);

      CORBA::String_var str =
        this->orb_manager_.activate (this->forwarder_impl_, ACE_TRY_ENV);
      ACE_TRY_CHECK;

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

      PortableServer::POAManager_var poa_manager =
        this->orb_manager_.poa_manager ();

      ACE_NEW_RETURN (this->activator_,
                      IR_Adapter_Activator (this->forwarder_impl_,
                                            poa_manager.in ()),
                      -1);

      PortableServer::AdapterActivator_var activator =
        this->activator_->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Register the Adapter_Activator reference to be the RootPOA's
      // Adapter Activator.

      PortableServer::POA_var root_poa = this->orb_manager_.root_poa ();
      root_poa->the_activator (activator.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Server_i::init");
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
  ACE_TString host, server_object_ior;
  unsigned short port;

  if (this->repository_.get_running_info (server, host, port, server_object_ior) != 0)
    return 0;

  return CORBA::string_dup (host.c_str ());
}

CORBA::UShort
ImplRepo_i::get_forward_port (const char *server)
{
  ACE_TString host, server_object_ior;
  unsigned short port;

  if (this->repository_.get_running_info (server, host, port, server_object_ior) != 0)
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


// Returns the startup information for a server

void 
ImplRepo_i::find (const char * server,
                  ImplementationRepository::ServerInformation_out info,
                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TString logical, command_line, working_directory;
  ACE_TString host, server_object_ior;
  unsigned short port;

  ACE_NEW_THROW_EX (info,
                    ImplementationRepository::ServerInformation,
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
  
  // Get the information from the server repository.
  if (this->repository_.get_startup_info (server, 
                                          logical, 
                                          command_line, 
                                          working_directory) != 0)
    ACE_THROW (ImplementationRepository::Administration::NotFound ());

  if (this->repository_.get_running_info (server, host, port, server_object_ior) != 0)
    ACE_THROW (ImplementationRepository::Administration::NotFound ());

  // Fill in <info>.
  info->logical_server = CORBA::string_dup (logical.c_str ());
  info->server = CORBA::string_dup (server);
  info->startup.command_line = CORBA::string_dup (command_line.c_str ());
  info->startup.working_directory = CORBA::string_dup (working_directory.c_str ());
  info->location.host = CORBA::string_dup (host.c_str ());
  info->location.port = port;
}


// Used to access the list of servers registered.  May also return an
// iterator which can be used to access more than <how_many> of them.

void 
ImplRepo_i::list (CORBA::ULong how_many,
                  ImplementationRepository::ServerInformationList_out server_list,
                  ImplementationRepository::ServerInformationIterator_out server_iterator,
                  CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize the out variables, so if we return early, they will
  // not be dangling.
  server_iterator = 
    ImplementationRepository::ServerInformationIterator::_nil ();

  ACE_NEW_THROW_EX (server_list,
                    ImplementationRepository::ServerInformationList (0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Get a new iterator
  auto_ptr<Server_Repository::HASH_IR_ITER> server_iter (this->repository_.new_iterator ());

  // Check for a memory error.
  if (server_iter.get () == 0)
  {
    ACE_THROW (CORBA::NO_MEMORY ());
    ACE_CHECK;
  }
  
  // Number of servers that will go into the server_list.
  CORBA::ULong n;

  if (this->repository_.get_repository_size () > how_many)
    n = how_many;
  else
    n = this->repository_.get_repository_size ();
  
  // Now copy over to the server list.

  server_list->length (n);

  Server_Repository::HASH_IR_ENTRY *server_entry;

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "list: Filling ServerList with %d servers\n", n));

  for (CORBA::ULong i = 0; i < n; i++)
    {
      server_iter->next (server_entry);
      server_iter->advance ();

      ACE_TString logical, server, command_line, working_directory, host, server_ior;
      unsigned short port;

      server_entry->int_id_->get_running_info (host, port, server_ior);
      server_entry->int_id_->get_startup_info (logical, command_line, working_directory);

      server_list[i].logical_server = CORBA::string_dup (logical.c_str ());
      server_list[i].server = CORBA::string_dup (server_entry->ext_id_.c_str ());
      server_list[i].startup.command_line = CORBA::string_dup (command_line.c_str ());
      server_list[i].startup.working_directory = CORBA::string_dup (working_directory.c_str ());
      server_list[i].location.host = CORBA::string_dup (host.c_str ());
      server_list[i].location.port = port;
    }


  // Now work on the iterator

  // Only set up an iterator if we need to
  if (this->repository_.get_repository_size () > how_many)
    {
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "list: Creating ServerInformationIterator\n"));

      // Create an ir_iter and give it the server_iter pointer
      IR_Iterator *ir_iter;

      // @@ The iterator object should be put in their own POA (a transient poa)
      ACE_NEW_THROW_EX (ir_iter,
                        IR_Iterator (server_iter.release (), this->orb_manager_.root_poa ()),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      ACE_TRY
        {
          CORBA::String_var str =
            this->orb_manager_.activate (ir_iter, ACE_TRY_ENV);
          ACE_TRY_CHECK;
          server_iterator = ir_iter->_this (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
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
  ACE_TString server_object_ior, host;
  unsigned short port;

  // Find out if it is already running
  if (this->repository_.get_running_info (server, host, port, server_object_ior) != 0)
    {
      // If we had problems getting the server_object_ior, probably meant that
      // there is no <server> registered
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find ServerObject IOR for server <%s>\n",
                  server));
      ACE_THROW (ImplementationRepository::Administration::NotFound ());
    }

  // Check to see if there is one running (if there is a server_object_ior)
  if (server_object_ior.length () != 0)
    {
      // It is running, so shut it down
      ACE_TRY
        {
          CORBA::Object_var object =
            this->orb_manager_.orb ()->string_to_object (server_object_ior.c_str (),
                                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ImplementationRepository::ServerObject_var server_object = 
            ImplementationRepository::ServerObject::_narrow (object.in (), ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (server_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Invalid ServerObject IOR: <%s>\n",
                          server_object_ior));
              ACE_TRY_THROW (ImplementationRepository::Administration::NotFound ());
            }

          // Call shutdown
          server_object->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Remove running info from repository
          if (this->repository_.update (server, "", 0, "") != 0)
            {
              ACE_ERROR ((LM_ERROR,
                         "Error: Could not update information for unknown server <%s>\n",
                         server));
              ACE_TRY_THROW (ImplementationRepository::Administration::NotFound ());
            }          
        }
      ACE_CATCHANY
        {
          ACE_RETHROW;
        }
      ACE_ENDTRY;
    }
}


IR_Adapter_Activator::IR_Adapter_Activator (IR_Forwarder *servant,
                                            PortableServer::POAManager_ptr poa_manager)
  : servant_ (servant),
    poa_manager_ (PortableServer::POAManager::_duplicate (poa_manager))
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
                                                          this->poa_manager_.in (),
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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "System Exception");
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
IR_Forwarder::_primary_interface (const PortableServer::ObjectId &,
                                  PortableServer::POA_ptr,
                                  CORBA::Environment &)
{
  return 0;
}

void
IR_Forwarder::invoke (CORBA::ServerRequest_ptr ,
                      CORBA::Environment &ACE_TRY_ENV)
{
  TAO_ORB_Core *orb_core = this->orb_var_->orb_core ();
  TAO_POA_Current_Impl *poa_current_impl = orb_core->poa_current ().implementation ();

  // The servant determines the key associated with the database entry
  // represented by self
  PortableServer::ObjectId_var oid = poa_current_impl->get_object_id (ACE_TRY_ENV);
  ACE_CHECK;

  // Now convert the id into a string
  CORBA::String_var key = PortableServer::ObjectId_to_string (oid.in ());

  PortableServer::POA_ptr poa = poa_current_impl->get_POA (ACE_TRY_ENV);
  ACE_CHECK;

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
    this->orb_var_->key_to_object (poa_current_impl->object_key (),
                                   0,
                                   ACE_TRY_ENV);
  ACE_CHECK;

  TAO_Stub *stub_obj = ACE_dynamic_cast (TAO_Stub *,
                                         forward_object->_stubobj ());

  TAO_IIOP_Profile *iiop_pfile =
            ACE_dynamic_cast (TAO_IIOP_Profile *,
                              stub_obj->profile_in_use ());

  iiop_pfile->port (this->ir_impl_->get_forward_port (poa->the_name ()));
  iiop_pfile->host (this->ir_impl_->get_forward_host (poa->the_name ()));

  if (!CORBA::is_nil (forward_object))
    ACE_THROW (PortableServer::ForwardRequest (forward_object));
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
  ACE_NEW_THROW_EX (server_list,
                    ImplementationRepository::ServerInformationList (0),
                    CORBA::NO_MEMORY ());
  
  // If there are no more bindings...
  if (this->iterator_->done ())
    return 0; // Return false

  // Initially assume that iterator has the requested number of
  // bindings.
  server_list->length (how_many);
  
  Server_Repository::HASH_IR_ENTRY *server_entry;
  
  // Iterate and populate the BindingList.
  
  for (CORBA::ULong i = 0; i < how_many; i++)
    {
      this->iterator_->next (server_entry);
    
      ACE_TString logical, server, command_line, working_directory, host, server_ior;
      unsigned short port;

      server_entry->int_id_->get_running_info (host, port, server_ior);
      server_entry->int_id_->get_startup_info (logical, command_line, working_directory);

      server_list[i].logical_server = CORBA::string_dup (logical.c_str ());
      server_list[i].server = CORBA::string_dup (server_entry->ext_id_.c_str ());
      server_list[i].startup.command_line = CORBA::string_dup (command_line.c_str ());
      server_list[i].startup.working_directory = CORBA::string_dup (working_directory.c_str ());
      server_list[i].location.host = CORBA::string_dup (host.c_str ());
      server_list[i].location.port = port;

      if (this->iterator_->advance () == 0)
        {
          // If no more server left, reset length to the actual
          // number servers and get out of the loop.
          server_list->length (i + 1);
          break;
        }
    }
  return 1;
}


// Destroys the iterator.

void IR_Iterator::destroy (CORBA::Environment &ACE_TRY_ENV)
{
}
