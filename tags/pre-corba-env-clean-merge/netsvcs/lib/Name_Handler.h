/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Name_Handler.h
//
// = AUTHOR
//    Prashant Jain, Gerhard Lenzer, and Douglas C. Schmidt
//
// ============================================================================

#ifndef ACE_NAME_HANDLER_H
#define ACE_NAME_HANDLER_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "ace/SString.h"
#include "ace/Naming_Context.h"
#include "ace/Name_Request_Reply.h"
#include "ace/Singleton.h"

class Naming_Context : public ACE_Naming_Context
{
  // = TITLE
  //
  //   This helper class adds the correct default constructor to the
  //   <ACE_Naming_Context> class so that we can use it in
  //   <ACE_Singleton>.
public:
  Naming_Context (void)
    : ACE_Naming_Context (ACE_Naming_Context::NET_LOCAL) {}
};

typedef ACE_Singleton<Naming_Context, ACE_SYNCH_NULL_MUTEX> NAMING_CONTEXT;

class ACE_Svc_Export ACE_Name_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //    Product object created by <ACE_Name_Acceptor>.  An
  //    <ACE_Name_Handler> exchanges messages with a <ACE_Name_Proxy>
  //    object on the client-side.
  //
  // = DESCRIPTION
  //   This class is the main workhorse of the <ACE_Name_Server>.  It
  //   handles client requests to bind, rebind, resolve, and unbind
  //   names.  It also schedules and handles timeouts that are used to
  //   support "timed waits."  Clients used timed waits to bound the
  //   amount of time they block trying to get a name.

  friend class ACE_Shutup_GPlusPlus;  // Turn off g++ warning
public:
  typedef int (ACE_Name_Handler::*OPERATION) (void);
  // Pointer to a member function of ACE_Name_Handler returning int

  typedef int (ACE_Naming_Context::*LIST_OP) (ACE_PWSTRING_SET &, const ACE_NS_WString &);
  // Pointer to a member function of ACE_Naming_Context returning int

  typedef ACE_Name_Request (ACE_Name_Handler::*REQUEST) (ACE_NS_WString *);
  // Pointer to a member function of ACE_Name_Handler returning ACE_Name_Request

  // = Initialization and termination.

  ACE_Name_Handler (ACE_Thread_Manager * = 0);
  // Default constructor.

  virtual int open (void * = 0);
  // Activate this instance of the <ACE_Name_Handler> (called by the
  // <ACE_Strategy_Acceptor>).

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

  virtual int send_reply (ACE_INT32 status,
                          ACE_UINT32 errnum = 0);
  // Create and send a reply to the client.

  virtual int send_request (ACE_Name_Request &);
  // Special kind of reply

  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  // Return the underlying <ACE_HANDLE>.

  virtual int handle_input (ACE_HANDLE);
  // Callback method invoked by the <ACE_Reactor> when client events
  // arrive.

  // = Timer hook.
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
  // Enable clients to limit the amount of time they wait for a name.

private:

  OPERATION op_table_[ACE_Name_Request::MAX_ENUM];
  // Table of pointers to member functions

  struct LIST_ENTRY
  {
    LIST_OP operation_;
    // A member function pointer that performs the appropriate
    // operation (e.g., LIST_NAMES, LIST_VALUES, or LIST_TYPES).

    REQUEST request_factory_;
    // A member function pointer that serves as a factory to create a
    // request that is passed back to the client.

    const char *description_;
    // Name of the operation we're dispatching (used for debugging).
  };

  LIST_ENTRY list_table_[ACE_Name_Request::MAX_LIST];
  // This is the table of pointers to functions that we use to
  // simplify the handling of list requests.

  ACE_Name_Request name_request_;
  // Cache request from the client.

  ACE_Name_Request name_request_back_;
  // Special kind of reply for resolve and listnames.

  ACE_Name_Reply name_reply_;
  // Cache reply to the client.

  ACE_INET_Addr addr_;
  // Address of client we are connected with.

  ~ACE_Name_Handler (void);
  // Ensure dynamic allocation...

  int bind (void);
  // Handle binds.

  int rebind (void);
  // Handle rebinds.

  int shared_bind (int rebind);
  // Handle binds and rebinds.

  int resolve (void);
  // Handle find requests.

  int unbind (void);
  // Handle unbind requests.

  int lists (void);
  // Handle LIST_NAMES, LIST_VALUES, and LIST_TYPES requests.

  int lists_entries (void);
  // Handle LIST_NAME_ENTRIES, LIST_VALUE_ENTRIES, and
  // LIST_TYPE_ENTRIES requests.

  ACE_Name_Request name_request (ACE_NS_WString *one_name);
  // Create a name request.

  ACE_Name_Request value_request (ACE_NS_WString *one_name);
  // Create a value request.

  ACE_Name_Request type_request (ACE_NS_WString *one_name);
  // Create a type request.
};

class ACE_Name_Acceptor : public ACE_Strategy_Acceptor<ACE_Name_Handler, ACE_SOCK_ACCEPTOR>
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
  ACE_Schedule_All_Reactive_Strategy<ACE_Name_Handler> scheduling_strategy_;
  // The scheduling strategy is designed for Reactive services.
};

ACE_SVC_FACTORY_DECLARE (ACE_Name_Acceptor)

#endif /* ACE_NAME_HANDLER_H */
