/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _LOGGING_EVENT_HANDLER_H
#define _LOGGING_EVENT_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/FILE_IO.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Stream.h"

#include "Logging_Handler.h"

class Logging_Event_Handler : public ACE_Event_Handler
{
protected:
  // File where log records are written.
  ACE_FILE_IO log_file_;

  // Connection to remote peer.
  Logging_Handler logging_handler_;

public:
  // Initialize the base class and logging handler.
  Logging_Event_Handler (ACE_Reactor *reactor)
    : ACE_Event_Handler (reactor),
      logging_handler_ (log_file_) {};

  virtual ~Logging_Event_Handler () {}; // No-op destructor.

  // Activate the object.
  virtual int open ();

  // Called by a reactor when logging events arrive.
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);

  // Called when this object is destroyed, e.g., when it's
  // removed from a reactor.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = 0);

  // Return socket handle of the contained <Logging_Handler>.
  virtual ACE_HANDLE get_handle (void) const {
    // Need a non-const reference to call peer(), but that's
    // safe since we call a const method using it.
    Logging_Handler& h =
      ACE_const_cast (Logging_Handler&, logging_handler_);
    return h.peer ().get_handle ();
  };

  // Get a reference to the contained <ACE_SOCK_Stream>.
  ACE_SOCK_Stream &peer () { return logging_handler_.peer (); };

  // Return a reference to the <log_file_>.
  ACE_FILE_IO &log_file () { return log_file_; };
};

#endif /* _LOGGING_EVENT_HANDLER_H */
