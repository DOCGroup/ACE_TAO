/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#ifndef _THREAD_PER_CONNECTION_LOGGING_SERVER_H
#define _THREAD_PER_CONNECTION_LOGGING_SERVER_H

#include "ace/SOCK_Stream.h"
#include "Logging_Server.h"

class Thread_Per_Connection_Logging_Server : public Logging_Server
{
private:
  struct Thread_Args {
    Thread_Args (Thread_Per_Connection_Logging_Server *lsp) : this_ (lsp) {}

    Thread_Per_Connection_Logging_Server *this_;
    ACE_SOCK_Stream logging_peer_;
  };

  // Passed as a parameter to <ACE_Thread_Manager::spawn>.
  static void *run_svc (void *arg);

protected:
  virtual int handle_connections ();
  virtual int handle_data (ACE_SOCK_Stream * = 0);

public:
  // Template Method that runs logging server's event loop. Need to
  // reimplement this here because the threads spawned from handle_connections
  // call handle_data; therefore, this method must not.
  virtual int run (int argc, char *argv[]) {
    if (open (argc > 1 ? atoi (argv[1]) : 0) == -1)
      return -1;

    for (;;) {
      if (wait_for_multiple_events () == -1)
        return -1;
      if (handle_connections () == -1)
        return -1;
    }

    return 0;
  }
};

#endif /* _THREAD_PER_CONNECTION_LOGGING_SERVER_H */
