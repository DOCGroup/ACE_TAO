/* -*- C++ -*- */
// @(#)CPP-acceptor.h	1.1	10/18/96

#if !defined (CPP_ACCEPTOR_H)
#define CPP_ACCEPTOR_H

#include "ace/Acceptor.h"

// This is the class that does the work once the ACE_Oneshot_Acceptor
// has accepted a connection.

template <ACE_PEER_STREAM_1>
class Svc_Handler : public ACE_Svc_Handler <ACE_PEER_STREAM_2, ACE_NULL_SYNCH>
{
public:
  // = Initialization method.
  Svc_Handler (ACE_Reactor *r);

  virtual int open (void *);
  // Perform the work of the SVC_HANDLER.
  
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Handle data from the client.

  virtual int close (u_long);
  // Called if ACE_Svc_Handler is closed down unexpectedly.

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg);
  // Handles acceptor timeouts.

private:
  typedef ACE_Svc_Handler <ACE_PEER_STREAM_2, ACE_NULL_SYNCH> SVC_HANDLER;
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class IPC_Server : public ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:
  // = Initialization and termination.
  IPC_Server (void);
  ~IPC_Server (void);

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  // Initialize the network server.

  virtual int fini (void);
  // Close down the server.

  virtual int svc (void);
  // Run the interative service.

private:
  typedef ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> inherited;

  PEER_ADDR server_addr_;
  // Address of this server.

  ACE_Synch_Options options_;
  // Options that this server is using.

  ACE_Sig_Adapter done_handler_;
  // Keeps track of when we shut down due to receipt of the SIGINT
  // signal.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "CPP-acceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#endif /* CPP_ACCEPTOR_H */
