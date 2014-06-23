/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _LOGGING_EVENT_HANDLER_EX_H
#define _LOGGING_EVENT_HANDLER_EX_H

#include "ace/Reactor.h"
#include "ace/Time_Value.h"
#include "ace/Recursive_Thread_Mutex.h"

#include "Logging_Event_Handler.h"

class Logging_Event_Handler_Ex : public Logging_Event_Handler
{
private:
  // Time when a client last sent a log record.
  ACE_Time_Value time_of_last_log_record_;

  // Maximum time to wait for a client log record.
  const ACE_Time_Value max_client_timeout_;

public:
  typedef Logging_Event_Handler PARENT;

  // 3600 seconds == one hour.
  enum { MAX_CLIENT_TIMEOUT = 3600 };

  Logging_Event_Handler_Ex
    (ACE_Reactor *reactor,
     const ACE_Time_Value &max_client_timeout
       = ACE_Time_Value (MAX_CLIENT_TIMEOUT))
    : Logging_Event_Handler (reactor),
      time_of_last_log_record_ (0),
      max_client_timeout_ (max_client_timeout) {}

  virtual ~Logging_Event_Handler_Ex () {}

  virtual int open (); // Activate the event handler.

  // Called by a reactor when logging events arrive.
  virtual int handle_input (ACE_HANDLE);

  // Called when a timeout expires to check if the client has
  // been idle for an excessive amount of time.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *act);

  // Called when this object is destroyed, e.g., when it's
  // removed from a reactor.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = 0);
};

#endif /* _LOGGING_EVENT_HANDLER_EX_H */
