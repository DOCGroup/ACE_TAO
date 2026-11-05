/* -*- C++ -*- */
#ifndef CPP_ACCEPTOR_H
#define CPP_ACCEPTOR_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"
#include "ace/Sig_Adapter.h"

template <typename PEER_STREAM>
class Svc_Handler : public ACE_Svc_Handler <PEER_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //     This class does the work once the <ACE_Oneshot_Acceptor> has
  //     accepted a connection.
public:
  explicit Svc_Handler (ACE_Reactor *r);
  Svc_Handler (Svc_Handler const &) = delete;
  Svc_Handler (Svc_Handler &&) = delete;
  Svc_Handler &operator= (Svc_Handler const &) = delete;
  Svc_Handler &operator= (Svc_Handler &&) = delete;

  int open (void *) override;
  // Perform the work of the SVC_HANDLER.

  int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE) override;
  // Handle data from the client.

  int close (u_long) override;
  // Called if ACE_Svc_Handler is closed down unexpectedly.

  int handle_timeout (ACE_Time_Value const &, void const *arg) override;
  // Handles acceptor timeouts.

private:
  using SVC_HANDLER = ACE_Svc_Handler<PEER_STREAM, ACE_NULL_SYNCH>;
};

template <typename SVC_HANDLER, typename PEER_ACCEPTOR>
class IPC_Server : public ACE_Oneshot_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>
{
  // = TITLE
  //     This class illustrates how the <ACE_Oneshot_Acceptor> works.
public:
  IPC_Server ();
  // Constructor.

  ~IPC_Server () override = default;
  IPC_Server (IPC_Server const &) = delete;
  IPC_Server (IPC_Server &&) = delete;
  IPC_Server &operator= (IPC_Server const &) = delete;
  IPC_Server &operator= (IPC_Server &&) = delete;
  // = Demultiplexing hooks.
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask mask) override;
  // Make sure not to close down the <handle> if we're removed from
  // the <Reactor>.

  // = Dynamic linking hooks.
  int init (int argc, ACE_TCHAR *argv[]) override;
  // Initialize the network server.

  int fini () override;
  // Close down the server.

  virtual int svc ();
  // Run the interative service.

private:
  using inherited = ACE_Oneshot_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>;

  typename PEER_ACCEPTOR::PEER_ADDR server_addr_;
  // Address of this server.

  ACE_Synch_Options options_;
  // Options that this server is using.

  ACE_Sig_Adapter done_handler_;
  // Keeps track of when we shut down due to receipt of the SIGINT
  // signal.
};

#include "CPP-acceptor.cpp"

#endif /* CPP_ACCEPTOR_H */
