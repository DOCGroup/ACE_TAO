// "$Id$"

#include "ImR_Activator_i.h"

#include "INS_Locator.h"
#include "Iterator.h"
#include "Activator_Options.h"

#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/PortableServer/ImR_LocatorC.h"

#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/ORB_Core.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/default_ports.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_sys_time.h"

ACE_RCSID (ImplRepo_Service,
           ImR_Activator_i,
           "$Id$")


ImR_Activator_i::ImR_Activator_i (void)
  : registration_token_(0)
{
  char host_name[MAXHOSTNAMELEN + 1];
  ACE_OS::hostname (host_name, MAXHOSTNAMELEN);
  name_ = host_name;
}

char *
ImR_Activator_i::find_ior (const char* oname ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  // We assume that the first part of the object name is the poa name.
  // So we would think that a name of foo/bar means that the POA name
  // is foo.
  ACE_CString object_name(oname);
  ssize_t pos = object_name.find ('/');

  if (pos == ACE_CString::npos)
    pos = ACE_static_cast (ssize_t, object_name.length ());

  ACE_CString poa_name;
  poa_name.set (object_name.fast_rep (), pos, 1);

  ACE_CString endpoint;
  ACE_TRY
    {
      // Activate the server and return the object reference.
      endpoint = this->activate_server_with_startup (poa_name.c_str (), 1
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_THROW_RETURN (ImplementationRepository::NotFound (), 0);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  // Have to do this so it is null terminated
  ACE_CString object_name2 (object_name.fast_rep (),
                            object_name.length ());

  ACE_CString ior = endpoint;
  ior += object_name2;

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Found IOR"
      " for %s is %s\n", oname, endpoint.c_str()));

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
  // Since this is called through the Admin interface, we should ignore some
  // of the activation modes.  Also ignore the return value.
  this->activate_server_i (server, 0 ACE_ENV_ARG_PARAMETER);
  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Activate server %s.\n", server));
}

char *
ImR_Activator_i::activate_server_with_startup (const char *server,
                                               int check_startup
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound,
                   ImplementationRepository::CannotActivate))
{
  if (OPTIONS::instance()->debug () >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Activate server"
      " %s. Check startup=%d.\n", server, check_startup));
  }
  return this->activate_server_i (server, check_startup ACE_ENV_ARG_PARAMETER);
}

// Activates the server and returns the IOR of the object that is
// activated.
char *
ImR_Activator_i::activate_server_i(const char *server,
                                               int check_startup
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound,
                   ImplementationRepository::CannotActivate))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  int start = 0;
  ACE_CString server_object_ior, partial_ior;

  ACE_CString logical, startup, working;
  ImplementationRepository::EnvironmentList environment;
  ImplementationRepository::ActivationMode activation;

  // Get the startup information for the server in question.
  if (this->repository_.get_startup_info (server,
                                          logical,
                                          startup,
                                          environment,
                                          working,
                                          activation) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot find startup info for server <%s>\n",
                  server));
      ACE_THROW_RETURN(ImplementationRepository::NotFound (), 0);
    }

  // Find out if it is already running
  if (this->repository_.get_running_info (server,
                                          partial_ior,
                                          server_object_ior) != 0)
    {
      // If we had problems getting the server_object_ior, probably meant that
      // there is no <server> registered
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot find ServerObject IOR for server <%s>\n",
                  server));
      ACE_THROW_RETURN (ImplementationRepository::NotFound (), 0);
    }

  // Check to see if there is one running (if there is a server_object_ior)
  if (server_object_ior.length () != 0)
    {
      // It is running
      ACE_TRY
        {
          CORBA::Object_var object =
            orb->string_to_object (server_object_ior.c_str ()
                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ImplementationRepository::ServerObject_var server_object =
            ImplementationRepository::ServerObject::_narrow (object.in ()
                                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (server_object.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "ImR Activator: Invalid ServerObject IOR: <%s>\n",
                          server_object_ior.c_str ()));
              ACE_THROW_RETURN (ImplementationRepository::NotFound (), 0);
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
  {
    start = 1;
  }

  if (start == 1)
    {
      // Make sure the activation allows us to start it up.
      if (activation == ImplementationRepository::MANUAL && check_startup)
        ACE_THROW_RETURN (CORBA::TRANSIENT (
            CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE,
                                                     0),
            CORBA::COMPLETED_NO),
          0);

      this->start_server_i (server);
      ACE_CHECK_RETURN (0);
    }

  // Get the latest information about where the server is running and
  // it's IOR
  if (this->repository_.get_running_info (server,
                                          partial_ior,
                                          server_object_ior) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Can not resolve server <%s>\n",
                  server));
    }

  if (activation == ImplementationRepository::PER_CLIENT && check_startup)
  {
    if (this->repository_.update (server, "", "") != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "ImR Activator: Could not update information for server <%s>\n",
                   server));
        ACE_THROW_RETURN (ImplementationRepository::NotFound (), 0);
      }
  }

  return CORBA::string_dup (partial_ior.c_str ());
}


