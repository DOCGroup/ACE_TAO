/* -*- C++ -*- */

// $Id$

#ifndef ACE_CONNECTION_HANDLER_H
#define ACE_CONNECTION_HANDLER_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"

class Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  virtual int open (void *);
  // Initialize the <Connection_Handler> and make it an Active Object.

  virtual int close (u_long);
  // Terminate the <Connection_Handler>.

  virtual int svc (void);
  // Run the <Connection_Handler>'s main event loop.

protected:
  virtual int handle_close (ACE_HANDLE,
			    ACE_Reactor_Mask);
  // Signal the Active Object to stop when called.

  virtual int handle_input (ACE_HANDLE);
  // Handle input from the client.

  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);
  // Handle timeouts.

  virtual int handle_signal (int signum,
			     siginfo_t *,
			     ucontext_t *);
  // Handle timeouts.

  sig_atomic_t finished_;
  // Keeps track of whether we're done.
};

#endif /* ACE_CONNECTION_HANDLER_H */
