// $Id$

#include "ImplRepo_i.h"
#include "Forwarder.h"
#include "Adapter_Activator.h"
#include "Locator.h"
#include "Iterator.h"
#include "Options.h"

#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/ORB_Core.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"

#include "ace/Auto_Ptr.h"

const char *
convert_str (ImplementationRepository::ActivationMode mode)
{
  switch (mode)
    {
    case ImplementationRepository::NORMAL:
      return "NORMAL";
    case ImplementationRepository::MANUAL:
      return "MANUAL";
    case ImplementationRepository::PER_CLIENT:
      return "PER_CLIENT";
    case ImplementationRepository::AUTO_START:
      return "AUTO_START";
    default:
      return "UNKNOWN";
    };
}

// Constructor

ImplRepo_i::ImplRepo_i (void)
  : locator_ (0),
    forwarder_impl_ (0),
    activator_ (0),
    ior_multicast_ (0)
{
  this->locator_ = new ImR_Locator (this);
}

char *
ImplRepo_i::find_ior (const ACE_CString &object_name
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, IORTable::NotFound))
{
  ACE_TString endpoint;
  ACE_TString poa_name;
  // We assume that the first part of the object name is the poa name.
  // So we would think that a name of foo/bar means that the POA name
  // is foo.

  int pos = object_name.find ('/');

  if (pos == object_name.npos)
    pos = object_name.length ();

  poa_name.set (object_name.fast_rep (), pos, 1);

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "find_ior: poa name <%s>, %d\n", poa_name.c_str (), pos));

  ACE_TRY
    {
      endpoint = this->activate_server_i (poa_name.c_str (), 1
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_THROW_RETURN (IORTable::NotFound (), 0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  // Have to do this so it is null terminated
  ACE_TString object_name2 (object_name.fast_rep (),
                            object_name.length ());

  ACE_CString ior = endpoint;
  ior += object_name2;

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "find_ior: new ior is <%s>\n", endpoint.c_str ()));

  return CORBA::string_dup (ior.c_str ());
}


// Starts the server defined by the POA name <server> if it is
// not already started and if it can be started.

void
ImplRepo_i::activate_server (const char *server
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound,
                     ImplementationRepository::Administration::CannotActivate))
{
  // Since this is called through the Admin interface, we should ignore some
  // of the activation modes.  Also ignore the return value.
  this->activate_server_i (server, 0 ACE_ENV_ARG_PARAMETER);
}


ACE_TString
ImplRepo_i::activate_server_i (const char *server,
                               const int check_startup
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound,
                     ImplementationRepository::Administration::CannotActivate))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  int start = 0;
  ACE_TString server_object_ior, location;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "Activating Server: %s\n", server));

  ACE_TString logical, startup, working;
  ImplementationRepository::EnvironmentList environment;
  ImplementationRepository::ActivationMode activation;
  if (this->repository_.get_startup_info (server,
                                          logical,
                                          startup,
                                          environment,
                                          working,
                                          activation) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find startup info for server <%s>\n",
                  server));
      ACE_THROW_RETURN(ImplementationRepository::Administration::NotFound (), "");
    }

  // Find out if it is already running
  if (this->repository_.get_running_info (server, location, server_object_ior) != 0)
    {
      // If we had problems getting the server_object_ior, probably meant that
      // there is no <server> registered
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find ServerObject IOR for server <%s>\n",
                  server));
      ACE_THROW_RETURN (ImplementationRepository::Administration::NotFound (), "");
    }

  // Check to see if there is one running (if there is a server_object_ior)
  if (server_object_ior.length () != 0)
    {
      // It is running
      ACE_TRY
        {
          CORBA::Object_var object =
            orb->string_to_object (server_object_ior.c_str () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ImplementationRepository::ServerObject_var server_object =
            ImplementationRepository::ServerObject::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (server_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Invalid ServerObject IOR: <%s>\n",
                          server_object_ior.c_str ()));
              ACE_THROW_RETURN (ImplementationRepository::Administration::NotFound (), "");
            }

          // Check to see if we can ping it
          server_object->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
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
      // Make sure the activation allows us to start it up.
      if (activation == ImplementationRepository::MANUAL && check_startup)
        ACE_THROW_RETURN (CORBA::TRANSIENT (
            CORBA_SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE,
                                                    0),
            CORBA::COMPLETED_NO),
          "");

      this->start_server_i (server);
      ACE_CHECK_RETURN ("");
    }

  if (this->repository_.get_running_info (server, location, server_object_ior) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ImplRepo_i::activate_server: cannot resolve server <%s>\n",
                  server));
    }

  if (activation == ImplementationRepository::PER_CLIENT && check_startup)
  {
    if (this->repository_.update (server, "", "") != 0)
      {
        ACE_ERROR ((LM_ERROR,
                   "Error: Could not update information for server <%s>\n",
                   server));
        ACE_THROW_RETURN (ImplementationRepository::Administration::NotFound (), "");
      }
  }

  return location;
}