// Starts the server process
void
ImR_Activator_i::start_server_i (const char *server ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  int spawned_pid = 0;
  ACE_CString logical, startup, working;
  ImplementationRepository::EnvironmentList environment;
  ImplementationRepository::ActivationMode activation;

  // Get the server startup information.
  if (this->repository_.get_startup_info (server,
                                          logical,
                                          startup,
                                          environment,
                                          working,
                                          activation) != 0)
    {
      // In case of failure, print it out.
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot find startup info for server <%s>\n",
                  server));

      // And, throw the exception.
      ACE_THROW (ImplementationRepository::NotFound ());
      ACE_CHECK;
    }

  // Check to see if it is already starting up
  int startup_val = this->repository_.starting_up (server, 1);

  if (startup_val == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot find startup info for server <%s>\n",
                  server));

      ACE_THROW (ImplementationRepository::NotFound ());
      ACE_CHECK;
    }

  // startup_val will be zero only if the server is running.
  if (startup_val == 0)
    {
      if (startup.length () == 0)
        {
          // If there is no startup information, throw an exception
          ACE_ERROR ((LM_ERROR,
                      "ImR Activator: No startup information for server <%s>\n",
                      server));
          ACE_THROW (ImplementationRepository::CannotActivate
            (CORBA::string_dup ("No startup information")));
          ACE_CHECK;
        }

      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Starting server %s\n", server));

      // Set the Process parameters.
      ACE_Process_Options proc_opts;

      proc_opts.command_line (startup.c_str ());
      proc_opts.working_directory (working.c_str ());
      proc_opts.handle_inheritence (0);

      // Win32 does not support the CLOSE_ON_EXEC semantics for sockets
      // the way unix does, so in order to avoid having the child process
      // hold the listen socket open, we force the child to inherit no
      // handles. This includes stdin, stdout, logs, etc.

      for (CORBA::ULong i = 0; i < environment.length(); ++i)
        proc_opts.setenv (environment[i].name.in (), environment[i].value.in ());

      // Spawn the proces.
      spawned_pid = this->process_mgr_.spawn (proc_opts);

      if (spawned_pid == ACE_INVALID_PID)
        {
          // If invalid PID..
          ACE_ERROR ((LM_ERROR,
                     "ImR Activator: Cannot activate server <%s> using <%s>\n",
                      server,
                      startup.c_str ()));

          // and throw the exception
          ACE_THROW (ImplementationRepository::CannotActivate
            (CORBA::string_dup ("Process Creation Failed")));
          ACE_CHECK;
        }
      else if (OPTIONS::instance ()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::start_server_i: Process ID is %d\n", spawned_pid));
    }

  // Now that the server has been started up, we need to go back into the event
  // loop so we can get the reponse or handle other requests
  TAO_ORB_Core *orb_core = orb->orb_core ();

  int starting_up;

  ACE_Time_Value timeout = OPTIONS::instance ()->startup_timeout ();

  // We will wait till the timeout period for the server to get
  // started up. The starting_up value will be made zero when the
  // server is up and running. And, thats the condition that we are
  // checking.
  while ((starting_up = this->repository_.starting_up (server)) == 1)
    {
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::activate_server: Going into handle_events\n"));

      // Will wait for the specified timeout.
      int result = orb_core->reactor ()->handle_events (&timeout);

      if (result == 0 && timeout == ACE_Time_Value::zero)
        {
          // The timer is out and we are still in the while loop which
          // means that the server is not activated. Set the
          // starting_up_ value to '0' and
          this->repository_.starting_up (server, 0);

          // print the same.
          ACE_ERROR ((LM_ERROR,
                      "ImR Activator: Cannot activate server <%s> using <%s>, "
                      "terminating it.\n",
                      server,
                      startup.c_str ()));

          // Kill the server
          this->process_mgr_.terminate (spawned_pid);

          ACE_THROW(ImplementationRepository::CannotActivate
                    (CORBA::string_dup ("Timeout")));
          ACE_CHECK;
        }
    }

  // If the control comes here, it means that the server got activated.
  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::activate_server: Got out of handle_events loop\n"));

  // Check to see if it disappeared on us
  if (starting_up == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot find startup info for server <%s>\n",
                  server));

      ACE_THROW (ImplementationRepository::NotFound ());
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
          ACE_THROW (ImplementationRepository::CannotActivate
            (CORBA::string_dup ("Timeout while pinging for readiness")));
        }
      else
        {
          ACE_THROW (ImplementationRepository::CannotActivate
            (CORBA::string_dup ("Unknown error")));
        }
    }
}


