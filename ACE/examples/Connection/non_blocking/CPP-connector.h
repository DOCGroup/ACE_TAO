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

template <typename PEER_STREAM>
class Peer_Handler : public ACE_Svc_Handler<PEER_STREAM, ACE_SYNCH>
{
  // = TITLE
  //   Handles communication with the server.
  //
  // = DESCRIPTION
  //   This class uses a very clever state machine pattern to keep
  //   track of how it interacts with the user and the server.
public:
  explicit Peer_Handler (ACE_Reactor *r = nullptr);

  int open (void * = nullptr) override;
  // Activate the handler when connection is established.

  int close (u_long flags = 0) override;
  // Called on failed connection attempt.

  // = Demultiplexing hooks.
  int handle_output (ACE_HANDLE) override;
  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask mask) override;
  int handle_signal (int signum,
                     siginfo_t * = nullptr,
                     ucontext_t * = nullptr) override;
  int handle_timeout (const ACE_Time_Value &time,
                      const void *) override;
protected:
  // = These methods implement the State pattern.
  int uninitialized ();
  int connected ();
  int stdio ();

  int (Peer_Handler<PEER_STREAM>::*action_) ();
  // Keeps track of which state we are in.
};

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
class IPC_Client : public ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>
{
  // = TITLE
  //   This class illustrates how the <ACE_Connector> works.
public:
  IPC_Client ();
  // Constructor.

  ~IPC_Client ();
  // Destructor.

  // = Dynamic linking hooks.
  int init (int argc, ACE_TCHAR *argv[]) override;
  // Initialize the IPC client.

  int fini () override;
  // Destroy the IPC client.

  virtual int svc ();
  // Run the svc.

private:
  using inherited = ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>;

  ACE_Synch_Options options_;
  // Options for the active connection factory.

  ACE_Sig_Adapter done_handler_;
  // Keeps track of when we shut down due to receipt of the SIGINT
  // signal.
};

#include "CPP-connector.cpp"

#endif /* CPP_CONNECTOR_H */
