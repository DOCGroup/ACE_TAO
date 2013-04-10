/* -*- C++ -*- */
// $Id$

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
  typedef ACE_Svc_Handler <PEER_STREAM, ACE_NULL_SYNCH>
          SVC_HANDLER;
};

template <typename SVC_HANDLER, typename PEER_ACCEPTOR>
class IPC_Server : public ACE_Oneshot_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>
{
  // = TITLE
  //     This class illustrates how the <ACE_Oneshot_Acceptor> works.
public:
  // = Initialization and termination.
  IPC_Server (void);
  // Constructor.

  ~IPC_Server (void);
  // Destructor.

  // = Demultiplexing hooks.
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask);
  // Make sure not to close down the <handle> if we're removed from
  // the <Reactor>.

  // = Dynamic linking hooks.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initialize the network server.

  virtual int fini (void);
  // Close down the server.

  virtual int svc (void);
  // Run the interative service.

private:
  typedef ACE_Oneshot_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>
          inherited;

  typename PEER_ACCEPTOR::PEER_ADDR server_addr_;
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

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("CPP-acceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CPP_ACCEPTOR_H */
