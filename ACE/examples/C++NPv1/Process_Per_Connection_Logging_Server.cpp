/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/Log_Msg.h"
#include "ace/Process_Manager.h"
#include "ace/Signal.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_fcntl.h"

#include "Process_Per_Connection_Logging_Server.h"
#include "Logging_Handler.h"

#include <errno.h>

namespace {
  extern "C" void sigterm_handler (int /* signum */) { /* No-op. */ }
}


Logging_Process::Logging_Process (const char *prog_name,
                                  const ACE_SOCK_Stream &logging_peer)
    : logging_peer_ (logging_peer.get_handle ())
{
  ACE_OS::strcpy (prog_name_, prog_name);
}

// Set up the process options here. If the decision to do a fork
// a no exec on POSIX needs to be changed, this is the only place
// that needs to change (omit the creation_flags() call).
// We request that the logging client's socket handle be passed
// to the child process. The internals of ACE_Process insure that
// it gets put on the command line if starting a new program image,
// and that if it needed to be duplicated to accomplish that (such
// as on Win32) it will get properly closed.
// The process_name () call sets the program to run and is also used
// for the fork() call on POSIX.
// avoid_zombies has a real affect only on POSIX; it's harmless on Win32.
// Setting the NO_EXEC creation flag is what prevents the exec() on
// POSIX. It has no affect on Win32.
int
Logging_Process::prepare (ACE_Process_Options &options)
{
  if (options.pass_handle (logging_peer_.get_handle ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "pass_handle"), -1);
  options.command_line ("%s", prog_name_);
  options.avoid_zombies (1);
  options.creation_flags (ACE_Process_Options::NO_EXEC);
  return 0;
}

// Just delete the process object. If any handles needed to be
// duplicated to be passed to the child, they'll get closed now
// by the ACE_Process destructor.
void
Logging_Process::unmanage ()
{
  delete this;
}


int
Process_Per_Connection_Logging_Server::handle_connections ()
{
  ACE_SOCK_Stream logging_peer;

  // Block until a client connects.
  if (acceptor ().accept (logging_peer) == -1)
    return -1;

  Logging_Process *logger =
      new Logging_Process (prog_name_, logging_peer);
  ACE_Process_Options options;
  pid_t pid;
  pid = ACE_Process_Manager::instance ()->spawn (logger,
                                                 options);
  // If we came back with pid 0 from the spawn(), this is a
  // POSIX fork system - we are in the child process. Handle the
  // logging records, then exit.
  if (pid == 0) {
    acceptor().close ();
    handle_data (&logging_peer);
    delete logger;
    ACE_OS::exit (0);
  }
  logging_peer.close ();
  if (pid == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn()"), -1);

  // See if there are any child processes that have
  // exited - reap their status and clean up handles held
  // open while the child executed.
  ACE_Process_Manager::instance ()->wait (0,
                                          ACE_Time_Value::zero);
  return 0;
}

int
Process_Per_Connection_Logging_Server::handle_data (ACE_SOCK_Stream *client)
{
  // Disable non-blocking mode.
  client->disable (ACE_NONBLOCK);
  ACE_FILE_IO log_file;
  make_log_file (log_file, client);
  Logging_Handler logging_handler (log_file, *client);

  while (logging_handler.log_record () != -1)
    continue;

  log_file.close ();
  return 0;
}


int
Process_Per_Connection_Logging_Server::run (int argc, char *argv[])
{
  ACE_OS::strncpy (prog_name_, argv[0], MAXPATHLEN);
  prog_name_[MAXPATHLEN] = '\0'; // Ensure NUL-termination.
  // If there are 2 command line arguments after prog_name_, this
  // is a spawned worker process. Else run as the master.
  if (argc == 3)
    return run_worker (argc, argv);  // Only on Win32.
  else
    return run_master (argc, argv);
}

int
Process_Per_Connection_Logging_Server::run_master (int argc, char *argv[])
{
  u_short logger_port = 0;
  if (argc == 2)
    logger_port = ACE_OS::atoi (argv[1]);
  if (this->open (logger_port) == -1)
    return -1;

  for (;;)
    if (handle_connections () == -1)
      return -1;

  ACE_NOTREACHED (return 0;)
}

int
Process_Per_Connection_Logging_Server::run_worker (int, char *argv[])
{
  // The handle value is passed as a hex pointer value on Windows and a
  // decimal number everywhere else. See ace/Process.cpp for info.
#if defined (ACE_WIN32)
  intptr_t client_handle_i = 0;
#  if defined (ACE_WIN64)
  const char *fmt = "%I64x";
#  else
  const char *fmt = "%x";
#  endif /* ACE_WIN64 */
  if (::sscanf_s (argv[2], fmt, &client_handle_i) == 0)
    return -1;
  ACE_HANDLE client_handle =
    reinterpret_cast<ACE_HANDLE> (client_handle_i);
#else
  ACE_HANDLE client_handle = static_cast<ACE_HANDLE> (ACE_OS::atoi (argv[2]));
#endif /* ACE_WIN32 */
  ACE_SOCK_Stream client (client_handle);

  handle_data (&client);
  client.close ();
  return 0;
}


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Register to receive the <SIGTERM> signal.
  ACE_Sig_Action sa ((ACE_SignalHandler)sigterm_handler,
                     SIGTERM);

  Process_Per_Connection_Logging_Server server;

  if (server.run (argc, argv) == -1 && errno != EINTR)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.run()"), 1);

  // Barrier synchronization.
  return ACE_Process_Manager::instance ()->wait ();
}