// ready_check will continuously ping a server and either return when it
// responds to the ping or return -1 if it times out.
int
ImR_Activator_i::ready_check (const char *server)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  ACE_CString ping_object_ior, location;
  ImplementationRepository::ServerObject_var ping_object;

  ACE_DECLARE_NEW_CORBA_ENV;

  // <end> is the end of the window where we can get a response before
  // timing out
  ACE_Time_Value end = ACE_OS::gettimeofday ()
                       + OPTIONS::instance ()->startup_timeout ();

  // Get the ior for the "ping" object for the server
  if (this->repository_.get_running_info (server,
                                          location,
                                          ping_object_ior) != 0)
    {
      // If get_running_info fails, something weird must have happened.
      // Maybe it was removed after we started it up, but before we got here.
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot find ServerObject IOR for server <%s>\n",
                  server));

      ACE_THROW_RETURN (
          ImplementationRepository::NotFound (),
          -2
        );
    }

  // Narrow the ping object

  ACE_TRY_EX (ping1)
    {
      // Get the Object
      CORBA::Object_var object =
        orb->string_to_object (ping_object_ior.c_str () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (ping1);

      // Narrow it down to the correct interface
      ping_object =
        ImplementationRepository::ServerObject::_narrow (object.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (ping1);

      if (CORBA::is_nil (ping_object.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "ImR Activator: Invalid ServerObject IOR: <%s>\n",
                      ping_object_ior.c_str ()));

          ACE_THROW_RETURN (
              ImplementationRepository::NotFound (),
              -2
            );
        }

  }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot activate server <%s>, "
                  "terminating it (Server Ping Object failed).\n",
                  server));

      return -2;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-2);

  // Now ping it until we get a response.
  while (ACE_OS::gettimeofday () < end)
    {
      ACE_TRY_EX (ping2);
        {
          if (OPTIONS::instance()->debug () >= 2)
            ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::ready_check: ping server.\n"));

          ping_object->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (ping2);

          if (OPTIONS::instance()->debug () >= 2)
            ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::ready_check: Pinged Server OK\n"));

          // If we got here, we successfully pinged, therefore we
          // can exit the function.
          return 0;
        }
      ACE_CATCHANY // todo : I doubt we really want to retry on all exceptions. Maybe just transient?
        {
          // Ignore the exception
          if (OPTIONS::instance()->debug () >= 2)
            ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::ready_check: Server not ready (Exception)\n"));
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-2);

      // Sleep between sending pings.
      ACE_OS::sleep (OPTIONS::instance ()->ping_interval ());
    }

  // If we fall out here, that means we didn't get a response before timing
  // out, so return an error.
  return -1;
}


// Adds an entry to the Repository about this <server>

void
ImR_Activator_i::register_server (
    const char *server,
    const ImplementationRepository::StartupOptions &options
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::AlreadyRegistered,
                   ImplementationRepository::NotFound
                   ))
{
  if (OPTIONS::instance ()->readonly ())
    {
      ACE_THROW (CORBA::NO_PERMISSION ());
    }

  if (OPTIONS::instance ()->debug () >= 1)
    ACE_DEBUG((LM_DEBUG, "Imr Activator: Register server %s.\n", server));

  if (OPTIONS::instance ()->debug () >= 2)
    {
      ACE_DEBUG ((LM_DEBUG,
        "ImR_Activator_i::register_server:\nServer: %s\n"
                  "Command Line: %s\n"
                  "Working Directory: %s\n"
                  "Activation Mode: %s\n\n",
                  server,
                  options.command_line.in (),
                  options.working_directory.in (),
                  OPTIONS::instance ()->convert_str (options.activation)));

      for (CORBA::ULong i = 0; i < options.environment.length (); ++i)
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
                  "ImR Activator: Server %s Already Registered!\n",
                  server));
      ACE_THROW (ImplementationRepository::AlreadyRegistered ());
    }
}