// Starts the server process

void
ImplRepo_i::start_server_i (const char *server
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound,
                     ImplementationRepository::Administration::CannotActivate))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  int spawned_pid = 0;
  ACE_TString logical, startup, working;
  ImplementationRepository::EnvironmentList environment;
  ImplementationRepository::ActivationMode activation;
  if (this->repository_.get_startup_info (server,
                                          logical,
                                          startup,
                                          environment,
                                          working,
                                          activation) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find startup info for server <%s>\n",
                  server));

      ACE_THROW (ImplementationRepository::Administration::NotFound ());
      ACE_CHECK;
    }

  // Check to see if it is already starting up
  int startup_val = this->repository_.starting_up (server, 1);

  if (startup_val == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find startup info for server <%s>\n",
                  server));

      ACE_THROW (ImplementationRepository::Administration::NotFound ());
      ACE_CHECK;
    }

  if (startup_val == 0)
    {
      if (startup.length () == 0)
        {
          // If there is no startup information, throw an exception
          ACE_ERROR ((LM_ERROR,
                      "Error: No startup information for server <%s>\n",
                      server));
          ACE_THROW (ImplementationRepository::Administration::CannotActivate
            (CORBA::string_dup ("No startup information")));
          ACE_CHECK;
        }

      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG, "Starting %s\n", server));

      ACE_Process_Options proc_opts;

      proc_opts.command_line (startup.c_str ());
      proc_opts.working_directory (working.c_str ());
      proc_opts.avoid_zombies (1);
      proc_opts.handle_inheritence (0);
      // Win32 does not support the CLOSE_ON_EXEC semantics for sockets
      // the way unix does, so in order to avoid having the child process
      // hold the listen socket open, we force the child to inherit no
      // handles. This includes stdin, stdout, logs, etc.

      for (size_t i = 0; i < environment.length(); ++i)
        proc_opts.setenv (environment[i].name.in (), environment[i].value.in ());

      spawned_pid = this->process_mgr_.spawn (proc_opts);

      if (spawned_pid == ACE_INVALID_PID)
        {
          ACE_ERROR ((LM_ERROR,
                     "Error: Cannot activate server <%s> using <%s>\n",
                      server,
                      startup.c_str ()));
          ACE_THROW (ImplementationRepository::Administration::CannotActivate
            (CORBA::string_dup ("Process Creation Failed")));
          ACE_CHECK;
        }
      else if (OPTIONS::instance ()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "Process ID is %d\n", spawned_pid));
    }

  // Now that the server has been started up, we need to go back into the event
  // loop so we can get the reponse or handle other requests
  TAO_ORB_Core *orb_core = orb->orb_core ();

  int starting_up;

  ACE_Time_Value timeout = OPTIONS::instance ()->startup_timeout ();

  while ((starting_up = this->repository_.starting_up (server)) == 1)
    {
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "activate_server: Going into handle_events\n"));

      int result = orb_core->reactor ()->handle_events (&timeout);

      if (result == 0 && timeout == ACE_Time_Value::zero)
        {
          this->repository_.starting_up (server, 0);
          ACE_ERROR ((LM_ERROR,
                     "Error: Cannot activate server <%s> using <%s>, "
                     "terminating it.\n",
                      server,
                      startup.c_str ()));

          // Kill the server
          this->process_mgr_.terminate (spawned_pid);

          ACE_THROW(ImplementationRepository::Administration::CannotActivate
            ("Timeout"));
          ACE_CHECK;
        }
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
      ACE_CHECK;
    }

  // Now check to see if it is responding yet.

  int ready_error = this->ready_check (server);
  if (ready_error < 0)
    {
      // We got an error in ready_check, so shutdown the server and
      // throw an exception

      this->process_mgr_.terminate (spawned_pid);

      if (ready_error == -1)
        {
          ACE_THROW (ImplementationRepository::Administration::CannotActivate
            (CORBA::string_dup ("Timeout while pinging for readiness")));
        }
      else
        {
          ACE_THROW (ImplementationRepository::Administration::CannotActivate
            (CORBA::string_dup ("Unknown error")));

        }
    }
}


