// "$Id$"

#include "ImR_Activator_i.h"

#include "Iterator.h"
#include "Activator_Options.h"
#include "utils.h"

#include "tao/PortableServer/ImR_LocatorC.h"
#include "tao/ORB_Core.h"

#include "ace/ARGV.h"
//#include "tao/default_ports.h"
#include "ace/OS_NS_netdb.h"
//#include "ace/OS_NS_sys_time.h"

ACE_RCSID (ImplRepo_Service,
           ImR_Activator_i,
           "$Id$")

static bool
is_alive(CORBA::ORB_ptr orb, const char* ior ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  ACE_ASSERT(! CORBA::is_nil(orb));
  ACE_ASSERT(ior != 0);

  CORBA::Object_var obj = orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(false);

  if (CORBA::is_nil(obj.in()))
  {
    ACE_ERROR ((LM_ERROR,
      "ImR Activator: Invalid ServerObject IOR: <%s>\n", ior));
    ACE_THROW_RETURN (ImplementationRepository::NotFound (), false);
  }

  ACE_TRY
  {
    ImplementationRepository::ServerObject_var server_object =
      ImplementationRepository::ServerObject::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil (server_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
        "ImR Activator: Invalid ServerObject IOR: <%s>\n", ior));
      ACE_TRY_THROW(ImplementationRepository::NotFound());
    }

    // Check to see if we can ping it
    // Note : This call could take an extremely long time if the server is
    // not responsive. For example, all of it's threads could be processing
    // other operations. We should set an extremely short timeout, and then
    // assume the server is alive if our ping times out. 
    server_object->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCH(ImplementationRepository::NotFound, ex)
  {
    ACE_RE_THROW;
  }
  ACE_CATCH(CORBA::TRANSIENT, ex)
  {
    const CORBA::ULong BITS_5_THRU_12_MASK = 0x00000f80;
    switch (ex.minor() & BITS_5_THRU_12_MASK)
      {
      case TAO_INVOCATION_SEND_REQUEST_MINOR_CODE:
        return false;
      case TAO_POA_DISCARDING:
      case TAO_POA_HOLDING:
        return true;
      default:
        ACE_ERROR ((LM_ERROR,
                    "ImR_Activator_i::is_alive(): unexpected TRANSIENT minor code (%d)\n",
                    ex.minor()));
        return false;
      }
  }
  ACE_CATCH(CORBA::TIMEOUT, ex)
  {
    // We know it's on localhost, so we can assume that it's just 
    // unresponsive, but alive.
    return true; 
  }
  ACE_CATCHANY
  {
    return false;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN(false);
  return true;
}

static ACE_CString getActivatorName() 
{
  char host_name[MAXHOSTNAMELEN];
  ACE_OS::hostname (host_name, MAXHOSTNAMELEN);
  return host_name;
}

ImR_Activator_i::ImR_Activator_i (void)
: repository_(getActivatorName())
, registration_token_(0)
, debug_(0)
, read_only_(false)
{
}

char *
ImR_Activator_i::find_ior (const char* oname ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(oname != 0);
  // We assume that the first part of the object name is the server name.
  // So a name of foo/bar means that the server name is foo.
  ACE_CString server_name(oname);
  ssize_t pos = server_name.find ('/');
  if (pos != ACE_CString::npos) 
    server_name = server_name.substr(pos + 1);

  ACE_CString ior;
  ACE_TRY
  {
    // Activate the server and return the partial ior
    CORBA::String_var pior = 
      this->activate_server_with_startup (server_name.c_str (), 1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ior = pior.in();
  }
  ACE_CATCHANY
  {
    ACE_TRY_THROW(ImplementationRepository::NotFound());
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Found IOR for %s is %s\n", 
      oname, ior.c_str()));
  }

  // add the object key to make a complete ior
  ior += oname;

  return CORBA::string_dup (ior.c_str ());
}


