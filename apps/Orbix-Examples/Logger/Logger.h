/* -*- C++ -*- */
// @(#)Logger.h	1.1	10/18/96


#if !defined (_LOGGER_H)
#define _LOGGER_H

#include "ace/OS.h"
#include "logger.hh"

class Logger
  // = TITLE
  //     Wrapper class that uses CORBA object reference 
  //     as the transport mechanism to simplify implementation.
{
public:
  Logger (char *server, size_t max_message_size);
  // Constructor takes the name of the host where the server
  // is located.  If server == 0, then use the locator service.

  ~Logger (void);
  // Destructor releases the object reference.

  int log (logger::Log_Priority prio, char msg[], int len);
  // Log a <msg> of length <len> with priority <prio>.

  int verbose (void);
  // Report current level of verbosity.

  int verbose (int verbosity);
  // Set the level of verbosity (0 == no verbose, > 0 == verbose).

  int start_timer (void);
  // Activate the timer.

  int stop_timer (profile_logger::Elapsed_Time &et);
  // Deactivate the timer and return the elapsed time.
  
private:
  profile_logger *logref_;
  // CORBA object reference proxy.

  int pid_;
  // Process ID.

  u_long ip_;
  // IP address of self.

  logger::Log_Record log_msg_;
  // Cache certain non-changing values to avoid recomputing them.

  char *server_;
  // Name of server that we are bound to.
};

#endif /* _LOGGER_H */