// ready_check will continuously ping a server and either return when it
// responds to the ping or return -1 if it times out.

int
ImplRepo_i::ready_check (const char *server)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  ACE_TString ping_object_ior, location;
  ImplementationRepository::ServerObject_var ping_object;

  ACE_DECLARE_NEW_CORBA_ENV;

  // <end> is the end of the window where we can get a response before
  // timing out
  ACE_Time_Value end = ACE_OS::gettimeofday ()
                       + OPTIONS::instance ()->startup_timeout ();

  // Get the ior for the "ping" object for the server

  if (this->repository_.get_running_info (server, location, ping_object_ior)
       != 0)
    {
      // If get_running_info fails, something weird must have happened.
      // Maybe it was removed after we started it up, but before we got here.
      ACE_ERROR ((LM_ERROR,
                  "Error: Cannot find ServerObject IOR for server <%s>\n",
                  server));

      ACE_THROW_RETURN (
          ImplementationRepository::Administration::NotFound (),
          -2
        );
    }

  // Narrow the ping object

  ACE_TRY_EX (ping1)
    {
      CORBA::Object_var object =
        orb->string_to_object (ping_object_ior.c_str () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (ping1);

      ping_object =
        ImplementationRepository::ServerObject::_narrow (object.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (ping1);

      if (CORBA::is_nil (ping_object.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Error: Invalid ServerObject IOR: <%s>\n",
                      ping_object_ior.c_str ()));

          ACE_THROW_RETURN (
              ImplementationRepository::Administration::NotFound (),
              -2
            );
        }

  }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                 "Error: Cannot activate server <%s>, "
                 "terminating it (Server Ping Object failed).\n",
                  server));

      return -2;
    }
  ACE_ENDTRY;

  // Now ping it until we get a response.

  while (ACE_OS::gettimeofday () < end)
    {
      ACE_TRY_EX (ping2);
        {
          // Check to see if we can ping it

          if (OPTIONS::instance()->debug () >= 2)
            ACE_DEBUG ((LM_DEBUG, "Pinging Server...\n"));

          ping_object->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (ping2);

          if (OPTIONS::instance()->debug () >= 2)
            ACE_DEBUG ((LM_DEBUG, "Pinged Server OK\n"));

          // If we got here, we successfully pinged, therefore we
          // can exit the function.
          return 0;
        }
      ACE_CATCHANY
        {
          // Ignore the exception
          if (OPTIONS::instance()->debug () >= 2)
             ACE_DEBUG ((LM_DEBUG, "Server not ready (Exception)\n"));
        }
      ACE_ENDTRY;

      // Sleep between sending pings.
      ACE_OS::sleep (OPTIONS::instance ()->ping_interval ());
    }

  // If we fall out here, that means we didn't get a response before timing
  // out, so return an error.
  return -1;
}


// Adds an entry to the Repository about this <server>

void
ImplRepo_i::register_server (const char *server,
                             const ImplementationRepository::StartupOptions &options
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::AlreadyRegistered))
{
  if (OPTIONS::instance ()->readonly ())
    {
      ACE_THROW (CORBA::NO_PERMISSION ());
    }

  if (OPTIONS::instance ()->debug () >= 2)
    {
      ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                            "Command Line: %s\n"
                            "Working Directory: %s\n"
                            "Activation Mode: %s\n\n",
                            server,
                            options.command_line.in (),
                            options.working_directory.in (),
                            convert_str (options.activation)));

      for (size_t i = 0; i < options.environment.length (); ++i)
         ACE_DEBUG ((LM_DEBUG, "Environment variable %s=%s\n",
                     options.environment[i].name.in (),
                     options.environment[i].value.in ()));
    }

  // Add the server
  int status = this->repository_.add (server,
                                      "",
                                      options.command_line.in (),
                                      options.environment,
                                      options.working_directory.in (),
                                      options.activation);

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
        {
          ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                                "Command Line: %s\n"
                                "Working Directory: %s\n"
                                "Activation Mode: %s\n\n",
                                server,
                                options.command_line.in (),
                                options.working_directory.in (),
                                convert_str (options.activation)));
          for (size_t i = 0; i < options.environment.length(); ++i)
            ACE_DEBUG ((LM_DEBUG, "Environment variable %s=%s\n",
                        options.environment[i].name.in (),
                        options.environment[i].value.in ()));
        }
    }
}