// Starts the server defined by the POA name <server> if it is
// not already started and if it can be started.
void
ImR_Activator_i::activate_server (const char *server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  ACE_ASSERT(server != 0);
  // Typically, called by the tao_imr utility to activate a server.
  // This is how MANUAL servers typically get started.
  this->activate_server_with_startup (server, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Here, check_startup=true means "This is not a manual activation."
char *
ImR_Activator_i::activate_server_with_startup (const char *server,
                                               int check_startup
                                               ACE_ENV_ARG_DECL)
                                               ACE_THROW_SPEC ((CORBA::SystemException,
                                               ImplementationRepository::NotFound,
                                               ImplementationRepository::CannotActivate))
{
  ACE_ASSERT(server != 0);
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Activate server"
      " %s. Check startup=%d.\n", server, check_startup));
  }
  Server_Info_Ptr info = this->repository_.get_server_info(server);
  if (info.get() == 0) 
  {
    ACE_ERROR ((LM_ERROR,
      "ImR Activator: Cannot find startup info for server <%s>\n",
      server));
    ACE_THROW_RETURN(ImplementationRepository::NotFound (), 0);
  }

  // If a new instance is supposed to be started for each client, then we 
  // must throw away or running info, so that a new one can be started. This
  // makes the implicity assumption that the server is capable of shutting itself
  // down. However, if we are being called from activate(), then we can just
  // return the last server that was registered. (if any)
  if (info->activation_mode == ImplementationRepository::PER_CLIENT && check_startup)
  {
    info->ior = "";
    info->partial_ior = "";
    // No need to save in this case
  }

  if (info->ior.length() != 0) // If one claims to be running
  {
    if (is_alive(this->orb_.in(), info->ior.c_str() ACE_ENV_ARG_PARAMETER))
    {
      ACE_ASSERT(info->partial_ior.length() > 0);
      return CORBA::string_dup (info->partial_ior.c_str());
    }
    else 
    {
      info->ior = "";
      info->partial_ior = "";
      // No need to save in this case
    }
  }

  if (info->activation_mode == ImplementationRepository::MANUAL && check_startup)
  {
    ACE_THROW_RETURN(ImplementationRepository::CannotActivate(), 0);
  }

  if (info->cmdline.length() == 0) // not startable
  {
    ACE_ERROR ((LM_ERROR,
      "ImR Activator: No startup information for server <%s>\n", info->name.c_str()));
    ACE_THROW_RETURN (ImplementationRepository::CannotActivate
      (CORBA::string_dup ("No startup information")), 0);
  }

  // Note: When the server starts, it will update it's information in the ImR. The
  // start_server_i() call pumps our orbs event handler, and therefore any number
  // of corba invocations may be handled, including the updating of
  // the status and partial_ior of our server.

  this->start_server_i (*info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (info->partial_ior.c_str());
}

