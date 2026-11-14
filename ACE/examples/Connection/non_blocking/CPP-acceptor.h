/* -*- C++ -*- */
#ifndef CPP_ACCEPTOR_H
#define CPP_ACCEPTOR_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"
#include "ace/Sig_Adapter.h"

/// Performs service work after the one-shot acceptor accepts a connection.
template <typename PEER_STREAM>
class Svc_Handler : public ACE_Svc_Handler <PEER_STREAM, ACE_NULL_SYNCH>
{
public:
  explicit Svc_Handler (ACE_Reactor *r);
  Svc_Handler (Svc_Handler const &) = delete;
  Svc_Handler (Svc_Handler &&) = delete;
  Svc_Handler &operator= (Svc_Handler const &) = delete;
  Svc_Handler &operator= (Svc_Handler &&) = delete;

  /// Performs the work of the service handler.
  int open (void *) override;

  /// Handles data from the client.
  int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE) override;

  /// Handles an unexpected service-handler shutdown.
  int close (u_long) override;

  /// Handles acceptor timeouts.
  int handle_timeout (ACE_Time_Value const &, void const *arg) override;

private:
  using SVC_HANDLER = ACE_Svc_Handler<PEER_STREAM, ACE_NULL_SYNCH>;
};

/// Illustrates the ACE_Oneshot_Acceptor interface.
template <typename SVC_HANDLER, typename PEER_ACCEPTOR>
class IPC_Server : public ACE_Oneshot_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>
{
public:
  IPC_Server ();

  ~IPC_Server () override = default;
  IPC_Server (IPC_Server const &) = delete;
  IPC_Server (IPC_Server &&) = delete;
  IPC_Server &operator= (IPC_Server const &) = delete;
  IPC_Server &operator= (IPC_Server &&) = delete;
  /// @name Demultiplexing hooks
  /// @{
  /// Keeps the handle open when it is removed from the reactor.
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask mask) override;
  /// @}

  /// @name Dynamic linking hooks
  /// @{
  /// Initializes the network server.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Closes down the server.
  int fini () override;
  /// @}

  /// Runs the interactive service.
  virtual int svc ();

private:
  using inherited = ACE_Oneshot_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>;

  /// Address of this server.
  typename PEER_ACCEPTOR::PEER_ADDR server_addr_;

  /// Options used by this server.
  ACE_Synch_Options options_;

  /// Tracks shutdown after receipt of SIGINT.
  ACE_Sig_Adapter done_handler_;
};

#include "CPP-acceptor.cpp"

#endif /* CPP_ACCEPTOR_H */