// Updates the entry in the Repository about this <server> or adds it
// if necessary.

void
ImplRepo_i::reregister_server (const char *server,
                               const ImplementationRepository::StartupOptions &options
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (OPTIONS::instance ()->readonly ())
    {
      ACE_THROW (CORBA::NO_PERMISSION ());
    }

  // Get current starting up value
  int starting_up = this->repository_.starting_up (server);

  // Remove old info
  this->repository_.remove (server);

  // Add server with new startup parameters
  this->repository_.add (server,
                         "",
                         options.command_line.in (),
                         options.environment,
                         options.working_directory.in (),
                         options.activation);

  // Set old starting up value, if there was one.
  if (starting_up != -1)
    this->repository_.starting_up (server, starting_up);

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "Server %s Successfully Registered\n",
                server));
  if (OPTIONS::instance()->debug () >= 2)
    {
      ACE_DEBUG ((LM_DEBUG, "Server: %s\n"
                            "Command Line: %s\n"
                            "Working Directory: %s\n"
                            "Activation: %s\n\n",
                            server,
                            options.command_line.in (),
                            options.working_directory.in (),
                            convert_str (options.activation)));
      for (size_t i = 0; i < options.environment.length (); ++i)
          ACE_DEBUG ((LM_DEBUG, "Environment variable %s=%s\n",
                      options.environment[i].name.in (),
                      options.environment[i].value.in ()));
    }
}


// Remove the server entry from the Repository

void
ImplRepo_i::remove_server (const char *server
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound))
{
  if (OPTIONS::instance ()->readonly ())
    {
      ACE_THROW (CORBA::NO_PERMISSION ());
    }

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

char *
ImplRepo_i::server_is_running (const char *server,
                               const char *location,
                               ImplementationRepository::ServerObject_ptr server_object
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  char *new_location = 0;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "Server <%s> is running \n", server));
  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, " at %s\n", location));

  // Get the stringified server_object_ior
  CORBA::String_var server_object_ior = orb->object_to_string (server_object
                                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (this->repository_.update (server, location, server_object_ior.in ()) == 0)
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
      ACE_THROW_RETURN (ImplementationRepository::Administration::NotFound (), new_location);
    }

  TAO_Acceptor_Registry &registry =
    orb->orb_core ()->lane_resources ().acceptor_registry ();

  TAO_MProfile mp;
  TAO_ObjectKey objkey;

  // Use a default acceptor filter, all the profiles in the ImR are valid, no
  // matter what the server has.
  TAO_Default_Acceptor_Filter filter;

  // Allocate space for storing the profiles.  There can never be more
  // profiles than there are endpoints.  In some cases, there can be
  // fewer profiles than endpoints.
  size_t pfile_count =
    registry.endpoint_count ();
  mp.set (pfile_count);

  // Leave it to the filter to decide which acceptors/in which order
  // go into the mprofile.
  filter.fill_profile (objkey,
                       mp,
                       registry.begin (),
                       registry.end ());

  // @@ (brunsch) Only look at current profile for now.
  TAO_Profile *profile = mp.get_current_profile ();

  if (profile)
    {
      new_location = profile->to_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    return new_location;


  // Search for "corbaloc:" alone, without the protocol.  This code
  // should be protocol neutral.
  const char corbaloc[] = "corbaloc:";
  char *pos = ACE_OS::strstr (new_location, corbaloc);
  pos = ACE_OS::strchr (pos + sizeof (corbaloc), ':');

  pos = ACE_OS::strchr (pos + 1, profile->object_key_delimiter ());

  if (pos)
    *(pos + 1) = 0;  // Crop the string
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not parse my own IOR, bailing out.\n"),
                       0);

  this->repository_.starting_up (server, 0);

  return new_location;
}

// Remove the state information for the current server