// Starts the server process
void
ImR_Activator_i::start_server_i (Server_Info& info ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  int spawned_pid = ACE_INVALID_PID;
  if (! info.starting_up)
  {
    info.starting_up = true; 

    if (this->debug_ >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, 
        "ImR Activator: Starting server %s\n", info.name.c_str()));
    }

    // Set the Process parameters.
    ACE_Process_Options proc_opts;
    proc_opts.command_line (info.cmdline.c_str ());
    proc_opts.working_directory (info.dir.c_str ());
    // Win32 does not support the CLOSE_ON_EXEC semantics for sockets
    // the way unix does, so in order to avoid having the child process
    // hold the listen socket open, we force the child to inherit no
    // handles. This includes stdin, stdout, logs, etc.
    proc_opts.handle_inheritence (0);

    for (CORBA::ULong i = 0; i < info.env_vars.length(); ++i)
      proc_opts.setenv (info.env_vars[i].name.in (), info.env_vars[i].value.in ());

    int spawned_pid = this->process_mgr_.spawn (proc_opts);

    if (spawned_pid == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
        "ImR Activator: Cannot activate server <%s> using <%s>\n",
        info.name.c_str(),
        info.cmdline.c_str()));

      // and throw the exception
      ACE_THROW (ImplementationRepository::CannotActivate
        (CORBA::string_dup ("Process Creation Failed")));
    }

    if (this->debug_ >= 2)
    {
      ACE_DEBUG ((LM_DEBUG, 
        "ImR_Activator_i::start_server_i: Process ID is %d\n", spawned_pid));
    }
  }
 
  // Now that the server has been started up, we need to go back into the event
  // loop so we can get the reponse or handle other requests
  TAO_ORB_Core* orb_core = orb_->orb_core();

  ACE_Time_Value timeout = this->startup_timeout_;
  // We will wait till the timeout period for the server to get
  // started up. The starting_up value will be made zero when the
  // server is up and running. And, thats the condition that we are
  // checking.
  if (this->debug_ >= 2)
  {
    ACE_DEBUG ((LM_DEBUG, 
      "ImR_Activator_i::activate_server: Going into handle_events loop\n"));
  }

  while (info.starting_up)
  {
    int result = orb_core->reactor()->handle_events(timeout);
    if (result == -1)
    {
      info.starting_up = false;
      ACE_THROW(ImplementationRepository::CannotActivate
        (CORBA::string_dup ("Error in orb_core->reactor()->handle_events()")));
    }
    if (result == 0 && timeout == ACE_Time_Value::zero)
    {
      ACE_ERROR ((LM_ERROR,
        "ImR Activator: Cannot activate server <%s> using <%s>, "
        "terminating it.\n", info.name.c_str(), info.cmdline.c_str()));

      if (spawned_pid != ACE_INVALID_PID)
        this->process_mgr_.terminate (spawned_pid);

      info.starting_up = false;
      ACE_THROW(ImplementationRepository::CannotActivate
        (CORBA::string_dup ("Timeout waiting for server start.")));
    } 
  }
  
  if (this->debug_ >= 2)
  {
    ACE_DEBUG ((LM_DEBUG, 
    "ImR_Activator_i::activate_server: Got out of handle_events loop\n"));
  }
}

void
ImR_Activator_i::register_server (const char *server,
                                  const ImplementationRepository::StartupOptions &options
                                  ACE_ENV_ARG_DECL)
                                  ACE_THROW_SPEC ((CORBA::SystemException,
                                  ImplementationRepository::AlreadyRegistered,
                                  ImplementationRepository::NotFound
                                  ))
{
  ACE_ASSERT(server != 0);
  if (this->read_only_)
  {
    ACE_THROW (CORBA::NO_PERMISSION (
      CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
      CORBA::COMPLETED_NO));
  }

  if (this->debug_ >= 1)
    ACE_DEBUG((LM_DEBUG, "Imr Activator: Register server %s.\n", server));

  int status = this->repository_.add (server,
    options.command_line.in (),
    options.environment,
    options.working_directory.in (),
    options.activation);

  if (status == 1)
  {
    ACE_ERROR ((LM_ERROR,
      "ImR Activator: Server %s Already Registered!\n", server));
    ACE_THROW (ImplementationRepository::AlreadyRegistered());
  }

  if (this->debug_ >= 2)
  {
    ACE_DEBUG ((LM_DEBUG,
      "ImR_Activator_i::register_server:\nServer: %s\n"
      "Command Line: %s\n"
      "Working Directory: %s\n"
      "Activation Mode: %s\n\n",
      server,
      options.command_line.in (),
      options.working_directory.in (),
      ImR_Utils::activationModeToString(options.activation).c_str()));

    for (CORBA::ULong i = 0; i < options.environment.length (); ++i)
      ACE_DEBUG ((LM_DEBUG, "Environment variable %s=%s\n",
      options.environment[i].name.in (),
      options.environment[i].value.in ()));
  }
}

