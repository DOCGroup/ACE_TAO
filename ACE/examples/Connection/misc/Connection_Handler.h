/* -*- C++ -*- */

#ifndef ACE_CONNECTION_HANDLER_H
#define ACE_CONNECTION_HANDLER_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

class Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  Connection_Handler () = default;
  ~Connection_Handler () override = default;
  Connection_Handler (Connection_Handler const &) = delete;
  Connection_Handler (Connection_Handler &&) = delete;
  Connection_Handler &operator= (Connection_Handler const &) = delete;
  Connection_Handler &operator= (Connection_Handler &&) = delete;

  // Initialize the <Connection_Handler> and make it an Active Object.
  int open (void *) override;

  // Terminate the <Connection_Handler>.
  int close (u_long) override;

  // Run the <Connection_Handler>'s main event loop.
  int svc () override;

protected:
  // Signal the Active Object to stop when called.
  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask) override;

  // Handle input from the client.
  int handle_input (ACE_HANDLE) override;

  // Handle timeouts.
  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg) override;

  // Handle signal.
  int handle_signal (int signum, siginfo_t *, ucontext_t *) override;

  // Keeps track of whether we're done.
  sig_atomic_t finished_ {};
};

#endif /* ACE_CONNECTION_HANDLER_H */
