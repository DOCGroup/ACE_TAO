/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Client_Logging_Handler.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_CLIENT_LOGGER_H)
#define ACE_CLIENT_LOGGER_H

#include "ace/Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/FIFO_Recv_Msg.h"

class ACE_Svc_Export ACE_Client_Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //    This client logging daemon is a mediator that receives logging
  //    records from local applications processes and forwards them to
  //    the server logging daemon running on another host. 
  //
  // = DESCRIPTION
  //     The default implementation uses an <ACE_FIFO_Recv_Msg> data
  //     member to receive the logging message from the application
  //     and an <ACE_SOCK_Stream> to forward the logging message to
  //     the server.  However, on platforms that don't support FIFOs
  //     (e.g., Win32) we use sockets instead of FIFOs.
{
friend class ACE_Client_Logging_Connector;
public:
  // = Initialization and termination.

  ACE_Client_Logging_Handler (const char rendezvous[] = 0);
  // Default constructor, which initializes the server endpoint.

  virtual int open (void * = 0);
  // Activate this instance of the <ACE_Client_Logging_Handler>
  // (called by the <ACE_Client_Logging_Connector>).

  virtual ACE_HANDLE get_handle (void) const;
  // Return the handle of the message_fifo_;

  virtual int close (u_long);
  // Called when object is removed from the ACE_Reactor.

protected:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle SIGPIPE.

  virtual int handle_input (ACE_HANDLE);
  // Receive logging records from applications.

  virtual int handle_exception (ACE_HANDLE);
  // Receive logging records from applications.  This is necessary to handle
  // madness with UNIX select, which can't deal with MSG_BAND data easily due
  // to its overly simple interface...  This just calls <handle_input>.

  virtual int handle_output (ACE_HANDLE);
  // Called back when it's ok to send.
  
  int send (ACE_Log_Record &log_record);
  // Send the <log_record> to the logging server.

  ACE_FIFO_Recv_Msg msg_queue_;
  // Message queue we use to receive logging records from clients.

  ACE_HANDLE logging_output_;
  // This is either a SOCKET (if we're connected to a logging server)
  // or ACE_STDOUT.

#if defined (ACE_LACKS_FIFO)  
  ACE_SOCK_Acceptor acceptor_;
  // Acceptor used for platforms that don't support FIFOs
#endif /* ACE_LACKS_FIFO */
};

ACE_SVC_FACTORY_DECLARE (ACE_Client_Logging_Connector)

#endif /* ACE_CLIENT_LOGGER_H */
