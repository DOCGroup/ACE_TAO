/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/streams.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_WIN32) && (!defined (ACE_HAS_STANDARD_CPP_LIBRARY) || \
                            (ACE_HAS_STANDARD_CPP_LIBRARY == 0))
#  include <stdio.h>
#else
#  include <string>
#endif

#include "Reactor_Logging_Server.h"
#include "Logging_Acceptor_Ex.h"

typedef Reactor_Logging_Server<Logging_Acceptor_Ex>
        Server_Logging_Daemon;


class Quit_Handler : public ACE_Event_Handler {
  friend class ace_dewarn_gplusplus;
public:
  Quit_Handler (ACE_Reactor *r) : ACE_Event_Handler (r) {}

  virtual int handle_exception (ACE_HANDLE) {
    reactor ()->end_reactor_event_loop ();
    return -1; // Trigger call to handle_close() method.
  }

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask)
  { delete this; return 0; }

private:

  // Private destructor ensures dynamic allocation.
  virtual ~Quit_Handler () {}
};


static ACE_THR_FUNC_RETURN event_loop (void *arg) {
  ACE_Reactor *reactor = ACE_static_cast (ACE_Reactor *, arg);

  reactor->owner (ACE_OS::thr_self ());
  reactor->run_reactor_event_loop ();
  return 0;
}


static ACE_THR_FUNC_RETURN controller (void *arg) {
  ACE_Reactor *reactor = ACE_static_cast (ACE_Reactor *, arg);

  Quit_Handler *quit_handler = 0;
  ACE_NEW_RETURN (quit_handler, Quit_Handler (reactor), 0);

#if defined (ACE_WIN32) && (!defined (ACE_HAS_STANDARD_CPP_LIBRARY) || \
                            (ACE_HAS_STANDARD_CPP_LIBRARY == 0))
  for (;;) {
    char user_input[80];
    gets (user_input);
    if (ACE_OS_String::strcmp (user_input, "quit") == 0) {
      reactor->notify (quit_handler);
      break;
    }
  }
#else
  for (;;) {
    std::string user_input;
    std::getline (cin, user_input, '\n');
    if (user_input == "quit") {
      reactor->notify (quit_handler);
      break;
    }
  }
#endif

  return 0;
}


int main (int argc, char *argv[])
{
  ACE_Select_Reactor select_reactor;
  ACE_Reactor reactor (&select_reactor);

  Server_Logging_Daemon *server;
  // Ignore argv[0]...
  --argc; ++argv;
  ACE_NEW_RETURN (server,
                  Server_Logging_Daemon (argc, argv, &reactor),
                  1);
  ACE_Thread_Manager::instance ()->spawn (event_loop, &reactor);
  ACE_Thread_Manager::instance ()->spawn (controller, &reactor);
  return ACE_Thread_Manager::instance ()->wait ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Reactor_Logging_Server<Logging_Acceptor_Ex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Reactor_Logging_Server<Logging_Acceptor_Ex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