// This operation used used by tao_imr update. The only difference, between
// this operation and register_server() is that any existing server info
// will be replaced, while register_server() will throw an exception.
void
ImR_Activator_i::reregister_server (const char *server,
                                    const ImplementationRepository::StartupOptions& options
                                    ACE_ENV_ARG_DECL)
                                    ACE_THROW_SPEC ((CORBA::SystemException,
                                    /* Note : Locator only, not thrown here */
                                    ImplementationRepository::AlreadyRegistered,
                                    ImplementationRepository::NotFound
                                    ))
{
  ACE_ASSERT(server != 0);
  if (this->debug_ >= 1)
    ACE_DEBUG((LM_DEBUG, "ImR Activator: Reregister server %s.\n", server));

  if (this->read_only_)
  {
    ACE_THROW (CORBA::NO_PERMISSION (
      CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
      CORBA::COMPLETED_NO));
  }

  Server_Info_Ptr info = this->repository_.get_server_info(server);
  if (info.get() == 0) 
  {
    this->repository_.add (server,
      options.command_line.in (),
      options.environment,
      options.working_directory.in (),
      options.activation);
  } 
  else 
  {
    // Note : It's possible to reregister while the server is in the middle
    // of starting up. In this case, any new StartupOptions won't take affect
    // until the next time the server is started.

    info->cmdline = options.command_line.in();
    info->env_vars = options.environment;
    info->dir = options.working_directory.in();
    info->activation_mode = options.activation;
    this->repository_.update(*info);
  }

  if (this->debug_ >= 2)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::reregister_server:\nServer: %s\n"
      "Command Line: %s\n"
      "Working Directory: %s\n"
      "Activation: %s\n\n",
      server,
      options.command_line.in (),
      options.working_directory.in (),
      ImR_Utils::activationModeToString(options.activation).c_str()));

    for (CORBA::ULong i = 0; i < options.environment.length (); ++i)
      ACE_DEBUG ((LM_DEBUG, "Environment variable %s=%s\n",
      options.environment[i].name.in (),
      options.environment[i].value.in ()));
  }
}


// Remove the server entry from the Repository
void
ImR_Activator_i::remove_server (const char *server
                                ACE_ENV_ARG_DECL)
                                ACE_THROW_SPEC ((CORBA::SystemException,
                                ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  if (this->read_only_)
  {
    ACE_THROW (CORBA::NO_PERMISSION (
      CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
      CORBA::COMPLETED_NO));
  }

  // Note : This will be safe, because any Server_Info_Ptr objects will still
  // be valid, and the actual Server_Info will be destroyed when the last
  // one goes out of scope.

  if (this->repository_.remove (server) == 0)
  {
    if (this->debug_ >= 1)
      ACE_DEBUG ((LM_DEBUG, "ImR Activator: Removed Server %s.\n", server));
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "ImR Activator: Can't remove unknown server %s.\n", server));
    ACE_THROW (ImplementationRepository::NotFound ());
  }
}

// Register the current partial_ior of the server. Also updates the callback object.
// If the server is unknown, then it will be automatically added with default settings.
void
ImR_Activator_i::server_is_running (const char *server,
                                    const char *partial_ior,
                                    ImplementationRepository::ServerObject_ptr server_object
                                    ACE_ENV_ARG_DECL)
                                    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_ASSERT(partial_ior != 0);
  ACE_ASSERT(! CORBA::is_nil(server_object));
  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Server %s is running.\n", server));

  if (this->debug_ >= 2)
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::server_is_running: at %s\n", partial_ior));

  CORBA::String_var server_object_ior =
    orb_->object_to_string (server_object ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Update the status of the server in the repository.
  Server_Info_Ptr info = this->repository_.get_server_info(server);
  if (info.get() == 0) 
  {
    if (this->debug_ >= 1)
      ACE_DEBUG ((LM_DEBUG, "ImR Activator: Auto added NORMAL server:\n", server));

    ImplementationRepository::EnvironmentList env(0);
    this->repository_.add (server,
      "",
      env,
      "",
      ImplementationRepository::NORMAL,
      partial_ior,
      server_object_ior.in());
  } 
  else 
  {
    info->ior = server_object_ior.in();
    info->partial_ior = partial_ior;
    // We are likely in a nested call to start_server_i(), and this will 
    // allow us to break out.
    info->starting_up = false;
    this->repository_.update(*info);
  }
}

