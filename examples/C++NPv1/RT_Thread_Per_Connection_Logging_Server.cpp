/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/FILE_IO.h"
#include "ace/Log_Msg.h"
#include "ace/Sched_Params.h"
#include "ace/Signal.h"
#include "ace/Thread_Manager.h"

#include "RT_Thread_Per_Connection_Logging_Server.h"
#include "Logging_Handler.h"

#include <errno.h>

static void sigterm_handler (int /* signum */) { /* No-op. */ }


int
RT_Thread_Per_Connection_Logging_Server::open (u_short port)
{
  ACE_Sched_Params fifo_sched_params
    (ACE_SCHED_FIFO,
     ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
     ACE_SCOPE_PROCESS);

  if (ACE_OS::sched_params (fifo_sched_params) != 0) {
    if (errno == EPERM || errno == ENOTSUP)
      ACE_DEBUG ((LM_DEBUG,
                  "Warning: user's not superuser, so "
                  "we're running in time-shared class\n"));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n", "ACE_OS::sched_params()"), -1);
  }
  // Initialize the parent classes.
  return Thread_Per_Connection_Logging_Server::open (port);
}


int
RT_Thread_Per_Connection_Logging_Server::handle_data (ACE_SOCK_Stream *client)
{
  int prio =
    ACE_Sched_Params::next_priority
        (ACE_SCHED_FIFO,
         ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
         ACE_SCOPE_THREAD);
  ACE_OS::thr_setprio (prio);
  return Thread_Per_Connection_Logging_Server::handle_data (client);
}


// For simplicity, the Thread_Per_Connection_Logging_Server methods
// are duplicated here.
void *Thread_Per_Connection_Logging_Server::run_svc (void *arg)
{
  Thread_Args *thread_args = ACE_static_cast (Thread_Args *, arg);

  int result =
    thread_args->this_->handle_data (&thread_args->logging_peer_);

  thread_args->logging_peer_.close ();
  delete thread_args;
  return ACE_reinterpret_cast (void *, result);
}

int
Thread_Per_Connection_Logging_Server::handle_connections ()
{
  Thread_Args *thread_args = new Thread_Args (this);

  if (acceptor ().accept (thread_args->logging_peer_) == -1)
    return -1;
  else if (ACE_Thread_Manager::instance ()->spawn (
                   // Pointer to function entry point.
                  Thread_Per_Connection_Logging_Server::run_svc,
                   // <run_svc> parameter.
                  ACE_static_cast (void *, thread_args),
                  THR_DETACHED | THR_NEW_LWP) == -1) return -1;
  return 0;
}

int
Thread_Per_Connection_Logging_Server::handle_data (ACE_SOCK_Stream *client)
{
  ACE_FILE_IO log_file;
  // Client's hostname is logfile name.
  make_log_file (log_file, client);

  // Place the connection into non-blocking model since this
  // thread isn't doing anything except handling this client.
  client->disable (ACE_NONBLOCK);

  Logging_Handler logging_handler (log_file, *client);

  // Keep handling log records until client closes connection
  // or this thread is asked to cancel itself.
  ACE_Thread_Manager *mgr = ACE_Thread_Manager::instance ();
  ACE_thread_t me = ACE_Thread::self ();
  while (!mgr->testcancel (me) &&
         logging_handler.log_record () != -1)
    continue;

  log_file.close ();
  return 0;
}



int main (int argc, char *argv[])
{
  // Register to receive the <SIGTERM> signal.
  ACE_Sig_Action sa (sigterm_handler, SIGTERM);

  RT_Thread_Per_Connection_Logging_Server server;

  if (server.run (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.run()"), 1);

  // Cooperative thread cancellation and barrier synchronization.
  ACE_Thread_Manager::instance ()->cancel_all ();
  return ACE_Thread_Manager::instance ()->wait ();
}
