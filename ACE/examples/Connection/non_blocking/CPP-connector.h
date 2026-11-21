/* -*- C++ -*- */
#ifndef CPP_CONNECTOR_H
#define CPP_CONNECTOR_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Sig_Adapter.h"

/**
 * Communicates with a server using a state machine to track interaction
 * with the user and server.
 */
template <typename PEER_STREAM>
class Peer_Handler : public ACE_Svc_Handler<PEER_STREAM, ACE_SYNCH>
{
public:
  explicit Peer_Handler (ACE_Reactor *r = nullptr);
  ~Peer_Handler () override = default;
  Peer_Handler (Peer_Handler const &) = delete;
  Peer_Handler (Peer_Handler &&) = delete;
  Peer_Handler &operator= (Peer_Handler const &) = delete;
  Peer_Handler &operator= (Peer_Handler &&) = delete;

  /// Activates the handler after a connection is established.
  int open (void * = nullptr) override;

  /// Handles a failed connection attempt.
  int close (u_long flags = 0) override;

  /// @name Demultiplexing hooks
  /// @{
  int handle_output (ACE_HANDLE) override;
  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask mask) override;
  int handle_signal (int signum,
                     siginfo_t * = nullptr,
                     ucontext_t * = nullptr) override;
  int handle_timeout (ACE_Time_Value const &time,
                      void const *) override;
  /// @}

protected:
  /// @name State handlers
  /// @{
  int uninitialized ();
  int connected ();
  int stdio ();
  /// @}

  /// Tracks the current state handler.
  int (Peer_Handler<PEER_STREAM>::*action_) ();
};

/// Illustrates the ACE_Connector interface.
template <typename SVC_HANDLER, typename PEER_CONNECTOR>
class IPC_Client : public ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>
{
public:
  IPC_Client ();

  ~IPC_Client () override = default;
  IPC_Client (IPC_Client const &) = delete;
  IPC_Client (IPC_Client &&) = delete;
  IPC_Client &operator= (IPC_Client const &) = delete;
  IPC_Client &operator= (IPC_Client &&) = delete;
  /// @name Dynamic linking hooks
  /// @{
  /// Initializes the IPC client.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Destroys the IPC client.
  int fini () override;
  /// @}

  /// Runs the client service.
  virtual int svc ();

private:
  using inherited = ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>;

  /// Options for the active connection factory.
  ACE_Synch_Options options_;

  /// Tracks shutdown after receipt of SIGINT.
  ACE_Sig_Adapter done_handler_;
};

#include "CPP-connector.cpp"

#endif /* CPP_CONNECTOR_H */
