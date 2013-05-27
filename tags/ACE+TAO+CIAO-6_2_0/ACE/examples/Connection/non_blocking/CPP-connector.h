/* -*- C++ -*- */
// $Id$

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
  Peer_Handler (ACE_Reactor *r = 0);

  virtual int open (void * = 0);
  // Activate the handler when connection is established.

  virtual int close (u_long flags = 0);
  // Called on failed connection attempt.

  // = Demultiplexing hooks.
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask mask);
  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0);
  virtual int handle_timeout (const ACE_Time_Value &time,
                              const void *);
protected:
  // = These methods implement the State pattern.
  int uninitialized (void);
  int connected (void);
  int stdio (void);

  int (Peer_Handler<PEER_STREAM>::*action_) (void);
  // Keeps track of which state we are in.
};

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
class IPC_Client : public ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>
{
  // = TITLE
  //   This class illustrates how the <ACE_Connector> works.
public:
  // = Initialization and termination methods.
  IPC_Client (void);
  // Constructor.

  ~IPC_Client (void);
  // Destructor.

  // = Dynamic linking hooks.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initialize the IPC client.

  virtual int fini (void);
  // Destroy the IPC client.

  virtual int svc (void);
  // Run the svc.

private:
  typedef ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>
          inherited;

  ACE_Synch_Options options_;
  // Options for the active connection factory.

  ACE_Sig_Adapter done_handler_;
  // Keeps track of when we shut down due to receipt of the SIGINT
  // signal.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "CPP-connector.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("CPP-connector.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CPP_CONNECTOR_H */
