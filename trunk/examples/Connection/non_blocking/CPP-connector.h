/* -*- C++ -*- */
// $Id$

#if !defined (CPP_CONNECTOR_H)
#define CPP_CONNECTOR_H

#include "ace/Service_Config.h"
#include "ace/Connector.h"

template <ACE_PEER_STREAM_1>
class Peer_Handler : public ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_SYNCH>
{
public:
  Peer_Handler (ACE_Reactor *r = 0);

  virtual int open (void * = 0);
  // Activate the handler when connection is established.

  // = Demultiplexing hooks.
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask mask);

  virtual ACE_HANDLE get_handle (void) const;

protected:
  // = These methods implement the State pattern.
  int idle (void);
  int connected (void);
  int disconnecting (void);
  int stdio (void);

  int (Peer_Handler<ACE_PEER_STREAM_2>::*action_) (void);
  // Keeps track of which state we are in.
};

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
class IPC_Client : public ACE_Connector<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
{
public:
  // = Initialization and termination methods.
  IPC_Client (void);
  ~IPC_Client (void);

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  // Initialize the IPC client.

  virtual int fini (void);
  // Destroy the IPC client.

  virtual int svc (void);
  // Run the svc.
  
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Report connection errors.

private:
  typedef ACE_Connector<SVC_HANDLER, ACE_PEER_CONNECTOR_2> inherited;

  ACE_Synch_Options options_;
  // Options for the active connection factory.

  ACE_Sig_Adapter done_handler_;
  // Keeps track of when we shut down due to receipt of the SIGINT
  // signal.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "CPP-connector.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#endif /* CPP_CONNECTOR_H */
