/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Service_Object.h"
#include "ace/Thread_Manager.h"
#include "ace/streams.h"

#if defined (ACE_WIN32) && (!defined (ACE_HAS_STANDARD_CPP_LIBRARY) || \
                            (ACE_HAS_STANDARD_CPP_LIBRARY == 0))
#  include <stdio.h>
#else
#  include <string>
#endif

#include "SLDEX_export.h"


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


class Server_Shutdown : public ACE_Service_Object {
public:
  virtual int init (int, ACE_TCHAR *[]) {
    reactor_ = ACE_Reactor::instance ();
    return ACE_Thread_Manager::instance ()->spawn
      (controller, reactor_, THR_DETACHED);
  }

  virtual int fini () {
    Quit_Handler *quit_handler = 0;
    ACE_NEW_RETURN (quit_handler,
                    Quit_Handler (reactor_), -1);
    return reactor_->notify (quit_handler);
  }

private:
  ACE_Reactor *reactor_;
};

ACE_FACTORY_DEFINE (SLDEX, Server_Shutdown)
