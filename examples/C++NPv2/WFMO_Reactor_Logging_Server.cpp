/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Thread_Manager.h"

#include "Reactor_Logging_Server.h"
#include "Logging_Acceptor_Ex.h"
#include "Logging_Event_Handler_Ex.h"


template <class ACCEPTOR>
Reactor_Logging_Server<ACCEPTOR>::Reactor_Logging_Server
  (int argc, char *argv[], ACE_Reactor *reactor)
  : ACCEPTOR (reactor) {
  u_short logger_port = argc > 1 ? atoi (argv[1]) : 0;
  ACE_TYPENAME ACCEPTOR::PEER_ADDR server_addr;
  int result;

  if (logger_port != 0)
    result = server_addr.set (logger_port, INADDR_ANY);
  else
    result = server_addr.set ("ace_logger", INADDR_ANY);
  if (result != -1)
    result = ACCEPTOR::open (server_addr);
  if (result == -1) reactor->end_reactor_event_loop ();
}


class Quit_Handler : public ACE_Event_Handler {
public:
  Quit_Handler (ACE_Reactor *r)
    : ACE_Event_Handler (r) {
    reactor ()->register_handler (this, ACE_STDIN);
    SetConsoleMode (ACE_STDIN, ENABLE_LINE_INPUT
                               | ENABLE_ECHO_INPUT
                               | ENABLE_PROCESSED_INPUT);
  }

  ~Quit_Handler () {
    reactor ()->remove_handler (ACE_STDIN,
                                ACE_Event_Handler::DONT_CALL);
  }

  virtual int handle_signal (int, siginfo_t *info, ucontext_t *) {
    CHAR user_input[BUFSIZ];
    DWORD count;
    if (ReadFile (info->si_handle_, user_input,
                  BUFSIZ, &count, 0)) {
      user_input[count] = '\0';
      if (ACE_OS::strncmp (user_input, "quit", 4) == 0)
        reactor ()->end_reactor_event_loop ();
    }
    return 0;
  }
};


class Logging_Event_Handler_WFMO : public Logging_Event_Handler_Ex
{
public:
  Logging_Event_Handler_WFMO (ACE_Reactor *r)
    : Logging_Event_Handler_Ex (r) {}

  int handle_input (ACE_HANDLE h) {
    reactor ()->suspend_handler (h);
    int status = logging_handler_.log_record ();
    reactor ()->resume_handler (h);
    return status;
  }
};


class Logging_Acceptor_WFMO : public Logging_Acceptor_Ex
{
public:
  // Simple constructor to pass ACE_Reactor to base class.
  Logging_Acceptor_WFMO (ACE_Reactor *r = ACE_Reactor::instance ())
    : Logging_Acceptor_Ex (r) {};

  virtual int handle_input (ACE_HANDLE) {
    Logging_Event_Handler_WFMO *peer_handler = 0;
    ACE_NEW_RETURN (peer_handler,
                    Logging_Event_Handler_WFMO (reactor ()),
                    -1);

    if (acceptor_.accept (peer_handler->peer ()) == -1) {
      delete peer_handler;
      return -1;
    } else if (peer_handler->open () == -1) {
      peer_handler->handle_close ();
      return -1;
    }
    return 0;
  }
};


static void *event_loop (void *arg) {
  ACE_Reactor *reactor = ACE_static_cast (ACE_Reactor *, arg);

  reactor->owner (ACE_OS::thr_self ());
  reactor->run_reactor_event_loop ();
  return 0;
}


typedef Reactor_Logging_Server<Logging_Acceptor_WFMO>
        Server_Logging_Daemon;

int main (int argc, char *argv[])
{
  const size_t N_THREADS = 4;

  size_t n_threads = argc > 1 ? atoi (argv[1]) : N_THREADS;

  ACE_WFMO_Reactor wfmo_reactor;
  ACE_Reactor reactor (&wfmo_reactor);

  Server_Logging_Daemon *server;
  ACE_NEW_RETURN (server,
                  Server_Logging_Daemon (argc, argv, &reactor),
                  1);
  Quit_Handler quit_handler (&reactor);
  ACE_Thread_Manager::instance ()->spawn_n
    (n_threads, event_loop, &reactor);
  return ACE_Thread_Manager::instance ()->wait ();
}