// Remove the state information for the current server
void
ImR_Activator_i::server_is_shutting_down (const char *server ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  Server_Info_Ptr info = this->repository_.get_server_info(server);
  if (info.get() == 0) 
  {
    if (this->debug_ >= 2)
    {
      ACE_DEBUG((LM_DEBUG, 
      "ImR_Activator_i::server_is_shutting_down: Unknown server:%s\n", server));
    }
    return;
  }

  if (this->debug_ >= 2)
    ACE_DEBUG((LM_DEBUG, "ImR_Activator_i::server_is_shutting_down: %s\n", server));

  info->ior = "";
  info->partial_ior = "";
  info->starting_up = false;
  this->repository_.update(*info);
}

int 
ImR_Activator_i::init_with_orb(CORBA::ORB_ptr orb, const Options& opts ACE_ENV_ARG_DECL)
{
  ACE_ASSERT(! CORBA::is_nil(orb));
  orb_ = CORBA::ORB::_duplicate(orb);
  debug_ = opts.debug();
  read_only_ = opts.readonly();
  startup_timeout_ = opts.startup_timeout();

  ACE_TRY
  {
    if (this->debug_ >= 1)
      ACE_DEBUG((LM_DEBUG, "Contacting ImplRepoService...\n"));

    // First, resolve the ImR Locator, without this we can go no further
    CORBA::Object_var obj = 
      orb->resolve_initial_references ("ImplRepoService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        "Unable to resolve_initial_references(\"ImplRepoService\")\n"), -1);
    }
    ImplementationRepository::Locator_var locator =
      ImplementationRepository::Locator::_narrow (obj.in ()
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(locator.in()));

    // Create a new poa and poa manager
    obj = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(obj.in()));
    this->root_poa_ = PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(this->root_poa_.in()));

    CORBA::PolicyList policies;
    this->imr_poa_ = this->root_poa_->create_POA("ImR_Activator", 
      PortableServer::POAManager::_nil(), policies ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(this->imr_poa_.in()));

    // Activate ourself
    PortableServer::ObjectId_var id = 
      this->imr_poa_->activate_object(this ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    obj = this->imr_poa_->id_to_reference (id.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ImplementationRepository::Administration_var activator = 
      ImplementationRepository::Administration::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(activator.in()));
    CORBA::String_var ior = this->orb_->object_to_string(activator.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG((LM_DEBUG, "Starting activator : %s\n", this->repository_.activator_name()));

    // initialize our process manager.
//    ACE_Reactor *reactor = orb_->orb_core ()->reactor ();
    ACE_Reactor *reactor =  ACE_Reactor::instance ();

    if (reactor != 0)
    {
      if (this->process_mgr_.open (ACE_Process_Manager::DEFAULT_SIZE, reactor) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          "The ACE_Process_Manager didnt get initialized\n"),
          -1);
      }
    }
    reactor = orb_->orb_core ()->reactor ();

    // Initialize the persistent storage. This will load any values that 
    // may have been persisted before.
    if (this->repository_.init(opts.repository_mode(), opts.file_name()) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Repository failed to initialize\n"), -1);
    }

    // We need to send the list of our persisted server names to the Locator
    // so that it knows we are managing them.
    CORBA::ULong cnt = this->repository_.servers().current_size();
    ImplementationRepository::Locator::ServerNameList server_names(cnt);
    if (cnt > 0) 
    {
      server_names.length(cnt);
      Server_Repository::SIMap::ENTRY* next_entry = 0;
      Server_Repository::SIMap::ITERATOR server_iter(this->repository_.servers());
      for (CORBA::ULong idx = 0; server_iter.next(next_entry) != 0; server_iter.advance())
      {
        ACE_CString server_name = next_entry->ext_id_;
        server_names[idx++] = server_name.c_str();
      }
    }

    // Register with the ImR Locator
    this->registration_token_ =
      locator->register_activator (this->repository_.activator_name(), 
      activator.in(), server_names ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POAManager_var poaman =
      this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    poaman = this->imr_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (this->debug_ >= 2)
    {
      ACE_DEBUG ((LM_DEBUG,
        "ImR_Activator_i::init: The Activator IOR is: <%s>\n", ior.in ()));
    }

    // The last thing we do is write out the ior so that a test program can assume
    // that the activator is ready to go as soon as the ior is written.
    if (opts.ior_filename().length() > 0)
    {
      FILE* fp = ACE_OS::fopen(opts.ior_filename().c_str(), "w");
      if (fp == 0) 
      {
        ACE_ERROR_RETURN ((LM_ERROR, 
          "ImR Activator: Could not open file: %s\n", opts.ior_filename().c_str()), -1);
      }
      ACE_OS::fprintf(fp, "%s", ior.in());
      ACE_OS::fclose(fp);
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Activator_i::init_with_orb");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Activator_i::init (Options& opts ACE_ENV_ARG_DECL)
{
  ACE_ARGV av(opts.cmdline().c_str());
  int argc = av.argc();
  char** argv = av.argv();
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "TAO_ImR_Activator" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  int ret = this->init_with_orb(orb.in(), opts ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  return ret;
}

int
ImR_Activator_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
  {
    this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->orb_->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Activator_i::fini");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Activator_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->auto_start_servers(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->orb_->run(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

void
ImR_Activator_i::auto_start_servers(ACE_ENV_SINGLE_ARG_DECL)
{
  // Note : This will have the side effect of running the orb reactor
  // as it tries to wait for the servers to start and register themselves
  // back with the activator.

  if (this->debug_ >= 2)
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::run: Activating AUTO_START servers\n"));

  Server_Repository::SIMap::ENTRY* server_entry;
  Server_Repository::SIMap::ITERATOR server_iter(this->repository_.servers());
  // For each of the entries in the Server_Repository, get the startup
  // information and activate the servers, if they are not already
  // running.
  for (;server_iter.next(server_entry) != 0; server_iter.advance())
  {
    Server_Info_Ptr info = server_entry->int_id_;
    ACE_ASSERT(info.get() != 0);

    ACE_TRY
    {
      if (info->activation_mode == ImplementationRepository::AUTO_START
        && info->cmdline.length() > 0)
      {
        this->start_server_i (*info ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    }
    ACE_CATCHANY
    {
      if (this->debug_ >= 2)
      {
        ACE_DEBUG ((LM_DEBUG,
          "ImR_Activator_i::run: AUTO_START Could not activate <%s>\n",
          server_entry->ext_id_.c_str ()));
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "AUTO_START");
      }
      // Ignore exceptions
    }
    ACE_ENDTRY;
    ACE_CHECK;
  }
}
// Returns the startup information for a server

void
ImR_Activator_i::find (const char *server,
                       ImplementationRepository::ServerInformation_out imr_info
                       ACE_ENV_ARG_DECL)
                       ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_NEW_THROW_EX (imr_info, ImplementationRepository::ServerInformation, CORBA::NO_MEMORY ());

  Server_Info_Ptr info = this->repository_.get_server_info(server);
  if (info.get() == 0) 
  {
    ACE_ERROR ((LM_ERROR,
      "ImR Activator: Cannot find startup info for server <%s>\n",
      server));
    ACE_THROW(ImplementationRepository::NotFound());
  }

  imr_info->server = info->name.c_str();
  imr_info->startup.command_line = info->cmdline.c_str();
  imr_info->startup.environment = info->env_vars;
  imr_info->startup.working_directory = info->dir.c_str();
  imr_info->startup.activation = info->activation_mode;
  imr_info->startup.activator = this->repository_.activator_name();
  imr_info->partial_ior = info->partial_ior.c_str();

  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Found server %s.\n", server));
}


// Used to access the list of servers registered.  May also return an
// iterator which can be used to access more than <how_many> of them.

void
ImR_Activator_i::list (CORBA::ULong how_many,
                       ImplementationRepository::ServerInformationList_out server_list,
                       ImplementationRepository::ServerInformationIterator_out server_iterator
                       ACE_ENV_ARG_DECL)
                       ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: List servers.\n"));

  // Initialize the out variables, so if we return early, they will
  // not be dangling.
  server_iterator = ImplementationRepository::ServerInformationIterator::_nil();
  ACE_NEW_THROW_EX (server_list, 
    ImplementationRepository::ServerInformationList(0), CORBA::NO_MEMORY());

  if (how_many == 0)
    return;

  Server_Repository::SIMap::ENTRY* entry = 0;
  Server_Repository::SIMap::ITERATOR it(this->repository_.servers());

  // Number of servers that will go into the server_list.
  CORBA::ULong n = this->repository_.servers().current_size();
  if (n > how_many)
  {
    n = how_many;
  }

  server_list->length(n);

  if (this->debug_ >= 2)
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::list: Filling ServerList with %d servers\n", n));

  for (CORBA::ULong i = 0; i < n; i++)
  {
    it.next(entry);
    it.advance();
    ACE_ASSERT(entry != 0);

    Server_Info_Ptr info = entry->int_id_;

    server_list[i].server = info->name.c_str();
    server_list[i].startup.command_line = info->cmdline.c_str();
    server_list[i].startup.environment = info->env_vars;
    server_list[i].startup.working_directory = info->dir.c_str();
    server_list[i].startup.activation = info->activation_mode;
    server_list[i].startup.activator = this->repository_.activator_name();
    server_list[i].partial_ior = info->partial_ior.c_str();
  }

  if (this->repository_.servers().current_size() > how_many)
  {
    if (this->debug_ >= 2)
      ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::list: Creating ServerInformation Iterator\n"));

    ImR_Iterator* imr_iter;

    ACE_NEW_THROW_EX (imr_iter,
      ImR_Iterator (how_many, this->repository_, this->imr_poa_.in()),
      CORBA::NO_MEMORY ());

    ACE_TRY
    {
      PortableServer::ObjectId_var id =
        this->imr_poa_->activate_object (imr_iter ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var obj = this->imr_poa_->id_to_reference(id.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      server_iterator = ImplementationRepository::
        ServerInformationIterator::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
    ACE_ENDTRY;
    ACE_CHECK;
  }
}


/**
 * Attempt to gracefully shut down the server, if that fails, will try
 * to do it ungracefully.
 */
void
ImR_Activator_i::shutdown_server (const char *server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Shutting down server.\n"));

  Server_Info_Ptr info = this->repository_.get_server_info(server);
  if (info.get() == 0) 
  {
    ACE_ERROR ((LM_ERROR,
      "ImR Activator: Cannot find startup info for server <%s>\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }

  // Check to see if there is one running (if there is a server_object_ior)
  if (info->ior.length() != 0)
  {
    CORBA::Object_var obj = 
      this->orb_->string_to_object (info->ior.c_str() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR, 
        "ImR Activator: Invalid ServerObject IOR: <%s>\n", info->ior.c_str()));
      ACE_THROW (ImplementationRepository::NotFound());
    }
    ImplementationRepository::ServerObject_var server_obj =
      ImplementationRepository::ServerObject::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    if (CORBA::is_nil (server_obj.in ()))
    {
      ACE_ERROR ((LM_ERROR, "ImR Activator: Couldn't narrow server obj.\n"));
      ACE_THROW (ImplementationRepository::NotFound());
    }

    server_obj->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    info->ior = "";
    info->partial_ior = "";
    this->repository_.update(*info);
  }
}
