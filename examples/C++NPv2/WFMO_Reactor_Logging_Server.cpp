/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Thread_Manager.h"

#include "Reactor_Logging_Server.h"
#include "Logging_Acceptor_Ex.h"
#include "Logging_Event_Handler_Ex.h"


class Quit_Handler : public ACE_Event_Handler {
private:
  ACE_Manual_Event quit_seen_;

public:
  Quit_Handler (ACE_Reactor *r) : ACE_Event_Handler (r) {
    SetConsoleMode (ACE_STDIN, ENABLE_LINE_INPUT
                               | ENABLE_ECHO_INPUT
                               | ENABLE_PROCESSED_INPUT);
    if (reactor ()->register_handler
          (this, quit_seen_.handle ()) == -1
        || ACE_Event_Handler::register_stdin_handler
             (this, r, ACE_Thread_Manager::instance ()) == -1)
      r->end_reactor_event_loop ();
  }

  ~Quit_Handler () {
    ACE_Event_Handler::remove_stdin_handler
      (reactor (), ACE_Thread_Manager::instance ());
    reactor ()->remove_handler (quit_seen_.handle (),
                                ACE_Event_Handler::DONT_CALL);
  }

  virtual int handle_input (ACE_HANDLE h) {
    CHAR user_input[BUFSIZ];
    DWORD count;
    if (!ReadFile (h, user_input, BUFSIZ, &count, 0))
      return -1;

    user_input[count] = '\0';
    if (ACE_OS_String::strncmp (user_input, "quit", 4) == 0)
      return -1;
    return 0;
  }

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask) {
    quit_seen_.signal ();
    return 0;
  }

  virtual int handle_signal (int, siginfo_t *, ucontext_t *) {
    reactor ()->end_reactor_event_loop ();
    return 0;
  }
};


class Logging_Event_Handler_WFMO : public Logging_Event_Handler_Ex
{
public:
  Logging_Event_Handler_WFMO (ACE_Reactor *r)
    : Logging_Event_Handler_Ex (r) {}

protected:
  int handle_input (ACE_HANDLE h) {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, monitor, lock_, -1);
    return logging_handler_.log_record ();
  }

  ACE_Thread_Mutex lock_; // Serialize threads in thread pool.
};


class Logging_Acceptor_WFMO : public Logging_Acceptor_Ex
{
public:
  // Simple constructor to pass ACE_Reactor to base class.
  Logging_Acceptor_WFMO (ACE_Reactor *r = ACE_Reactor::instance ())
    : Logging_Acceptor_Ex (r) {};

protected:
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


static ACE_THR_FUNC_RETURN event_loop (void *arg) {
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
  ACE_WFMO_Reactor wfmo_reactor;
  ACE_Reactor reactor (&wfmo_reactor);

  Server_Logging_Daemon *server;
  // Ignore argv[0]...
  --argc; ++argv;
  ACE_NEW_RETURN (server,
                  Server_Logging_Daemon (argc, argv, &reactor),
                  1);
  Quit_Handler quit_handler (&reactor);
  ACE_Thread_Manager::instance ()->spawn_n
    (N_THREADS, event_loop, &reactor);
  return ACE_Thread_Manager::instance ()->wait ();
}
