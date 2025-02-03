/* -*- C++ -*- */

#ifndef ACE_CONNECTION_HANDLER_H
#define ACE_CONNECTION_HANDLER_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

class Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  // Initialize the <Connection_Handler> and make it an Active Object.
  virtual int open (void *);

  // Terminate the <Connection_Handler>.
  virtual int close (u_long);

  // Run the <Connection_Handler>'s main event loop.
  virtual int svc ();

protected:
  // Signal the Active Object to stop when called.
  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask);

  // Handle input from the client.
  virtual int handle_input (ACE_HANDLE);

  // Handle timeouts.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  // Handle signal.
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  // Keeps track of whether we're done.
  sig_atomic_t finished_;
};

#endif /* ACE_CONNECTION_HANDLER_H */
