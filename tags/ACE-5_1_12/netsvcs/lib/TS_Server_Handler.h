/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    TS_Server_Handler.h
//
// = AUTHOR
//    Prashant Jain
//
// ============================================================================

#ifndef ACE_TS_SERVER_HANDLER_H
#define ACE_TS_SERVER_HANDLER_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "ace/Time_Request_Reply.h"

class ACE_Svc_Export ACE_TS_Server_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //    Product object created by <ACE_TS_Server_Acceptor>.
  //
  // = DESCRIPTION

  friend class ACE_Shutup_GPlusPlus;  // Turn off g++ warning
public:
  // = Initialization and termination.

  ACE_TS_Server_Handler (ACE_Thread_Manager * = 0);
  // Default constructor.

  virtual int open (void * = 0);
  // Activate this instance of the <ACE_TS_Server_Handler> (called by the
  // <ACE_Strategy_Acceptor>).

  ~ACE_TS_Server_Handler (void);
  // Must be allocated dynamically.

protected:
  // = Helper routines for the operations exported to clients.

  virtual int abandon (void);
  // Give up waiting (e.g., when a timeout occurs or a client shuts
  // down unexpectedly).

  // = Low level routines for framing requests, dispatching
  // operations, and returning replies.

  virtual int recv_request (void);
  // Receive, frame, and decode the client's request.

  virtual int dispatch (void);
  // Dispatch the appropriate operation to handle the client's
  // request.

  virtual int send_request (ACE_Time_Request &);
  // Special kind of reply

  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  // Return the underlying <ACE_HANDLE>.

  virtual int handle_input (ACE_HANDLE);
  // Callback method invoked by the <ACE_Reactor> when client events
  // arrive.

  // = Timer hook.
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
  // Enable clients to limit the amount of time they wait.

private:
  ACE_Time_Request time_request_;
  // Cache request from the client.

  ACE_INET_Addr addr_;
  // Address of client we are connected with.
};

class ACE_TS_Server_Acceptor : public ACE_Strategy_Acceptor<ACE_TS_Server_Handler, ACE_SOCK_ACCEPTOR>
{
  // = TITLE
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Strategy_Acceptor>.

public:
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

private:
  ACE_Schedule_All_Reactive_Strategy<ACE_TS_Server_Handler> scheduling_strategy_;
  // The scheduling strategy is designed for Reactive services.
};

ACE_SVC_FACTORY_DECLARE (ACE_TS_Server_Acceptor)

#endif /* ACE_TS_SERVER_HANDLER_H */