// Updates the entry in the Repository about this <server> or adds it
// if necessary.

void
ImR_Activator_i::reregister_server (const char *server,
                                    const ImplementationRepository::StartupOptions &options
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
  ImplementationRepository::AlreadyRegistered,
  ImplementationRepository::NotFound
  ))
{
  if (OPTIONS::instance ()->debug () >= 1)
    ACE_DEBUG((LM_DEBUG, "ImR Activator: Reregister server %s.\n", server));

  if (OPTIONS::instance ()->readonly ())
    {
      ACE_THROW (CORBA::NO_PERMISSION ());
    }

  // Get current starting up value
  int starting_up = this->repository_.starting_up (server);

  // Get ready to save the running info
  ACE_CString location;
  ACE_CString server_object_ior;

  // Only save the running info if it's still running before we remove it.
  ACE_TRY
    {
      // Get the current running information
      ACE_CString ping_location;
      ACE_CString ping_object_ior;
      this->repository_.get_running_info (server,
                                          ping_location,
                                          ping_object_ior);

      // Narrow the server
      CORBA::ORB_var orb = OPTIONS::instance ()->orb ();
      CORBA::Object_var object =
        orb->string_to_object (ping_object_ior.c_str () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ImplementationRepository::ServerObject_var ping_object =
        ImplementationRepository::ServerObject::_narrow (object.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (ping_object.in ()))
        {
          // Ok, we've found our ior, now we remove the server from the
          // repository in order to avoid restarting it if it's not
          // running
          this->repository_.remove (server);

          // Now ping the object
          ping_object->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // If the ping succeeded, then we will save the
          // running information for this server
          location = ping_location;
          server_object_ior = ping_object_ior;
        }
  }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;

  // Remove old info
  this->repository_.remove (server);

  // Add server with new startup parameters
  this->repository_.add (server,
                         "",
                         options.command_line.in (),
                         options.environment,
                         options.working_directory.in (),
                         options.activation);

  // Set the old running info
  if (location.length () != 0)
    this->repository_.update (server, location, server_object_ior);

  // Set old starting up value, if there was one.
  if (starting_up != -1)
    this->repository_.starting_up (server, starting_up);

  if (OPTIONS::instance()->debug () >= 2)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::reregister_server:\nServer: %s\n"
                            "Command Line: %s\n"
                            "Working Directory: %s\n"
                            "Activation: %s\n\n",
                            server,
                            options.command_line.in (),
                            options.working_directory.in (),
                            OPTIONS::instance ()->convert_str (options.activation)));
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
  if (OPTIONS::instance ()->readonly ())
    {
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Activator is readonly. Can't remove server %s.\n",
                  server));
      ACE_THROW (CORBA::NO_PERMISSION ());
    }

  if (this->repository_.remove (server) == 0)
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Removed Server %s.\n", server));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Can't remove unknown server %s.\n",
                  server));
      ACE_THROW (ImplementationRepository::NotFound ());
    }
}

// Register the current location of the server
void
ImR_Activator_i::server_is_running (const char *server,
                                    const char *partial_ior,
                                    ImplementationRepository::ServerObject_ptr server_object
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Server %s is running.\n", server));

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::server_is_running: at %s\n", partial_ior));

  CORBA::String_var server_object_ior =
    orb->object_to_string (server_object ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Update the status of the server in the repository.
  if (this->repository_.update (server, partial_ior, server_object_ior.in ()) == 0)
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG,
          "ImR Activator: Status updated for server %s.\n", server));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                 "ImR Activator: Could not update running information for server <%s>\n",
                 server));
      ACE_THROW(ImplementationRepository::NotFound());
    }

  this->repository_.starting_up (server, 0);
}

// Remove the state information for the current server
void
ImR_Activator_i::server_is_shutting_down (const char *server ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound))
{
  if (this->repository_.update (server, "", "") == 0)
    {
      if (OPTIONS::instance()->debug () >= 1)
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Shut down server %s.\n", server));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                 "ImR Activator: Could not update information for unknown server <%s>\n",
                 server));
      ACE_THROW (ImplementationRepository::NotFound ());
    }
}