void
ImplRepo_i::server_is_shutting_down (const char *server
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound))
{
  if (this->repository_.update (server, "", "") == 0)
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
ImplRepo_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  ACE_TRY
    {
      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          adapter->set_locator (this->locator_.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      CORBA::Object_var root_poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to initialize the ROOT POA.\n"),
                          -1);

      // Get the POA object.
      this->root_poa_ = PortableServer::POA::_narrow (root_poa_object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);

      // Id Assignment policy
      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Lifespan policy
      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->imr_poa_ =
        this->root_poa_->create_POA ("ImplRepoService",
                                     poa_manager.in (),
                                     policies
                                     ACE_ENV_ARG_PARAMETER);

      // Warning!  If create_POA fails, then the policies won't be
      // destroyed and there will be hell to pay in memory leaks!
      ACE_TRY_CHECK;

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_NEW_RETURN (this->forwarder_impl_,
                      ImR_Forwarder (this),
                      -1);

      PortableServer::ObjectId_var imr_id =
        PortableServer::string_to_ObjectId ("ImplRepoService");

      this->imr_poa_->activate_object_with_id (imr_id.in (),
                                               this
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the IMR object
      CORBA::Object_var imr_obj =
        this->imr_poa_->id_to_reference (imr_id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // And its string

      this->imr_ior_ =
        orb->object_to_string (imr_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Register with INS.
      adapter->bind ("ImplRepoService", this->imr_ior_.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (OPTIONS::instance ()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "The IMR IOR is: <%s>\n",
                    this->imr_ior_.in ()));

      if (OPTIONS::instance()->output_file ())
        {
          ACE_OS::fprintf (OPTIONS::instance()->output_file (),
                           "%s",
                           this->imr_ior_.in ());
          ACE_OS::fclose (OPTIONS::instance()->output_file ());
        }

      ACE_NEW_RETURN (this->activator_,
                      ImR_Adapter_Activator (this->forwarder_impl_),
                      -1);

      // Register the Adapter_Activator reference to be the RootPOA's
      // Adapter Activator.

      this->root_poa_->the_activator (this->activator_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get reactor instance from TAO.
      ACE_Reactor *reactor = orb->orb_core ()->reactor ();

      // = Set up the process manager

      // Init the Process Manager.
      this->process_mgr_.open (ACE_Process_Manager::DEFAULT_SIZE, reactor);

      // Set up multicast.
      if (this->setup_multicast (reactor) != 0)
        return -1;

      // Initialize the persistent storage
      if (this->repository_.init ())
        ACE_ERROR_RETURN ((LM_ERROR, "Repository failed to initialize\n"), -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Server_i::init");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}



/**
 * First, figure out if we should set up a multicast handler (based on command
 * line settings) and which port to set it up (based on the environment,
 * command line, or the default IMPLREPOSERVICE port).  Then register the
 * handler with the @param reactor.
 *
 * @retval  0 ok
 * @retval -1 failed
 */
int
ImplRepo_i::setup_multicast (ACE_Reactor *reactor)
{
  ACE_ASSERT (reactor);
#if defined (ACE_HAS_IP_MULTICAST)
  if (!OPTIONS::instance ()->multicast ())
    {
      if (OPTIONS::instance ()->debug () > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Implementation Repository: "
                    "Multicast Handler disabled.\n"));

      return 0;
    }

  // Install ior multicast handler.

  TAO_ORB_Parameters *tao_params = TAO_ORB_Core_instance ()->orb_params ();

  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (tao_params->mcast_discovery_endpoint ());

  // First, see if the user has given us a multicast port number
  // on the command-line;
  u_short port = tao_params->service_port (IMPLREPOSERVICE);

  if (port == 0)
    {
      // Check environment var. for multicast port.
      const char *port_number = ACE_OS::getenv ("ImplRepoServicePort");

      if (port_number != 0)
        port = ACE_OS::atoi (port_number);
    }

  // Port wasn't specified on the command-line or in environment -
  // use the default.
  if (port == 0)
    port = TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;

  // Instantiate a handler which will handle client requests for
  // the ImplRepoService ior, received on the multicast port.
  ACE_NEW_RETURN (this->ior_multicast_, TAO_IOR_Multicast (), -1);

  if (mde.length () != 0)
    {
      if (this->ior_multicast_->init (this->imr_ior_.in (),
                                      mde.c_str (),
                                      TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        return -1;
    }
  else
    {
      if (this->ior_multicast_->init (this->imr_ior_.in (),
                                      port,
                                      ACE_DEFAULT_MULTICAST_ADDR,
                                      TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        return -1;
    }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (this->ior_multicast_,
                                 ACE_Event_Handler::READ_MASK) == -1)
    {
      if (OPTIONS::instance ()->debug () > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Implementation Repository: "
                    "cannot register Event handler\n"));
      return -1;
    }

  if (OPTIONS::instance ()->debug () > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Implementation Repository: Multicast Handler enabled.\n"));

  return 0;
#else /* ACE_HAS_IP_MULTICAST */
  ACE_UNUSED_ARG(reactor);
  if (OPTIONS::instance ()->multicast ())
    ACE_ERROR ((LM_ERROR,
                "Implementation Repository: "
                "ACE_HAS_IP_MULTICAST not defined, multicast disabled.\n"));

  return 0;
#endif /* ACE_HAS_IP_MULTICAST */
}


int
ImplRepo_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  ACE_TRY
    {
      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          adapter->set_locator (IORTable::Locator::_nil ()
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      this->imr_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Server_i::init");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImplRepo_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  PortableServer::POAManager_var poa_manager =
    this->imr_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get a new iterator
  auto_ptr<Server_Repository::HASH_IMR_ITER>
    server_iter (this->repository_.new_iterator ());

  // Check for a memory error.
  if (server_iter.get () == 0)
    ACE_THROW_RETURN (CORBA::NO_MEMORY (), -1);

  Server_Repository::HASH_IMR_ENTRY *server_entry;

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "run: Activating AUTO_START servers\n"));

  while (!server_iter->done ())
    {
      server_iter->next (server_entry);
      server_iter->advance ();

      ACE_TString logical, server, command_line, working_directory;
      ImplementationRepository::ActivationMode activation
        = ImplementationRepository::NORMAL;
      ImplementationRepository::EnvironmentList environment_vars ;

      server_entry->int_id_->get_startup_info (logical,
                                               command_line,
                                               environment_vars,
                                               working_directory,
                                               activation);

      ACE_TRY
        {
          if (activation == ImplementationRepository::AUTO_START)
            this->activate_server (server_entry->ext_id_.c_str ()
                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          if (OPTIONS::instance()->debug () >= 2)
            {
              ACE_DEBUG ((LM_DEBUG,
                         "AUTO_START: Could not activate <%s>\n",
                         server_entry->ext_id_.c_str ()));
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "AUTO_START");
            }
          // Ignore exceptions
        }
      ACE_ENDTRY;
    }


  orb->run (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

ImplRepo_i::~ImplRepo_i (void)
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  // Get reactor instance from TAO.
  ACE_Reactor *reactor = orb->orb_core ()->reactor ();

  // Register event handler for the ior multicast.
  if (this->ior_multicast_
      && reactor->remove_handler (this->ior_multicast_,
                                  ACE_Event_Handler::READ_MASK) == -1)
    if (OPTIONS::instance ()->debug () > 0)
      ACE_DEBUG ((LM_DEBUG,
                  "Implementation Repository: cannot remove handler\n"));

  delete this->forwarder_impl_;
  CORBA::release (this->activator_);
  delete this->ior_multicast_;
}


// Returns the startup information for a server

void
ImplRepo_i::find (const char *server,
                  ImplementationRepository::ServerInformation_out info
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::Administration::NotFound))
{
  ACE_TString logical, command_line, working_directory;
  ACE_TString location, server_object_ior;
  ImplementationRepository::EnvironmentList environment_vars;
  ImplementationRepository::ActivationMode activation;

  ACE_NEW_THROW_EX (info,
                    ImplementationRepository::ServerInformation,
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Get the information from the server repository.
  if (this->repository_.get_startup_info (server,
                                          logical,
                                          command_line,
                                          environment_vars,
                                          working_directory,
                                          activation) != 0)
    ACE_THROW (ImplementationRepository::Administration::NotFound ());

  if (this->repository_.get_running_info (server, location, server_object_ior) != 0)
    ACE_THROW (ImplementationRepository::Administration::NotFound ());

  // Fill in <info>.
  info->logical_server = CORBA::string_dup (logical.c_str ());
  info->server = CORBA::string_dup (server);
  info->startup.command_line = CORBA::string_dup (command_line.c_str ());
  info->startup.environment = environment_vars;
  info->startup.working_directory = CORBA::string_dup (working_directory.c_str ());
  info->startup.activation = activation;
  info->location = CORBA::string_dup (location.c_str ());
}


// Used to access the list of servers registered.  May also return an
// iterator which can be used to access more than <how_many> of them.

void
ImplRepo_i::list (CORBA::ULong how_many,
                  ImplementationRepository::ServerInformationList_out server_list,
                  ImplementationRepository::ServerInformationIterator_out server_iterator
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Initialize the out variables, so if we return early, they will
  // not be dangling.
  server_iterator =
    ImplementationRepository::ServerInformationIterator::_nil ();

  ACE_NEW_THROW_EX (server_list,
                    ImplementationRepository::ServerInformationList (0),
                    CORBA::NO_MEMORY ());

  // Get a new iterator
  auto_ptr<Server_Repository::HASH_IMR_ITER> server_iter (this->repository_.new_iterator ());

  // Check for a memory error.
  if (server_iter.get () == 0)
    ACE_THROW (CORBA::NO_MEMORY ());

  // Number of servers that will go into the server_list.
  CORBA::ULong n;

  if (this->repository_.get_repository_size () > how_many)
    n = how_many;
  else
    n = this->repository_.get_repository_size ();

  // Now copy over to the server list.

  server_list->length (n);

  Server_Repository::HASH_IMR_ENTRY *server_entry;

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "list: Filling ServerList with %d servers\n", n));

  for (CORBA::ULong i = 0; i < n; i++)
    {
      server_iter->next (server_entry);
      server_iter->advance ();

      ACE_TString logical, server, command_line, working_directory;
      ACE_TString location, server_ior;

      ImplementationRepository::EnvironmentList environment_vars;
      ImplementationRepository::ActivationMode activation =
        ImplementationRepository::NORMAL;

      server_entry->int_id_->get_running_info (location, server_ior);
      server_entry->int_id_->get_startup_info (logical,
                                               command_line,
                                               environment_vars,
                                               working_directory,
                                               activation);

      server_list[i].logical_server = CORBA::string_dup (logical.c_str ());
      server_list[i].server = CORBA::string_dup (server_entry->ext_id_.c_str ());
      server_list[i].startup.command_line = CORBA::string_dup (command_line.c_str ());
      server_list[i].startup.environment = environment_vars;
      server_list[i].startup.working_directory = CORBA::string_dup (working_directory.c_str ());
      server_list[i].startup.activation = activation;
      server_list[i].location = CORBA::string_dup (location.c_str ());
    }


  // Now work on the iterator

  // Only set up an iterator if we need to
  if (this->repository_.get_repository_size () > how_many)
    {
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "list: Creating ServerInformationIterator\n"));

      // Create an imr_iter and give it the server_iter pointer
      ImR_Iterator *imr_iter;

      ACE_NEW_THROW_EX (imr_iter,
                        ImR_Iterator (server_iter.release (), this->root_poa_.in ()),
                        CORBA::NO_MEMORY ());

      ACE_TRY
        {
          PortableServer::ObjectId_var id =
            this->root_poa_->activate_object (imr_iter ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          server_iterator = imr_iter->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
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
ImplRepo_i::shutdown_server (const char *server
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::Administration::NotFound))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  ACE_TString server_object_ior, location;

  // Find out if it is already running
  if (this->repository_.get_running_info (server, location, server_object_ior) != 0)
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
            orb->string_to_object (server_object_ior.c_str () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ImplementationRepository::ServerObject_var server_object =
            ImplementationRepository::ServerObject::_narrow (object.in ()
                                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (server_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Error: Invalid ServerObject IOR: <%s>\n",
                          server_object_ior.c_str ()));
              ACE_THROW (ImplementationRepository::Administration::NotFound ());
            }

          // Call shutdown
          server_object->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Remove running info from repository
          if (this->repository_.update (server, "", "") != 0)
            {
              ACE_ERROR ((LM_ERROR,
                         "Error: Could not update information for unknown server <%s>\n",
                         server));
              ACE_THROW (ImplementationRepository::Administration::NotFound ());
            }
        }
      ACE_CATCHANY
        {
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<ACE_Hash_Map_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex> >;
template class ACE_Auto_Basic_Ptr<ACE_Hash_Map_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<ACE_Hash_Map_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex> >
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Hash_Map_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
