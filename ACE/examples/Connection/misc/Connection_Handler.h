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

  /// Initializes the handler and makes it an active object.
  int open (void *) override;

  /// Terminates the handler.
  int close (u_long) override;

  /// Runs the handler's main event loop.
  int svc () override;

protected:
  /// Signals the active object to stop when called.
  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask) override;

  /// Handles input from the client.
  int handle_input (ACE_HANDLE) override;

  /// Handles timeouts.
  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg) override;

  /// Handles a signal.
  int handle_signal (int signum, siginfo_t *, ucontext_t *) override;

  /// Tracks whether the handler's event loop should finish.
  sig_atomic_t finished_ {};
};

#endif /* ACE_CONNECTION_HANDLER_H */