int
ImR_Activator_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  ACE_TRY
    {
      CORBA::Object_var obj =
        orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      IORTable::Table_var adapter =
        IORTable::Table::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
          return -1;
        }

      obj = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (obj.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to initialize the ROOT POA.\n"),
                          -1);
      }
      this->root_poa_ = PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a child POA with userId "ImR_Activator"
      this->imr_poa_ =
        this->root_poa_->create_POA ("ImR_Activator",
                                     poa_manager.in (),
                                     policies
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      // If create_POA throws an exception then the process will end, and free all memory.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Get the ObjectId for the string 'ImR_Activator'
      PortableServer::ObjectId_var imr_id =
        PortableServer::string_to_ObjectId ("ImR_Activator");

      // Activate the object
      this->imr_poa_->activate_object_with_id (imr_id.in (),
                                               this
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = this->imr_poa_->id_to_reference (imr_id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ImplementationRepository::Administration_var activator = 
        ImplementationRepository::Administration::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);

      CORBA::String_var ior = orb->object_to_string (activator.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("ImR_Activator", ior.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = orb->resolve_initial_references ("ImplRepoService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (obj.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to get a reference to a Locator.\n"),
                          -1);
      }

      ImplementationRepository::Locator_var locator =
        ImplementationRepository::Locator::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG((LM_DEBUG, "Starting activator : %s\n", this->name_.c_str()));

      ACE_Reactor *reactor = orb->orb_core ()->reactor ();

      if (reactor != 0)
        {
          if (this->process_mgr_.open (ACE_Process_Manager::DEFAULT_SIZE,
                                       reactor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "The ACE_Process_Manager didnt get initialized\n"),
                                 -1);
            }
        }

      // Initialize the persistent storage
      if (this->repository_.init ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Repository failed to initialize\n"),
                            -1);
        }

      
      // We need to send the list of our persisted server names to the Locator
      // so that it knows we are managing them.
      auto_ptr<Server_Repository::HASH_IMR_MAP::ITERATOR>
        server_iter(this->repository_.new_iterator());
      if (server_iter.get () == 0) 
      {
        ACE_THROW_RETURN (CORBA::NO_MEMORY (), -1);
      }
      ImplementationRepository::Locator::ServerNameList server_names;
      server_names.length(this->repository_.get_repository_size());
      Server_Repository::HASH_IMR_MAP::ENTRY* next_entry = 0;
      for (CORBA::ULong idx = 0;server_iter->next(next_entry) != 0; server_iter->advance())
      {
        ACE_CString server_name = next_entry->ext_id_;
        server_names[idx++] = server_name.c_str();
      }

      this->registration_token_ =
        locator->register_activator (this->name_.c_str(), 
                                     activator.in (),
                                     server_names
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The last thing we do is write out the ior so that a test program can assume
      // that the activator is ready to go as soon as the ior is written.
      if (OPTIONS::instance ()->debug () >= 2)
      {
        ACE_DEBUG ((LM_DEBUG,
          "ImR_Activator_i::init: The Activator IOR is: <%s>\n",
          ior.in ()));
      }
      
      ACE_CString filename = OPTIONS::instance()->output_filename();
      if (filename.length() > 0)
      {
        FILE* fp = ACE_OS::fopen(filename.c_str(), "w");
        if (fp != 0) {
          ACE_OS::fprintf(fp, "%s", ior.in());
          ACE_OS::fclose(fp);
        } else {
          ACE_ERROR((LM_ERROR, "ImR Activator: Could not open file %s\n", filename.c_str()));
        }
      }

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
ImR_Activator_i::fini (ACE_ENV_SINGLE_ARG_DECL)
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
ImR_Activator_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  PortableServer::POAManager_var poa_manager =
    this->imr_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  auto_ptr<Server_Repository::HASH_IMR_MAP::ITERATOR>
    server_iter (this->repository_.new_iterator ());

  if (server_iter.get () == 0)
    ACE_THROW_RETURN (CORBA::NO_MEMORY (), -1);

  Server_Repository::HASH_IMR_MAP::ENTRY *server_entry;

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::run: Activating AUTO_START servers\n"));

  // For each of the entries in the Server_Repository, get the startup
  // information and activate the servers, if they are not already
  // running.
  while (!server_iter->done ())
    {
      server_iter->next (server_entry);
      server_iter->advance ();

      ACE_CString logical, server, command_line, working_directory, location;
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
                "ImR_Activator_i::run: AUTO_START Could not activate <%s>\n",
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

ImR_Activator_i::~ImR_Activator_i (void)
{
}


// Returns the startup information for a server

void
ImR_Activator_i::find (const char *server,
                  ImplementationRepository::ServerInformation_out info
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  ACE_CString logical, command_line, working_directory;
  ACE_CString location, server_object_ior;
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
    ACE_THROW (ImplementationRepository::NotFound ());

  // Get the running information which would include the location
  // where the server is running and its IOR.
  if (this->repository_.get_running_info (server,
                                          location,
                                          server_object_ior) != 0)
  {
    ACE_THROW (ImplementationRepository::NotFound ());
  }

  // Fill in <info>.
  info->logical_server = CORBA::string_dup (logical.c_str ());
  info->server = CORBA::string_dup (server);
  info->startup.command_line = CORBA::string_dup (command_line.c_str ());
  info->startup.environment = environment_vars;
  info->startup.working_directory = CORBA::string_dup (working_directory.c_str ());
  info->startup.activation = activation;
  info->startup.activator = this->name_.c_str();
  info->location = CORBA::string_dup (location.c_str ());

  if (OPTIONS::instance()->debug () >= 1)
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
  if (OPTIONS::instance()->debug () >= 1)
      ACE_DEBUG ((LM_DEBUG, "ImR Activator: List servers.\n"));

  // Initialize the out variables, so if we return early, they will
  // not be dangling.
  server_iterator =
    ImplementationRepository::ServerInformationIterator::_nil ();

  ACE_NEW_THROW_EX (server_list,
                    ImplementationRepository::ServerInformationList (0),
                    CORBA::NO_MEMORY ());

  // Get a new iterator
  auto_ptr<Server_Repository::HASH_IMR_MAP::ITERATOR> server_iter (this->repository_.new_iterator ());

  // Check for a memory error.
  if (server_iter.get () == 0)
    ACE_THROW (CORBA::NO_MEMORY ());

  // Number of servers that will go into the server_list.
  CORBA::ULong n;

  if (this->repository_.get_repository_size () > how_many)
    n = how_many;
  else
    n = ACE_static_cast (CORBA::ULong,
                         this->repository_.get_repository_size ());

  // Now copy over to the server list.

  server_list->length (n);

  Server_Repository::HASH_IMR_MAP::ENTRY *server_entry;

  if (OPTIONS::instance()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::list: Filling ServerList with %d servers\n", n));

  for (CORBA::ULong i = 0; i < n; i++)
    {
      server_iter->next (server_entry);
      server_iter->advance ();

      ACE_CString logical, server, command_line, working_directory;
      ACE_CString location, server_ior;

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
      server_list[i].startup.activator = CORBA::string_dup(this->name_.c_str());
      server_list[i].location = CORBA::string_dup (location.c_str ());
    }


  // Now work on the iterator

  // Only set up an iterator if we need to
  if (this->repository_.get_repository_size () > how_many)
    {
      if (OPTIONS::instance()->debug () >= 2)
        ACE_DEBUG ((LM_DEBUG, "ImR_Activator_i::list: Creating ServerInformationIterator\n"));

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
ImR_Activator_i::shutdown_server (const char *server ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  if (OPTIONS::instance()->debug () >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Shutting down server.\n"));

  CORBA::ORB_var orb = OPTIONS::instance ()->orb ();

  ACE_CString server_object_ior, location;

  // Find out if it is already running
  if (this->repository_.get_running_info (server, location, server_object_ior) != 0)
    {
      // If we had problems getting the server_object_ior, probably meant that
      // there is no <server> registered
      ACE_ERROR ((LM_ERROR,
                  "ImR Activator: Cannot find ServerObject IOR for server <%s>\n",
                  server));
      ACE_THROW (ImplementationRepository::NotFound ());
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
                          "ImR Activator: Invalid ServerObject IOR: <%s>\n",
                          server_object_ior.c_str ()));
              ACE_THROW (ImplementationRepository::NotFound ());
            }

          server_object->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // This removes running info from repository
          if (this->repository_.update (server, "", "") != 0)
            {
              ACE_ERROR ((LM_ERROR,
                         "ImR Activator: Could not update information for unknown server <%s>\n",
                         server));
              ACE_THROW (ImplementationRepository::NotFound ());
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
template class auto_ptr<ACE_Hash_Map_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex> >;
template class ACE_Auto_Basic_Ptr<ACE_Hash_Map_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<ACE_Hash_Map_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex> >
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Hash_Map_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
